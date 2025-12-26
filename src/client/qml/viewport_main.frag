#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec2 pixelStep;
    int resolutionx;
    int resolutiony;

    // background color
    vec4 bgColor;

    // border
    bool borderActive;
    vec4 borderColor;

    // blur
    float blurStrength;

    bool wtf;
    int radius;
    float deviation;
};
layout(binding = 1) uniform sampler2D src;

#define PI 3.1415926538

const float borderWidth = 2.0;
const float borderRadius = 10.0;

float gaussianWeight(vec2 coords) {
    float x2 = pow(coords.x, 2.0);
    float y2 = pow(coords.y, 2.0);
    float deviation2 = pow(deviation, 2.0);

    return (1.0 / (2.0 * PI * deviation2)) * exp(-(x2 + y2) / (2.0 * deviation2));
}

void main(void) {
    vec2 uv = vec2(qt_TexCoord0.x, 1.0 - qt_TexCoord0.y); // flipped Y, because FBO is flipped
    vec4 tex;

    vec4 original = texture(src, uv) * qt_Opacity;

    if(blurStrength > 0.0) {
        vec3 sum = vec3(0.0);
        float gaussianSum = 0.0;
        const int r = 8;
        for(int x = -r; x <= r; ++x) {
            for(int y = -r; y <= r; ++y) {
                vec2 c = uv + vec2(x, y) * pixelStep * blurStrength;
                float w = gaussianWeight(vec2(x, y));
                sum += texture(src, c).rgb * w;
                gaussianSum += w;
            }
        }
        vec3 blurred = sum / gaussianSum;
        vec4 blurredColor = vec4(blurred, 1.0) * qt_Opacity;
        tex = mix(original, blurredColor, blurStrength);
    } else {
        tex = original;
    }


    float px = qt_TexCoord0.x * float(resolutionx);
    float py = (1.0 - qt_TexCoord0.y) * float(resolutiony);

    bool drawBorder = false;
    bool insideCorner = true;

    if(borderActive) {
        if(px >= borderRadius && px <= float(resolutionx) - borderRadius) {
            if(py < borderWidth || py > float(resolutiony) - borderWidth) drawBorder = true;
        }
        if(py >= borderRadius && py <= float(resolutiony) - borderRadius) {
            if(px < borderWidth || px > float(resolutionx) - borderWidth) drawBorder = true;
        }
    }

    vec2 corner = vec2(-1.0);
    if(px < borderRadius && py < borderRadius) corner = vec2(borderRadius, borderRadius);
    else if(px > float(resolutionx) - borderRadius && py < borderRadius) corner = vec2(float(resolutionx)-borderRadius, borderRadius);
    else if(px < borderRadius && py > float(resolutiony) - borderRadius) corner = vec2(borderRadius, float(resolutiony)-borderRadius);
    else if(px > float(resolutionx) - borderRadius && py > float(resolutiony) - borderRadius) corner = vec2(float(resolutionx)-borderRadius, float(resolutiony)-borderRadius);

    if(corner.x >= 0.0) {
        float dist = length(vec2(px, py) - corner);
        if(dist <= borderRadius) {
            if(borderActive && dist >= borderRadius - borderWidth) drawBorder = true;
        } else {
            insideCorner = false;
        }
    }

    if(drawBorder) fragColor = borderColor;
    else if(!insideCorner) fragColor = bgColor;
    else fragColor = tex;
}
