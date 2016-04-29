#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "model.h"

class MyGLWidget : public QGLWidget {
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);
  
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resizeGL - Es cridat quan canvi la mida del widget
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
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void calculaCapsaModel ();
    
    void modelTransformCow();
    
    
    void iluminacion();

    // VAO i VBO names
    GLuint VAO_Cow, 
      VBO_CowPos, 
      VBO_CowNorm, 
      VBO_CowMatamb, 
      VBO_CowMatdiff, 
      VBO_CowMatspec, 
      VBO_CowMatshin;
    
    GLuint VAO_Terra, 
      VBO_TerraPos, 
      VBO_TerraNorm, 
      VBO_TerraMatamb, 
      VBO_TerraMatdiff, 
      VBO_TerraMatspec, 
      VBO_TerraMatshin;
      
    QGLShaderProgram *program;

    GLuint transLoc, projLoc, viewLoc, focusLoc, colorLoc, llumAmbLoc, pintaVacaLoc;
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;
    Model cow;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreCow;
    float escala;
    float radiEsc;
    float d;
    float zoom;

    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    
    //CajaEscena
    struct {
      glm::vec3 max;
      glm::vec3 min;
    } cajaContenedoraEscena;
    
    struct {
      glm::vec3 centro;
      float radio;
    } esferaContenedoraEscena;
    
    GLfloat focus;
    glm::vec3 posFocus;
};
