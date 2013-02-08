PROJECT(nmeaOSMviewer)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)
INCLUDE(GenerateExportHeader)

OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)

FIND_PACKAGE(Boost REQUIRED)
MESSAGE(STATUS "** Search Boost root: ${BOOST_ROOT}")
MESSAGE(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
MESSAGE(STATUS "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
MESSAGE(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTNETWORK TRUE)

SET(osmViewer_SOURCES OSMMap.cpp)
SET(osmViewer_HEADERS OSMMap.h)

SET(nmeaOSMviewer_SOURCES main.cpp browserwindow.cpp gps.cpp gpsNMEA.cpp Tools.cpp Settings.cpp) 
SET(nmeaOSMviewer_HEADERS browserwindow.h gps.h gpsNMEA.h Tools.h Settings.h)

## -g Debugging
## -Wall all warnings
## -std=c++0x
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")

QT4_WRAP_CPP(nmeaOSMviewer_HEADERS_MOC ${nmeaOSMviewer_HEADERS})
QT4_WRAP_CPP(osmViewer_HEADERS_MOC ${osmViewer_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(nmeaOSMviewer ${nmeaOSMviewer_SOURCES} ${nmeaOSMviewer_HEADERS_MOC} ${osmViewer_SOURCES} ${osmViewer_HEADERS_MOC})
TARGET_LINK_LIBRARIES(nmeaOSMviewer ${QT_LIBRARIES})

## build libraries now
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
	SET (LIB_TYPE SHARED)
ENDIF (BUILD_SHARED_LIBS)

ADD_LIBRARY(osmViewer ${LIB_TYPE} ${osmViewer_SOURCES} ${osmViewer_HEADERS_MOC})
TARGET_LINK_LIBRARIES(osmViewer ${QT_LIBRARIES})
GENERATE_EXPORT_HEADER(osmViewer
	BASE_NAME osmViewer
	EXPORT_MACRO_NAME osmViewer_EXPORT
	EXPORT_FILE_NAME osmViewer_Export.h
	STATIC_DEFINE osmViewer_BUILT_AS_STATIC
)
