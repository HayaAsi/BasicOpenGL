#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);
	v_TexCoord = texCoord;
}

#shader fragment
#version 330

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
	// Sample the texture color
	vec4 texColor = texture(u_Texture, v_TexCoord);

	// Combine with uniform color (optional)
	FragColor = texColor * u_Color;
}