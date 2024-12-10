#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circulargauge.h"
#include "emafilter.h"
#include <QMainWindow>
#include <QCanBusFrame>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QProgressBar *battery;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    double calculateSpeed(double rpm);

public slots:
    void updateSpeedAnimation(const QCanBusFrame &frame);

private:
    Ui::MainWindow *ui;
    CircularGauge *gauge;
    EmaFilter *emaFilter;

    void setWidgets();
    void setScreenOptions();
    void setCanBus(const QString &interfaceName);
};

#endif
