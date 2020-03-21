#ifndef RIALIZER_METADATATYPES_H
#define RIALIZER_METADATATYPES_H

#include <rializer/metatypesystem/metadatatypes_fd.h>

#include <metatypesystem/rmetatype.h>
#include <metatypesystem/rmetacontainer.h>

#include <QtCore/QMetaType>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QVariant>

#include <vector>

// This is the data model.
// It is more convenient than using the meta type system directly.

namespace Rializer {

#define RIALIZER_MOVE_DEFAULT(Class) \
    Class(Class &&) = default; \
    Class & operator=(Class &&) = default;

#define RIALIZER_MOVE_ONLY_HACK(Class) \
    protected: \
        Class() = default; \
        Class(const Class &) {} \
        Class & operator=(const Class &) { return *this; } \
        template<typename T, bool> \
        friend struct QtMetaTypePrivate::QMetaTypeFunctionHelper;

namespace Type {

enum struct Description
{
    None, // todo: remove
    Primitive,
    Enumeration,
    Composite,
    Container
};

enum class Primitive { Primitive };

struct Type
{
    RMetaType metaType;
    Description description;

    const Enumeration & enumeration() const;
    const Composite & composite() const;
    const Container::Container & container() const;

    Type(RMetaType &&metaType, Primitive);
    Type(
        RMetaType &&metaType,
        Enumeration &&enumeration
    );
    Type(
        RMetaType &&metaType,
        Composite &&composite
    );
    Type(
        RMetaType &&metaType,
        Container::Container &&container
    );

    Type(); // todo: remove

    RIALIZER_MOVE_DEFAULT(Type)
    Q_DISABLE_COPY(Type)

protected:
    // Acts as a union.
    // todo (C++17): std::variant
    // Remove default constructor and Q_DECLARE_METATYPE of variants.
    QVariant type;
};

struct Enumeration
{
    QMetaEnum metaEnumeration;

    explicit Enumeration(QMetaEnum &&metaEnumeration);

    RIALIZER_MOVE_DEFAULT(Enumeration)
    RIALIZER_MOVE_ONLY_HACK(Enumeration)
};

struct Property
{
    QMetaProperty metaProperty;
    Type type;

    Property(
        QMetaProperty &&metaProperty,
        Type &&type
    );

    RIALIZER_MOVE_DEFAULT(Property)
    RIALIZER_MOVE_ONLY_HACK(Property)
};

using Properties = std::vector<Property>;

struct Composite
{
    const QMetaObject *metaObject;
    Properties properties;

    Composite(
        const QMetaObject *metaObject,
        Properties &&properties
    );

    RIALIZER_MOVE_DEFAULT(Composite)
    RIALIZER_MOVE_ONLY_HACK(Composite)
};

namespace Container {

using Rializer::Container::Dimensionality;

struct Container
{
    const RMetaContainer *metaContainer;
    Dimensionality description;

    const SingleDimensional & singleDimensional() const;
    const DoubleDimensional & doubleDimensional() const;

    bool isSingleDimensional() const;
    bool isDoubleDimensional() const;

    Container(
        const RMetaContainer *metaContainer,
        SingleDimensional &&singleDimensional
    );
    Container(
        const RMetaContainer *metaContainer,
        DoubleDimensional &&doubleDimensional
    );

    RIALIZER_MOVE_DEFAULT(Container)
    RIALIZER_MOVE_ONLY_HACK(Container)

protected:
    QVariant container;
};

struct SingleDimensional
{
    Type elementType;

    explicit SingleDimensional(Type &&elementType);

    RIALIZER_MOVE_DEFAULT(SingleDimensional)
    RIALIZER_MOVE_ONLY_HACK(SingleDimensional)
};

struct DoubleDimensional
{
    Type elementKeyType;
    Type elementValueType;

    DoubleDimensional(Type &&elementKeyType, Type &&elementValueType);

    RIALIZER_MOVE_DEFAULT(DoubleDimensional)
    RIALIZER_MOVE_ONLY_HACK(DoubleDimensional)
};

} // namespace Container

} // namespace Type

namespace Value {

using Type::Description;

using Type::Primitive;

struct Value
{
    const QVariant & primitive() const;
    QVariant & primitive();
    const Enumeration & enumeration() const;
    Enumeration & enumeration();
    const Composite & composite() const;
    Composite & composite();
    const Container::Container & container() const;
    Container::Container & container();

    Value(QVariant &&primitive, Primitive);
    explicit Value(Enumeration &&enumeration);
    explicit Value(Composite &&composite);
    explicit Value(Container::Container &&container);

