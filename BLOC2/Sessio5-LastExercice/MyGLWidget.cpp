#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent) {
  setFocusPolicy(Qt::ClickFocus); // per rebre events de teclat
  
  xClick = yClick = 0;
  DoingInteractive = NONE;
  cam = PERSPECTIVE;
}



void MyGLWidget::initializeGL () {
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  
  carregaShaders();
  
  //Usado para consultar y calcular caja contenedora y esfera minima contenedora
  //Dado que tanto la caja contenedora como la esfera contenedora tiene exactamente las mismas dimensiones
  m.load("./models/Patricio.obj");
  
  
  //Modelos a tratar
  m1.load("./models/Patricio.obj");
  m2.load("./models/Patricio.obj");
  createBuffers();
  
  calcularCajaContenedora();
  calcularEsferaMinimaContenedora();
  
  initParametros();
  
}

void MyGLWidget::paintGL () {
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransformTerra();
  // Carreguem la transformació de la projecció
  projectTransform();
  viewTransform();
  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);
  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  
  //Patricio 1
    // Carreguem la transformació de model
    modelTransformModel1();
    // Carreguem la transformació de la projecció
    projectTransform();
    viewTransform();
    // Activem el VAO per a pintar el model
    glBindVertexArray (VAO_model1);
    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m1.faces().size()*3);
  
  
  //Patricio 2
    // Carreguem la transformació de model
    modelTransformModel2();
    // Carreguem la transformació de la projecció
    projectTransform();
    viewTransform();
    // Activem el VAO per a pintar el model
    glBindVertexArray (VAO_model2);
    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m2.faces().size()*3);
  
  glBindVertexArray (0);
}

void MyGLWidget::initParametros() {
  scale = 1.0f;
  theta = M_PI/4;
  psi = 0.0;
  phi = 0.0;
  zoom = 0.0;
  
  sclPatricios = 1/(cajaContenedora.puntoMax.y - cajaContenedora.puntoMin.y);
  
  d = 2 * esfera.radio;
  //viewTransform
  OBS = glm::vec3(0. , 1. , d);
  VRP = glm::vec3(0., 0., 0.);
  up = glm::vec3(1., 1., 0.);
  //projectTransform
  FOV = FOVinicial = 2 * asin (esfera.radio/d);
  zNear = d - esfera.radio;
  zFar = d + esfera.radio;
}

void MyGLWidget::calcularCajaContenedora() {
  cajaContenedora.puntoMax = glm::vec3 (m.vertices()[0], m.vertices()[1], m.vertices()[2]);
  cajaContenedora.puntoMin = glm::vec3 (m.vertices()[0], m.vertices()[1], m.vertices()[2]);
  
  //puntoMax y puntoMin del homer
  for (unsigned int i = 3; i < m.vertices().size(); i+= 3){
    if (m.vertices()[i] > cajaContenedora.puntoMax.x) cajaContenedora.puntoMax.x= m.vertices()[i];
    if (m.vertices()[i] < cajaContenedora.puntoMin.x) cajaContenedora.puntoMin.x = m.vertices()[i];
    
    if (m.vertices()[i+1] > cajaContenedora.puntoMax.y) cajaContenedora.puntoMax.y = m.vertices()[i+1];
    if (m.vertices()[i+1] < cajaContenedora.puntoMin.y) cajaContenedora.puntoMin.y = m.vertices()[i+1];
    
    if (m.vertices()[i+2] > cajaContenedora.puntoMax.z) cajaContenedora.puntoMax.z = m.vertices()[i+2];
    if (m.vertices()[i+2] < cajaContenedora.puntoMin.z) cajaContenedora.puntoMin.z = m.vertices()[i+2];
  }
  std::cout << std::endl;
  std::cout << "Patricio" << std::endl;
  std::cout << "\tPunto max: " << cajaContenedora.puntoMax.x << " " << cajaContenedora.puntoMax.y << " " << cajaContenedora.puntoMax.z << std::endl;
  std::cout << "\tPunto min: " << cajaContenedora.puntoMin.x << " " << cajaContenedora.puntoMin.y << " " << cajaContenedora.puntoMin.z << std::endl;
  std::cout << std::endl;

  /*
  std::cout << "Escena" << std::endl;
  std::cout << "\tPunto max: " << cajaContenedora.puntoMax.x << " " << cajaContenedora.puntoMax.y << " " << cajaContenedora.puntoMax.z << std::endl;
  std::cout << "\tPunto min: " << cajaContenedora.puntoMin.x << " " << cajaContenedora.puntoMin.y << " " << cajaContenedora.puntoMin.z << std::endl;
  std::cout << std::endl;*/
}

void MyGLWidget::calcularEsferaMinimaContenedora() {
  esfera.centro.x = (cajaContenedora.puntoMin.x + cajaContenedora.puntoMax.x)/2;
  esfera.centro.y = (cajaContenedora.puntoMin.y + cajaContenedora.puntoMax.y)/2;
  esfera.centro.z = (cajaContenedora.puntoMin.z + cajaContenedora.puntoMax.z)/2;
  
  esfera.radio = sqrt(
			pow(cajaContenedora.puntoMax.x - esfera.centro.x, 2) + 
			pow(cajaContenedora.puntoMax.y - esfera.centro.y, 2) + 
			pow(cajaContenedora.puntoMax.z - esfera.centro.z, 2)
		     );
  
  std::cout << "Esfera" << std::endl;
  std::cout << "\tCentro: " << esfera.centro.x << " " << esfera.centro.y << " " << esfera.centro.z << std::endl;
  std::cout << "\tRadio: " << esfera.radio << std::endl;
  std::cout << std::endl;
}

