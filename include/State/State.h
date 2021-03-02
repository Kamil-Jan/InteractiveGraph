#pragma once

class State
{
    protected:
        bool mQuitRequested;

    public:
        State();

        virtual void update() = 0;
        virtual void render() = 0;

        void quit();
        void resetQuitRequested();
        bool quitRequested();
};

