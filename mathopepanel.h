#ifndef MATHOPEPANEL_H
#define MATHOPEPANEL_H

#include <QDialog>
#include "requires.h"

namespace Ui {
class MathOpePanel;
}

class MathOpePanel : public QDialog
{
    Q_OBJECT

public:
    explicit MathOpePanel(QWidget *parent = nullptr, QStringList graphNames = {});
    ~MathOpePanel();

private:
    Ui::MathOpePanel *ui;

    // QT UI
        // form
        QComboBox* nameFormFst;
        QComboBox* nameFormSnd;

        // btn
        QPushButton* yesBtn;
        QPushButton* cancelBtn;

        // edit
        QLineEdit* mathOpeEdit;
        QLineEdit* newPlotNameEdit;
signals:
    void sendMathOpeInfo(int,int,QString,QString);
private slots:
    void yesExit();
    void cancelExit();
};

#endif // MATHOPEPANEL_H
