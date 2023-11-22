hello, world

#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentColor;
out vec2 UV;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1);
	UV = vertexUV;
	fragmentColor = vertexColor;
};

#shader fragment
#version 330 core

in vec3 fragmentColor;
in vec2 UV;
out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{
	color = texture(myTextureSampler, UV).rgb;
};