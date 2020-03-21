#ifndef RIALIZER_RMETACONTAINER_H
#define RIALIZER_RMETACONTAINER_H

#include <metatypesystem/rmetatype.h>

#include <vector>

namespace Rializer {

// todo: rename Qt
// would be nice to move to qnamespace.h, but depends on std::vector
namespace Rializer {
namespace Container {

enum struct Dimensionality
{
    Single,
    Double
};

enum struct Structure
{
    Sequence,
    Associative
};

template<typename T>
using ElementsView = std::vector<T /*element*/>;

template<typename T>
using ElementPairsView = std::vector<std::pair<T /*key*/, T /*value*/>>;

} // namespace Container
} // namespace Rializer

class RMetaContainer;

// todo: rename QtPrivate
namespace RializerPrivate {
namespace MetaContainer {

using Rializer::Container::Dimensionality;
using Rializer::Container::Structure;

using Rializer::Container::ElementsView;
using Rializer::Container::ElementPairsView;

// todo (review): rename?
// is it stylistically allowed to define RMetaContainerPrivate
// in a public header?
// todo (review): wish this could be declared after RMetaContainer
struct Data
{
    using MetaType = RMetaType ();

    using Elements = ElementsView<const void *> (const void *container);
    using SetElements = void (
        void *container, const ElementsView<void *> &&elements
    );
    using SetElementsUnique = void (
        void *container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    );

    using ElementPairs = ElementPairsView<const void *> (const void *container);
    using SetElementPairs = void (
        void *container, const ElementPairsView<void *> &&elementPairs
    );
    using SetElementPairsUnique = void (
        void *container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    );

    Dimensionality dimensionality;

    Structure structure;

    bool isMultiple;

    MetaType *elementMetaType;

    MetaType *elementKeyMetaType;
    MetaType *elementValueMetaType;

    Elements *elements;
    SetElements *setElements;
    SetElementsUnique *setElementsUnique;

    ElementPairs *elementPairs;
    SetElementPairs *setElementPairs;
    SetElementPairsUnique *setElementPairsUnique;

    Data(
        Dimensionality dimensionality,

        Structure structure,

        bool isMultiple,

        MetaType *elementMetaType,

        MetaType *elementKeyMetaType,
        MetaType *elementValueMetaType,

        Elements *elements,
        SetElements *setElements,
        SetElementsUnique *setElementsUnique,

        ElementPairs *elementPairs,
        SetElementPairs *setElementPairs,
        SetElementPairsUnique *setElementPairsUnique
    );

    static RMetaContainer create(Data &&data);
};

} // namespace MetaContainer
} // namespace RializerPrivate

// todo: rename QMetaContainer
class RMetaContainer
{
    // todo: support std::array (isMutable)? pair, tuple? tree, graph?
public:
    using Dimensionality = Rializer::Container::Dimensionality;
    using Structure = Rializer::Container::Structure;

    template<typename T>
    using ElementsView = Rializer::Container::ElementsView<T>;
    template<typename T>
    using ElementPairsView = Rializer::Container::ElementPairsView<T>;

    // todo (review): should mark both declarations and definitions inline?
    Dimensionality dimensionality() const;
    bool isSingleDimensional() const;
    bool isDoubleDimensional() const;

    Structure structure() const;
    bool isSequence() const;
    bool isAssociative() const;

    bool isMultiple() const;

    RMetaType elementMetaType() const;

    RMetaType elementKeyMetaType() const;
    RMetaType elementValueMetaType() const;

    // todo (review): void pointer?
    // maybe QAny won't force a copy as QVariant does?
    ElementsView<const void *> elements(const void *container) const;
    // todo (review): is it nice to explicitly require move semantics?
    // the reference is const, because a view itself isn't moved,
    // but elements are.
    // a pointer in a view can be used to free memory after moving.
    // the clang static analyzer gives warnings about passing the result
    // of std::move as a const reference.
    void setElements(
        void *container, const ElementsView<void *> &&elements
    ) const;
    void setElements(
        void *container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    ) const;

    ElementPairsView<const void *> elementPairs(const void *container) const;
    void setElementPairs(
        void *container, const ElementPairsView<void *> &&elementPairs
    ) const;
    void setElementPairs(
        void *container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    ) const;

    // todo (review): is there a use case for isContiguous, isUnordered?
    // isEmpty? clear? meta type can default construct

public:
    // todo: move to QMetaType, QMetaType::typeFlags
    // todo: make these compile time, static assert
    template<class T>
    static bool isContainer();
    static bool isContainer(int typeId);
    // todo (review): should this be a value or a pointer to a static instance?
    template<class T>
    static const RMetaContainer * metaContainer();
    static const RMetaContainer * metaContainer(int typeId);

    // todo: make it automatic, remove
    template<class T>
    static bool registerMetaContainer();

    // todo (review): add a default constructor (alike QMetaObject) and
    // isNull/isValid method (alike QMetaType, QMetaProperty and QMetaMethod)?

protected:
    // The container type of the registered meta container is erased,
    // and its traits are dynamically stored.

    using Data = RializerPrivate::MetaContainer::Data;

    // This stores the traits of the container class.
    Data data;

    explicit RMetaContainer(Data &&data);

