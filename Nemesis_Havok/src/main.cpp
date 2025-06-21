#include <functional>
#include <iostream>
#include <chrono>
#include <algorithm>

#include "Serialize/PackfileDeserializer.h"
#include "Serialize/PackfileSerializer.h"

#include "Serialize/XmlDeserializer.h"
#include "Serialize/XmlSerializer.h"

#include "Enums/HavokVersion.h"

constexpr std::string_view version("1.0.0");

UMap<std::string, nemesis::PlatformType> PlatformMap = {{"win32", nemesis::PlatformType::WIN32},
                                                        {"amd64", nemesis::PlatformType::AMD64},
                                                        {"ps3", nemesis::PlatformType::PS3},
                                                        {"ps4", nemesis::PlatformType::PS4},
                                                        {"xb360", nemesis::PlatformType::XB360}};

template <typename Func>
void benchmark(Func&& func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
}

void print_help(const std::string& program_name)
{
    std::cout << "Nemesis Havok Compiler made by Shikyo Kira v" << version << "\n"
              << "This program will compile or decompile hkx file to/from xml file"
              << "Usage:\n"
              << "     " << std::filesystem::path(program_name).filename().string()
              << " -t:<output_type> -v:<version> <input_path> <output_path>\n\n"
              << "Arguments:\n"
              << "     <output_type>   Output format: win32, amd64, xml, ps3, ps4, xb360\n"
              << "     <version>       One of the supported versions\n"
              << "     <input_path>    Path to the input file\n"
              << "     <output_path>   Path where output will be written\n\n"
              << "Example:\n"
              << "     " << std::filesystem::path(program_name).filename().string()
              << " -t:win32 -v:hk_2010.2.0-r1 input.xml output.hkx\n\n"
              << "Use --help to show this message.\n";
}

bool is_valid_output_type(const std::string& type)
{
    return PlatformMap.find(type) != PlatformMap.end();
}

std::string to_lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) { return std::tolower(ch); });
    return str;
}

int main(int argc, char* argv[])
{
    if (argc != 5 || to_lower(argv[1]) == "--help")
    {
        print_help(argv[0]);
        return 1;
    }

    std::string output_type, version;
    std::string input_path = argv[3], output_path = argv[4];

    for (size_t i = 0; i < 3; i++)
    {
        std::string arg = argv[i];

        if (arg.find("-t:") == 0)
        {
            output_type = to_lower(arg.substr(3));
            continue;
        }
        
        if (arg.find("-v:") != 0) continue;

        version = to_lower(arg.substr(3));
    }

    try
    {
        if (output_type.empty() || !is_valid_output_type(output_type))
        {
            throw std::runtime_error("Invalid output type. Only win32, amd64 and xml are supported");
        }

        nemesis::HavokVersion hk_ver;

        if (version.empty()) throw std::runtime_error("Invalid havok version");

        hk_ver = nemesis::GetEnumVersion(version);
        std::ifstream in(input_path, std::ios::binary);

        if (!in.is_open())
        {
            std::error_code ec(errno, std::system_category());
            throw std::runtime_error("Failed to open file: \"" + input_path + "\"\nMessage: " + ec.message());
        }

        constexpr std::int64_t magic = 0x57E0E05710C0C010;
        std::int64_t magic_num;
        in.read(reinterpret_cast<char*>(&magic_num), sizeof(std::int64_t));

        if (in.gcount() < sizeof(std::int64_t))
        {
            throw std::runtime_error("Unsupported input file: \"" + input_path + "\"");
        }

        in.close();

        std::function<nemesis::hkPackfile()> get_packfile;
        std::function<void(const nemesis::hkPackfile&)> to_output;

        if (magic_num == magic)
        {
            get_packfile = [input_path]()
            {
                nemesis::PackfileDeserializer pf_desr;
                pf_desr.LoadFile(input_path);
                return pf_desr.Deserialize();
            };
        }
        else
        {
            get_packfile = [input_path]()
            {
                nemesis::XmlDeserializer xml_desr;
                xml_desr.LoadFile(input_path);
                return xml_desr.Deserialize();
            };
        }

        if (output_type == "xml")
        {
            to_output = [output_path, hk_ver](const nemesis::hkPackfile& packfile)
            {
                nemesis::XmlSerializer xml_ser(hk_ver);
                xml_ser.Serialize(packfile);
                xml_ser.Save(output_path);
            };
        }
        else
        {
            nemesis::PlatformType plt_type = PlatformMap[output_type];

            to_output = [output_path, plt_type, hk_ver](const nemesis::hkPackfile& packfile)
            {
                nemesis::PackfileSerializer pf_ser(plt_type, hk_ver);
                pf_ser.Serialize(packfile);
                pf_ser.Save(output_path);
            };
        }

        benchmark(
            [get_packfile, to_output]
            {
                auto packfile = get_packfile();
                to_output(packfile);
            });
        exit(0);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
