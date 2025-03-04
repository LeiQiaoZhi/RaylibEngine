#include "JelloComponent.h"

#include <config.h>
#include <iostream>
#include <ostream>
#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "../../editor/Editor.h"
#include "../../GCM/GameObject.h"
#include "../../utils/MathUtils.h"
#include "../../utils/Plane.h"

void JelloComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;
    if (!modelComponent || !proceduralMeshComponent) {
        statusText = "Model Component not found";
        statusWarning = true;
        return;
    }

    proceduralMeshComponent->meshType = ProceduralMeshComponent::MeshType::Cube;
    proceduralMeshComponent->sizeProperty.OnEditorGUI(rect);
    proceduralMeshComponent->worldSizeProperty.OnEditorGUI(rect);

    massProperty.OnEditorGUI(rect);
    speedProperty.OnEditorGUI(rect);
    elasticityProperty.OnEditorGUI(rect);
    dampingProperty.OnEditorGUI(rect);
    collisionElasticityProperty.OnEditorGUI(rect);
    collisionDampingProperty.OnEditorGUI(rect);
    dragStrengthProperty.OnEditorGUI(rect);
    dragStrengthDecayProperty.OnEditorGUI(rect);
    gravityProperty.OnEditorGUI(rect);
    airResistanceProperty.OnEditorGUI(rect);

    int *integrationMethodPtr = reinterpret_cast<int *>(&integrationMethod);
    GuiToggleGroup({rect.x, rect.y, rect.width / 2, Editor::TextSize() * 1.5f},
                   "RK4;Euler", integrationMethodPtr);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    int *interactionPtr = reinterpret_cast<int *>(&interaction);
    GuiToggleGroup({rect.x, rect.y, rect.width / 2, Editor::TextSize() * 1.5f},
                   "Global;Local", interactionPtr);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    const char *buttonText = started ? "Restart" : "Start";
    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 2.0f}, buttonText)) {
        started = !started;
        if (started)
            StartSimulation();
        statusText = started ? "Simulation started" : "Simulation stopped";
        statusWarning = false;
    }
    rect.y += Editor::TextSize() * 2.0f + Editor::SmallGap();

    // warning
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    // material
    GuiLine({rect.x, rect.y, rect.width, Editor::LargeGap() * 1.0f}, nullptr);
    rect.y += Editor::LargeGap();
    materialProperty.OnEditorGUI(rect);

    // debug
    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded() && started) {
        Editor::BeginIndent(rect, Editor::SmallGap() * 2.0f);

        // for [0, 0, 0]
        std::ostringstream oss;
        oss.precision(2);
        oss << "Position: " << positions[0][0][0].x << ", " << positions[0][0][0].y << ", " << positions[0][0][0].z;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
        oss.str("");
        oss << "Velocity: " << velocities[0][0][0].x << ", " << velocities[0][0][0].y << ", " << velocities[0][0][0].z;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
        oss.str("");
        oss << "Acceleration: " << accelerations[0][0][0].x << ", " << accelerations[0][0][0].y << ", " << accelerations
                [0][
                    0][0].z;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();

        Editor::EndIndent(rect, Editor::SmallGap() * 2.0f);
    }

    height = rect.y - originalY;
}

float JelloComponent::GetEditorHeight() const {
    return height;
}

void JelloComponent::OnEditorDraw(Scene *scene) const {
}

void JelloComponent::OnDrawGizmos(Scene *scene) const {
    if (interaction == Interaction::GlobalImpulse) {
        EndMode3D();
        dragState.OnDrawGizmos(this);
        BeginMode3D(*scene->GetMainCamera()->GetRaylibCamera());
    }

    if (interaction == Interaction::LocalImpulse && selection.hit && dragState.isDragging) {
        const Vector3 selectionPos = LocalToWorld(&nearestPointLocal);
        DrawSphere(selectionPos, 0.5f, YELLOW);
        DrawLine3D(selectionPos, selectionPos + dragDir * 0.05f, YELLOW);
    }
}

void JelloComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void JelloComponent::EditorStart() {
    modelComponent = gameObject->GetComponent<ModelComponent>();
    if (!modelComponent) {
        statusText = "Model Component not found";
        statusWarning = true;
        return;
    }
    proceduralMeshComponent = gameObject->GetComponent<ProceduralMeshComponent>();
    if (!proceduralMeshComponent) {
        statusText = "Procedural Mesh Component not found";
        statusWarning = true;
        return;
    }
}

void JelloComponent::EditorUpdate() {
    if (!started) return;

    dragState.Update();

    Camera *camera = gameObject->scene->GetMainCamera()->GetRaylibCamera();
    if (interaction == Interaction::LocalImpulse && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        const Ray ray = gameObject->scene->GetMouseScreenToWorldRay();
        selection = RaylibUtils::GetRayCollisionModel(ray, *modelComponent->model);
        pointJustSelected = selection.hit;
        std::cout << selection.hit << std::endl;
    } else {
        pointJustSelected = false;
    }

    // compute drag direction
    const Vector3 up = GetCameraUp(camera);
    const Vector3 right = GetCameraRight(camera);
    const Vector2 dragScreenDir = GetMousePosition() - dragState.startDragPosition;
    dragDir = right * dragScreenDir.x + up * -dragScreenDir.y;

    // physics
    if (integrationMethod == IntegrationMethod::RK4) {
        RK4();
    } else {
        ComputeAcceleration();
        PhysicsUtils::Euler(positions, velocities, accelerations, GetFrameTime() * speed * 0.1);
    }

    UpdateMesh();
}

void JelloComponent::ComputeAcceleration() {
    const int vertexCounts[3] = {
        static_cast<int>(proceduralMeshComponent->cubeSize.x),
        static_cast<int>(proceduralMeshComponent->cubeSize.y),
        static_cast<int>(proceduralMeshComponent->cubeSize.z)
    };

    float minDistanceToSelection = 1000000;
    for (int i = 0; i < vertexCounts[0]; i++)
        for (int j = 0; j < vertexCounts[1]; j++)
            for (int k = 0; k < vertexCounts[2]; k++) {
                accelerations[i][j][k] = {0, 0, 0};

                // external input
                if (interaction == Interaction::GlobalImpulse && dragState.JustFinishedDragging()) {
                    accelerations[i][j][k] += dragDir * 50 * dragStrength / GetFrameTime();
                }
                if (interaction == Interaction::LocalImpulse && pointJustSelected) {
                    const float d = Vector3DistanceSqr(selection.point, positions[i][j][k]);
                    if (d < minDistanceToSelection) {
                        minDistanceToSelection = d;
                        nearestPointLocal = {static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)};
                    }
                }
                if (interaction == Interaction::LocalImpulse && dragState.JustFinishedDragging() && selection.hit) {
                    const float d = Vector3DistanceSqr(positions[i][j][k], LocalToWorld(&nearestPointLocal));
                    const float decay = exp(-d * dragStrengthDecay);
                    accelerations[i][j][k] += dragDir * 100 * dragStrength * decay / GetFrameTime();
                }

                // gravity
                accelerations[i][j][k].y -= 100 * gravity;
                // air resistance
                accelerations[i][j][k] -= velocities[i][j][k] * airResistance * 100;

                Vector3 current_local = {static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)};
                Vector3 neighbour_local;
                // structural
                for (int n = 0; n < 6; n++) {
                    neighbour_local = neighbour_directions[n] + current_local;
                    if (!ValidPoint(&neighbour_local)) continue;;
                    const Vector3 acc = AccelerationFromNeighbour(&current_local, &neighbour_local,
                                                                  proceduralMeshComponent->cubeWorldSize.x * 1.0 / (
                                                                      vertexCounts[0] - 1));
                    accelerations[i][j][k] += acc;
                }
                // shear
                for (int n = 0; n < 20; n++) {
                    neighbour_local = shear_neighbour_directions[n] + current_local;
                    if (!ValidPoint(&neighbour_local)) continue;
                    const Vector3 acc = AccelerationFromNeighbour(&current_local, &neighbour_local,
                                                                  proceduralMeshComponent->cubeWorldSize.x * sqrt(2.0) /
                                                                  (vertexCounts[0] - 1));
                    accelerations[i][j][k] += acc;
                }
                // bend
                for (int n = 0; n < 6; n++) {
                    neighbour_local = neighbour_directions[n] * 2 + current_local;
                    if (!ValidPoint(&neighbour_local)) continue;
                    const Vector3 acc = AccelerationFromNeighbour(&current_local, &neighbour_local,
                                                                  proceduralMeshComponent->cubeWorldSize.x * 2.0 / (
                                                                      vertexCounts[0] - 1));
                    accelerations[i][j][k] += acc;
                }

                // collision
                const Vector3 self_world = LocalToWorld(&current_local);
                const Vector3 relative_vel = VelocityFromLocal(&current_local);
                for (int p = 0; p < 6; p++) {
                    Plane bounding_box_plane = bounding_box_planes[p];
                    const double dist = bounding_box_plane.distanceWithSign(self_world);
                    // collision
                    if (dist > 0) {
                        Vector3 contact = bounding_box_plane.closestPoint(self_world);
                        const Vector3 acc = AccelerationFromCollision(&self_world, &contact, relative_vel);
                        accelerations[i][j][k] += acc;
                    }
                }
                const double dist = sphere.distanceWithSign(self_world);
                if (dist < 0) {
                    std::cout << "collision with sphere" << std::endl;
                    Vector3 contact = sphere.closestPoint(self_world);
                    const Vector3 acc = AccelerationFromCollision(&self_world, &contact, relative_vel);
                    accelerations[i][j][k] += acc;
                }
            }
}