    friend Data;
};

// These are imlpementation requirements of the meta container
// for a container class.
// todo: rename QContainerTraits
namespace RContainerTraits {

// Container traits require descriptions,
// namely, properties and element type aliases,
// and methods implementations.

// Each trait is a separate template class,
// so that it can be partially specialized
// for a specific template container class.

// Due to the fact that members of partial specializations are
// unrelated to the members of the primary template,
// to avoid possible repetition,
// a generalized implementation is decoupled
// so that it may be reused.
// The generalized implementation itself may be switched
// for the template container class using specialization.

// To summarize up, there are two scenarios of enabling support
// for a custom container class.
// If the container class is alike the standard ones,
// but a few changes required
// (some method has another name or perfomance can be improved),
// then one should specialize a corresponding trait.
// But in case there are several container classes from another library,
// and one wants to implement the meta container for them in a generic way,
// then switching the generalized implementation is the way to go.

// There is the second template parameter that serves two purposes.
// Firstly, it enables template metaprogramming.
// Secondly, it allows to give an example declaration.
// The example declaration is a primary template that acts as a contract.
// It does not conflict with the declaration that gets instantiated,
// because the last is a specialization for that template argument.

// todo (C++20): Constraints and concepts
// todo (C++17): if constexpr
// todo (C++20): Ranges

namespace Traits = RContainerTraits;

namespace Container = Rializer::Container;

using Container::ElementsView;
using Container::ElementPairsView;

// These are template metaprogramming tags.
struct SingleDimensional : std::integral_constant<
    Container::Dimensionality, Container::Dimensionality::Single
> {};
struct DoubleDimensional : std::integral_constant<
    Container::Dimensionality, Container::Dimensionality::Double
> {};
template<class T, class = void>
struct Dimensionality : SingleDimensional {};
template<class T>
using IsSingleDimensional = std::is_same<
    typename Dimensionality<T>::type, SingleDimensional::type
>;
template<class T>
using IsDoubleDimensional = std::is_same<
    typename Dimensionality<T>::type, DoubleDimensional::type
>;

struct Sequence : std::integral_constant<
    Container::Structure, Container::Structure::Sequence
> {};
struct Associative : std::integral_constant<
    Container::Structure, Container::Structure::Associative
> {};
template<class T, class = void>
struct Structure : Sequence {};
template<class T>
using IsSequence = std::is_same<
    typename Structure<T>::type, Sequence::type
>;
template<class T>
using IsAssociative = std::is_same<
    typename Structure<T>::type, Associative::type
>;

template<class T, class = void>
struct IsMultiple : std::false_type {};

template<class T, class = void>
struct IsReservable : std::false_type {};

// These contain type aliases.
template<class T, class = void>
struct ElementType { struct type; };

template<class T, class = void>
struct ElementKeyType { struct type; };
template<class T, class = void>
struct ElementValueType { struct type; };

template<class T, class = void>
struct SizeType { struct type; };

// These are methods.
template<class T, class = void>
struct Elements
{
    static void elements(
        const T &container, ElementsView<const void *> *elements
    );
};
template<class T, class = void>
struct SetElements
{
    static void setElements(
        T &container, const ElementsView<void *> &&elements
    );
};
template<class T, class = void>
struct SetElementsUnique
{
    static void setElements(
        T &container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    );
};

template<class T, class = void>
struct ElementPairs
{
    static void elementPairs(
        const T &container, ElementPairsView<const void *> *elements
    );
};
template<class T, class = void>
struct SetElementPairs
{
    static void setElementPairs(
        T &container, const ElementPairsView<void *> &&elementPairs
    );
};
template<class T, class = void>
struct SetElementPairsUnique
{
    static void setElementPairs(
        T &container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    );
};

template<class T, class = void>
struct Size
{
    using SizeType = typename SizeType<T>::type;

    static SizeType size(const T &container);
};
template<class T, class = void>
struct Reserve
{
    using SizeType = typename SizeType<T>::type;

    static void reserve(T &container, SizeType size);
};
template<class T, class = void>
struct Clear
{
    static void clear(T &container);
};

// This is a generalized implementation switch.
// todo (review): rename? Implementation?
template<class T, class = void>
struct Library { struct type; };

// This is a generalized implementation
// as per the SequenceContainer and AssociativeContainer standard requirements.
struct Std;

// This is a generalized implementation for the Qt container classes.
struct Qt;

} // namespace RContainerTraits

} // namespace Rializer

Q_DECLARE_METATYPE(const Rializer::RMetaContainer *)

