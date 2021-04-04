#pragma once

class Mode
{
    public:
        Mode();
        virtual void quit() = 0;
        virtual void update(float ticks) = 0;
        virtual void render() = 0;
};

