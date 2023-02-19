#pragma once

#include "base/file.h"
#include "base/exporter.h"
#include "base/importer.h"

#include "core/linkedpreprocessline.h"

#include "utilities/linkedvar.h"
#include "utilities/modpriority.h"
#include "utilities/animtemplate.h"
#include "utilities/animqueryfile.h"

#include "generate/hkxcompiler.h"

#include "hkx/base/hkxparserbase.h"

namespace nemesis
{
    struct Template;
    struct QueryManager;
    struct Patch;

    struct HkxBehavior : public nemesis::File
    {
    private:
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

        struct Node
        {
            Vec<SPtr<nemesis::LinkedPreprocessLine>>* list;
            nemesis::LinkedPreprocessLine* start;
            nemesis::LinkedPreprocessLine* ending;

            nemesis::LinkedPreprocessLine* operator[](size_t index);
            size_t size() const;

            Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator begin();
            Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator end();

            Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator begin() const;
            Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator end() const;

            Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator
            insert(Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator itr,
                        SPtr<nemesis::LinkedPreprocessLine> value);

            void push_back(SPtr<nemesis::LinkedPreprocessLine> lnkline_ptr);
        };

        struct Parser : public nemesis::HkxParserBase
        {
        private:
            using Base = nemesis::HkxParserBase;

            enum Scope
            {
                WITHIN_SCOPE = -1,
                OUT_OF_SCOPE = 0,
            };

            using RtnFunc = std::function<std::string(nemesis::ScopeInfo&)>;

            nemesis::HkxBehavior& hostref;
            UPtr<nemesis::ProcessParser> processparser;
            Node* node_ptr = nullptr;

            std::string currentid_late;
            std::string currentid;
            std::string smachineid;
            std::string nodeclass;

            int bonenum    = 0;
            int oribone    = 0;
            std::string rigfile;

            bool findnewnodezone = true;
            bool newbone         = false;
            bool bonefound       = false;

            bool process_active = true;
            bool state_active   = true;
            bool topnode_active = true;

            USet<const nemesis::TemplateClass*> templateclasses;

            USet<const nemesis::TemplateClass*>& GetTemplateClasses();

            bool TrySetNewNodeZone();

            bool TryGetNodeId(const std::string& line, std::string& nodeid);
            void TrySaveTopNodeId(const std::string& line);

            void TryCacheStateInfo(const std::string& line);
            void TryCacheStateNodeId(const std::string& line);
            void TryCacheStateId(const std::string& line);

            void TryCacheStateMachineInfo(const std::string& line);
            void TryCacheTemplateSMInfo(const std::string& line, const nemesis::ConditionInfo* conditioninfo);
            void TryCacheStateMachineNodeId(const std::string& line);
            void TryCacheStateMachineNum(const std::string& line);
            void TryCacheStateMachineId(const std::string& line);

            void BonePatch(const std::filesystem::path& rigfile, int oribone);
            bool TryPatchBone(const nemesis::Line& line);

            void UpdateHightStateId();

            void GetFileLines();

            std::string GetRegexClassNames();

            void PrepareAllRegexes();
            void PrepareStateIdRegex();
            void PrepareNodeIdRegex();
            void PrepareAnimObjRegex();
            
            void PrepareAllLexers();
            void PrepareLexer(const std::string& keyword, const std::string& classname, RtnFunc callback);
            void PrepareVariableLexer(const nemesis::TemplateClass* templtclass);

            void TryUpdateNode();
            void AddStartNode();
            void TryEndNode();

            void ParseProcesses(const nemesis::Line& line);

        protected:
            void ParseLines(const VecNstr& storeline) override;
            void TryCacheData(const nemesis::Line& line,
                              const nemesis::ConditionInfo* conditioninfo) override;
            void PostAddLineProcess(nemesis::LinkedPreprocessLine& linkedline) override;
            void AddLine(const nemesis::Line& line) override;

        public:
            Parser(nemesis::HkxBehavior& host, const VecStr& behaviorpriotiy);
            ~Parser();

