#ifndef PUYOPUYO_WINDOWRENDERER_HPP
#define PUYOPUYO_WINDOWRENDERER_HPP

#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
 * Responsible to deal with the game graphics.
 * Singleton.
 */
class WindowRenderer {
private:
    inline static SDL_Window *window;
    inline static SDL_Renderer *renderer;
    std::pair<int, int> screenSize;

    WindowRenderer();
    static WindowRenderer &getWindowRenderer();
    friend class GameManager;

public:
    WindowRenderer(const WindowRenderer &) = delete;
    WindowRenderer &operator=(const WindowRenderer &) = delete;
    WindowRenderer(WindowRenderer &&) = delete;
    WindowRenderer &operator=(WindowRenderer &&) = delete;

    static SDL_Texture *loadTexture(const char *filePath);
    static SDL_Rect buildSDL_Rect(int x, int y, SDL_Texture *texture);
    static void render(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst);
    static void render(SDL_Texture *texture, const SDL_Rect &dst);
    static void
    render(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst, double angle, const SDL_Point *rotationPoint,
           const SDL_RendererFlip &flags = SDL_FLIP_NONE);
    void clear();
    void display();
    void shutdown();

    [[nodiscard]] std::pair<int, int> getScreenSize() const;
    [[nodiscard]] std::pair<int, int> getScreenCenter() const;
};


#endif//PUYOPUYO_WINDOWRENDERER_HPP
