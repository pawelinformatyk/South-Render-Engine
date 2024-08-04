#pragma once


namespace South
{

class Camera
{
public:
    Camera();

    glm::mat4 GetViewProjection() const;

    glm::quat GetOrientation() const;

    VectorFlt GetForwardVector() const;
    VectorFlt GetRightVector() const;
    VectorFlt GetUpVector() const;

    void MoveForward(float Delta);
    void MoveRight(float Delta);
    void MoveUp(float Delta);

    void LookRight(float Delta);
    void LookUp(float Delta);

    void UpdateProjection();
    void UpdateView();

    glm::mat4 View = glm::mat4(1.f);

    VectorFlt Location = VectorFlt::ZeroVector;

    float Pitch = 0.f;
    float Yaw   = 0.f;

    glm::mat4 Projection = glm::mat4(1.f);

    float Fov    = glm::radians(60.f);
    float Aspect = 0.f;
    float Near   = 0.1f;
    float Far    = 100000.f;

    float MoveSpeed     = 1.5f;
    float RotationSpeed = 1.f;
};

} // namespace South