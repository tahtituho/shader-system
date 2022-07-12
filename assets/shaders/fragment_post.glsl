#version 330 core
out vec4 FragColor;

uniform sampler2D fbo_texture;
in vec2 f_texcoord;

void main(void) {
  vec4 colorFromTex = texture2D(fbo_texture, f_texcoord);
  // Testing values from texture
  if (colorFromTex.x > 0.2) {
	  gl_FragColor = vec4(1,0,0,0);
  }
  else {
	  gl_FragColor = colorFromTex;
  }
} 