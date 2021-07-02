#ifndef BLOOPER_FUNCTIONAL_MACROS_HPP
#define BLOOPER_FUNCTIONAL_MACROS_HPP
#pragma once

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "OCUnusedMacroInspection"
#endif // __JETBRAINS_IDE__

#define BLOOPER_FORWARD(_var) std::forward<decltype(_var)>(_var)

#define BLOOPER_DECL(_var) blooper::meta::decl(BLOOPER_FORWARD(_var))

#define BLOOPER_TYPEID(_var) blooper::meta::typeid_c<decltype(_var)>

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif // __JETBRAINS_IDE__

#endif // BLOOPER_FUNCTIONAL_MACROS_HPP
