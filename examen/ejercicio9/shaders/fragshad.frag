#version 330 core

in vec3 fcolor;
out vec4 FragColor;

uniform int franjas;

void main() {
  if(franjas == 1) {
    if(int(gl_FragCoord.y)% 20 < 10) {
      FragColor = vec4(vec3(1.0, 1.0, 1.0), 1);
    } else {
      FragColor = vec4(vec3(0.0, 0.0, 0.0), 1);
    }
  }
  else {
    FragColor = vec4(fcolor,1);
  }

}
