file(GLOB_RECURSE SOURCES *.cpp *.h qml/qml.qrc)

include(../vendor/CMakeLists.txt)

add_executable(bible-app ${SOURCES} ${VENDOR_SOURCES})
target_link_libraries(bible-app
                      Qt5::Core
                      Qt5::Qml
                      Qt5::Quick)

set_target_properties(bible-app PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/data/Info.plist)

install(TARGETS bible-app
        DESTINATION ${CMAKE_INSTALL_BINDIR})
