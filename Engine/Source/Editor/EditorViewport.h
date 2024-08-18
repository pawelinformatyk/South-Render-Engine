#pragma once

#include "Camera.h"

namespace South
{
class SCamera;

class SEditorViewport
{
public:
    void Tick();

    void RenderGrid();

private:
    SCamera m_Camera;
};

} // namespace South