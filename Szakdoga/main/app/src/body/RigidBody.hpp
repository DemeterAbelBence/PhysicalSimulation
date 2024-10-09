#include <glm.hpp>

namespace RigidBody {
	struct BodyData {
		float invMass = 0.0f;
        glm::mat3 Ibody = glm::mat3(1.0f);
        glm::mat3 Ibodyinv = glm::mat3(1.0f);

        glm::vec3 X = glm::vec3(0.0f);
        glm::mat3 R = glm::mat3(1.0f);
        glm::vec3 P = glm::vec3(0.0f);
        glm::vec3 L = glm::vec3(0.0f);

        glm::mat3 Iinv = glm::mat3(1.0f);
        glm::vec3 vel = glm::vec3(0.0f);
        glm::vec3 omega = glm::vec3(0.0f);

        glm::vec3 force = glm::vec3(0.0f);
        glm::vec3 torque = glm::vec3(0.0f);
	};

    static glm::mat3 star(glm::vec3 a) {
        return glm::mat3(
            glm::vec3(0.0f, -a[2], a[1]),
            glm::vec3(a[2], 0.0f, -a[0]),
            glm::vec3(-a[1], a[0], 0.0f)
        );
    }

    class BodyStateSolver {

    public:
        BodyData Body;
        BodyData Initial;
        BodyData Previous;

        bool ContactOccured = false;

    public:
        BodyStateSolver(BodyData _Body) : Body( _Body ) {}

        void rollbackToInitial() {
            Body = Initial; 
            ContactOccured = false;
        }

        void rollbackToPrevious() {
            Body = Previous;
        }

        void makeStateInitial() {
            Initial = Body;
        }

        void computeTotalTorque(const std::vector<glm::vec3>& particles) {
            glm::vec3 torque = glm::vec3(0.0f);
            for (const auto& p : particles) {
                torque += glm::cross(p - Body.X, Body.force);
            }
            Body.torque = 0.1f * torque;
        }

        void updateAngularComponents(float t) {
           // Body.L += Body.torque * t;
            Body.Iinv = Body.R * Body.Ibodyinv * glm::transpose(Body.R);
            Body.omega = Body.Iinv * Body.L;
            Body.R += star(Body.omega) * Body.R * t;
        }

        void updateLinearComponents(float t) {
            Body.P += Body.force * t;
            Body.vel = Body.invMass * Body.P;
            Body.X += Body.vel * t;
        }

        void updateState(float t) {
            Previous = Body;
            updateAngularComponents(t);
            updateLinearComponents(t);
        }
    };
}
