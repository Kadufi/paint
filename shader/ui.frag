#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

uniform int u_Mode;
uniform vec3 u_Color;

void main()
{
    if (u_Mode == 0) {
        FragColor = vec4(u_Color, 1.0);
    } 
    else if (u_Mode == 1) {
        FragColor = vec4(v_TexCoord.x, 0.0, 0.0, 1.0);
    }
    else if (u_Mode == 2) {
        FragColor = vec4(0.0, v_TexCoord.x, 0.0, 1.0);
    }
    else if (u_Mode == 3) {
        FragColor = vec4(0.0, 0.0, v_TexCoord.x, 1.0);
    }
}