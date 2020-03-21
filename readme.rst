Rializer is a demonstration of what a serialization/deserialization API
for the Qt framework could look like. It serves two purposes.

  - Mainly, it is a use case for a meta container functionality.
    There is a meta container class in sources/metatypesystem.
    It is a further development of QSequentialIterable_ ideas by `Stephen Kelly`_.
    This example is to show the necessity of improvement of the Qt meta type system
    in terms of operating on container values.
    It allows determining container and element type,
    viewing and setting element values.

  - Secondary, it is a proof of concept of *automatic* serialization/deserialization.
    Automatic means that ideally no code is required to operate on a supported type value.
    It is a nice thing to have. It decouples a model from a representation by design
    and requires less effort when scaling.
    The secondary purpose of this example is to kick off a discussion and
    to find possible caveats.
    I would like to have such a library. But it is not possible to implement
    without support of containers from the meta type system.

Documents:

 - Design_.
 - Links_.
 - todo_

.. _Design: documents/design.rst
.. _Links: documents/links.rst
.. _todo: documents/todo.rst

.. _QSequentialIterable: https://doc.qt.io/qt-5/qsequentialiterable.html
.. _Stephen Kelly: https://codereview.qt-project.org/q/owner:%2522stephen+kelly%2522
