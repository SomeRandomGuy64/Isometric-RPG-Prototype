#include "Game.h"

Game::Game(uint32_t width, uint32_t height)
	: m_width{ width }
	, m_height{ height }
{

	// setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
		fmt::println("Error: SDL_Init(): {}", SDL_GetError());
		// error
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
	m_window = SDL_CreateWindow("RPG PROTOTYPE", m_width, m_height, windowFlags);
	if (m_window == nullptr) {
		fmt::println("Error: SDL_CreateWindow(): {}", SDL_GetError());
		// error
	}
	SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	m_glContext = { SDL_GL_CreateContext(m_window) };
	SDL_GL_MakeCurrent(m_window, m_glContext);
	SDL_GL_SetSwapInterval(1);	// enable vsync
	SDL_ShowWindow(m_window);

	// setup dear imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// enable gamepad controls

	// setup dear imgui style
	ImGui::StyleColorsDark();

	// setup platform/renderer backends
	ImGui_ImplSDL3_InitForOpenGL(m_window, m_glContext);
	ImGui_ImplOpenGL3_Init(glslVersion);

	m_clearColour = { 0.45f, 0.55f, 0.6f, 1.0f };

}

Game::~Game() {

	// cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

}

void Game::run() {

	// main loop
	bool done{ false };

	while (!done) {
		SDL_Event event{};
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(m_window))
				done = true;
		}

		// start the dear imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// 2. show a simple window that we create ourselves
		// we use a begin/end pair to create a named window
		{
			static float f{ 0.0f };
			static int counter{ 0 };

			ImGui::Begin("Hello, world!");								// Create a window called "Hello, world!" and append to it

			ImGui::Text("This is some useful text.");					// Display some text (you can use a format string too)

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);				// Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear colour", (float*)&m_clearColour);	// Edit 3 floats representing a colour

			if (ImGui::Button("Button"))								// Buttons return true when clicked (most widgets return truen when edited/activated)
				++counter;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::End();
		}

		// rendering
		ImGui::Render();
		glViewport(0, 0, static_cast<int>(m_width), static_cast<int>(m_height));
		glClearColor(m_clearColour.x * m_clearColour.w, 
			m_clearColour.y * m_clearColour.w, 
			m_clearColour.z * m_clearColour.w, 
			m_clearColour.w
		);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_window);
	}

}
