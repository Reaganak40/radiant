#version 450 core

smooth in vec4 vColor;
in vec2 vTexCoords;
flat in uint vTexIndex;

layout (location = 0) out vec4 color;

uniform vec4 uColor;
uniform sampler2D uTextures[30];

void main() {
	if (vTexIndex > 0) {
		color = texture(uTextures[vTexIndex], vTexCoords);
	} else {
		color = vColor
	}
}