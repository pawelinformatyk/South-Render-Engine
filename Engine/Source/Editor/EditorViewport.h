#pragma once

#include "SCamera.h"

namespace South
{
class SCamera;

class EditorViewport
{
public:
    void Tick();

    void RenderGrid();

private:
    SCamera m_Camera;
};

} // namespace South