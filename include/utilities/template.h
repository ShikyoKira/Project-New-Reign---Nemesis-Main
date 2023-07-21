#pragma once

#include "utilities/regex.h"

#include "base/file.h"
#include "base/exporter.h"

#include "core/linkedpreprocessline.h"

#include "hkx/base/hkxparserbase.h"

namespace nemesis
{
    struct Process;
    struct TemplateCategory;
    struct AnimQuery;
    struct HkxBehaviorFile;
    struct ScopeInfo;

    struct Template : public std::enable_shared_from_this<Template>, public nemesis::File
    {
    private:
        struct Parser : public nemesis::HkxParserBase
        {
        private:
            // Persistences at template level
            // They only last as long as the TEMPLATE being parsed
            nemesis::Template& rTemplate;

            std::filesystem::path GetFilePath() const noexcept;

            using RtnFunc = std::function<std::string(nemesis::ScopeInfo&)>;

            void GetFileLines();

            void PrepareAllRegexes();
            void PrepareStateIdRegex();
            void PrepareNodeIdRegex();
            void PrepareAnimObjRegex();

            void PrepareAllLexers();
            void PrepareVariableLexer();
            void PrepareLexer(const std::string& keyword,
                              RtnFunc callback);


        protected:
            void ParseLines(const VecNstr& storeline) override;
            void TryCacheData(const nemesis::Line& line,
                              const nemesis::ConditionInfo* conditioninfo) override;
            void AddConditionScope(const nemesis::ConditionInfo* conditioninfo) override;
            void PostAddLineProcess(nemesis::LinkedPreprocessLine& linkedline) override;

        public:
            Parser(nemesis::Template& host);

            void ReadFile(const std::filesystem::path& filepath) override;

            friend ProcessParser;
        };

        struct Exporter : nemesis::Exporter
        {
            VecNstr& storeline;

            const nemesis::Template& GetSelf();

        public:
            Exporter(const nemesis::Template& templateref, VecNstr& storeline);
            void ExportCurrentQuery(const nemesis::AnimQuery& query);

            const nemesis::AnimTemplate* GetAnimTemplate() override;
            const nemesis::TemplateCategory* GetTemplateCategory(const std::string& name) override;
            void Export() override;
            bool IsTemplateActive(const std::string& name) override;
            const Vec<const nemesis::AnimQuery*>* GetQueriesByTemplate(const std::string& name) override;
        };

    private:
        Vec<SPtr<nemesis::LinkedPreprocessLine>> contents;
        const nemesis::TemplateCategory* pTemplateCategory;
        mutable nemesis::HkxBehaviorFile* hkxbehavior;

        std::wstring behaviorfile;
        Vec<int> statemultiplier;

        Vec<nemesis::Template> parents;
        Vec<nemesis::Template> children;

        mutable UPtr<nemesis::Template::Exporter> exporter;

        void SetFilePath(const std::filesystem::path& behaviorfile);
        void SetBehaviorFile();
        void SetTemplateType();

        Template() = default;

    public:
        Template(const nemesis::TemplateCategory& templateclass) noexcept;     // for common template

        bool IsGroup() const noexcept;
        bool IsMaster() const noexcept;
        bool IsImport() const noexcept;
        FileType GetType() const noexcept;

        const Vec<SPtr<nemesis::LinkedPreprocessLine>> GetContents() const noexcept;
        std::wstring GetBehaviorFileW() const noexcept;
        const Vec<int>& GetStateMultiplier() const noexcept;
        const nemesis::HkxBehaviorFile& GetBehavior() const noexcept;
        const nemesis::TemplateCategory& GetTemplateCategory() const noexcept;

        void GetQueryResult(const nemesis::AnimQuery& query, VecNstr& storeline, nemesis::Exporter& exptr) const;
        void GetImportResult(VecNstr& storeline, nemesis::Exporter& exptr) const;

        void ReadFile(const std::filesystem::path& filepath);

        void AddBehavior(nemesis::HkxBehaviorFile& behavior) const noexcept;
        bool TryAddBehavior(nemesis::HkxBehaviorFile& behavior) const;
        bool TryAddBehaviorList(const VecSPtr<nemesis::HkxBehaviorFile>& behaviorlist) const;

        static SPtr<Template> CreateImport(const std::filesystem::path& filepath);

        static const char* GetImporterStr()
        {
            return "Import";
        }

        friend ProcessParser;
    };
}
