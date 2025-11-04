cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(eve4)

target_sources(app PRIVATE src/main.c)

zephyr_library_include_directories(
  src/FT_Eve_Hal
  src/FT_Esd_Framework
  src/FT_Esd_Widgets/ThirdPartylibs
  src/FT_Esd_Widgets
  src/Esd_Core
  src/Pages
  )

add_subdirectory(src/FT_Eve_Hal)
add_subdirectory(src/FT_Esd_Framework)
add_subdirectory(src/FT_Esd_Widgets/ThirdPartylibs)
add_subdirectory(src/FT_Esd_Widgets)
add_subdirectory(src/Esd_Core)
add_subdirectory(src/Pages)
