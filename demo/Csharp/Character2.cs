using System;
using Godot;
using Godot.Collections;

[GlobalClass]
public partial class Character2 : Godot.CharacterBody3D
{


	/* 3D character controller
	usable for both players and AI
	for players, input is handled an overlay script which sets local_dir and view_dir
	speed and acceleration is based on movementState which is a Ressource (see MovementState2.gd)
	*/

	// TODO:
	// * add ways to get over obstacles
	// * implement fall damage
	// * hold crouch key to increase gravity ?
	// * hold jump key to lower gravity strength ?
	// * change gravity into a non-linear acceleration ?

	/* movement physics */

	public const double MIN_JUMP_VELOCITY = 3.5;
	public const double MAX_Y_SPEED = 10.0;

	public static readonly Vector3 XZ = new Vector3(1.0f, 0.0f, 1.0f);
	public static readonly Vector3 XY = new Vector3(1.0f, 1.0f, 0.0f);
	public static readonly Vector3 YZ = new Vector3(0.0f, 1.0f, 1.0f);

	public static double Gravity = (double)Godot.ProjectSettings.GetSetting("physics/3d/default_gravity");

	public Godot.Timer CoyoteTime = new Timer();
	public Godot.Timer JumpCoolDown = new Timer();

	public Character2()
	{
		CoyoteTime.WaitTime = .15;
		AddChild(CoyoteTime);

		JumpCoolDown.WaitTime = .15;
		AddChild(JumpCoolDown);
	}


	public override void _Process(double delta)
	{
		var velocity = Velocity;
		// in air
		if (!IsOnFloor())
		{
			velocity.Y = (float)Mathf.Clamp(Velocity.Y - Gravity * delta,  - MAX_Y_SPEED, MAX_Y_SPEED);
			MovementState = MovementEnum.fall;
		}
		else
		{
			// landing
			if(MovementState == MovementEnum.fall)
			{
				JumpCoolDown.Start();
				// TODO: apply fall damage + play landing animation
			}

			// on ground
			MovementState = WantedMovement;
			CoyoteTime.Start();
		}

		var ground_speed = CalculateGroundSpeed();

		// jump
		// TODO?: maybe add a special function to jump called on just_pressed
		if(GlobalMovDir.Y > 0.0 && !CoyoteTime.IsStopped() && JumpCoolDown.IsStopped())
		{
			velocity.Y += (float)Mathf.Max(MIN_JUMP_VELOCITY, ground_speed);
			CoyoteTime.Stop();
			EmitSignal("Jump", ground_speed);
		}

		// when running, always go forward 
		var direction = (MovementState != MovementEnum.run ? GlobalMovDir : Basis.Z );

		var movementValues = Movements[(int)MovementState];
		var top_speed = movementValues.TopSpeed;
		var nimbleness = movementValues.Nimbleness;
		var acceleration = movementValues.Acceleration + ground_speed * nimbleness;

		var redirect = Mathf.Clamp(1.0 - nimbleness * delta, 0.0, 1.0);
		var vel_delta = acceleration * delta;

		velocity.X = (float)Mathf.MoveToward(Velocity.X * redirect, direction.X * top_speed, vel_delta);
		velocity.Z = (float)Mathf.MoveToward(Velocity.Z * redirect, direction.Z * top_speed, vel_delta);

		var new_ground_speed = CalculateGroundSpeed();

		EmitSignal("Movement", LocalDir, new_ground_speed);

		Velocity = velocity;

		MoveAndSlide();

		foreach(int i in GD.Range(GetSlideCollisionCount()))
		{
			EmitSignal("Collision", GetSlideCollision(i));
		}
	}


	/* movement state / animations */

	[Signal]
	public delegate void ChangedStateEventHandler(MovementEnum state);
	[Signal]
	public delegate void CollisionEventHandler(Godot.KinematicCollision3D collision);
	[Signal]
	public delegate void MovementEventHandler(Vector3 dir, double speed);
	[Signal]
	public delegate void JumpEventHandler(double speed);

	public enum MovementEnum {crouch, walk, run, fall};
	[Export] public Array<MovementState2> Movements;

	public MovementEnum MovementState {
		get => _MovementState;
		set
		{
			if (_MovementState != value)
			{
				_MovementState = value;
				EmitSignal("ChangedState", (int)_MovementState);
			}
		}
	}
	private MovementEnum _MovementState = MovementEnum.walk;


	public MovementEnum WantedMovement = MovementEnum.walk;


	/* steering variables */

	protected Vector3 _GlobalMovDir = new Vector3();
	public Vector3 GlobalMovDir
	{
		get
		{ return _GlobalMovDir;
		}
		set
		{
			_GlobalMovDir = value;
			// TODO: verify up (y) is not inversed
			_LocalDir =  - value * Basis.Inverse();
		}
	}

	// NOTE: local_dir is normalized on the xz plane by Overlay
	protected Vector3 _LocalDir;
	public Vector3 LocalDir
	{
		get
		{return _LocalDir;
		}
		set
		{
			_LocalDir = value;
			_GlobalMovDir =  - value.X * Basis.X + value.Y * Vector3.Up + value.Z * Basis.Z;
		}
	}

	public double CalculateGroundSpeed()
	{
		return Mathf.Sqrt(Velocity.X * Velocity.X + Velocity.Z * Velocity.Z);
	}

	/* view */

	[Signal]
	public delegate void ViewDirChangedEventHandler(Vector3 euler);

	public Vector3 ViewDir
	{
		get => _ViewDir;

		set
		{
			_ViewDir = value;
			EmitSignal("ViewDirChanged", _ViewDir);
		}
	}
	private Vector3 _ViewDir;
}
