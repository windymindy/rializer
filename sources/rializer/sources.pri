HEADERS += serializer.h \
           deserializer.h \
           abstractformat.h \
           datatypes.h \
           datatypes_fd.h \
           textmessages.h

HEADERS += metatypesystem/metatypesystem.h \
           metatypesystem/metadatatypes.h \
           metatypesystem/metadatatypes_fd.h

HEADERS += json/jsonformat.h \
           json/jsonserializer.h \
           json/jsondeserializer.h

HEADERS += private/serializer_p.h \
           private/deserializer_p.h \
           private/abstractformat_p.h

HEADERS += private/metatypesystem/metatypesystem_p.h

HEADERS += private/json/jsonformat_p.h

SOURCES += private/serializer.cpp \
           private/deserializer.cpp \
           private/abstractformat.cpp \
           private/textmessages.cpp

SOURCES += private/metatypesystem/metatypesystem.cpp

SOURCES += private/json/jsonformat.cpp \
           private/json/jsonserializer.cpp \
           private/json/jsondeserializer.cpp

VPATH *= $$PWD