void MyGLWidget::modelTransformTerra () {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  //transform = glm::rotate(transform, .58f, glm::vec3(1.,0.,0.));
  //transform = glm::translate(transform,  esfera.centro);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

//Patricio 1
void MyGLWidget::modelTransformModel1 () {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(1., 0.5, 1.));
  transform = glm::scale(transform, glm::vec3(sclPatricios));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, float(M_PI), glm::vec3(0., 1., 0.));
  transform = glm::translate(transform, -esfera.centro);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

//Patricio 2
void MyGLWidget::modelTransformModel2 () {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-1., 0.5, -1.));
  transform = glm::scale(transform, glm::vec3(sclPatricios));
  transform = glm::scale(transform, glm::vec3(scale));
  //transform = glm::rotate(transform, .58f, glm::vec3(1.,0.,0.));
  //transform = glm::translate(transform,  elevacion);
  transform = glm::translate(transform, -esfera.centro);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
  if (cam == PERSPECTIVE) projectTransformPerspective();
  else projectTransformOrtho();
}

void MyGLWidget::projectTransformPerspective() {
  ra = float(width())/float(height());
  if (ra >= 1) {
    glm::mat4 Proj = glm::perspective(FOV+zoom, ra, zNear, zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
  } 
  else {
    FOV = 2*atan(tan(FOVinicial/2)/ra);
    //glm::perspective(FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective(FOV+zoom, ra, zNear, zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::projectTransformOrtho() {
    glm::mat4 Proj = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, zNear, zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
  //glm::lockAt(OBS, VRP, up)
  //calculados en initParametros y recalcularParametros
  glm::mat4 View (1.0f);
  View = glm::translate(View, glm::vec3(0, 0, -d));
  View = glm::rotate(View, -phi, glm::vec3(0., 0., 1.));
  View = glm::rotate(View, theta, glm::vec3(1., 0., 0.));
  View = glm::rotate(View, -psi, glm::vec3(0., 1., 0.));
  View = glm::translate(View, glm::vec3(-VRP));
  
  //glm::mat4 View = glm::lookAt(OBS, VRP, up);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) {
  glViewport(0, 0, w, h);
  projectTransform();
}


void MyGLWidget::createBuffers () {
  // Creació del Vertex Array Object per pintar
  //Patricio 1
  glGenVertexArrays(1, &VAO_model1);
  glBindVertexArray(VAO_model1);

  glGenBuffers(1, &VBO_modelPos1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_modelPos1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m1.faces().size()*3*3, m1.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_modelCol1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_modelCol1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m1.faces().size()*3*3, m1.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  
  //Patricio 2
  glGenVertexArrays(1, &VAO_model2);
  glBindVertexArray(VAO_model2);

  glGenBuffers(1, &VBO_modelPos2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_modelPos2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m2.faces().size()*3*3, m2.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_modelCol2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_modelCol2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m2.faces().size()*3*3, m2.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Dades del terra
  // Dos VBOs, un amb posició i l'altre amb color
  /*posterra = {
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, 1.0)
  };*/
  
  posterra[0] = glm::vec3(-2.0, 0.0, 2.0);
  posterra[1] = glm::vec3(-2.0, 0.0, -2.0);
  posterra[2] = glm::vec3(2.0, 0.0, 2.0);
  posterra[3] = glm::vec3(2.0, 0.0, -2.0);

  /*colterra = {
	glm::vec3(1,0,1),
	glm::vec3(1,0,1),
	glm::vec3(1,0,1),
	glm::vec3(1,0,1)
  };*/
  colterra[0] = glm::vec3(1,0,1);
  colterra[1] = glm::vec3(1,0,1);
  colterra[2] = glm::vec3(1,0,1);
  colterra[3] = glm::vec3(1,0,1);

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}


void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      updateGL();
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      updateGL();
      break;
    }
    case Qt::Key_K:
      if (cam == PERSPECTIVE) cam = ORTHO;
      else if (cam == ORTHO) cam = PERSPECTIVE;
      updateGL();
      break;
    default: event->ignore(); break;
  }
}

void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton && ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) {
    DoingInteractive = ROTATE;
  } else if (e->button() & Qt::RightButton && ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) {
    DoingInteractive = ZOOM;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *){
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)  {
    // Fem la rotació
    psi += (e->x() - xClick) * M_PI / 180.0;
    theta += (e->y() - yClick) * M_PI / 180.0;
    viewTransform ();
  } else if (DoingInteractive == ZOOM ) {
    zoom += (e->y() - yClick) * 0.005;
    projectTransform(); 
  }

  xClick = e->x();
  yClick = e->y();

  updateGL ();
}

void MyGLWidget::carregaShaders() {
  // Creem els shaders per al fragment shader i el vertex shader
  QGLShader fs (QGLShader::Fragment, this);
  QGLShader vs (QGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}
