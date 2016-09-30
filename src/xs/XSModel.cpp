#include "XSModel.h"
#include "Logger.h"
#include "XSController.h"
#include "XSFileNative.h"
#include "XSLayer.h"
#include "XSProperties.h"

#define LOG_NAME "xs.Model"

XSModel::XSModel(unsigned int x, unsigned int y)
    : m_controller(0)
    , m_sizeX(x)
    , m_sizeY(y)
    , m_zoom(10)
    // TODO: XSView::getPreferredZoom()),
    , m_minZoom(4)
    , m_maxZoom(50)
    , m_currentLayerIndex(0)
    , m_showGrid(true)
    , m_showRulers(true)
    , m_editable(true)
    , m_properties()
    , m_drawStyle(DrawStyle_Design)
{
    addLayer();
}

XSModel::XSModel(XSModel&& rhs)
{
    m_toolState = rhs.m_toolState;
    m_controller = rhs.m_controller;
    m_sizeX = rhs.m_sizeX;
    m_sizeX = rhs.m_sizeX;
    m_sizeY = rhs.m_sizeY;
    m_zoom = rhs.m_zoom;
    m_minZoom = rhs.m_minZoom;
    m_maxZoom = rhs.m_maxZoom;
    m_currentLayerIndex = rhs.m_currentLayerIndex;
    m_layers = std::move(rhs.m_layers);
    m_showGrid = rhs.m_showGrid;
    m_showRulers = rhs.m_showRulers;
    m_editable = rhs.m_editable;
    m_properties = rhs.m_properties;
    m_drawStyle = rhs.m_drawStyle;

    m_controller->m_model = this;
}

std::ostream& XSModel::saveObject(std::ostream& stream) const
{
    StreamOutNative(stream, this);
    return stream;
}

std::istream& XSModel::loadObject(std::istream& stream)
{
    StreamInNative(stream, this);
    return stream;
}

XSProperties& XSModel::getProperties()
{
    return m_properties;
}

const XSProperties& XSModel::getProperties() const
{
    return m_properties;
}

void XSModel::setProperties(XSProperties const& properties)
{
    m_properties = properties;
    // TODO:  notify controller to refresh title on window
}

XSFlossPalette const& XSModel::getFlossPalette() const
{
    return m_toolState.m_flossPalette;
}

void XSModel::setFlossPalette(XSFlossPalette const& flossPalette)
{
    m_toolState.m_flossPalette = flossPalette;
}

void XSModel::previousFloss()
{
    if (m_toolState.m_flossIndex) {
        //        m_controller->getFlossPaletteView()->refreshWell(m_toolState.m_flossIndex);
        m_toolState.m_flossIndex--;
        //        m_controller->getFlossPaletteView()->refreshWell(m_toolState.m_flossIndex);
    }
}

void XSModel::nextFloss()
{
    if (m_toolState.m_flossIndex + 1 < m_toolState.m_flossPalette.size()) {
        //        m_controller->getFlossPaletteView()->refreshWell(m_toolState.m_flossIndex);
        m_toolState.m_flossIndex++;
        //        m_controller->getFlossPaletteView()->refreshWell(m_toolState.m_flossIndex);
    }
}

void XSModel::setFloss(unsigned int i)
{
    if (i < m_toolState.m_flossPalette.size())
        m_toolState.m_flossIndex = i;
}

void XSModel::addLayer()
{
    m_layers.push_back(std::unique_ptr<XSLayer>(new XSLayer(m_sizeX, m_sizeY)));
}

void XSModel::delLayer(unsigned int i)
{
    assert(i < m_layers.size());
    assert(m_layers.size() > 1);

    //    if (m_currentLayerIndex == i)
    //        m_controller->getView()->refresh();

    m_layers.erase(m_layers.begin() + i);

    if (m_currentLayerIndex == m_layers.size())
        m_currentLayerIndex--;
}

void XSModel::downLayer()
{
    if (m_currentLayerIndex < m_layers.size() - 1) {
        m_currentLayerIndex++;
        //        m_controller->getView()->refresh();
    }
}

