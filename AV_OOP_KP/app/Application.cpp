#include "Application.hpp"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <GL\gl3w.h>

namespace av
{
    void Application::Cleanup()
    {
        if (Status(AppComponent::ImGui_GL3)) ImGui_ImplOpenGL3_Shutdown();
        if (Status(AppComponent::ImGui_SDL)) ImGui_ImplSDL2_Shutdown();
        if (Status(AppComponent::ImGui_Context)) ImGui::DestroyContext();
        if (Status(AppComponent::GL_Context)) SDL_GL_DeleteContext(glctx);
        if (Status(AppComponent::Window)) SDL_DestroyWindow(window);
        if (Status(AppComponent::SDL_Subsystems)) SDL_Quit();
    }

    Application::Application(int argc, char* argv[])
    {
        try
        {
            SetTargetFPS(60);

            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
                throw std::logic_error("Failed to initialize SDL subsystems");
            else Status(AppComponent::SDL_Subsystems) = true;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL 3.3
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

            window = SDL_CreateWindow("SDL Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

            if (window == nullptr) throw std::logic_error("Failed to create SDL window");
            else Status(AppComponent::Window) = true;

            glctx = SDL_GL_CreateContext(window);
            if (glctx == nullptr) throw std::logic_error("Failed to create OpenGL context");
            else Status(AppComponent::GL_Context) = true;
            SDL_GL_MakeCurrent(window, glctx);

            GLenum gl_status = gl3wInit();
            if (gl_status) throw std::logic_error("Failed to bind OpenGL procedures");
            else Status(AppComponent::GL_Procedures) = true;

            // »нициализаци€ контекста Dear ImGui + OpenGL3 + SDL2
            guictx = ImGui::CreateContext();
            if (guictx == nullptr) throw std::logic_error("Failed to create ImGui context");
            else Status(AppComponent::ImGui_Context) = true;

            if (!ImGui_ImplSDL2_InitForOpenGL(window, glctx)) throw std::logic_error("Failed to initialize Dear ImGui OpenGL3 backend");
            else Status(AppComponent::ImGui_SDL) = true;

            if (!ImGui_ImplOpenGL3_Init()) throw std::logic_error("Failed to initialize Dear ImGui OpenGL3 backend");
            else Status(AppComponent::ImGui_GL3) = true;

            ImGui::StyleColorsLight();

            app_status = 0;
        }
        catch (std::exception& e)
        {
            Cleanup();
            throw e;
        }
    }

    int Application::Run()
    {
        AppInit();

        unsigned time_next_frame = 0;
        unsigned sleep_duration = 0;

        while (running)
        {
            // «асекаем врем€ начала кадра
            time_frame_start = SDL_GetTicks();
            // ѕодсчитываем, в какое врем€ по плану должна начинатьс€ отрисовка следующего кадра
            time_next_frame = time_frame_start + frame_delay;
            // ќбработка событий
            HandleEvents();
            // ¬нутренние вычислени€
            Process();
            // ѕредставление
            Draw();

            SDL_GL_SwapWindow(window);

            // «асекаем врем€ конца кадра
            time_frame_end = SDL_GetTicks();
            // ≈сли врем€ конца кадра меньше планового дл€ начала следующего (успели обработать кадр быстрее,
            // чем за целевое дл€ данного FPS врем€), тогда ищему разницу между ними, и ждЄм найденное
            // количество миллисекунд. ¬ ином случае(если не успеваем) - "ждЄм" 0 миллисекунд.
            sleep_duration = (time_frame_end < time_next_frame) ? (time_next_frame - time_frame_end) : 0;

            SDL_Delay(sleep_duration);
        }

        return app_status;
    }

    void Application::SetTargetFPS(unsigned fps)
    {
        if (fps == 0) fps = 1000;
        target_fps = fps;

        frame_delay = (unsigned)round(1000.0f / fps);
    }

    void Application::HandleEvents()
    {
        while (SDL_PollEvent(&evt))
        {
            ImGui_ImplSDL2_ProcessEvent(&evt);

            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
    }

    void Application::Draw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::Begin("Test Window",nullptr);
        ImGui::Text("Dear ImGui works!");
        ImGui::End();

        glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}