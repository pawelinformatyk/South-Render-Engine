#pragma once

#include <filesystem>

namespace South
{
    class AssetManager
    {
    public:
        static const char* GetFontsDirectory() { return "misc\\fonts\\"; };
        // static const char* GetWorkingDirectory() { return std::filesystem::current_path(); };
    };
} // namespace South