#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "XSCommand.h"
#include "XSCommandStack.h"

class TestXSCommand : public XSCommand {
public:
    int Do()
    {
        return 0;
    }

    int Undo()
    {
        return 0;
    }

    char const *GetDescription() const
    {
        return "test command that always succeeds.";
    }
};

TEST_CASE("CommandStack") {
    CommandStack cs;

    SECTION("init") {
        CHECK_FALSE(cs.CanReDo());
        CHECK_FALSE(cs.CanUndo());
        CHECK_FALSE(cs.IsModified());
    }

    SECTION("undo") {
        Command *c = new TestXSCommand;

        cs.Do(c);
        CHECK_FALSE(cs.CanReDo());
        CHECK(cs.GetReDoCommand() == 0);
        CHECK(cs.CanUndo());
        CHECK(cs.GetUndoCommand() == c);
        CHECK(cs.IsModified());

        cs.Undo();
        CHECK(cs.CanReDo());
        CHECK(cs.GetReDoCommand() == c);
        CHECK_FALSE(cs.CanUndo());
        CHECK(cs.GetUndoCommand() == 0);
        CHECK_FALSE(cs.IsModified());
    }
}
