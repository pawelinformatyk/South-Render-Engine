#pragma once

#include "vulkan/vk_platform.h"

struct GLFWwindow;

namespace South
{
	// Target of graphics rendering and inputs. 
	// Events need to be picked up by this window 
	// 
	class AppWindow
	{
	public:
		AppWindow();
		~AppWindow();

		void tick();

		static AppWindow* Create();

	private:
		GLFWwindow* pWindow = nullptr;

		uint32_t Width = 1600;
		uint32_t Height = 1600;
	};
}

