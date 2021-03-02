#pragma once
#include <GUI/Widget/Button.h>

class PressedButton : public Button
{
    public:
        PressedButton(string text, Action* action,
                      int x, int y, int w, int h,
                      int borderWidth, vector<int> bgColor,
                      vector<int> selectedBgColor,
                      vector<int> borderColor);
        void update(float ticks) override;
};

