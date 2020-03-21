#ifndef RIALIZER_METATYPESYSTEM_H
#define RIALIZER_METATYPESYSTEM_H

#include <rializer/datatypes_fd.h>
#include <rializer/metatypesystem/metadatatypes_fd.h>

#include <QtCore/QScopedPointer>

class QVariant;

namespace Rializer {

class RMetaType;

class MetaTypeSystemPrivate;
class RIALIZER_LIBRARY MetaTypeSystem
{
public:
    MetaTypeSystem();
    Q_DISABLE_COPY(MetaTypeSystem)

    ~MetaTypeSystem();

    void setData(Data *data);

    bool fromType(
        const RMetaType &metaType,
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

protected:
    Data & data();

protected:
    const QScopedPointer<MetaTypeSystemPrivate> d_ptr;

    explicit MetaTypeSystem(MetaTypeSystemPrivate &d);
    Q_DECLARE_PRIVATE(MetaTypeSystem)
};

} // namespace Rializer

#endif // RIALIZER_METATYPESYSTEM_H
