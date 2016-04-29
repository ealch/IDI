#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;   // Buffer de component ambient
in vec3 matdiff;  // Buffer de component difusa
in vec3 matspec;  // Buffer de component especular
in float matshin; // Buffer de component shininness

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 colFocus;

//Sesion7
out vec3 matamb_frag;
out vec3 matdiff_frag;
out vec3 matspec_frag;
out float matshin_frag;
out vec4 vertSCO;
out vec3 normalSCO;

out vec3 fcolor;

// Valors per als components que necessitem dels focus de llum
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
//vec3 posFocus = vec3(1, 0, -1);  // en SCA
//vec3 colFocus = vec3(0.8, 0.8, 0.8);

void main() {
    vertSCO = view * TG * vec4(vertex, 1.0);
    mat3 nMatrix = inverse(transpose(mat3(view * TG)));
    normalSCO = normalize(nMatrix * normal);
    
    fcolor = colFocus;
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    matamb_frag = matamb;
    matdiff_frag = matdiff;
    matspec_frag = matspec;
    matshin_frag = matshin;
}
