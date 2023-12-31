#version 330 core

in vec2 vUv;
out vec4 outColor;

uniform int layer;
uniform sampler2DArray spritesheet;

void main()
{
    outColor = texture(spritesheet, vec3(vUv, layer));
}