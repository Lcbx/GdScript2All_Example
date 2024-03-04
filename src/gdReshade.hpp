#ifndef GDRESHADE_H
#define GDRESHADE_H

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/Shader.hpp>


namespace godot {

class gd2AllTest : public Object {
	GDCLASS(gd2AllTest, Object)

	private:
		
	protected:
		
		static void _bind_methods();
		
		gd2AllTest(){}
		~gd2AllTest(){}

	public:
	
		static String hello_world();
};

}

#endif