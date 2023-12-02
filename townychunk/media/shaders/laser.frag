#version 330 core

out vec4 fragColor;

void main() {
    vec3 quadColor = vec3(0.0, 1.0, 0.0);
    fragColor = vec4(quadColor, 1.0);
}