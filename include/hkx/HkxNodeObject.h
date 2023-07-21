#pragma once

#include "utilities/linkedvar.h"
#include "utilities/conditionscope.h"

namespace nemesis
{
    struct HkxBehaviorFile;

	struct HkxNodeObject
    {
    private:
        struct Exporter
        {
        private:
            const HkxNodeObject& hostref;
            VecNstr& lines;

        public:
            Exporter(const HkxNodeObject& host, VecNstr& lines);
            void Compile(VecNstr& storeline);
            SPtr<nemesis::Line> TryProcessLine(const nemesis::LinkedVar<nemesis::Line>& line);
        };

        const nemesis::Line id;
        const nemesis::HkxBehaviorFile& parent;
        Vec<nemesis::LinkedVar<nemesis::Line>> lines;
        Vec<Vec<nemesis::LinkedVar<nemesis::Line>>*> stream;
        Vec<nemesis::LinkedVar<nemesis::Line>> modscope;

    public:
        HkxNodeObject(const nemesis::Line& _id, const nemesis::HkxBehaviorFile& _parent);

        void InitializeStream();
        void AddLine(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& conditioninfo);
        void Compile(VecNstr& storeline) const;
        void getlines(VecNstr& storeline) const;

        static std::string_view GetIDView(std::string_view line);
        static std::string GetID(const std::string& line);
    };
} // namespace nemesis

void getLinkedLines(const nemesis::LinkedVar<nemesis::HkxNodeObject>& linkedline, VecNstr& storeline);
