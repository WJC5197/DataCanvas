#include "mathopepanel.h"
#include "ui_mathopepanel.h"

MathOpePanel::MathOpePanel(QWidget *parent, QStringList graphNames) :
    QDialog(parent),
    ui(new Ui::MathOpePanel)
{
    // map to ui
    ui->setupUi(this);
    nameFormFst = ui->nameFormFst;
    nameFormSnd = ui->nameFormSnd;
    mathOpeEdit = ui->mathOpeEdit;
    newPlotNameEdit = ui->newPlotNameEdit;
    yesBtn = ui->yesBtn;
    cancelBtn = ui->cancelBtn;

    // connect
    connect(yesBtn, SIGNAL(clicked()), this, SLOT(yesExit()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelExit()));

    // init content config
    nameFormFst->addItems(graphNames);
    nameFormSnd->addItems(graphNames);
}

void MathOpePanel::yesExit()
{
    QString func = mathOpeEdit->text();
    QString newPlotName = newPlotNameEdit->text();
    if(func=="" || newPlotName=="") return;
    emit sendMathOpeInfo(nameFormFst->currentIndex(),nameFormSnd->currentIndex(),newPlotName,func);
    this->close();
}

void MathOpePanel::cancelExit()
{
    this->close();
}

MathOpePanel::~MathOpePanel()
{
    delete ui;
}
