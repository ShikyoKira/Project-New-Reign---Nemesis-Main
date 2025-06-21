#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    vec4 solidColor;
    float maxOpacity;
    float minOpacity;
    float fadeLength;
    float widthRatio;
    float itemWidth;
    float itemHeight;
    float radius;
};

void main() {
    vec2 fragPos = qt_TexCoord0 * vec2(itemWidth, itemHeight);
    float maxItemWidth = itemWidth / widthRatio;

    if (fragPos.x < radius && fragPos.y < radius) {
        float dx = radius - fragPos.x;
        float dy = radius - fragPos.y;

        if (dx*dx + dy*dy > radius * radius) {
            discard;
        }
    }
    else if (fragPos.x > (maxItemWidth - radius) && fragPos.y < radius) {
        float dx = fragPos.x - (maxItemWidth - radius);
        float dy = radius - fragPos.y;

        if (dx * dx + dy * dy > radius * radius) {
            discard;
        }
    }
    else if (fragPos.x < radius && fragPos.y > (itemHeight - radius)) {
        float dx = radius - fragPos.x;
        float dy = fragPos.y - (itemHeight - radius);

        if (dx * dx + dy * dy > radius * radius) {
            discard;
        }
    }
    else if (fragPos.x > (maxItemWidth - radius) && fragPos.y > (itemHeight - radius)) {
        float dx = fragPos.x - (maxItemWidth - radius);
        float dy = fragPos.y - (itemHeight - radius);

        if (dx * dx + dy * dy > radius * radius) {
            discard;
        }
    }

    float x = qt_TexCoord0.x;

    float alpha = maxOpacity;

    if (fadeLength / widthRatio <= 1 - x) {
        alpha = minOpacity;
    }
    else {
        float delta = maxOpacity - minOpacity;
        delta *= (1 - x) / (fadeLength / widthRatio);
        alpha = maxOpacity - delta;
    }

    fragColor = solidColor * alpha;
}
