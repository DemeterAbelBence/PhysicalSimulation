#include "CuboidCollider.hpp"

void CuboidCollider::createCorners() {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float l = length / 2.0f;

    points[0] = glm::vec3(-w, -h, l);
    points[1] = glm::vec3(w, -h, l);
    points[2] = glm::vec3(w, -h, -l);
    points[3] = glm::vec3(-w, -h, -l);

    points[4] = glm::vec3(-w, h, l);
    points[5] = glm::vec3(w, h, l);
    points[6] = glm::vec3(w, h, -l);
    points[7] = glm::vec3(-w, h, -l);
}

CuboidCollider::Side CuboidCollider::createSide(const std::array<unsigned int, 4>& indices) const {
    glm::vec3 sideCenter = glm::vec3(0.0f);
    for (unsigned int i : indices)
        sideCenter += points[i];
    sideCenter /= static_cast<float>(indices.size());

    return { sideCenter, glm::normalize(sideCenter - calculateCenter()) };
}

void CuboidCollider::createSides() {
    std::array<unsigned int, 4> sideIndices[6] =
    {
        {0, 1, 2, 3},
        {4, 5, 6, 7},

        {0, 3, 7, 4},
        {1, 2, 6, 5},

        {0, 1, 5, 4},
        {3, 2, 6, 7}
    };

    for (int i = 0; i < 6; ++i)
        sides[i] = createSide(sideIndices[i]);
}

glm::vec3* CuboidCollider::calculateEdgeContact(const CuboidCollider& c1, const CuboidCollider& c2) const  {
    auto points = c2.getPoints();
    std::vector<glm::vec3> edges;

    edges.push_back(points[0]); edges.push_back(points[1]);
    edges.push_back(points[1]); edges.push_back(points[2]);
    edges.push_back(points[2]); edges.push_back(points[3]);
    edges.push_back(points[3]); edges.push_back(points[0]);

    edges.push_back(points[4]); edges.push_back(points[5]);
    edges.push_back(points[5]); edges.push_back(points[6]);
    edges.push_back(points[6]); edges.push_back(points[7]);
    edges.push_back(points[7]); edges.push_back(points[4]);

    edges.push_back(points[0]); edges.push_back(points[4]);
    edges.push_back(points[1]); edges.push_back(points[5]);
    edges.push_back(points[2]); edges.push_back(points[6]);
    edges.push_back(points[3]); edges.push_back(points[7]);

    for (int i = 0; i < edges.size(); i += 2) {
        glm::vec3 e = edges[i + 1] - edges[i];
        float l = glm::length(e);
        e = glm::normalize(e);

        float eps = l / 10.0f;
        float t = 0.0f;
        while (t < l) {
            glm::vec3 p = edges[i] + t * e;

            bool w = glm::abs(p.x) < c1.getWidth() / 2.0f;
            bool h = glm::abs(p.y) < c1.getHeight() / 2.0f;
            bool l = glm::abs(p.z) < c1.getLength() / 2.0f;

            if (w && h && l)
                return new glm::vec3(p);

            t += eps;
        }
    }
    return nullptr;
}

float CuboidCollider::distanceFromSide(const Side& side, const glm::vec3& point) const {
    using namespace glm;

    vec3 n = side.normal;
    vec3 v = point - side.center;

    float a1 = glm::abs(dot(n, v));
    float a2 = glm::abs(dot(n, n));
    float k = a1 / a2;

    return k;
}

CuboidCollider::CuboidCollider(float w, float h, float l) {
    width = w;
    height = h;
    length = l;

    type = Type::C_CUBOID;

    createCorners();
    createSides();
}

CuboidCollider::CuboidCollider(const CuboidCollider& c) {
    type = c.type;

    width = c.width;
    height = c.height;
    length = c.length;

    points = c.points;
    sides = c.sides;

    transformation = c.transformation;
}

glm::vec3 CuboidCollider::calculateCenter() const {
    glm::vec3 result = glm::vec3(0.0f);

    for (const auto& p : points)
        result += p;

    return result / static_cast<float>(points.size());
}