void JelloComponent::StartSimulation() {
    proceduralMeshComponent->GenerateMesh();
    materialProperty.SetModel(modelComponent->model);
    materialProperty.LoadMaterial();

    int vertexCounts[3] = {
        static_cast<int>(proceduralMeshComponent->cubeSize.x),
        static_cast<int>(proceduralMeshComponent->cubeSize.y),
        static_cast<int>(proceduralMeshComponent->cubeSize.z)
    };

    positions = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    velocities = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    accelerations = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k1PositionChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k1VelocityChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k2PositionChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k2VelocityChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k3PositionChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k3VelocityChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k4PositionChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);
    k4VelocityChanges = MathUtils::CreateMatrix3D(vertexCounts[0], vertexCounts[1], vertexCounts[2]);

    for (int x = 0; x < vertexCounts[0]; x++) {
        for (int y = 0; y < vertexCounts[1]; y++) {
            for (int z = 0; z < vertexCounts[2]; z++) {
                accelerations[x][y][z] = {0, 0, 0};
                velocities[x][y][z] = {200, 0, 0};
                positions[x][y][z] = {
                    x * proceduralMeshComponent->cubeWorldSize.x / (vertexCounts[0] - 1),
                    y * proceduralMeshComponent->cubeWorldSize.y / (vertexCounts[1] - 1),
                    z * proceduralMeshComponent->cubeWorldSize.z / (vertexCounts[2] - 1)
                };
                positions[x][y][z] -= proceduralMeshComponent->cubeWorldSize / 2;
            }
        }
    }
}

void JelloComponent::UpdateMesh() {
    Model *model = modelComponent->model;
    if (!model) return;

    int vertexCounts[3] = {
        static_cast<int>(proceduralMeshComponent->cubeSize.x),
        static_cast<int>(proceduralMeshComponent->cubeSize.y),
        static_cast<int>(proceduralMeshComponent->cubeSize.z)
    };
    for (int x = 0; x < vertexCounts[0]; x++) {
        for (int y = 0; y < vertexCounts[1]; y++) {
            for (int z = 0; z < vertexCounts[2]; z++) {
                for (int index: LocalToMeshIndex(x, y, z)) {
                    model->meshes[0].vertices[3 * index] = positions[x][y][z].x;
                    model->meshes[0].vertices[3 * index + 1] = positions[x][y][z].y;
                    model->meshes[0].vertices[3 * index + 2] = positions[x][y][z].z;
                }
            }
        }
    }
    UpdateMeshBuffer(model->meshes[0], 0, model->meshes[0].vertices, model->meshes[0].vertexCount * sizeof(float) * 3,
                     0);

    RaylibUtils::RecalculateMeshNormals(&model->meshes[0]);
}

