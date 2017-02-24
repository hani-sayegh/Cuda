#version 430
out vec4 color;
in vec3 fin_color;
void main()
{
color = vec4(fin_color, 1);
}
