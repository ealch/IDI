#version 330 core

in vec3 fcolor;

in vec4 vertSCO;
in vec3 normalSCO;

in vec3 matamb_frag;
in vec3 matdiff_frag;
in vec3 matspec_frag;
in float matshin_frag;

uniform vec3 posFocus;
uniform vec3 llumAmbient;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec4 FragColor;

vec3 Lambert (vec3 NormSCO, vec3 L) {
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb_frag;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0) colRes = colRes + fcolor * matdiff_frag * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) {
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0) return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin_frag == 0)) return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin_frag);
    return (colRes + matspec_frag * fcolor * shine); 
}

void main() {
  //Iluminación de escena
  //vec4 posFocusSCO =  view * TG * vec4(posFocus, 1.0);
  //vec3 L = normalize(posFocusSCO.xyz - vertSCO.xyz);
  
  //Iluminación cámara
  vec3 L = normalize(posFocus - vertSCO.xyz);
  FragColor = vec4(Phong(normalSCO, L, vertSCO),1);
}
