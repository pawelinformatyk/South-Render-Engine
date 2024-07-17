#pragma once


namespace South
{

class Camera
{
public:
    Camera() = default;
    Camera(const VectorFlt& Pos, const VectorFlt& Target, float Fov, float Aspect, float Near, float Far);

    void SetView(const VectorFlt& Pos, const VectorFlt& Target);
    void SetProjection(float Fov, float Aspect, float Near, float Far);

    const glm::mat4& GetView() const;
    const glm::mat4& GetProjection() const;
    glm::mat4        GetViewProjection() const;

    glm::quat GetOrientation() const;

    VectorFlt GetForwardVector() const;
    VectorFlt GetRightVector() const;
    VectorFlt GetUpVector() const;

    void MoveForward(float Delta);
    void MoveRight(float Delta);
    void MoveUp(float Delta);

    void LookRight(float Delta);
    void LookUp(float Delta);

private:
    void UpdateView();
    void UpdateDirection();

    glm::mat4 m_View = glm::mat4(1.f);

    VectorFlt m_Position  = VectorFlt::ZeroVector;
    VectorFlt m_Direction = VectorFlt::ZeroVector;

    glm::mat4 m_Projection = glm::mat4(1.f);

    float m_Fov    = 0.f;
    float m_Aspect = 0.f;
    float m_Near   = 0.f;
    float m_Far    = 0.f;

    float m_Pitch = 0.f;
    float m_Yaw   = 0.f;

    float m_MoveSpeed     = 1.5f;
    float m_RotationSpeed = 5.f;
};

} // namespace South