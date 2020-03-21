#ifndef RIALIZER_SERIALIZER_H
#define RIALIZER_SERIALIZER_H

#include <rializer/datatypes_fd.h>

#include <metatypesystem/rmetatype.h>

namespace Rializer {

class AbstractFormat;
class MetaTypeSystem;

class SerializerPrivate;
class RIALIZER_LIBRARY Serializer
{
public:
    explicit Serializer(AbstractFormat *format);
    Q_DISABLE_COPY(Serializer)

    virtual ~Serializer();

    const Options & options() const;
    Options & options();
    void setOptions(const Options &options);
    void setOptions(Options &&options);

    const Error & error() const;

    const AbstractFormat & format() const;

    // todo (C++17): std::optional
    // Return by value.
    template <typename T>
    bool serialize(
        const T &value,
        QByteArray *bytes
    );
    virtual bool serialize(
        const RMetaType &metaType, const QVariant &value,
        QByteArray *bytes
    );

protected:
    Data & data();

    AbstractFormat & format1();
    MetaTypeSystem & metaTypeSystem();

protected:
    const QScopedPointer<SerializerPrivate> d_ptr;

    explicit Serializer(SerializerPrivate &d);
    Q_DECLARE_PRIVATE(Serializer)
};

template <typename T>
bool Serializer::serialize(
    const T &value,
    QByteArray * const bytes
)
{
    RMetaType metaType(RMetaType::fromType<T>());
    return serialize(metaType, makeVariant(metaType, &value), bytes);
}

} // namespace Rializer

#endif // RIALIZER_SERIALIZER_H
