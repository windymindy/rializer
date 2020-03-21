#include <QtTest/QtTest>

#include <metatypesystem/RMetaContainer>

#include <deque>
#include <unordered_set>
#include <unordered_map>

using namespace Rializer;
using namespace ::Rializer::Rializer::Container;

class test_RMetaContainer : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();

    void registerMetaContainer_data();
    void registerMetaContainer();

    void description_data();
    void description();

    void elements_data();
    void elements();

    void setElements_data();
    void setElements();
    void setElementsUnique_data();
    void setElementsUnique();

    void elementPairs_data();
    void elementPairs();

    void setElementPairs_data();
    void setElementPairs();
    void setElementPairsUnique_data();
    void setElementPairsUnique();
};

Q_DECLARE_METATYPE(Dimensionality)
Q_DECLARE_METATYPE(Structure)

Q_DECLARE_METATYPE(const void *)
Q_DECLARE_METATYPE(std::string)

Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::deque)
Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::set)
Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::multiset)
Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::unordered_set)
Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::unordered_multiset)
Q_DECLARE_METATYPE_TEMPLATE_2ARG(std::multimap)
Q_DECLARE_METATYPE_TEMPLATE_2ARG(std::unordered_map)
Q_DECLARE_METATYPE_TEMPLATE_2ARG(std::unordered_multimap)

Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiMap)
Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiHash)

struct MyVector;
Q_DECLARE_METATYPE(MyVector)

// todo (review): is it sensible to test for different element types?
// QT_FOR_EACH_STATIC_TYPE?
// nested container?
// tst_qmetatype.h TestValueFactory?

// todo: remove std::initializer_list newRow variants
// after adding range constructors and iterators for Qt containers.

template<class T>
void registerMetaContainer();

template<
    template<typename ...> class T,
    typename V
>
void description_newRow();
template<
    template<typename ...> class T,
    typename V
>
void description_newRow(bool isMultiple);
template<
    template<typename ...> class T,
    typename K, typename V
>
void description_newRow(bool isMultiple);

template<
    template<typename ...> class T,
    typename V
>
void elements_newRow(const T<V> &value);
template<
    template<typename ...> class T,
    typename K, typename V
>
void elements_newRow(const T<K, V> &value);

template<
    template<typename ...> class T,
    typename V
>
void setElements_newRow(const T<V> &value);
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElements_newRow();

template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow(const std::vector<V> &value);
template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow_qt(std::initializer_list<V> value);
template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow();
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementsUnique_newRow();

template<
    template<typename ...> class T,
    typename K, typename V
>
void elementPairs_newRow(const T<K, V> &value);
template<
    template<typename ...> class T,
    typename V
>
void elementPairs_newRow(const T<V> &value);

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairs_newRow(const T<K, V> &value);
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairs_newRow_qt(std::initializer_list<std::pair<K, V>> value);
template<
    template<typename ...> class T,
    typename V
>
void setElementPairs_newRow();
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow(const std::vector<std::pair<K, V>> &value);
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow_qt(std::initializer_list<std::pair<K, V>> value);
template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow();
template<
    template<typename ...> class T,
    typename V
>
void setElementPairsUnique_newRow();

template<template<typename ...> class T, typename ...>
struct ContainerWithElements;

template<class T>
void registerMetaContainer()
{
    RMetaContainer::registerMetaContainer<T>();

    QMetaType::registerEqualsComparator<T>();
}

void test_RMetaContainer::initTestCase()
{
    using ::registerMetaContainer;

    registerMetaContainer<std::vector<int>>();
    registerMetaContainer<std::deque<int>>();
    registerMetaContainer<std::list<int>>();

    registerMetaContainer<std::set<QString>>();
    registerMetaContainer<std::multiset<QString>>();
    registerMetaContainer<std::unordered_set<std::string>>();
    registerMetaContainer<std::unordered_multiset<std::string>>();

    registerMetaContainer<std::map<int, QString>>();
    registerMetaContainer<std::multimap<int, QString>>();
    registerMetaContainer<std::unordered_map<std::string, int>>();
    registerMetaContainer<std::unordered_multimap<std::string, int>>();

    registerMetaContainer<QVector<int>>();
    registerMetaContainer<QList<int>>();

    registerMetaContainer<QSet<QString>>();

    registerMetaContainer<QMap<int, QString>>();
    registerMetaContainer<QMultiMap<int, QString>>();
    registerMetaContainer<QHash<QString, int>>();
    registerMetaContainer<QMultiHash<QString, int>>();
}

