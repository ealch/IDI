#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

//Sesion 2
void MyGLWidget::loadShaders () {
  QGLShader vs(QGLShader::Vertex, this);
  vs.compileSourceFile("./shaders/vertshad.vert");
  QGLShader fs(QGLShader::Fragment, this);
  fs.compileSourceFile("./shaders/fragshad.frag");
  
  program = new QGLShaderProgram(this);
  program->addShader(&vs);
  program->addShader(&fs);
  program->link();
  program->bind();
  
  
  //Uniforms and locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  pos = glGetAttribLocation(program->programId(), "vertex");
  col = glGetAttribLocation(program->programId(), "color");
}

void MyGLWidget::initializeGL () {
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL
  
  glEnable(GL_DEPTH_TEST);
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  //Sesion 2
  loadShaders();
  
  m1.load("./models/HomerProves.obj");
  m2.load("./models/HomerProves.obj");
  
  createBuffers();
  initVariables();
  
  //Sesión 3
  modelTransform1();
  modelTransform2();
  
}

void MyGLWidget::resizeGL (int w, int h) {
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers() {
  glm::vec3 ejes[6];  //Las 4 coordenadas que forman los ejes
      ejes[0] = glm::vec3(1.0, 0.0, 0.0);
      ejes[1] = glm::vec3(0.0, 0.0, 0.0);
      ejes[2] = glm::vec3(0.0, 1.0, 0.0);
      ejes[3] = glm::vec3(0.0, 0.0, 0.0);
      ejes[4] = glm::vec3(0.0, 0.0, 1.0);
      ejes[5] = glm::vec3(0.0, 0.0, 0.0);
      
  glm::vec3 color_ejes[6];
    color_ejes[0] = glm::vec3(1.0, 0.0, 0.0);
    color_ejes[1] = glm::vec3(1.0, 0.0, 0.0);
    color_ejes[2] = glm::vec3(0.0, 1.0, 0.0);
    color_ejes[3] = glm::vec3(0.0, 1.0, 0.0);
    color_ejes[4] = glm::vec3(0.0, 0.0, 1.0);
    color_ejes[5] = glm::vec3(0.0, 0.0, 1.0);
      
  //Ejes
    glGenVertexArrays(1, &VAO_Ejes);
    glBindVertexArray(VAO_Ejes);
    
    glGenBuffers(1, &VBO_EjesPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_EjesPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ejes), ejes, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(0);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos);
    
    glGenBuffers(1, &VBO_EjesColor);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_EjesColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_ejes), color_ejes, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(0);
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(col);
  
    glBindVertexArray(0);
  
  //Homer1
    glGenVertexArrays(1, &VAO_model1);
    glBindVertexArray(VAO_model1);
    
    //Posición
    glGenBuffers(1, &VBO_modelPos1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_modelPos1);
    glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * m1.faces().size() * 3 * 3,
		m1.VBO_vertices(),
		GL_STATIC_DRAW);

    // Activem l'atribut pos
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos);
    
    //Color
    glGenBuffers(1, &VBO_modelCol1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_modelCol1);
    glBufferData(GL_ARRAY_BUFFER, 
		sizeof(GLfloat) * m1.faces().size() * 3 * 3, 
		m1.VBO_matdiff(), 
		GL_STATIC_DRAW);

    // Activem l'atribut col
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(col);
  
  
    glBindVertexArray(0);
  
  //Homer2
    glGenVertexArrays(1, &VAO_model2);
    glBindVertexArray(VAO_model2);
    
    //Posición
    glGenBuffers(1, &VBO_modelPos2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_modelPos2);
    glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * m2.faces().size() * 3 * 3,
		m2.VBO_vertices(),
		GL_STATIC_DRAW);

    // Activem l'atribut pos
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos);
    
    //Color
    glGenBuffers(1, &VBO_modelCol2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_modelCol2);
    glBufferData(GL_ARRAY_BUFFER, 
		sizeof(GLfloat) * m2.faces().size() * 3 * 3, 
		m2.VBO_matdiff(), 
		GL_STATIC_DRAW);

    // Activem l'atribut col
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(col);
    
  
    glBindVertexArray(0);
}

void MyGLWidget::initVariables() {
  //scl = 0.5;
  
  sclV.x = 1.0;
  sclV.y = 1.0;
  sclV.z = 1.0;
  
  transV.x = 0.0;
  transV.y = 0.0;
  transV.z = 0.0;
  
  rot = 0.0;
}

void MyGLWidget::paintGL () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Esborrem el frame-buffer
  
  
  //Homer1
  // Activem l'Array a pintar
  glBindVertexArray(VAO_model1);
  //Actualizar Transformaciones geométricas
  modelTransform1();
  glDrawArrays(GL_TRIANGLES, 0, m1.faces().size() * 3);
  // Desactivem el VAO
  glBindVertexArray(0);
  
  
  //Homer2
  glBindVertexArray(VAO_model2);
  //Actualizar Transformaciones geométricas
  modelTransform2();
  glDrawArrays(GL_TRIANGLES, 0, m2.faces().size() * 3);
  // Desactivem el VAO
  glBindVertexArray(0);
  
  
  glBindVertexArray(VAO_Ejes);
  modelTransform3();
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);
  glDrawArrays(GL_LINES, 4, 2);
  // Desactivem el VAO
  glBindVertexArray(0);
}


void MyGLWidget::modelTransform1() {
  TG = glm::mat4(1.0);
  //TG = glm::scale(TG, sclV);
  TG = glm::rotate(TG, rot, glm::vec3(0.0, 1.0, 0.0));
  //Transladar y rotar
  TG = glm::translate(TG, transV);
  TG = glm::scale(TG, glm::vec3(0.5, 0.5, 0.5));
  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform2() {
  TG = glm::mat4(1.0);
  TG = glm::translate(TG, glm::vec3(0.0, 0.75, 0.0));
  //TG = glm::scale(TG, sclV);
  //Transladar y rotar
  TG = glm::rotate(TG, rot, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate(TG, transV);
  TG = glm::scale(TG, glm::vec3(0.25, 0.25, 0.25));
  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform3() {
  TG = glm::mat4(1.0);  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  switch(e->key()) {
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S:
      sclV.x += 0.1;
      sclV.y += 0.1;
      updateGL();
      break;
    case Qt::Key_D:
      sclV.x -= 0.1;
      sclV.y -= 0.1;
      updateGL();
      break;
    case Qt::Key_R:
      rot += M_PI/2;
      updateGL();
      break;
    case Qt::Key_Left:
      transV.x -= 0.1;
      updateGL();
      break;
    case Qt::Key_Right:
      transV.x += 0.1;
      updateGL();
      break;
    case Qt::Key_Up:
      transV.y += 0.1;
      updateGL();
      break;
    case Qt::Key_Down:
      transV.y -= 0.1;
      updateGL();
      break;
    default:
      e->ignore();
  }
}
