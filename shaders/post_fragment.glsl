#version 330 core
out vec4 FragColor;

uniform sampler2D fbo_texture;
in vec2 f_texcoord;

void main(void) {
  //vec2 texcoord = f_texcoord;
  //texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;
  FragColor = vec4(0.1, 0.1, 0.9, 0.5);
}