struct MyType {};
struct MyVector {};
struct MyTraits;

namespace Rializer {
namespace RContainerTraits {

template<>
struct Library<MyVector> { using type = MyTraits; };

} // namespace RContainerTraits
} // namespace Rializer

struct MyTraits
{
    template<typename ...>
    struct Dimensionality
      : RContainerTraits::SingleDimensional {};
    template<typename ...>
    struct Structure
      : RContainerTraits::Sequence {};

    template<typename ...>
    struct IsReservable
      : std::false_type {};

    template<typename ...>
    struct ElementType
    {
        using type = MyType;
    };

    template<typename ...>
    struct SizeType
    {
        using type = int;
    };

    template<typename ...>
    struct Elements
    {
        static void elements(
            const MyVector &, ElementsView<const void *> *
        ) {}
    };
    template<typename ...>
    struct SetElements
    {
        static void setElements(
            MyVector &, const ElementsView<void *> &&
        ) {}
    };

    template<typename ...>
    struct Size
    {
        static int size(const MyVector &) { return 0; }
    };
    template<typename ...>
    struct Clear
    {
        static void clear(MyVector &) {}
    };
};

void test_RMetaContainer::registerMetaContainer_data()
{
    QTest::addColumn<int>("typeId");

    QTest::addColumn<bool>("isContainer");
    QTest::addColumn<const RMetaContainer *>("metaContainer");

    QTest::newRow("registered")
     << qMetaTypeId<std::deque<int>>()

     << true
     << RMetaContainer::metaContainer<std::deque<int>>();

    RMetaContainer::registerMetaContainer<MyVector>();
    QTest::newRow("registered custom")
     << qMetaTypeId<MyVector>()

     << true
     << RMetaContainer::metaContainer<MyVector>();

    QTest::newRow("not registered")
     << qMetaTypeId<void>()

     << false
     << static_cast<const RMetaContainer *>(nullptr);
}

void test_RMetaContainer::registerMetaContainer()
{
    QFETCH(const int, typeId);
    QFETCH(const bool, isContainer);
    QFETCH(const RMetaContainer * const, metaContainer);

    QCOMPARE(RMetaContainer::isContainer(typeId), isContainer);
    QCOMPARE(RMetaContainer::metaContainer(typeId), metaContainer);
}

template<
    template<typename ...> class T,
    typename V
>
void description_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()

     << Dimensionality::Single
     << true << false

     << Structure::Sequence
     << true << false

     << true

     << qMetaTypeId<V>()

     << static_cast<int>(QMetaType::UnknownType)
     << static_cast<int>(QMetaType::UnknownType);
}

template<
    template<typename ...> class T,
    typename V
>
void description_newRow(const bool isMultiple)
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()

     << Dimensionality::Single
     << true << false

     << Structure::Associative
     << false << true

     << isMultiple

     << qMetaTypeId<V>()

     << static_cast<int>(QMetaType::UnknownType)
     << static_cast<int>(QMetaType::UnknownType);
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void description_newRow(const bool isMultiple)
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()

     << Dimensionality::Double
     << false << true

     << Structure::Associative
     << false << true

     << isMultiple

     << static_cast<int>(QMetaType::UnknownType)

     << qMetaTypeId<K>()
     << qMetaTypeId<V>();
}

void test_RMetaContainer::description_data()
{
    QTest::addColumn<int>("typeId");

    QTest::addColumn<Dimensionality>("dimensionality");
    QTest::addColumn<bool>("isSingleDimensional");
    QTest::addColumn<bool>("isDoubleDimensional");

    QTest::addColumn<Structure>("structure");
    QTest::addColumn<bool>("isSequence");
    QTest::addColumn<bool>("isAssociative");

    QTest::addColumn<bool>("isMultiple");

    QTest::addColumn<int>("elementTypeId");

    QTest::addColumn<int>("elementKeyTypeId");
    QTest::addColumn<int>("elementValueTypeId");

    description_newRow<std::vector, int>();
    description_newRow<std::deque, int>();
    description_newRow<std::list, int>();

    description_newRow<std::set, QString>(false);
    description_newRow<std::multiset, QString>(true);
    description_newRow<std::unordered_set, std::string>(false);
    description_newRow<std::unordered_multiset, std::string>(true);

    description_newRow<std::map, int, QString>(false);
    description_newRow<std::multimap, int, QString>(true);
    description_newRow<std::unordered_map, std::string, int>(false);
    description_newRow<std::unordered_multimap, std::string, int>(true);

    description_newRow<QVector, int>();
    description_newRow<QList, int>();

    description_newRow<QSet, QString>(false);

    description_newRow<QMap, int, QString>(false);
    description_newRow<QMultiMap, int, QString>(true);
    description_newRow<QHash, QString, int>(false);
    description_newRow<QMultiHash, QString, int>(true);
}

