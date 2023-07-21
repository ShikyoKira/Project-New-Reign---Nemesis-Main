#pragma once

#include "base/file.h"
#include "base/exporter.h"
#include "base/importer.h"

#include "animsetdata/linked/animsetdata-linkedproject.h"

namespace nemesis
{
	namespace animsetdata
    {
        struct SingleFile : nemesis::File
        {
        private:
            class Parser : public nemesis::Importer<nemesis::animsetdata::LinkedProject>
            {
                nemesis::animsetdata::SingleFile& file;

            public:
                Parser(nemesis::animsetdata::SingleFile& file);

                //void UpdateConditionScope(const nemesis::ConditionInfo* conditioninfo) override
                //{
                //    SPtr<nemesis::animsetdata::LinkedProject>& ptr = stream.at(stream.size() - 2)->back();
                //    ptr->AddCondition(*conditioninfo, file);
                //    stream.back() = &ptr->GetLastCondition().GetDataList();
                //}
            };

            struct Exporter : nemesis::Exporter
            {
            private:
                VecNstr lines;
                const nemesis::animsetdata::SingleFile& singlefile;

            public:
                Exporter(const nemesis::animsetdata::SingleFile& file);

                const VecNstr GetLines() const;

                const nemesis::AnimTemplate* GetAnimTemplate() override;
                const nemesis::TemplateCategory* GetTemplateCategory(const std::string& name) override;
                void Export() override;
                bool IsTemplateActive(const std::string& name) override;
                const Vec<const nemesis::AnimQuery*>* GetQueriesByTemplate(const std::string& name) override;
            };

            Vec<nemesis::animsetdata::LinkedProject> projects;
            const nemesis::AnimTemplate* animtemplate = nullptr;
            UMap<const nemesis::Template*, Vec<const nemesis::AnimQuery*>> queries;

        public:
            const nemesis::AnimTemplate* GetAnimTemplate() const noexcept;

            void SaveAs(const std::filesystem::path& path) const;
            void CompileAs(const std::filesystem::path& path) const;
        };
    }
}
