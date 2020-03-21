#include "../abstractformat.h"
#include "abstractformat_p.h"

#include "../datatypes.h"
#include "../metatypesystem/metadatatypes.h"
#include "../textmessages.h"

namespace Rializer {

AbstractFormat::AbstractFormat()
  : d_ptr(new AbstractFormatPrivate())
{}

AbstractFormat::AbstractFormat(AbstractFormatPrivate &d)
  : d_ptr(&d)
{}

AbstractFormat::~AbstractFormat() {}

void AbstractFormat::setData(Data * const data)
{
    Q_D(AbstractFormat);

    d->data = data;
}

Data & AbstractFormat::data()
{
    Q_D(const AbstractFormat);

    return *d->data;
}

bool AbstractFormatPrivate::setError(Error &&value)
{
    data->error = std::move(value);
    return false;
}

bool AbstractFormatPrivate::typeIsNotSupported(
    const Type::Type &type,
    const SourceLocation location
)
{
    if (data->mode.serialize) {
        using TextMessages::Serializer::Format
          ::typeIsNotSupported;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    typeIsNotSupported::id,
                    typeIsNotSupported::text,
                    {
                        root->metaType.name(),
                        type.metaType.name()
                    },
                    location
                )
            )
        );
    }

    if (data->mode.deserialize) {
        using TextMessages::Deserializer::Format
          ::typeIsNotSupported;

        return setError(
            Error(
                Reason::TypeIsNotValid,
                TextMessage(
                    typeIsNotSupported::id,
                    typeIsNotSupported::text,
                    {
                        root->metaType.name(),
                        type.metaType.name()
                    },
                    location
                )
            )
        );
    }

    return false;
}

bool AbstractFormatPrivate::representationIsNotValid(
    const QString &textMessage,
    const SourceLocation location
)
{
    using TextMessages::Deserializer::Format
      ::representationIsNotValid;

    return setError(
        Error(
            Reason::RepresentationIsNotValid,
            TextMessage(
                representationIsNotValid::id,
                representationIsNotValid::text,
                {
                    root->metaType.name(),
                    textMessage
                },
                location
            )
        )
    );
}

bool AbstractFormatPrivate::representationIsNotOfType(
    const Type::Type &type,
    const SourceLocation location
)
{
    using TextMessages::Deserializer::Format
      ::representationIsNotOfType;

    return setError(
        Error(
            Reason::RepresentationIsNotValid,
            TextMessage(
                representationIsNotOfType::id,
                representationIsNotOfType::text,
                {
                    root->metaType.name(),
                    type.metaType.name()
                },
                location
            )
        )
    );
}

} // namespace Rializer
