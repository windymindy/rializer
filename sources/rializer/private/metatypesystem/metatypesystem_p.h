#ifndef RIALIZER_METATYPESYSTEM_P_H
#define RIALIZER_METATYPESYSTEM_P_H

#include "../../datatypes_fd.h"
#include "../../metatypesystem/metadatatypes_fd.h"

class QMetaProperty;
class QVariant;

namespace Rializer {

class RMetaType;
class RMetaContainer;

class MetaTypeSystemPrivate
{
public:
    MetaTypeSystemPrivate() = default;

    Data *data;

    bool fromType(
        RMetaType &&metaType,
        Type::Type *returnType
    );

    bool fromValue(
        const Type::Type &type, const QVariant &value,
        Value::Value *returnValue
    );
    bool toValue(
        const Type::Type &type, Value::Value &&value,
        QVariant *returnValue
    );

    bool fromEnumerationType(
        RMetaType &&metaType,
        Type::Type *returnType
    );
    bool fromEnumerationValue(
        const Type::Type &type, const QVariant &value,
        Value::Value *returnValue
    );
    bool toEnumerationValue(
        const Type::Type &type, Value::Value &&value,
        QVariant *returnValue
    );

    bool fromCompositeType(
        RMetaType &&metaType,
        Type::Type *returnType
    );
    bool fromCompositeValue(
        const Type::Type &type, const QVariant &value,
        Value::Value *returnValue
    );
    bool toCompositeValue(
        const Type::Type &type, Value::Value &&value,
        QVariant *returnValue
    );

    bool fromSingleDimensionalContainerType(
        RMetaType &&metaType,
        Type::Type *returnType
    );
    bool fromSingleDimensionalContainerValue(
        const Type::Type &type, const QVariant &value,
        Value::Value *returnValue
    );
    bool toSingleDimensionalContainerValue(
        const Type::Type &type, Value::Value &&value,
        QVariant *returnValue
    );
    bool fromDoubleDimensionalContainerType(
        RMetaType &&metaType,
        Type::Type *returnType
    );
    bool fromDoubleDimensionalContainerValue(
        const Type::Type &type, const QVariant &value,
        Value::Value *returnValue
    );
    bool toDoubleDimensionalContainerValue(
        const Type::Type &type, Value::Value &&value,
        QVariant *returnValue
    );

    bool setError(Error &&value);

    bool typeIsNotSupported(
        const RMetaType &metaType,
        SourceLocation location
    );

    bool metaTypeIsNotRegistered(SourceLocation location);
    bool metaTypeIsNotRegistered(
        const RMetaType &metaType, const QMetaProperty &metaProperty,
        SourceLocation location
    );
    bool metaEnumerationIsNotRegistered(
        const RMetaType &metaType,
        SourceLocation location
    );

    bool valueIsNotOfEnumerationType1(
        const Type::Type &type,
        SourceLocation location
    );
    bool valueIsNotOfEnumerationType2(
        const Type::Type &type,
        SourceLocation location
    );

    const RMetaType *root1;
    const Type::Type *root2;
};

} // namespace Rializer

#endif // RIALIZER_METATYPESYSTEM_P_H
