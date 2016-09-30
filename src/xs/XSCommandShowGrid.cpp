#include "XSCommandShowGrid.h"
#include "XSModel.h"


XSCommandShowGrid::XSCommandShowGrid(bool showGrid)
    : m_oldShowGrid(showGrid /* meaninless, to pacify compiler*/)
    , m_newShowGrid(showGrid)
{
}

int XSCommandShowGrid::doCommand()
{
    m_oldShowGrid = m_model->isShowGrid();
    m_model->setShowGrid(m_newShowGrid);
    return true;
}

int XSCommandShowGrid::undoCommand()
{
    m_model->setShowGrid(m_oldShowGrid);
    return true;
}

char const* XSCommandShowGrid::getDescription() const
{
    return _("show grid");
}
