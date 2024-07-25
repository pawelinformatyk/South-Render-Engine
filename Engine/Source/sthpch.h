#pragma once

//
#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>

//
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

#include "Core/Logger/Logger.h"

// Math types
#include "Core/Math/Vector.h"
#include "ext/matrix_transform.hpp"
#include "glm.hpp"
#include "mat4x4.hpp"
#include <cmath>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>

// #TODO: Move somewhere
template<typename T>
glm::vec3 Convert(const South::TVector<T>& Vector)
{
    return glm::vec3(Vector.X, Vector.Y, Vector.Z);
}

template<typename T>
South::TVector<T> Convert(const glm::vec3& Vector)
{
    return glm::vec3(Vector.x, Vector.y, Vector.z);
}