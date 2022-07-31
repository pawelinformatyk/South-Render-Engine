#pragma once

namespace South
{

    class GraphicalInterface
    {
    public:
        void Init();
        void DeInit();

        void BeginFrame();

        void Draw();

        void EndFrame();

    private:
        void SetupStyleAndFonts();
    };

} // namespace South
