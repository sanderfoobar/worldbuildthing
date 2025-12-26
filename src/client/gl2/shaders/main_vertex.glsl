#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

flat out vec3 faceNormal;
out vec3 fragPos;
out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    fragPos = vec3(uModel * vec4(inPosition, 1.0));
    faceNormal = normalize(mat3(transpose(inverse(uModel))) * inNormal);
    fragTexCoord = inTexCoord;
    gl_Position = uProjection * uView * vec4(fragPos, 1.0);
}