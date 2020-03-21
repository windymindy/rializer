#include "../../json/jsonserializer.h"
#include "../serializer_p.h"

#include "../../json/jsonformat.h"
#include "../../metatypesystem/metadatatypes.h"

namespace Rializer {

JsonSerializer::JsonSerializer()
  : Serializer(new JsonFormat())
{}

bool JsonSerializer::serialize(
    const RMetaType &metaType, const QVariant &value,
    QJsonValue * const document
)
{
    Q_D(Serializer);

    if (!d->data.error.isNull())
        d->data.error.clear();

    Type::Type type;
    if (!d->metaTypeSystem.fromType(metaType, &type))
        return false;

    Value::Value value1;
    if (!d->metaTypeSystem.fromValue(type, value, &value1))
        return false;

    JsonFormat * const format(static_cast<JsonFormat *>(d->format.data()));

    return format->serialize(type, value1, document);
}

} // namespace Rializer
