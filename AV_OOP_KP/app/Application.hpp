#pragma once
#include <stdexcept>
#include <SDL.h>

struct ImGuiContext;

namespace av
{
    enum class AppComponent : unsigned char
    {
        SDL_Subsystems = 0,
        Window = 1,
        GL_Context = 2,
        GL_Procedures = 3,
        ImGui_Context = 4,
        ImGui_GL3 = 5,
        ImGui_SDL = 6,
        LAST
    };

    class Application
    {
    protected:
        static const size_t STATUS_SIZE = (size_t)AppComponent::LAST;

        ImGuiContext* guictx = nullptr;
        SDL_Window* window = nullptr;
        SDL_GLContext glctx = nullptr;
        SDL_Event evt;

        bool running = true;

        bool status[STATUS_SIZE] = {}; // ������ ������ �������������
        int app_status = -1; // return code ��� main

        unsigned target_fps = 60;
        unsigned frame_delay = 16;
        unsigned time_frame_start = 0;
        unsigned time_frame_end = 0;

        unsigned WinWidth = 800;
        unsigned WinHeight = 600;

    protected:
        constexpr inline bool& Status(AppComponent cmp) { return status[(size_t)cmp]; }

        void Cleanup();
    public:
        Application(int argc, char* argv[]);

        virtual ~Application() { Cleanup(); }

        // �������� ���� ����������
        int Run();

        // ����������� �������� FPS
        void SetTargetFPS(unsigned fps);
    protected:
        // ������������� ���������� (����������� ��������, �� ����������� � ������������)
        virtual void AppInit() {}

        // ��������� �������
        virtual void HandleEvents();

        // ������� � �.�.
        virtual void Process() {}

        // ���������
        virtual void Draw();
    };
}