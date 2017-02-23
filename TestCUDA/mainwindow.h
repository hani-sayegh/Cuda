#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <visualizerwidget.h>


class Mainwindow : public QWidget
{
    Q_OBJECT

public:
Mainwindow();
public slots:
void testFunction();
void callMultiplication();

private:
VisualizerWidget * v; //Should be mainwindow instead?
};

#endif // MAINWINDOW_H
