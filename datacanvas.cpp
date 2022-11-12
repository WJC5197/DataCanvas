#include "datacanvas.h"
#include "setcolorpanel.h"
#include "./ui_datacanvas.h"

// DataCanvas Initialize function
DataCanvas::DataCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataCanvas)
{
    ui->setupUi(this);
    // map to ui
        // qcustomplot
        dataCanvas  = ui->Canvas;

        // menu
        fileMenu = ui->menuFile;
        editMenu = ui->menuEdit;

        // action
        openFile = ui->actionOpen;
        saveImage = ui->actionSaveImage;

        // button
        clearBtn = ui->clearBtn;
        setColorBtn = ui->setColorBtn;
        mathOpeBtn = ui->mathOpeBtn;
        setVisualityBtn = ui->setVisualityBtn;

        // Form
        nameForm = ui->nameForm;

    // connect
    connect(openFile, SIGNAL(triggered()), this, SLOT(selectAndRead()));
    connect(saveImage, SIGNAL(triggered()), this, SLOT(selectAndSaveImg()));
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearPlots()));
    connect(setColorBtn, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(mathOpeBtn, SIGNAL(clicked()), this, SLOT(mathOpe()));
    connect(setVisualityBtn, SIGNAL(clicked()), this, SLOT(setGraphVisuality()));
    connect(nameForm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSetVisualityBtn(int)));

    // init content config
    dataCanvas->xAxis2->setVisible(true);
    dataCanvas->xAxis2->setTickLabels(false);
    dataCanvas->yAxis2->setVisible(true);
    dataCanvas->yAxis2->setTickLabels(false);
    dataCanvas->legend->setVisible(true);
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    dataCanvas->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    dataCanvas->axisRect()->setRangeDrag(Qt::Vertical);
    dataCanvas->axisRect()->setRangeZoom(Qt::Vertical);
}

void DataCanvas::selectAndRead()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "/home",
                                                  tr("DataFile (*.txt *.csv *.json)"));
    readDataToQCP(fileName);
}

void DataCanvas::selectAndSaveImg()
{
    QString path = QFileDialog::getSaveFileName(0,
                                                tr("Save File"),
                                                ".");
    dataCanvas->savePng(path,dataCanvas->width(),dataCanvas->height(),1,-1);
}

