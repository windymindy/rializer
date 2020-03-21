#include "../textmessages.h"

namespace Rializer {

// todo: remove
namespace TextMessages {

#undef RIALIZER_TEXTMESSAGE
#define RIALIZER_TEXTMESSAGE(message) \
    constexpr char message::id[]; \
    constexpr char message::text[]; \

namespace Serializer {
namespace Format {
RIALIZER_TEXTMESSAGE(typeIsNotSupported)
} // namespace Format
namespace MetaTypeSystem {
RIALIZER_TEXTMESSAGE(typeIsNotSupported)
RIALIZER_TEXTMESSAGE(metaTypeIsNotRegistered)
RIALIZER_TEXTMESSAGE(propertyMetaTypeIsNotRegistered)
RIALIZER_TEXTMESSAGE(metaEnumerationIsNotRegistered)
RIALIZER_TEXTMESSAGE(valueIsNotOfEnumerationType)
} // namespace MetaTypeSystem
} // namespace Serializer

namespace Deserializer {
namespace Format {
RIALIZER_TEXTMESSAGE(typeIsNotSupported)
RIALIZER_TEXTMESSAGE(representationIsNotValid)
RIALIZER_TEXTMESSAGE(representationIsNotOfType)
} // namespace Format
namespace MetaTypeSystem {
RIALIZER_TEXTMESSAGE(typeIsNotSupported)
RIALIZER_TEXTMESSAGE(metaTypeIsNotRegistered)
RIALIZER_TEXTMESSAGE(propertyMetaTypeIsNotRegistered)
RIALIZER_TEXTMESSAGE(metaEnumerationIsNotRegistered)
RIALIZER_TEXTMESSAGE(valueIsNotOfEnumerationType)
} // namespace MetaTypeSystem
} // namespace Deserializer

} // namespace TextMessages

} // namespace Rializer
