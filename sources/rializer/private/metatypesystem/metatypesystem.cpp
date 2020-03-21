#include "../../metatypesystem/metatypesystem.h"
#include "metatypesystem_p.h"

#include "../../datatypes.h"
#include "../../metatypesystem/metadatatypes.h"
#include "../../textmessages.h"

#include <QtCore/QScopeGuard>

namespace Rializer {

MetaTypeSystem::MetaTypeSystem()
  : d_ptr(new MetaTypeSystemPrivate())
{}

MetaTypeSystem::MetaTypeSystem(MetaTypeSystemPrivate &d)
  : d_ptr(&d)
{}

MetaTypeSystem::~MetaTypeSystem()
{}

void MetaTypeSystem::setData(Data * const data)
{
    Q_D(MetaTypeSystem);

    d->data = data;
}

Data & MetaTypeSystem::data()
{
    Q_D(const MetaTypeSystem);

    return *d->data;
}

bool MetaTypeSystem::fromType(
    const RMetaType &metaType,
    Type::Type * const returnType
)
{
    Q_D(MetaTypeSystem);

    if (!metaType.isValid())
        return d->metaTypeIsNotRegistered(RIALIZER_SOURCELOCATION);

    // Do not clutter up the routine with secondary parameters.
    d->root1 = &metaType;
    const auto cleanup(qScopeGuard([d]() { d->root1 = nullptr; }));

    return d->fromType(RMetaType(metaType), returnType);
}

bool MetaTypeSystem::fromValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    Q_D(MetaTypeSystem);

    d->root2 = &type;
    const auto cleanup(qScopeGuard([d]() { d->root2 = nullptr; }));

    return d->fromValue(type, value, returnValue);
}

bool MetaTypeSystem::toValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    Q_D(MetaTypeSystem);

    d->root2 = &type;
    const auto cleanup(qScopeGuard([d]() { d->root2 = nullptr; }));

    return d->toValue(type, std::move(value), returnValue);
}