std::vector<int> JelloComponent::LocalToMeshIndex(int x, int y, int z) const {
    std::vector<int> indices = {};
    const int vertexCounts[3] = {
        static_cast<int>(proceduralMeshComponent->cubeSize.x),
        static_cast<int>(proceduralMeshComponent->cubeSize.y),
        static_cast<int>(proceduralMeshComponent->cubeSize.z)
    };
    int verticesAdded = 0;
    // Bottom
    if (y == 0) {
        indices.push_back(x * vertexCounts[2] + z + verticesAdded);
    }
    verticesAdded += vertexCounts[0] * vertexCounts[2];
    // Top
    if (y == vertexCounts[1] - 1) {
        indices.push_back(x * vertexCounts[2] + z + verticesAdded);
    }
    verticesAdded += vertexCounts[0] * vertexCounts[2];
    // Left
    if (x == 0) {
        indices.push_back(y * vertexCounts[2] + z + verticesAdded);
    }
    verticesAdded += vertexCounts[1] * vertexCounts[2];
    // Right
    if (x == vertexCounts[0] - 1) {
        indices.push_back(y * vertexCounts[2] + z + verticesAdded);
    }
    verticesAdded += vertexCounts[1] * vertexCounts[2];
    // Front
    if (z == 0) {
        indices.push_back(x * vertexCounts[1] + y + verticesAdded);
    }
    verticesAdded += vertexCounts[0] * vertexCounts[1];
    // Back
    if (z == vertexCounts[2] - 1) {
        indices.push_back(x * vertexCounts[1] + y + verticesAdded);
    }
    return indices;
}

Vector3 JelloComponent::AccelerationFromNeighbour(const Vector3 *self_local, const Vector3 *neighbour_local,
                                                  const double rest) const {
    const Vector3 neighbour_world = LocalToWorld(neighbour_local);
    const Vector3 self_world = LocalToWorld(self_local);
    const Vector3 hooke_f = PhysicsUtils::Hooke(&self_world, &neighbour_world, rest, elasticity);
    const Vector3 hooke_a = hooke_f / GetMassPerPoint();

    const Vector3 relative_vel = VelocityFromLocal(self_local) - VelocityFromLocal(neighbour_local);
    const Vector3 damp_f = PhysicsUtils::SpringDamp(&self_world, &neighbour_world, relative_vel, damping);
    const Vector3 damp_a = damp_f / GetMassPerPoint();

    return hooke_a + damp_a;
}

Vector3 JelloComponent::AccelerationFromCollision(const Vector3 *self_world, const Vector3 *contact,
                                                  const Vector3 relative_vel, const double rest) const {
    const Vector3 hooke_f = PhysicsUtils::Hooke(self_world, contact, rest, collisionElasticity);
    const Vector3 hooke_a = hooke_f / GetMassPerPoint();
    const Vector3 damp_f = PhysicsUtils::SpringDamp(self_world, contact, relative_vel, collisionDamping);
    const Vector3 damp_a = damp_f / GetMassPerPoint();
    return hooke_a + damp_a;
}

