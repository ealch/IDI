#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include "glm/glm.hpp"


//Gestiona la definición, compilación y vinculación de los shaders
#include <QGLShader>
//Permite agrupar los shaders diseñados para que funcionen conjuntamente
#include <QGLShaderProgram>

//Keyboard Events
#include <QKeyEvent>

#include "glm/gtc/matrix_transform.hpp"

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
    void getUniforms();
    
    void modelTransform();
    //void modelTransform1();
    //void modelTransform2();
    
    //Cuadrado a partir de dos triangulos
    GLuint VAO_TRIANGULO1, VAO_TRIANGULO2;
    //Casa con triangle-strip
    GLuint VAO_CASA, VBO, VBO_color;
    
    //Sesión 2
    //Se encarga de cargar los shaders: vertshad.vert y fragshad.frag
    
    QGLShaderProgram *program;
    int pos, pos1, pos2, col;
    
    //Sesión 3: uniform location
    GLint varLoc;
    GLuint transLoc;
    float scl, transX, transY, rot;
    
    glm::mat4 TG;
    glm::vec3 transV;
    glm::vec3 sclV;

    
};