void test_RMetaContainer::description()
{
    QFETCH(const int, typeId);

    QFETCH(const Dimensionality, dimensionality);
    QFETCH(const bool, isSingleDimensional);
    QFETCH(const bool, isDoubleDimensional);

    QFETCH(const Structure, structure);
    QFETCH(const bool, isSequence);
    QFETCH(const bool, isAssociative);

    QFETCH(const bool, isMultiple);

    QFETCH(const int, elementTypeId);

    QFETCH(const int, elementKeyTypeId);
    QFETCH(const int, elementValueTypeId);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    QCOMPARE(metaContainer.dimensionality(), dimensionality);
    QCOMPARE(metaContainer.isSingleDimensional(), isSingleDimensional);
    QCOMPARE(metaContainer.isDoubleDimensional(), isDoubleDimensional);

    QCOMPARE(metaContainer.structure(), structure);
    QCOMPARE(metaContainer.isSequence(), isSequence);
    QCOMPARE(metaContainer.isAssociative(), isAssociative);

    QCOMPARE(metaContainer.isMultiple(), isMultiple);

    QCOMPARE(metaContainer.elementMetaType().id(), elementTypeId);

    QCOMPARE(metaContainer.elementKeyMetaType().id(), elementKeyTypeId);
    QCOMPARE(metaContainer.elementValueMetaType().id(), elementValueTypeId);
}

template<
    template<typename ...> class T,
    typename V
>
void elements_newRow(const T<V> &value)
{
    auto elements(
        ContainerWithElements<
            T, V,
            const void *
        >::create(value)
    );

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << elements.container

     << elements.elements;
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void elements_newRow(const T<K, V> &value)
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant::fromValue<T<K, V>>(value)

     << ElementsView<const void *>();
}

void test_RMetaContainer::elements_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<ElementsView<const void *>>("elements");

    elements_newRow<std::deque, int>({ 1, 2, 3 });

    elements_newRow<std::set, QString>({ "1", "2", "3" });

    elements_newRow<QVector, int>({ 1, 2, 3 });

    elements_newRow<QSet, QString>({ "1", "2", "3" });

    elements_newRow<std::map, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );
}

void test_RMetaContainer::elements()
{
    QFETCH(const int, typeId);
    QFETCH(const QVariant, container);

    QFETCH(const ElementsView<const void *>, elements);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    QCOMPARE(metaContainer.elements(container.constData()), elements);
}

template<
    template<typename ...> class T,
    typename V
>
void setElements_newRow(const T<V> &value)
{
    auto elements(
        ContainerWithElements<
            T, V,
            void *
        >::create(std::vector<V>(value.cbegin(), value.cend()))
    );

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << elements.container
     << elements.elements
     << QVariant::fromValue<T<V>>(value);
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElements_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << QVariant()
     << ElementsView<void *>{ nullptr, nullptr, nullptr }
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr);
}

void test_RMetaContainer::setElements_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<QVariant>("elements1");
    QTest::addColumn<ElementsView<void *>>("elements");
    QTest::addColumn<QVariant>("value");

    setElements_newRow<std::deque, int>({ 1, 2, 3 });

    setElements_newRow<std::set, QString>({ "1", "2", "3" });

    setElements_newRow<QVector, int>({ 1, 2, 3 });

    setElements_newRow<QSet, QString>({ "1", "2", "3" });

    setElements_newRow<std::map, int, QString>();
}

void test_RMetaContainer::setElements()
{
    QFETCH(const int, typeId);
    QFETCH(QVariant, container);

    QFETCH(const ElementsView<void *>, elements);
    QFETCH(const QVariant, value);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    metaContainer.setElements(
        container.data(), std::move(elements)
    );
    QCOMPARE(container, value);
}

