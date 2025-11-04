cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(eve4)

FILE(GLOB eve_hal_sources src/FT_Eve_Hal/*.c)
FILE(GLOB esd_core_sources src/Esd_Core/*.c)
FILE(GLOB esd_fw_sources src/FT_Esd_Framework/*.c)
FILE(GLOB esd_wdgt_sources src/FT_Esd_Widgets/*.c)
FILE(GLOB pages_sources src/Pages/*.c)

target_sources(app PRIVATE 
  ${eve_hal_sources}
  ${esd_core_sources}
  ${esd_fw_sources}
  ${esd_wdgt_sources}  
  ${pages_sources}
  )

zephyr_library_include_directories(
  src/FT_Eve_Hal
  src/FT_Esd_Framework
  src/FT_Esd_Widgets/ThirdPartylibs
  src/FT_Esd_Widgets
  src/Esd_Core
  src/Pages
  )