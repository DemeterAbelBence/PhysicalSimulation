#include "CuboidCollider.hpp"

void CuboidCollider::createCorners() {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float l = length / 2.0f;

    basePoints[0] = glm::vec3(-w, -h, l);
    basePoints[1] = glm::vec3(w, -h, l);
    basePoints[2] = glm::vec3(w, -h, -l);
    basePoints[3] = glm::vec3(-w, -h, -l);

    basePoints[4] = glm::vec3(-w, h, l);
    basePoints[5] = glm::vec3(w, h, l);
    basePoints[6] = glm::vec3(w, h, -l);
    basePoints[7] = glm::vec3(-w, h, -l);

    transPoints = basePoints;
}

CuboidCollider::Side CuboidCollider::createSide(const std::array<unsigned int, 4>& indices) const {
    std::array<glm::vec3, 4> points;
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < indices.size(); ++i) {
        points[i] = basePoints[indices[i]];
        center += points[i];
    }
    center /= static_cast<float>(indices.size());

    return { points, center, glm::normalize(center) };
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

    for (int i = 0; i < 6; ++i) {
        baseSides[i] = createSide(sideIndices[i]);
        transSides[i] = createSide(sideIndices[i]);
    }     
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::vector<glm::vec3> CuboidCollider::getEdgesOf(const CuboidCollider& collider) const {
    auto p = collider.transPoints;
    std::vector<glm::vec3> edges;

    edges.push_back(p[0]); edges.push_back(p[1]);
    edges.push_back(p[1]); edges.push_back(p[2]);
    edges.push_back(p[2]); edges.push_back(p[3]);
    edges.push_back(p[3]); edges.push_back(p[0]);

    edges.push_back(p[4]); edges.push_back(p[5]);
    edges.push_back(p[5]); edges.push_back(p[6]);
    edges.push_back(p[6]); edges.push_back(p[7]);
    edges.push_back(p[7]); edges.push_back(p[4]);

    edges.push_back(p[0]); edges.push_back(p[4]);
    edges.push_back(p[1]); edges.push_back(p[5]);
    edges.push_back(p[2]); edges.push_back(p[6]);
    edges.push_back(p[3]); edges.push_back(p[7]);

    return edges;
}

glm::vec3* CuboidCollider::calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const {
    using namespace glm;
    float areParallel = dot(va, vb);
    const float eps = glm::pow(10, -3);

    if (areParallel + eps >= 1.0f && areParallel - eps <= 1.0f) {
        return nullptr;
    }
    if (areParallel + eps >= -1.0f && areParallel - eps <= -1.0f) {
        return nullptr;
    }

    if (va == vec3(0.0f, 0.0f, 0.0f) || vb == vec3(0.0f, 0.0f, 0.0f)) {
        std::cout << "Edge direction vector is zero" << std::endl;
        return nullptr;
    }

    vec3 c = cross(va, vb);
    if (c == vec3(0.0f, 0.0f, 0.0f)) {
        return nullptr;
    }

    float dist = glm::length(dot(pb - pa, c)) / glm::length(c);
    if (dist > 0.1f) {
        return nullptr;
    }
    else {
        float PA = dot(pb, va) - dot(pa, va);
        float PB = dot(pa, vb) - dot(pb, vb);
        float VBA = dot(vb, va);
        float VBB = dot(vb, vb);
        float VAA = dot(va, va);
        float beta = (-PB - (PA * VBA) / VAA) / (VBA * VBA / VAA - VBB);
        float alpha = (beta * VBA + PA) / VAA;
        
        if (!(0 <= alpha && alpha <= a)) {
            return nullptr;
        }
        if (!(0 <= beta && beta <= b)) {
            return nullptr;
        }

        vec3 ra = pa + alpha * va;
        vec3 rb = pb + beta * vb;
        return new vec3((ra + rb) / 2.0f);
    }
}