template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow(const std::vector<V> &value)
{
    auto elements1(
        ContainerWithElements<
            T, V,
            void *
        >::create(value)
    );
    auto elements2(
        ContainerWithElements<
            T, V,
            void *
        >::create(value)
    );
    std::set<V> elements3(value.cbegin(), value.cend());

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << elements1.container
     << elements1.elements
     << elements2.container
     << elements2.elements
     << QVariant::fromValue<T<V>>(T<V>(value.cbegin(), value.cend()))
     << static_cast<bool>(value.size() == elements3.size());
}

template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow_qt(const std::initializer_list<V> value)
{
    auto elements1(
        ContainerWithElements<
            T, V,
            void *
        >::create(value)
    );
    auto elements2(
        ContainerWithElements<
            T, V,
            void *
        >::create(value)
    );
    std::set<V> elements3(value);

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << elements1.container
     << elements1.elements
     << elements2.container
     << elements2.elements
     << QVariant::fromValue<T<V>>(value)
     << static_cast<bool>(value.size() == elements3.size());
}

template<
    template<typename ...> class T,
    typename V
>
void setElementsUnique_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << QVariant()
     << ElementsView<void *>{ nullptr, nullptr, nullptr }
     << QVariant()
     << ElementsView<void *>{ nullptr, nullptr, nullptr }
     << QVariant(qMetaTypeId<T<V>>(), nullptr)
     << true;
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementsUnique_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << QVariant()
     << ElementsView<void *>{ nullptr, nullptr, nullptr }
     << QVariant()
     << ElementsView<void *>{ nullptr, nullptr, nullptr }
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)
     << true;
}

void test_RMetaContainer::setElementsUnique_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<QVariant>("elements11");
    QTest::addColumn<ElementsView<void *>>("elements1");
    QTest::addColumn<QVariant>("elements21");
    QTest::addColumn<ElementsView<void *>>("elements2");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("unique");

    setElementsUnique_newRow<std::set, QString>({ "1", "2", "3" });
    setElementsUnique_newRow<std::set, QString>({ "1", "2", "2", "3" });

    setElementsUnique_newRow_qt<QSet, QString>({ "1", "2", "3" });
    setElementsUnique_newRow_qt<QSet, QString>({ "1", "2", "2", "3" });

    setElementsUnique_newRow<std::multiset, QString>();

    setElementsUnique_newRow<std::deque, int>();

    setElementsUnique_newRow<std::map, int, QString>();
}

void test_RMetaContainer::setElementsUnique()
{
    QFETCH(const int, typeId);
    QFETCH(QVariant, container);

    QFETCH(const ElementsView<void *>, elements1);
    QFETCH(const ElementsView<void *>, elements2);
    QFETCH(const QVariant, value);
    QFETCH(const bool, unique);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    if (!metaContainer.isMultiple() && metaContainer.isSingleDimensional()) {
        bool elementsUnique1(true);
        metaContainer.setElements(
            container.data(), std::move(elements1),
            &elementsUnique1
        );

        QCOMPARE(container, value);
        QCOMPARE(elementsUnique1, unique);

        bool elementsUnique2(false);
        metaContainer.setElements(
            container.data(), std::move(elements2),
            &elementsUnique2
        );

        QCOMPARE(container, value);
        QCOMPARE(elementsUnique2, unique);

        bool elementsUnique3(true);
        metaContainer.setElements(
            container.data(), ElementsView<void *>(),
            &elementsUnique3
        );

        QCOMPARE(elementsUnique3, true);

        bool elementsUnique4(false);
        metaContainer.setElements(
            container.data(), ElementsView<void *>(),
            &elementsUnique4
        );

        QCOMPARE(elementsUnique4, true);
    } else {
        bool elementsUnique1(true);
        metaContainer.setElements(
            container.data(), std::move(elements1),
            &elementsUnique1
        );

        QCOMPARE(container, value);
        QCOMPARE(elementsUnique1, true);

        bool elementsUnique2(false);
        metaContainer.setElements(
            container.data(), std::move(elements2),
            &elementsUnique2
        );

        QCOMPARE(container, value);
        QCOMPARE(elementsUnique2, false);
    }
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void elementPairs_newRow(const T<K, V> &value)
{
    auto elements(
        ContainerWithElements<
            T, K, V,
            const void *
        >::create(value)
    );

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << elements.container

     << elements.elementPairs;
}

template<
    template<typename ...> class T,
    typename V
>
void elementPairs_newRow(const T<V> &value)
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant::fromValue<T<V>>(value)

     << ElementPairsView<const void *>();
}

