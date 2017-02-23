#ifndef VISUALIZERWIDGET_H
#define VISUALIZERWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QVector3D>
#include <string>
#include <fstream>
#include <iostream>



inline char * concat(const char * str1, const char * str2)
{
 char * r = (char *)malloc(strlen(str1) + strlen(str2) + 1);
 strcpy(r, str1);
 strcat(r, str2);
 //free((void*)str1); //No need since these are const variables?
 //free((void*)str2);
 return r;
}

inline std::string readShaderCode(const  char* fileName)
{
 const char * systemPath; //Why has to be const?
#ifdef _WIN32
 systemPath = "C:/Users/hani/Desktop/qtprojects/visualizer/";
#elif __linux__
  systemPath = "../opengl/";
#endif

  fileName = concat(systemPath, fileName);
 std::ifstream input(fileName);
 if(!input.good())
 {
  std::cout << "failed to load file: " << fileName << std::endl;
  exit(1);
 }
 //Need to free fileName
 /* delete systemPath; */
 return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

class VisualizerWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 public:
  VisualizerWidget() = default; //Why need default?
  VisualizerWidget(QWidget *parent) : QOpenGLWidget(parent) { }

 private:
  GLfloat red = 0;
  GLfloat green = 0;
  GLfloat blue = 0;
  QVector3D color;

 public:


  void initializeGL()
  {
   constexpr GLfloat vertices []=
   {
    -1, -1,
    1, 0, 0,
    0, 1,
    0, 1, 0,
    1, -1,
    0, 0, 1,
   };

   initializeOpenGLFunctions();
   color.setX(red);
   color.setY(green);
   color.setZ(blue);

   // Set up the rendering context, load shaders and other resources, etc.:
   //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
   glEnable(GL_DEPTH_TEST);
   //send data to opengl
   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

   GLushort indicies [] =
   {
    0, 1, 2
   };

   GLuint vertexBufferId;
   //generate a buffer
   glGenBuffers(1, &vertexBufferId);
   //bind buffer to the gl_array_buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
   //send the data WE have to the gpu
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   //enable the position attribute of our vertex
   //vertices can have many attributes such as position color and normals for example
   glEnableVertexAttribArray(0);

   //tell opengl what our data is, it doesn't know that we sent floats or not for example
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (char*)(sizeof(GL_FLOAT) * 2));

   GLuint indexBufferId;
   glGenBuffers(1, &indexBufferId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

   /////////////Shader Code Below////////////////
   //First we create the shader objects
   GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
   GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

   const char * adapter[1];

   auto temp = readShaderCode("vertex_shader.vert");

   adapter[0] = temp.c_str();
   //then we tell the shader objects where they can find the source code
   glShaderSource(vShader, 1, adapter, 0);
   temp = readShaderCode("frag.frag");
   adapter[0] = temp.c_str();
   glShaderSource(fShader, 1, adapter, 0);

   //Then we compile the shaders
   glCompileShader(vShader);
   glCompileShader(fShader);

   GLint compileStatus; //This is known as an out parameter
   //This function returns you information you want depending on the enum you pass
   glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileStatus);
   if(compileStatus != GL_TRUE)
   {
       GLint infoLogLength;
       glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infoLogLength);

       GLchar * errorMessage = new GLchar[infoLogLength]; //Difference here is you don't need to multiply
       //by sizeof????

       GLsizei bufferSize;
glGetShaderInfoLog(vShader, infoLogLength, &bufferSize, errorMessage );

std::cout << "Problem with shader code:" << std::endl;
std::cout << errorMessage << std::endl;

       delete [] errorMessage;
exit(1);
   }

   //Then we create a shader program
   GLuint program = glCreateProgram();
   //then we attached compiled shaders to this program
   glAttachShader(program, vShader);
   glAttachShader(program, fShader);

   //then we link the shader
   glLinkProgram(program);

   //finally we use the porgram
   glUseProgram(program);
  }

  void resizeGL(int w, int h)
  {
   // Update projection matrix and other size related settings:
   //    m_projection.setToIdentity();
   //    m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
  }

  void paintGL()
  {

   initializeOpenGLFunctions();
   glClear(GL_DEPTH_BUFFER_BIT);
   // Draw the scene:
   //initializeOpenGLFunctions();
   // glClear(GL_COLOR_BUFFER_BIT);
   //glDrawArrays(GL_TRIANGLES,0, 6);
   glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_SHORT, 0);
  }

};
#endif // VISUALIZERWIDGET_H
