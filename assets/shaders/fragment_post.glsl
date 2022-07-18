#version 330 core
out vec4 FragColor;

uniform sampler2D mainImage;
in vec2 texPosition;

void main(void) {
  vec4 colorFromTex = texture2D(mainImage, texPosition);
  // Values from texture
  gl_FragColor = colorFromTex;
} 