void DataCanvas::clearPlots()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Warning");
    msgBox.setText("Do you want to delete current plots?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
          dataCanvas->clearGraphs();
          dataCanvas->replot();
          // Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void DataCanvas::setColor()
{
    QStringList nameList;
    unsigned int size = dataCanvas->graphCount();
    if(size==0){
        QMessageBox::information(0,"graphs' number is 0","Currently there is no graph");
        return; // null
    }
    for(int i=0;i<size;i++){
        nameList.append(dataCanvas->graph(i)->name());
    }
    setColorPanel = new SetColorPanel(this,nameList);
    connect(setColorPanel, SIGNAL(sendSetColorInfo(int,QColor)), this, SLOT(receiveSetColorInfo(int,QColor)));
    setColorPanel->setFixedSize(620,510);
    setColorPanel->exec();
    delete setColorPanel;
}

//void DataCanvas::updateXAxisInterval()
//{
//    xInterval = xIntervalEdit->text().toDouble();
//    unsigned int size = dataCanvas->graphCount();
//    for(int i=0;i<size;i++){
//        double key;
//        double value;
//        auto graphData = dataCanvas->graph(i)->data();
//        unsigned int dataSize = graphData->size();
//        for(int j=0;j<dataSize;j++){
//            key = graphData->at(j)->key;
//            value = graphData->at(j)->value;
//            graphData->remove(key);
//            graphData->add(QCPGraphData(j*xInterval,value));
//        }
//    }
//    dataCanvas->graph(maxCnt)->rescaleAxes();
//    dataCanvas->replot();
//}
void DataCanvas::mathOpe()
{
    unsigned int size = dataCanvas->graphCount();
    if(size==0){
        QMessageBox::information(0,"graphs' number is 0","Currently there is no graph");
        return; // null
    }
    QStringList nameList;
    for(int i=0;i<size;i++){
        nameList.append(dataCanvas->graph(i)->name());
    }
    mathOpePanel = new MathOpePanel(this,nameList);
    connect(mathOpePanel, SIGNAL(sendMathOpeInfo(int,int,QString,QString)), this, SLOT(receiveMathOpeInfo(int,int,QString,QString)));
    mathOpePanel->setFixedSize(540,120);
    mathOpePanel->exec();
    delete mathOpePanel;
}

void DataCanvas::changeSetVisualityBtn(int idx)
{
    if(idx<0) return;
    if(dataCanvas->graph(idx)->visible()) setVisualityBtn->setText("Conceal");
    else setVisualityBtn->setText("Reveal");
    setVisualityBtn->repaint();
}

void DataCanvas::setGraphVisuality()
{
    int idx = nameForm->currentIndex();
    if(idx<0) return;
    if(dataCanvas->graph(idx)->visible()) {
        dataCanvas->graph(idx)->setVisible(false);
        setVisualityBtn->setText("Reveal");
    }
    else {
        dataCanvas->graph(idx)->setVisible(true);
        setVisualityBtn->setText("Conceal");
    }
    dataCanvas->replot();
    setVisualityBtn->repaint();
}

void DataCanvas::receiveSetColorInfo(int idx, QColor color)
{
    dataCanvas->graph(idx)->setPen(QPen(color));
    dataCanvas->replot();
}

void DataCanvas::receiveMathOpeInfo(int idxFst, int idxSnd, QString newPlotName, QString func)
{
    std::function<double(double,double)>opeFunc;
    if(func == "add") opeFunc = DataCanvas::add;
    else if(func == "multiply") opeFunc = DataCanvas::multiply;
    else if(func == "log") opeFunc = DataCanvas::log;
    else {
        QMessageBox::information(0,"input function error","not support "+func+" function");
        return; // null
    }
    unsigned int fstSize = dataCanvas->graph(idxFst)->dataCount();
    unsigned int sndSize = dataCanvas->graph(idxSnd)->dataCount();
    unsigned int size = fstSize<=sndSize?fstSize:sndSize;
    bool isFst = fstSize<=sndSize?true:false;
    dataCanvas->addGraph();
    nameForm->addItem(newPlotName);

    auto graphDataFst = dataCanvas->graph(idxFst)->data();
    auto graphDataSnd = dataCanvas->graph(idxSnd)->data();
    auto graphData = dataCanvas->graph(dataCanvas->graphCount()-1)->data();
    double key;
    for(int i=0;i<size;i++){
        if(isFst) key = graphDataFst->at(i)->key;
        else key = graphDataSnd->at(i)->key;
        graphData->add(QCPGraphData(key,opeFunc(graphDataFst->at(i)->value,graphDataSnd->at(i)->value)));
    }
    dataCanvas->graph(dataCanvas->graphCount()-1)->setName(newPlotName);
    dataCanvas->graph(dataCanvas->graphCount()-1)->setPen(QPen(randomColor(dataCanvas->graphCount()-1)));
    dataCanvas->replot();
}

void DataCanvas::readDataToQCP(QString fileName)
{
    if(fileName == "") return;
    dataCanvas->clearGraphs();
    nameForm->clear();
    // block global variables

    QFile file(fileName);
    QByteArray line;
    QString plotName;
    QVector<QVector<double>>plotsData;
    QVector<double>tmpData;
    unsigned int size;
    unsigned int plotCnt = 0;

    // file name split
    QStringList pieces = fileName.split(".");
    if(pieces.length()!=2){ // error data file name format
        QMessageBox::information(0,"Read dataFile Error","File name format isn't \"fileName.suffixName\".");
        return; // null
    }
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"Read dataFile Error",file.errorString());
        return; // null
    }
    auto suffixName = pieces[1]; // format:1.txt, 2.csv, 3.json, etc

    if(suffixName=="txt"){
        // if variables
        QRegExp match("<PlotName:(.+),Separator:(space|comma|tab)>"); // regex match Name(can't be empty) & Separator
        QString separatorName;
        char splitChar;
        bool isMatchedPlotName = false;
        while (!file.atEnd()) {
            line = file.readLine();
            if(line[0]=='<' && !isMatchedPlotName){
                line = line.simplified();
                line.replace( " ", "" ); // delete whitespace
                int pos = match.indexIn(line);
                if (pos > -1) {
                    isMatchedPlotName = true;
                    plotName = match.cap(1); // plotName
                    separatorName = match.cap(2);  // Separator
                    if(separatorName == "space") splitChar = ' ';
                    else if(separatorName == "comma") splitChar = ',';
                    else if(separatorName == "tab") splitChar = '\t';
                }
                else{ // miss match
                    QMessageBox::information(0,"dataFile Format Error","Can't match PlotName or Separator");
                    return; // null
                }
            }
            else if(line!="\r\n" && isMatchedPlotName){
                auto pieces = line.split(splitChar);
                size = pieces.size();
                for(int i=0;i<size;i++){
                    try{
                        tmpData.push_back((pieces.at(i)).toDouble());
                    }catch(std::exception e){
                        QMessageBox::information(0,"dataFile Format Error","Can't parse str to double due to the separator.");
                        return;
                    }
                }
            }
            else if(line=="\r\n" && isMatchedPlotName){ // match over data
                isMatchedPlotName = false;
                dataCanvas->addGraph();
                dataCanvas->graph(plotCnt)->setName(plotName);
                nameForm->addItem(plotName);
                plotsData.push_back(tmpData);
                size = tmpData.size();
                if(xSize < size) {
                    xSize = size;
                    maxCnt = plotCnt;
                }
                plotCnt++;
                tmpData.clear();
            }
        }
    }
    else if(suffixName=="csv"){

    }
    else if(suffixName=="json"){

    }
    else{
        QMessageBox::information(0,"dataFile suffix name error","fileName should be csv, json or txt.");
        return;
    }
    // xAxisVal initialize
    size = plotsData.size();
    auto colors = randomColors(size);
    for(int i=0;i<size;i++){
        dataCanvas->graph(i)->setPen(QPen(colors[i]));
//        dataCanvas->graph(i)->setData(xAxisVal,plotsData[i]);
        auto graphData = dataCanvas->graph(i)->data();
        unsigned int dataSize = plotsData[i].size();
        for(int j=0;j<dataSize;j++){
            graphData->add(QCPGraphData(j*xInterval,plotsData[i][j]));
        }
    }
    dataCanvas->graph(maxCnt)->rescaleAxes();
    dataCanvas->replot();
}

QVector<QColor> DataCanvas::randomColors(int count){
    QVector<QColor> colors;
    float currentHue = 0.0;
    for (int i = 0; i < count; i++){
        colors.push_back( QColor::fromHslF(currentHue, 1.0, 0.5) );
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    return colors;
}

QColor DataCanvas::randomColor(int idx){
    float currentHue = 0.0;
    for (int i = 0; i < idx; i++){
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    return QColor::fromHslF(currentHue, 1.0, 0.5);
}


double DataCanvas::add(double x,double y)
{
    return x+y;
}

double DataCanvas::multiply(double x,double y)
{
    return x*y;
}

double DataCanvas::log(double x,double y)
{
    return ::log(x+y);
}
DataCanvas::~DataCanvas()
{
    delete ui;
}

