#ifndef XS_CONTROLLER_H
#define XS_CONTROLLER_H

/** @file
 */

#include <vector>

#include "XSCommandStack.h"
#include "XSModel.h"
#include "XSPoint.h"

class CommandEvent;
class XSEditFlossWindow;
class XSPropertiesWindow;


/**
 */
class XSController {
public:
    XSController(XSModel *model);

    void save(const XSModel &model, const char *filename);
    void open(const char *filename);

    void OnClearSquare(unsigned int x, unsigned int y);
    void OnClearSquares(std::vector<XSPoint> const &points);
    void OnClick(unsigned int x, unsigned int y);
    void OnColorPicker(CommandEvent &event);
    void OnDesignStyle(CommandEvent &event);
    void OnEditFlossPalette(CommandEvent &event);
    void OnExport(CommandEvent &event);
    void OnFloodFill(unsigned int x, unsigned int y, XSSquareIO const &newsq);
    void OnFullStitch();
    void OnHalfAutoStitch();
    void OnHalfBottomStitch();
    void OnHalfTopStitch();
    void OnImport(CommandEvent &event);
    // void OnKeyDown(KeyEvent& event);
    void OnLayerAdd(CommandEvent &event);
    void OnLayerDel(CommandEvent &event);
    void OnLayerDown(CommandEvent &event);
    void OnLayerUp(CommandEvent &event);
    void OnNew();
    void OnOverwrite();
    void OnPatternStyle(CommandEvent &event);
    void OnProperties(CommandEvent &event);
    void OnQuarterAutoStitch();
    void OnQuarterLLStitch();
    void OnQuarterLRStitch();
    void OnQuarterULStitch();
    void OnQuarterURStitch();
    void OnRealisticStyle(CommandEvent &event);
    void OnSetBead(unsigned int x, unsigned int y, unsigned int region);
    void OnSetKnot(unsigned int x, unsigned int y, unsigned int region);
    void OnSetStitch(unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent);
    void OnSetStitches(std::vector<XSPoint> &points);
    void OnShowGrid(CommandEvent &event);
    void OnThreeQuarterAutoStitch();
    void OnThreeQuarterLLStitch();
    void OnThreeQuarterLRStitch();
    void OnThreeQuarterULStitch();
    void OnThreeQuarterURStitch();
    void OnToolEraserLarge(CommandEvent &event);
    void OnToolEraserMedium(CommandEvent &event);
    void OnToolEraserSmall(CommandEvent &event);
    void OnToolFloodFill(CommandEvent &event);
    void OnZoomIn(CommandEvent &event);
    void OnZoomOut(CommandEvent &event);
    void setFloss(unsigned int i);
    void nextFloss();
    void previousFloss();

    void PropertiesWindowClosing(XSProperties const &properties);
    void FlossPaletteWindowClosing(XSFlossPalette const &flossPalette);

    inline XSModel *GetModel()
    {
        return m_model;
    }

    StitchType GetStitchType() const;
    ToolType   GetToolType()   const;

private:
    XSCommandStack m_commandStack;

public:
    XSModel *m_model;

private:
    XSPropertiesWindow *m_propertiesWindow;
    XSEditFlossWindow *m_editFlossWindow;
};

#endif
