#pragma once

#include "base/file.h"
#include "base/exporter.h"
#include "base/importer.h"

#include "animdata/linked/animdata-linkedproject.h"


namespace nemesis
{
	namespace animdata
    {
        struct SingleFile : nemesis::File
        {
        private:
            struct Exporter : nemesis::Exporter
            {
            private:
                VecNstr headers;
                VecNstr lines;
                const nemesis::animdata::SingleFile& singlefile;

            public:
                Exporter(const nemesis::animdata::SingleFile& file);

                const VecNstr GetLines() const;

                const nemesis::AnimTemplate* GetAnimTemplate() override;
                const nemesis::TemplateCategory* GetTemplateCategory(const std::string& name) override;
                void Export() override;
                bool IsTemplateActive(const std::string& name) override;
                const Vec<const nemesis::AnimQuery*>* GetQueriesByTemplate(const std::string& name) override;
            };

            Vec<nemesis::animdata::LinkedProject> projects;
            const nemesis::AnimTemplate* animtemplate = nullptr;
            UMap<const nemesis::Template*, Vec<const nemesis::AnimQuery*>> queries;

        public:
            void LoadFile(const std::filesystem::path& path);
            const nemesis::AnimTemplate* GetAnimTemplate() const noexcept;

            void SaveAs(const std::filesystem::path& path) const;
            void CompileAs(const std::filesystem::path& path) const;

        };
    }
}
