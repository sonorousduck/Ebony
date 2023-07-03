#version 330 core

out vec4 outColor;
// in vec3 ourColor;
in vec2 vUvCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    outColor = mix(texture(texture1, vUvCoord), texture(texture2, vUvCoord), 0.2);
    // outColor = texture(textureSampler, vUvCoord) * vec4(ourColor, 1.0f);
}