bool MetaTypeSystemPrivate::fromType(
    RMetaType &&metaType,
    Type::Type * const returnType
)
{
    const int typeId(metaType.id());

    switch (static_cast<QMetaType::Type>(typeId)) {
    // QT_FOR_EACH_STATIC_TYPE

    // QT_FOR_EACH_STATIC_PRIMITIVE_TYPE
    case QMetaType::Bool:
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
    case QMetaType::Double:
    case QMetaType::Long:
    case QMetaType::Short:
    case QMetaType::Char:
    case QMetaType::ULong:
    case QMetaType::UShort:
    case QMetaType::UChar:
    case QMetaType::Float:
    case QMetaType::SChar:

    // case QMetaType::Void:
    // case QMetaType::Nullptr:

    // case QMetaType::QCborSimpleType:

    // Same as QMetaType::Double or QMetaType::Float.
    // case QMetaType::QReal:

    // QT_FOR_EACH_STATIC_PRIMITIVE_POINTER
    // case QMetaType::VoidStar:

    // QT_FOR_EACH_STATIC_CORE_CLASS
    case QMetaType::QChar:
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QBitArray:

    case QMetaType::QDate:
    case QMetaType::QTime:
    case QMetaType::QDateTime:

    case QMetaType::QUrl:
    case QMetaType::QUuid:

    // case QMetaType::QStringList:

    // case QMetaType::QLocale:

    // case QMetaType::QRect:
    // case QMetaType::QRectF:
    // case QMetaType::QSize:
    // case QMetaType::QSizeF:
    // case QMetaType::QLine:
    // case QMetaType::QLineF:
    // case QMetaType::QPoint:
    // case QMetaType::QPointF:
    // case QMetaType::QRegExp:
    // case QMetaType::QEasingCurve:

    // case QMetaType::QVariant:

    // case QMetaType::QRegularExpression:

    // case QMetaType::QJsonValue:
    // case QMetaType::QJsonObject:
    // case QMetaType::QJsonArray:
    // case QMetaType::QJsonDocument:
    // case QMetaType::QCborValue:
    // case QMetaType::QCborArray:
    // case QMetaType::QCborMap:

    // case QMetaType::QModelIndex:
    // case QMetaType::QPersistentModelIndex:

    // QT_FOR_EACH_STATIC_CORE_POINTER
    // case QMetaType::QObjectStar:

    // QT_FOR_EACH_STATIC_CORE_TEMPLATE
    // case QMetaType::QVariantMap:
    // case QMetaType::QVariantList:
    // case QMetaType::QVariantHash:
    // case QMetaType::QByteArrayList:

    // QT_FOR_EACH_STATIC_GUI_CLASS
    // case QMetaType::QFont:
    // case QMetaType::QPixmap:
    // case QMetaType::QBrush:
    // case QMetaType::QColor:
    // case QMetaType::QPalette:
    // case QMetaType::QIcon:
    // case QMetaType::QImage:
    // case QMetaType::QPolygon:
    // case QMetaType::QRegion:
    // case QMetaType::QBitmap:
    // case QMetaType::QCursor:
    // case QMetaType::QKeySequence:
    // case QMetaType::QPen:
    // case QMetaType::QTextLength:
    // case QMetaType::QTextFormat:
    // case QMetaType::QMatrix:
    // case QMetaType::QTransform:
    // case QMetaType::QMatrix4x4:
    // case QMetaType::QVector2D:
    // case QMetaType::QVector3D:
    // case QMetaType::QVector4D:
    // case QMetaType::QQuaternion:
    // case QMetaType::QPolygonF:

    // QT_FOR_EACH_STATIC_WIDGETS_CLASS
    // case QMetaType::QSizePolicy:

        *returnType = Type::Type(
            std::move(metaType), Type::Primitive::Primitive
        );
        return true;

    default: break;

    }

    const QMetaType::TypeFlags flags(metaType.flags());

    if (flags.testFlag(QMetaType::IsEnumeration))
        return fromEnumerationType(std::move(metaType), returnType);

    if (flags.testFlag(QMetaType::IsGadget))
        return fromCompositeType(std::move(metaType), returnType);

    if (RMetaContainer::isContainer(typeId)) {
        const RMetaContainer &metaContainer(
            *RMetaContainer::metaContainer(typeId)
        );

        if (metaContainer.isSingleDimensional()) {
            return fromSingleDimensionalContainerType(
                std::move(metaType), returnType
            );
        }

        if (metaContainer.isDoubleDimensional()) {
            return fromDoubleDimensionalContainerType(
                std::move(metaType), returnType
            );
        }
    }

    return typeIsNotSupported(metaType, RIALIZER_SOURCELOCATION);
}

bool MetaTypeSystemPrivate::fromValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    switch (type.description) {
    case Type::Description::Primitive:
        *returnValue = Value::Value(
            QVariant(value), Value::Primitive::Primitive
        );
        return true;
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
            return fromDoubleDimensionalContainerValue(
                type, value, returnValue
            );
        }
        break;
    default: break;
    }

    return false;
}

bool MetaTypeSystemPrivate::toValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    switch (type.description) {
    case Type::Description::Primitive:
        *returnValue = std::move(value.primitive());
        return true;
    case Type::Description::Enumeration:
        return toEnumerationValue(type, std::move(value), returnValue);
    case Type::Description::Composite:
        return toCompositeValue(type, std::move(value), returnValue);
    case Type::Description::Container:
        if (type.container().isSingleDimensional()) {
            return toSingleDimensionalContainerValue(
                type, std::move(value), returnValue
            );
        } else if (type.container().isDoubleDimensional()) {
            return toDoubleDimensionalContainerValue(
                type, std::move(value), returnValue
            );
        }
        break;
    default: break;
    }

    return false;
}