CuboidCollider::Side CuboidCollider::getClosestSide(const glm::vec3& point, bool insideCube) const {
    Side result;

    if(insideCube) {
        float min_dist = 100.0f;
        Side result;
        for (auto& s : sides) {
            float d = distanceFromSide(s, point);
            if(d < min_dist) {
                min_dist = d;
                result = s;
            }
        }
        return result;
    }
    else {
        for (auto& s : sides) {
            glm::vec3 v = point - s.center;
            float d = glm::dot(v, s.normal);
            if (d > 0)
                return s;
        }
    }
}

Collider::CollisionData* CuboidCollider::collidesWith(const CuboidCollider& collider) const {
    auto createCopies = [&](std::array<CuboidCollider*, 2>& a, const Transformation& t) {
        a[0] = new CuboidCollider(*this);
        a[1] = new CuboidCollider(collider);
        for (auto c : a) {
            c->translate(-t.getTranslation());
            c->rotate(t.getRotationAxis(), -t.getRotationAngle());
        }
    };

    auto calculateDepth = [](const CuboidCollider& collider, const Side& side) -> float {
        float depth = 0.0f;
        for (auto p : collider.getPoints()) {
            if (glm::dot(p - side.center, side.normal) < 0.0f) {
                float d = collider.distanceFromSide(side, p);
                if (d > depth)
                    depth = d;
            }
        }
        return depth;
    };

    std::array<CuboidCollider*, 2> thisInOrigin; createCopies(thisInOrigin, transformation);
    std::array<CuboidCollider*, 2> colliderInOrigin; createCopies(colliderInOrigin, collider.transformation);

    glm::vec3* collisionResult0 = calculateEdgeContact(*thisInOrigin[0], *thisInOrigin[1]);
    glm::vec3* collisionResult1 = calculateEdgeContact(*colliderInOrigin[1], *colliderInOrigin[0]);

    glm::vec3 contactPoint;
    float depth = 0.0f;

    if (collisionResult0) {
        Side side = thisInOrigin[0]->getClosestSide(*collisionResult0, true);
        depth = calculateDepth(*thisInOrigin[1], side);
        contactPoint = *collisionResult0;
        contactPoint += transformation.getTranslation();
    }
    else {
        if (collisionResult1) {
            Side side = colliderInOrigin[1]->getClosestSide(*collisionResult1, true);
            depth = calculateDepth(*colliderInOrigin[0], side);
            contactPoint = *collisionResult1;
            contactPoint += collider.transformation.getTranslation();
        }
        else
            return nullptr;
    }

    delete collisionResult0;
    delete collisionResult1;
    
    Side side = collider.getClosestSide(contactPoint, true);
    return new CollisionData(contactPoint, side.normal, depth);
}

Collider::CollisionData* CuboidCollider::collidesWith(const SphereCollider& collider) const {
    CuboidCollider* copy = new CuboidCollider(*this);
    copy->translate(-transformation.getTranslation());
    copy->rotate(transformation.getRotationAxis(), -transformation.getRotationAngle());

    glm::vec3 p = collider.getPosition();
    float r = collider.getRadius();
    p -= transformation.getTranslation();

    float w = glm::abs(p.x) - r;
    float h = glm::abs(p.y) - r;
    float l = glm::abs(p.z) - r;

    if (w < width / 2.0f && h < height / 2.0f && l < length / 2.0f) {
        return new CollisionData(glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
    }

    delete copy;
    return nullptr;
}

void CuboidCollider::translate(glm::vec3 t) {
    transformation.translate(t);
    for (auto& p : points)
        p += t;

    for (auto& side : sides)
        side.center += t;

}

void CuboidCollider::scale(float s) {
    transformation.scale(s);
    for (auto& p : points)
        p *= s;

    for (auto& side : sides)
        side.center *= s;
}

void CuboidCollider::rotate(glm::vec3 axis, float angle) {
    transformation.setRotationAxis(axis);
    transformation.rotate(angle);
}