void test_RMetaContainer::elementPairs_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<ElementPairsView<const void *>>("elementPairs");

    elementPairs_newRow<std::map, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );

    elementPairs_newRow<QMap, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );

    elementPairs_newRow<std::deque, int>({ 1, 2, 3 });
}

void test_RMetaContainer::elementPairs()
{
    QFETCH(const int, typeId);
    QFETCH(const QVariant, container);

    QFETCH(const ElementPairsView<const void *>, elementPairs);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    QCOMPARE(metaContainer.elementPairs(container.constData()), elementPairs);
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairs_newRow(const T<K, V> &value)
{
    auto elements(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(std::vector<std::pair<K, V>>(value.cbegin(), value.cend()))
    );

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << elements.container
     << elements.elementPairs
     << QVariant::fromValue<T<K, V>>(value);
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairs_newRow_qt(const std::initializer_list<std::pair<K, V>> value)
{
    auto elements(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(std::vector<std::pair<K, V>>(value))
    );

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << elements.container
     << elements.elementPairs
     << QVariant::fromValue<T<K, V>>(value);
}

template<
    template<typename ...> class T,
    typename V
>
void setElementPairs_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << QVariant()
     << ElementPairsView<void *>{ { nullptr, nullptr }, { nullptr, nullptr } }
     << QVariant(qMetaTypeId<T<V>>(), nullptr);
}

void test_RMetaContainer::setElementPairs_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<QVariant>("elementPairs1");
    QTest::addColumn<ElementPairsView<void *>>("elementPairs");
    QTest::addColumn<QVariant>("value");

    setElementPairs_newRow<std::map, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );

    setElementPairs_newRow_qt<QMap, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );

    setElementPairs_newRow<std::deque, int>();
}

void test_RMetaContainer::setElementPairs()
{
    QFETCH(const int, typeId);
    QFETCH(QVariant, container);

    QFETCH(const ElementPairsView<void *>, elementPairs);
    QFETCH(const QVariant, value);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    metaContainer.setElementPairs(
        container.data(), std::move(elementPairs)
    );
    QCOMPARE(container, value);
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow(const std::vector<std::pair<K, V>> &value)
{
    auto elementPairs1(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(value)
    );
    auto elementPairs2(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(value)
    );
    std::map<K, V> elementPairs3(value.cbegin(), value.cend());

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << elementPairs1.container
     << elementPairs1.elementPairs
     << elementPairs2.container
     << elementPairs2.elementPairs
     << QVariant::fromValue<T<K, V>>(T<K, V>(value.cbegin(), value.cend()))
     << static_cast<bool>(value.size() == elementPairs3.size());
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow_qt(const std::initializer_list<std::pair<K, V>> value)
{
    auto elementPairs1(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(value)
    );
    auto elementPairs2(
        ContainerWithElements<
            T, K, V,
            void *
        >::create(value)
    );
    std::map<K, V> elementPairs3(value.begin(), value.end());

    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << elementPairs1.container
     << elementPairs1.elementPairs
     << elementPairs2.container
     << elementPairs2.elementPairs
     << QVariant::fromValue<T<K, V>>(value)
     << static_cast<bool>(value.size() == elementPairs3.size());
}

template<
    template<typename ...> class T,
    typename K, typename V
>
void setElementPairsUnique_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<K, V>>()))
     << qMetaTypeId<T<K, V>>()
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)

     << QVariant()
     << ElementPairsView<void *>{ { nullptr, nullptr }, { nullptr, nullptr } }
     << QVariant()
     << ElementPairsView<void *>{ { nullptr, nullptr }, { nullptr, nullptr } }
     << QVariant(qMetaTypeId<T<K, V>>(), nullptr)
     << true;
}

template<
    template<typename ...> class T,
    typename V
