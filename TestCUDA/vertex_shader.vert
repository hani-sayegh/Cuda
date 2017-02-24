#version 430
in layout (location = 0) vec2 position;
in layout (location = 1) vec3 color;
out vec3 fin_color;
void main()
{
gl_Position = vec4(position, 0, 1);
fin_color = color;
};
