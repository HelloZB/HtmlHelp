#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWebKit>
#include <QWebView>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void zoomDec();
    void zoomInc();
    void zoomNorm();
    void slotFinished(bool bOk);
    void startLoad();
    void zoomChanged();
    void newUrl(QUrl url);

    void loadSettings();
    void saveSettings();

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QWebView *myWeb;
    QTimer timerDec;
    QTimer timerInc;
    QString filePath;

};

#endif // WIDGET_H
