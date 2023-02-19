#pragma once

#include "base/importer.h"

namespace nemesis
{
    struct HkxParserBase : public nemesis::Importer<nemesis::LinkedPreprocessLine>
    {
        using Base = nemesis::Importer<nemesis::LinkedPreprocessLine>;

        struct Vector4Info
        {
            Vec<nemesis::Line> elements;
            nemesis::Line prefix;
            nemesis::Line suffix;

            bool ishkx = false;

            static UPtr<Vector4Info> TryCreate(nemesis::LinkedPreprocessLine& linkedline);
        };

    protected:
        bool read_numele = false;

        virtual bool IsElement(std::string_view vline);
        virtual bool IsMultiElement(nemesis::LinkedPreprocessLine& linkedline);
        virtual void AddMultiElementLine(nemesis::LinkedPreprocessLine& linkedline);
        virtual UPtr<nemesis::HkxParserBase::Vector4Info>
        TryGetVectorInfo(nemesis::LinkedPreprocessLine& linkedline);
        virtual void AddLineWithVector(nemesis::LinkedPreprocessLine& linkedline,
                                       nemesis::HkxParserBase::Vector4Info& vec4info);

        void AddLinkedLine(SPtr<nemesis::LinkedPreprocessLine> linkedline) override;

    protected:
        HkxParserBase(nemesis::File& file);

    protected:
        static constexpr std::string_view numele_str         = " numelements=\"";
        static constexpr std::string_view paramstart_str     = "\t\t\t<hkparam name=\"";
        static constexpr std::string_view paramend_str       = "\t\t\t</hkparam>";
        static constexpr std::string_view paramend_str_nospc = "</hkparam>";

        static constexpr std::string_view element_spaces = "\t\t\t\t";
        static constexpr std::string_view nodeid_ele     = "\t\t\t\t#";
        static constexpr std::string_view digit0_ele     = "\t\t\t\t0.";
        static constexpr std::string_view digit1_ele     = "\t\t\t\t1.";

        static constexpr std::string_view vec4_prefix = "\t\t\t\t(";
        static constexpr std::string_view vec4_suffix   = ")";

        static constexpr std::string_view hkxvec_prefix = "\">(";
        static constexpr std::string_view hkxvec_suffix = ")</hkparam>";
    };
}
