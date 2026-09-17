cmake_minimum_required(VERSION 3.16)

project(ExpenseTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(ExpenseTracker
    main.cpp

    include/Expense.h
    include/FileManager.h
    include/ExpenseManager.h

    src/FileManager.cpp
    src/ExpenseManager.cpp
)

target_include_directories(ExpenseTracker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ExpenseTracker PRIVATE
    Qt6::Widgets
)