>
void setElementPairsUnique_newRow()
{
    QTest::newRow(QMetaType::typeName(qMetaTypeId<T<V>>()))
     << qMetaTypeId<T<V>>()
     << QVariant(qMetaTypeId<T<V>>(), nullptr)

     << QVariant()
     << ElementPairsView<void *>{ { nullptr, nullptr }, { nullptr, nullptr } }
     << QVariant()
     << ElementPairsView<void *>{ { nullptr, nullptr }, { nullptr, nullptr } }
     << QVariant(qMetaTypeId<T<V>>(), nullptr)
     << true;
}

void test_RMetaContainer::setElementPairsUnique_data()
{
    QTest::addColumn<int>("typeId");
    QTest::addColumn<QVariant>("container");

    QTest::addColumn<QVariant>("elementPairs11");
    QTest::addColumn<ElementPairsView<void *>>("elementPairs1");
    QTest::addColumn<QVariant>("elementPairs21");
    QTest::addColumn<ElementPairsView<void *>>("elementPairs2");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("unique");

    setElementPairsUnique_newRow<std::map, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );
    setElementPairsUnique_newRow<std::map, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 2, "2" }, { 3, "3" } }
    );

    setElementPairsUnique_newRow_qt<QMap, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 3, "3" } }
    );
    setElementPairsUnique_newRow_qt<QMap, int, QString>(
        { { 1, "1" }, { 2, "2" }, { 2, "2" }, { 3, "3" } }
    );

    setElementPairsUnique_newRow<std::multimap, int, QString>();

    setElementPairsUnique_newRow<std::deque, int>();
}

void test_RMetaContainer::setElementPairsUnique()
{
    QFETCH(const int, typeId);
    QFETCH(QVariant, container);

    QFETCH(const ElementPairsView<void *>, elementPairs1);
    QFETCH(const ElementPairsView<void *>, elementPairs2);
    QFETCH(const QVariant, value);
    QFETCH(const bool, unique);

    const RMetaContainer &metaContainer(*RMetaContainer::metaContainer(typeId));

    if (!metaContainer.isMultiple() && metaContainer.isDoubleDimensional()) {
        bool elementPairsUnique1(true);
        metaContainer.setElementPairs(
            container.data(), std::move(elementPairs1),
            &elementPairsUnique1
        );

        QCOMPARE(container, value);
        QCOMPARE(elementPairsUnique1, unique);

        bool elementPairsUnique2(false);
        metaContainer.setElementPairs(
            container.data(), std::move(elementPairs2),
            &elementPairsUnique2
        );

        QCOMPARE(container, value);
        QCOMPARE(elementPairsUnique2, unique);

        bool elementPairsUnique3(true);
        metaContainer.setElementPairs(
            container.data(), ElementPairsView<void *>(),
            &elementPairsUnique3
        );

        QCOMPARE(elementPairsUnique3, true);

        bool elementPairsUnique4(false);
        metaContainer.setElementPairs(
            container.data(), ElementPairsView<void *>(),
            &elementPairsUnique4
        );

        QCOMPARE(elementPairsUnique4, true);
    } else {
        bool elementPairsUnique1(true);
        metaContainer.setElementPairs(
            container.data(), std::move(elementPairs1),
            &elementPairsUnique1
        );

        QCOMPARE(container, value);
        QCOMPARE(elementPairsUnique1, true);

        bool elementPairsUnique2(false);
        metaContainer.setElementPairs(
            container.data(), std::move(elementPairs2),
            &elementPairsUnique2
        );

        QCOMPARE(container, value);
        QCOMPARE(elementPairsUnique2, false);
    }
}

template<
    template<typename ...> class T,
    typename V
>
struct ContainerWithElements<T, V, const void *>
{
    QVariant container;
    ElementsView<const void *> elements;

    static ContainerWithElements create(const T<V> &value);
};

template<
    template<typename ...> class T,
    typename V
>
auto ContainerWithElements<T, V, const void *>
  ::create(const T<V> &value)
 -> ContainerWithElements
{
    const QVariant container(QVariant::fromValue<T<V>>(value));
    const T<V> &container1(
        *static_cast<const T<V> *>(container.constData())
    );
    ElementsView<const void *> elements;
    elements.reserve(
        static_cast<ElementsView<const void *>::size_type>(
            container1.size()
        )
    );
    for (const auto &element : container1)
        elements.push_back(std::addressof(element));
    return ContainerWithElements{ container, elements };
}

template<
    template<typename ...> class T,
    typename V
