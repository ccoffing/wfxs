#ifndef XS_QT_EDIT_WINDOW_H
#define XS_QT_EDIT_WINDOW_H

#include "qt/XSQtCanvas.h"
#include "xs/XSController.h"

#include <QMainWindow>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class XSQtEditWindow : public QMainWindow {
    Q_OBJECT

public:
    XSQtEditWindow();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void newFile();
    void open();
    void save();
    void print();
    void properties();
    void fullStitch();
    void halfStitch();
    void halfAutoStitch();
    void halfTopStitch();
    void halfBottomStitch();
    void threeQuarterAutoStitch();
    void threeQuarterULStitch();
    void threeQuarterURStitch();
    void threeQuarterLLStitch();
    void threeQuarterLRStitch();
    void quarterStitch();
    void setFloss(int i);
    void overwrite();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();

private:
    void createActions();
    void createMenus();

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* formatMenu;
    QMenu* viewMenu;
    QMenu* toolsMenu;
    QMenu* helpMenu;
    QActionGroup* alignmentGroup;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* printAct;
    QAction* propertiesAct;
    QAction* closeAct;
    QAction* exitAct;
    QAction* undoAct;
    QAction* redoAct;
    QAction* cutAct;
    QAction* copyAct;
    QAction* pasteAct;
    QAction* fullStitchAct;
    QAction* halfAutoStitchAct;
    QAction* halfTopStitchAct;
    QAction* halfBottomStitchAct;
    QAction* threeQuarterAutoStitchAct;
    QAction* threeQuarterULStitchAct;
    QAction* threeQuarterURStitchAct;
    QAction* threeQuarterLLStitchAct;
    QAction* threeQuarterLRStitchAct;
    QAction* overwriteAct;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* leftAlignAct;
    QAction* rightAlignAct;
    QAction* justifyAct;
    QAction* centerAct;
    QAction* setLineSpacingAct;
    QAction* setParagraphSpacingAct;
    QAction* aboutAct;

    XSModel m_model;
    XSController m_controller;
    XSQtCanvas* m_area;

    void updateStitchMessage();
};

#endif
