#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 v_TexCoord;

uniform mat4 u_projection;
uniform mat4 u_model;

void main()
{
    gl_Position = u_projection * u_model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    v_TexCoord = aTexCoord;
}