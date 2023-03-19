#pragma once

#include "Camera.h"

namespace South
{
class Camera;

class Editor
{
public:
    Editor();
    ~Editor();

    void Tick();
    void Render();

    void BeginGUI();
    void RenderGUI();
    void EndGUI();

private:
    // #TODO: Part of viewport.
    Camera m_Camera;
};

} // namespace South