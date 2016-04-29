#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent) {
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = 0.0;
  zoom = 0.0;
  DoingInteractive = NONE;
  posFocus = glm::vec3(0.0, 0.0, 0.0);
  
  //Caja contenedora de la escena HARDCODED
  cajaContenedoraEscena.max = glm::vec3(1.0f, 2.0f, 1.0f);
  cajaContenedoraEscena.min = glm::vec3(-1.0f, -1.0f, -1.0f);
  
  esferaContenedoraEscena.centro = glm::vec3(
    (cajaContenedoraEscena.max[0]+cajaContenedoraEscena.min[0])/2.0,
    (cajaContenedoraEscena.max[1]+cajaContenedoraEscena.min[1])/2.0,
    (cajaContenedoraEscena.max[2]+cajaContenedoraEscena.min[2])/2.0
  );
  esferaContenedoraEscena.radio =  sqrt(
    pow(cajaContenedoraEscena.max[0] - esferaContenedoraEscena.centro[0], 2) + 
    pow(cajaContenedoraEscena.max[1] - esferaContenedoraEscena.centro[1], 2) + 
    pow(cajaContenedoraEscena.max[2] - esferaContenedoraEscena.centro[2], 2)
  );
  
  radiEsc = esferaContenedoraEscena.radio;
  d = 2* radiEsc;
}

void MyGLWidget::initializeGL () {
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor (0.5, 0.7, 1.0, 1.0);  // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();
  projectTransform();
  viewTransform();
}

void MyGLWidget::paintGL () {
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  iluminacion();
  
  
  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);
  modelTransformTerra();
  // pintem
  glUniform1i(pintaVacaLoc, 0);
  glDrawArrays(GL_TRIANGLES, 0, 12);
  
  //Cow
  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Cow);
  modelTransformCow();

  // Pintem l'escena
  glUniform1i(pintaVacaLoc, 1);
  glDrawArrays(GL_TRIANGLES, 0, cow.faces().size()*3);
  
  
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h){
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers (){
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  //patr.load("/assig/idi/models/Patricio.obj");
  cow.load("./models/cow.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();
  
  // Creació del Vertex Array Object del Patricio
  //Cow
	    glGenVertexArrays(1, &VAO_Cow);
	    glBindVertexArray(VAO_Cow);

	    // Creació dels buffers del model patr
	    // Buffer de posicions
	    glGenBuffers(1, &VBO_CowPos);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowPos);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_vertices(), GL_STATIC_DRAW);

	    // Activem l'atribut vertexLoc
	    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(vertexLoc);

	    // Buffer de normals
	    glGenBuffers(1, &VBO_CowNorm);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowNorm);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_normals(), GL_STATIC_DRAW);

	    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(normalLoc);

	    // En lloc del color, ara passem tots els paràmetres dels materials
	    // Buffer de component ambient
	    glGenBuffers(1, &VBO_CowMatamb);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatamb);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matamb(), GL_STATIC_DRAW);

	    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(matambLoc);

	    // Buffer de component difusa
	    glGenBuffers(1, &VBO_CowMatdiff);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatdiff);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matdiff(), GL_STATIC_DRAW);

	    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(matdiffLoc);

	    // Buffer de component especular
	    glGenBuffers(1, &VBO_CowMatspec);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatspec);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matspec(), GL_STATIC_DRAW);

	    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(matspecLoc);

	    // Buffer de component shininness
	    glGenBuffers(1, &VBO_CowMatshin);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatshin);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3, cow.VBO_matshin(), GL_STATIC_DRAW);

	    glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(matshinLoc);

  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[12] = {
	glm::vec3(-1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(-1.0, 1.0, -1.0),
	glm::vec3(-1.0, 1.0, -1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, 1.0, -1.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };

  // Definim el material del terra
  glm::vec3 amb(0., 0., 0.6);
  glm::vec3 diff(0., 0., 1.);
  glm::vec3 spec(1, 1, 1);
  float shin = 25;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}


void MyGLWidget::modelTransformCow() {
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(0.0, -0.5, 0.0));
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(0.0, 1.0, 0.0));
  TG = glm::rotate(TG, float(-M_PI/2), glm::vec3(1.0, 0.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreCow);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra () {
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform () {
  //sempre mantenim la aspect ratio de la pantalla per evitar deformacions
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(float(M_PI/5.0) + zoom, 1.0f, d, 3.0f*d);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
  glm::mat4 View;  // Matriu de posició i orientació
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*d));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iluminacion() {
  //glm::vec3 posFocus(posFocus.x, 0, 1);
  glUniform3fv(focusLoc, 1, &posFocus[0]);
  
  glm::vec3 colFocus(1.0, 1.0, 1.0);
  glUniform3fv(colorLoc, 1, &colFocus[0]);
  
  glm::vec3 llumAmbient(0.1, 0.1, 0.1);
  glUniform3fv(llumAmbLoc, 1, &llumAmbient[0]);
}


void MyGLWidget::calculaCapsaModel () {
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = cow.vertices()[0];
  miny = maxy = cow.vertices()[1];
  minz = maxz = cow.vertices()[2];
  for (unsigned int i = 3; i < cow.vertices().size(); i+=3) {
    if (cow.vertices()[i+0] < minx) minx = cow.vertices()[i+0];
    if (cow.vertices()[i+0] > maxx) maxx = cow.vertices()[i+0];
    if (cow.vertices()[i+1] < miny) miny = cow.vertices()[i+1];
    if (cow.vertices()[i+1] > maxy) maxy = cow.vertices()[i+1];
    if (cow.vertices()[i+2] < minz) minz = cow.vertices()[i+2];
    if (cow.vertices()[i+2] > maxz) maxz = cow.vertices()[i+2];
  }
  escala = 1.0/(maxz-minz);
  centreCow[0] = (minx+maxx)/2.0; 
  centreCow[1] = (miny+maxy)/2.0; 
  centreCow[2] = (minz+maxz)/2.0;
}

void MyGLWidget::carregaShaders () {
  // Creem els shaders per al fragment i vertex shader
  QGLShader fs(QGLShader::Fragment, this);
  QGLShader vs(QGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem 
  fs.compileSourceFile("./shaders/fragshad.frag");
  vs.compileSourceFile("./shaders/vertshad.vert");

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
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");
  

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  
  focusLoc = glGetUniformLocation (program->programId(), "posFocus");
  colorLoc = glGetUniformLocation (program->programId(), "colFocus");
  llumAmbLoc = glGetUniformLocation (program->programId(), "llumAmbient");
  
  pintaVacaLoc = glGetUniformLocation(program->programId(), "pintaVaca");
  
}

void MyGLWidget::keyPressEvent (QKeyEvent *e){
  switch (e->key()) {
    case Qt::Key_Escape:
        exit(0);
    case Qt::Key_Left:
	posFocus.x -= 0.5;
	iluminacion();
	break;
    case Qt::Key_Right:
	posFocus.x += 0.5;
	iluminacion();
	break;
    case Qt::Key_Up:
	posFocus.z -= 0.5;
	iluminacion();
	break;
    case Qt::Key_Down:
	posFocus.z += 0.5;
	iluminacion();
	break;
    default: e->ignore(); break;
  }
  updateGL();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e){
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton && ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))  {
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
  if (DoingInteractive == ROTATE)   {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    viewTransform ();
  } else if (DoingInteractive == ZOOM ) {
    zoom += (e->y() - yClick) * 0.005;
    projectTransform(); 
  }

  xClick = e->x();
  yClick = e->y();

  updateGL();
}


