#include "qt/XSPropertiesDialog.h"
#include "qt/mainwindow.h"
#include "xs/Logger.h"
#include "xs/XSColor.h"
#include "xs/XSFloss.h"
#include "xs/XSVersion.h"

#include <QFileDialog>
#include <QtGui>

#define LOG_NAME "qt.main"


XSQApplication::XSQApplication(int &argc, char *argv[]) :
    QApplication(argc, argv),
    m_app(argc, argv)
{
    m_window.show();
}


MainWindow::MainWindow() :
    m_model(40, 40),
    m_controller(&m_model)
{
    QWidget *widget = new QWidget;

    setCentralWidget(widget);

    // Toolbar
    QToolBar *toolbar = addToolBar(tr("Test"));
    toolbar->setMovable(1);
    toolbar->setFloatable(1);

    QPixmap openPix("resources/icons/open.xpm");
    QAction *openAct = toolbar->addAction(QIcon(openPix), "Open");
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    QPixmap savePix("resources/icons/save.xpm");
    QAction *saveAct = toolbar->addAction(QIcon(savePix), "Save");
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    QPixmap toolPix("resources/icons/full.bmp");
    QAction *toolAct = toolbar->addAction(QIcon(toolPix), "Full Stitch");
    connect(toolAct, SIGNAL(triggered()), this, SLOT(fullStitch()));

    QPixmap halfPix("resources/icons/half.bmp");
    QAction *halfAct = toolbar->addAction(QIcon(halfPix), "Half Stitch");
    connect(halfAct, SIGNAL(triggered()), this, SLOT(halfStitch()));

    QPixmap quarterPix("resources/icons/quarter.bmp");
    QAction *quarterAct = toolbar->addAction(QIcon(quarterPix), "Quarter Stitch");
    connect(quarterAct, SIGNAL(triggered()), this, SLOT(quarterStitch()));

    // m_model.m_toolState.m_flossIndex
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for (unsigned int i = 0; i < m_model.m_toolState.m_flossPalette.size(); ++i) {
        XSFloss &floss = m_model.m_toolState.m_flossPalette.GetFloss(i);
        XSColor c = floss.GetColor();
        QColor color(c.red, c.green, c.blue);
        QPixmap px(20, 20);
        px.fill(color);
        QAction *flossAct = toolbar->addAction(QIcon(px), "Floss");
        // connect(flossAct, SIGNAL(triggered()), this, SLOT(setFloss(i)));
        connect(flossAct, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(flossAct, i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setFloss(int)));

    // Drawing area
    m_area = new XSArea(m_model, m_controller, widget);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(m_area);

    QWidget *topFiller = scrollArea;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

#if 0
    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                    "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);
#endif

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    widget->setLayout(layout);

    createActions();
    createMenus();

    statusBar()->showMessage("Wildflower Cross Stitch " WFXS_VERSION);

    setWindowTitle(tr("Wildflower Cross Stitch"));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}

void MainWindow::newFile()
{
    m_controller.OnNew();

    MainWindow *window = new MainWindow;
    window->show();

// http://qt-project.org/doc/qt-4.8/painting-basicdrawing.html
}

void MainWindow::updateStitchMessage()
{
    const char *message;
    StitchType stitchType = m_controller.GetStitchType();

    switch (stitchType) {
    case Stitch_Full:
        message = "Full stitch";
        break;
    case Stitch_ThreeQuarterUL:
        message = "Three-quarter stitch (upper left)";
        break;
    case Stitch_ThreeQuarterUR:
        message = "Three-quarter stitch (upper right)";
        break;
    case Stitch_ThreeQuarterLL:
        message = "Three-quarter stitch (lower left)";
        break;
    case Stitch_ThreeQuarterLR:
        message = "Three-quarter stitch (lower right)";
        break;
    case Stitch_HalfBottom:
        message = "Half stitch (bottom)";
        break;
    case Stitch_HalfTop:
        message = "Half stitch (top)";
        break;
    case Stitch_QuarterUL:
        message = "Quarter stitch (upper left)";
        break;
    case Stitch_QuarterUR:
        message = "Quarter stitch (upper right)";
        break;
    case Stitch_QuarterLL:
        message = "Quarter stitch (lower left)";
        break;
    case Stitch_QuarterLR:
        message = "Quarter stitch (lower right)";
        break;
    case Stitch_ThreeQuarterAuto:
        message = "Three-quarter stitch (automatic orientation)";
        break;
    case Stitch_HalfAuto:
        message = "Half stitch (automatic orientation)";
        break;
    case Stitch_QuarterAuto:
        message = "Quarter stitch (automatic orientation)";
        break;
    default:
        return;
    }
    statusBar()->showMessage(message);
}

void MainWindow::fullStitch()
{
    m_controller.OnFullStitch();
    updateStitchMessage();
}

void MainWindow::halfStitch()
{
    // TODO toggle among
    m_controller.OnHalfAutoStitch();
    updateStitchMessage();
}

void MainWindow::halfAutoStitch()
{
    m_controller.OnHalfAutoStitch();
    updateStitchMessage();
}

void MainWindow::halfTopStitch()
{
    m_controller.OnHalfTopStitch();
    updateStitchMessage();
}

void MainWindow::halfBottomStitch()
{
    m_controller.OnHalfBottomStitch();
    updateStitchMessage();
}

void MainWindow::threeQuarterAutoStitch()
{
    m_controller.OnThreeQuarterAutoStitch();
    updateStitchMessage();
}

void MainWindow::threeQuarterULStitch()
{
    m_controller.OnThreeQuarterULStitch();
    updateStitchMessage();
}

void MainWindow::threeQuarterURStitch()
{
    m_controller.OnThreeQuarterURStitch();
    updateStitchMessage();
}

void MainWindow::threeQuarterLLStitch()
{
    m_controller.OnThreeQuarterLLStitch();
    updateStitchMessage();
}

void MainWindow::threeQuarterLRStitch()
{
    m_controller.OnThreeQuarterLRStitch();
    updateStitchMessage();
}

void MainWindow::quarterStitch()
{
    // TODO:  alternate
    m_controller.OnQuarterAutoStitch();
    updateStitchMessage();
}

void MainWindow::setFloss(int i)
{
    Log::debug(LOG_NAME, "setFloss(%d)", i);

    m_controller.setFloss(i);
}

void MainWindow::overwrite()
{
    Log::debug(LOG_NAME, "Overwrite");

    m_controller.OnOverwrite();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Pattern"),
            "",
            tr("All files (*.*)"));

    if (!fileName.isNull()) {
        try {
            m_controller.open(fileName.toUtf8().constData());
        } catch (std::exception &e) {
            Log::error(LOG_NAME, "Failed to open file: %s", e.what());
            QMessageBox msgBox;
            QString msg("The file could not be opened: ");
            msg += e.what();
            msgBox.setText(msg);
            msgBox.exec();
            return;
        }
        m_area->update();
    }
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Pattern"),
            "",
            "", 0,
            QFileDialog::HideNameFilterDetails);

    if (!fileName.isNull()) {
        try {
            m_controller.save(m_model, fileName.toUtf8().constData());
        } catch (std::exception &e) {
            Log::error(LOG_NAME, "Failed to save file: %s", e.what());
            QMessageBox msgBox;
            QString msg("The file could not be saved: ");
            msg += e.what();
            msgBox.setText(msg);
            msgBox.exec();
            return;
        }
    }
}