namespace Rializer {

namespace RializerPrivate {
namespace MetaContainer {

// todo: remove, store a meta container instance somehow else
// Currently, a meta container instance is obtained
// through the QMetaType::registerConverter mechanism
// of the meta type system.
// The approach was borrowed from QSequentialIterable.
// But I hope this class will get adopted by Qt,
// and the data of the container class will receive some static storage
// alike QMetaType, QMetaEnum or QMetaProperty,
// and will be automatically enabled for every instantiation
// of the supported container class
// that is registered with qRegisterMetaType.
template<class T>
const RMetaContainer * convert(const T &);

template<class T>
Data create();

template<class T>
void check();

} // namespace MetaContainer
} // namespace RializerPrivate

inline auto RMetaContainer::dimensionality() const -> Dimensionality
{
    return data.dimensionality;
}

inline bool RMetaContainer::isSingleDimensional() const
{
    return data.dimensionality == Dimensionality::Single;
}

inline bool RMetaContainer::isDoubleDimensional() const
{
    return data.dimensionality == Dimensionality::Double;
}

inline auto RMetaContainer::structure() const -> Structure
{
    return data.structure;
}

inline bool RMetaContainer::isSequence() const
{
    return data.structure == Structure::Sequence;
}

inline bool RMetaContainer::isAssociative() const
{
    return data.structure == Structure::Associative;
}

inline bool RMetaContainer::isMultiple() const
{
    return data.isMultiple;
}

inline RMetaType RMetaContainer::elementMetaType() const
{
    return data.elementMetaType();
}

inline RMetaType RMetaContainer::elementKeyMetaType() const
{
    return data.elementKeyMetaType();
}

inline RMetaType RMetaContainer::elementValueMetaType() const
{
    return data.elementValueMetaType();
}

inline auto RMetaContainer::elements(const void * const container) const
 -> ElementsView<const void *>
{
    return data.elements(container);
}

inline void RMetaContainer::setElements(
    void * const container, const ElementsView<void *> &&elements
) const
{
    data.setElements(container, std::move(elements));
}

inline void RMetaContainer::setElements(
    void * const container, const ElementsView<void *> &&elements,
    bool * const elementsUnique
) const
{
    data.setElementsUnique(
        container, std::move(elements),
        elementsUnique
    );
}

inline auto RMetaContainer::elementPairs(const void * const container) const
 -> ElementPairsView<const void *>
{
    return data.elementPairs(container);
}

inline void RMetaContainer::setElementPairs(
    void * const container, const ElementPairsView<void *> &&elementPairs
) const
{
    data.setElementPairs(container, std::move(elementPairs));
}

inline void RMetaContainer::setElementPairs(
    void * const container, const ElementPairsView<void *> &&elementPairs,
    bool * const elementKeysUnique
) const
{
    data.setElementPairsUnique(
        container, std::move(elementPairs),
        elementKeysUnique
    );
}

template<class T>
bool RMetaContainer::isContainer()
{
    return QMetaType::hasRegisteredConverterFunction(
        qMetaTypeId<T>(), qMetaTypeId<const RMetaContainer *>()
    );
}

inline bool RMetaContainer::isContainer(const int typeId)
{
    return QMetaType::hasRegisteredConverterFunction(
        typeId, qMetaTypeId<const RMetaContainer *>()
    );
}

template<class T>
const RMetaContainer * RMetaContainer::metaContainer()
{
    const RMetaContainer * metaContainer(nullptr);
    QMetaType::convert(
        nullptr, qMetaTypeId<T>(),
        &metaContainer, qMetaTypeId<const RMetaContainer *>()
    );
    return metaContainer;
}

inline const RMetaContainer * RMetaContainer::metaContainer(const int typeId)
{
    const RMetaContainer * metaContainer(nullptr);
    QMetaType::convert(
        nullptr, typeId,
        &metaContainer, qMetaTypeId<const RMetaContainer *>()
    );
    return metaContainer;
}

template<class T>
bool RMetaContainer::registerMetaContainer()
{
    if (isContainer<T>())
        return true;

    return QMetaType::registerConverter<T, const RMetaContainer *>(
        &RializerPrivate::MetaContainer::convert<T>
    );
}

inline RMetaContainer::RMetaContainer(Data &&data)
  : data(std::move(data))
{}

namespace RializerPrivate {
namespace MetaContainer {

namespace Traits = RContainerTraits;

struct Container
{
    template<class T>
    static RMetaType elementMetaType();

    template<class T>
    static RMetaType elementKeyMetaType();
    template<class T>
    static RMetaType elementValueMetaType();

    // These are instantiations of the methods,
    // the addresses of which to be stored.
    // Could take the addresses of the traits methods,
    // but for convenience, the traits methods
    // are strongly typed and have their single responsibility,
    // while these cast the container type and do multiple things.
    template<class T>
    static ElementsView<const void *> elements(const void *container);
    template<class T>
    static void setElements(
        void *container, const ElementsView<void *> &&elements
    );
    template<class T>
    static void setElementsUnique(
        void *container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    );

    template<class T>
    static ElementPairsView<const void *> elementPairs(const void *container);
    template<class T>
    static void setElementPairs(
        void *container, const ElementPairsView<void *> &&elementPairs
    );
    template<class T>
    static void setElementPairsUnique(
        void *container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    );

    // Reuse conditional reserve.
    template<class T>
    static void reserve(
        T &container, typename Traits::SizeType<T>::type size
    );
    template<class T>
    static void reserve(
        T &container, typename Traits::SizeType<T>::type size,
        std::true_type
    );
    template<class T>
    static void reserve(
        T &container, typename Traits::SizeType<T>::type size,
        std::false_type
    );

    struct Stub;
};

struct Container::Stub
{
    // todo (review): maybe these should pring warning debug messages.

    template<class T>
    static RMetaType elementMetaType();

    template<class T>
    static RMetaType elementKeyMetaType();
    template<class T>
    static RMetaType elementValueMetaType();

    template<class T>
    static ElementsView<const void *> elements(const void *container);
    template<class T>
    static void setElements(
        void *container, const ElementsView<void *> &&elements
    );
    template<class T>
    static void setElementsUnique(
        void *container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    );

    template<class T>
    static ElementPairsView<const void *> elementPairs(const void *container);
    template<class T>
    static void setElementPairs(
        void *container, const ElementPairsView<void *> &&elementPairs
    );
    template<class T>
    static void setElementPairsUnique(
        void *container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    );
};

inline Data::Data(
    const Dimensionality dimensionality,

    const Structure structure,

    const bool isMultiple,

    MetaType * const elementMetaType,

    MetaType * const elementKeyMetaType,
    MetaType * const elementValueMetaType,

    Elements * const elements,
    SetElements * const setElements,
    SetElementsUnique * const setElementsUnique,

    ElementPairs * const elementPairs,
    SetElementPairs * const setElementPairs,
    SetElementPairsUnique * const setElementPairsUnique
) : dimensionality(dimensionality),

    structure(structure),

    isMultiple(isMultiple),

    elementMetaType(elementMetaType),

    elementKeyMetaType(elementKeyMetaType),
    elementValueMetaType(elementValueMetaType),

    elements(elements),
    setElements(setElements),
    setElementsUnique(setElementsUnique),

