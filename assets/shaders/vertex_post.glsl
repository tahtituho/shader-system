#version 330 core
layout (location = 0) in vec3 aPos;

in vec2 v_coord;
uniform sampler2D fbo_texture;
out vec2 f_texcoord;

void main(void) {
  gl_Position = vec4(v_coord, 0.0, 1.0);
  f_texcoord = (v_coord + 1.0) / 2.0;
}