uniform float time;
varying vec4 light;

void main() {
    float alpha = 0.5 + 0.5 * sin(time * 5.0);
    gl_FragColor = vec4(0.0, 1.0, 0.0, alpha);
}