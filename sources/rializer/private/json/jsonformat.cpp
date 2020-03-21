#include "../../json/jsonformat.h"
#include "jsonformat_p.h"

#include "../../datatypes.h"
#include "../../metatypesystem/metadatatypes.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QUuid>
#include <QtCore/QScopeGuard>

namespace Rializer {

JsonFormat::JsonFormat()
  : AbstractFormat()
{}

JsonFormat::JsonFormat(JsonFormatPrivate &d)
  : AbstractFormat(d)
{}

JsonFormat::~JsonFormat() {}

bool JsonFormat::serialize(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const document
)
{
    Q_D(JsonFormat);

    d->root = &type;
    const auto cleanup(qScopeGuard([d]() { d->root = nullptr; }));

    return d->fromValue(type, value, document);
}

bool JsonFormat::deserialize(
    const Type::Type &type, const QJsonValue &document,
    Value::Value * const value
)
{
    Q_D(JsonFormat);

    d->root = &type;
    const auto cleanup(qScopeGuard([d]() { d->root = nullptr; }));

    return d->toValue(type, document, value);
}

bool JsonFormat::serialize(
    const Type::Type &type, const Value::Value &value,
    QByteArray * const bytes
)
{
    Q_D(JsonFormat);

    d->root = &type;
    const auto cleanup(qScopeGuard([d]() { d->root = nullptr; }));

    QJsonValue document1;
    if (!d->fromValue(type, value, &document1))
        return false;

    const QJsonDocument::JsonFormat format(
        !d->data->options.noHumanReadableFormat
      ? QJsonDocument::Indented
      : QJsonDocument::Compact
    );

    if (document1.isObject()) {
        QJsonDocument document(document1.toObject());
        *bytes = document.toJson(format);
        return true;
    }

    if (document1.isArray()) {
        QJsonDocument document(document1.toArray());
        *bytes = document.toJson(format);
        return true;
    }

    QJsonArray document2;
    document2.append(document1);
    QJsonDocument document(document2);
    const int prefixSize(
        !d->data->options.noHumanReadableFormat
      ? 6 : 1
    );
    const int postfixSize(
        !d->data->options.noHumanReadableFormat
      ? 2 : 1
    );
    *bytes = document.toJson(format);
    bytes->remove(0, prefixSize);
    bytes->chop(postfixSize);
    return true;
}

bool JsonFormat::deserialize(
    const Type::Type &type, const QByteArray &bytes,
    Value::Value * const value
)
{
    Q_D(JsonFormat);

    d->root = &type;
    const auto cleanup(qScopeGuard([d]() { d->root = nullptr; }));

    QByteArray document2(bytes);
    document2.prepend("[");
    document2.append("]");
    QJsonParseError parseError;
    QJsonDocument document(QJsonDocument::fromJson(document2, &parseError));
    if (parseError.error != QJsonParseError::NoError)
        return d->representationIsNotValid(parseError, RIALIZER_SOURCELOCATION);

    if (!document.isArray())
        return false;

    const QJsonValue document1(document.array().first());

    return d->toValue(type, document1, value);
}

bool JsonFormatPrivate::fromValue(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const returnValue
)
{
    switch (type.description) {
    case Type::Description::Primitive:
        return fromPrimitiveValue(type, value, returnValue);
    case Type::Description::Enumeration:
        return fromEnumerationValue(type, value, returnValue);
    case Type::Description::Composite:
        return fromCompositeValue(type, value, returnValue);
    case Type::Description::Container:
        if (type.container().isSingleDimensional()) {
            return fromSingleDimensionalContainerValue(
                type, value, returnValue
            );
        } else if (type.container().isDoubleDimensional()) {
            return typeIsNotSupported(type, RIALIZER_SOURCELOCATION);
        }
        break;
    default: break;
    }

    return false;
}

bool JsonFormatPrivate::toValue(
    const Type::Type &type, const QJsonValue &value,
    Value::Value * const returnValue
)
{
    switch (type.description) {
    case Type::Description::Primitive:
        return toPrimitiveValue(type, value, returnValue);
    case Type::Description::Enumeration:
        return toEnumerationValue(type, value, returnValue);
    case Type::Description::Composite:
        return toCompositeValue(type, value, returnValue);
    case Type::Description::Container:
        if (type.container().isSingleDimensional()) {
            return toSingleDimensionalContainerValue(
                type, value, returnValue
            );
        } else if (type.container().isDoubleDimensional()) {
            return typeIsNotSupported(type, RIALIZER_SOURCELOCATION);
        }
        break;
    default: break;
    }

    return false;
}

bool JsonFormatPrivate::fromPrimitiveValue(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const returnValue
)
{
    const QVariant &primitive(value.primitive());

    // Almost every conversion is already implemented in QVariant::convert.
    // But it is nice to have them explicitly restated.
    // This enables precise control over the conversion.
    // And there are no static guarantees when converting
    // through the meta type system.
    switch (static_cast<QMetaType::Type>(type.metaType.id())) {
    case QMetaType::Bool: {
        const bool &boolean(variantValue<const bool>(primitive));

        *returnValue = QJsonValue(boolean);
        return true;
    }
    case QMetaType::Int: {
        double number(0.0);
        if (!fromNumberValue<int>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::UInt: {
        double number(0.0);
        if (!fromNumberValue<unsigned int>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    // case QMetaType::LongLong:
    // case QMetaType::ULongLong:
    case QMetaType::Double: {
        const double &number(variantValue<const double>(primitive));

        *returnValue = QJsonValue(number);
        return true;
    }
    // case QMetaType::Long:
    case QMetaType::Short: {
        double number(0.0);
        if (!fromNumberValue<short>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::Char: {
        double number(0.0);
        if (!fromNumberValue<char>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    // case QMetaType::ULong:
    case QMetaType::UShort: {
        double number(0.0);
        if (!fromNumberValue<unsigned short>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::UChar: {
        double number(0.0);
        if (!fromNumberValue<unsigned char>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::Float: {
        double number(0.0);
        if (!fromNumberValue<float>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::SChar: {
        double number(0.0);
        if (!fromNumberValue<signed char>(type, primitive, &number))
            return false;

        *returnValue = QJsonValue(number);
        return true;
    }
    case QMetaType::QChar: {
        const QChar &character(variantValue<const QChar>(primitive));
        const QString string(character);

        *returnValue = QJsonValue(string);
        return true;
    }
    case QMetaType::QString: {
        const QString &string(variantValue<const QString>(primitive));

        *returnValue = QJsonValue(string);
        return true;
    }
    // case QMetaType::QByteArray:
    // case QMetaType::QBitArray:
    // todo (review): QT_CONFIG(datestring)???
    case QMetaType::QDate: {
        const QDate &date(variantValue<const QDate>(primitive));
        const QString string(date.toString(Qt::ISODate));

        *returnValue = QJsonValue(string);
        return true;
    }
    case QMetaType::QTime: {
        const QTime &time(variantValue<const QTime>(primitive));
        const QString string(time.toString(Qt::ISODateWithMs));

        *returnValue = QJsonValue(string);
        return true;
    }
    case QMetaType::QDateTime: {
        const QDateTime &datetime(variantValue<const QDateTime>(primitive));
        const QString string(datetime.toString(Qt::ISODateWithMs));

        *returnValue = QJsonValue(string);
        return true;
    }
    case QMetaType::QUrl: {
        const QUrl &url(variantValue<const QUrl>(primitive));
        const QString string(url.toString());

        *returnValue = QJsonValue(string);
        return true;
    }
    case QMetaType::QUuid: {
        const QUuid &uuid(variantValue<const QUuid>(primitive));
        const QString string(uuid.toString(QUuid::WithoutBraces));

        *returnValue = QJsonValue(string);
        return true;
    }
    default: break;
    }

    return typeIsNotSupported(type, RIALIZER_SOURCELOCATION);
}

template<typename T>
bool JsonFormatPrivate::fromNumberValue(
    const Type::Type &type, const QVariant &primitive,
    double * const returnValue
)
{
    Q_UNUSED(type)

    const T &number1(variantValue<const T>(primitive));
    const double number(static_cast<double>(number1));

    *returnValue = number;
    return true;
}

bool JsonFormatPrivate::toPrimitiveValue(
    const Type::Type &type, const QJsonValue &value,
    Value::Value * const returnValue
)
{
    switch (static_cast<QMetaType::Type>(type.metaType.id())) {
    case QMetaType::Bool: {
        if (!value.isBool())
            return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

        const bool boolean(value.toBool());

        *returnValue = Value::Value(
            QVariant(boolean), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::Int: {
        int number(0);
        if (!toNumberValue<int>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::UInt: {
        unsigned int number(0);
        if (!toNumberValue<unsigned int>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant(number), Value::Primitive::Primitive
        );
        return true;
    }
    // case QMetaType::LongLong:
    // case QMetaType::ULongLong:
    case QMetaType::Double: {
        if (!value.isDouble())
            return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

        const double number(value.toDouble());

        *returnValue = Value::Value(
            QVariant(number), Value::Primitive::Primitive
        );
        return true;
    }
    // case QMetaType::Long:
    case QMetaType::Short: {
        short number(0);
        if (!toNumberValue<short>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant::fromValue<short>(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::Char: {
        char number(0);
        if (!toNumberValue<char>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant::fromValue<char>(number), Value::Primitive::Primitive
        );
        return true;
    }
    // case QMetaType::ULong:
    case QMetaType::UShort: {
        unsigned short number(0);
        if (!toNumberValue<unsigned short>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant::fromValue<unsigned short>(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::UChar: {
        unsigned char number(0);
        if (!toNumberValue<unsigned char>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant::fromValue<unsigned char>(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::Float: {
        float number(0.0);
        if (!toNumberValue<float>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::SChar: {
        signed char number(0);
        if (!toNumberValue<signed char>(type, value, &number))
            return false;

        *returnValue = Value::Value(
            QVariant::fromValue<signed char>(number), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QChar: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        if (string.size() != 1)
            return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

        const QChar character(string.front());

        *returnValue = Value::Value(
            QVariant(character), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QString: {
    // case QMetaType::QByteArray:
    // case QMetaType::QBitArray:
        if (!value.isString())
            return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

        QString string(value.toString());

        *returnValue = Value::Value(
            QVariant(string), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QDate: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        QDate date;
        if (!string.isEmpty()) {
            date = QDate::fromString(string, Qt::ISODate);
            if (date.isNull())
                return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);
        }

        *returnValue = Value::Value(
            QVariant::fromValue<QDate>(date), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QTime: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        QTime time;
        if (!string.isEmpty()) {
            time = QTime::fromString(string, Qt::ISODateWithMs);
            if (time.isNull())
                return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);
        }

        *returnValue = Value::Value(
            QVariant::fromValue<QTime>(time), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QDateTime: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        QDateTime dateTime;
        if (!string.isEmpty()) {
            dateTime = QDateTime::fromString(string, Qt::ISODateWithMs);
            if (dateTime.isNull())
                return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);
        }

        *returnValue = Value::Value(
            QVariant::fromValue<QDateTime>(dateTime), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QUrl: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        QUrl url;
        if (!string.isEmpty()) {
            url = QUrl(string);
            if (url.isValid())
                return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);
        }

        *returnValue = Value::Value(
            QVariant::fromValue<QUrl>(url), Value::Primitive::Primitive
        );
        return true;
    }
    case QMetaType::QUuid: {
        QString string;
        if (!toStringValue(type, value, &string))
            return false;

        QUuid uuid;
        if (!string.isEmpty()) {
            uuid = QUuid::fromString(string);
            if (uuid.isNull())
                return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);
        }

        *returnValue = Value::Value(
            QVariant::fromValue<QUuid>(uuid), Value::Primitive::Primitive
        );
        return true;
    }
    default: break;
    }

    return typeIsNotSupported(type, RIALIZER_SOURCELOCATION);
}

template<typename T>
bool JsonFormatPrivate::toNumberValue(
    const Type::Type &type, const QJsonValue &value,
    T * const returnValue
)
{
    if (!value.isDouble())
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    const double number1(value.toDouble());
    const T number(static_cast<T>(number1));
    const double number2(static_cast<double>(number));
    if (!qFuzzyIsNull(number1 - number2))
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    *returnValue = number;
    return true;
}

bool JsonFormatPrivate::toStringValue(
    const Type::Type &type, const QJsonValue &value,
    QString * const returnValue
)
{
    if (!value.isString())
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    *returnValue = value.toString();
    return true;
}

bool JsonFormatPrivate::fromEnumerationValue(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const returnValue
)
{
    const Type::Enumeration &enumerationType(type.enumeration());
    const Value::Enumeration &enumerationValue(value.enumeration());

    if (!enumerationValue.isNumber())
        return false;

    const QByteArray string(enumerationValue.toString(enumerationType));

    *returnValue = QJsonValue(QString::fromUtf8(string));
    return true;
}

bool JsonFormatPrivate::toEnumerationValue(
    const Type::Type &type, const QJsonValue &value,
    Value::Value * const returnValue
)
{
    if (!value.isString())
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    const QString string(value.toString());

    *returnValue = Value::Value(
        Value::Enumeration(
            string.toUtf8()
        )
    );
    return true;
}

bool JsonFormatPrivate::fromCompositeValue(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const returnValue
)
{
    using SizeType = Type::Properties::size_type;

    const Type::Composite &compositeType(type.composite());
    const Value::Composite &compositeValue(value.composite());

    QJsonObject composite;

    const SizeType propertiesSize(compositeType.properties.size());
    for (SizeType i(0); i < propertiesSize; ++i) {
        const Type::Property &compositeTypeProperty(
            compositeType.properties.at(i)
        );
        const Type::Type &propertyType(compositeTypeProperty.type);
        const Value::Value &propertyValue(
            compositeValue.properties.at(i)
        );

        QJsonValue property;
        if (!fromValue(propertyType, propertyValue, &property))
            return false;

        const QMetaProperty &metaProperty(compositeTypeProperty.metaProperty);
        const QString propertyName(QString::fromUtf8(metaProperty.name()));

        composite.insert(propertyName, property);
    }

    *returnValue = QJsonValue(composite);
    return true;
}

bool JsonFormatPrivate::toCompositeValue(
    const Type::Type &type, const QJsonValue &value,
    Value::Value * const returnValue
)
{
    using SizeType = Type::Properties::size_type;

    if (!value.isObject())
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    const Type::Composite &compositeType(type.composite());

    const QJsonObject composite(value.toObject());

    const SizeType propertiesSize(compositeType.properties.size());
    if (propertiesSize != static_cast<SizeType>(composite.size()))
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    Value::Properties compositeValueProperties;
    compositeValueProperties.reserve(propertiesSize);
    for (SizeType i(0); i < propertiesSize; ++i) {
        const Type::Property &compositeTypeProperty(
            compositeType.properties.at(i)
        );
        const QMetaProperty &metaProperty(compositeTypeProperty.metaProperty);
        const QString propertyName(QString::fromUtf8(metaProperty.name()));
        if (!composite.contains(propertyName))
            return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

        const QJsonValue property(composite.value(propertyName));

        const Type::Type &propertyType(compositeTypeProperty.type);

        Value::Value propertyValue;
        if (!toValue(propertyType, property, &propertyValue))
            return false;

        compositeValueProperties.push_back(std::move(propertyValue));
    }

    *returnValue = Value::Value(
        Value::Composite(
            std::move(compositeValueProperties)
        )
    );
    return true;
}

bool JsonFormatPrivate::fromSingleDimensionalContainerValue(
    const Type::Type &type, const Value::Value &value,
    QJsonValue * const returnValue
)
{
    const Type::Container::SingleDimensional &containerType(
        type.container().singleDimensional()
    );
    const Value::Container::SingleDimensional &containerValue(
        value.container().singleDimensional()
    );
    const Type::Type &elementType(containerType.elementType);

    QJsonArray array;

    for (const auto &elementValue : containerValue.elements) {
        QJsonValue element;
        if (!fromValue(elementType, elementValue, &element))
            return false;

        array.append(element);
    }

    *returnValue = QJsonValue(array);
    return true;
}

bool JsonFormatPrivate::toSingleDimensionalContainerValue(
    const Type::Type &type, const QJsonValue &value,
    Value::Value * const returnValue
)
{
    using SizeType = Type::Properties::size_type;

    if (!value.isArray())
        return representationIsNotOfType(type, RIALIZER_SOURCELOCATION);

    const Type::Container::SingleDimensional &containerType(
        type.container().singleDimensional()
    );
    const Type::Type &elementType(containerType.elementType);

    const QJsonArray array(value.toArray());

    Value::Container::Elements containerValueElements;
    containerValueElements.reserve(static_cast<SizeType>(array.size()));
    for (const auto &element : array) {
        Value::Value elementValue;
        if (!toValue(elementType, element, &elementValue))
            return false;

        containerValueElements.push_back(std::move(elementValue));
    }

    *returnValue = Value::Value(
        Value::Container::Container(
            Value::Container::SingleDimensional(
                std::move(containerValueElements)
            )
        )
    );
    return true;
}

bool JsonFormatPrivate::representationIsNotValid(
    const QJsonParseError parseError,
    const SourceLocation location
)
{
    // todo: create a separate text message id for every json parse error
    return representationIsNotValid(parseError.errorString(), location);
}
} // namespace Rializer
