#ifndef XS_COMMAND_STACK_H
#define XS_COMMAND_STACK_H

class Command;
class XSCommand;
class XSModel;

/**
 *  Stack that tracks undoing and redoing Commmands.
 */
class CommandStack {
public:
    CommandStack();
    virtual ~CommandStack();

    /**
     * Performs the command for the first time (giving the command a chance to save the old state)
     * and then saves it on the command stack.  Ownership of the command is transferred.  The
     * CommandStack will free the Command when it is no longer relevant.
     */
    virtual int doCommand(Command* command);
    virtual int redoCommand();
    virtual int undoCommand();

    bool canRedo() const;
    bool canUndo() const;

    Command const* getUndoCommand() const;
    Command const* getRedoCommand() const;

    void saved();
    bool isModified() const;

private:
    int m_allocated;
    int m_used;
    int m_current;
    int m_lastSave;
    Command** m_commands;

    // disallow copying
    CommandStack(CommandStack const&);
    CommandStack& operator=(CommandStack const&);
};

class XSCommandStack : public CommandStack {
public:
    XSCommandStack(XSModel* model);

    virtual int doCommand(XSCommand* command);
    virtual int undoCommand();
    virtual int redoCommand();

protected:
    XSModel* m_model;
};

#endif
