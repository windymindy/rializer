#include "../serializer.h"
#include "serializer_p.h"

#include "../abstractformat.h"
#include "../metatypesystem/metadatatypes.h"

namespace Rializer {

Serializer::Serializer(AbstractFormat * const format)
  : d_ptr(new SerializerPrivate(format))
{}

SerializerPrivate::SerializerPrivate(AbstractFormat * const format)
  : data(),
    format(format),
    metaTypeSystem()
{
    data.mode.serialize = true;

    format->setData(&data);
    metaTypeSystem.setData(&data);
}

Serializer::Serializer(SerializerPrivate &d)
  : d_ptr(&d)
{}

Serializer::~Serializer() {}

const Options & Serializer::options() const
{
    Q_D(const Serializer);

    return d->data.options;
}

Options & Serializer::options()
{
    Q_D(Serializer);

    return d->data.options;
}

void Serializer::setOptions(const Options &options)
{
    Q_D(Serializer);

    d->data.options = options;
}

void Serializer::setOptions(Options &&options)
{
    Q_D(Serializer);

    d->data.options = std::move(options);
}

const Error & Serializer::error() const
{
    Q_D(const Serializer);

    return d->data.error;
}

Data & Serializer::data()
{
    Q_D(Serializer);

    return d->data;
}

const AbstractFormat & Serializer::format() const
{
    Q_D(const Serializer);

    return *d->format.data();
}

AbstractFormat & Serializer::format1()
{
    Q_D(Serializer);

    return *d->format.data();
}

MetaTypeSystem & Serializer::metaTypeSystem()
{
    Q_D(Serializer);

    return d->metaTypeSystem;
}

bool Serializer::serialize(
    const RMetaType &metaType, const QVariant &value,
    QByteArray * const bytes
)
{
    Q_D(Serializer);

    return d->serialize(metaType, value, bytes);
}

bool SerializerPrivate::serialize(
    const RMetaType &metaType, const QVariant &value,
    QByteArray * const bytes
)
{
    if (!data.error.isNull())
        data.error.clear();

    Type::Type type;
    if (!metaTypeSystem.fromType(metaType, &type))
        return false;

    Value::Value value1;
    if (!metaTypeSystem.fromValue(type, value, &value1))
        return false;

    return format->serialize(type, value1, bytes);
}

} // namespace Rializer
