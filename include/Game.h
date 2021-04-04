#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <stack>

#include <State/State.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 750
#define FPS 60
#define TICKS_PER_FRAME 1000 / FPS

using std::vector;
using std::string;
using std::stack;

class Game
{
    private:
        static Game* instance;
        SDL_Window* window;
        SDL_Surface* screen;
        SDL_Renderer* renderer;
        TTF_Font* font;

        string title;

        State* storedState;
        stack<State*> stateStack;

        SDL_Texture* vertexTexture;
        SDL_Texture* selectedVertexTexture;
        SDL_Texture* edgeTexture;
        SDL_Texture* pathEdgeTexture;

    public:
        static const vector<int> BG_COLOR;
        static const int MENU_BORDER_WIDTH;
        static const vector<int> MENU_BG_COLOR;
        static const vector<int> MENU_BORDER_COLOR;
        static const int BUTTON_BORDER_WIDTH;
        static const vector<int> BUTTON_BG_COLOR;
        static const vector<int> BUTTON_SELECTED_BG_COLOR;
        static const vector<int> BUTTON_BORDER_COLOR;

        static const int VERTEX_RADIUS;
        static const int VERTEX_BORDER;
        static const vector<int> VERTEX_COLOR;
        static const vector<int> VERTEX_BORDER_COLOR;
        static const vector<int> SELECTED_VERTEX_COLOR;
        static const vector<int> PATH_VERTEX_COLOR;

        static const int EDGE_WIDTH;
        static const int ARROW_LINE_LENGTH;
        static const float ARROW_LINE_ANGLE;
        static const vector<int> EDGE_COLOR;
        static const vector<int> PATH_EDGE_COLOR;

        Game(string title);
        ~Game();

        static Game* getInstance();

        State* getCurrentState();
        SDL_Renderer* getRenderer();
        SDL_PixelFormat* getScreenFormat();
        TTF_Font* getFont();
        SDL_Texture* getVertexTexture();
        SDL_Texture* getSelectedVertexTexture();
        SDL_Texture* getEdgeTexture();
        SDL_Texture* getPathEdgeTexture();

        void push(State* state);
        void manageStack();

        void capFrameRate(float delta);
        void run();
};

