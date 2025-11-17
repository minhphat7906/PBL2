cmake_minimum_required(VERSION 3.16)

project(test1 VERSION 0.1 LANGUAGES CXX)

# Bạn không dùng .ui (Qt Designer) nữa, tắt AUTOUIC cho đỡ lỗi
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# ==== KHAI BÁO CÁC FILE NGUỒN CỦA PROJECT ====
# Ở đây:
# - main.cpp  : chứa applyTheme, MainWindow, main(...)
# - core.cpp  : chứa data + CSV + utils
# - core.h    : khai báo struct, biến toàn cục, hàm
# - ui_pages.h: chứa các class UI (LoginDialog, PatientsPage, ...)
set(PROJECT_SOURCES
    main.cpp
    core.cpp
    core.h
    ui_pages.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(test1 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(test1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(test1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# ===== PHẦN DƯỚI GIỮ NGUYÊN (bundle, install, finalize) =====
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.test1)
endif()

set_target_properties(test1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS test1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test1)
endif()
