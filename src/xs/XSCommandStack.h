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
    virtual int Do(Command* command);
    virtual int ReDo();
    virtual int Undo();

    bool CanReDo() const;
    bool CanUndo() const;

    Command const* GetUndoCommand() const;
    Command const* GetReDoCommand() const;

    void Saved();
    bool IsModified() const;

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

    virtual int Do(XSCommand* command);
    virtual int Undo();
    virtual int ReDo();

protected:
    XSModel* m_model;
};

#endif
