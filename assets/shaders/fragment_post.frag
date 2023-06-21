#version 330 core
out vec4 FragColor;

uniform sampler2D mainImage;
uniform sampler2D logo;
uniform float effect1;
in vec2 texPosition;

#define PI 3.14159265359

vec2 rotateUV(vec2 uv, float rotation)
{
    float mid = 0.5;
    return vec2(
        cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
        cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid
    );
}

void main(void) {
	vec4 color = mod(gl_FragCoord.y, 2.0) * texture(mainImage, texPosition);
	
	vec2 logoUv = texPosition;
	logoUv.y = (logoUv.y - 1.0) * -1.0;
	vec4 logoColor = texture(logo, rotateUV(logoUv * vec2(4.0, 4.0) - vec2(0.1, 0.1), PI * 1.9));

	color = mix(color, logoColor, logoColor.a);
	color = pow(color, vec4(1.0 / 1.2));
	gl_FragColor = color;
} 