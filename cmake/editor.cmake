find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(Editor.Dir.INCLUDE editor/include/)
set(Editor.Dir.Qt.SRC editor/src/qt/)

file(GLOB_RECURSE Editor.File.Qt.GUI_HDR editor/include/editor/qt/*.hpp)

file(GLOB_RECURSE Editor.File.Qt.SRC ${Editor.Dir.Qt.SRC}*.cpp ${Editor.Dir.Qt.SRC}.c)

qt5_wrap_cpp(Editor.File.Qt.WRAP_CPP ${Editor.File.Qt.GUI_HDR})

if (WIN32)
    add_executable(editor-qt WIN32 ${Editor.File.Qt.SRC} ${Editor.File.Qt.WRAP_CPP})
else ()
    add_executable(editor-qt ${Editor.File.Qt.SRC} ${Editor.File.Qt.WRAP_CPP})
endif ()

target_include_directories(editor-qt PUBLIC ${Engine.Dir.INCLUDE} ${Editor.Dir.INCLUDE} ${Editor.Dir.Qt.SRC} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(editor-qt mana Qt5::Core Qt5::Widgets)