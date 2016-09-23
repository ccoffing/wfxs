#include "XSCommand.h"
#include "XSCommandStack.h"
#include "XSDocument.h"

#include <assert.h>


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

Command const* CommandStack::GetUndoCommand() const
{
    if (!CanUndo())
        return 0;
    return m_commands[m_current - 1];
}

Command const* CommandStack::GetReDoCommand() const
{
    if (!CanReDo())
        return 0;
    return m_commands[m_current];
}

int CommandStack::Do(Command* command)
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

    // printf("doing %d (%s)\n", m_current - 1, m_commands[m_current - 1]->GetDescription());
    int error = m_commands[m_current - 1]->Do();
    if (error)
        --m_current;
    return error;
}

int CommandStack::ReDo()
{
    assert(CanReDo());
    // printf("redoing %d of %d (%s)\n", m_current, m_used,
    // m_commands[m_current]->GetDescription());
    int error = m_commands[m_current]->ReDo();
    if (!error)
        ++m_current;
    return error;
}

int CommandStack::Undo()
{
    assert(CanUndo());
    // printf("undoing %d of %d (%s)\n", m_current-1, m_used,
    // m_commands[m_current-1]->GetDescription());
    int error = m_commands[m_current - 1]->Undo();
    if (!error)
        --m_current;
    return error;
}

bool CommandStack::CanReDo() const
{
    return m_used > m_current;
}

bool CommandStack::CanUndo() const
{
    return m_current > 0;
}

void CommandStack::Saved()
{
    m_lastSave = m_current - 1;
}

bool CommandStack::IsModified() const
{
    return m_lastSave != m_current - 1;
}

XSCommandStack::XSCommandStack(XSModel* model)
    : m_model(model)
{
}

int XSCommandStack::Do(XSCommand* command)
{
    command->Init(m_model);
    return CommandStack::Do(command);
}

int XSCommandStack::Undo()
{
    return CommandStack::Undo();
}

int XSCommandStack::ReDo()
{
    return CommandStack::ReDo();
}
