#include "physics3d_module.h"
#include "../ModelManager.h"
#include <LuaBridge/LuaBridge.h>

core::Physics3D::Physics3D()
{
    // First you need to create the PhysicsCommon object. This is a factory module
    // that you can use to create physics world and other objects. It is also responsible
    // for logging and memory management
    factory = new rp3d::PhysicsCommon();

    // Create a physics world
    world = factory->createPhysicsWorld();

}
core::Physics3D::~Physics3D()
{
    factory->destroyPhysicsWorld(world);
    delete factory;
    factory = nullptr;
    world = nullptr;
}

void add_rigidbody(rp3d::PhysicsCommon& factory, rp3d::PhysicsWorld* world, flecs::entity e, const core::ObjectModel& mod, const core::Transform3D& t3d)
{
    //return;
    rp3d::Vector3 position(t3d.position.x, t3d.position.y, t3d.position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);

    core::RigidBody3D rb;
    rb.bodyType = mod.mId == ModelManager::planeId ? (int)rp3d::BodyType::STATIC : (int)rp3d::BodyType::DYNAMIC;
    rp3d::RigidBody* rigidbody = world->createRigidBody(transform);
    rigidbody->setType((rp3d::BodyType)rb.bodyType);
    //rigidbody->setType(rp3d::BodyType::DYNAMIC);

    rb.body = rigidbody;

    rp3d::CollisionShape* shape;
    rp3d::Vector3 half_extents = rp3d::Vector3(t3d.scale.x * 0.5f, t3d.scale.y * 0.5f, t3d.scale.z * 0.5f);
    if (mod.mId == ModelManager::planeId || mod.mId == ModelManager::cubeId)
    {
        shape = factory.createBoxShape(half_extents);
    }
    else if (mod.mId == ModelManager::sphereId)
    {
        rp3d::decimal radius = t3d.scale.x * 0.5f;
        shape = factory.createSphereShape(radius);
    }
    else
    {
        shape = factory.createBoxShape(half_extents);
    }
    transform = rp3d::Transform::identity();
    rp3d::Collider* collider =  rb.body->addCollider(shape, transform);
    rp3d::Material mat = collider->getMaterial();
    mat.setBounciness(rb.restitution);
    mat.setFrictionCoefficient(rb.friction);

    //collider->setMaterial(mat);
    //collider->setIsTrigger(rb.isSensor);
    e.set<core::RigidBody3D>(rb);
    //printf("%d Add rigid body now\n", e.id());
}

void update_rigidbody(core::RigidBody3D& rb, core::Transform3D& tr)
{
    rp3d::Transform t = rb.body->getTransform();
    rp3d::Vector3 pos = t.getPosition();
    tr.position.x = pos.x;
    tr.position.y = pos.y;
    tr.position.z = pos.z;

    rp3d::decimal angle;
    rp3d::Vector3 v3;
    t.getOrientation().getRotationAngleAxis(angle, v3);
    tr.rotationAxis.x = v3.x;
    tr.rotationAxis.y = v3.y;
    tr.rotationAxis.z = v3.z;
    tr.rotationAngle = angle * RAD2DEG;
}

void pollVehicleController(core::RigidBodyVehicleController& controller)
{
    controller.forwardDirection = (IsKeyDown(controller.accel_key) ? 1 : 0)         - (IsKeyDown(controller.brake_key) ? 1 : 0);
    controller.turningDirection = (IsKeyDown(controller.left_turn_key) ? 1 : 0)    - (IsKeyDown(controller.right_turn_key) ? 1 : 0);

    //calculate forward vector here?

}
void moveVehicle(const core::RigidBodyVehicleController& controller, core::RigidBody3D& rigidbody)
{
    rp3d::Vector3 torque, velocity;

    if (controller.turningDirection == controller.forwardDirection && controller.forwardDirection == 0)
        return;
    torque = controller.torque;
    torque *= controller.torquePow * controller.turningDirection;

    velocity = controller.velocity;
    velocity *= controller.speed * controller.forwardDirection;

    rigidbody.body->applyLocalTorque(torque);
    rigidbody.body->applyLocalForceAtCenterOfMass(velocity);

}
void core::Physics3D::initialize(flecs::world& ecs)
{
    //ecs.observer<core::ObjectModel, core::Transform3D>().event(flecs::OnSet)
    //    .each([this](flecs::entity e, core::ObjectModel& mod, core::Transform3D& transform) {add_rigidbody(*this->factory, this->world, e, mod, transform); });

    rigidbody_sync = ecs.system<core::RigidBody3D, core::Transform3D>().kind(0).each(update_rigidbody);
    vehicle_polling = ecs.system<core::RigidBodyVehicleController>().kind(0).each(pollVehicleController);
    vehicle_movement = ecs.system<core::RigidBodyVehicleController, core::RigidBody3D>().kind(0).each(moveVehicle);

}


