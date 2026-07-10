#version 410 core

in vec2 TexCoord;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    vec4 c = texture(uTexture, TexCoord);

    FragColor = texture(uTexture, TexCoord);
}