    elementPairs(elementPairs),
    setElementPairs(setElementPairs),
    setElementPairsUnique(setElementPairsUnique)
{}

inline RMetaContainer Data::create(Data &&data)
{
    return RMetaContainer(std::move(data));
}

template<class T>
const RMetaContainer * convert(const T &)
{
    static const RMetaContainer metaContainer(Data::create(create<T>()));
    return &metaContainer;
}

template<class T>
Data create()
{
    // Make sure the tags are valid.
    check<T>();

    // Do not instatiate conditional traits,
    // that are inappropriate for the container class.
    // There might be no proper implementation,
    // and it should not be demanded.

    constexpr bool isSingleDimensional(Traits::IsSingleDimensional<T>::value);
    constexpr bool isDoubleDimensional(Traits::IsDoubleDimensional<T>::value);
    constexpr bool isAssociative(Traits::IsAssociative<T>::value);

    using IsMultiple = typename std::conditional<
        isAssociative, Traits::IsMultiple<T>, std::true_type
    >::type;

    constexpr bool isMultiple(IsMultiple::value);

    using ElementMetaType = typename std::conditional<
        isSingleDimensional, Container, Container::Stub
    >::type;

    using ElementKeyMetaType = typename std::conditional<
        isDoubleDimensional, Container, Container::Stub
    >::type;
    using ElementValueMetaType = typename std::conditional<
        isDoubleDimensional, Container, Container::Stub
    >::type;

    using Elements = typename std::conditional<
        isSingleDimensional, Container, Container::Stub
    >::type;
    using SetElements = typename std::conditional<
        isSingleDimensional, Container, Container::Stub
    >::type;
    using SetElementsUnique = typename std::conditional<
        isSingleDimensional && isAssociative && !isMultiple,
        Container, Container::Stub
    >::type;

    using ElementPairs = typename std::conditional<
        isDoubleDimensional, Container, Container::Stub
    >::type;
    using SetElementPairs = typename std::conditional<
        isDoubleDimensional, Container, Container::Stub
    >::type;
    using EetElementPairsUnique = typename std::conditional<
        isDoubleDimensional && isAssociative && !isMultiple,
        Container, Container::Stub
    >::type;

    return Data(
        Traits::Dimensionality<T>::value,

        Traits::Structure<T>::value,

        isMultiple,

        &ElementMetaType::template elementMetaType<T>,

        &ElementKeyMetaType::template elementKeyMetaType<T>,
        &ElementValueMetaType::template elementValueMetaType<T>,

        &Elements::template elements<T>,
        &SetElements::template setElements<T>,
        &SetElementsUnique::template setElementsUnique<T>,

        &ElementPairs::template elementPairs<T>,
        &SetElementPairs::template setElementPairs<T>,
        &EetElementPairsUnique::template setElementPairsUnique<T>
    );
}

template<class T>
void check()
{
    Q_STATIC_ASSERT(
        (Traits::Dimensionality<T>::value == Dimensionality::Single)
         || (Traits::Dimensionality<T>::value == Dimensionality::Double)
    );

    Q_STATIC_ASSERT(
        (Traits::Structure<T>::value == Structure::Sequence)
         || (Traits::Structure<T>::value == Structure::Associative)
    );

    // A double dimensional container can not be sequential.
    Q_STATIC_ASSERT(
        (Traits::Dimensionality<T>::value != Dimensionality::Double)
         || (Traits::Structure<T>::value != Structure::Sequence)
    );
}

template<typename T>
RMetaType Container::elementMetaType()
{
    using ElementType = typename Traits::ElementType<T>::type;

    return RMetaType::fromType<ElementType>();
}

template<typename T>
RMetaType Container::elementKeyMetaType()
{
    using ElementKeyType = typename Traits::ElementKeyType<T>::type;

    return RMetaType::fromType<ElementKeyType>();
}

template<typename T>
RMetaType Container::elementValueMetaType()
{
    using ElementValueType = typename Traits::ElementValueType<T>::type;

    return RMetaType::fromType<ElementValueType>();
}

template<class T>
ElementsView<const void *>
Container::elements(const void * const container1)
{
    const T &container(*static_cast<const T *>(container1));
    ElementsView<const void *> elements;
    // todo (review): what should happen if there is an overflow?
    elements.reserve(
        static_cast<ElementsView<const void *>::size_type>(
            Traits::Size<T>::size(container)
        )
    );
    Traits::Elements<T>::elements(container, &elements);
    return elements;
}

template<class T>
void Container::setElements(
    void * const container1, const ElementsView<void *> &&elements
)
{
    using SizeType = typename Traits::SizeType<T>::type;

    T &container(*static_cast<T *>(container1));
    Traits::Clear<T>::clear(container);
    // todo (review): what should happen if there is an overflow?
    reserve<T>(container, static_cast<SizeType>(elements.size()));
    Traits::SetElements<T>::setElements(
        container, std::move(elements)
    );
}

template<class T>
void Container::setElementsUnique(
    void * const container1, const ElementsView<void *> &&elements,
    bool * const elementsUnique
)
{
    using SizeType = typename Traits::SizeType<T>::type;

    T &container(*static_cast<T *>(container1));
    Traits::Clear<T>::clear(container);
    reserve<T>(container, static_cast<SizeType>(elements.size()));
    Traits::SetElementsUnique<T>::setElements(
        container, std::move(elements),
        elementsUnique
    );
}

template<class T>
ElementPairsView<const void *>
Container::elementPairs(const void * const container1)
{
    const T &container(*static_cast<const T *>(container1));
    ElementPairsView<const void *> elementPairs;
    elementPairs.reserve(
        static_cast<ElementPairsView<const void *>::size_type>(
            Traits::Size<T>::size(container)
        )
    );
    Traits::ElementPairs<T>::elementPairs(container, &elementPairs);
    return elementPairs;
}

template<class T>
void Container::setElementPairs(
    void * const container1, const ElementPairsView<void *> &&elementPairs
)
{
    using SizeType = typename Traits::SizeType<T>::type;

    T &container(*static_cast<T *>(container1));
    Traits::Clear<T>::clear(container);
    reserve<T>(container, static_cast<SizeType>(elementPairs.size()));
    Traits::SetElementPairs<T>::setElementPairs(
        container, std::move(elementPairs)
    );
}

template<class T>
void Container::setElementPairsUnique(
    void * const container1, const ElementPairsView<void *> &&elementPairs,
    bool * const elementKeysUnique
)
{
    using SizeType = typename Traits::SizeType<T>::type;

    T &container(*static_cast<T *>(container1));
    Traits::Clear<T>::clear(container);
    reserve<T>(container, static_cast<SizeType>(elementPairs.size()));
    Traits::SetElementPairsUnique<T>::setElementPairs(
        container, std::move(elementPairs),
        elementKeysUnique
    );
}

template<class T>
void Container::reserve(
    T &container, const typename Traits::SizeType<T>::type size
)
{
    using IsReservable = typename Traits::IsReservable<T>::type;

    reserve<T>(container, size, IsReservable());
}

template<class T>
void Container::reserve(
    T &container, const typename Traits::SizeType<T>::type size,
    std::true_type
)
{
    Traits::Reserve<T>::reserve(container, size);
}

template<class T>
void Container::reserve(
    T &container, const typename Traits::SizeType<T>::type size,
    std::false_type
)
{
    Q_UNUSED(container) Q_UNUSED(size)
}

template<typename T>
RMetaType Container::Stub::elementMetaType()
{
    return RMetaType();
}

template<typename T>
RMetaType Container::Stub::elementKeyMetaType()
{
    return RMetaType();
}

template<typename T>
RMetaType Container::Stub::elementValueMetaType()
{
    return RMetaType();
}

template<class T>
ElementsView<const void *>
Container::Stub::elements(const void * const container)
{
    Q_UNUSED(container)
    return ElementsView<const void *>();
}

template<class T>
void Container::Stub::setElements(
    void * const container, const ElementsView<void *> &&elements
)
{
    Q_UNUSED(container) Q_UNUSED(elements)
}

template<class T>
void Container::Stub::setElementsUnique(
    void * const container, const ElementsView<void *> &&elements,
    bool * const elementsUnique
)
{
    Q_UNUSED(container) Q_UNUSED(elements)
    Q_UNUSED(elementsUnique)
}

template<class T>
ElementPairsView<const void *>
Container::Stub::elementPairs(const void * const container)
{
    Q_UNUSED(container)
    return ElementPairsView<const void *>();
}

template<class T>
void Container::Stub::setElementPairs(
    void * const container, const ElementPairsView<void *> &&elementPairs
)
{
    Q_UNUSED(container) Q_UNUSED(elementPairs)
}

template<class T>
void Container::Stub::setElementPairsUnique(
    void * const container, const ElementPairsView<void *> &&elementPairs,
    bool * const elementKeysUnique
)
{
    Q_UNUSED(container) Q_UNUSED(elementPairs)
    Q_UNUSED(elementKeysUnique)
}

} // namespace MetaContainer
} // namespace RializerPrivate

namespace RializerPrivate {

// todo (C++17): std::void_t
template<typename ...>
struct make_void { using type = void; };
template<typename ... T>
using void_t = typename make_void<T ...>::type;

} // namespace RializerPrivate

namespace RContainerTraits {

template<class T>
struct Dimensionality<T, void>
  : Library<T>::type::template Dimensionality<T> {};

template<class T>
struct Structure<T, void>
  : Library<T>::type::template Structure<T> {};

template<class T>
struct IsMultiple<T, void>
  : Library<T>::type::template IsMultiple<T> {};

template<class T>
struct IsReservable<T, void>
  : Library<T>::type::template IsReservable<T> {};

template<class T>
struct ElementType<T, void>
  : Library<T>::type::template ElementType<T> {};

template<class T>
struct ElementKeyType<T, void>
  : Library<T>::type::template ElementKeyType<T> {};

template<class T>
struct ElementValueType<T, void>
  : Library<T>::type::template ElementValueType<T> {};

template<class T>
struct SizeType<T, void>
  : Library<T>::type::template SizeType<T> {};

template<class T>
struct Elements<T, void>
  : Library<T>::type::template Elements<T> {};

template<class T>
struct SetElements<T, void>
  : Library<T>::type::template SetElements<T> {};

template<class T>
struct SetElementsUnique<T, void>
  : Library<T>::type::template SetElementsUnique<T> {};

template<class T>
struct ElementPairs<T, void>
  : Library<T>::type::template ElementPairs<T> {};

template<class T>
struct SetElementPairs<T, void>
  : Library<T>::type::template SetElementPairs<T> {};

template<class T>
struct SetElementPairsUnique<T, void>
  : Library<T>::type::template SetElementPairsUnique<T> {};

template<class T>
struct Size<T, void>
  : Library<T>::type::template Size<T> {};

template<class T>
struct Reserve<T, void>
  : Library<T>::type::template Reserve<T> {};

template<class T>
struct Clear<T, void>
  : Library<T>::type::template Clear<T> {};

template<class T>
struct Library<T, void>
{
    using type = Std;
};

struct Std
{
    template<class T, class = void>
    struct Dimensionality;

