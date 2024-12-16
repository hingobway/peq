cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# PROJECT INIT
project(peq VERSION 1.1.0)
set(TARGET_NAME "PEQ")

# PACKAGE INCLUDE
add_subdirectory(include/juce JUCE)

# JUCE CONFIG
juce_add_plugin(${TARGET_NAME}
  COMPANY_NAME foster
  PLUGIN_MANUFACTURER_CODE Fost

  PLUGIN_CODE Yw2t
  PRODUCT_NAME "${TARGET_NAME}"
  PLUGIN_NAME "PEQ"

  FORMATS Standalone VST3 AU
  VST3_CATEGORIES EQ
  AU_MAIN_TYPE kAudioUnitType_Effect

  COPY_PLUGIN_AFTER_BUILD TRUE
)

target_compile_definitions(${TARGET_NAME}
  PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0

  DONT_SET_USING_JUCE_NAMESPACE=1

  CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
  VERSION="${CURRENT_VERSION}"

  PRODUCT_NAME_WITHOUT_VERSION="${TARGET_NAME}"
)

# ADD FILES
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
target_sources(${TARGET_NAME} PRIVATE ${SOURCE_FILES})

# BINARY DATA
juce_add_binary_data(BinaryData
  SOURCES
  img/knob.svg
)

# ADD LIBRARIES
target_link_libraries(${TARGET_NAME}
  PRIVATE
  BinaryData

  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_utils
  juce::juce_audio_processors
  juce::juce_gui_basics
  juce::juce_gui_extra

  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)
