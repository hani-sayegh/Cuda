#include "mainwindow.h"
#include <visualizerwidget.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <vectorAddition.h>

void printArray(const float* a, const unsigned int n) {
    QString s = "(";
    unsigned int ii;
    for (ii = 0; ii < n - 1; ++ii)
        s.append(QString::number(a[ii])).append(", ");
    s.append(QString::number(a[ii])).append(")");


    qDebug() << s;
}

Mainwindow::Mainwindow ()
{
 //why has to be here and not in intializaGL?
 setMinimumSize(800, 800);

 QVBoxLayout * mainLayout;
 setLayout(mainLayout = new QVBoxLayout);
 QVBoxLayout * controlsLayout;
 mainLayout->addLayout(controlsLayout = new QVBoxLayout);
 QPushButton * runCudaFunction = new QPushButton;
 //Do we need tr function?
 runCudaFunction->setText("Run Algorithm");
 mainLayout->addWidget(runCudaFunction    );


 //Below no work
 // while(true);
 // v->show();
 // v->initializeGL();

 mainLayout->addWidget(this->v = new VisualizerWidget);

 //connect(runCudaFunction, SIGNAL(clicked(bool)), this, SLOT(testFunction()));
 connect(runCudaFunction, SIGNAL(clicked(bool)), this, SLOT(callMultiplication()));

}

void  Mainwindow::testFunction()
{
    unsigned int N = 50;
    float *a, *b, *c;

    a = new float [N];    b = new float [N];    c = new float [N];
    for (unsigned int ii = 0; ii < N; ++ii) {
        a[ii] = qrand();
        b[ii] = qrand();
    }

    // This is the function call in which the kernel is called
    vectorAddition(a, b, c, N);

    qDebug() << "input a:"; printArray(a, N);
    qDebug() << "input b:"; printArray(b, N);
    qDebug() << "output c:"; printArray(c, N);


}

void Mainwindow::callMultiplication()
{
    const int Width = 3;
       float M[Width*Width], N[Width*Width], P[Width*Width];
       for(int i = 0; i < (Width*Width) ; i++) {
           M[i] = 5;
           N[i] = 4;
           P[i] = 0;
       }
//       MatrixMultiplication
       MatrixMultiplication(M, N, P, Width);
       for(int i = 0; i < (Width*Width) ; i++) {
           printf("%f", P[i]);
           std::cout << std::endl;
       }
       std::cout << "Done Multiplying" << std::endl;

}
