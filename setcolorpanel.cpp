#include "setcolorpanel.h"
#include "ui_setcolorpanel.h"

SetColorPanel::SetColorPanel(QWidget *parent, QStringList graphNames) :
    QDialog(parent),
    ui(new Ui::SetColorPanel)
{
    ui->setupUi(this);
    // map to ui
    colorWidget = ui->colorWidget;
    nameForm = ui->nameForm;
    yesBtn = ui->yesBtn;
    cancelBtn = ui->cancelBtn;
    // connect
    connect(yesBtn, SIGNAL(clicked()), this, SLOT(yesExit()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelExit()));
    // init content config
    colorWidget->setWindowFlags(Qt::Widget);
    /* a few options that we must set for it to work nicely */
    colorWidget->setOptions(
                /* do not use native dialog */
                QColorDialog::DontUseNativeDialog |
                QColorDialog::NoButtons
    );
    this->graphNames = graphNames;
    nameForm->addItems(graphNames);
}

void SetColorPanel::yesExit()
{
    QString curName = nameForm->currentText();
    QColor color = colorWidget->currentColor();
    unsigned int size = graphNames.size();
    for(int i=0;i<size;i++){
        if(graphNames.at(i)==curName){
            emit sendSetColorInfo(i,color);
            break;
        }
    }
    this->close();
}

void SetColorPanel::cancelExit()
{
    this->close();
}
SetColorPanel::~SetColorPanel()
{
    delete ui;
}
