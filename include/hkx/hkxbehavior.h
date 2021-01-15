#pragma once

#include <unordered_set>

#include "hkx/hkxnode.h"

#include "utilities/linkedvar.h"
#include "utilities/modpriority.h"
#include "utilities/animtemplate.h"
#include "utilities/animquerylist.h"

#include "generate/hkxcompiler.h"

namespace nemesis
{
    struct Template;

    struct HkxBehavior
    {
    private:
        using LinkedNode = nemesis::LinkedVar<HkxNode>;

        struct BhvSlctInfo
        {
            USet<std::string> behaviorset;
            VecStr behaviorlist;

            void Clear()
            {
                behaviorset.clear();
                behaviorlist.clear();
                behaviorlist.shrink_to_fit();
            }
        };

        struct Parser
        {
        private:
            enum Scope
            {
                WITHIN_SCOPE = -1,
                OUT_OF_SCOPE = 0,
            };

            UPtr<ConditionScope> cscope;
            Vec<LinkedNode>& stream;
            HkxBehavior& hostref;
            std::filesystem::path filepath;

            std::pair<SPtr<nemesis::ConditionInfo>, const nemesis::Line*> astercache;
            SPtr<nemesis::ConditionInfo> openmodcondinfo;

            UPtr<ModPriority> priorities;

            bool success   = true;
            bool newbone   = false;
            bool bonefound = false;
            int bonenum    = 0;
            int oribone    = 0;
            std::string rigfile;

            void BonePatch(const std::filesystem::path& rigfile, int oribone);
            bool TryPatchBone(const nemesis::Line& line);
            void ReadBehaviorFile(VecNstr& storeline);
            void ParseLine(const nemesis::Line& line);
            void ParseLines(const VecNstr& storeline);
            void AddNewNode(const nemesis::Line& line);
            void AddLine(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& conditioninfo);
            bool HasConditionMet(const nemesis::Line& line,
                                 const SPtr<nemesis::ConditionInfo>& conditioninfo);
            void AsteriskCondition(const nemesis::Line& line);
            void ModCodeCondition(const SPtr<nemesis::ConditionInfo>& conditioninfo);
            void ModCloseCondition(const SPtr<nemesis::ConditionInfo>& tobedeleted);
            void LowerOriginal(const nemesis::Line& line,
                                      const SPtr<nemesis::ConditionInfo>& conditioninfo);
            void ResetAsterCache();
            void AddToAsterCache(const nemesis::Line* line, SPtr<nemesis::ConditionInfo> conditioninfo);

        public:
            Parser(const std::string& modcode, const std::filesystem::path& filepath, HkxBehavior& host);
            ~Parser();

            void GenerateNodes(const VecStr& behaviorpriority);

        private:
            const static std::string_view ignore;
            const static std::string_view signature;
            const static std::string_view end;
            const static std::string_view start;
        };

        struct Exporter
        {
        private:
            const HkxBehavior& hostref;
            VecNstr& linesref;

            void CompileNodes();
            void CompileNode(const LinkedNode& linkednode);

        public:
            Exporter(const HkxBehavior& host, VecNstr& storeline);

            void Compile();
        };

        std::filesystem::path filepath;
        nemesis::Line rootnode;
        Vec<LinkedNode> nodes;
        VecSPtr<nemesis::Template> templatelist;
        VecStr events;
        VecStr variables;
        VecStr attributes;
        BhvSlctInfo behaviorinfo;
        VecSPtr<nemesis::AnimQueryList> queries;

        bool isfirstperson = false;
        bool ischaracter   = false;
        bool sse           = false;

        // Prevent creating object from outside of ReadFile
        HkxBehavior() = default;

        void CompileBehavior(VecNstr& storeline);

    public:
        std::filesystem::path GetFilePath() const;
        std::wstring GetBehaviorName() const;

        void AddSelection(const VecStr& behaviorlist);
        void ClearSelection();
        void AddQueries(const VecSPtr<nemesis::AnimQueryList>& queries);
        void ClearQueries();
        void AddTemplateList(const VecSPtr<nemesis::Template> templatelist);
        void ClearTemplateList();
        void SetToFirstPerson();
        void SetToCharacter();
        void SetToSSE();
        void getlines(VecNstr& storeline);
        void SaveStagedBehavior(const std::filesystem::path& outputpath);
        void SaveAsXml(const std::filesystem::path& outputpath);
        void SaveAsHkx(const std::filesystem::path& outputpath);
        void ReadAndProcess(const VecStr& behaviorpriority);

        bool IsFirstPerson() const;
        bool IsCharacter() const;
        bool IsSSE() const;

        static SPtr<nemesis::HkxBehavior> File(const std::filesystem::path& filepath);
        static SPtr<nemesis::HkxBehavior> ReadPatchedFile(const std::filesystem::path& filepath,
                                                          const VecStr& behaviorpriority);

        static void AddFileLines();

    private:
        const static std::string xmlstart;
        const static std::string xmlend;
        const static std::wstring xmlext;
    };
}
