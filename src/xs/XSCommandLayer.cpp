#include "XSCommandLayer.h"
#include "XSModel.h"


int XSCommandLayerAdd::Do()
{
    m_model->AddLayer();
    return true;
}

int XSCommandLayerAdd::Undo()
{
    m_model->DelLayer(m_model->GetNumberLayers());
    return true;
}

char const* XSCommandLayerAdd::GetDescription() const
{
    return _("new layer");
}

int XSCommandLayerDel::Do()
{
    m_model->DelLayer(m_model->GetNumberLayers());
    return true;
}

int XSCommandLayerDel::Undo()
{
    // FIXME
    return false;
}

char const* XSCommandLayerDel::GetDescription() const
{
    return _("delete layer");
}

int XSCommandLayerUp::Do()
{
    if (m_model->GetCurrentLayerIndex() > 0) {
        m_model->UpLayer();
        return true;
    }
    return false;
}

int XSCommandLayerUp::Undo()
{
    m_model->DownLayer();
    return true;
}

char const* XSCommandLayerUp::GetDescription() const
{
    return _("layer up");
}

int XSCommandLayerDown::Do()
{
    if (m_model->GetCurrentLayerIndex() + 1 < m_model->GetNumberLayers()) {
        m_model->DownLayer();
        return true;
    }
    return false;
}

int XSCommandLayerDown::Undo()
{
    m_model->UpLayer();
    return true;
}

char const* XSCommandLayerDown::GetDescription() const
{
    return _("layer down");
}
