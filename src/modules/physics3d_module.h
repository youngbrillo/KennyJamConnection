#pragma once
#include <flecs.h>
#include <lua.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include "../components3D.h"

namespace core
{
	struct RigidBodySettings
	{
		float restitution = 0.4f;
		float friction = 0.3f;
		bool trigger = false;
		bool isStatic = false;
		bool isKinematic = false;
	};

	struct RigidBody3D
	{
		//int bodyType = (int)rp3d::BodyType::DYNAMIC;
		//float restitution = 0.4f, friction = 0.3f;
		rp3d::RigidBody* body = nullptr;
		rp3d::Collider* collider = nullptr;
	};
	struct RigidBodyVehicleController
	{
		float speed = 6.0f;
		float torquePow = 2.0f;
		int accel_key = KEY_UP, brake_key = KEY_DOWN, left_turn_key = KEY_LEFT, right_turn_key = KEY_RIGHT;
		rp3d::Vector3 velocity = rp3d::Vector3(0.0f, 0.0f, 1.0f);
		rp3d::Vector3 torque = rp3d::Vector3(0.0f, 1.0f, 0.0f);
		int forwardDirection = 0, turningDirection = 0;
	};
	class Physics3D
	{
	public:
		rp3d::PhysicsCommon* factory = nullptr;
		rp3d::PhysicsWorld* world = nullptr;
		RigidBody3D nextRb;
		flecs::system rigidbody_sync, vehicle_polling, vehicle_movement;
	public:
		//life cycle methods
		Physics3D();
		~Physics3D();
		void initialize(flecs::world& world);
		void fixedUpdate(const float& timestep);
		void debugRender();
		void poll();
		void inspect(const char* title = "Physics 3D");
		static void Extend(lua_State* L);
	public:
		//public facing API methods
		void AddBody(core::RigidBody3D& rigidbody);
		core::RigidBody3D* AddBoxBody(flecs::world* world, flecs::entity e, bool isStatic);
		core::RigidBody3D* AddBoxBodyEX(flecs::entity e, float sx, float sy, float sz, bool isStatic);
		core::RigidBody3D* AddSphereBody(flecs::world* world, flecs::entity e);
	};
}

