#include <wx/sizer.h>
#include <wx/stattext.h>

#include "XSData.h"
#include "XSDocChild.h"
#include "XSEditFlossWindow.h"
#include "XSFlossPaletteView.h"
#include "XSMessages.h"


BEGIN_EVENT_TABLE(XSEditFlossWindow, wxFrame)
// FIXME
END_EVENT_TABLE()

XSEditFlossWindow::XSEditFlossWindow(XSDocChild* document, XSFlossPalette const& flossPalette)
    : wxFrame(NULL, -1, wxT("Floss Palette"))
    , m_flossPalette(flossPalette)
    , m_flossIndex(0)
    , m_flossPaletteView(0)
    , m_document(document)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* topsizer = new wxStaticBoxSizer(wxVERTICAL, panel);

    topsizer->Add(new wxStaticText(panel, -1, wxT("Current Floss Palette"), wxDefaultPosition,
                          wxDefaultSize, wxALIGN_LEFT),
            1, wxEXPAND);
    topsizer->Add(
            new wxStaticText(panel, -1, wxT("xxx"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT),
            1, wxALIGN_CENTER);
    topsizer->SetSizeHints(panel);
    panel->SetSizer(topsizer);
    panel->SetAutoLayout(true);
#if 0
    wxButton *ok_button;

// http://www.wxwidgets.org/manuals/stable/wx_wxcomboctrl.html#wxcomboctrl
    m_makerList = new wxComboCtrl(  );
    for (unsigned int i = 0; i < makerNames.size(); ++i) {
        m_makerList->InsertItem(m_makerList->GetItemCount(), makerNames[i]);
    }

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    FIXME

#if 0
    Box * topview = new Box(NULL, B_PLAIN_BORDER, -1,
            new VGroup(B_ALIGN_LEFT,
                    new StringView("Current Floss Palette"),
                    new Box(NULL, B_PLAIN_BORDER, 0,
                            new ScrollView(m_flossPaletteView =
                                        new XSFlossPaletteView(&m_flossPalette, &m_flossIndex),
                                    B_WILL_DRAW, false, true)),
                    new HGroup(B_ALIGN_TOP,
                            new Box("Available Colors", B_FANCY_BORDER, -1,
                                    new VGroup(B_ALIGN_LEFT,
                                            new HGroup(B_ALIGN_MIDDLE,
                                                    new HSpacer(),
                                                    new StringView("Floss List:"),
                                                    m_makerList,
                                                    0
                                                    ),
                                            // new PaletteView(  #, description, color, ...
                                            0
                                            )
                                    ),
                            new Box("Palette Editor", B_FANCY_BORDER, -1,
                                    new HGroup(B_ALIGN_MIDDLE,
                                            // new XSFlossView(),
                                            // new SpinBox(1, 6),
                                            new StringView("strands"),
                                            // new PictButton(a "X" button, to remove this entry.
                                            0
                                            )
                                    ),
                            0
                            ),
                    new HGroup(B_ALIGN_MIDDLE,
                            new HSpacer(),
                            ok_button = new wxButton(wxID_OK),
                            new wxButton(wxID_CANCEL),
                            0
                            ),
                    0
                    )
            );
#endif

    SetDefaultButton(ok_button);

    SetSizer(topsizer);

#endif
    CenterOnParent();
    Show(true);
}


XSEditFlossWindow::~XSEditFlossWindow()
{
}
