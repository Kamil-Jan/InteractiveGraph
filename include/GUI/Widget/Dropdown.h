#include <SDL2/SDL.h>
#include <GUI/Widget/Widget.h>
#include <Action/Action.h>
#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;

class Dropdown : public Widget
{
    private:
        unordered_map<string, Action*> items;
        vector<int> bgColor;
        vector<int> textBgColor;
        vector<int> selectedTextBgColor;
        vector<int> borderColor;
        unordered_map<string, SDL_Texture*> textureMap;
        unordered_map<string, SDL_Rect> textRectMap;
        unordered_map<string, SDL_Rect> itemsRectMap;

        SDL_Texture* itemsListTexture;
        SDL_Rect itemsListRect;
        SDL_Rect itemsListSrcRect;
        SDL_Rect scrollBarRect;
        SDL_Rect sliderRect;

        SDL_Rect defaultTextRect;
        SDL_Texture* defaultTextTexture;
        string defaultText;
        string selected = "";

        const int padding = 4;
        const int shownItemNum = 3;
        int borderWidth;
        int itemsSize;
        bool expanded = false;
        bool sliderDragged = false;

    public:
        Dropdown(unordered_map<string, Action*> items,
                 string defaultText,
                 int x, int y,
                 int w, int h, int borderWidth,
                 vector<int> bgColor,
                 vector<int> textBgColor,
                 vector<int> selectedTextBgColor,
                 vector<int> borderColor);
        ~Dropdown();
        string getSelectedItem();
        void resetSelectedItem();
        void reset() override;

        void processItems(unordered_map<string, Action*> items);

        bool isInside(int x, int y) override;
        bool isInsideExpanded(int x, int y);
        bool isExpanded();

        void update(float ticks) override;
        void drawArrowUp(SDL_Renderer* renderer);
        void drawArrowDown(SDL_Renderer* renderer);
        void renderItems(SDL_Renderer* renderer);
        void render() override;
};

