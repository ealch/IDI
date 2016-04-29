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

uniform vec3 posFocus;
uniform vec3 colFocus;
uniform vec3 llumAmbient;


// Valors per als components que necessitem dels focus de llum
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
//vec3 posFocus = vec3(1, 0, -1);  // en SCA
//vec3 colFocus = vec3(0.8, 0.8, 0.8);

out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L) {
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) {
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine); 
}

void main() {
    //fcolor = matdiff;
    mat3 nMatrix = inverse(transpose(mat3(view * TG)));
    vec3 NormSCO = normalize(nMatrix * normal);
    
    vec4 vertexSCO = view * TG * vec4(vertex, 1.0);
    
    vec3 L = normalize(posFocus - vertexSCO.xyz);
    
    //Lambert
    //fcolor = Lambert(NormSCO, L);
    
    //Phong
    fcolor = Phong(NormSCO, L, vertexSCO);
    
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
