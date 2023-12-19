#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textTexutre;
uniform vec3 textColor;

void main()
{
    vec4 fontSample = vec4(1.0, 1.0, 1.0, texture(textTexutre, TexCoords).r);
    color = vec4(textColor, 1.0) * fontSample;
}