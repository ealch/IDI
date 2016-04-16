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
  vs.compileSourceFile("./vertshad.vert");
  QGLShader fs(QGLShader::Fragment, this);
  fs.compileSourceFile("./fragshad.frag");
  
  program = new QGLShaderProgram(this);
  program->addShader(&vs);
  program->addShader(&fs);
  program->link();
  program->bind();
}

void MyGLWidget::initializeGL () {
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  //Sesion 2
  loadShaders();
  createBuffers();
  
  //Sesión 3
  modelTransform();
  
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

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  
  /*
  //Cuadrado a partir de dos triangulos
      // Activem l'Array a pintar 
      glBindVertexArray(VAO_TRIANGULO1);
      modelTransform1();
      // Pintem l'escena
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      // Activem l'Array a pintar 
      glBindVertexArray(VAO_TRIANGULO2);
      modelTransform2();
    
      // Pintem l'escena
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
  */
  //Casa con tringle-strip
    // Activem l'Array a pintar
    glBindVertexArray(VAO_CASA);
    
    //Actualizar Transformaciones geométricas
    modelTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers()
{
  /*
  //Cuadrado a partir de dos triangulos
      //Triangulo1
      glm::vec3 triangulo1[3];  // Tres vèrtexs amb X, Y i Z
      triangulo1[0] = glm::vec3(-0.75, -0.75, 0.0);
      triangulo1[1] = glm::vec3(-0.75, 0.75, 0.0);
      triangulo1[2] = glm::vec3(0.75, -0.75, 0.0);
      
	//Triangulo2
      glm::vec3 triangulo2[3];  // Tres vèrtexs amb X, Y i Z
      triangulo2[0] = glm::vec3(-0.75, 0.75, 0.0);
      triangulo2[1] = glm::vec3(0.75, -0.75, 0.0);
      triangulo2[2] = glm::vec3(0.75, 0.75, 0.0);
	
      
      //Triangulo1
	// Creació del Vertex Array Object (VAO) que usarem per pintar
	glGenVertexArrays(1, &VAO_TRIANGULO1);
	glBindVertexArray(VAO_TRIANGULO1);

	// Creació del buffer amb les dades dels vèrtexs
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo1), triangulo1, GL_STATIC_DRAW);
	// Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);
	
	pos1 = glGetAttribLocation(program->programId(), "vertex");
	glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos1);
      
      
      //Triangulo2
	// Creació del Vertex Array Object (VAO) que usarem per pintar
	glGenVertexArrays(1, &VAO_TRIANGULO2);
	glBindVertexArray(VAO_TRIANGULO2);

	// Creació del buffer amb les dades dels vèrtexs
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo2), triangulo2, GL_STATIC_DRAW);
	// Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);
	pos1 = glGetAttribLocation(program->programId(), "vertex");
	glVertexAttribPointer(pos2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos2);
	
      
      
      // Desactivem el VAO
      glBindVertexArray(0);
  
  */
  //Casa con triangle-strip
      //Casa
      glm::vec3 casa[5];
      casa[0] = glm::vec3(-0.75, -0.75, 0.0);
      casa[1] = glm::vec3(0.75, -0.75, 0.0);
      casa[2] = glm::vec3(-0.75, 0.25, 0.0);
      casa[3] = glm::vec3(0.75, 0.25, 0.0);
      casa[4] = glm::vec3(0.0, 0.75, 0.0);
      
      // Creació del Vertex Array Object (VAO) que usarem per pintar
      glGenVertexArrays(1, &VAO_CASA);
      glBindVertexArray(VAO_CASA);
      
      // Creació del buffer amb les dades dels vèrtexs
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(casa), casa, GL_STATIC_DRAW);
      
      // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
      //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      pos = glGetAttribLocation(program->programId(), "vertex");
      glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(pos);
      
      // Desactivem el VAO
      glBindVertexArray(0);
      
      //Sesión 2
      //Casa color
      glm::vec3 casa_color[5];
      casa_color[0] = glm::vec3(1.0, 0.0, 0.0);
      casa_color[1] = glm::vec3(0.0, 0.0, 1.0);
      casa_color[2] = glm::vec3(1.0, 1.0, 0.0);
      casa_color[3] = glm::vec3(0.0, 1.0, 1.0);
      casa_color[4] = glm::vec3(0.0, 1.0, 0.0);

      glBindVertexArray(VAO_CASA);

      // Creació del buffer amb les dades dels colors
      glGenBuffers(1, &VBO_color);
      glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
      glBufferData(GL_ARRAY_BUFFER, sizeof(casa_color), casa_color, GL_STATIC_DRAW);

      col = glGetAttribLocation(program->programId(), "color");	
      glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(col);

      // Desactivem el VAO
      glBindVertexArray(0);
      
      //Sesión 3
      //Pintamos por primera vez
      initVariables();
      getUniforms();
      //glUniform1f(varLoc, scl);
      modelTransform();
  
}

void MyGLWidget::modelTransform() {
  TG = glm::mat4(1.0);
  TG = glm::scale(TG, sclV);
  //Transladar y rotar
  TG = glm::rotate(TG, rot, glm::vec3(0.0,0.0,1.0));
  TG = glm::translate(TG, transV);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

/*
void MyGLWidget::modelTransform1() {
  TG = glm::mat4(1.0);
  TG = glm::scale(TG, sclV);
  //Transladar y rotar
  TG = glm::rotate(TG, rot, glm::vec3(0.0,0.0,1.0));
  TG = glm::translate(TG, transV);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform2() {
  TG = glm::mat4(1.0);
  TG = glm::scale(TG, sclV);
  //Transladar y rotar
  TG = glm::rotate(TG, -rot, glm::vec3(0.0,0.0,1.0));
  TG = glm::translate(TG, transV);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
*/

//Sesion 3

void MyGLWidget::getUniforms() {
  //varLoc = glGetUniformLocation(program->programId(), "val");
  transLoc = glGetUniformLocation(program->programId(), "TG");
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  switch(e->key()) {
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S:
      sclV.x += 0.1;
      sclV.y += 0.1;
      //glUniform1f(varLoc, scl);
      updateGL();
      break;
    case Qt::Key_D:
      sclV.x -= 0.1;
      sclV.y -= 0.1;
      //glUniform1f(varLoc, scl);
      updateGL();
      break;
    case Qt::Key_Left:
      transV.x -= 0.1;
      rot += 30.0;
      //modelTransform();
      updateGL();
      break;
    case Qt::Key_Right:
      transV.x += 0.1;
      rot += 30.0;
      //modelTransform();
      updateGL();
      break;
    case Qt::Key_Up:
      transV.y += 0.1;
      rot += 30.0;
      //modelTransform();
      updateGL();
      break;
    case Qt::Key_Down:
      transV.y -= 0.1;
      rot += 30.0;
      //modelTransform();
      updateGL();
      break;
    default:
      e->ignore();
  }
}