            void SetParseProcess(bool is_active) noexcept;
            void SetParseState(bool is_active) noexcept;
            void SetParseTopNode(bool is_active) noexcept;

            void ReadFile(const std::filesystem::path& filepath) override;
            void ParseFile() override;

            using funcptr = void (nemesis::HkxBehavior::Parser::*)(const std::string&);

        private:
            funcptr cachestate = &nemesis::HkxBehavior::Parser::TryCacheStateNodeId;
            funcptr cachestatemachine = &nemesis::HkxBehavior::Parser::TryCacheStateMachineNodeId;

            static const nemesis::regex statergx_id;
            static const nemesis::regex statergx;
            static const nemesis::regex smachinergx_id;
            static const nemesis::regex smachinergx_list;

            static constexpr std::string_view ignore = "SERIALIZE_IGNORED";
            static constexpr std::string_view end    = "\t</hksection>";

            static constexpr std::string_view smachine_num   = "<hkparam name=\"states\" numelements=\"";
            static constexpr std::string_view smachine_close = "\t</hkparam>";

            static constexpr std::string_view statemachineinfo_str = "\"hkbStateMachineStateInfo\"";
            static constexpr std::string_view stateid_str          = "stateId";
            static constexpr std::string_view statemachine_str     = "\"hkbStateMachine\"";
            static constexpr std::string_view hkobj_prefix         = "<hkobject name=\"";
            static constexpr std::string_view hkparam_prefix       = "<hkparam name=\"";

