#include "Logger.h"
#include "XSController.h"
#include "XSFileNative.h"
#include "XSLayer.h"
#include "XSModel.h"
#include "XSProperties.h"

#define LOG_NAME "xs.Model"

XSModel::XSModel(unsigned int x,
        unsigned int y) :
    m_controller(0),
    m_sizeX(x),
    m_sizeY(y),
    m_zoom(10),
    // TODO: XSView::GetPreferredZoom()),
    m_minZoom(4),
    m_maxZoom(50),
    m_numLayers(0),
    m_numAllocatedLayers(16),
    m_currentLayerIndex(0),
    m_layers(new XSLayer *[m_numAllocatedLayers]),
    m_showGrid(true),
    m_showRulers(true),
    m_editable(true),
    m_properties(),
    m_drawStyle(DrawStyle_Design)
{
    AddLayer();
}

XSModel &XSModel::operator=(XSModel &&rhs)
{
    m_toolState = rhs.m_toolState;
    m_controller = rhs.m_controller;
    m_sizeX = rhs.m_sizeX;
    m_sizeX = rhs.m_sizeX;
    m_sizeY = rhs.m_sizeY;
    m_zoom = rhs.m_zoom;
    m_minZoom = rhs.m_minZoom;
    m_maxZoom = rhs.m_maxZoom;
    m_numLayers = rhs.m_numLayers;
    m_numAllocatedLayers = rhs.m_numAllocatedLayers;
    m_currentLayerIndex = rhs.m_currentLayerIndex;
    m_layers = rhs.m_layers;
    m_numLayers = rhs.m_numLayers;
    m_showGrid = rhs.m_showGrid;
    m_showRulers = rhs.m_showRulers;
    m_editable = rhs.m_editable;
    m_properties = rhs.m_properties;
    m_drawStyle = rhs.m_drawStyle;

    rhs.m_layers = nullptr;

    m_controller->m_model = this;

    return *this;
}

XSModel::~XSModel()
{
    delete[] m_layers;
}

std::ostream &XSModel::SaveObject(std::ostream &stream) const
{
    StreamOutNative(stream, this);
    return stream;
}

std::istream &XSModel::LoadObject(std::istream &stream)
{
    StreamInNative(stream, this);
    return stream;
}

XSProperties &XSModel::GetProperties()
{
    return m_properties;
}

const XSProperties &XSModel::GetProperties() const
{
    return m_properties;
}

void XSModel::SetProperties(XSProperties const &properties)
{
    m_properties = properties;
    // TODO:  notify controller to refresh title on window
}

XSFlossPalette const &XSModel::GetFlossPalette() const
{
    return m_toolState.m_flossPalette;
}

void XSModel::SetFlossPalette(XSFlossPalette const &flossPalette)
{
    m_toolState.m_flossPalette = flossPalette;
}

void XSModel::PreviousFloss()
{
    if (m_toolState.m_flossIndex) {
        //        m_controller->GetFlossPaletteView()->RefreshWell(m_toolState.m_flossIndex);
        m_toolState.m_flossIndex--;
        //        m_controller->GetFlossPaletteView()->RefreshWell(m_toolState.m_flossIndex);
    }
}

void XSModel::NextFloss()
{
    if (m_toolState.m_flossIndex + 1 < m_toolState.m_flossPalette.size()) {
        //        m_controller->GetFlossPaletteView()->RefreshWell(m_toolState.m_flossIndex);
        m_toolState.m_flossIndex++;
        //        m_controller->GetFlossPaletteView()->RefreshWell(m_toolState.m_flossIndex);
    }
}

void XSModel::SetFloss(unsigned int i)
{
    if (i < m_toolState.m_flossPalette.size())
        m_toolState.m_flossIndex = i;
}

void XSModel::AddLayer()
{
    XSLayer *layer = new XSLayer(m_sizeX, m_sizeY);

    if (m_numLayers == m_numAllocatedLayers) {
        XSLayer **newLayers = new XSLayer *[m_numAllocatedLayers * 2];
        for (unsigned int i = 0; i < m_numLayers; ++i) {
            newLayers[i] = m_layers[i];
        }
        delete[] m_layers;
        m_layers = newLayers;
        m_numAllocatedLayers *= 2;
    }
    m_layers[m_numLayers++] = layer;
}

void XSModel::DelLayer(unsigned int i)
{
    assert(i < m_numLayers);
    assert(m_numLayers > 1);

    //    if (m_currentLayerIndex == i)
    //        m_controller->GetView()->Refresh();

    delete m_layers[i];
    for (unsigned int layer = i; layer < m_numLayers - 1; ++layer) {
        m_layers[layer] = m_layers[layer + 1];
    }
    m_numLayers--;

    if (m_currentLayerIndex == m_numLayers)
        m_currentLayerIndex--;
}

void XSModel::DownLayer()
{
    if (m_currentLayerIndex < m_numLayers - 1) {
        m_currentLayerIndex++;
        //        m_controller->GetView()->Refresh();
    }
}