    Value() = default; // todo: remove

    RIALIZER_MOVE_DEFAULT(Value)
    Q_DISABLE_COPY(Value)

protected:
    QVariant value;
};

struct Enumeration
{
    bool description;

    int number() const;
    const QByteArray & string() const;

    bool isNumber() const;
    bool isString() const;

    QByteArray toString(const Type::Enumeration &type) const;

    explicit Enumeration(int value);
    explicit Enumeration(QByteArray &&value);

    RIALIZER_MOVE_DEFAULT(Enumeration)
    RIALIZER_MOVE_ONLY_HACK(Enumeration)

protected:
    int number1;
    QByteArray string1;
};

using Properties = std::vector<Value>;

struct Composite
{
    Properties properties;

    explicit Composite(Properties &&properties);

    RIALIZER_MOVE_DEFAULT(Composite)
    RIALIZER_MOVE_ONLY_HACK(Composite)
};

namespace Container {

using Rializer::Container::Dimensionality;

struct Container
{
    const SingleDimensional & singleDimensional() const;
    SingleDimensional & singleDimensional();
    const DoubleDimensional & doubleDimensional() const;
    DoubleDimensional & doubleDimensional();

    explicit Container(SingleDimensional &&singleDimensional);
    explicit Container(DoubleDimensional &&doubleDimensional);

    RIALIZER_MOVE_DEFAULT(Container)
    RIALIZER_MOVE_ONLY_HACK(Container)

protected:
    QVariant container;
};

using Elements = std::vector<Value>;

struct SingleDimensional
{
    Elements elements;

    explicit SingleDimensional(Elements &&elements);

    RIALIZER_MOVE_DEFAULT(SingleDimensional)
    RIALIZER_MOVE_ONLY_HACK(SingleDimensional)
};

using ElementPairs = std::vector<std::pair<Value, Value>>;

struct DoubleDimensional
{
    ElementPairs elementPairs;

    explicit DoubleDimensional(ElementPairs &&elementPairs);

    RIALIZER_MOVE_DEFAULT(DoubleDimensional)
    RIALIZER_MOVE_ONLY_HACK(DoubleDimensional)
};

} // namespace Container

} // namespace Value

#undef RIALIZER_MOVE_DEFAULT
#undef RIALIZER_MOVE_ONLY_HACK

} // namespace Rializer

Q_DECLARE_METATYPE(Rializer::Type::Enumeration);
Q_DECLARE_METATYPE(Rializer::Type::Composite);
Q_DECLARE_METATYPE(Rializer::Type::Property);
Q_DECLARE_METATYPE(Rializer::Type::Container::Container);
Q_DECLARE_METATYPE(Rializer::Type::Container::SingleDimensional);
Q_DECLARE_METATYPE(Rializer::Type::Container::DoubleDimensional);

Q_DECLARE_METATYPE(Rializer::Value::Enumeration);
Q_DECLARE_METATYPE(Rializer::Value::Composite);
Q_DECLARE_METATYPE(Rializer::Value::Container::Container);
Q_DECLARE_METATYPE(Rializer::Value::Container::SingleDimensional);
Q_DECLARE_METATYPE(Rializer::Value::Container::DoubleDimensional);

