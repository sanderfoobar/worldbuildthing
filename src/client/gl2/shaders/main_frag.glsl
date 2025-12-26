#version 400 core
in vec3 fragPos;
flat in vec3 faceNormal;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D uAlbedoMap;
uniform vec2 uUVScale;
uniform vec2 uUVOffset;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;
uniform vec3 uViewPos;
uniform bool uPhongEnabled;
uniform bool uFakeShadingEnabled;
uniform bool uDebugNormalsEnabled;
uniform bool uDrawAABB;

const float FAKE_FACE_SHADES[6] = float[6](0.90, 0.98, 1.04, 1.07, 1.10, 1.13);

const float PHONG_DIFFUSE_STRENGTH = 0.85;
const float PHONG_SPECULAR_STRENGTH = 0.15;
const float PHONG_SPECULAR_POWER = 24.0;
const float PHONG_MAX_LIGHT = 1.2;

void main() {
    if(uDrawAABB) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0); // red wireframe
        return;
    }

    vec2 uv = fragTexCoord / 0.8;
    uv = uv * uUVScale + uUVOffset;
    vec3 albedo = texture(uAlbedoMap, uv).rgb;

    vec3 N = normalize(faceNormal);
    vec3 color = albedo;

    if (uDebugNormalsEnabled) {
        fragColor = vec4(0.5 * (N + vec3(1.0)), 1.0);
        return;
    }

    if (uPhongEnabled) {
        vec3 L = normalize(-uLightDir);
        vec3 V = normalize(uViewPos - fragPos);
        vec3 R = reflect(-L, N);

        float diff = max(dot(N,L), 0.0);
        float spec = pow(max(dot(V,R), 0.0),PHONG_SPECULAR_POWER);

        vec3 lighting =
            uAmbientIntensity * uAmbientColor +
            diff * uLightColor * PHONG_DIFFUSE_STRENGTH +
            spec * uLightColor * PHONG_SPECULAR_STRENGTH;

        lighting = min(lighting, vec3(PHONG_MAX_LIGHT));
        color = lighting * albedo;
    }

    if (uFakeShadingEnabled) {
        int axis;
        if (abs(N.x) > abs(N.y) && abs(N.x) > abs(N.z))
        axis = N.x > 0.0 ? 0 : 1;
        else if (abs(N.y) > abs(N.z))
        axis = N.y > 0.0 ? 2 : 3;
        else
        axis = N.z > 0.0 ? 4 : 5;

        color *= FAKE_FACE_SHADES[axis];
    }

    fragColor = vec4(color, 1.0);
}