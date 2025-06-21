#version 440

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 vTexCoord;

layout(std140, binding = 0) uniform buf {
    float blurAmount;
    mat4 qt_Matrix;
};

void main() {
    vTexCoord = texCoord;
    gl_Position = qt_Matrix * vertex;
}
