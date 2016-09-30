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
    XSQtCanvas(XSModel& model, XSController& controller, QWidget* parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    QRect squareInnerRect(unsigned int x, unsigned int y);
    void refreshSquare(unsigned int x, unsigned int y);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    void drawDesignSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
            unsigned int zoom, XSFlossPalette const& flossPalette);
    void drawPatternSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
            unsigned int zoom, XSFlossPalette const& flossPalette);
    void drawRealisticSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
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
