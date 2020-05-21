#pragma once

#include <hkfutils.h>
#include <string_view>

class AnimationsConvert final
{
public:
    AnimationsConvert();
    ~AnimationsConvert();
    void convert(std::string_view filePath, const hkPackFormat &pkFormat) const;

private:
    static hkMemoryRouter *_memoryRouter;
};

static void convertAnimation(std::string_view filePath, const hkPackFormat &pkFormat)
{
    static AnimationsConvert converter;
    converter.convert(filePath, pkFormat);
}
