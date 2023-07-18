#pragma once

//
#include "algorithm"
#include "iostream"
#include "memory"
#include "random"
#include "string"
#include "utility"

//
#include "array"
#include "map"
#include "set"
#include "unordered_map"
#include "vector"

//
#include "Core/Log/Logger.h"

// Math types
#include "ext/matrix_transform.hpp"
#include "glm.hpp"

#include "Math/Vector.h"

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