#ifndef RIALIZER_ABSTRACTFORMAT_H
#define RIALIZER_ABSTRACTFORMAT_H

#include <rializer/datatypes_fd.h>
#include <rializer/metatypesystem/metadatatypes_fd.h>

#include <QtCore/QScopedPointer>

namespace Rializer {

class RMetaType;

class AbstractFormatPrivate;
class RIALIZER_LIBRARY AbstractFormat
{
public:
    AbstractFormat();
    Q_DISABLE_COPY(AbstractFormat)

    virtual ~AbstractFormat();

    void setData(Data *data);

    virtual bool serialize(
        const Type::Type &type, const Value::Value &value,
        QByteArray *bytes
    ) = 0;

    virtual bool deserialize(
        const Type::Type &type, const QByteArray &bytes,
        Value::Value *value
    ) = 0;

protected:
    Data & data();

protected:
    const QScopedPointer<AbstractFormatPrivate> d_ptr;

    explicit AbstractFormat(AbstractFormatPrivate &d);
    Q_DECLARE_PRIVATE(AbstractFormat)
};

} // namespace Rializer

#endif // RIALIZER_ABSTRACTFORMAT_H
