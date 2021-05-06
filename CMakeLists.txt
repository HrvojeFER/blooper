include_guard()


# -----------------------------------------------------------------------------
# Project

cmake_minimum_required(VERSION 3.17...3.20)

project(blooper
        DESCRIPTION "Loopstation made with JUCE."
        VERSION 1.0.0
        LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)


# -----------------------------------------------------------------------------
# Fetch

#env_fetch(
#        asio
#        FROM
#        URL https://www.steinberg.net/asiosdk
#
#        SCAFFOLD
#        INCLUDE_DIRS
#        "/common"
#        "/host"
#        "/host/pc")
#
#env_fetch(
#        engine
#        FROM
#        GIT_REPOSITORY https://github.com/Hrle97/tracktion_engine.git
#        GIT_TAG cmake
#
#        SCAFFOLD
#        OPTIONS)


# -----------------------------------------------------------------------------
# Dependencies

#env_add_library(juce STATIC)
#
#env_target_link(
#        juce
#        PRIVATE
#        blooper::asio
#        juce::juce_core
#        juce::juce_events
#        juce::juce_audio_basics
#        juce::juce_audio_devices
#        juce::juce_audio_formats
#        juce::juce_dsp
#        juce::tracktion_graph
#        PRIVATE
#        juce::juce_recommended_config_flags
#        juce::juce_recommended_lto_flags
#        juce::juce_recommended_warning_flags)
#
#env_target_set(
#        juce
#        POSITION_INDEPENDENT_CODE TRUE
#        VISIBILITY_INLINES_HIDDEN TRUE
#        C_VISIBILITY_PRESET hidden
#        CXX_VISIBILITY_PRESET hidden)
#
#env_target_definitions(
#        juce
#        PUBLIC
#        JUCE_WEB_BROWSER=0
#        JUCE_USE_CURL=0
#        JUCE_ASIO=1
#        INTERFACE
#        $<TARGET_PROPERTY:env_juce,COMPILE_DEFINITIONS>)
#
#env_target_include(
#        juce
#        INTERFACE
#        $<TARGET_PROPERTY:env_juce,INCLUDE_DIRECTORIES>)
#
#env_target_link(audio INTERFACE juce)


# -----------------------------------------------------------------------------
# Targets