    template<class T, class = void>
    struct Structure;

    template<class T, class = void>
    struct IsMultiple;

    template<class T, class = void>
    struct IsReservable;

    template<class T, class = void>
    struct ElementType;

    template<class T, class = void>
    struct ElementKeyType;
    template<class T, class = void>
    struct ElementValueType;

    template<class T, class = void>
    struct SizeType;

    template<class T, class = void>
    struct Elements;
    template<class T, class = void>
    struct SetElements;
    template<class T, class = void>
    struct SetElementsUnique;

    template<class T, class = void>
    struct ElementPairs;
    template<class T, class = void>
    struct SetElementPairs;
    template<class T, class = void>
    struct SetElementPairsUnique;

    template<class T, class = void>
    struct Size;
    template<class T, class = void>
    struct Reserve;
    template<class T, class = void>
    struct Clear;

    // These are separate because in template metaprogramming:
    // there is no other way to tell if a class does not have a member;
    // member type aliases are not a part of the immediate context, -
    // and element type aliases traits are instantiated conditionally.
    template<class T, class = void>
    struct HasElementType;
    template<class T, class = void>
    struct HasElementKeyType;
    template<class T, class = void>
    struct HasElementValueType;

    template<class T, class = void>
    struct HasInsertMultiple;
    template<class T, class = void>
    struct HasInsertUnique;
};

template<class T>
struct Std::Dimensionality<
    T,
    typename std::enable_if<
        Std::HasElementType<T>::value
         && !Std::HasElementValueType<T>::value
    >::type
> : SingleDimensional {};

template<class T>
struct Std::Dimensionality<
    T,
    typename std::enable_if<
        Std::HasElementKeyType<T>::value
         && Std::HasElementValueType<T>::value
    >::type
> : DoubleDimensional {};

template<class T>
struct Std::Structure<
    T,
    typename std::enable_if<
        Std::HasElementType<T>::value
         && !Std::HasElementKeyType<T>::value
    >::type
> : Sequence {};

template<class T>
struct Std::Structure<
    T,
    typename std::enable_if<
        Std::HasElementKeyType<T>::value
    >::type
> : Associative {};

template<class T>
struct Std::IsMultiple<
    T,
    typename std::enable_if<
        Std::HasInsertMultiple<T>::value
         && !Std::HasInsertUnique<T>::value
    >::type
> : std::true_type {};

template<class T>
struct Std::IsMultiple<
    T,
    typename std::enable_if<
        Std::HasInsertUnique<T>::value
    >::type
> : std::false_type {};

template<class T, class>
struct Std::IsReservable
  : std::false_type {};

template<class T>
struct Std::IsReservable<
    T,
    RializerPrivate::void_t<
        decltype(
            static_cast<
                void (T::*)(typename Traits::SizeType<T>::type)
            >(&T::reserve)
        )
    >
> : std::true_type {};

template<class T, class>
struct Std::ElementType
{
    using type = typename T::value_type;
};

template<class T, class>
struct Std::ElementKeyType
{
    using type = typename T::key_type;
};

// mapped_type is not an actual standard requirement. Hope this changes.
template<class T, class>
struct Std::ElementValueType
{
    using type = typename T::mapped_type;
};

template<class T, class>
struct Std::SizeType
{
    using type = typename T::size_type;
};

template<class T, class>
struct Std::Elements
{
    static void elements(
        const T &container, ElementsView<const void *> *elements
    );
};

template<class T, class Void>
void Std::Elements<T, Void>::elements(
    const T &container, ElementsView<const void *> * const elements
)
{
    using ElementType = typename Traits::ElementType<T>::type;

    // Make sure the element is a reference to an element,
    // not to an iterator, or a proxy, or a copy.
    for (auto &element : T()) {
        constexpr bool isElementReference(
            std::is_same<decltype(element), ElementType &>::value
             || std::is_same<decltype(element), const ElementType &>::value
        );
        Q_STATIC_ASSERT(isElementReference);
    }

    for (const auto &element : container) {
        elements->push_back(
            static_cast<const void *>(std::addressof(element))
        );
    }
}

template<class T>
struct Std::SetElements<
    T,
    typename std::enable_if<
        Traits::IsSequence<T>::value
    >::type
>
{
    static void setElements(
        T &container, const ElementsView<void *> &&elements
    );
};

template<class T>
void Std::SetElements<
    T,
    typename std::enable_if<
        Traits::IsSequence<T>::value
    >::type
>::setElements(
    T &container, const ElementsView<void *> &&elements
)
{
    using ElementType = typename Traits::ElementType<T>::type;

    for (const auto &element1 : elements) {
        ElementType &element(*static_cast<ElementType *>(element1));
        container.push_back(std::move(element));
    }
}

template<class T>
struct Std::SetElements<
    T,
    typename std::enable_if<
        Traits::IsAssociative<T>::value
    >::type
>
{
    static void setElements(
        T &container, const ElementsView<void *> &&elements
    );
};

template<class T>
void Std::SetElements<
    T,
    typename std::enable_if<
        Traits::IsAssociative<T>::value
    >::type
>::setElements(
    T &container, const ElementsView<void *> &&elements
)
{
    using ElementType = typename Traits::ElementType<T>::type;

    for (const auto &element1 : elements) {
        ElementType &element(*static_cast<ElementType *>(element1));
        container.insert(std::move(element));
    }
}

template<class T, class>
struct Std::SetElementsUnique
{
    static void setElements(
        T &container, const ElementsView<void *> &&elements,
        bool *elementsUnique
    );
};

template<class T, class Void>
void Std::SetElementsUnique<T, Void>::setElements(
    T &container, const ElementsView<void *> &&elements,
    bool * const elementsUnique
)
{
    Traits::SetElements<T>::setElements(
        container, std::move(elements)
    );
    // todo (review): what should happen if there is an overflow?
    *elementsUnique = (
        elements.size()
         == static_cast<ElementsView<void *>::size_type>(
                Traits::Size<T>::size(container)
            )
    );
}

template<class T, class>
struct Std::ElementPairs
{
    static void elementPairs(
        const T &container, ElementPairsView<const void *> *elementPairs
    );
};

template<class T, class Void>
void Std::ElementPairs<T, Void>::elementPairs(
    const T &container, ElementPairsView<const void *> * const elementPairs
)
{
    using ElementKeyType = typename Traits::ElementKeyType<T>::type;
    using ElementValueType = typename Traits::ElementValueType<T>::type;

    for (auto &element : T()) {
        constexpr bool isElementPairReference = std::is_same<
            decltype(element),
            std::pair<const ElementKeyType, ElementValueType> &
        >::value;
        Q_STATIC_ASSERT(isElementPairReference);
    }

    for (const auto &elementPair : container) {
        elementPairs->push_back(
            std::pair<const void *, const void *>(
                static_cast<const void *>(std::addressof(elementPair.first)),
                static_cast<const void *>(std::addressof(elementPair.second))
            )
        );
    }
}

template<class T, class>
struct Std::SetElementPairs
{
    static void setElementPairs(
        T &container, const ElementPairsView<void *> &&elementPairs
    );
};

template<class T, class Void>
void Std::SetElementPairs<T, Void>::setElementPairs(
    T &container, const ElementPairsView<void *> &&elementPairs
)
{
    using ElementKeyType = typename Traits::ElementKeyType<T>::type;
    using ElementValueType = typename Traits::ElementValueType<T>::type;

    for (const auto &elementPair : elementPairs) {
        ElementKeyType &elementKey(
            *static_cast<ElementKeyType *>(elementPair.first)
        );
        ElementValueType &elementValue(
            *static_cast<ElementValueType *>(elementPair.second)
        );
        container.insert(
            std::pair<const ElementKeyType, ElementValueType>(
                std::move(elementKey), std::move(elementValue)
            )
        );
    }
}

template<class T, class>
struct Std::SetElementPairsUnique
{
    static void setElementPairs(
        T &container, const ElementPairsView<void *> &&elementPairs,
        bool *elementKeysUnique
    );
};

template<class T, class Void>
void Std::SetElementPairsUnique<T, Void>::setElementPairs(
    T &container, const ElementPairsView<void *> &&elementPairs,
    bool * const elementKeysUnique
)
{
    Traits::SetElementPairs<T>::setElementPairs(
        container, std::move(elementPairs)
    );
    *elementKeysUnique = (
        elementPairs.size()
         == static_cast<ElementPairsView<void *>::size_type>(
                Traits::Size<T>::size(container)
            )
    );
}

template<class T, class>
struct Std::Size
{
    using SizeType = typename Traits::SizeType<T>::type;

