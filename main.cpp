#include "requires.h"
#include "datacanvas.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataCanvas w;
    /* QT test */
//    QRegExp regExp("<PlotName:(.+),Separator:(space|comma|tab)>");
//    int pos = regExp.indexIn("<PlotName:wjc,Separator:tab>");
//    qDebug() << pos << regExp.cap(1) << regExp.cap(2);
    /* read test */
    //w.readDataToQCP("D:\\OneDrive\\Project\\QT\\DataCanvas\\Data\\separator_text_data.txt");
    w.show();
    return a.exec();
}
