#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out float fogCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0f);

    FragPos = vec3(modelMatrix * vec4(inPosition, 1.0f));
    Normal = mat3(transpose(inverse(modelMatrix))) * inNormal;  
    TexCoords = vec2(inTexCoord[0],1.0 - inTexCoord[1]);
	fogCoord = abs(FragPos.z);
} 