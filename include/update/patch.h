#pragma once

#include "base/importer.h"

#include "core/preprocessline.h"
#include "core/linkedpreprocessline.h"

#include "template/processparser.h"

#include "hkx/base/hkxparserbase.h"

namespace nemesis
{
    struct HkxBehaviorFile;

    struct Patch : public nemesis::File
    {
    protected:
        struct PatchParser : public nemesis::HkxParserBase
        {
        protected:
            Patch& patch_ref;
            Deq<SPtr<nemesis::LinkedPreprocessLine>> modlines_cache;
            SPtr<nemesis::ConditionInfo> modcondition_cache;

            bool IsModCodeScope();
            void AddAllModLines();

            void AddLinkedLine(SPtr<nemesis::LinkedPreprocessLine> linkedline) override;

            bool TryProcessCondition(const nemesis::ConditionInfo* conditioninfo) override;
            void ParseLine(const nemesis::Line& line) override;
            void ParseLines(const VecNstr& lines) override;

        public:
            PatchParser(nemesis::Patch& patch);

            void ReadFile(const std::filesystem::path& filepath) override;

            friend ProcessParser;
        };

        Vec<SPtr<nemesis::LinkedPreprocessLine>> contents;
        std::string code;

    public:
        Patch() = default;
        Patch(const std::string& code, const std::filesystem::path& filepath);

        VecNstr GetRawData() const;

        std::string_view GetModCode() const;

        virtual nemesis::CondType GetConditionType() const = 0;

        friend nemesis::HkxBehaviorFile;
    };
}