    static SizeType size(const T &container);
};

template<class T, class Void>
auto Std::Size<T, Void>::size(const T &container) -> SizeType
{
    return container.size();
}

template<class T, class>
struct Std::Reserve
{
    using SizeType = typename Traits::SizeType<T>::type;

    static void reserve(T &container, SizeType size);
};

template<class T, class Void>
void Std::Reserve<T, Void>::reserve(T &container, const SizeType size)
{
    container.reserve(size);
}

template<class T, class>
struct Std::Clear
{
    static void clear(T &container);
};

template<class T, class Void>
void Std::Clear<T, Void>::clear(T &container)
{
    container.clear();
}

template<class T, class>
struct Std::HasElementType
  : std::false_type {};

template<class T>
struct Std::HasElementType<
    T,
    RializerPrivate::void_t<
        typename T::value_type
    >
> : std::true_type {};

template<class T, class>
struct Std::HasElementKeyType
  : std::false_type {};

template<class T>
struct Std::HasElementKeyType<
    T,
    RializerPrivate::void_t<
        typename T::key_type
    >
> : std::true_type {};

template<class T, class>
struct Std::HasElementValueType
  : std::false_type {};

template<class T>
struct Std::HasElementValueType<
    T,
    RializerPrivate::void_t<
        typename T::mapped_type
    >
> : std::true_type {};

template<class T, class>
struct Std::HasInsertMultiple
  : std::false_type {};

// todo (C++14): Substitution proceeds in lexical order
// and stops when a failure is encountered.
// Remove the conditional traits wrapping with std::enable_if.
template<class T>
struct Std::HasInsertMultiple<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsSingleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename T::iterator (T::*)(
                    const typename std::enable_if<
                        Traits::IsSingleDimensional<T>::value,
                        Traits::ElementType<T>
                    >::type::type &
                )
            >(&T::insert)
        )
    >
> : std::true_type {};

template<class T>
struct Std::HasInsertMultiple<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsDoubleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename T::iterator (T::*)(
                    const std::pair<
                        const typename std::enable_if<
                            Traits::IsDoubleDimensional<T>::value,
                            Traits::ElementKeyType<T>
                        >::type::type,
                        typename std::enable_if<
                            Traits::IsDoubleDimensional<T>::value,
                            Traits::ElementValueType<T>
                        >::type::type
                    > &
                )
            >(&T::insert)
        )
    >
