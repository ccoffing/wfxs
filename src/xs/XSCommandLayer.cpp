#include "XSCommandLayer.h"
#include "XSModel.h"


int XSCommandLayerAdd::doCommand()
{
    m_model->addLayer();
    return true;
}

int XSCommandLayerAdd::undoCommand()
{
    m_model->delLayer(m_model->getNumberLayers());
    return true;
}

char const* XSCommandLayerAdd::getDescription() const
{
    return _("new layer");
}

int XSCommandLayerDel::doCommand()
{
    m_model->delLayer(m_model->getNumberLayers());
    return true;
}

int XSCommandLayerDel::undoCommand()
{
    // FIXME
    return false;
}

char const* XSCommandLayerDel::getDescription() const
{
    return _("delete layer");
}

int XSCommandLayerUp::doCommand()
{
    if (m_model->getCurrentLayerIndex() > 0) {
        m_model->upLayer();
        return true;
    }
    return false;
}

int XSCommandLayerUp::undoCommand()
{
    m_model->downLayer();
    return true;
}

char const* XSCommandLayerUp::getDescription() const
{
    return _("layer up");
}

int XSCommandLayerDown::doCommand()
{
    if (m_model->getCurrentLayerIndex() + 1 < m_model->getNumberLayers()) {
        m_model->downLayer();
        return true;
    }
    return false;
}

int XSCommandLayerDown::undoCommand()
{
    m_model->upLayer();
    return true;
}

char const* XSCommandLayerDown::getDescription() const
{
    return _("layer down");
}