void JelloComponent::RK4() {
    const int vertexCounts[3] = {
        static_cast<int>(proceduralMeshComponent->cubeSize.x),
        static_cast<int>(proceduralMeshComponent->cubeSize.y),
        static_cast<int>(proceduralMeshComponent->cubeSize.z)
    };
    const float dt = GetFrameTime() * speed * 0.1;
    const Matrix3D startPositions = positions;
    const Matrix3D startVelocities = velocities;

    ComputeAcceleration();
    for (int i = 0; i < vertexCounts[0]; i++)
        for (int j = 0; j < vertexCounts[1]; j++)
            for (int k = 0; k < vertexCounts[2]; k++) {
                k1PositionChanges[i][j][k] = velocities[i][j][k] * dt;
                k1VelocityChanges[i][j][k] = accelerations[i][j][k] * dt;
                positions[i][j][k] = startPositions[i][j][k] + k1PositionChanges[i][j][k] / 2;
                velocities[i][j][k] = startVelocities[i][j][k] + k1VelocityChanges[i][j][k] / 2;
            }

    ComputeAcceleration();
    for (int i = 0; i < vertexCounts[0]; i++)
        for (int j = 0; j < vertexCounts[1]; j++)
            for (int k = 0; k < vertexCounts[2]; k++) {
                k2PositionChanges[i][j][k] = velocities[i][j][k] * dt;
                k2VelocityChanges[i][j][k] = accelerations[i][j][k] * dt;
                positions[i][j][k] = startPositions[i][j][k] + k2PositionChanges[i][j][k] / 2;
                velocities[i][j][k] = startVelocities[i][j][k] + k2VelocityChanges[i][j][k] / 2;
            }

    ComputeAcceleration();
    for (int i = 0; i < vertexCounts[0]; i++)
        for (int j = 0; j < vertexCounts[1]; j++)
            for (int k = 0; k < vertexCounts[2]; k++) {
                k3PositionChanges[i][j][k] = velocities[i][j][k] * dt;
                k3VelocityChanges[i][j][k] = accelerations[i][j][k] * dt;
                positions[i][j][k] = startPositions[i][j][k] + k3PositionChanges[i][j][k];
                velocities[i][j][k] = startVelocities[i][j][k] + k3VelocityChanges[i][j][k];
            }

    ComputeAcceleration();
    for (int i = 0; i < vertexCounts[0]; i++)
        for (int j = 0; j < vertexCounts[1]; j++)
            for (int k = 0; k < vertexCounts[2]; k++) {
                k4PositionChanges[i][j][k] = velocities[i][j][k] * dt;
                k4VelocityChanges[i][j][k] = accelerations[i][j][k] * dt;
                positions[i][j][k] = startPositions[i][j][k] + (
                                         k1PositionChanges[i][j][k] + k2PositionChanges[i][j][k] * 2
                                         + k3PositionChanges[i][j][k] * 2 + k4PositionChanges[i][j][k]
                                     ) / 6;
                velocities[i][j][k] = startVelocities[i][j][k] + (
                                          k1VelocityChanges[i][j][k] + k2VelocityChanges[i][j][k] * 2
                                          + k3VelocityChanges[i][j][k] * 2 + k4VelocityChanges[i][j][k]
                                      ) / 6;
            }
}

nlohmann::json JelloComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "JelloComponent";
    j["integrationMethod"] = static_cast<int>(integrationMethod);
    j["totalMass"] = totalMass;
    j["elasticity"] = elasticity;
    j["damping"] = damping;
    j["collisionElasticity"] = collisionElasticity;
    j["collisionDamping"] = collisionDamping;
    j["speed"] = speed;
    j["dragStrength"] = dragStrength;
    j["dragStrengthDecay"] = dragStrengthDecay;
    j["interaction"] = static_cast<int>(interaction);
    j["material"] = materialProperty.ToJson();
    return j;
}

void JelloComponent::FromJson(const nlohmann::json &json) {
    integrationMethod = static_cast<IntegrationMethod>(json.value("integrationMethod",
                                                                  static_cast<int>(integrationMethod)));
    totalMass = json.value("totalMass", totalMass);
    elasticity = json.value("elasticity", elasticity);
    damping = json.value("damping", damping);
    collisionElasticity = json.value("collisionElasticity", collisionElasticity);
    collisionDamping = json.value("collisionDamping", collisionDamping);
    speed = json.value("speed", speed);
    dragStrength = json.value("dragStrength", dragStrength);
    dragStrengthDecay = json.value("dragStrengthDecay", dragStrengthDecay);
    interaction = static_cast<Interaction>(json.value("interaction", static_cast<int>(interaction)));
    materialProperty.FromJson(json.value("material", materialProperty.ToJson()));
}


void JelloComponent::OnDrawGizmosBottom(Scene *scene) const {
}
