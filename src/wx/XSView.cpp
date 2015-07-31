#include "XSDocChild.h"
#include "XSView.h"


IMPLEMENT_DYNAMIC_CLASS(XSView, wxView)

XSView::XSView()
{
}


XSView::~XSView()
{
}



bool XSView::OnCreate(wxDocument *doc, long flags)
{
    m_viewDocument = doc;
    XSDocument *document = dynamic_cast<XSDocument *>(doc);
    XSDocChild *docchild = new XSDocChild(document, this);
    m_viewFrame = docchild;
    return true;
}


void XSView::OnDraw(wxDC *dc)
{
}


void XSView::OnUpdate(wxView *sender, wxObject *hint)
{
}


bool XSView::OnClose(bool deleteWindow)
{
    return true;
}
