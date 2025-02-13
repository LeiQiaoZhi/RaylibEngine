#ifndef JELLOCOMPONENT_H
#define JELLOCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "ModelComponent.h"
#include "ProceduralMeshComponent.h"
#include "../../utils/MathUtils.h"
#include "../../utils/PhysicsUtils.h"
#include "../../utils/Plane.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/MaterialProperty.h"
#include "../../editor/VectorProperty.h"

class JelloComponent final : public Component {
public:
    explicit JelloComponent() {
        headerProperty.label = "Jello";
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

    void OnDrawGizmosBottom(Scene *scene) const override;

private:
    void ComputeAcceleration();

    void StartSimulation();

    void UpdateMesh();

    std::vector<int> LocalToMeshIndex(int x, int y, int z) const;

    bool ValidPoint(const Vector3 *p) {
        if (!proceduralMeshComponent) return false;
        return p->x >= 0 && p->x < proceduralMeshComponent->cubeSize.x &&
               p->y >= 0 && p->y < proceduralMeshComponent->cubeSize.y &&
               p->z >= 0 && p->z < proceduralMeshComponent->cubeSize.z;
    }

    Vector3 VelocityFromLocal(const Vector3 *p) const {
        return velocities[static_cast<int>(p->x)][static_cast<int>(p->y)][static_cast<int>(p->z)];
    }

    Vector3 LocalToWorld(const Vector3 *p) const {
        return positions[static_cast<int>(p->x)][static_cast<int>(p->y)][static_cast<int>(p->z)];
    }

    Vector3 AccelerationFromNeighbour(const Vector3 *self_local, const Vector3 *neighbour_local, const double rest) const;

    Vector3 AccelerationFromCollision(const Vector3 *self_world, const Vector3 *contact, const Vector3 relative_vel,
                                        const double rest = 0.) const;

private:
    // dependencies
    ModelComponent *modelComponent = nullptr;
    ProceduralMeshComponent *proceduralMeshComponent = nullptr;

    // states
    bool started = false;
    Matrix3D accelerations;
    Matrix3D velocities;
    Matrix3D positions;
    float mass = 1.0f / 512.0f;
    float elasticity = 200;
    float damping = 0.25;
    float collisionElasticity = 400;
    float collisionDamping = 0.25;

    // ui
    std::string warningText;
    float height;
    FoldoutProperty debugFoldout = FoldoutProperty("Debug");

    // consts
    static constexpr Vector3 neighbour_directions[6] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {-1, 0, 0},
        {0, -1, 0},
        {0, 0, -1}
    };
    static constexpr Vector3 shear_neighbour_directions[20] = {
        {1, 1, 0},
        {-1, 1, 0},
        {-1, -1, 0},
        {1, -1, 0},
        {0, 1, 1},
        {0, -1, 1},
        {0, -1, -1},
        {0, 1, -1},
        {1, 0, 1},
        {-1, 0, 1},
        {-1, 0, -1},
        {1, 0, -1},
        {1, 1, 1},
        {-1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1}
    };

    // face outside
    const Vector3 boundingBoxSize = {40, 40, 40};
    const Plane bounding_box_top = Plane({0, 1, 0}, {0, boundingBoxSize.y / 2, 0});
    const Plane bounding_box_bot = Plane({0, -1, 0}, {0, -boundingBoxSize.y / 2, 0});
    const Plane bounding_box_left = Plane({-1, 0, 0}, {-boundingBoxSize.x / 2, 0, 0});
    const Plane bounding_box_right = Plane({1, 0, 0}, {boundingBoxSize.x / 2, 0, 0});
    const Plane bounding_box_front = Plane({0, 0, 1}, {0, 0, boundingBoxSize.z / 2});
    const Plane bounding_box_back = Plane({0, 0, -1}, {0, 0, -boundingBoxSize.z / 2});
    const Plane bounding_box_planes[6] = {
        bounding_box_top, bounding_box_bot, bounding_box_left, bounding_box_right, bounding_box_front, bounding_box_back
    };
};


#endif //JELLOCOMPONENT_H
