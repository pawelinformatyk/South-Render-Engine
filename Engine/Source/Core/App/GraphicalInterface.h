#pragma once
#include "imgui.h"

namespace South
{

    class GraphicalInterface
    {
    public:
        void Init();
        void DeInit();

        void BeginFrame();

        void Show(float FrameTimeSeconds);

        void EndFrame();

    private:
        // TitleBar : Various options, control buttons etc.
        void ShowTitlebar() const;

        // void ShowStyleEditor() const;

        static void FindAndAddFonts();

        // Sets gui style to application default.
        static void StyleColorsSouth();
    };

} // namespace South
