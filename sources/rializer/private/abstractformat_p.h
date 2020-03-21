#ifndef RIALIZER_ABSTRACTFORMAT_P_H
#define RIALIZER_ABSTRACTFORMAT_P_H

#include "../datatypes_fd.h"
#include "../metatypesystem/metadatatypes_fd.h"

class QString;

namespace Rializer {

class AbstractFormatPrivate
{
public:
    AbstractFormatPrivate() = default;

    Data *data;

    bool setError(Error &&value);

    bool typeIsNotSupported(
        const Type::Type &type,
        SourceLocation location
    );

    bool representationIsNotValid(
        const QString &textMessage,
        SourceLocation location
    );

    bool representationIsNotOfType(
        const Type::Type &type,
        SourceLocation location
    );

    const Type::Type *root;
};

} // namespace Rializer

#endif // RIALIZER_ABSTRACTFORMAT_P_H
