#include "widget.h"
#include "ui_widget.h"
#include <QTextCodec> //qt4
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileInfo>
//#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8"); //qt4
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);

    filePath =qApp->applicationDirPath();

    myWeb = new QWebView;

    timerDec.setInterval(350);
    timerInc.setInterval(350);

    ui->vltWeb->addWidget(myWeb);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,3);

    connect(ui->pbBack, SIGNAL(clicked()), myWeb, SLOT(back()));
    connect(ui->pbForward, SIGNAL(clicked()), myWeb,SLOT(forward()));

    connect(ui->pbFontDec, SIGNAL(clicked()), this, SLOT(zoomDec()));
    connect(ui->pbFontInc, SIGNAL(clicked()), this, SLOT(zoomInc()));
    connect(ui->pbFontNorm, SIGNAL(clicked()), this, SLOT(zoomNorm()));

    connect(ui->pbFontDec, SIGNAL(pressed()), &timerDec, SLOT(start()));
    connect(ui->pbFontInc, SIGNAL(pressed()), &timerInc, SLOT(start()));

    connect(ui->pbFontDec,SIGNAL(released()), &timerDec, SLOT(stop()));
    connect(ui->pbFontInc,SIGNAL(released()), &timerInc, SLOT(stop()));

    connect(&timerDec,SIGNAL(timeout()), this, SLOT(zoomDec()));
    connect(&timerInc, SIGNAL(timeout()), this, SLOT(zoomInc()));

    connect(myWeb, SIGNAL(loadProgress(int)), ui->pbrLoad , SLOT(setValue(int)));
    connect(myWeb, SIGNAL(loadFinished(bool)), SLOT(slotFinished(bool)));
    connect(myWeb, SIGNAL(loadStarted()), this, SLOT(startLoad()));
//    connect(myWeb, SIGNAL(linkClicked(QUrl)), this, SLOT(newUrl(QUrl)));
//tmp windows gmn    connect(myWeb, SIGNAL(urlChanged(QUrl)), SLOT(newUrl(QUrl)));

   // myWeb->load(QUrl("http://img0.liveinternet.ru/images/attach/c/5//3970/3970473_sprite198.swf"));//http://img0.liveinternet.ru/images/attach/c/5//3970/3970473_sprite198.swf
    myWeb->load(QUrl(filePath+"/html/sniffer_index.html"));

//    qDebug()<<filePath;

//    connect(myWeb,SIGNAL(titleChanged(QString)), ui->widget, SLOT(setWindowTitle(QString)));
//    myWeb->load(QUrl(filePath+"/html2/ddd.html"));
    ui->treeWidget->setSortingEnabled(0);


    QTreeWidgetItem *myItem = new QTreeWidgetItem(ui->treeWidget);
    myItem->setText(0, "EMBEE Radio Sniffer");//zmist

    QTreeWidgetItem *vstup = new QTreeWidgetItem(myItem);
    vstup->setText(0, "Введение");


    QTreeWidgetItem *vykor = new QTreeWidgetItem(myItem);
    vykor->setText(0, "Использование");

    QTreeWidgetItem *vykorNast = new QTreeWidgetItem(vykor);
    vykorNast->setText(0, "Настройка соединения");

    QTreeWidgetItem *vykorNtty = new QTreeWidgetItem(vykorNast);
    vykorNtty->setText(0, "Настройка последовательного порта");

    QTreeWidgetItem *vykorNhayes = new QTreeWidgetItem(vykorNast);
    vykorNhayes->setText(0, "Настройка соединения через Hayes-модем");

    QTreeWidgetItem *vykorNeth = new QTreeWidgetItem(vykorNast);
    vykorNeth->setText(0, "Настройка соединения через Ethernet");

    QTreeWidgetItem *vykorParam = new QTreeWidgetItem(vykor);
    vykorParam->setText(0, "Конфигурация основных параметров");

    QTreeWidgetItem *vykorParamO = new QTreeWidgetItem(vykorParam);
    vykorParamO->setText(0, "Описание параметров");

    QTreeWidgetItem *vykorParamR = new QTreeWidgetItem(vykorParam);
    vykorParamR->setText(0, "Режимы");

    QTreeWidgetItem *vykorTerm = new QTreeWidgetItem(vykor);
    vykorTerm->setText(0, "Встроенный терминал");

    QTreeWidgetItem *vykorCom = new QTreeWidgetItem(myItem);
    vykorCom->setText(0, "Command Reference");

    QTreeWidgetItem *vykorComF = new QTreeWidgetItem(vykorCom);
    vykorComF->setText(0, "The File Menu");

    QTreeWidgetItem *vykorComV = new QTreeWidgetItem(vykorCom);
    vykorComV->setText(0, "The View Menu");

    QTreeWidgetItem *vykorComA = new QTreeWidgetItem(vykorCom);
    vykorComA->setText(0, "The Advanced Menu");

    QTreeWidgetItem *vykorComSw = new QTreeWidgetItem(vykorCom);
    vykorComSw->setText(0, "The Switch Menu");

    QTreeWidgetItem *vykorComSet = new QTreeWidgetItem(vykorCom);
    vykorComSet->setText(0, "The Settings Menu");

    QTreeWidgetItem *vykorComH = new QTreeWidgetItem(vykorCom);
    vykorComH->setText(0, "The Help Menu");

    ui->treeWidget->setItemExpanded(myItem, true);
    ui->treeWidget->setItemExpanded(vykor, true);
    ui->treeWidget->setItemExpanded(vykorNast, true);
    ui->treeWidget->setItemExpanded(vykorParam, true);
    ui->treeWidget->setItemExpanded(vykorCom, true);

    ui->pbFontNorm->setToolTip(tr("x1"));
    QTimer::singleShot(50, this, SLOT(zoomChanged()));
  /*  QTreeWidgetItem *vykor = new QTreeWidgetItem(myItem);
    vykor->setText(0, "Использование");*/
    loadSettings();

}
//===================================================================================
Widget::~Widget()
{
    saveSettings();
    delete ui;
}
//===================================================================================
void Widget::zoomDec()
{
    myWeb->setZoomFactor(myWeb->zoomFactor()-0.05);
    zoomChanged();
}
//===================================================================================
void Widget::zoomInc()
{
    myWeb->setZoomFactor(myWeb->zoomFactor()+0.05);
    zoomChanged();
}
//=================================================================================
void Widget::zoomChanged()
{
    ui->pbFontDec->setToolTip(tr("x%1").arg(myWeb->zoomFactor()-0.05));
    ui->pbFontInc->setToolTip(tr("x%1").arg(myWeb->zoomFactor()+0.05));
}

