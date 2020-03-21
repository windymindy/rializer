  - copy free design
      | Value tree should store either a view or a value.
      |
      | a value should be passed by void pointer (or QAny?)
      | in the serializer/deserialize method
      |
      | limitations
      |     QMetaProperty
      |         check and proceed conditionaly
      |         read a property by reference or by value
      |         write a property by move or by copy
      |     QMetaType
      |         move or copy

  - data model
      | tuple/pair
      | flags?
      |     a composite of booleans seems better
      | option? some or none
      | any?
      |     no static type
      |     a type should be known
      |     no arbitary construction
      | one of? variant/union
      | inhomogeneous collection?
      | polymorphism?
      | pass through?
      |     https://microsoft.github.io/bond/manual/bond_cs.html#pass-through
      |     QJsonValue
      |         how to represent in other formats?
      |     QByteArray
      | pointer or reference
      |     how to deserialize? in an array or a composite
      |     manage memory?
      |     should the previous be deleted?
      |     cross references
      | custom meta type traits
      |     provide type reflection and value/view converters
      |     primitives, properties, everything
      | properties
      |     order
      |     STORED flag

  - format
      | xml
      |     attribute
      |     namespace
      | cbor?

  - options
      | withIgnoreErrors
      |     errorsIgnored property of Error array type
      |     Type and Value Error variant
      |     every setError result is evaluated to determine
      |     if should proceed or return false
      | noStrict
      |     number conversion
      |         fractial/integer, overflows
      |     enumerations
      |         don't require to register Q_ENUM
      |         number conversion only
      |     properties
      |         missing
      |         extra
      |     case sensitivity?
      |     JSON map
      |         array of arrays of size 2
      |     container elements uniqueness
      | 
      | find a way to extend options
      |     template private class?
      |     new options types
      | enable precise control over every option scenario
      | with separate and common options

  - error
      | ValueIsNotValid
      |     container elements are not unique

  - log position
      | find a way to represent type and value trees
      | include index, key and property name

  - QIODevice overloads
      | a message should be complete

  - return by value
      | can't apply guidelines
      | requires introducing return type entity pre C++17
      |     std::optional
      |     structured binding
      |     std::tuple/std::tie
      | cant overload for QByteArray and QJsonDocument
      |     need tag
      | QIODevice is an input-output parameter
      | 
      | should be supported by the language
      |     no way to define anonymous struct
      |     need multiple return values with names

  - compile time
      | C++ compiletime reflection
      | or Qt?

  - Qt in namespace
      | https://wiki.qt.io/Qt_In_Namespace

  - come up with a proper name
      | rializer is a pun
