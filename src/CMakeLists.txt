file(GLOB_RECURSE SOURCES *.cpp *.h)

set(SOURCES ${SOURCES}
            assets/assets.qrc
            icons/icons.qrc
            qml/qml.qrc)

include(../vendor/CMakeLists.txt)

add_executable(bible-app ${SOURCES} ${VENDOR_SOURCES})
target_link_libraries(bible-app
                      Qt5::Core
                      Qt5::Qml
                      Qt5::Quick
                      Qt5::Concurrent
                      ${SWORD_LIBRARIES})

include_directories(${SWORD_INCLUDE_DIRS})

set_target_properties(bible-app PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/data/Info.plist)

install(TARGETS bible-app
        DESTINATION ${CMAKE_INSTALL_BINDIR})