>
struct ContainerWithElements<T, V, void *>
{
    QVariant container;
    ElementsView<void *> elements;

    static ContainerWithElements create(const std::vector<V> &value);
};

template<
    template<typename ...> class T,
    typename V
>
auto ContainerWithElements<T, V, void *>
  ::create(const std::vector<V> &value)
 -> ContainerWithElements
{
    QVariant container(QVariant::fromValue<std::vector<V>>(value));
    std::vector<V> &container1(
        *static_cast<std::vector<V> *>(container.data())
    );
    ElementsView<void *> elements;
    elements.reserve(
        static_cast<ElementsView<void *>::size_type>(
            container1.size()
        )
    );
    for (auto &element : container1)
        elements.push_back(std::addressof(element));
    return ContainerWithElements{ container, elements };
}

template<
    template<typename ...> class T,
    typename K, typename V
>
struct ContainerWithElements<T, K, V, const void *>
{
    QVariant container;
    ElementPairsView<const void *> elementPairs;

    static ContainerWithElements create(const T<K, V> &value);
};

template<
    template<typename ...> class T,
    typename K, typename V
>
auto ContainerWithElements<T, K, V, const void *>
  ::create(const T<K, V> &value)
 -> ContainerWithElements
{
    const QVariant container(QVariant::fromValue<T<K, V>>(value));
    const T<K, V> &container1(
        *static_cast<const T<K, V> *>(container.constData())
    );
    ElementPairsView<const void *> elementPairs;
    elementPairs.reserve(
        static_cast<ElementPairsView<const void *>::size_type>(
            container1.size()
        )
    );
    for (const auto &elementPair : container1) {
        elementPairs.push_back(
            std::pair<const void *, const void *>(
                static_cast<const void *>(std::addressof(elementPair.first)),
                static_cast<const void *>(std::addressof(elementPair.second))
            )
        );
    }
    return ContainerWithElements{ container, elementPairs };
}

template<
    typename K, typename V
>
struct ContainerWithElements<QMap, K, V, const void *>
{
    QVariant container;
    ElementPairsView<const void *> elementPairs;

    static ContainerWithElements create(const QMap<K, V> &value);
};

template<
    typename K, typename V
>
auto ContainerWithElements<QMap, K, V, const void *>
  ::create(const QMap<K, V> &value)
 -> ContainerWithElements
{
    const QVariant container(QVariant::fromValue<QMap<K, V>>(value));
    const QMap<K, V> &container1(
        *static_cast<const QMap<K, V> *>(container.constData())
    );
    ElementPairsView<const void *> elementPairs;
    elementPairs.reserve(
        static_cast<ElementPairsView<const void *>::size_type>(
            container1.size()
        )
    );
    auto i(container1.constBegin());
    auto end(container1.constEnd());
    while (i != end) {
        elementPairs.push_back(
            std::pair<const void *, const void *>(
                static_cast<const void *>(std::addressof(i.key())),
                static_cast<const void *>(std::addressof(i.value()))
            )
        );
        ++i;
    }
    return ContainerWithElements{ container, elementPairs };
}

template<
    template<typename ...> class T,
    typename K, typename V
>
struct ContainerWithElements<T, K, V, void *>
{
    QVariant container;
    ElementPairsView<void *> elementPairs;

    static ContainerWithElements create(const std::vector<std::pair<K, V>> &value);
};

template<
    template<typename ...> class T,
    typename K, typename V
>
auto ContainerWithElements<T, K, V, void *>
  ::create(const std::vector<std::pair<K, V>> &value)
 -> ContainerWithElements
{
    QVariant container(QVariant::fromValue<std::vector<std::pair<K, V>>>(value));
    std::vector<std::pair<K, V>> &container1(
        *static_cast<std::vector<std::pair<K, V>> *>(container.data())
    );
    ElementPairsView<void *> elementPairs;
    elementPairs.reserve(
        static_cast<ElementPairsView<void *>::size_type>(
            container1.size()
        )
    );
    for (auto &elementPair : container1) {
        elementPairs.push_back(
            std::pair<void *, void *>(
                static_cast<void *>(std::addressof(elementPair.first)),
                static_cast<void *>(std::addressof(elementPair.second))
            )
        );
    }
    return ContainerWithElements{ container, elementPairs };
}

QTEST_MAIN(test_RMetaContainer)

#include "test_rmetacontainer.moc"
