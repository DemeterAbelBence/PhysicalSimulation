#include "CollisionHandler.hpp"
#include <iostream>

bool CollisionHandler::apply_impulse = false;

bool CollisionHandler::apply_displacement = false;

std::vector<SceneObject::interaction> CollisionHandler::removeDuplicates(const std::vector<SceneObject::interaction>& interactions) {
    std::vector<SceneObject::interaction> result;

    auto contains = [&result](SceneObject::interaction i) {
        SceneObject* i_collider = std::get<0>(i);
        SceneObject* i_second = std::get<1>(i);

        for (const auto& r : result) {
            SceneObject* r_collider = std::get<0>(r);
            SceneObject* r_second = std::get<1>(r);

            if (r_collider == i_second && r_second == i_collider) {
                return true;
            }
        }

        return false;
    };

    for (const auto& i : interactions) {
        if (!contains(i)) {
            result.push_back(i);
        }
    }

    return result;
}

bool CollisionHandler::isCollidingContact(float relativeVelocity) {
    static const float threshold = 0.00001f;

    if (relativeVelocity > threshold) {
        return false;
    }
    if (relativeVelocity > -threshold) {
        return false;
    }
    else {
        return true;
    }
}

void CollisionHandler::debugContacts(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData, const Camera& camera) {
    RigidBody::BodyData* A = &colliderObject->getBodyStateSolver()->Body;
    RigidBody::BodyData* B = &collideeObject->getBodyStateSolver()->Body;
    CuboidCollider* collidee = dynamic_cast<CuboidCollider*>(collideeObject->getCollider());
    std::vector<glm::vec3> contactPoints;

    for (const auto& c : contactData) {
        contactPoints.push_back(c.point);

        glm::vec3 ra = c.point - A->X;
        glm::vec3 rb = c.point - B->X;
        glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
        glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
        float vrelm = glm::dot(c.normal, velpa - velpb);

        DebugDrawer::setOverrideZ(1);
        DebugDrawer::setMode(GL_LINES);

        DebugDrawer::setVertexData({ c.point, c.point + 10.0f * velpa });
        DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 0.0f));

        DebugDrawer::setVertexData({ c.point, c.point + 10.0f * velpb });
        DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 1.0f));

        DebugDrawer::setVertexData({ c.point, c.point + 10.0f * (velpa - velpb) });
        DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 1.0f));

        //DebugDrawer::setVertexData({ c.point, c.point + 5.0f * c.normal });
        //DebugDrawer::draw(camera, glm::vec3(0.5f, 0.5f, 0.5f));
       
        glm::vec3* distanceVector = nullptr;
        if (collidee != nullptr) {
            if (c.vertexFace == true) {
                distanceVector = collidee->vertexFaceContactDepth(*collidee, c);

                if (distanceVector != nullptr) {
                    DebugDrawer::setMode(GL_LINES);
                    DebugDrawer::setOverrideZ(1);
                    DebugDrawer::setVertexData({ c.point, c.point + *distanceVector });
                    DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));
                }
            }
            else {
                distanceVector = collidee->edgeEdgeContactDepth(*collidee, c);

                DebugDrawer::setMode(GL_POINTS);
                DebugDrawer::setOverrideZ(1);

                glm::vec3 rA = c.edgeA[0];
                glm::vec3 xA = colliderObject->getBodyStateSolver()->Body.X;
                //DebugDrawer::setVertexData({rA, xA});
                //DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));

                glm::vec3 rB = c.edgeB[0];
                glm::vec3 xB = collideeObject->getBodyStateSolver()->Body.X;
                /*DebugDrawer::setVertexData({rB, xB});
                DebugDrawer::draw(camera, glm::vec3(1.0f, 1.0f, 1.0f));*/

                if (distanceVector != nullptr) {
                    DebugDrawer::setMode(GL_LINES);
                    DebugDrawer::setOverrideZ(1);
                    DebugDrawer::setVertexData({ rA, rA + *distanceVector });
                    DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));
                }
            }
            delete distanceVector;
        }
    }

    DebugDrawer::setMode(GL_POINTS);
    DebugDrawer::setOverrideZ(1);
    DebugDrawer::setVertexData(contactPoints);
    DebugDrawer::draw(camera, glm::vec3(0.0f, 1.0f, 0.0f));
}

