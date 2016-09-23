#ifndef XS_QT_CANVAS_H
#define XS_QT_CANVAS_H

#include "xs/XSController.h"
#include "xs/XSModel.h"

#include <QBrush>
#include <QPen>
#include <QWidget>

class XSSelectionPath;
class BitMask;

class XSQtCanvas : public QWidget {
    Q_OBJECT

public:
    XSQtCanvas(XSModel& model, XSController& controller, QWidget* parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QRect squareInnerRect(unsigned int x, unsigned int y);
    void refreshSquare(unsigned int x, unsigned int y);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

    void DrawDesignSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
            unsigned int zoom, XSFlossPalette const& flossPalette);
    void DrawPatternSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
            unsigned int zoom, XSFlossPalette const& flossPalette);
    void DrawRealisticSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
            unsigned int zoom, XSFlossPalette const& flossPalette);

private:
    XSModel& m_model;
    XSController& m_controller;

    unsigned int cursorX, cursorY;
    bool m_resetCursor;

    bool m_mouseDown;
    bool m_cmdDown;
    unsigned int m_prevX;
    unsigned int m_prevY;
    unsigned int m_prevRegion;

    XSSelectionPath* m_selectionPath;
    BitMask* m_selectionMask;
};

#endif
