#include "XSCommandShowGrid.h"
#include "XSModel.h"


XSCommandShowGrid::XSCommandShowGrid(bool showGrid)
    : m_oldShowGrid(showGrid /* meaninless, to pacify compiler*/)
    , m_newShowGrid(showGrid)
{
}

int XSCommandShowGrid::Do()
{
    m_oldShowGrid = m_model->IsShowGrid();
    m_model->SetShowGrid(m_newShowGrid);
    return true;
}

int XSCommandShowGrid::Undo()
{
    m_model->SetShowGrid(m_oldShowGrid);
    return true;
}

char const* XSCommandShowGrid::GetDescription() const
{
    return _("show grid");
}
