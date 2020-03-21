DEFINES += RIALIZER_LIBRARY=Q_DECL_IMPORT

CONFIG(debug, debug|release): LIBS += -lrializer_debug
else: LIBS += -lrializer
