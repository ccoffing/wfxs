#ifndef _XS_FRAME_H
#define _XS_FRAME_H

/** @file
 *  @brief TODO
 */

#include <vector>

#include <wx/docview.h>
#include <wx/gdicmn.h>

#include "XSCanvas.h"
#include "XSDocument.h"
#include "XSToolState.h"

class XSView;
class XSEditFlossWindow;
class XSFlossPaletteView;
class XSPropertiesWindow;
class XSToolWindow;
class wxMenuBar;

/**
 *  @brief TODO
 */
class XSDocChild : public wxDocChildFrame {
    DECLARE_EVENT_TABLE()

public:
    XSDocChild(XSDocument* doc, XSView* view);
    //    XSCanvas *CreateCanvas(wxView *view, wxFrame *parent);

    void OnImport(wxCommandEvent& event);
    void OnExport(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void OnColorPicker(wxCommandEvent& event);
    void OnDesignStyle(wxCommandEvent& event);
    void OnFullStitch(wxCommandEvent& event);
    void OnHalfAutoStitch(wxCommandEvent& event);
    void OnHalfBottomStitch(wxCommandEvent& event);
    void OnHalfTopStitch(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnLayerAdd(wxCommandEvent& event);
    void OnLayerDel(wxCommandEvent& event);
    void OnLayerDown(wxCommandEvent& event);
    void OnLayerUp(wxCommandEvent& event);
    void OnOverwrite(wxCommandEvent& event);
    void OnPatternStyle(wxCommandEvent& event);
    void OnProperties(wxCommandEvent& event);
    void OnQuarterAutoStitch(wxCommandEvent& event);
    void OnQuarterLLStitch(wxCommandEvent& event);
    void OnQuarterLRStitch(wxCommandEvent& event);
    void OnQuarterULStitch(wxCommandEvent& event);
    void OnQuarterURStitch(wxCommandEvent& event);
    void OnRealisticStyle(wxCommandEvent& event);
    void OnShowGrid(wxCommandEvent& event);
    void OnThreeQuarterAutoStitch(wxCommandEvent& event);
    void OnThreeQuarterLLStitch(wxCommandEvent& event);
    void OnThreeQuarterLRStitch(wxCommandEvent& event);
    void OnThreeQuarterULStitch(wxCommandEvent& event);
    void OnThreeQuarterURStitch(wxCommandEvent& event);
    void OnEditFlossPalette(wxCommandEvent& event);
    void OnToolEraserLarge(wxCommandEvent& event);
    void OnToolEraserMedium(wxCommandEvent& event);
    void OnToolEraserSmall(wxCommandEvent& event);
    void OnToolFloodFill(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnClearSquare(unsigned int x, unsigned int y);
    void OnClearSquares(std::vector<wxPoint> const& points);
    void OnClick(unsigned int x, unsigned int y);
    void OnFloodFill(
            unsigned int x, unsigned int y, XSSquareIO const& oldsq, XSSquareIO const& newsq);
    void OnSetBead(unsigned int x, unsigned int y, unsigned int region);
    void OnSetKnot(unsigned int x, unsigned int y, unsigned int region);
    void OnSetStitch(unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent);
    void OnSetStitches(std::vector<wxPoint>& points);

    void PropertiesWindowClosing(XSProperties const& properties);
    void FlossPaletteWindowClosing(XSFlossPalette const& flossPalette);

    inline XSDocument* GetDocument()
    {
        return m_document;
    }
    inline XSCanvasPage* GetView()
    {
        return m_view;
    }
    inline XSCanvas* GetCanvas()
    {
        return m_view->GetCanvas();
    }
    inline XSFlossPaletteView* GetFlossPaletteView()
    {
        return m_flossPaletteView;
    }

    StitchType GetStitchType() const;
    ToolType GetToolType() const;

private:
    XSDocument* m_document;
    XSCanvasPage* m_view;
    wxMenuBar* m_menuBar;
    XSFlossPaletteView* m_flossPaletteView;
    XSPropertiesWindow* m_propertiesWindow;
    XSEditFlossWindow* m_editFlossWindow;

    void HighlightTool(Tool tool);
};

#endif
