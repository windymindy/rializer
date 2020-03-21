This document contains design decisions written so that they can be revised.

Rializer
--------

Description
~~~~~~~~~~~

`Serialization/deserialization`_ API is a simple synchronous blocking function
that converts a value and a representation.
Rializer is not a data format,
nor a performance oriented message transfer protocol with versioning.
It is not a reflection API.
Rializer is an example of serialization/deserialization library.
It shows how to bring together a type reflection system with a data format.
It is automatic. If a type is supported by the meta type system,
it should be serializable/deserializable.
Derived type definitions are completely decoupled from representation.
This gives reusability and scalability.
It is runtime and is looking forward to C++ (or Qt) static reflection.

Rializer has a property-based object-oriented Qt-style API.
Its program structure is based on a tree data model
and the components that operate on it:
a meta type system and a format.
The Qt meta type system provides an ability to create and introspect a value of type.
The format is an encoder/decoder.
Serializer/deserializer has options and error properties.
A format implementation can have its private properties, 
for example, a stream reader (parser) or writer,
to use them anywhere in a call stack.

The serialization routine is:
  - build a type tree;
  - build a value tree;
  - create a representation.

And the deserialization is:
  - build a type tree;
  - build a value tree from a representation;
  - create a value from the value tree.

Type information is the source of truth and decides the routine.

Boolean options default to false.
The names contain 'with' or 'no' postfix
so it is clear that setting an option is making its value true.

The error comprises a reason enumeration and a text.

Extending
~~~~~~~~~

Serializer/deserializer and the format are dynamically polymorphic.
There might be a use case for choosing a serializer/deserializer
based, for example, on a content type in an HTTP message.
So adding a new format implementation uses dynamic polymorphism.
It is also nice to have a serializer/deserializer with a statically set format.

Extending the routine should be done with static polymorphism.
The routine is composed of parts that can be used on their own.
If one is to represent some attributes or schema data,
he should replace the default meta type system component.
If one is to add schema validation,
then he should add a parameter into the signature
and a validation step.

Document vs. fragment
~~~~~~~~~~~~~~~~~~~~~

There is a use case of `multistep serialization/deserialization`_ of encapsulated documents.
It seems comfortable to produce fragment representation without any prolog or header.
In some formats, such as JSON, a value fragment equals a document.
Nevertheless, serializer should produce a document.
Producing a document may be an optional feature.
Multistep concatenation/separation may be performed with a format specific tool.
Then the tool can also switch between a document and a fragment.

Document Object Model vs. byte array
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A representation might be an object of a transformation.
And a format might be implemented not only as a stream reader/writer,
but also as a Document Object Model.
In this case it is rational to provide an overload
that operates on a document, not on a byte array.
Otherwise, a user would require an extra encoding/decoding step.

This is a perfect use case for extending both serializer/deserializer
and a format and adding method overloads.

.. _serialization/deserialization: https://en.wikipedia.org/wiki/Serialization
.. _multistep serialization/deserialization: https://microsoft.github.io/bond/manual/bond_cs.html#pass-through

Meta container
--------------

Setting elements moves them into a container.
The elements that have been moved from remain accessible.
Copy can be made via QMetaType.
Moving seems symmetric to viewing elements without copying.

There is a way to check if elements are unique.
There is a boolean output parameter in the elements setter.
This works only for associative containers.
The associative Qt container classes without 'Multi' postfix
in their names are described as not multiple,
even though they have special methods for storing
element pairs with not unique keys.

std::forward_list is not supported.
It has no push_back and size methods.
The container adaptor classes are outside the field of interest.
