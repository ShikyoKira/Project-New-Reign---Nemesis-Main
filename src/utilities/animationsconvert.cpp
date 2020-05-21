#include <filesystem>
#include <stdexcept>

#include "utilities/animationsconvert.h"

AnimationsConvert::AnimationsConvert()
{
    // Need to have memory allocated for the solver. Allocate 1mb for it.
    _memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator,
                                                  hkMemorySystem::FrameInfo(1024 * 1024));
    hkBaseSystem::init(_memoryRouter, errorReport);

    LoadDefaultRegistry();
}

AnimationsConvert::~AnimationsConvert()
{
    hkBaseSystem::quit();
    hkMemoryInitUtil::quit();
}

void AnimationsConvert::convert(std::string_view filePath, const hkPackFormat &pkFormat) const
{
    auto flags = static_cast<hkSerializeUtil::SaveOptionBits>(hkSerializeUtil::SAVE_DEFAULT);

    if (pkFormat == HKPF_DEFAULT)
        flags = static_cast<hkSerializeUtil::SaveOptionBits>(hkSerializeUtil::SAVE_TEXT_FORMAT
                                                             | hkSerializeUtil::SAVE_TEXT_NUMBERS);
    if (pkFormat == HKPF_XML || pkFormat == HKPF_TAGXML) // set text format to indicate xml
        flags = static_cast<hkSerializeUtil::SaveOptionBits>(flags
                                                             | hkSerializeUtil::SAVE_TEXT_FORMAT);

    auto packFileOptions = GetWriteOptionsFromFormat(pkFormat);

    hkIstream istream(filePath.data());
    if (!istream.isOk())
        throw std::runtime_error("An error occured while loading this file.");

    hkStreamReader *reader = istream.getStreamReader();
    hkVariant root;
    hkResource *resource;

    hkResult res = hkSerializeLoad(reader, root, resource);

    auto chopped = filePath;
    filePath.remove_suffix(3);
    std::string outHkx = std::string(chopped) + ".out.hkx";

    if (res)
        throw std::runtime_error(std::string("File is not loadable: ") + std::string(filePath)
                                 + std::string("\nIt is probably already converted."));
    else {
        if (root.m_object != nullptr) {
            //Outputting the file to a different location
            hkOstream ostream((outHkx + "out.hkx").c_str());

            if (!ostream.isOk())
                throw std::runtime_error("An error occured while saving this file.");

            res = hkSerializeUtilSave(pkFormat, root, ostream, flags, packFileOptions);

            if (res) {
                throw std::runtime_error(std::string("Failed to save file: ")
                                         + std::string(filePath));
            }
        } else
            throw std::runtime_error(std::string("Failed to load file: ") + std::string(filePath));
    }
    if (resource != nullptr)
        resource->removeReference();

    //Renaming the file to the correct name

    if (std::filesystem::exists(outHkx)) {
        std::filesystem::remove(filePath);
        std::filesystem::rename(outHkx, filePath);
    }
}
