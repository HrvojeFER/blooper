include_guard()

option(BLOOPER_BUILD_STANDALONE "Build blooper standalone app.")


# -----------------------------------------------------------------------------
# Project

cmake_minimum_required(VERSION 3.19...3.20)

project(blooper
        DESCRIPTION "LoopStation made with JUCE."
        VERSION 1.0.0
        LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)


# -----------------------------------------------------------------------------
# Fetch

env_fetch(env
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/cpp_env17.git
          GIT_TAG v0.1.0

          SCAFFOLD
          OPTIONS)

env_fetch(asio
          FROM
          URL https://www.steinberg.net/asiosdk

          SCAFFOLD
          INCLUDE_DIRS
          "/common"
          "/host"
          "/host/pc")

env_fetch(engine
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/tracktion_engine.git
          GIT_TAG cmake-v0.0.2

          SCAFFOLD
          OPTIONS
          "JUCE_BUILD_EXAMPLES ON")


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
        juce::juce_recommended_lto_flags)

env_target_definitions(
        juce

        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_ASIO=1

        INTERFACE
        $<TARGET_PROPERTY:blooper_juce,COMPILE_DEFINITIONS>)

env_target_include(
        juce
        INTERFACE
        $<TARGET_PROPERTY:blooper_juce,INCLUDE_DIRECTORIES>)


env_add_dep(audio blooper_juce)
env_add_dep(env env_export)


# -----------------------------------------------------------------------------
# Targets

file(GLOB_RECURSE
     __blooper_sources
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp")

if (BLOOPER_BUILD_STANDALONE)
    juce_add_gui_app(
            blooper_standalone

            PRODUCT_NAME "${PROJECT_NAME}"
            VERSION "${PROJECT_VERSION}"
            BUNDLE_ID "fer.hrvoje.blooper"
            DESCRIPTION "Blooper is a loopstation made for beatboxers."

            MICROPHONE_PERMISSION_ENABLED TRUE
            MICROPHONE_PERMISSION_TEXT
            "Please give blooper permission to use your microphone.")

    env_target_warn(blooper_standalone)
    env_target_optimize(blooper_standalone)

    env_set_cpp17(blooper_standalone)
    env_target_conform(blooper_standalone)

    env_target_link(
            standalone
            PRIVATE
            blooper::env
            blooper::audio)

    env_target_precompile(
            standalone
            PRIVATE
            "${PROJECT_SOURCE_DIR}/pch/pch.hpp")

    env_target_sources(
            standalone
            PRIVATE
            "${PROJECT_SOURCE_DIR}/audio/standalone.cpp"
            ${__blooper_sources})

    env_target_include(
            standalone
            PRIVATE
            "${PROJECT_SOURCE_DIR}/include")

    env_target_definitions(
            standalone
            PRIVATE
            JUCE_APPLICATION_NAME_STRING="${PROJECT_NAME}"
            JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}")
endif ()
