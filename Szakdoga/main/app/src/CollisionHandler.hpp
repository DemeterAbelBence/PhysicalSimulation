#pragma once

#include "SceneObject.hpp"
#include "Box.hpp"

#include <algorithm>

class CollisionHandler {
private:
	static std::vector<SceneObject::interaction> removeDuplicates(const std::vector<SceneObject::interaction>& interactions) {
		std::vector<SceneObject::interaction> result;

		auto contains = [&result](SceneObject::interaction i) {
			SceneObject* i_first = std::get<0>(i);
			SceneObject* i_second = std::get<1>(i);

			for (const auto& r : result) {
				SceneObject* r_first = std::get<0>(r);
				SceneObject* r_second = std::get<1>(r);

				if (r_first == i_second && r_second == i_first) {
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

public:
	static bool apply_impulse;

	static bool isCollidingContact(float relativeVelocity) {
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

	static void debugContacts(SceneObject* first, SceneObject* second, const std::vector<Collider::ContactData> contactData, const Camera& camera) {
		RigidBody::BodyData* A = &first->getBodyStateSolver()->Body;
		RigidBody::BodyData* B = &second->getBodyStateSolver()->Body;
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

			/*DebugDrawer::setVertexData({ A->X, A->X + ra });
			DebugDrawer::draw(camera, glm::vec3(1.0f, 1.0f, 1.0f));*/

			DebugDrawer::setVertexData({ c.point, c.point + 5.0f * c.normal });
			DebugDrawer::draw(camera, glm::vec3(0.2f, 0.0f, 0.0f));


			if (isCollidingContact(vrelm)) {
				std::cout << "Colliding contact! vrelm=" << vrelm << std::endl;
			}
			else {
				std::cout << "Not colliding contact! vrelm=" << vrelm << std::endl;
			}
		}

		DebugDrawer::setVertexData(contactPoints);
		DebugDrawer::setMode(GL_POINTS);
		DebugDrawer::setOverrideZ(0);
		DebugDrawer::draw(camera, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	static void applyImpulse(SceneObject* first, SceneObject* second, const std::vector<Collider::ContactData> contactData) {
		RigidBody::BodyData* A = &first->getBodyStateSolver()->Body;
		RigidBody::BodyData* B = &second->getBodyStateSolver()->Body;
		const float eps = 0.8f;
		bool hadCollidingContact;

		do {
			hadCollidingContact = false;
			for (const auto& c : contactData) {
				glm::vec3 ra = c.point - A->X;
				glm::vec3 rb = c.point - B->X;
				glm::vec3 velpa = A->vel;// +glm::cross(A->omega, ra);
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

					if (vrelp < -0.00001f) {
						std::cout << "Incorrect impulse calculation! vrelp=" << vrelp << std::endl;
					}
					hadCollidingContact = true;
				}
			}
		} while (hadCollidingContact);
	}

	static void HandleCollisions(const std::vector<SceneObject::interaction>& interactions, const Camera& camera) {
		if (interactions.size() == 0) {
			return;
		}

		auto contacts = removeDuplicates(interactions);

		for (const auto& c : contacts) {
			SceneObject* first = std::get<0>(c);
			SceneObject* second = std::get<1>(c);

			auto contactData = std::get<2>(c);
			debugContacts(first, second, contactData, camera);
			applyImpulse(first, second, contactData);

			if (apply_impulse) {
				applyImpulse(first, second, contactData);
				apply_impulse = false;
			}
		}
	}
};