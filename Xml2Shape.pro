QT += \
    quick \
    xml \
    qml \
    gui-private \
    widgets

HEADERS += \
    appcore.h \
    feature.h \
    fs_category.h \
    fs_property_manager.h \
    fs_xslt.h \
    geometry.h \
    geometry_attribute.h \
    io_shape.h \
    loggingcategories.h \
    model_generator.h \
    shapelib/shapefil.h \
    xml_parser.h \
    xslt_processor.h \

SOURCES += \
    appcore.cpp \
    feature.cpp \
    fs_category.cpp \
    fs_property_manager.cpp \
    fs_xslt.cpp \
    geometry.cpp \
    geometry_attribute.cpp \
    io_shape.cpp \
    loggingcategories.cpp \
    main.cpp    \
    model_generator.cpp \
    shapelib/safileio.c \
    shapelib/shpopen.c  \
    shapelib/dbfopen.c \
    xml_parser.cpp \
    xslt_processor.cpp \

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    qml.qrc

CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
