#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float selected;

out vec2 fragCoord;
out float isSelected;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	fragCoord = texCoord;
	isSelected = selected;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 fragCoord;
in float isSelected;

out vec4 fragColor;

uniform vec3 u_Color;

uniform float iTime;

void main() {
    //vec2 uv = fragCoord * 2.0 - 1.0;

    //float distance = 1.0 - length(uv);

    //distance = smoothstep(0.0, 1.0, distance);

    //fragColor.rgb = u_Color;
    //fragColor.a = sin(distance*31.4159);

	vec2 p = fragCoord;
	float tau = 3.1415926535 * 2.0;
	float a = atan(p.x, p.y);
	float r = length(p) * 0.75;
	vec2 uv = vec2(a / tau, r);

	//get the color
	float xCol = (uv.x - (iTime / 3.0)) * 3.0;
	xCol = mod(xCol, 3.0);
	vec3 horColour = vec3(0.25, 0.25, 0.25);

	if (xCol < 1.0) {

		horColour.r += 1.0 - xCol;
		horColour.g += xCol;
	}
	else if (xCol < 2.0) {

		xCol -= 1.0;
		horColour.g += 1.0 - xCol;
		horColour.b += xCol;
	}
	else {

		xCol -= 2.0;
		horColour.b += 1.0 - xCol;
		horColour.r += xCol;
	}

	// draw color beam
	uv = (2.0 * uv) - 1.0;
	float beamWidth = (1.7 + 0.5 * cos(uv.x * 2.0 * tau * 0.15 * 5.0 + 10.0 * cos(iTime))) * abs(1.0 / (30.0 * uv.y));
	vec3 horBeam = vec3(beamWidth);
	fragColor.rgb = (horBeam)*horColour*u_Color;
	fragColor.a = smoothstep(0.05, 0.1, (fragColor.r + fragColor.g + fragColor.b)/3);
	if (isSelected==1.0) {
		fragColor.rgb *= 3.0;
	}
};