CollisionHandler::contacts CollisionHandler::pushObjectsApart(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData) {
    if (contactData.size() == 0) {
        return contactData;
    }

    CuboidCollider* collider = dynamic_cast<CuboidCollider*>(colliderObject->getCollider());
    CuboidCollider* collidee = dynamic_cast<CuboidCollider*>(collideeObject->getCollider());
    if (collider == nullptr || collidee == nullptr) {
        std::cout << "Cuboid collider cast failed during displacement!" << std::endl;
        return contactData;
    }

    const float eps = 0.05f;
    glm::vec3 displacementVector = glm::vec3(0.0f, 0.0f, 0.0f);
    float max_dist = 0.0f;
    for (const auto& c : contactData) {
        glm::vec3* d = nullptr;

        if (c.vertexFace == true) {
            d = collider->vertexFaceContactDepth(*collidee, c);
        }
        else {
            //d = collider->edgeEdgeContactDepth(*collidee, c);
        }

        if (d != nullptr) {
            float ld = glm::length(*d);
            if (ld > max_dist) {
                max_dist = ld;
                displacementVector = *d;
            }
            delete d;
        }
    }

    if (max_dist > eps) {
        colliderObject->getBodyStateSolver()->Body.X += 0.5f * displacementVector;
        colliderObject->updateTransformations();
    }

    return contactData;
}

void CollisionHandler::applyImpulse(SceneObject* colliderObject, SceneObject* collideeObject, contacts contactData) {
    RigidBody::BodyData* A = &colliderObject->getBodyStateSolver()->Body;
    RigidBody::BodyData* B = &collideeObject->getBodyStateSolver()->Body;
    const float eps = 1.0f;

    for (const auto& c : contactData) {
        glm::vec3 ra = c.point - A->X;
        glm::vec3 rb = c.point - B->X;
        glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
        glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
        float vrelm = glm::dot(c.normal, velpa - velpb);

        if (isCollidingContact(vrelm)) {
            glm::vec3 term1a = A->Iinv * glm::cross(ra, c.normal);
            glm::vec3 term1b = B->Iinv * glm::cross(rb, c.normal);
            float term2a = glm::dot(c.normal, glm::cross(term1a, ra));
            float term2b = glm::dot(c.normal, glm::cross(term1b, rb));

            float numerator = -(1.0f + eps) * vrelm;
            float denominator = A->invMass + B->invMass + term2a + term2b;
            float j = numerator / denominator;

            glm::vec3 impForce = j * c.normal;
            glm::vec3 impTorque = glm::cross(ra, impForce);

            A->P += impForce;
            A->vel = A->P * A->invMass;
            A->L += impTorque;
            A->omega = A->Iinv * A->L;

            B->P -= impForce;
            B->vel = B->P * B->invMass;
            B->L -= impTorque;
            B->omega = B->Iinv * B->L;

            glm::vec3 velpap = velpa + j * A->invMass * c.normal;
            glm::vec3 velpbp = velpb + j * B->invMass * c.normal;
            float vrelp = glm::dot(c.normal, velpap - velpbp);
        }
    }
}

void CollisionHandler::handleCollisions(const std::vector<SceneObject::interaction>& interactions, const Camera& camera) {
    if (interactions.size() == 0) {
        return;
    }

    auto contacts = removeDuplicates(interactions);

    for (const auto& c : contacts) {
        SceneObject* colliderObject = std::get<0>(c);
        SceneObject* collideeObject = std::get<1>(c);

        auto contactData = std::get<2>(c);

        if (SceneObject::getEnableDebug()) {
            debugContacts(colliderObject, collideeObject, contactData, camera);
        }

        applyImpulse(colliderObject, collideeObject, contactData);
        contactData = pushObjectsApart(colliderObject, collideeObject, contactData);
       
       /* if (apply_displacement) {
            pushObjectsApart(colliderObject, collideeObject, contactData);
            apply_displacement = false;
        }

        if (apply_impulse) {
            applyImpulse(colliderObject, collideeObject, contactData);
            apply_impulse = false;
        }*/
    }
}