void MainWindow::properties()
{
    XSPropertiesDialog *propertiesDialog = new XSPropertiesDialog(this, m_model.GetProperties());

    propertiesDialog->show();
}

void MainWindow::print()
{
}

void MainWindow::undo()
{
}

void MainWindow::redo()
{
}

void MainWindow::cut()
{
}

void MainWindow::copy()
{
}

void MainWindow::paste()
{
}

void MainWindow::zoomIn()
{
    unsigned int zoom = m_model.GetZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom += increment;
    m_model.SetZoom(zoom);
    m_area->update();
}

void MainWindow::zoomOut()
{
    unsigned int zoom = m_model.GetZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom -= increment;
    m_model.SetZoom(zoom);
    m_area->update();
}

void MainWindow::leftAlign()
{
}

void MainWindow::rightAlign()
{
}

void MainWindow::justify()
{
}

void MainWindow::center()
{
}

void MainWindow::setLineSpacing()
{
}

void MainWindow::setParagraphSpacing()
{
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Wildflower Cross Stitch"),
            tr("<center>Wildflower Cross Stitch<br>...grows freely anywhere...<br>version " WFXS_VERSION "</center>"));
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    // TODO:  Open Recent

    // TODO import

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    // TODO: save as

    // TODO revert to saved
    //
    // TODO export
    //
    propertiesAct = new QAction(tr("Properties"), this);
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(properties()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    closeAct = new QAction(tr("C&lose"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close the pattern"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                    "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                    "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                    "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    fullStitchAct = new QAction(tr("Full Stitch"), this);
    connect(fullStitchAct, SIGNAL(triggered()), this, SLOT(fullStitch()));
    halfAutoStitchAct = new QAction(tr("Automatic"), this);
    connect(halfAutoStitchAct, SIGNAL(triggered()), this, SLOT(halfAutoStitch()));
    halfTopStitchAct = new QAction(tr("Top"), this);
    connect(halfTopStitchAct, SIGNAL(triggered()), this, SLOT(halfTopStitch()));
    halfBottomStitchAct = new QAction(tr("Bottom"), this);
    connect(halfBottomStitchAct, SIGNAL(triggered()), this, SLOT(halfBottomStitch()));
    threeQuarterAutoStitchAct = new QAction(tr("Automatic"), this);
    connect(threeQuarterAutoStitchAct, SIGNAL(triggered()), this, SLOT(threeQuarterAutoStitch()));
    threeQuarterULStitchAct = new QAction(tr("Upper Left"), this);
    connect(threeQuarterULStitchAct, SIGNAL(triggered()), this, SLOT(threeQuarterULStitch()));
    threeQuarterURStitchAct = new QAction(tr("Upper Right"), this);
    connect(threeQuarterURStitchAct, SIGNAL(triggered()), this, SLOT(threeQuarterURStitch()));
    threeQuarterLLStitchAct = new QAction(tr("Lower Left"), this);
    connect(threeQuarterLLStitchAct, SIGNAL(triggered()), this, SLOT(threeQuarterLLStitch()));
    threeQuarterLRStitchAct = new QAction(tr("Lower Right"), this);
    connect(threeQuarterLRStitchAct, SIGNAL(triggered()), this, SLOT(threeQuarterLRStitch()));
    overwriteAct = new QAction(tr("Overwrite"), this);
    overwriteAct->setCheckable(true);
    connect(overwriteAct, SIGNAL(triggered()), this, SLOT(overwrite()));

    zoomInAct = new QAction(tr("Zoom In"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    zoomOutAct = new QAction(tr("Zoom Out"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                    "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    // fileMenu->addAction(openRecentAct);
    fileMenu->addSeparator();
    // fileMenu->addAction(importAct);
    fileMenu->addSeparator();
    // fileMenu->addAction(closeAct);
    fileMenu->addAction(saveAct);
    // fileMenu->addAction(saveAsAct);
    // fileMenu->addAction(revertToSavedAct);
    // fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(propertiesAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    // editMenu->addAction(centerAct);
    // editMenu->addAction(centerHorizontallyAct);
    // editMenu->addAction(centerVerticallyAct);
    // editMenu->addAction(centerBothAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    // viewMenu->addAction(showGridAct);
    // viewMenu->addAction(showRulersAct);
    // viewMenu->addAction(styleAct);
    // viewMenu->addAction(stylePatternAct);
    // viewMenu->addAction(styleDesignAct);
    // viewMenu->addAction(styleRealisticAct);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(fullStitchAct);
    QMenu *halfStitchMenu = toolsMenu->addMenu(tr("Half Stitches"));
    halfStitchMenu->addAction(halfAutoStitchAct);
    halfStitchMenu->addAction(halfTopStitchAct);
    halfStitchMenu->addAction(halfBottomStitchAct);
    QMenu *threeQuarterMenu = toolsMenu->addMenu(tr("Three-quarter Stitches"));
    threeQuarterMenu->addAction(threeQuarterAutoStitchAct);
    threeQuarterMenu->addAction(threeQuarterULStitchAct);
    threeQuarterMenu->addAction(threeQuarterURStitchAct);
    threeQuarterMenu->addAction(threeQuarterLLStitchAct);
    threeQuarterMenu->addAction(threeQuarterLRStitchAct);
    // Beads
    // French Knots
    // Eraser
    //	small
    //	medium
    //	large
    // Flood fill
    // Color picker
    // Floss Palette
    //	Edit...
    //	Clear unused entries
    // Layers
    //	Add layer
    //	Delete layer
    //	Move layer up
    //	Move layer down
    toolsMenu->addAction(overwriteAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

#if 0
    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
#endif
}
