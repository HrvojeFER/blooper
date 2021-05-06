include_guard()


# -----------------------------------------------------------------------------
# Project

cmake_minimum_required(VERSION 3.19...3.20)

project(blooper
        DESCRIPTION "LoopStation made with JUCE."
        VERSION 1.0.0
        LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)

env_project_default_options()


# -----------------------------------------------------------------------------
# Fetch

env_fetch(env
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/cpp_env17.git
          GIT_TAG v0.0.1

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
          GIT_TAG cmake

          SCAFFOLD
          OPTIONS)


# -----------------------------------------------------------------------------
# Dependencies

env_add_static(juce STATIC)

env_target_link(
        juce
        PUBLIC
        blooper::asio
        juce::juce_core
        juce::juce_events
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_dsp
        juce::tracktion_graph
        juce::tracktion_engine
        PRIVATE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

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

env_project_targets(
        DEPS
        blooper::audio
        blooper::env

        TEST_DEPS
        env::test

        BENCH_DEPS
        env::bench)
