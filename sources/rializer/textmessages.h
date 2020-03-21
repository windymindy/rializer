#ifndef RIALIZER_TEXTMESSAGES_H
#define RIALIZER_TEXTMESSAGES_H

namespace Rializer {

// todo (C++17): inline variable
namespace TextMessages {

#define RIALIZER_TEXTMESSAGE(message, id1, text1) \
    struct message \
    { \
        static constexpr char id[] = id1; \
        static constexpr char text[] = text1; \
    };

namespace Serializer {

namespace Format {

RIALIZER_TEXTMESSAGE(
    typeIsNotSupported,
    "rializer_serializer_format_typeisnotsupported",
    "Failed to serialize a value of the type '%1'. "
    "Failed to translate a value of the type '%2' into a represantation. "
    "The type is not supported."
)

} // namespace Format

namespace MetaTypeSystem {

RIALIZER_TEXTMESSAGE(
    typeIsNotSupported,
    "rializer_serializer_metatypesystem_typeisnotsupported",
    "Failed to serialize a value of the type '%1'. "
    "Failed to reflect the type '%2'. "
    "The type is not supported."
)

RIALIZER_TEXTMESSAGE(
    metaTypeIsNotRegistered,
    "rializer_serializer_metatypesystem_metatypeisnotregistered",
    "Failed to serialize a value of the type '%1'. "
    "Failed to reflect the serialized type. "
    "The meta type is not registered."
)

RIALIZER_TEXTMESSAGE(
    propertyMetaTypeIsNotRegistered,
    "rializer_serializer_metatypesystem_propertymetatypeisnotregistered",
    "Failed to serialize a value of the type '%1'. "
    "Failed to reflect the type '%2' of the property '%3' of the composite type '%4'. "
    "The meta type is not registered."
)

RIALIZER_TEXTMESSAGE(
    metaEnumerationIsNotRegistered,
    "rializer_serializer_metatypesystem_metaenumerationisnotregistered",
    "Failed to serialize a value of the type '%1'. "
    "Failed to reflect the enumeration type '%2'. "
    "The meta enumeration is not registered."
)

RIALIZER_TEXTMESSAGE(
    valueIsNotOfEnumerationType,
    "rializer_serializer_metatypesystem_valueisnotofenumerationtype",
    "Failed to serialize a value of the type '%1'. "
    "Failed to reflect a value of the enumeration type '%2'. "
    "The value is not of the type."
)

} // namespace MetaTypeSystem

} // namespace Serializer

namespace Deserializer {

namespace Format {

RIALIZER_TEXTMESSAGE(
    typeIsNotSupported,
    "rializer_deserializer_format_typeisnotsupported",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to translate a representation into a value of the type '%2'. "
    "The type is not supported."
)

RIALIZER_TEXTMESSAGE(
    representationIsNotValid,
    "rializer_deserializer_format_representationisnotvalid",
    "Failed to deserialize a value of the type '%1'. "
    "The representation is not valid: '%2'."
)

RIALIZER_TEXTMESSAGE(
    representationIsNotOfType,
    "rializer_deserializer_format_representationisnotoftype",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to translate a representation into a value of the type '%2'. "
    "The representation is not of the type."
)

} // namespace Format

namespace MetaTypeSystem {

RIALIZER_TEXTMESSAGE(
    typeIsNotSupported,
    "rializer_deserializer_metatypesystem_typeisnotsupported",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to reflect the type '%2'. "
    "The type is not supported."
)

RIALIZER_TEXTMESSAGE(
    metaTypeIsNotRegistered,
    "rializer_serializer_metatypesystem_metatypeisnotregistered",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to reflect the deserialized type. "
    "The meta type is not registered."
)

RIALIZER_TEXTMESSAGE(
    propertyMetaTypeIsNotRegistered,
    "rializer_deserializer_metatypesystem_propertymetatypeisnotregistered",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to reflect the type '%2' of the property '%3' of the composite type '%4'. "
    "The meta type is not registered."
)

RIALIZER_TEXTMESSAGE(
    metaEnumerationIsNotRegistered,
    "rializer_deserializer_metatypesystem_metaenumerationisnotregistered",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to reflect the enumeration type '%2'. "
    "The meta enumeration is not registered."
)

RIALIZER_TEXTMESSAGE(
    valueIsNotOfEnumerationType,
    "rializer_deserializer_metatypesystem_valueisnotofenumerationtype",
    "Failed to deserialize a value of the type '%1'. "
    "Failed to reflect a value of the enumeration type '%2'. "
    "The value is not of the type."
)

} // namespace MetaTypeSystem

} // namespace Deserializer

} // namespace TextMessages

} // namespace Rializer

#endif // RIALIZER_TEXTMESSAGES_H
