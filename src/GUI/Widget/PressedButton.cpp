#include <GUI/Widget/PressedButton.h>
#include <InputManager.h>
#include <Logger.h>

PressedButton::PressedButton(string text, Action* action,
                             int x, int y, int w, int h,
                             int borderWidth, vector<int> bgColor,
                             vector<int> selectedBgColor,
                             vector<int> borderColor) :
    Button(text, action, x, y, w, h, borderWidth,
           bgColor, selectedBgColor, borderColor)
{
    Logger::debug("PressedButton::PressedButton");
}

void PressedButton::update(float ticks)
{
    Logger::debug("PressedButton::update");
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    curColor = (selected)? selectedBgColor : bgColor;

    if (!isInside(mouseX, mouseY)) {
        return;
    }

    if (InputManager::getInstance()->mousePressed(1) &&
            action != NULL) {
        action->execute();
        selected = true;
    }
}

