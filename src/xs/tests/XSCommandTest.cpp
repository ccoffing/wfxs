#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "XSCommand.h"
#include "XSCommandStack.h"

class TestXSCommand : public XSCommand {
public:
    int doCommand()
    {
        return 0;
    }

    int undoCommand()
    {
        return 0;
    }

    char const* GetDescription() const
    {
        return "test command that always succeeds.";
    }
};

TEST_CASE("CommandStack")
{
    CommandStack cs;

    SECTION("init")
    {
        CHECK_FALSE(cs.canRedo());
        CHECK_FALSE(cs.canUndo());
        CHECK_FALSE(cs.isModified());
    }

    SECTION("undo")
    {
        Command* c = new TestXSCommand;

        cs.doCommand(c);
        CHECK_FALSE(cs.canRedo());
        CHECK(cs.getRedoCommand() == 0);
        CHECK(cs.canUndo());
        CHECK(cs.getUndoCommand() == c);
        CHECK(cs.isModified());

        cs.undoCommand();
        CHECK(cs.canRedo());
        CHECK(cs.getRedoCommand() == c);
        CHECK_FALSE(cs.canUndo());
        CHECK(cs.getUndoCommand() == 0);
        CHECK_FALSE(cs.isModified());
    }
}
