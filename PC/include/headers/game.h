#pragma once
#include <iostream>
#include <map>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

enum class GameState {PLAY, EXIT};

class Sprite {
    public:
    Sprite(SDL_Texture *tex, int s);
    Sprite();
    void reconstructor(SDL_Texture *tex, int s);
    SDL_Texture* texture;
    int size;
};

class Button {
    public:
    Button(SDL_KeyCode key);
    Button();
    bool value;
    SDL_KeyCode SDLKey;
};

class Input {
    public:
    Input();
    void clear();
    void saveMousePos();


    // fields
    std::map<std::string, Button> keyPressed = {
        // Letters
        { "q", Button(SDLK_q) },
        { "a", Button(SDLK_a) },
        { "w", Button(SDLK_w) },
        { "s", Button(SDLK_s) },
        { "e", Button(SDLK_e) },
        { "d", Button(SDLK_d) },
        // Numbers
        { "1", Button(SDLK_1) },
        { "2", Button(SDLK_2) },
        { "3", Button(SDLK_3) },
        { "4", Button(SDLK_4) },
        { "5", Button(SDLK_5) },
        { "6", Button(SDLK_6) },
        { "0", Button(SDLK_0) },
        // Arrows
        { "up", Button(SDLK_UP) },
        { "down", Button(SDLK_DOWN) },
        { "left", Button(SDLK_LEFT) },
        { "right", Button(SDLK_RIGHT) },
        // Else
        { "backspace", Button(SDLK_BACKSPACE) },
        { "shift", Button(SDLK_LSHIFT) },
    };
    std::map<std::string, Button> modKeyPressed = {
        { "shift", Button(SDLK_LSHIFT) },
    };

    bool LeftMouseDown;

    // bool RightMB;
    int mouseX;
    int mouseY;

};


class Game {
    public:
        Game();
        ~Game();

        void run(void (*startF)(Game *game), void (*drawF)(Game *game));

        int getScreenWidth();
        int getScreenHeight();
        void setWindowTitle(std::string title);
        bool doScreenClearing;

        float deltaTime(); 

        int renderFill(int r, int g, int b);
        int renderCircle(int x, int y, int radius, int r, int g, int b, int a=255);
        int renderRect(int x, int y, int w, int h, int r, int g, int b, int a=255);
        int renderSprite(int x, int y, Sprite sprite);
        
        SDL_Texture *textureFromPath(const char *path);

        Input input;

    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void render();


        SDL_Window *window;
        SDL_Renderer *renderer;

        int screenWidth;
        int screenHeight;

        float deltatime;

        GameState gameState;

        void (*draw)(Game *game);
};