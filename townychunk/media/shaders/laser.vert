#version 330 core

layout(location = 0) in vec4 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

void main() {
    gl_Position = modelViewProjectionMatrix * vertexPosition;
}