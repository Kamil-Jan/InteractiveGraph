#include <Mode/Mode.h>
#include <Action/Action.h>

class SetModeAction : public Action
{
    private:
        Mode* mode;

    public:
        SetModeAction(Mode* mode);
        void execute() override;
};

