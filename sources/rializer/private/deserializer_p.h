#ifndef RIALIZER_DESERIALIZER_P_H
#define RIALIZER_DESERIALIZER_P_H

#include "../metatypesystem/metatypesystem.h"
#include "../datatypes.h"

namespace Rializer {

class AbstractFormat;

class RMetaType;

class DeserializerPrivate
{
public:
    explicit DeserializerPrivate(AbstractFormat *format);

    Data data;

    const QScopedPointer<AbstractFormat> format;
    MetaTypeSystem metaTypeSystem;

    bool deserialize(
        const RMetaType &metaType, const QByteArray &bytes,
        QVariant *value
    );
};

} // namespace Rializer

#endif // RIALIZER_DESERIALIZER_P_H
