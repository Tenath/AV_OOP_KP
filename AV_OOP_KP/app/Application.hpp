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
        // Размер массива флагов инициализации, берётся по последнему значению enum
        static const size_t STATUS_SIZE = (size_t)AppComponent::LAST;

        /* Внешние ресурсы */
        ImGuiContext* guictx = nullptr; // Контекст Dear ImGUI
        SDL_Window* window = nullptr; // Окно SDL
        SDL_GLContext glctx = nullptr; // Контекст OpenGL в рамках окна SDL
        SDL_Event evt; // Структура - событие SDL

        bool running = true; // флаг продолжения работы

        bool status[STATUS_SIZE] = {}; // массив флагов инициализации
        int app_status = -1; // return code для main

        unsigned target_fps = 60; // целевой FPS
        unsigned frame_delay = 16; // вычисленная задержка
        unsigned time_frame_start = 0; // фактическое время начала кадра
        unsigned time_frame_end = 0; // фактическое время конца кадра

        unsigned WinWidth = 800; // ширина окна
        unsigned WinHeight = 600; // высота окна
        float aspect_ratio = 0.75; // вычисленное отношение ширина:высота

    protected:
        // Враппер для получения статуса указанного компонента
        constexpr inline bool& Status(AppComponent cmp) { return status[(size_t)cmp]; }

        void Cleanup();
    public:
        // Конструктор
        Application(int argc, char* argv[]);
        // Деструктор, отказываюсь переносить в .cpp и убивать inline
        virtual ~Application() { Cleanup(); }

        // Основной цикл приложения
        int Run();

        // Выставление целевого FPS
        void SetTargetFPS(unsigned fps);
    protected:
        // Инициализация приложения (отсроченные процессы, не выполняемые в конструкторе)
        virtual void AppInit() {}

        // Обработка событий
        virtual void HandleEvents();

        // Расчёты и т.д.
        virtual void Process() {}

        // Отрисовка
        virtual void Draw();

    public:
        SDL_Window* GetWindow() { return window; }
        void Exit() { running = false; }

        unsigned GetWinWidth() { return WinWidth; }
        unsigned GetWinHeight() { return WinHeight; }
    };
}