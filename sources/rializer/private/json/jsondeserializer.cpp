#include "../../json/jsondeserializer.h"
#include "../deserializer_p.h"

#include "../../json/jsonformat.h"
#include "../../metatypesystem/metadatatypes.h"

namespace Rializer {

JsonDeserializer::JsonDeserializer()
  : Deserializer(new JsonFormat())
{}

bool JsonDeserializer::deserialize(
    const RMetaType &metaType, const QJsonValue &document,
    QVariant * const value
)
{
    Q_D(Deserializer);

    if (!d->data.error.isNull())
        d->data.error.clear();

    Type::Type type;
    if (!d->metaTypeSystem.fromType(metaType, &type))
        return false;

    JsonFormat * const format(static_cast<JsonFormat *>(d->format.data()));

    Value::Value value1;
    if (!format->deserialize(type, document, &value1))
        return false;

    return d->metaTypeSystem.toValue(type, std::move(value1), value);
}

} // namespace Rializer
