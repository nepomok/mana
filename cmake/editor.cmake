find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(DIR_EDITOR_INCLUDE editor/include/)
set(DIR_EDITOR_SRC_QT editor/src/qt/)

set(HDR_GUI_MANA_EDITOR editor/include/editor/qt/mainwindow.hpp)

file(GLOB_RECURSE EDITOR_SRC_QT ${DIR_EDITOR_SRC_QT}*.cpp ${DIR_EDITOR_SRC_QT}.c)

qt5_wrap_cpp(WRAP_CPP ${HDR_GUI_MANA_EDITOR})

if (WIN32)
    add_executable(mana-editor-qt WIN32 ${EDITOR_SRC_QT} ${WRAP_CPP})
else ()
    add_executable(mana-editor-qt ${EDITOR_SRC_QT} ${WRAP_CPP})
endif ()

target_include_directories(mana-editor-qt PUBLIC ${DIR_EDITOR_INCLUDE} ${DIR_EDITOR_SRC_QT})
target_link_libraries(mana-editor-qt mana Qt5::Core Qt5::Widgets)