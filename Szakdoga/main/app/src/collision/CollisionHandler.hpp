#pragma once

#include "../SceneObject.hpp"
#include "../Box.hpp"

#include <algorithm>
#include <vector>

class CollisionHandler {
public:
    static bool apply_impulse;
    static bool apply_displacement;

private:
    static std::vector<SceneObject::interaction> removeDuplicates(const std::vector<SceneObject::interaction>& interactions);
    static bool isCollidingContact(float relativeVelocity);

private:
    using contacts = const std::vector<Collider::ContactData>;
    static void debugContacts(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData, const Camera& camera);
    static contacts pushObjectsApart(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData);
    static void applyImpulse(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData);

public:
    static void handleCollisions(const std::vector<SceneObject::interaction>& interactions, const Camera& camera);
};
