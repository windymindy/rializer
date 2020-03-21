#ifndef RIALIZER_JSONFORMAT_P_H
#define RIALIZER_JSONFORMAT_P_H

#include "../abstractformat_p.h"

class QJsonValue;
struct QJsonParseError;
class QString;
class QVariant;

namespace Rializer {

class JsonFormatPrivate : public AbstractFormatPrivate
{
public:
    JsonFormatPrivate() = default;

    bool fromValue(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *returnValue
    );
    bool toValue(
        const Type::Type &type, const QJsonValue &value,
        Value::Value *returnValue
    );

    bool fromPrimitiveValue(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *returnValue
    );
    template<typename T>
    bool fromNumberValue(
        const Type::Type &type, const QVariant &primitive,
        double *returnValue
    );
    bool toPrimitiveValue(
        const Type::Type &type, const QJsonValue &value,
        Value::Value *returnValue
    );
    template<typename T>
    bool toNumberValue(
        const Type::Type &type, const QJsonValue &value,
        T *returnValue
    );
    bool toStringValue(
        const Type::Type &type, const QJsonValue &value,
        QString *returnValue
    );

    bool fromEnumerationValue(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *returnValue
    );
    bool toEnumerationValue(
        const Type::Type &type, const QJsonValue &value,
        Value::Value *returnValue
    );

    bool fromCompositeValue(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *returnValue
    );
    bool toCompositeValue(
        const Type::Type &type, const QJsonValue &value,
        Value::Value *returnValue
    );

    bool fromSingleDimensionalContainerValue(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *returnValue
    );
    bool toSingleDimensionalContainerValue(
        const Type::Type &type, const QJsonValue &value,
        Value::Value *returnValue
    );

    bool representationIsNotValid(
        QJsonParseError parseError,
        SourceLocation location
    );
    using AbstractFormatPrivate::representationIsNotValid;
};

} // namespace Rializer

#endif // RIALIZER_JSONFORMAT_P_H
