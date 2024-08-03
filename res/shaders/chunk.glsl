#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 fragCoord;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	fragCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 fragCoord;

out vec4 fragColor;

uniform vec3 u_Color;

void main() {
    vec2 uv = fragCoord * 2.0 - 1.0;

    float distance = 1.0 - length(uv);

    distance = smoothstep(0.0, 1.0, distance);

    fragColor.rgb = u_Color;
    fragColor.a = sin(distance*31.4159);
};