void XSModel::UpLayer()
{
    if (m_currentLayerIndex > 0) {
        m_currentLayerIndex--;
        //        m_controller->GetView()->Refresh();
    }
}

unsigned int XSModel::GetCurrentLayerIndex() const
{
    return m_currentLayerIndex;
}

unsigned int XSModel::GetNumberLayers() const
{
    return m_numLayers;
}

void XSModel::Resize(unsigned int sizeX, unsigned int sizeY)
{
    // FIXME:  catch exceptions...
    for (unsigned int i = 0; i < m_numLayers; ++i) {
        m_layers[i]->Resize(sizeX, sizeY);
    }
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    //    if (m_controller) {
    //        m_controller->GetView()->AdjustScrollBars();
    //        m_controller->GetView()->Refresh();
    //    }
}

unsigned int XSModel::SquaresX() const
{
    return m_sizeX;
}

unsigned int XSModel::SquaresY() const
{
    return m_sizeY;
}

StitchType XSModel::SetStitch(unsigned int squareX,
        unsigned int squareY,
        unsigned int xPercent,
        unsigned int yPercent,
        StitchType stitchType,
        unsigned int flossIndex,
        bool overwrite)
{
    Log::debug(LOG_NAME, "%u %u %u", squareX, squareY, flossIndex);
    StitchType st = m_layers[m_currentLayerIndex]->SetStitch(
            squareX, squareY, xPercent, yPercent, stitchType, flossIndex, overwrite);

    //    m_controller->GetView()->RefreshSquareContents(squareX, squareY);
    return st;
}

void XSModel::SetKnot(unsigned int x,
        unsigned int y,
        unsigned int region,
        KnotType knotType,
        unsigned int flossIndex,
        bool overwrite)
{
    m_layers[m_currentLayerIndex]->SetKnot(x, y, region, knotType, flossIndex, overwrite);
    //    m_controller->GetView()->RefreshSquareContents(x, y);
}

void XSModel::SetBead(unsigned int x,
        unsigned int y,
        unsigned int region,
        unsigned int colorIndex,
        bool overwrite)
{
    m_layers[m_currentLayerIndex]->SetBead(x, y, region, colorIndex, overwrite);
    //    m_controller->GetView()->RefreshSquareContents(x, y);
}

void XSModel::SetSquareData(XSSquareIO const *square,
        unsigned int x,
        unsigned int y,
        unsigned int layer)
{
    assert(layer < m_numLayers);
    m_layers[layer]->SetSquareData(square, x, y);
    //    m_controller->GetView()->RefreshSquareContents(x, y);
}

void XSModel::SetSquareDataNoInval(XSSquareIO const *square,
        unsigned int x,
        unsigned int y,
        unsigned int layer)
{
    assert(layer < m_numLayers);
    m_layers[layer]->SetSquareData(square, x, y);
}

void XSModel::GetSquareData(XSSquareIO *square,
        unsigned int x,
        unsigned int y,
        unsigned int layer) const
{
    assert(layer < m_numLayers);
    m_layers[layer]->GetSquareData(square, x, y);
}

XSSquare &XSModel::getSquare(unsigned int x, unsigned int y, unsigned int layer)
{
    assert(layer < m_numLayers);
    return *m_layers[layer]->getSquare(x, y);
}

void XSModel::ClearSquare(unsigned int x,
        unsigned int y,
        unsigned int layer)
{
    assert(layer < m_numLayers);
    m_layers[layer]->ClearSquare(x, y);
    //    m_controller->GetView()->RefreshSquareContents(x, y);
}

unsigned int XSModel::SetZoom(unsigned int zoom)
{
    unsigned int oldZoom = m_zoom;

    if (zoom < m_minZoom)
        m_zoom = m_minZoom;
    else if (zoom > m_maxZoom)
        m_zoom = m_maxZoom;
    else
        m_zoom = zoom;

    if (m_zoom != oldZoom) {
        // FIXME: reset originX,Y to move towards cursorX,Y
        //        m_controller->GetView()->ResetScrollBars();
        //        m_controller->GetView()->Refresh();
    }

    return m_zoom;
}

unsigned int XSModel::GetZoom() const
{
    return m_zoom;
}

void XSModel::GetZoomRange(unsigned int *min, unsigned int *max) const
{
    if (min)
        *min = m_minZoom;
    if (max)
        *max = m_maxZoom;
}

enum DrawStyle XSModel::GetDrawStyle() const
{
    return m_drawStyle;
}

void XSModel::SetDrawStyle(enum DrawStyle style)
{
    m_drawStyle = style;
    //    m_controller->GetView()->Refresh();
}

void XSModel::SetShowGrid(bool grid)
{
    if (m_showGrid != grid) {
        m_showGrid = grid;
        //        m_controller->GetView()->Refresh();
    }
}

void XSModel::SetShowRulers(bool rulers)
{
    if (m_showRulers != rulers) {
        m_showRulers = rulers;
        // FIXME
    }
}
