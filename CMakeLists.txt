include_guard()

option(BLOOPER_BUILD_STANDALONE "Build blooper standalone app.")


# -----------------------------------------------------------------------------
# Project

cmake_minimum_required(VERSION 3.19...3.22)

project(
  blooper
  DESCRIPTION "LoopStation made with JUCE and Tracktion Engine."
  VERSION 1.0.0
  LANGUAGES CXX C
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)


option(
  BLOOPER_BUILD_STANDALONE
  "Build the blooper standalone app."
  ON
)


# -----------------------------------------------------------------------------
# Fetch

env_fetch(
  env
  FROM
  GIT_REPOSITORY https://github.com/Hrle97/cpp_env17.git
  GIT_TAG v0.1.0
  
  SCAFFOLD
  OPTIONS
)

env_fetch(
  asio
  FROM
  URL https://www.steinberg.net/asiosdk
  
  SCAFFOLD
  INCLUDE_DIRS
  "/common"
  "/host"
  "/host/pc"
)

env_fetch(
  engine
  FROM
  GIT_REPOSITORY https://github.com/Hrle97/tracktion_engine.git
  GIT_TAG cmake-v0.0.2
  
  SCAFFOLD
  OPTIONS
  "JUCE_BUILD_EXAMPLES ON"
  "JUCE_BUILD_EXTRAS ON"
)


# -----------------------------------------------------------------------------
# Dependencies

env_add_static(juce STATIC)

env_target_link(
  juce
  
  PRIVATE
  blooper::asio
  
  juce::juce_core
  juce::juce_events
  juce::juce_data_structures
  
  juce::juce_audio_basics
  juce::juce_audio_devices
  juce::juce_audio_processors
  juce::juce_audio_plugin_client
  juce::juce_dsp
  juce::juce_audio_formats
  juce::juce_audio_utils
  juce::tracktion_graph
  
  juce::juce_graphics
  juce::juce_gui_basics
  juce::juce_gui_extra
  juce::tracktion_engine
  
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
)

env_target_definitions(
  juce
  
  PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_ASIO=1
  
  INTERFACE
  $<TARGET_PROPERTY:blooper_juce,COMPILE_DEFINITIONS>
)

env_target_include(
  juce
  INTERFACE
  $<TARGET_PROPERTY:blooper_juce,INCLUDE_DIRECTORIES>
)


env_add_dep(audio blooper_juce)
env_add_dep(env env_export)


# -----------------------------------------------------------------------------
# Env Targets

env_project_targets(
  DEPS
  blooper::env
  blooper::audio
  blooper::assets
  
  TEST_DEPS
  env::test
  
  BENCH_DEPS
  env::bench
)


# -----------------------------------------------------------------------------
# Audio Targets

file(
  GLOB_RECURSE
  __blooper_binaries
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/assets/static/**.*"
)

if(BLOOPER_BUILD_STANDALONE)
  juce_add_binary_data(
    blooper_assets
    
    HEADER_NAME "blooper_binaries.hpp"
    NAMESPACE "blooper::binaries"
    
    SOURCES
    ${__blooper_binaries}
  )

  env_target_suppress(blooper_assets)
  env_target_optimize(blooper_assets)

  env_set_cpp17(blooper_assets)
  env_target_conform(blooper_assets)

  env_add_alias(assets)

  env_target_set_bin_output(
    blooper_assets
    SUB blooper/assets
  )
endif()


file(
  GLOB_RECURSE
  __blooper_sources
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

if(BLOOPER_BUILD_STANDALONE)
  # TODO: company things
  juce_add_gui_app(
    blooper_standalone
    
    PRODUCT_NAME "${PROJECT_NAME}"
    VERSION "${PROJECT_VERSION}"
    BUNDLE_ID "fer.hrvoje.blooper"
    DESCRIPTION "Blooper is a loopstation made for beatboxers."
    
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT
    "Please give blooper permission to use your microphone."
  )

  env_target_warn(blooper_standalone)
  env_target_optimize(blooper_standalone)

  env_set_cpp17(blooper_standalone)
  env_target_conform(blooper_standalone)

  env_target_link(
    standalone
    PRIVATE
    blooper::env
    blooper::audio
    blooper::assets
    blooper::pch
  )

  env_target_sources(
    standalone
    PRIVATE
    "${PROJECT_SOURCE_DIR}/audio/standalone.cpp"
    ${__blooper_sources}
  )

  env_target_definitions(
    standalone
    PRIVATE
    JUCE_APPLICATION_NAME_STRING="${PROJECT_NAME}"
    JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
  )

  # TODO: add these to Env
  if(ENV_MSVC)
    # speed up:
    # https://devblogs.microsoft.com/cppblog/recommendations-to-speed-c-builds-in-visual-studio/

    env_target_safely_compile_with(
      standalone
      PRIVATE
      # inheritance via dominance
      "/wd4250"
      # multiprocess compilation
      "/MP9"
    )
  endif()

  env_target_set_bin_output(
    blooper_standalone
    SUB blooper/standalone
  )
endif()