void XSModel::upLayer()
{
    if (m_currentLayerIndex > 0) {
        m_currentLayerIndex--;
        //        m_controller->getView()->refresh();
    }
}

unsigned int XSModel::getCurrentLayerIndex() const
{
    return m_currentLayerIndex;
}

unsigned int XSModel::getNumberLayers() const
{
    return m_layers.size();
}

void XSModel::resize(unsigned int sizeX, unsigned int sizeY)
{
    // FIXME:  catch exceptions...
    for (unsigned int i = 0; i < m_layers.size(); ++i) {
        m_layers[i]->resize(sizeX, sizeY);
    }
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    //    if (m_controller) {
    //        m_controller->getView()->adjustScrollBars();
    //        m_controller->getView()->refresh();
    //    }
}

unsigned int XSModel::squaresX() const
{
    return m_sizeX;
}

unsigned int XSModel::squaresY() const
{
    return m_sizeY;
}

StitchType XSModel::setStitch(unsigned int squareX, unsigned int squareY, unsigned int xPercent,
        unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite)
{
    Log::debug(LOG_NAME, "%u %u %u", squareX, squareY, flossIndex);
    StitchType st = m_layers[m_currentLayerIndex]->setStitch(
            squareX, squareY, xPercent, yPercent, stitchType, flossIndex, overwrite);

    //    m_controller->getView()->refreshSquareContents(squareX, squareY);
    return st;
}

void XSModel::setKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
        unsigned int flossIndex, bool overwrite)
{
    m_layers[m_currentLayerIndex]->setKnot(x, y, region, knotType, flossIndex, overwrite);
    //    m_controller->getView()->refreshSquareContents(x, y);
}

void XSModel::setBead(unsigned int x, unsigned int y, unsigned int region, unsigned int colorIndex,
        bool overwrite)
{
    m_layers[m_currentLayerIndex]->setBead(x, y, region, colorIndex, overwrite);
    //    m_controller->getView()->refreshSquareContents(x, y);
}

void XSModel::setSquareData(
        XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer)
{
    assert(layer < m_layers.size());
    m_layers[layer]->setSquareData(square, x, y);
    //    m_controller->getView()->refreshSquareContents(x, y);
}

void XSModel::setSquareDataNoInval(
        XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer)
{
    assert(layer < m_layers.size());
    m_layers[layer]->setSquareData(square, x, y);
}

void XSModel::getSquareData(
        XSSquareIO* square, unsigned int x, unsigned int y, unsigned int layer) const
{
    assert(layer < m_layers.size());
    m_layers[layer]->getSquareData(square, x, y);
}

XSSquare& XSModel::getSquare(unsigned int x, unsigned int y, unsigned int layer)
{
    assert(layer < m_layers.size());
    return m_layers[layer]->getSquare(x, y);
}

void XSModel::clearSquare(unsigned int x, unsigned int y, unsigned int layer)
{
    assert(layer < m_layers.size());
    m_layers[layer]->clearSquare(x, y);
    //    m_controller->getView()->refreshSquareContents(x, y);
}

unsigned int XSModel::setZoom(unsigned int zoom)
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
        //        m_controller->getView()->resetScrollBars();
        //        m_controller->getView()->refresh();
    }

    return m_zoom;
}

unsigned int XSModel::getZoom() const
{
    return m_zoom;
}

void XSModel::getZoomRange(unsigned int* min, unsigned int* max) const
{
    if (min)
        *min = m_minZoom;
    if (max)
        *max = m_maxZoom;
}

enum DrawStyle XSModel::getDrawStyle() const
{
    return m_drawStyle;
}

void XSModel::setDrawStyle(enum DrawStyle style)
{
    m_drawStyle = style;
    //    m_controller->getView()->refresh();
}

void XSModel::setShowGrid(bool grid)
{
    if (m_showGrid != grid) {
        m_showGrid = grid;
        //        m_controller->getView()->refresh();
    }
}

void XSModel::setShowRulers(bool rulers)
{
    if (m_showRulers != rulers) {
        m_showRulers = rulers;
        // FIXME
    }
}
