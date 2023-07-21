#pragma once

#include "base/file.h"

#include "core/animvalue.h"

#include "utilities/alphanum.hpp"
#include "utilities/regex.h"
#include "utilities/types.h"

namespace nemesis
{
    struct TemplateCategory;
    struct ScopeInfo;
    struct OptionModel;
    struct HkxBehaviorFile;
    struct OptionModelList;

    struct AnimVarPtr : public AnimValue
    {
        struct Parser;

        using RtnFunc = std::function<std::string(nemesis::ScopeInfo&)>;

        struct Lexer
        {
        private:
            std::string name;
            std::string varname;
            std::string grp_name;
            VecStr arguments;

            std::string formattedargs;

            Map<std::string, DeqStr, alphanum_less> cache_results;
            Map<std::string, DeqStr, alphanum_less> cache_group_results;

            std::string search_suf;
            std::string search_pref;

            RtnFunc result_func;

            void AddArguments() {}

            template <typename... Args>
            void AddArguments(const std::string& arg, const Args&... args)
            {
                arguments.emplace_back(arg);
                AddArguments(args...);
            }

            std::string GetFormattedArguments();

            void CacheGroupIndexSearch();
            void CacheVar();
            void CacheAnimVar();
            void CacheGroupAnimVar(const std::string& index);

            bool TryGetConstant(const std::string& line, DeqStr& components) const;

            static bool TryGetCacheResult(const std::string& line,
                                          const Map<std::string, DeqStr, alphanum_less>& cache,
                                          std::string& match,
                                          DeqStr& components,
                                          size_t& refpos);

            bool TryGetIndexGroupVar(std::string_view vline,
                                     std::string& match,
                                     DeqStr& components,
                                     size_t& refpos) const;

        public:
            struct Iterator
            {
            private:
                bool end = true;
                std::string line;
                std::string refline;
                const Lexer* lexer = nullptr;

                Iterator(const Lexer& lexer, const std::string& refline);

            public:
                size_t pos;
                std::string match;
                DeqStr groups;

                Iterator() = default;

                bool operator==(Iterator iter);
                bool operator!=(Iterator iter);
                Iterator& operator++();

                friend Lexer;
            };

            template <typename... Args>
            Lexer(const std::string& name, const std::string& varname, const Args&... args)
            {
                this->name    = name;
                this->varname = varname;
                AddArguments(args...);
                std::string formattedargs = GetFormattedArguments();

                CacheGroupIndexSearch();
                CacheVar();
                CacheAnimVar();
                CacheGroupAnimVar("");
                CacheGroupAnimVar("F");
                CacheGroupAnimVar("N");
                CacheGroupAnimVar("B");
                CacheGroupAnimVar("L");
            }

            void AddResultFunction(RtnFunc result_func) noexcept;
            RtnFunc GetResultFunc() const noexcept;

            bool TokenizeOnce(const std::string& line, Pair<std::string, DeqStr>& output) const;
            bool TokenizeOnce(const std::string& line, Iterator& itr) const;

            Iterator TokenizeString(const std::string& line) const;

            friend Parser;
        };

        struct Parser
        {
            DeqStr _components;
            DeqStr& components;
            std::string line;
            size_t linenum;
            const nemesis::File& fileref;

            UPtr<AnimVarPtr> animvar_uptr;
            AnimVarPtr* animvar_ptr = nullptr;

            bool b_custom_selector = false;

            using funcptr = bool (AnimVarPtr::Parser::*)();
            static const funcptr base_functions[];

            const std::string mae_str = "main_anim_event";

            Parser(DeqStr& components, size_t linenum, const nemesis::File& fileref);
            Parser(const std::string& expression,
                   size_t linenum,
                   const nemesis::File& fileref,
                   AnimVarPtr* target);

            void SetSelector(RtnFunc selector);
            UPtr<AnimVarPtr> Parse();

        private:
            bool IsFileModPatch();
            bool TryPopulateAsModPatch();

            void SetupTemplateCategory();

            bool TryPopulateConstant();

            bool TryPopulateFromMaster();

            bool TryPopulateFromGroup();
            void PopulateFromGroup();

            bool TryPopulateFromTemplate();
            void PopulateFromTemplate();

            bool TryPopulateFromOption();
            void PopulateFromOption(const nemesis::OptionModelList& model);

            bool TryPopulateVariable(const nemesis::OptionModel& model);
            void PopulateFromVariable(const nemesis::OptionModel& model);

            void PopulateElement();

            bool TestCustomSelector();

            bool TryAddAnimObject();
            void AddAnimObject(size_t groupnum);
            void AddAnimObjectIndex();

            bool TryAddStateId();
            void AddStateId();

            bool TryAddAsBase();
            bool BaseCheck();

            void AddAnimValue();
        };

    private:
        enum ErrorType
        {
            UNKNOWN_OPTION   = 1117,
            SYNTAX_ERROR     = 1223,
            UNKNOWN_VARIABLE = 1197,
        };

        struct AnimIndex
        {
            enum Order
            {
                NONE,
                LAST,
                NEXT,
                BACK,
                ALL,
                ANY,
                CURRENT,
            };

