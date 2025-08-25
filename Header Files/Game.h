#pragma once

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <fmt/core.h>

#include <glm/glm.hpp>

class Game {

public:

	Game(uint32_t width, uint32_t height);
	~Game();


	void run();

private:

	uint32_t m_width{};
	uint32_t m_height{};

	SDL_Window* m_window;
	SDL_GLContext m_glContext{};

	glm::vec4 m_clearColour{};
};