            static constexpr std::string_view parentindices
                = R"(<hkparam name="parentIndices" numelements=")";
            static constexpr std::string_view rigname = R"(<hkparam name="rigName">)";
            static constexpr std::string_view bonemap = R"(<hkparam name="bonePairMap" numelements=")";
        };

        struct Exporter : nemesis::Exporter
        {
        private:
            using funcptr = void (nemesis::HkxBehavior::Exporter::*)(const nemesis::Line&);

            VecNstr& linesref;
            UMap<const nemesis::AnimQuery*, UMap<size_t, size_t>> nodeidmap;
            std::atomic_flag id_atomic_flag{};

            size_t eventcounter = 0;
            size_t varcounter   = 0;
            bool isdata       = false;
            bool isevent      = true;

            const nemesis::HkxBehavior& GetSelf();

            void ProcessDataLine(const nemesis::Line& line);

            void CompileBehavior(VecNstr& datalines, VecNstr& behaviorlines);
            void CompileTemplate(VecNstr& templines);
            void CompileImport(VecNstr& importlines);

            void AddStateIdManager();

        public:
            Exporter(const HkxBehavior& host, VecNstr& storeline);

            const nemesis::AnimTemplate* GetAnimTemplate() override;
            const nemesis::TemplateClass* GetTemplateClass(const std::string& name) override;
            void Export() override;
            bool IsTemplateActive(const std::string& name) override;
            const Vec<const nemesis::AnimQuery*>* GetQueriesByTemplate(const std::string& name) override;

            static bool IsDataClass(const std::string& classname);
            static bool IsNewClass(const nemesis::Line& line, std::string& classname);

        private:
            static const USet<std::string> dataclasses;
            static const nemesis::regex datastr_rgx;

            static constexpr std::string_view eventelem = R"(<hkparam name="eventNames" numelements=)";
            static constexpr std::string_view charpropelem
                = R"(<hkparam name="characterPropertyNames" numelements=)";
            static constexpr std::string_view varelem      = R"(<hkparam name="variableNames" numelements=)";
            static constexpr std::string_view hkobj_prefix = R"(<hkobject name=")";
            static constexpr std::string_view hkxstr_str   = "<hkcstring>";
        };

        using VecAnimQuery = Vec<const nemesis::AnimQuery*>;
        
        nemesis::Line rootnode;
        UMap<std::string, size_t> stateids;
        UMap<std::string, size_t> topstate;           // state machine node id
        UMap<std::string, VecStr> statelinks;       // state machine node id, template node id list
        const nemesis::AnimTemplate* animtemplate = nullptr;

        nemesis::Atomic<size_t> topnodeid = 0;
        UMap<std::string, Vec<Pair<std::string, size_t*>>> statemachines;
        Vec<SPtr<nemesis::LinkedPreprocessLine>> lines;
        Vec<nemesis::LinkedPreprocessLine*> lines_ptr;

        nemesis::LinkedPreprocessLine* newnode_zone = nullptr;
        UMap<std::string, Node> node_map;
        Vec<const nemesis::Template*> templatelist;
        Vec<const nemesis::Template*> animimportlist;
        UMap<const nemesis::Template*, VecAnimQuery> queries;
        const Vec<const nemesis::AnimQueryFile*>* queryfiles = nullptr;

        SPtr<nemesis::EventList> events;
        SPtr<nemesis::VarList> variables;
        SPtr<nemesis::AttrList> attibutes;
        BhvSlctInfo behaviorinfo;
        VecNstr behaviorheaders;

        bool isfirstperson = false;
        bool ischaracter   = false;
        bool sse           = false;

        // Prevent creating object from outside of ReadFile
        HkxBehavior();

        void CompileBehavior(VecNstr& contents);

        nemesis::HkxBehavior::Node* TryGetNodeExist(const nemesis::Patch& patch);
        void AddNewNode(const nemesis::Patch& patch);
        void PatchNode(nemesis::HkxBehavior::Node& node, const nemesis::Patch& patch);

        SPtr<nemesis::LinkedPreprocessLine> CreateCondLine(const nemesis::Patch& patch);
        SPtr<nemesis::LinkedPreprocessLine> CreateCondLine(const nemesis::Line& condition,
                                                           nemesis::CondType type, const nemesis::File& file);
        SPtr<nemesis::LinkedPreprocessLine> CreateCondLine(const std::string& condition,
                                                           nemesis::CondType type, const nemesis::File& file);

    public:
        std::wstring GetBehaviorName() const;
        const nemesis::AnimTemplate* GetAnimTemplate() const noexcept;

        void PatchTemplateNode(nemesis::Template& templt);

        void AddStateId(const std::string& smid, size_t id);
        void AddSelection(const VecStr& behaviorlist);
        void ClearSelection();
        void AddQueries(const VecSPtr<const nemesis::AnimQueryFile>& animqueries);
        void AddQueries(const QueryManager& query_manager);
        void AddQueriesByFile(const nemesis::AnimQueryFile& queryfile,
                              USet<const nemesis::Template*>& templateset);
        void AddPatch(const nemesis::Patch& patch);
        void ClearQueries() noexcept;
        void AddTemplate(const nemesis::Template& ntemplate);
        void ClearTemplateList() noexcept;
        void AddAnimImport(const nemesis::Template& animimport);
        void ClearAnimImportList() noexcept;
        void SetToFirstPerson();
        void SetToCharacter();
        void SetToSSE();
        void SetAnimTemplate(nemesis::AnimTemplate* animtemplate) noexcept;
        void GetRawBehavior(VecNstr& storeline);
        void SaveStagedBehavior(const std::filesystem::path& outputpath);
        void SaveAs(const std::filesystem::path& outputpath, bool compile = false);
        void SaveAsHkx(const std::filesystem::path& xmlpath);
        void SaveAsStaged(const std::filesystem::path& xmldir);
        void SaveAsTemplateBehavior();
        void ParseBehavior(const VecStr& behaviorpriority);

        bool IsFirstPerson() const;
        bool IsCharacter() const;
        bool IsSSE() const;

        static SPtr<nemesis::HkxBehavior> File(const std::filesystem::path& filepath);
        static SPtr<nemesis::HkxBehavior> ReadPatchedFile(const std::filesystem::path& filepath,
                                                          const VecStr& behaviorpriority);

        //static void AddFileLines();

    private:
        static const std::string xmlstart;
        static const std::string xmlend;
        static const std::wstring xmlext;
        static const nemesis::regex nodergx_id;

        friend struct StateIdManager;
    };
}
