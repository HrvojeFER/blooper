#ifndef BLOOPER_ABSTRACT_HPP
#define BLOOPER_ABSTRACT_HPP
#pragma once

// TODO:
//  state/listener/identifier abstractions,
//  base construction assumptions,
//  UndoManager management,
//  fix template base tests,
//  abstract/base asset, edit, sync
//  explicit instantiation for commonly used things
//  panels?

#include <blooper/internal/abstract/id.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/abstract/matchers.hpp>

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/midi.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/abstract/windows.hpp>

#include <blooper/internal/abstract/tracks.hpp>
#include <blooper/internal/abstract/clips.hpp>
#include <blooper/internal/abstract/plugins.hpp>

#endif // BLOOPER_ABSTRACT_HPP
