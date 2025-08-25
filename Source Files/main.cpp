#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <fmt/core.h>

#include <glm/glm.hpp>

int main(int, char**) {
	// setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
		fmt::println("Error: SDL_Init(): {}", SDL_GetError());
		return -1;
	}

	const char* glslVersion{ "#version 460" };
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	uint32_t windowFlags{ SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN };
	SDL_Window* window{ SDL_CreateWindow("glTetris", 1920, 1080, windowFlags) };
	if (window == nullptr) {
		fmt::println("Error: SDL_CreateWindow(): {}", SDL_GetError());
		return -1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_GLContext glContext{ SDL_GL_CreateContext(window) };
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);	// enable vsync
	SDL_ShowWindow(window);

	// setup dear imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// enable gamepad controls

	// setup dear imgui style
	ImGui::StyleColorsDark();

	// setup platform/renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init(glslVersion);

	// imgui state
	bool showDemoWindow{ true };
	bool showAnotherWindow{ false };
	glm::vec4 clearColour{ 0.45f, 0.55f, 0.6f, 1.0f };

	// main loop
	bool done{ false };

	while (!done) {
		SDL_Event event{};
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// start the dear imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// 1. show the big demo window
		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);

		// 2. show a simple window othat we create ourselves
		// we use a begin/end pair to create a named window
		{
			static float f{ 0.0f };
			static int counter{ 0 };

			ImGui::Begin("Hello, world!");								// Create a window called "Hello, world!" and append to it

			ImGui::Text("This is some useful text.");					// Display some text (you can use a format string too)
			ImGui::Checkbox("Demo Window", &showDemoWindow);			// Edit bools storing our window open.close state
			ImGui::Checkbox("Another Window", &showAnotherWindow);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);				// Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear colour", (float*)&clearColour);	// Edit 3 floats representing a colour

			if (ImGui::Button("Button"))								// Buttons return true when clicked (most widgets return truen when edited/activated)
				++counter;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. show another simple window
		if (showAnotherWindow) {
			ImGui::Begin("Another Window", &showAnotherWindow);			// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				showAnotherWindow = false;
			ImGui::End();
		}

		// rendering
		ImGui::Render();
		glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
		glClearColor(clearColour.x* clearColour.w, clearColour.y* clearColour.w, clearColour.z* clearColour.w, clearColour.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}