std::vector<Collider::ContactData> CuboidCollider::edgeEdgeCollision(const CuboidCollider& collider) const {
    std::vector<Collider::ContactData> result;
    std::vector<glm::vec3> edgesA = getEdgesOf(*this);
    std::vector<glm::vec3> edgesB = getEdgesOf(collider);

    for (int i = 0; i < edgesA.size(); i += 2) {
        glm::vec3 vecA = edgesA[i + 1] - edgesA[i];
        float a = glm::length(vecA); 

        for (int j = 0; j < edgesB.size(); j += 2) {
            glm::vec3 vecB = edgesB[j + 1] - edgesB[j];
            float b = glm::length(vecB);

            vecA = glm::normalize(vecA);
            vecB = glm::normalize(vecB);

            glm::vec3* inter = calculateEdgeIntersection(edgesA[i], vecA, a, edgesB[j], vecB, b);
            if (inter != nullptr) {   
                result.push_back({
                    *inter,
                    glm::normalize(glm::cross(vecA, vecB)),
                    vecA,
                    vecB,
                    false
                });
                delete inter;
            }
        }
    }

    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

bool CuboidCollider::calculateSideIntersection(glm::vec3 point, const Side& side) const {
    struct edge {
        glm::vec3 edgePoint;
        glm::vec3 edgeVector;
    };

    const float eps = 0.2f;//glm::pow(10.0f, -1);
    float arePerpendicular = glm::dot(side.normal, point - side.points[0]);
    if (!(-eps <= arePerpendicular && arePerpendicular <= eps)) {
        return false;
    }
    else {
        std::vector<edge> edges;
        edges.push_back({ side.points[0], side.points[1] - side.points[0] });
        edges.push_back({ side.points[1], side.points[2] - side.points[1] });
        edges.push_back({ side.points[2], side.points[3] - side.points[2] });
        edges.push_back({ side.points[3], side.points[0] - side.points[3] });

        for (const auto& e : edges) {
            glm::vec3 ev = glm::normalize(e.edgeVector);
            glm::vec3 pv = glm::normalize(point - e.edgePoint);
            float d = glm::dot(ev, pv);
            if (!(0.0f <= d && d <= 1.0f)) {
                return false;
            }
        }
        return true;
    }
}

std::vector<Collider::ContactData> CuboidCollider::vertexFaceCollision(const CuboidCollider& collider) const {
    std::vector<Collider::ContactData> result;

    for (const Side& side : collider.getTransSides()) {
        for (const glm::vec3& point : transPoints) {
            if (calculateSideIntersection(point, side)) {
                result.push_back({
                    point,
                    side.normal,
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    true
                });
            }
        }
    }

    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

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

    basePoints = c.basePoints;
    baseSides = c.baseSides;

    transPoints = c.transPoints;
    transSides = c.transSides;

    transformation = c.transformation;
}

std::vector<glm::vec3> CuboidCollider::getSideDrawData(unsigned int sideIndex)
{
    std::vector<glm::vec3> result;
    auto points = transSides[sideIndex].points;
    glm::vec3 center = transSides[sideIndex].center;
    glm::vec3 normal = transSides[sideIndex].normal;

    for (glm::vec3& point : points) {
        point += (0.2f * normal); 
    }
    result.push_back(center); result.push_back(center + 1.5f * normal);

    result.push_back(points[0]); result.push_back(points[1]);
    result.push_back(points[1]); result.push_back(points[2]);
    result.push_back(points[2]); result.push_back(points[3]);
    result.push_back(points[3]); result.push_back(points[0]);
    
    return result;
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const CuboidCollider& collider) const {

    auto edgeEdge = edgeEdgeCollision(collider);
    auto vertexFace = vertexFaceCollision(collider);
    edgeEdge.insert(edgeEdge.end(), vertexFace.begin(), vertexFace.end());
    return edgeEdge;
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const SphereCollider& collider) const {
    return std::vector<ContactData>();
}

void CuboidCollider::updateTransformations() {
    glm::mat4 M = transformation->makeModelMatrix();
    glm::mat4 MI = transformation->makeModelInverseMatrix();

    for (int i = 0; i < transPoints.size(); ++i) {
        transPoints[i] = glm::vec3(glm::vec4(basePoints[i], 1.0f) * M);
    }

    for (int i = 0; i < transSides.size(); ++i) {
        for (int j = 0; j < transSides[i].points.size(); ++j) {
            glm::vec3 newPoint = glm::vec3(glm::vec4(baseSides[i].points[j], 1.0f) * M);
            transSides[i].points[j] = newPoint;
        }
        transSides[i].center = glm::vec3(glm::vec4(baseSides[i].center, 1.0f) * M);
        transSides[i].normal = glm::vec3(MI * glm::vec4(baseSides[i].normal, 0.0f));
    }
}
