#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model/model.h"
#include <cmath>


//Gestiona la definición, compilación y vinculación de los shaders
#include <QGLShader>
//Permite agrupar los shaders diseñados para que funcionen conjuntamente
#include <QGLShaderProgram>

//Keyboard Events
#include <QKeyEvent>


class MyGLWidget : public QGLWidget 
{
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);
  
  protected:
    // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resizeGL() - Es cridat quan canvi la mida del widget
    virtual void resizeGL (int width, int height);
    
    virtual void keyPressEvent(QKeyEvent *e);

  private:
    void createBuffers();
    //Sesión 2
    void loadShaders();
    //Sesión 3
    void initVariables();
    
    //void modelTransform();
    
    void modelTransform1();
    void modelTransform2();
    void modelTransform3();
    
    
    GLuint VAO_Ejes, VBO_EjesPos, VBO_EjesColor;
    GLuint VAO_model1, VBO_modelPos1, VBO_modelCol1;
    GLuint VAO_model2, VBO_modelPos2, VBO_modelCol2;
    
    //Sesión 2
    //Se encarga de cargar los shaders: vertshad.vert y fragshad.frag
    
    QGLShaderProgram *program;
    int pos, col;
    
    //Sesión 3: uniform location
    GLint varLoc;
    GLuint transLoc;
    float scl, transX, transY, rot;
    
    glm::mat4 TG;
    glm::vec3 transV;
    glm::vec3 sclV;
    
    
    //Sesión 3bis
    Model m1;
    Model m2;
    
    //Model vectorModels[3];
    //std::vector<Model> models;
};
