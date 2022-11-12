#ifndef SETCOLORPANEL_H
#define SETCOLORPANEL_H

#include <QDialog>
#include "requires.h"

namespace Ui {
class SetColorPanel;
}

class SetColorPanel : public QDialog
{
    Q_OBJECT

public:
    explicit SetColorPanel(QWidget *parent = nullptr, QStringList graphNames = {});
    ~SetColorPanel();

private:
    Ui::SetColorPanel *ui;

    // QT UI
        // Widget
        QColorDialog* colorWidget;
        QComboBox* nameForm;

        // btn
        QPushButton* yesBtn;
        QPushButton* cancelBtn;
    // QStringList
    QStringList graphNames;
signals:
    void sendSetColorInfo(int,QColor);
private slots:
    void yesExit();
    void cancelExit();

};

#endif // SETCOLORPANEL_H
