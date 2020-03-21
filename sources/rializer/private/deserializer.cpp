#include "../deserializer.h"
#include "deserializer_p.h"

#include "../abstractformat.h"
#include "../metatypesystem/metadatatypes.h"

namespace Rializer {

Deserializer::Deserializer(AbstractFormat * const format)
  : d_ptr(new DeserializerPrivate(format))
{}

DeserializerPrivate::DeserializerPrivate(AbstractFormat * const format)
  : data(),
    format(format),
    metaTypeSystem()
{
    data.mode.deserialize = true;

    format->setData(&data);
    metaTypeSystem.setData(&data);
}

Deserializer::Deserializer(DeserializerPrivate &d)
  : d_ptr(&d)
{}

Deserializer::~Deserializer() {}

const Options & Deserializer::options() const
{
    Q_D(const Deserializer);

    return d->data.options;
}

Options & Deserializer::options()
{
    Q_D(Deserializer);

    return d->data.options;
}

void Deserializer::setOptions(const Options &options)
{
    Q_D(Deserializer);

    d->data.options = options;
}

void Deserializer::setOptions(Options &&options)
{
    Q_D(Deserializer);

    d->data.options = std::move(options);
}

const Error & Deserializer::error() const
{
    Q_D(const Deserializer);

    return d->data.error;
}

Data & Deserializer::data()
{
    Q_D(Deserializer);

    return d->data;
}

const AbstractFormat & Deserializer::format() const
{
    Q_D(const Deserializer);

    return *d->format.data();
}

AbstractFormat & Deserializer::format1()
{
    Q_D(Deserializer);

    return *d->format.data();
}

MetaTypeSystem & Deserializer::metaTypeSystem()
{
    Q_D(Deserializer);

    return d->metaTypeSystem;
}

bool Deserializer::deserialize(
    const RMetaType &metaType, const QByteArray &bytes,
    QVariant * const value
)
{
    Q_D(Deserializer);

    return d->deserialize(metaType, bytes, value);
}

bool DeserializerPrivate::deserialize(
    const RMetaType &metaType, const QByteArray &bytes,
    QVariant * const value
)
{
    if (!data.error.isNull())
        data.error.clear();

    Type::Type type;
    if (!metaTypeSystem.fromType(metaType, &type))
        return false;

    Value::Value value1;
    if (!format->deserialize(type, bytes, &value1))
        return false;

    return metaTypeSystem.toValue(type, std::move(value1), value);
}

} // namespace Rializer
