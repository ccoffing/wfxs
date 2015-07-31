#ifndef _XS_PROPERTIES_WINDOW_H
#define _XS_PROPERTIES_WINDOW_H

#include "XSProperties.h"
#include <BAF/interface/Window.h>

class BMessage;
namespace BAF {
class TextControl;
class TextView;
}
class XSDocument;

class XSPropertiesWindow : public BAF::Window {
public:
    XSPropertiesWindow(XSDocument *document,
            XSProperties const &properties);

    bool QuitRequested();
    virtual void MessageReceived(BMessage *message);

    XSProperties m_properties;

protected:
    void GetProperties();
    void SetProperties();

private:
    XSDocument *m_document;
    bool m_save;

    BAF::TextControl *m_author;
    BAF::TextControl *m_fabric;
    BAF::TextControl *m_title;
    BAF::TextView *m_instructions;
};

#endif
