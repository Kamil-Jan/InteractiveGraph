#include <SDL2/SDL.h>

#include <Game.h>
#include <Logger.h>
#include <InputManager.h>

InputManager* InputManager::instance = NULL;

InputManager::InputManager()
{
    Logger::debug("InputManager::InputManager");
    updateCounter = 0;
    mQuitRequested = false;
}

InputManager* InputManager::getInstance()
{
    Logger::debug("InputManager::getInstance started");

    if (instance == NULL) {
        Logger::debug("InputManager::getInstance - instance=NULL. Creating an instance");
        instance = new InputManager();
    }

    return instance;
}

bool InputManager::quitRequested()
{
    return mQuitRequested;
}

void InputManager::update()
{
    Logger::debug("InputManager::update started");
    SDL_Event event;
    updateCounter++;
    mQuitRequested = false;

    int keyId, buttonId;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Logger::debug("InputManager::update - SDL_QUIT event");
                mQuitRequested = true;
                break;

            case SDL_KEYDOWN:
                if (event.key.repeat)
                    break;

                Logger::debug("InputManager::update - SDL_KEYDOWN event");
                keyId = event.key.keysym.sym;
                keyState[keyId] = true;
                keyUpdate[keyId] = updateCounter;
                break;

            case SDL_KEYUP:
                Logger::debug("InputManager::update - SDL_KEYUP event");
                keyId = event.key.keysym.sym;
                keyState[keyId] = false;
                keyUpdate[keyId] = updateCounter;
                break;

			case SDL_MOUSEBUTTONDOWN:
                Logger::debug("InputManager::update - SDL_MOUSEBUTTONDOWN event");
				buttonId = event.button.button;
				mouseState[buttonId] = true;
				mouseUpdate[buttonId] = updateCounter;
				break;

			case SDL_MOUSEBUTTONUP:
                Logger::debug("InputManager::update - SDL_MOUSEBUTTONUP event");
				buttonId = event.button.button;
				mouseState[buttonId] = false;
				mouseUpdate[buttonId] = updateCounter;
				break;
        }
    }

    Logger::debug("InputManager::update ended");
}

bool InputManager::keyPressed(int key)
{
    return keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::keyReleased(int key)
{
    return !keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::isKeyDown(int key)
{
    return keyState[key];
}

bool InputManager::mousePressed(int button)
{
    return mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::mouseReleased(int button)
{
    return !mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::isMouseDown(int button)
{
    return mouseState[button];
}