bool MetaTypeSystemPrivate::fromEnumerationType(
    RMetaType &&metaType,
    Type::Type * const returnType
)
{
    // Why does QMetaObject::metaObject return by pointer to const,
    // whereas QMetaObject::enumerator and QMetaObject::property
    // return by value?
    const QMetaObject * const metaObject(metaType.metaObject());
    if (metaObject == nullptr)
        return metaEnumerationIsNotRegistered(metaType, RIALIZER_SOURCELOCATION);

    // Why can not search by typeId?
    const int i(metaObject->indexOfEnumerator(metaType.name()));
    QMetaEnum metaEnumeration(metaObject->enumerator(i));

    *returnType = Type::Type(
        std::move(metaType),
        Type::Enumeration(
            std::move(metaEnumeration)
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::fromEnumerationValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    const QMetaEnum &metaEnumeration(type.enumeration().metaEnumeration);

    bool ok(false);
    int enumeration(value.toInt(&ok));
    if (!ok)
        return false;

    if (metaEnumeration.valueToKey(enumeration) == nullptr)
        return valueIsNotOfEnumerationType1(type, RIALIZER_SOURCELOCATION);

    *returnValue = Value::Value(
        Value::Enumeration(
            enumeration
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::toEnumerationValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    const Type::Enumeration &enumerationType(type.enumeration());
    Value::Enumeration &enumerationValue(value.enumeration());
    const QMetaEnum &metaEnumeration(enumerationType.metaEnumeration);

    if (enumerationValue.isNumber()) {
        const int number(enumerationValue.number());

        if (metaEnumeration.valueToKey(number) == nullptr)
            return valueIsNotOfEnumerationType2(type, RIALIZER_SOURCELOCATION);

        QVariant enumeration(number);
        if (!enumeration.convert(type.metaType.id()))
            return false;

        *returnValue = std::move(enumeration);
        return true;
    }

    if (enumerationValue.isString()) {
        const QByteArray &string(enumerationValue.string());

        bool ok(false);
        const int number(
            metaEnumeration.keyToValue(string, &ok)
        );
        if (!ok)
            return valueIsNotOfEnumerationType2(type, RIALIZER_SOURCELOCATION);

        QVariant enumeration(number);
        if (!enumeration.convert(type.metaType.id()))
            return false;

        *returnValue = std::move(enumeration);
        return true;
    }

    return false;
}

bool MetaTypeSystemPrivate::fromCompositeType(
    RMetaType &&metaType,
    Type::Type * const returnType
)
{
    using SizeType = Type::Properties::size_type;

    const QMetaObject &metaObject(*metaType.metaObject());

    const SizeType propertiesSize(
        static_cast<SizeType>(metaObject.propertyCount())
    );
    Type::Properties properties;
    properties.reserve(propertiesSize);
    for (SizeType i(0); i < propertiesSize; ++i) {
        QMetaProperty metaProperty(
            metaObject.property(static_cast<int>(i))
        );

        const bool isSerializable(
            data->mode.serialize
             && metaProperty.isReadable()
        );
        const bool isDeserializable(
            data->mode.deserialize
              && metaProperty.isWritable()
        );
        if (!(isSerializable || isDeserializable))
            continue;

        // todo (Qt 6): metaProperty.metaType()
        // will it always be valid and registered?
        const int propertyTypeId(metaProperty.userType());
        if (propertyTypeId == static_cast<int>(QMetaType::UnknownType))
            return metaTypeIsNotRegistered(metaType, metaProperty, RIALIZER_SOURCELOCATION);

        RMetaType propertyMetaType(propertyTypeId);

        Type::Type propertyType;
        if (!fromType(std::move(propertyMetaType), &propertyType))
            return false;

        properties.push_back(
            Type::Property(
                std::move(metaProperty),
                std::move(propertyType)
            )
        );
    }

    *returnType = Type::Type(
        std::move(metaType),
        Type::Composite(
            &metaObject,
            std::move(properties)
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::fromCompositeValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    using SizeType = Type::Properties::size_type;

    const Type::Composite &compositeType(type.composite());

    const SizeType propertiesSize(compositeType.properties.size());
    Value::Properties compositeValueProperties;
    compositeValueProperties.reserve(propertiesSize);
    for (SizeType i(0); i < propertiesSize; ++i) {
        const Type::Property &compositeTypeProperty(
            compositeType.properties.at(i)
        );
        const QMetaProperty &metaProperty(compositeTypeProperty.metaProperty);

        const QVariant property(
            metaProperty.readOnGadget(value.constData())
        );
        if (!property.isValid())
            return false;

        const Type::Type &propertyType(compositeTypeProperty.type);

        Value::Value propertyValue;
        if (!fromValue(propertyType, property, &propertyValue))
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

bool MetaTypeSystemPrivate::toCompositeValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    using SizeType = Type::Properties::size_type;

    const Type::Composite &compositeType(type.composite());
    Value::Composite &compositeValue(value.composite());

    QVariant composite(makeVariant(type.metaType));

    const SizeType propertiesSize(compositeType.properties.size());
    for (SizeType i(0); i < propertiesSize; ++i) {
        const Type::Property &compositeTypeProperty(
            compositeType.properties.at(i)
        );
        const Type::Type &propertyType(compositeTypeProperty.type);
        Value::Value &propertyValue(
            compositeValue.properties.at(i)
        );

        QVariant property;
        if (!toValue(propertyType, std::move(propertyValue), &property))
            return false;

        const QMetaProperty &metaProperty(compositeTypeProperty.metaProperty);

        if (!metaProperty.writeOnGadget(composite.data(), property))
            return false;
    }

    *returnValue = std::move(composite);
    return true;
}

bool MetaTypeSystemPrivate::fromSingleDimensionalContainerType(
    RMetaType &&metaType,
    Type::Type * const returnType
)
{
    const RMetaContainer &metaContainer(
        *RMetaContainer::metaContainer(metaType.id())
    );
    RMetaType elementMetaType(metaContainer.elementMetaType());

    Type::Type elementType;
    if (!fromType(std::move(elementMetaType), &elementType))
        return false;

    *returnType = Type::Type(
        std::move(metaType),
        Type::Container::Container(
            &metaContainer,
            Type::Container::SingleDimensional(
                std::move(elementType)
            )
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::fromSingleDimensionalContainerValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    using Rializer::Container::ElementsView;

    const Type::Container::SingleDimensional &containerType(
        type.container().singleDimensional()
    );
    const RMetaContainer &metaContainer(*type.container().metaContainer);
    const Type::Type &elementType(containerType.elementType);

    ElementsView<const void *> elements(
        metaContainer.elements(value.constData())
    );
    Value::Container::Elements containerValueElements;
    containerValueElements.reserve(elements.size());
    for (const auto &element1 : elements) {
        const QVariant element(
            makeVariant(elementType.metaType, element1)
        );
        Value::Value elementValue;
        if (!fromValue(elementType, element, &elementValue))
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

bool MetaTypeSystemPrivate::toSingleDimensionalContainerValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    using Rializer::Container::ElementsView;

    const Type::Container::SingleDimensional &containerType(
        type.container().singleDimensional()
    );
    Value::Container::SingleDimensional &containerValue(
        value.container().singleDimensional()
    );
    const Type::Type &elementType(containerType.elementType);

    std::vector<QVariant> elements1;
    ElementsView<void *> elements;
    elements1.reserve(containerValue.elements.size());
    elements.reserve(containerValue.elements.size());
    for (auto &elementValue : containerValue.elements) {
        QVariant element;
        if (!toValue(elementType, std::move(elementValue), &element))
            return false;

        elements1.push_back(std::move(element));
        elements.push_back(elements1.back().data());
    }

    const RMetaContainer &metaContainer(*type.container().metaContainer);

    QVariant container(makeVariant(type.metaType));
    metaContainer.setElements(container.data(), std::move(elements));

    *returnValue = std::move(container);
    return true;
}

bool MetaTypeSystemPrivate::fromDoubleDimensionalContainerType(
    RMetaType &&metaType,
    Type::Type * const returnType
)
{
    const RMetaContainer &metaContainer(
        *RMetaContainer::metaContainer(metaType.id())
    );
    RMetaType elementKeyMetaType(metaContainer.elementKeyMetaType());

    Type::Type elementKeyType;
    if (!fromType(std::move(elementKeyMetaType), &elementKeyType))
        return false;

    RMetaType elementValueMetaType(metaContainer.elementValueMetaType());

    Type::Type elementValueType;
    if (!fromType(std::move(elementValueMetaType), &elementValueType))
        return false;

    *returnType = Type::Type(
        std::move(metaType),
        Type::Container::Container(
            &metaContainer,
            Type::Container::DoubleDimensional(
                std::move(elementKeyType), std::move(elementValueType)
            )
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::fromDoubleDimensionalContainerValue(
    const Type::Type &type, const QVariant &value,
    Value::Value * const returnValue
)
{
    using Rializer::Container::ElementPairsView;

    const Type::Container::DoubleDimensional &containerType(
        type.container().doubleDimensional()
    );
    const RMetaContainer &metaContainer(*type.container().metaContainer);
    const Type::Type &elementKeyType(containerType.elementKeyType);
    const Type::Type &elementValueType(containerType.elementValueType);

    ElementPairsView<const void *> elementPairs(
        metaContainer.elementPairs(value.constData())
    );
    Value::Container::ElementPairs containerValueElementPairs;
    containerValueElementPairs.reserve(elementPairs.size());
    for (const auto &elementPair : elementPairs) {
        const QVariant elementKey(
            makeVariant(elementKeyType.metaType, elementPair.first)
        );
        Value::Value elementKeyValue;
        if (!fromValue(elementKeyType, elementKey, &elementKeyValue))
            return false;

        const QVariant elementValue(
            makeVariant(elementValueType.metaType, elementPair.second)
        );
        Value::Value elementValueValue;
        if (!fromValue(elementValueType, elementValue, &elementValueValue))
            return false;

        containerValueElementPairs.push_back(
            std::pair<Value::Value, Value::Value>(
                std::move(elementKeyValue), std::move(elementValueValue)
            )
        );
    }

    *returnValue = Value::Value(
        Value::Container::Container(
            Value::Container::DoubleDimensional(
                std::move(containerValueElementPairs)
            )
        )
    );
    return true;
}

bool MetaTypeSystemPrivate::toDoubleDimensionalContainerValue(
    const Type::Type &type, Value::Value &&value,
    QVariant * const returnValue
)
{
    using Rializer::Container::ElementPairsView;

    const Type::Container::DoubleDimensional &containerType(
        type.container().doubleDimensional()
    );
    Value::Container::DoubleDimensional &containerValue(
        value.container().doubleDimensional()
    );
    const Type::Type &elementKeyType(containerType.elementKeyType);
    const Type::Type &elementValueType(containerType.elementValueType);

    std::vector<QVariant> elementKeys;
    std::vector<QVariant> elementValues;
    ElementPairsView<void *> elementPairs;
    elementKeys.reserve(containerValue.elementPairs.size());
    elementValues.reserve(containerValue.elementPairs.size());
    elementPairs.reserve(containerValue.elementPairs.size());
    for (auto &containerValueElementPair : containerValue.elementPairs) {
        Value::Value &elementKeyValue(containerValueElementPair.first);

        QVariant elementKey;
        if (!toValue(elementKeyType, std::move(elementKeyValue), &elementKey))
            return false;

        Value::Value &elementValueValue(containerValueElementPair.first);

        QVariant elementValue;
        if (!toValue(elementValueType, std::move(elementValueValue), &elementValue))
            return false;

        elementKeys.push_back(std::move(elementKey));
        elementValues.push_back(std::move(elementValue));
        elementPairs.push_back(
            std::pair<void *, void *>(
                elementKeys.back().data(), elementValues.back().data()
            )
        );
    }

    const RMetaContainer &metaContainer(*type.container().metaContainer);

    QVariant container(makeVariant(type.metaType));
    metaContainer.setElementPairs(container.data(), std::move(elementPairs));

    *returnValue = std::move(container);
    return true;
}

bool MetaTypeSystemPrivate::setError(Error &&value)
{
    data->error = std::move(value);
    return false;
}

bool MetaTypeSystemPrivate::typeIsNotSupported(
    const RMetaType &metaType,
    const SourceLocation location
)
{
    if (data->mode.serialize) {
        using TextMessages::Serializer::MetaTypeSystem
          ::typeIsNotSupported;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    typeIsNotSupported::id,
                    typeIsNotSupported::text,
                    {
                        root1->name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    if (data->mode.deserialize) {
        using TextMessages::Deserializer::MetaTypeSystem
          ::typeIsNotSupported;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    typeIsNotSupported::id,
                    typeIsNotSupported::text,
                    {
                        root1->name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    return false;
}

bool MetaTypeSystemPrivate::metaTypeIsNotRegistered(
    const SourceLocation location
)
{
    if (data->mode.serialize) {
        using TextMessages::Serializer::MetaTypeSystem
          ::metaTypeIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    metaTypeIsNotRegistered::id,
                    metaTypeIsNotRegistered::text,
                    { "unknown" },
                    location
                )
            )
        );
    }

    if (data->mode.deserialize) {
        using TextMessages::Deserializer::MetaTypeSystem
          ::metaTypeIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    metaTypeIsNotRegistered::id,
                    metaTypeIsNotRegistered::text,
                    { "unknown" },
                    location
                )
            )
        );
    }

    return false;
}

bool MetaTypeSystemPrivate::metaTypeIsNotRegistered(
    const RMetaType &metaType, const QMetaProperty &metaProperty,
    const SourceLocation location
)
{
    if (data->mode.serialize) {
        using TextMessages::Serializer::MetaTypeSystem
          ::propertyMetaTypeIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    propertyMetaTypeIsNotRegistered::id,
                    propertyMetaTypeIsNotRegistered::text,
                    {
                        root1->name(),
                        metaProperty.typeName(),
                        metaProperty.name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    if (data->mode.deserialize) {
        using TextMessages::Deserializer::MetaTypeSystem
          ::propertyMetaTypeIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    propertyMetaTypeIsNotRegistered::id,
                    propertyMetaTypeIsNotRegistered::text,
                    {
                        root1->name(),
                        metaProperty.typeName(),
                        metaProperty.name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    return false;
}

bool MetaTypeSystemPrivate::metaEnumerationIsNotRegistered(
    const RMetaType &metaType,
    const SourceLocation location
)
{
    if (data->mode.serialize) {
        using TextMessages::Serializer::MetaTypeSystem
          ::metaEnumerationIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    metaEnumerationIsNotRegistered::id,
                    metaEnumerationIsNotRegistered::text,
                    {
                        root1->name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    if (data->mode.deserialize) {
        using TextMessages::Deserializer::MetaTypeSystem
          ::metaEnumerationIsNotRegistered;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    metaEnumerationIsNotRegistered::id,
                    metaEnumerationIsNotRegistered::text,
                    {
                        root1->name(),
                        metaType.name()
                    },
                    location
                )
            )
        );
    }

    return false;
}

bool MetaTypeSystemPrivate::valueIsNotOfEnumerationType1(
    const Type::Type &type,
    const SourceLocation location
)
{
    using TextMessages::Serializer::MetaTypeSystem
      ::valueIsNotOfEnumerationType;

    return setError(
        Error(
            Reason::ValueIsNotValid,
            TextMessage(
                valueIsNotOfEnumerationType::id,
                valueIsNotOfEnumerationType::text,
                {
                    root2->metaType.name(),
                    type.metaType.name()
                },
                location
            )
        )
    );
}

bool MetaTypeSystemPrivate::valueIsNotOfEnumerationType2(
    const Type::Type &type,
    const SourceLocation location
)
{
    using TextMessages::Deserializer::MetaTypeSystem
      ::valueIsNotOfEnumerationType;

    return setError(
        Error(
            Reason::ValueIsNotValid,
            TextMessage(
                valueIsNotOfEnumerationType::id,
                valueIsNotOfEnumerationType::text,
                {
                    root2->metaType.name(),
                    type.metaType.name()
                },
                location
            )
        )
    );
}

} // namespace Rializer
