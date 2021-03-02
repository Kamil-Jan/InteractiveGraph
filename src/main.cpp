#define SDL_MAIN_HANDLED
#include <Game.h>
#include <State/MainState.h>
#include <Logger.h>

int main()
{
    Logger::setLevel("CRITICAL");

    Graph graph = Graph();
    Game game = Game("Interactive graph");
    State* state = new MainState();
    game.push(state);
    game.run();
    return 0;
}
