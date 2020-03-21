#ifndef RIALIZER_SERIALIZER_P_H
#define RIALIZER_SERIALIZER_P_H

#include "../metatypesystem/metatypesystem.h"
#include "../datatypes.h"

namespace Rializer {

class AbstractFormat;

class RMetaType;

class SerializerPrivate
{
public:
    explicit SerializerPrivate(AbstractFormat *format);

    Data data;

    const QScopedPointer<AbstractFormat> format;
    MetaTypeSystem metaTypeSystem;

    bool serialize(
        const RMetaType &metaType, const QVariant &value,
        QByteArray *bytes
    );
};

} // namespace Rializer

#endif // RIALIZER_SERIALIZER_P_H
