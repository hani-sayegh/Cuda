//Cuda includes
#include <cuda.h>
#include <cuda_runtime.h>
#include <builtin_types.h>
//#include <drvapi_error_string.h>

//My includes
#include <gl_window.h>

//Qt includes
#include <QtCore/QCoreApplication>
//#include <QApplication>
#include <QDebug>

#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QMainWindow>

//Opengl includes
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

#include <mainwindow.h>


//**************
// Forward declare the function in the .cu file
void vectorAddition(const float* a, const float* b, float* c, int n);

//void printArray(const float* a, const unsigned int n) {
//    QString s = "(";
//    unsigned int ii;
//    for (ii = 0; ii < n - 1; ++ii)
//        s.append(QString::number(a[ii])).append(", ");
//    s.append(QString::number(a[ii])).append(")");


//    qDebug() << s;
//}


//
//int main(int argc, char* argv [])
//{
//    //QCoreApplication(argc, argv);
//    //QApplication app(argc, argv);
//    //return app.exec();
//
//
//
//    //CUDA CODE BELOW
//    int deviceCount = 0;
//    int cudaDevice = 0;
//    char cudaDeviceName [100];
//
//    unsigned int N = 50;
//    float *a, *b, *c;
//
//    cuInit(0);
//    cuDeviceGetCount(&deviceCount);
//    cuDeviceGet(&cudaDevice, 0);
//    cuDeviceGetName(cudaDeviceName, 100, cudaDevice);
//    qDebug() << "Number of devices: " << deviceCount;
//    qDebug() << "Device name:" << cudaDeviceName;
//
//    a = new float [N];    b = new float [N];    c = new float [N];
//    for (unsigned int ii = 0; ii < N; ++ii) {
//        a[ii] = qrand();
//        b[ii] = qrand();
//    }
//
//    // This is the function call in which the kernel is called
//    vectorAddition(a, b, c, N);
//
//    qDebug() << "input a:"; printArray(a, N);
//    qDebug() << "input b:"; printArray(b, N);
//    qDebug() << "output c:"; printArray(c, N);
//
//    if (a) delete a;
//    if (b) delete b;
//    if (c) delete c;
//}
//**************

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() override;
    void render() override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    //TriangleWindow window;

    ///////////////////////////////////////////

Mainwindow w;
w.show();


           ////////////////////////////////////////////

    //window.setFormat(format);
   // window.resize(640, 480);
   // window.show();

   // window.setAnimating(true);

    //CUDA CODE BELOW
    int deviceCount = 0;
    int cudaDevice = 0;
    char cudaDeviceName [100];

    unsigned int N = 50;
    float *a, *b, *c;

    cuInit(0);
    cuDeviceGetCount(&deviceCount);
    cuDeviceGet(&cudaDevice, 0);
    cuDeviceGetName(cudaDeviceName, 100, cudaDevice);
    qDebug() << "Number of devices: " << deviceCount;
    qDebug() << "Device name:" << cudaDeviceName;

    a = new float [N];    b = new float [N];    c = new float [N];
    for (unsigned int ii = 0; ii < N; ++ii) {
        a[ii] = qrand();
        b[ii] = qrand();
    }

    // This is the function call in which the kernel is called
    vectorAddition(a, b, c, N);

//    qDebug() << "input a:"; printArray(a, N);
//    qDebug() << "input b:"; printArray(b, N);
//    qDebug() << "output c:"; printArray(c, N);

    if (a) delete []a;
    if (b) delete []b;
    if (c) delete []c;

    return app.exec();
}



TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}



static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";
//! [3]

//! [4]
void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}
//! [4]

//! [5]
void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}
