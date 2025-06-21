#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    vec4 cornerColor;
    float radius;
    float itemWidth;
    float itemHeight;
    float backgroundOpacity;
};

void main() {
    vec2 fragPos = qt_TexCoord0 * vec2(itemWidth, itemHeight);

    if (fragPos.x < radius && fragPos.y < radius) {
        float dx = radius - fragPos.x;
        float dy = radius - fragPos.y;

        if (dx*dx + dy*dy > radius * radius) {
            fragColor = cornerColor * backgroundOpacity;
        }
        else {
            discard;
        }
    }
    else if (fragPos.x > (itemWidth - radius) && fragPos.y < radius) {
        float dx = fragPos.x - (itemWidth - radius);
        float dy = radius - fragPos.y;

        if (dx * dx + dy * dy > radius * radius) {
            fragColor = cornerColor * backgroundOpacity;
        }
        else {
            discard;
        }
    }
    else if (fragPos.x < radius && fragPos.y > (itemHeight - radius)) {
        float dx = radius - fragPos.x;
        float dy = fragPos.y - (itemHeight - radius);

        if (dx * dx + dy * dy > radius * radius) {
            fragColor = cornerColor * backgroundOpacity;
        }
        else {
            discard;
        }
    }
    else if (fragPos.x > (itemWidth - radius) && fragPos.y > (itemHeight - radius)) {
        float dx = fragPos.x - (itemWidth - radius);
        float dy = fragPos.y - (itemHeight - radius);

        if (dx * dx + dy * dy > radius * radius) {
            fragColor = cornerColor * backgroundOpacity;
        }
        else {
            discard;
        }
    }
    else {
        discard;
    }
}
