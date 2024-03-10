
#include "register_types.h"

#include <gdextension_interface.h>

//#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "g2All_generated/example_MovementState.hpp"
#include "g2All_generated/example_controller.hpp"
#include "gdReshade.hpp"


using namespace godot;

const ModuleInitializationLevel INITIALISATION_LEVEL = ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE;

void initialize_gd2AllTest(ModuleInitializationLevel p_level) {
	if(p_level != INITIALISATION_LEVEL) return;
	ClassDB::register_abstract_class<gd2AllTest>(); // to not allow instancing
	
	ClassDB::register_class<MovementState>();
	ClassDB::register_class<Character>();
}

void uninitialize_gd2AllTest(ModuleInitializationLevel p_level) {
	if(p_level != INITIALISATION_LEVEL) return;
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gd2AllTest_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_gd2AllTest);
	init_obj.register_terminator(uninitialize_gd2AllTest); 
	init_obj.set_minimum_library_initialization_level(INITIALISATION_LEVEL);

	return init_obj.init();
}
}
