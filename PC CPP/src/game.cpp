#include <headers/game.h>


Sprite::Sprite(SDL_Texture *tex, int s) {
    texture = tex;
    size = s;
}

Sprite::Sprite() {
}

void Sprite::reconstructor(SDL_Texture *tex, int s){
    texture = tex;
    size = s;
}


Input::Input(){
    clear();
}

void Input::clear(){
    LeftMBPressed = false;
    mouseX = -1;
    mouseY = -1;
    KeypPressed = false;
    KeyrPressed = false;
    KeynPressed = false;
    KeybPressed = false;
    KeykPressed = false;
    KeyqPressed = false;
    Key0Pressed = false;
    KeybsPressed = false;
}

void Input::saveMousePos() {
    SDL_GetMouseState(&mouseX, &mouseY);
}

// PUBLIC
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1000;
    screenHeight = 1000;
    doScreenClearing = true;
    gameState = GameState::PLAY;
    draw = nullptr;
};

Game::~Game() {

};

void Game::run(void (*startF)(Game *game),void (*drawF)(Game *game)) {
    draw = drawF;
    init("SDL SCREEN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    startF(this);
    gameLoop();
}

int Game::getScreenWidth(){
    return screenWidth;
}

int Game::getScreenHeight(){
    return screenHeight;
}

void Game::setWindowTitle(std::string title) {
    SDL_SetWindowTitle(window, title.c_str());
}

float Game::deltaTime(){
    return deltatime;
}


// DRAWING
int Game::renderFill(int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer,r,g,b,255);
    SDL_RenderClear(renderer);
    return 0;
}

int Game::renderCircle(int x, int y, int radius, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
    
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int Game::renderRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,a);

    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}

int Game::renderSprite(int x, int y, Sprite sprite)
{
    SDL_SetTextureBlendMode(sprite.texture,SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {x, y, sprite.size, sprite.size};
    SDL_RenderCopy(renderer,sprite.texture,NULL,&rect);
    return 0;
}

SDL_Texture* Game::textureFromPath(const char *path)
{   
    SDL_Surface *tmpSurface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return texture;
}

// PRIVATE
void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Init(SDL_INIT_EVERYTHING);
 
    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void Game::gameLoop() {
    Uint64 oldTime = 0;
    while(gameState != GameState::EXIT){
        deltatime = (SDL_GetTicks64() - oldTime) / 1000.0;
        oldTime = SDL_GetTicks64();
        
        handleEvents();
        render();
        input.clear();
        SDL_Delay(1);
    }
}

void Game::handleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch (evnt.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(evnt.button.button == SDL_BUTTON_LEFT){
                input.LeftMBPressed = true;
                input.saveMousePos();
            } 
            break;
        case SDL_KEYDOWN:
            if(evnt.key.keysym.sym == SDLK_p){
                input.KeypPressed = true;
                input.saveMousePos();
                break;
            } 
            if(evnt.key.keysym.sym == SDLK_r){
                input.KeyrPressed = true;
                input.saveMousePos();
                break;
            }
            if(evnt.key.keysym.sym == SDLK_n){
                input.KeynPressed = true;
                input.saveMousePos();
                break;
            }
            if(evnt.key.keysym.sym == SDLK_b){
                input.KeybPressed = true;
                input.saveMousePos();
                break;
            }
            if(evnt.key.keysym.sym == SDLK_k){
                input.KeykPressed = true;
                input.saveMousePos();
                break;
            }
            if(evnt.key.keysym.sym == SDLK_q){
                input.KeyqPressed = true;
                input.saveMousePos();
                break;
            } 
            if(evnt.key.keysym.sym == SDLK_0){
                input.Key0Pressed = true;
                input.saveMousePos();
                break;
            } 
            if(evnt.key.keysym.sym == SDLK_BACKSPACE){
                input.KeybsPressed = true;
                input.saveMousePos();
                break;
            } 
            if(evnt.key.keysym.sym == SDLK_LSHIFT){
                input.shiftDown = true;
                break;
            } 
            
            break;

        case SDL_KEYUP:
            if(evnt.key.keysym.sym == SDLK_LSHIFT){
                input.shiftDown = false;
                break;
            }
            break;
        
        default:
            break;
    }
}

void Game::render(){
    // if(doScreenClearing){
    //     SDL_SetRenderDrawColor(renderer,240,230,230,255);
    //     SDL_RenderClear(renderer);
    // }
    (*draw)(this);
    SDL_RenderPresent(renderer);
}
