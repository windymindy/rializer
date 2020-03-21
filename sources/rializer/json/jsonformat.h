#ifndef RIALIZER_JSONFORMAT_H
#define RIALIZER_JSONFORMAT_H

#include <rializer/abstractformat.h>

class QJsonValue;

namespace Rializer {

class JsonFormatPrivate;
class RIALIZER_LIBRARY JsonFormat : public AbstractFormat
{
public:
    JsonFormat();
    Q_DISABLE_COPY(JsonFormat)

    ~JsonFormat() override;

    // todo (QTBUG-62502): change document parameter type to QJsonDocument

    bool serialize(
        const Type::Type &type, const Value::Value &value,
        QJsonValue *document
    );

    bool deserialize(
        const Type::Type &type, const QJsonValue &document,
        Value::Value *value
    );

    bool serialize(
        const Type::Type &type, const Value::Value &value,
        QByteArray *bytes
    ) override;

    bool deserialize(
        const Type::Type &type, const QByteArray &bytes,
        Value::Value *value
    ) override;

protected:
    explicit JsonFormat(JsonFormatPrivate &d);
    Q_DECLARE_PRIVATE(JsonFormat)
};

} // namespace Rializer

#endif // RIALIZER_JSONFORMAT_H
