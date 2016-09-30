#ifndef XS_COMMAND_H
#define XS_COMMAND_H

#define _(x) x

class XSModel;

/**
 *  Abstract base class for undo-able commands.
 */
class Command {
public:
    virtual ~Command(){};

    /**
     *  Attempts to perform the command.  If the command failed to be performed (e.g., trying to
     *  modify a read-only document, or out of memory), return non-zero, in which case the
     *  command will not be saved to the undo/redo stack.
     *  @return 0 iff success.
     */
    virtual int doCommand() = 0;

    /**
     *  Attempts to undo the command.  (Will only be called if the command was previously
     *  sucessfuly performed, so that may be assumed.)
     *  @return 0 iff success.
     */
    virtual int undoCommand() = 0;

    /**
     *  Attempts to redo the command.  (Will only be called if the command was previously
     *  sucessfuly done and later undone, so that may be assumed.) Default implementation is to
     *  call doCommand, but subclasses may override for a more efficient implementation (for example,
     *  doCommand may save away state that makes undoCommand and redoCommand more efficient.)
     *  @return 0 iff success.
     */
    virtual int redoCommand()
    {
        return doCommand();
    }

    /**
     *  Returns an internationalized description in UTF-8.  The description should describe the
     *  action in general terms.  Do not include "undo" or "redo" in the description.
     */
    virtual char const* getDescription() const = 0;
};

/** Base class for cross-stitch commands.
 *
 *  In general, it's a good idea to pass as little as possible to the constructor of the derived
 *  class.  The remaining information can be obtained in the doCommand() function.  This way, the command
 *  is more like a macro -- can (in theory) be replayed elsewhere.
 */
class XSCommand : public Command {
public:
    XSCommand()
        : m_model(0)
    {
    }

    void init(XSModel* model);

    char const* getDescription() const;

protected:
    XSModel* m_model;
};

#endif
