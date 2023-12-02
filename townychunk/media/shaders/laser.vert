varying vec4 light;

void main() {
    light = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
