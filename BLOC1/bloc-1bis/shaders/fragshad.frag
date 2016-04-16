#version 330 core

out vec4 FragColor;

in vec3 fcolor;

void main() {
  /*
    //Colorear
      //3er cuadrante
      if (gl_FragCoord.x < 400.0 && gl_FragCoord.y < 400.0) {
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
      } 
      //4o cuadrante
      else if (gl_FragCoord.x > 400.0 && gl_FragCoord.y < 400.0) {
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
      }
      //2o cuadrante
      else if (gl_FragCoord.x < 400.0 && gl_FragCoord.y > 400.0) {
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
      }
      //1er cuadrante
      else if (gl_FragCoord.x > 400.0 && gl_FragCoord.y > 400.0) {
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
      }
      else {
	FragColor = vec4(1.0);
      }
    
    //Descartar
      if (int(gl_FragCoord.y)%80 <= 10.0) {
	discard;
      }
  */
  FragColor = vec4(fcolor,1.0);
}