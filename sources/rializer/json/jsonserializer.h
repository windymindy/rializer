#ifndef RIALIZER_JSONSERIALIZER_H
#define RIALIZER_JSONSERIALIZER_H

#include <rializer/serializer.h>

namespace Rializer {

class RIALIZER_LIBRARY JsonSerializer : public Serializer
{
public:
    JsonSerializer();

    template <typename T>
    bool serialize(
        const T &value,
        QJsonValue *document
    );
    virtual bool serialize(
        const RMetaType &metaType, const QVariant &value,
        QJsonValue *document
    );
    using Serializer::serialize;

};

template <typename T>
bool JsonSerializer::serialize(
    const T &value,
    QJsonValue * const document
)
{
    RMetaType metaType(RMetaType::fromType<T>());
    return serialize(metaType, makeVariant(metaType, &value), document);
}

} // namespace Rializer

#endif // RIALIZER_JSONSERIALIZER_H