void core::Physics3D::fixedUpdate(const float& timestep)
{
    vehicle_movement.run();
    world->update(timestep);
    rigidbody_sync.run();
}

void core::Physics3D::debugRender()
{
}

void core::Physics3D::poll()
{
    vehicle_polling.run();
}

void core::Physics3D::inspect(const char* title)
{
}
core::RigidBodyVehicleController* AttachRigidBodyVehicleController(flecs::entity e, float speed, float torque)
{
    e.set<core::RigidBodyVehicleController>({speed, torque});

    return e.get_mut<core::RigidBodyVehicleController>();
}
void core::Physics3D::Extend(lua_State* L)
{
    luabridge::getGlobalNamespace(L)
        .beginNamespace("core")
            .addFunction("AttachRigidBodyVehicleController", AttachRigidBodyVehicleController)
            .beginClass<core::RigidBodyVehicleController>("RigidBodyVehicleController")
                .addData("speed", &core::RigidBodyVehicleController::speed)
                .addData("torquePow", &core::RigidBodyVehicleController::torquePow)
                .addData("accel_key", &core::RigidBodyVehicleController::accel_key)
                .addData("brake_key", &core::RigidBodyVehicleController::brake_key)
                .addData("left_turn_key", &core::RigidBodyVehicleController::left_turn_key)
                .addData("right_turn_key", &core::RigidBodyVehicleController::right_turn_key)
                //.addData("speed", &core::RigidBodyVehicleController::speed)
            .endClass()
            .beginClass<core::Physics3D>("Physics3D")
                .addFunction("AddBoxBody", &core::Physics3D::AddBoxBody)
                .addFunction("AddBoxBodyEX", &core::Physics3D::AddBoxBodyEX)
                .addFunction("AddSphereBody", &core::Physics3D::AddSphereBody)
            .endClass()
        .endNamespace();
}

void core::Physics3D::AddBody(core::RigidBody3D& rigidbody)
{
}

void core::Physics3D::AddBoxBody(flecs::world* ecs, flecs::entity e, bool isStatic)
{
    //get transform
    core::Transform3D* t3d = e.get_mut<core::Transform3D>();
    rp3d::Vector3 half_extents = rp3d::Vector3(t3d->scale.x * 0.5f, t3d->scale.y * 0.5f, t3d->scale.z * 0.5f);

    AddBoxBodyEX(e, half_extents.x, half_extents.y, half_extents.z, isStatic);
  
}

void core::Physics3D::AddBoxBodyEX(flecs::entity e, float sx, float sy, float sz, bool isStatic)
{

    //get transform
    core::Transform3D* t3d = e.get_mut<core::Transform3D>();

    rp3d::Vector3 position(t3d->position.x, t3d->position.y, t3d->position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);


    rp3d::BodyType btype = rp3d::BodyType::DYNAMIC;
    if (isStatic)
        btype = rp3d::BodyType::STATIC;
    //set rigidbody
    rp3d::RigidBody* rigidbody = world->createRigidBody(transform);
    rigidbody->setType(btype);
    core::RigidBody3D rb;
    rb.body = rigidbody;
    //set collision shape
    rp3d::CollisionShape* shape;
    rp3d::Vector3 half_extents = rp3d::Vector3(sx, sy, sz);
    shape = factory->createBoxShape(half_extents);
    //set collider
    transform = rp3d::Transform::identity();
    rp3d::Collider* collider = rb.body->addCollider(shape, transform);
    rp3d::Material mat = collider->getMaterial();
    mat.setBounciness(rb.restitution);
    mat.setFrictionCoefficient(rb.friction);
    collider->setMaterial(mat);

    //set rigidbody component
    e.set<core::RigidBody3D>(rb);

}

void core::Physics3D::AddSphereBody(flecs::world* ecs, flecs::entity e)
{
    //get transform
    core::Transform3D* t3d = e.get_mut<core::Transform3D>();

    rp3d::Vector3 position(t3d->position.x, t3d->position.y, t3d->position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);


    rp3d::BodyType btype = rp3d::BodyType::DYNAMIC;
    //set rigidbody
    rp3d::RigidBody* rigidbody = world->createRigidBody(transform);
    rigidbody->setType(btype);
    core::RigidBody3D rb;
    rb.body = rigidbody;
    //set collision shape
    rp3d::CollisionShape* shape;
    shape = factory->createSphereShape(rp3d::decimal(t3d->scale.x * 0.5f));
    //set collider
    transform = rp3d::Transform::identity();
    rp3d::Collider* collider = rb.body->addCollider(shape, transform);
    rp3d::Material mat = collider->getMaterial();
    mat.setBounciness(rb.restitution);
    mat.setFrictionCoefficient(rb.friction);
    collider->setMaterial(mat);

    //set rigidbody component
    e.set<core::RigidBody3D>(rb);
}

