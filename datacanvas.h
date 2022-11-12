#ifndef DATACANVAS_H
#define DATACANVAS_H

#include <QMainWindow>
#include "setcolorpanel.h"
#include "mathopepanel.h"
#include "requires.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DataCanvas; }
QT_END_NAMESPACE

class DataCanvas : public QMainWindow
{
    Q_OBJECT

public:
    DataCanvas(QWidget *parent = nullptr);
    ~DataCanvas();
    void readDataToQCP(QString);
    QColor randomColor(int);
    QVector<QColor> randomColors(int);
    static double add(double,double);
    static double multiply(double,double);
    static double log(double,double);
private: // variables
    Ui::DataCanvas *ui;

    // QT UI
        // widget
        QCustomPlot* dataCanvas;
        SetColorPanel* setColorPanel;
        MathOpePanel* mathOpePanel;
        // btn
        QPushButton* clearBtn;
        QPushButton* setColorBtn;
        QPushButton* mathOpeBtn;
        QPushButton* setVisualityBtn;
        // form
        QComboBox* nameForm;
//        QPushButton *updateXInterval;

        // line edit
//        QLineEdit *xIntervalEdit;
//        QDoubleValidator *numValidator = new QDoubleValidator(0, 10000 , 5);

        // menu
        QMenu* fileMenu;
        QMenu* editMenu;

        // action
        QAction* openFile;
        QAction* saveImage;

    // num
//    unsigned int curColor = 0;
    unsigned int xSize = 0;
    unsigned int maxCnt = 0;
    double xInterval = 1;

    // vector
//    QVector<QColor>defaultColors{
//        QColor(255, 0, 0, 1),   // red
//        QColor(0, 255, 0, 1),   // green
//        QColor(0, 0, 255, 1),   // blue
//        QColor(255, 255, 0, 1), // yellow
//        QColor(255, 0, 255, 1), // purple
//        QColor(0, 255, 255, 1), // cyan
//        QColor(100,100,100,1), // gray
//        QColor(100,50,50,1), // brown
//        QColor(100,0,150,1), // dark purple
//        QColor(128,0,0,1), // crimson
//        QColor(255,255,255,1),  // black
//    };
private: // functions
private slots:
    void selectAndRead();
    void selectAndSaveImg();
    void clearPlots();
    void setColor();
    void mathOpe();
    void changeSetVisualityBtn(int);
    void setGraphVisuality();
    void receiveSetColorInfo(int, QColor);
    void receiveMathOpeInfo(int, int, QString, QString);
//    void updateXAxisInterval();
};
#endif // DATACANVAS_H
