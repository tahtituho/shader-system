#version 330 core
layout (location = 0) in vec3 aPos;

in vec2 v_coord;
uniform sampler2D fbo_texture;
out vec2 f_texcoord;

void main()
{
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0);
    //f_texcoord = (v_coord + 1.0) / 2.0;
}