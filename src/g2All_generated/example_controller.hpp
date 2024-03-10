
#ifndef EXAMPLE_CONTROLLER_H
#define EXAMPLE_CONTROLLER_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/kinematic_collision3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

class Character : public CharacterBody3D {
	GDCLASS(Character, CharacterBody3D);
public:

/* 3D character controller
usable for both players and AI
for players, input is handled an overlay script which sets local_dir and view_dir
speed and acceleration is based on movementState which is a Ressource (see MovementState.gd)
*/

// TODO:
// * add ways to get over obstacles
// * implement fall damage
// * hold crouch key to increase gravity ?
// * hold jump key to lower gravity strength ?
// * change gravity into a non-linear acceleration ?

/* movement physics */

protected:
	const double MIN_JUMP_VELOCITY = 3.5;
	const double MAX_Y_SPEED = 10.0;

	const Vector3 XZ = Vector3(1.0, 0.0, 1.0);
	const Vector3 YZ = Vector3(0.0, 1.0, 1.0);
	const Vector3 XY = Vector3(1.0, 1.0, 0.0);

	static double gravity;

	Character(){
		coyoteTime.set_wait_time(.15);
		add_child(&coyoteTime);
		jumpCoolDown.set_wait_time(.15);
		add_child(&jumpCoolDown);
	}

	Timer coyoteTime = Timer(); //= Utils.createTimer(this, 0.15);
	Timer jumpCoolDown = Timer(); //= Utils.createTimer(this, 0.15);

/* movement state / animations */

public:
	void _process(double delta) override;
	/* signal changedState(Ref<MovementEnum> state) */
	/* signal collision(Ref<KinematicCollision3D> collision) */
	/* signal movement(Vector3 dir, double speed) */
	/* signal jump(double speed) */

	enum MovementEnum {crouch, walk, run, fall};

protected:
	Array movements;

	Character::MovementEnum movementState = MovementEnum::walk;

public:
	void set_movementState(Character::MovementEnum value);
	Character::MovementEnum get_movementState();

protected:
	Character::MovementEnum wantedMovement = MovementEnum::walk;

/* steering variables */

	Vector3 _global_mov_dir = Vector3();

	Vector3 global_mov_dir = Vector3();

public:
	Vector3 get_global_mov_dir();

// NOTE: local_dir is normalized on the xz plane by Overlay
	void set_global_mov_dir(Vector3 value);

protected:
	Vector3 _local_dir;

	Vector3 local_dir;

public:
	Vector3 get_local_dir();

	void set_local_dir(Vector3 value);

/* view */

	double calculate_ground_speed();
	/* signal viewDirChanged(Vector3 euler) */

protected:
	Vector3 view_dir = Vector3();

public:
	void set_view_dir(Vector3 value);
	Vector3 get_view_dir();
	void set_movements(Array value);
	Array get_movements();
	void set_wantedMovement(Character::MovementEnum value);
	Character::MovementEnum get_wantedMovement();

	static void _bind_methods();
};

VARIANT_ENUM_CAST(Character::MovementEnum)

#endif // EXAMPLE_CONTROLLER_H

