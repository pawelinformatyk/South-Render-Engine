#pragma once

#include "Camera.h"

namespace South
{
class Camera;

class EditorViewport
{
public:
    void Tick();

private:
    Camera m_Camera;
};

} // namespace South
