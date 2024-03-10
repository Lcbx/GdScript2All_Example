
#include "example_controller.hpp"
#include "g2All_generated/example_MovementState.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


double Character::gravity = 10.;// ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

void Character::_process(double delta)
{
	auto velocity = get_velocity(); 
	// in air
	if(!is_on_floor())
	{
		velocity.y = Math::clamp(velocity.y - gravity * delta,  - MAX_Y_SPEED, MAX_Y_SPEED);
		movementState = MovementEnum::fall;
	}
	else
	{
		// landing
		if(movementState == MovementEnum::fall)
		{
			jumpCoolDown.start();
			// TODO: apply fall damage + play landing animation
		}

		// on ground
		movementState = wantedMovement;
		coyoteTime.start();
	}

	double ground_speed = calculate_ground_speed();

	// jump
	// TODO?: maybe add a special function to jump called on just_pressed
	if(global_mov_dir.y > 0.0 && !coyoteTime.is_stopped() && jumpCoolDown.is_stopped())
	{
		velocity.y += Math::max(MIN_JUMP_VELOCITY, ground_speed);
		coyoteTime.stop();
		emit_signal("jump", ground_speed);
	}

	// when running, always go forward 
	Vector3 direction = ( movementState != MovementEnum::run ? global_mov_dir : get_basis().get_column(2) );

	Ref<MovementState> movementValues = movements[movementState];
	double top_speed = movementValues->get_top_speed();
	double nimbleness = movementValues->get_nimbleness();
	double acceleration = movementValues->get_acceleration() + ground_speed * nimbleness;

	double redirect = Math::clamp(1.0 - nimbleness * delta, 0.0, 1.0);
	double vel_delta = acceleration * delta;

	velocity.x = Math::move_toward(velocity.x * redirect, direction.x * top_speed, vel_delta);
	velocity.z = Math::move_toward(velocity.z * redirect, direction.z * top_speed, vel_delta);

	double new_ground_speed = calculate_ground_speed();

	emit_signal("movement", local_dir, new_ground_speed);

	set_velocity(velocity);

	move_and_slide();
}

void Character::set_movementState(MovementEnum value)
{
	if(movementState != value)
	{
		movementState = value;
		emit_signal("changedState", movementState);
	}
}

Character::MovementEnum Character::get_movementState() {
	return movementState;
}

Vector3 Character::get_global_mov_dir()
{return global_mov_dir;
}

void Character::set_global_mov_dir(Vector3 value)
{
	global_mov_dir = value;
	// TODO: verify up (y) is not inversed
	local_dir =  get_basis().inverse().xform(-value);
}

Vector3 Character::get_local_dir()
{return local_dir;
}

void Character::set_local_dir(Vector3 value)
{
	local_dir = value;
	global_mov_dir =  - value.x * get_basis().get_column(1) + value.y * Vector3(0,1,0) + value.z * get_basis().get_column(2);
}

double Character::calculate_ground_speed()
{
	auto velocity = get_velocity();
	return Math::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
}

void Character::set_view_dir(Vector3 value)
{
	view_dir = value;
	emit_signal("viewDirChanged", view_dir);
}

Vector3 Character::get_view_dir() {
	return view_dir;
}

void Character::set_movements(Array value) {
	movements = value;
}

Array Character::get_movements() {
	return movements;
}

void Character::set_wantedMovement(Character::MovementEnum value) {
	wantedMovement = value;
}

Character::MovementEnum Character::get_wantedMovement() {
	return wantedMovement;
}

void Character::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("_process", "delta"), &Character::_process);
	ClassDB::bind_method(D_METHOD("calculate_ground_speed"), &Character::calculate_ground_speed);
	ClassDB::bind_method(D_METHOD("set_movementState", "value"), &Character::set_movementState);
	ClassDB::bind_method(D_METHOD("get_movementState"), &Character::get_movementState);
	ClassDB::bind_method(D_METHOD("get_global_mov_dir"), &Character::get_global_mov_dir);
	ClassDB::bind_method(D_METHOD("set_global_mov_dir", "value"), &Character::set_global_mov_dir);
	ClassDB::bind_method(D_METHOD("get_local_dir"), &Character::get_local_dir);
	ClassDB::bind_method(D_METHOD("set_local_dir", "value"), &Character::set_local_dir);
	ClassDB::bind_method(D_METHOD("set_view_dir", "value"), &Character::set_view_dir);
	ClassDB::bind_method(D_METHOD("get_view_dir"), &Character::get_view_dir);
	ClassDB::bind_method(D_METHOD("set_movements", "value"), &Character::set_movements);
	ClassDB::bind_method(D_METHOD("get_movements"), &Character::get_movements);
	ClassDB::bind_method(D_METHOD("set_wantedMovement", "value"), &Character::set_wantedMovement);
	ClassDB::bind_method(D_METHOD("get_wantedMovement"), &Character::get_wantedMovement);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(crouch, "crouch"), "crouch", crouch);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(walk, "walk"), "walk", walk);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(run, "run"), "run", run);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(fall, "fall"), "fall", fall);
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::ARRAY, "movements"), "set_movements", "get_movements");
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::INT, "movementState"), "set_movementState", "get_movementState");
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::INT, "wantedMovement"), "set_wantedMovement", "get_wantedMovement");
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::VECTOR3, "global_mov_dir"), "set_global_mov_dir", "get_global_mov_dir");
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::VECTOR3, "local_dir"), "set_local_dir", "get_local_dir");
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::VECTOR3, "view_dir"), "set_view_dir", "get_view_dir");
	ClassDB::add_signal(get_class_static(), MethodInfo("changedState", PropertyInfo(Variant::OBJECT, "state")));
	ClassDB::add_signal(get_class_static(), MethodInfo("collision", PropertyInfo(Variant::OBJECT, "collision")));
	ClassDB::add_signal(get_class_static(), MethodInfo("movement", PropertyInfo(Variant::VECTOR3, "dir"), PropertyInfo(Variant::FLOAT, "speed")));
	ClassDB::add_signal(get_class_static(), MethodInfo("jump", PropertyInfo(Variant::FLOAT, "speed")));
	ClassDB::add_signal(get_class_static(), MethodInfo("viewDirChanged", PropertyInfo(Variant::VECTOR3, "euler")));
}
