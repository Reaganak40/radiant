#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in uint texIndex;

smooth out vec4 vColor;
out vec2 vTexCoords;
out uint vTexIndex;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * position;
    vColor = color;
    vTexCoords = texCoords;
    vTexIndex = texIndex;
};