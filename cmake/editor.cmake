find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(Dir.Editor.INCLUDE editor/include/)
set(Dir.Editor.Qt.SRC editor/src/qt/)

file(GLOB_RECURSE File.Editor.Qt.GUI_HDR editor/include/editor/qt/*.hpp)

file(GLOB_RECURSE File.Editor.Qt.SRC ${Dir.Editor.Qt.SRC}*.cpp ${Dir.Editor.Qt.SRC}.c)

qt5_wrap_cpp(File.Editor.Qt.WRAP_CPP ${File.Editor.Qt.GUI_HDR})

if (WIN32)
    add_executable(mana-editor-qt WIN32 ${File.Editor.Qt.SRC} ${File.Editor.Qt.WRAP_CPP})
else ()
    add_executable(mana-editor-qt ${File.Editor.Qt.SRC} ${File.Editor.Qt.WRAP_CPP})
endif ()

target_include_directories(mana-editor-qt PUBLIC ${Dir.Engine.INCLUDE} ${Dir.Editor.INCLUDE} ${Dir.Editor.Qt.SRC} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mana-editor-qt mana Qt5::Core Qt5::Widgets)