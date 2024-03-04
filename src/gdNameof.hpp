#ifndef GD_NAMEOF_H
#define GD_NAMEOF_H

// utility file to avoid a bunch of renaming
// because function and type names exist to be changed
// based on https://github.com/Neargye/nameof/blob/master/include/nameof.hpp

#define FUNC_NAME(...) []() constexpr noexcept {  \
  ::std::void_t<decltype(__VA_ARGS__)>();         \
  return #__VA_ARGS__; }()

// below are godot-exclusive macros (won't work elsewhere)

#define CLASS_NAME get_class_static()

#define FAST_BIND(method, ...) D_METHOD(FUNC_NAME(method), __VA_ARGS__)

#endif