// Texture Shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TextureCoord;

void main()
{
	v_Color = a_Color;
	v_TextureCoord = a_TextureCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TextureCoord;

uniform vec4 u_Color;
uniform float u_TilingFactor;
uniform sampler2D u_Texture;

void main()
{
	// color = texture(u_Texture, v_TextureCoord * u_TilingFactor) * u_Color;
	color = v_Color;
}
