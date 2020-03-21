#ifndef RIALIZER_JSONDESERIALIZER_H
#define RIALIZER_JSONDESERIALIZER_H

#include <rializer/deserializer.h>

namespace Rializer {

class RIALIZER_LIBRARY JsonDeserializer : public Deserializer
{
public:
    JsonDeserializer();

    template <typename T>
    bool deserialize(
        const QJsonValue &document,
        T *value
    );
    virtual bool deserialize(
        const RMetaType &metaType, const QJsonValue &document,
        QVariant *value
    );
    using Deserializer::deserialize;

};

template <typename T>
bool JsonDeserializer::deserialize(
    const QJsonValue &document,
    T * const value
)
{
    QVariant value1;
    if (!deserialize(RMetaType::fromType<T>(), document, &value1))
        return false;

    *value = variantValue<T>(value1);
    return true;
}

} // namespace Rializer

#endif // RIALIZER_JSONDESERIALIZER_H
