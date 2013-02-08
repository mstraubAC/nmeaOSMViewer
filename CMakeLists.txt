PROJECT(nmeaOSMviewer)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

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

SET(nmeaOSMviewer_SOURCES main.cpp browserwindow.cpp gps.cpp gpsNMEA.cpp Tools.cpp Settings.cpp OSMMap.cpp) 
SET(nmeaOSMviewer_HEADERS browserwindow.h gps.h gpsNMEA.h Tools.h Settings.h OSMMap.h)

## -g Debugging
## -Wall all warnings
## -std=c++0x
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")

QT4_WRAP_CPP(nmeaOSMviewer_HEADERS_MOC ${nmeaOSMviewer_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(nmeaOSMviewer ${nmeaOSMviewer_SOURCES} ${nmeaOSMviewer_HEADERS_MOC})
TARGET_LINK_LIBRARIES(nmeaOSMviewer ${QT_LIBRARIES})
