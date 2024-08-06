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

in vec2 fragCoord;

out vec4 fragColor;

uniform float iTime;

void main() {
    vec3 col = 0.5 + 0.5 * cos(iTime + fragCoord.xyx + vec3(0, 2, 4));

    // Output to screen
    fragColor.rgb = col;

    fragColor.a = 1-length(fragCoord);
};