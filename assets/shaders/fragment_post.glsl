#version 330 core
out vec4 FragColor;

uniform sampler2D mainImage;
uniform float effect1;
in vec2 texPosition;

void main(void) {
	vec4 color = mod(gl_FragCoord.y, 2.0) * texture(mainImage, texPosition);
	gl_FragColor = color;
} 