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
    
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    
    void modelTransformTerra ();
    
    //último ejercicio
    void modelTransformModel1 ();
    void modelTransformModel2 ();
    //
    
    void projectTransform ();
    void viewTransform ();
    
    void calcularCajaContenedora();
    void calcularEsferaMinimaContenedora();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_model1, VBO_modelPos1, VBO_modelCol1;
    GLuint VAO_model2, VBO_modelPos2, VBO_modelCol2;
    //GLuint VAO_Casa, VBO_CasaPos, VBO_CasaCol;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;
    // Program
    QGLShaderProgram *program;
    
    // Internal vars
    float scale;
    glm::vec3 posterra[4];
    glm::vec3 colterra[4];
  
    Model m;
    Model m1;
    Model m2;
    
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
    
    //Sesión 5
    GLfloat phi; 	//φ
    GLfloat theta; 	//θ
    GLfloat psi; 	//ψ
    
    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    float zoom;
    
    
    //Último ejercicio
    GLfloat sclPatricios;
    
    //Ortho
    void projectTransformPerspective ();
    void projectTransformOrtho ();
    typedef enum {PERSPECTIVE, ORTHO} Camera;
    Camera cam;
    
    
};

