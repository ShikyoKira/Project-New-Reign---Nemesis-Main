#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    float blurAmount;
    mat4 qt_Matrix;
};
layout(binding = 1) uniform sampler2D src;

void main() {
    float kernel[5] = float[](0.204164, 0.304005, 0.093913, 0.016216, 0.001279);

    vec2 texelSize = vec2(blurAmount, blurAmount); // in normalized UV space
    vec4 sum = vec4(0.0);

    for (int x = -4; x <= 4; ++x) {
        for (int y = -4; y <= 4; ++y) {
            int ax = abs(x);
            int ay = abs(y);
            float weight = kernel[ax] * kernel[ay];
            sum += texture(src, qt_TexCoord0 + vec2(x, y) * texelSize) * weight;
        }
    }

    fragColor = sum;
}
