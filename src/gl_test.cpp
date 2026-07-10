#include <SDL3/SDL.h>
#include <glad/gl.h>

#include "RendererGL.hpp"

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return -1;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
        SDL_CreateWindow(
            "OpenGL Test",
            1280,
            720,
            SDL_WINDOW_OPENGL);

    SDL_GLContext context =
        SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, context);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
        return -1;

    RendererGL renderer;

    if (!renderer.initialize())
        return -1;

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        renderer.beginFrame();
        renderer.drawTriangle();
        SDL_GL_SwapWindow(window);
    }

    renderer.destroy();

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}