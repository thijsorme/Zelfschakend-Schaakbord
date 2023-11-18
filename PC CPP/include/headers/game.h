#pragma once
#include <iostream>
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


class Input {
    public:
    Input();
    void clear();
    void saveMousePos();


    // fields
    bool LeftMBPressed;

    bool KeypPressed;
    bool KeyrPressed;
    bool KeynPressed;
    bool KeybPressed;
    bool KeykPressed;
    bool KeyqPressed;
    bool Key0Pressed;
    bool KeybsPressed;


    bool shiftDown = false;

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