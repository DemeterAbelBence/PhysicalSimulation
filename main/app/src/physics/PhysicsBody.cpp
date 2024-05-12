#include "PhysicsBody.hpp"

glm::vec3 PhysicsBody::calculateReflection(glm::vec3 vector, glm::vec3 normal) {
    glm::vec3 v = normalize(vector);
    glm::vec3 n = normalize(normal);
        
    return glm::reflect(v, n);
}

PhysicsBody::PhysicsBody() {
    inverseMass = 0.0f;
    position = glm::vec3(0.0f);
    resultant = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
}

PhysicsBody::PhysicsBody(float _inverseMass, float _bounciness) {
    inverseMass = _inverseMass;
    bounciness = _bounciness;

    position = glm::vec3(0.0f);
    resultant = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
}

void PhysicsBody::exertForce(glm::vec3 direction, float force) {
    direction = glm::normalize(direction);
    resultant = force * direction;
}

void PhysicsBody::update(std::vector<Collider::CollisionData*> collisions) {
    using namespace glm;
    float eps1 = 0.01f;
    float eps2 = 0.0001f;
    float sn = 0.9f;
    float sd = 0.9f;
    float so = 0.5f;

    if (!collisions.empty()) {
        vec3 v = vec3(0.0f);
        for (Collider::CollisionData* c : collisions) {
            vec3 n = c->contactNormal;
            vec3 vn = (dot(n, velocity) / dot(n, n)) * n;
            vec3 vd = velocity - vn;

            if (length(vn) < eps1)
                vn = vec3(0.0f);
            else
                vn = -sn * vn;

            if (length(vd) < eps2)
                vd = vec3(0.0f);
            else
                vd = sd * vd;

            v += (vd + vn);
        }
        velocity = v;
    }

    acceleration = resultant * inverseMass;
    velocity += acceleration;

    float l = length(velocity);
    if (l < eps1)
        orientation = vec3(0.0f);
    else
        orientation = cross(vec3(0.0f, 1.0f, 0.0f), so * velocity);
}

void PhysicsBody::translate(glm::vec3 t) {
    position += t;
}

void PhysicsBody::rotate(glm::vec3 axis, float angle) {
}
