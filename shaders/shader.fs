
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{   
	// saturation filter example
	float m = 1;
	float l = ((m - 1) * -0.1);
	mat4 colorFilter = mat4(
		m, l, l, 0,
		l, m, l, 0,
		l, l, m, 0,
		0, 0, 0, 1);
    FragColor = colorFilter * texture(texture_diffuse1, TexCoords);
}
