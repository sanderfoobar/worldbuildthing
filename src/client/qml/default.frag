#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec2 pixelStep;
    int radius;
    float deviation;
};
layout(binding = 1) uniform sampler2D src;

void main(void)
{
    vec2 uv = vec2(qt_TexCoord0.x, 1.0 - qt_TexCoord0.y);  // flip-Y
    vec4 tex = texture(src, uv);
    fragColor = tex;
}
