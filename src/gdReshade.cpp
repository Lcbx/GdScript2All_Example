
#include "gdNameof.hpp"
#include "gdReshade.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#define PRINT(...) UtilityFunctions::print(__VA_ARGS__)

using namespace godot;


String gd2AllTest::hello_world() {
	PRINT("hello world");
	return "hello world";
}

void gd2AllTest::_bind_methods(){
	ClassDB::bind_static_method(CLASS_NAME, FAST_BIND(hello_world), &gd2AllTest::hello_world);
}