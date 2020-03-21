#ifndef RIALIZER_DESERIALIZER_H
#define RIALIZER_DESERIALIZER_H

#include <rializer/datatypes_fd.h>

#include <metatypesystem/rmetatype.h>

namespace Rializer {

class AbstractFormat;
class MetaTypeSystem;

class DeserializerPrivate;
class RIALIZER_LIBRARY Deserializer
{
public:
    explicit Deserializer(AbstractFormat *format);
    Q_DISABLE_COPY(Deserializer)

    virtual ~Deserializer();

    const Options & options() const;
    Options & options();
    void setOptions(const Options &options);
    void setOptions(Options &&options);

    const Error & error() const;

    const AbstractFormat & format() const;

    template <typename T>
    bool deserialize(
        const QByteArray &bytes,
        T *value
    );
    virtual bool deserialize(
        const RMetaType &metaType, const QByteArray &bytes,
        QVariant *value
    );

protected:
    Data & data();

    AbstractFormat & format1();
    MetaTypeSystem & metaTypeSystem();

protected:
    const QScopedPointer<DeserializerPrivate> d_ptr;

    explicit Deserializer(DeserializerPrivate &d);
    Q_DECLARE_PRIVATE(Deserializer)
};

template <typename T>
bool Deserializer::deserialize(
    const QByteArray &bytes,
    T * const value
)
{
    QVariant value1;
    if (!deserialize(RMetaType::fromType<T>(), bytes, &value1))
        return false;

    *value = variantValue<T>(value1);
    return true;
}

} // namespace Rializer

#endif // RIALIZER_DESERIALIZER_H
