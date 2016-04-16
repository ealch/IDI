#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath> 
#include "model.h"

class MyGLWidget : public QGLWidget {
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void projectTransform ();
    void viewTransform ();
    
    void calcularCajaContenedora();
    void calcularEsferaMinimaContenedora();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_model, VBO_modelPos, VBO_modelCol;
    //GLuint VAO_Casa, VBO_CasaPos, VBO_CasaCol;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;
    // Program
    QGLShaderProgram *program;
    
    // Internal vars
    float scale;
    glm::vec3 posterra[4];
    glm::vec3 colterra[4];
  
    Model m;
    
    //Parámetros de visión
    void initParametros();
    void calcularParametros();
    
      //projectTransform
      GLfloat FOV, FOVinicial;
      GLfloat ra ;
      GLfloat zNear, zFar;
      GLfloat d;
      
      //viewTransform
      glm::vec3 OBS;
      glm::vec3 VRP;
      glm::vec3 up;
      
    
    //Caja contenedora
    struct CajaContenedora {
      glm::vec3 puntoMax;
      glm::vec3 puntoMin;
    } cajaContenedora;
    
    
    struct Esfera {
      glm::vec3 centro;
      GLfloat radio;
    } esfera;
};

