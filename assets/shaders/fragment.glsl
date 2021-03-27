#version 330 core
out vec4 FragColor;

uniform float time;
uniform vec2 resolution;
uniform vec3 position;
uniform vec2 mouse;
uniform vec3 user;

uniform vec3 scene1Color;
uniform vec3 scene1Position;
uniform sampler2D texture01;
uniform sampler2D texture02;

void main() {
    vec2 position = (gl_FragCoord.xy / resolution.xy);
    float color = 0.0;
    color += sin(position.x * cos(time / 15.0) * 80.0) + cos(position.y * cos(time / 15.0) * 10.0);
    color += sin(position.y * sin(time / 10.0) * 40.0) + cos(position.x * sin(time / 25.0) * 40.0);
    color += sin(position.x * sin(time / 5.0) * 10.0) + sin(position.y * sin(time / 35.0) * 80.0);
    color *= sin(time / 10.0) * 0.5;
    FragColor = vec4(vec3(color, color * 0.5, sin(color + time / 3.0) * 0.75) * (scene1Color * texture2D(texture02, position).rgb * texture2D(texture02, position).rgb), 1.0); 
}