#ifndef XS_MODEL_H
#define XS_MODEL_H

#include "XSLayer.h"
#include "XSProperties.h"
#include "XSSquareIO.h"
#include "XSStitchTypes.h"
#include "XSToolState.h"

#include <memory>
#include <vector>

class XSCommand;
class XSController;
class XSPoint;
class XSSquare;

enum DrawStyle { DrawStyle_Design, DrawStyle_Pattern, DrawStyle_Realistic };


/** The model stores the state of a cross stitch pattern.
 */
class XSModel {
public:
    XSModel(unsigned int sizeX, unsigned int sizeY);
    XSModel(XSModel&& rhs);

    void setController(XSController* controller)
    {
        m_controller = controller;
    }

    std::ostream& saveObject(std::ostream& stream) const;
    std::istream& loadObject(std::istream& stream);

    /** Resizes the doc to the specified size.  If the size is larger than the existing size,
     * the new squares are left empty.  If the size is smaller, existing squares are simply
     * deleted.  The pictures remains rooted at the same origin.
     */
    void resize(unsigned int sizeX, unsigned int sizeY);

    unsigned int squaresX() const;
    unsigned int squaresY() const;

    XSProperties& getProperties();
    const XSProperties& getProperties() const;
    XSFlossPalette const& getFlossPalette() const;

    void setProperties(XSProperties const& properties);
    void setFlossPalette(XSFlossPalette const&);
    void previousFloss();
    void nextFloss();
    void setFloss(unsigned int i);
    StitchType setStitch(unsigned int squareX, unsigned int squareY, unsigned int xPercent,
            unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite);
    void setKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
            unsigned int flossIndex, bool overwrite);
    void setBead(unsigned int x, unsigned int y, unsigned int region, unsigned int colorIndex,
            bool overwrite);
    void clearSquare(unsigned int x, unsigned int y, unsigned int layer);
    void setSquareData(
            XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer);
    void setSquareDataNoInval(
            XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer);
    void getSquareData(XSSquareIO* square, unsigned int squareX, unsigned int squareY,
            unsigned int layer) const;
    XSSquare& getSquare(unsigned int x, unsigned int y, unsigned int layer);
    void addLayer();
    void delLayer(unsigned int index);
    void downLayer();
    void upLayer();
    unsigned int getNumberLayers() const;
    unsigned int getCurrentLayerIndex() const;
    void backstitch();
    void setShowGrid(bool f);
    void setShowRulers(bool f);
    inline bool isShowGrid() const
    {
        return m_showGrid;
    }

    inline bool isShowRulers() const
    {
        return m_showRulers;
    }

    unsigned int setZoom(unsigned int zoom);
    unsigned int getZoom() const;
    void getZoomRange(unsigned int* min, unsigned int* max) const;
    enum DrawStyle getDrawStyle() const;
    void setDrawStyle(enum DrawStyle);

    inline XSToolState& toolState()
    {
        return m_toolState;
    }

    inline const XSToolState& toolState() const
    {
        return m_toolState;
    }

    XSToolState m_toolState;

private:
    XSController* m_controller;

    unsigned int m_sizeX;
    unsigned int m_sizeY;

    /** Width of each square (not including the dividing lines)
     */
    unsigned int m_zoom;
    unsigned int m_minZoom;
    unsigned int m_maxZoom;

    unsigned int m_currentLayerIndex;
    std::vector<std::unique_ptr<XSLayer>> m_layers;
    bool m_showGrid;
    bool m_showRulers;
    bool m_editable;

    XSProperties m_properties;

    enum DrawStyle m_drawStyle;
};

#endif
