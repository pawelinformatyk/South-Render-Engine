#pragma once

#include "SceneObject.h"

namespace South
{

/*
 * Collection of objects placed on a level (scene). Shared by several viewports/cameras etc.
 */
class SScene
{
public:
    void LoadExample();

    std::vector<std::unique_ptr<SSceneObject>> SceneObjects;
};

} // namespace South