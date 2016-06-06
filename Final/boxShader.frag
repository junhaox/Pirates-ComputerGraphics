#version 330 core

out vec4 color;

uniform int collided;

void main()
{
    if (collided == 0)
        color = vec4(255.0, 255.0f, 255.0f, 0.0f);
    else if (collided == 1)
        color = vec4(255.0, 0.0f, 0.0f, 0.0f);
}