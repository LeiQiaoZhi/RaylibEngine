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
#include "../../utils/Sphere.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/SingleValueProperty.h"
#include "../../editor/VectorProperty.h"
#include "../../input/MouseDragState.h"

class JelloComponent final : public Component {
public:
    explicit JelloComponent() {
        headerProperty.label = "Jello";
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnEditorDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void EditorStart() override;

    void EditorUpdate() override;

    void OnDrawGizmosBottom(Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    std::vector<std::type_index> Dependencies() override {
        return {std::type_index(typeid(ModelComponent)), std::type_index(typeid(ProceduralMeshComponent))};
    }

private:
    void ComputeAcceleration();

    void StartSimulation();

    void UpdateMesh();

    float GetMassPerPoint() const {
        if (!proceduralMeshComponent) return 0;
        return totalMass / proceduralMeshComponent->cubeSize.x / proceduralMeshComponent->cubeSize.y /
               proceduralMeshComponent->cubeSize.z;
    }

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

    Vector3 AccelerationFromNeighbour(const Vector3 *self_local, const Vector3 *neighbour_local,
                                      const double rest) const;

    Vector3 AccelerationFromCollision(const Vector3 *self_world, const Vector3 *contact, const Vector3 relative_vel,
                                      const double rest = 0.) const;

    void RK4();

private:
    // dependencies
    ModelComponent *modelComponent = nullptr;
    ProceduralMeshComponent *proceduralMeshComponent = nullptr;

    // states
    enum class IntegrationMethod {
        RK4,
        Euler
    };

    enum class Interaction {
        GlobalImpulse,
        LocalImpulse,
    };

    bool started = false;
    Matrix3D accelerations;
    Matrix3D velocities;
    Matrix3D positions;

    // params
    IntegrationMethod integrationMethod = IntegrationMethod::RK4;
    float totalMass = 2.0f;
    float elasticity = 200;
    float damping = 0.25;
    float collisionElasticity = 400;
    float collisionDamping = 0.25;
    float speed = 1.0f;
    float dragStrength = 0.5;
    float dragStrengthDecay = 0.1;
    float gravity = 9.8;
    float airResistance = 0.1;

    // interaction states
    Interaction interaction = Interaction::LocalImpulse;
    bool pointJustSelected = false;
    Vector3 nearestPointLocal;
    MouseDragState dragState;
    Vector3 dragDir;
    RayCollision selection = {0};

    // ui
    std::string statusText;
    bool statusWarning = false;
    float height;
    FoldoutProperty debugFoldout = FoldoutProperty("Debug", true);
    FloatSlider massProperty = FloatSlider(&totalMass, "Total Mass", 0, 10);
    FloatSlider speedProperty = FloatSlider(&speed, "Speed", 0, 2);
    FloatSlider elasticityProperty = FloatSlider(&elasticity, "Elasticity", 0, 1000);
    FloatSlider dampingProperty = FloatSlider(&damping, "Damping", 0, 1);
    FloatSlider collisionElasticityProperty = FloatSlider(&collisionElasticity, "Col Elasticity", 0, 1000);
    FloatSlider collisionDampingProperty = FloatSlider(&collisionDamping, "Col Damping", 0, 1);
    FloatSlider dragStrengthProperty = FloatSlider(&dragStrength, "Drag Strength", 0, 1);
    FloatSlider dragStrengthDecayProperty = FloatSlider(&dragStrengthDecay, "Drag Decay", 0, 1);
    FloatSlider gravityProperty = FloatSlider(&gravity, "Gravity", 0, 10);
    FloatSlider airResistanceProperty = FloatSlider(&airResistance, "Air Resistance", 0, 1);
    MaterialProperty materialProperty = MaterialProperty(nullptr, 0, this);

    // rk4 states
    Matrix3D k1PositionChanges;
    Matrix3D k1VelocityChanges;
    Matrix3D k2PositionChanges;
    Matrix3D k2VelocityChanges;
    Matrix3D k3PositionChanges;
    Matrix3D k3VelocityChanges;
    Matrix3D k4PositionChanges;
    Matrix3D k4VelocityChanges;

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

    const Sphere sphere = Sphere({10, -10, 10}, 5);
};


#endif //JELLOCOMPONENT_H