> : std::true_type {};

template<class T, class>
struct Std::HasInsertUnique
  : std::false_type {};

template<class T>
struct Std::HasInsertUnique<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsSingleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                std::pair<typename T::iterator, bool> (T::*)(
                    const typename std::enable_if<
                        Traits::IsSingleDimensional<T>::value,
                        Traits::ElementType<T>
                    >::type::type &
                )
            >(&T::insert)
        )
    >
> : std::true_type {};

template<class T>
struct Std::HasInsertUnique<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsDoubleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                std::pair<typename T::iterator, bool> (T::*)(
                    const std::pair<
                        const typename std::enable_if<
                            Traits::IsDoubleDimensional<T>::value,
                            Traits::ElementKeyType<T>
                        >::type::type,
                        typename std::enable_if<
                            Traits::IsDoubleDimensional<T>::value,
                            Traits::ElementValueType<T>
                        >::type::type
                    > &
                )
            >(&T::insert)
        )
    >
> : std::true_type {};

template<typename T>
struct Library<QVector<T>, void> { using type = Qt; };
template<typename T>
struct Library<QList<T>, void> { using type = Qt; };
template<typename T>
struct Library<QSet<T>, void> { using type = Qt; };
template<typename K, typename V>
struct Library<QMap<K, V>, void> { using type = Qt; };
template<typename K, typename V>
struct Library<QMultiMap<K, V>, void> { using type = Qt; };
template<typename K, typename V>
struct Library<QHash<K, V>, void> { using type = Qt; };
template<typename K, typename V>
struct Library<QMultiHash<K, V>, void> { using type = Qt; };

