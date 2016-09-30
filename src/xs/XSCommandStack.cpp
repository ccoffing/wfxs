#include "XSCommand.h"
#include "XSCommandStack.h"
#include "XSDocument.h"

#include <cassert>


static const int NONE = -2;


CommandStack::CommandStack()
    : m_allocated(100)
    , m_used(0)
    , m_current(0)
    ,
    // m_lastSave(NONE),
    m_lastSave(m_current - 1)
    , m_commands(new Command*[m_allocated])
{
}

CommandStack::~CommandStack()
{
    for (int i = 0; i < m_used; ++i) {
        delete m_commands[i];
    }
    delete[] m_commands;
}

Command const* CommandStack::getUndoCommand() const
{
    if (!canUndo())
        return 0;
    return m_commands[m_current - 1];
}

Command const* CommandStack::getRedoCommand() const
{
    if (!canRedo())
        return 0;
    return m_commands[m_current];
}

int CommandStack::doCommand(Command* command)
{
    if (m_current < m_used) {
        for (int i = m_current; i < m_used; ++i) {
            delete m_commands[i];
        }
        if (m_current <= m_lastSave)
            m_lastSave = NONE;
    } else if (m_current == m_allocated) {
        delete m_commands[0];
        for (int i = 1; i < m_allocated; ++i) {
            m_commands[i - 1] = m_commands[i];
        }
        if (m_lastSave != NONE)
            --m_lastSave;
        --m_current;
    }

    m_commands[m_current] = command;
    ++m_current;
    m_used = m_current;

    // printf("doing %d (%s)\n", m_current - 1, m_commands[m_current - 1]->getDescription());
    int error = m_commands[m_current - 1]->doCommand();
    if (error)
        --m_current;
    return error;
}

int CommandStack::redoCommand()
{
    assert(canRedo());
    // printf("redoing %d of %d (%s)\n", m_current, m_used,
    // m_commands[m_current]->getDescription());
    int error = m_commands[m_current]->redoCommand();
    if (!error)
        ++m_current;
    return error;
}

int CommandStack::undoCommand()
{
    assert(canUndo());
    // printf("undoing %d of %d (%s)\n", m_current-1, m_used,
    // m_commands[m_current-1]->getDescription());
    int error = m_commands[m_current - 1]->undoCommand();
    if (!error)
        --m_current;
    return error;
}

bool CommandStack::canRedo() const
{
    return m_used > m_current;
}

bool CommandStack::canUndo() const
{
    return m_current > 0;
}

void CommandStack::saved()
{
    m_lastSave = m_current - 1;
}

bool CommandStack::isModified() const
{
    return m_lastSave != m_current - 1;
}

XSCommandStack::XSCommandStack(XSModel* model)
    : m_model(model)
{
}

int XSCommandStack::doCommand(XSCommand* command)
{
    command->init(m_model);
    return CommandStack::doCommand(command);
}

int XSCommandStack::undoCommand()
{
    return CommandStack::undoCommand();
}

int XSCommandStack::redoCommand()
{
    return CommandStack::redoCommand();
}
