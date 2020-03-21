#include <QtTest/QtTest>

#include <rializer/rializer>

#include <metatypesystem/RMetaContainer>

#include <deque>

using namespace Rializer;

class test_Rializer : public QObject
{
private Q_SLOTS:
    void initTestCase();

    void harness_data();
    void harness();

private:
    JsonSerializer jsonSerializer;
    JsonDeserializer jsonDeserializer;

private:
    Q_OBJECT
};

Q_DECLARE_METATYPE(Serializer *)
Q_DECLARE_METATYPE(Deserializer *)

Q_DECLARE_METATYPE_TEMPLATE_1ARG(std::deque)

struct MyStruct
{
    int number1 = 123;
    double number2 = 123.456;
    QString string1 = "123";
    std::deque<int> array1 = { 123, 456 };

    Q_GADGET
    Q_PROPERTY(int number1 MEMBER number1)
    Q_PROPERTY(double number2 MEMBER number2)
    Q_PROPERTY(QString string1 MEMBER string1)
    Q_PROPERTY(std::deque<int> array1 MEMBER array1)
};

bool operator==(const MyStruct &a, const MyStruct &b)
{
    return a.number1 == b.number1
     && qFuzzyIsNull(a.number2 - b.number2)
     && a.string1 == b.string1
     && a.array1 == b.array1;
}

Q_DECLARE_METATYPE(MyStruct)

QByteArray readTestDataFile(const QString &fileName);

QByteArray readTestDataFile(const QString &fileName)
{
    QFile file(QFINDTESTDATA(fileName));
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

void test_Rializer::initTestCase()
{
    RMetaContainer::registerMetaContainer<std::deque<int>>();

    QMetaType::registerEqualsComparator<MyStruct>();
}

void test_Rializer::harness_data()
{
    QTest::addColumn<Serializer *>("serializer");
    QTest::addColumn<Deserializer *>("deserializer");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QByteArray>("bytes");

    QTest::newRow("mystruct")
     << static_cast<Serializer *>(&jsonSerializer)
     << static_cast<Deserializer *>(&jsonDeserializer)
     << QVariant::fromValue<MyStruct>(MyStruct())
     << readTestDataFile("data/mystruct.json");
}

void test_Rializer::harness()
{
    QFETCH(Serializer * const, serializer);
    QFETCH(Deserializer * const, deserializer);
    QFETCH(const QVariant, value);
    QFETCH(const QByteArray, bytes);

    const RMetaType metaType(value.userType());

    QByteArray bytes1;
    bool result = serializer->serialize(metaType, value, &bytes1);
    if (!result)
        qWarning().noquote() << serializer->error().message.text();
    QCOMPARE(result, true);
    QCOMPARE(bytes1, bytes);

    QVariant value1;
    result = deserializer->deserialize(metaType, bytes, &value1);
    if (!result)
        qWarning().noquote() << deserializer->error().message.text();
    QCOMPARE(result, true);
    QCOMPARE(value1, value);
}

QTEST_MAIN(test_Rializer)

#include "test_rializer.moc"