namespace Rializer {

namespace Type {

inline const Enumeration & Type::enumeration() const
{
    return variantValue<const Enumeration>(type);
}

inline const Composite & Type::composite() const
{
    return variantValue<const Composite>(type);
}

inline const Container::Container & Type::container() const
{
    return variantValue<const Container::Container>(type);
}

inline Type::Type(RMetaType &&metaType, Primitive)
  : metaType(std::move(metaType)),
    description(Description::Primitive),
    type()
{}

inline Type::Type(
    RMetaType &&metaType,
    Enumeration &&enumeration
) : metaType(std::move(metaType)),
    description(Description::Enumeration),
    type(makeVariantFromValue(std::move(enumeration)))
{}

inline Type::Type(
    RMetaType &&metaType,
    Composite &&composite
) : metaType(std::move(metaType)),
    description(Description::Composite),
    type(makeVariantFromValue(std::move(composite)))
{}

inline Type::Type(
    RMetaType &&metaType,
    Container::Container &&container
) : metaType(std::move(metaType)),
    description(Description::Container),
    type(makeVariantFromValue(std::move(container)))
{}

inline Type::Type()
  : metaType(),
    description(Description::None),
    type()
{}

inline Enumeration::Enumeration(QMetaEnum &&metaEnumeration)
  : metaEnumeration(std::move(metaEnumeration))
{}

inline Property::Property(
    QMetaProperty &&metaProperty,
    Type &&type
) : metaProperty(std::move(metaProperty)),
    type(std::move(type))
{}

inline Composite::Composite(
    const QMetaObject * const metaObject,
    Properties &&properties
) : metaObject(metaObject),
    properties(std::move(properties))
{}

namespace Container {

inline const SingleDimensional & Container::singleDimensional() const
{
    return variantValue<const SingleDimensional>(container);
}

inline const DoubleDimensional & Container::doubleDimensional() const
{
    return variantValue<const DoubleDimensional>(container);
}

inline bool Container::isSingleDimensional() const
{
    return description == Dimensionality::Single;
}

inline bool Container::isDoubleDimensional() const
{
    return description == Dimensionality::Double;
}

inline Container::Container(
    const RMetaContainer *metaContainer,
    SingleDimensional &&singleDimensional
) : metaContainer(metaContainer),
    description(Dimensionality::Single),
    container(makeVariantFromValue(std::move(singleDimensional)))
{}

inline Container::Container(
    const RMetaContainer *metaContainer,
    DoubleDimensional &&doubleDimensional
) : metaContainer(metaContainer),
    description(Dimensionality::Double),
    container(makeVariantFromValue(std::move(doubleDimensional)))
{}

inline SingleDimensional::SingleDimensional(Type &&elementType)
  : elementType(std::move(elementType))
{}

inline DoubleDimensional::DoubleDimensional(
    Type &&elementKeyType, Type &&elementValueType
) : elementKeyType(std::move(elementKeyType)),
    elementValueType(std::move(elementValueType))
{}

} // namespace Container

} // namespace Type

namespace Value {

inline const QVariant & Value::primitive() const
{
    return value;
}

inline QVariant & Value::primitive()
{
    return value;
}

inline const Enumeration & Value::enumeration() const
{
    return variantValue<const Enumeration>(value);
}

inline Enumeration & Value::enumeration()
{
    return variantValue<Enumeration>(value);
}

inline const Composite & Value::composite() const
{
    return variantValue<const Composite>(value);
}

inline Composite & Value::composite()
{
    return variantValue<Composite>(value);
}

inline const Container::Container & Value::container() const
{
    return variantValue<const Container::Container>(value);
}

inline Container::Container & Value::container()
{
    return variantValue<Container::Container>(value);
}

inline Value::Value(QVariant &&value, Primitive)
  : value(std::move(value))
{}

inline Value::Value(Enumeration &&enumeration)
  : value(makeVariantFromValue(std::move(enumeration)))
{}

inline Value::Value(Composite &&composite)
  : value(makeVariantFromValue(std::move(composite)))
{}

inline Value::Value(Container::Container &&container)
  : value(makeVariantFromValue(std::move(container)))
{}

inline int Enumeration::number() const
{
    return number1;
}

inline const QByteArray & Enumeration::string() const
{
    return string1;
}

inline bool Enumeration::isNumber() const
{
    return description;
}

inline bool Enumeration::isString() const
{
    return !description;
}

inline QByteArray Enumeration::toString(const Type::Enumeration &type) const
{
    return type.metaEnumeration.valueToKey(number1);
}

inline Enumeration::Enumeration(const int value)
  : description(true),
    number1(value),
    string1()
{}

inline Enumeration::Enumeration(QByteArray &&value)
  : description(false),
    number1(),
    string1(std::move(value))
{}

inline Composite::Composite(Properties &&properties)
  : properties(std::move(properties))
{}

namespace Container {

inline const SingleDimensional & Container::singleDimensional() const
{
    return variantValue<const SingleDimensional>(container);
}

inline SingleDimensional & Container::singleDimensional()
{
    return variantValue<SingleDimensional>(container);
}

inline const DoubleDimensional & Container::doubleDimensional() const
{
    return variantValue<const DoubleDimensional>(container);
}

inline DoubleDimensional & Container::doubleDimensional()
{
    return variantValue<DoubleDimensional>(container);
}

inline Container::Container(SingleDimensional &&singleDimensional)
  : container(makeVariantFromValue(std::move(singleDimensional)))
{}

inline Container::Container(DoubleDimensional &&doubleDimensional)
  : container(makeVariantFromValue(std::move(doubleDimensional)))
{}

inline SingleDimensional::SingleDimensional(Elements &&elements)
  : elements(std::move(elements))
{}

inline DoubleDimensional::DoubleDimensional(ElementPairs &&elementPairs)
  : elementPairs(std::move(elementPairs))
{}

} // namespace Container

} // namespace Value

} // namespace Rializer

#endif // RIALIZER_METADATATYPES_H