struct Qt
{
    template<class T>
    using Dimensionality = Std::Dimensionality<T>;

    template<class T>
    using Structure = Std::Structure<T>;

    template<class T, class = void>
    struct IsMultiple;

    template<class T>
    using IsReservable = Std::IsReservable<T>;

    template<class T>
    using ElementType = Std::ElementType<T>;

    template<class T>
    using ElementKeyType = Std::ElementKeyType<T>;
    template<class T>
    using ElementValueType = Std::ElementValueType<T>;

    template<class T>
    using SizeType = Std::SizeType<T>;

    template<class T>
    using Elements = Std::Elements<T>;
    template<class T>
    using SetElements = Std::SetElements<T>;
    template<class T>
    using SetElementsUnique = Std::SetElementsUnique<T>;

    template<class T, class = void>
    struct ElementPairs;
    template<class T, class = void>
    struct SetElementPairs;
    template<class T>
    using SetElementPairsUnique = Std::SetElementPairsUnique<T>;

    template<class T>
    using Size = Std::Size<T>;
    template<class T>
    using Reserve = Std::Reserve<T>;
    template<class T>
    using Clear = Std::Clear<T>;

    template<class T, class = void>
    struct HasCountUnique;
    template<class T, class = void>
    struct HasCountMultiple;
};

template<class T>
struct Qt::IsMultiple<
    T,
    typename std::enable_if<
        Qt::HasCountUnique<T>::value
         && !Qt::HasCountMultiple<T>::value
    >::type
> : std::false_type {};

template<class T>
struct Qt::IsMultiple<
    T,
    typename std::enable_if<
        Qt::HasCountMultiple<T>::value
    >::type
> : std::true_type {};

template<class T, class>
struct Qt::ElementPairs
{
    static void elementPairs(
        const T &container, ElementPairsView<const void *> *elementPairs
    );
};

template<class T, class Void>
void Qt::ElementPairs<T, Void>::elementPairs(
    const T &container, ElementPairsView<const void *> * const elementPairs
)
{
    using IteratorType = typename T::const_iterator;

    IteratorType i(container.constBegin());
    IteratorType end(container.constEnd());
    while (i != end) {
        elementPairs->push_back(
            std::pair<const void *, const void *>(
                static_cast<const void *>(std::addressof(i.key())),
                static_cast<const void *>(std::addressof(i.value()))
            )
        );
        ++i;
    }
}

template<class T, class>
struct Qt::SetElementPairs
{
    static void setElementPairs(
        T &container, const ElementPairsView<void *> &&elementPairs
    );
};

template<class T, class Void>
void Qt::SetElementPairs<T, Void>::setElementPairs(
    T &container, const ElementPairsView<void *> &&elementPairs
)
{
    using ElementKeyType = typename Traits::ElementKeyType<T>::type;
    using ElementValueType = typename Traits::ElementValueType<T>::type;

    for (const auto &elementPair : elementPairs) {
        ElementKeyType &elementKey(
            *static_cast<ElementKeyType *>(elementPair.first)
        );
        ElementValueType &elementValue(
            *static_cast<ElementValueType *>(elementPair.second)
        );
        container.insert(
            std::move(elementKey), std::move(elementValue)
        );
    }
}

template<class T, class>
struct Qt::HasCountUnique
  : std::false_type {};

template<class T>
struct Qt::HasCountUnique<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsSingleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename Traits::SizeType<T>::type (T::*)() const
            >(&T::count)
        )
    >
> : std::true_type {};

template<class T>
struct Qt::HasCountUnique<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsDoubleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename Traits::SizeType<T>::type (T::*)(
                    const typename std::enable_if<
                        Traits::IsDoubleDimensional<T>::value,
                        Traits::ElementKeyType<T>
                    >::type::type &
                ) const
            >(&T::count)
        )
    >
> : std::true_type {};

template<class T, class>
struct Qt::HasCountMultiple
  : std::false_type {};

template<class T>
struct Qt::HasCountMultiple<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsSingleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename Traits::SizeType<T>::type (T::*)(
                    const typename std::enable_if<
                        Traits::IsSingleDimensional<T>::value,
                        Traits::ElementType<T>
                    >::type::type &
                ) const
            >(&T::count)
        )
    >
> : std::true_type {};

template<class T>
struct Qt::HasCountMultiple<
    T,
    RializerPrivate::void_t<
        typename std::enable_if<
            Traits::IsDoubleDimensional<T>::value
        >::type,
        decltype(
            static_cast<
                typename Traits::SizeType<T>::type (T::*)(
                    const typename std::enable_if<
                        Traits::IsDoubleDimensional<T>::value,
                        Traits::ElementKeyType<T>
                    >::type::type &,
                    const typename std::enable_if<
                        Traits::IsDoubleDimensional<T>::value,
                        Traits::ElementValueType<T>
                    >::type::type &
                ) const
            >(&T::count)
        )
    >
> : std::true_type {};

} // namespace RContainerTraits

} // namespace Rializer

#endif // RIALIZER_RMETACONTAINER_H
