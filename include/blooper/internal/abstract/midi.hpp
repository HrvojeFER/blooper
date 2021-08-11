#ifndef BLOOPER_MIDI_HPP
#define BLOOPER_MIDI_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto minMidiNoteNumber =
    0;

[[maybe_unused]] inline constexpr auto maxMidiNoteNumber =
    127;

[[maybe_unused]] inline constexpr auto midiNoteNumberCount =
    static_cast<int>(maxMidiNoteNumber - minMidiNoteNumber) + 1;


[[maybe_unused]] inline constexpr auto minVelocity =
    0;

[[maybe_unused]] inline constexpr auto maxVelocity =
    127;

[[maybe_unused]] inline constexpr auto velocityValueCount =
    static_cast<int>(maxVelocity - minVelocity) + 1;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MIDI_HPP
