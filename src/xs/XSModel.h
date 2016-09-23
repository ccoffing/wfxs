#ifndef XS_MODEL_H
#define XS_MODEL_H

#include "XSProperties.h"
#include "XSSquareIO.h"
#include "XSStitchTypes.h"
#include "XSToolState.h"

class XSCommand;
class XSController;
class XSLayer;
class XSPoint;
class XSSquare;

enum DrawStyle { DrawStyle_Design, DrawStyle_Pattern, DrawStyle_Realistic };


/** The model stores the state of a cross stitch pattern.
 */
class XSModel {
public:
    XSModel(unsigned int sizeX, unsigned int sizeY);
    XSModel& operator=(XSModel&& rhs);
    ~XSModel();

    void setController(XSController* controller)
    {
        m_controller = controller;
    }

    std::ostream& SaveObject(std::ostream& stream) const;
    std::istream& LoadObject(std::istream& stream);

    /** Resizes the doc to the specified size.  If the size is larger than the existing size,
     * the new squares are left empty.  If the size is smaller, existing squares are simply
     * deleted.  The pictures remains rooted at the same origin.
     */
    void Resize(unsigned int sizeX, unsigned int sizeY);

    unsigned int SquaresX() const;
    unsigned int SquaresY() const;

    XSProperties& GetProperties();
    const XSProperties& GetProperties() const;
    XSFlossPalette const& GetFlossPalette() const;

    void SetProperties(XSProperties const& properties);
    void SetFlossPalette(XSFlossPalette const&);
    void PreviousFloss();
    void NextFloss();
    void SetFloss(unsigned int i);
    StitchType SetStitch(unsigned int squareX, unsigned int squareY, unsigned int xPercent,
            unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite);
    void SetKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
            unsigned int flossIndex, bool overwrite);
    void SetBead(unsigned int x, unsigned int y, unsigned int region, unsigned int colorIndex,
            bool overwrite);
    void ClearSquare(unsigned int x, unsigned int y, unsigned int layer);
    void SetSquareData(
            XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer);
    void SetSquareDataNoInval(
            XSSquareIO const* square, unsigned int x, unsigned int y, unsigned int layer);
    void GetSquareData(XSSquareIO* square, unsigned int squareX, unsigned int squareY,
            unsigned int layer) const;
    XSSquare& getSquare(unsigned int x, unsigned int y, unsigned int layer);
    void AddLayer();
    void DelLayer(unsigned int index);
    void DownLayer();
    void UpLayer();
    unsigned int GetNumberLayers() const;
    unsigned int GetCurrentLayerIndex() const;
    void Backstitch();
    void SetShowGrid(bool f);
    void SetShowRulers(bool f);
    inline bool IsShowGrid() const
    {
        return m_showGrid;
    }

    inline bool IsShowRulers() const
    {
        return m_showRulers;
    }

    unsigned int SetZoom(unsigned int zoom);
    unsigned int GetZoom() const;
    void GetZoomRange(unsigned int* min, unsigned int* max) const;
    enum DrawStyle GetDrawStyle() const;
    void SetDrawStyle(enum DrawStyle);

    inline XSToolState& ToolState()
    {
        return m_toolState;
    }

    inline const XSToolState& ToolState() const
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

    unsigned int m_numLayers;
    unsigned int m_numAllocatedLayers;
    unsigned int m_currentLayerIndex;
    XSLayer** m_layers;
    bool m_showGrid;
    bool m_showRulers;
    bool m_editable;

    XSProperties m_properties;

    enum DrawStyle m_drawStyle;
};

#endif