        private:
            UPtr<size_t> index = nullptr;
            Order order        = NONE;
            nemesis::AnimVarPtr& animvar;

        public:
            AnimIndex(const std::string& animindex, nemesis::AnimVarPtr& animvar);

            bool HasValue() const noexcept;
            size_t GetValue(nemesis::ScopeInfo& scopeinfo) const;

        private:
            static const UMap<char, Order> orderparser;
        };

        std::string templatename;

        bool b_allmaster = false;
        bool b_allgroup  = false;
        bool b_allanim   = false;
        bool b_base      = false;
        bool b_mae       = false;
        bool b_linked    = false;
        bool b_filepath  = false;
        bool b_modcode   = false;

        UPtr<std::string> constant = nullptr;
        UPtr<size_t> group         = nullptr;
        UPtr<AnimIndex> anim_index = nullptr;
        UPtr<std::string> option   = nullptr;
        std::string optionvar;
        UPtr<size_t> optionindex = nullptr;

        UPtr<size_t> animobj_grp   = nullptr;
        UPtr<size_t> animobj_index = nullptr;

        UPtr<size_t> stateid = nullptr;

        std::string raw_expression;

        const nemesis::TemplateCategory* templateclass = nullptr;

        // grp_result_ptr
        std::string GetResultFromConstant(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromBase(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromGroup(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromNoGroup(nemesis::ScopeInfo& scopeinfo) const;

        // anim_result_ptr
        std::string GetResultFromAnim(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromNoAnim(nemesis::ScopeInfo& scopeinfo) const;

        // opt_result_ptr
        std::string GetResultFromAnimObj(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromMainAnimEvent(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromStateId(nemesis::ScopeInfo& scopeinfo) const;

        std::string GetResultFromOption(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromNoOption(nemesis::ScopeInfo& scopeinfo) const;

        // opt_index_result_ptr
        std::string GetResultFromOptionWithIndex(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetResultFromOptionWithNoIndex(nemesis::ScopeInfo& scopeinfo) const;

        std::string GetFromAnim(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetFromOption(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetFromOptionIndex(nemesis::ScopeInfo& scopeinfo) const;

    public:
        AnimVarPtr() = default;
        AnimVarPtr(const std::string& expression, size_t linenum, const nemesis::File& fileref);
        AnimVarPtr(const std::string& expression,
                   size_t linenum,
                   const nemesis::File& fileref,
                   RtnFunc selector);

        std::string GetResult(nemesis::ScopeInfo& scopeinfo) const;
        bool IsExist(nemesis::ScopeInfo& scopeinfo) const;
        bool IsMatch(const nemesis::AnimVarPtr& animvarptr) const;

        bool IsConstant() const noexcept;
        bool IsAllMaster() const noexcept;
        bool IsAllGroup() const noexcept;
        bool IsAllAnim() const noexcept;
        bool IsBase() const noexcept;
        bool IsMainAnimEvent() const noexcept;
        bool IsModCode() const noexcept;

        bool HasGroup() const noexcept;
        bool HasAnim() const noexcept;
        bool HasOption() const noexcept;
        bool HasOptionVar() const noexcept;
        bool HasOptionIndex() const noexcept;
        bool HasAnimObj() const noexcept;
        bool HasAnimObjIndex() const noexcept;
        bool HasStateId() const noexcept;

        size_t GetGroup() const;
        size_t GetAnim(nemesis::ScopeInfo& scopeinfo) const;
        std::string GetOption() const;
        std::string GetOptionVar() const;
        size_t GetOptionIndex() const;
        size_t GetAnimObj() const;
        size_t GetAnimObjIndex() const;
        size_t GetStateId() const;

        const nemesis::TemplateCategory* GetTemplateCategory() const;

        std::string_view GetRawExpression() const;

        bool IsGroupExist(nemesis::ScopeInfo& scopeinfo) const;
        bool IsAnimExist(nemesis::ScopeInfo& scopeinfo) const;
        bool IsOptionExist(nemesis::ScopeInfo& scopeinfo) const;

        static std::string ParseClassName(const std::string& first_component);

        static const std::regex lexing_rgx;
        static const std::sregex_token_iterator end;
        static const nemesis::regex animobj_rgx;
        static const nemesis::regex constant_rgx;

        const std::string mae_str = "main_anim_event";

    private:
        using ResFuncPtr                = std::string (AnimVarPtr::*)(nemesis::ScopeInfo&) const;
        ResFuncPtr grp_result_ptr       = &nemesis::AnimVarPtr::GetResultFromNoGroup;
        ResFuncPtr anim_result_ptr      = &nemesis::AnimVarPtr::GetResultFromNoAnim;
        ResFuncPtr opt_result_ptr       = &nemesis::AnimVarPtr::GetResultFromNoOption;
        ResFuncPtr opt_index_result_ptr = &nemesis::AnimVarPtr::GetResultFromOptionWithNoIndex;

        RtnFunc opt_selector
            = [&](nemesis::ScopeInfo& scopeinfo) { return (this->*opt_result_ptr)(scopeinfo); };
    };
} // namespace nemesis
