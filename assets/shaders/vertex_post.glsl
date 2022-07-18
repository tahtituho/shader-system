#version 330 core
layout (location = 0) in vec3 aPos;

in vec2 position;
uniform sampler2D mainImage;
out vec2 texPosition;

void main(void) {
  gl_Position = vec4(position, 0.0, 1.0);
  texPosition = (position + 1.0) / 2.0;
}