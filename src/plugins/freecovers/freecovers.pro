SOURCES = ../shared2/QUMultilingualImageSource.cpp \
	../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	QUFreeCoversSource.cpp \
	../shared2/QURequestUrl.cpp \
	../shared2/QUHttpCollector.cpp \
	QUFreeCoversCollector.cpp \
	QUFreeCoversRequestUrl.cpp \
	QUFreeCoversResponse.cpp
HEADERS = ../shared2/QUMultilingualImageSource.h \
	../../QUStringSupport.h \
	../../QUSongSupport.h \
	QUFreeCoversSource.h \
	../shared2/QURequestUrl.h \
	../shared2/QUHttpCollector.h \
	QUFreeCoversCollector.h \
	QUFreeCoversRequestUrl.h \
	QUFreeCoversResponse.h
OTHER_FILES += freecovers.json
TRANSLATIONS = freecovers.de.ts \
	freecovers.pl.ts \
	freecovers.es.ts \
	freecovers.fr.ts \
	freecovers.pt.ts
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared2

TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml \
	network \
	widgets

CONFIG(release, debug|release) {
	TARGET = freecovers
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/freecovers/release
	MOC_DIR = ../tmp/freecovers/release
}

CONFIG(debug, debug|release) {
	TARGET = freecovers_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/freecovers/debug
	MOC_DIR = ../tmp/freecovers/debug
}