//===================================================================================
void Widget::zoomNorm()
{
    myWeb->setZoomFactor(1);
}
//===================================================================================
void Widget::newUrl(QUrl url)
{
//    qDebug()<<url.toLocalFile();
    QFileInfo fi(url.toLocalFile());
//    qDebug()<<fi.fileName();
    if(fi.fileName()=="sniffer_emb.html"){
         myWeb->load(QUrl(filePath+"/html/sniffer_index.html"));
         QDesktopServices::openUrl(QUrl("http://embee.ua/"));
    }
}

//===================================================================================
void Widget::slotFinished(bool bOk)
{
//    QFile file(myWeb->url().toString().right(myWeb->url().toString().size()-7));
//    qDebug()<<myWeb->url().toString().right(myWeb->url().toString().size()-7);
//    if(file.open(QIODevice::ReadOnly)){
//        ui->plainTextEdit->appendHtml(file.readAll()); //"file:///home/hello_zb/My_Prog/Qt5_progs/HtmlHelp/build-untitled-Qt_4_8_1_in_PATH_System-Debug/html/sniffer_index.html");//myWeb->url().toString());
//        qDebug()<<myWeb->url();
//        file.close();
//    }

    if (!bOk) {
        myWeb->setHtml("<CENTER>An error has occured"
                       " while loading the web page</CENTER>"
                      );
    }

    ui->pbBack->setEnabled(myWeb->page()->history()->canGoBack());
    ui->pbForward->setEnabled(myWeb->page()->history()->canGoForward());
    ui->pbrLoad->hide();
    ui->widget->window()->setWindowTitle(myWeb->title());
}

//===================================================================================
void Widget::startLoad()
{
//    qDebug()<<myWeb->url();
    ui->pbrLoad->setValue(0);
    ui->pbrLoad->setHidden(0);
}
//===================================================================================

void Widget::loadSettings()
{
    QSettings settings("Hello_ZB","assistant");
    settings.beginGroup("Qt_RSniffer");
    QRect rect=settings.value("position").toRect();
    qint32 desktopW= qApp->desktop()->width();
    qint32 desktopH=qApp->desktop()->height();

    if(rect.x()< 10)
        rect.setX(10);
    if(rect.y()<30)
        rect.setY(31);
    if(rect.x()>(desktopW)  || rect.y()>(desktopH)) {
        rect.setX(10);
        rect.setY(31);
        //         rect.setWidth(670);
        //         rect.setHeight(500);
    }
    setGeometry(rect);
    ui->splitter->restoreState(settings.value("rozmir").toByteArray());
    myWeb->setZoomFactor(settings.value("zoom",1).toDouble());
    settings.endGroup();
}
//==================================================================

void Widget::saveSettings()
{
    QSettings settings("Hello_ZB","assistant"); //");
    settings.beginGroup("Qt_RSniffer");
    settings.setValue("position", this->geometry());
    settings.setValue("rozmir", ui->splitter->saveState());
    settings.setValue("zoom", myWeb->zoomFactor());
    settings.endGroup();
}
//==================================================================

void Widget::on_treeWidget_clicked(const QModelIndex &index)
{
    QList<int> indexy;
    indexy.append(index.row());

    for(QModelIndex parentIndex=index.parent(); parentIndex.row() >=0; parentIndex=parentIndex.parent()){
        indexy.prepend(parentIndex.row());
    }

    while (indexy.size()<4) {
        indexy.append(-1);
    }

    switch(indexy.at(1)){
    case 0: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter1.html")); break;}
    case 1: {
        switch(indexy.at(2)){
        case 0: {
            switch(indexy.at(3)){
            case 0: {    myWeb->load(QUrl(filePath+"/html/sniffer_chapter2tty.html#tty_set")); break;}
            case 1: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2tty.html#hayes_set")); break;}
            case 2: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2tty.html#ethy_set")); break;}
            default: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2tty.html#connection-section")); break;}
            }
            break;}
        case 1: {
            switch(indexy.at(3)){
            case 0: {    myWeb->load(QUrl(filePath+"/html/sniffer_chapter2param.html#param")); break;}
            case 1: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2param.html#modes")); break;}
            default: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2param.html")); break;}
            }
            break;}
        case 2: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2term.html")); break;}
        default: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter2tty.html")); break;}

        }

        break;}

    case 2: {
        switch(indexy.at(2)){
        case 0: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3file.html#file")); break;}
        case 1: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3view.html")); break;}
        case 2: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3adv.html")); break;}
        case 3: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3switch.html")); break;}
        case 4: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3sett.html")); break;}
        case 5: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3help.html")); break;}
        default: { myWeb->load(QUrl(filePath+"/html/sniffer_chapter3file.html")); break;}
        }

        break;}
    default: { myWeb->load(QUrl(filePath+"/html/sniffer_index.html")); break;}
    }
}
