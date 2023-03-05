#include "core/animvarptr.h"

#include "utilities/option.h"
#include "utilities/templateclass.h"
#include "utilities/stringextension.h"

#include "hkx/hkxbehavior.h"

#include "scope/scopeinfo.h"

#include "update/patch.h"

const UMap<char, nemesis::AnimVarPtr::AnimIndex::Order> nemesis::AnimVarPtr::AnimIndex::orderparser
{
    {'L', nemesis::AnimVarPtr::AnimIndex::LAST},
    {'N', nemesis::AnimVarPtr::AnimIndex::NEXT},
    {'B', nemesis::AnimVarPtr::AnimIndex::BACK}
};

const nemesis::AnimVarPtr::Parser::funcptr nemesis::AnimVarPtr::Parser::base_functions[] =
{
    &nemesis::AnimVarPtr::Parser::TryPopulateFromOption,
    &nemesis::AnimVarPtr::Parser::TestCustomSelector,
    &nemesis::AnimVarPtr::Parser::TryAddAnimObject,
    &nemesis::AnimVarPtr::Parser::TryAddStateId
};

const std::regex nemesis::AnimVarPtr::lexing_rgx("(\\]\\[|\\[|\\])");
const std::sregex_token_iterator nemesis::AnimVarPtr::end;

const nemesis::regex nemesis::AnimVarPtr::animobj_rgx("^AnimObject/([0-9]+)$");
const nemesis::regex nemesis::AnimVarPtr::constant_rgx("(?:[^\\\\]|^)\"(.*?[^\\\\]|)\"$");

std::string nemesis::AnimVarPtr::Lexer::GetFormattedArguments()
{
    if (!formattedargs.empty()) return formattedargs;

    std::string str;

    for (auto& arg : arguments)
    {
        str.append("[" + arg + "]");
    }

    formattedargs = str;
    return formattedargs;
}

void nemesis::AnimVarPtr::Lexer::CacheGroupIndexSearch()
{
    search_pref = name + "_group[][";
    search_suf  = "][" + varname + "]" + formattedargs;
}

void nemesis::AnimVarPtr::Lexer::CacheVar()
{
    auto* result = &cache_results[varname + formattedargs];
    *result      = {varname};
    result->insert(result->end(), arguments.begin(), arguments.end());
}

void nemesis::AnimVarPtr::Lexer::CacheAnimVar()
{
    auto* result = &cache_results[name + "[][" + varname + "]" + formattedargs];
    *result      = {name, "", varname};
    result->insert(result->end(), arguments.begin(), arguments.end());
}

void nemesis::AnimVarPtr::Lexer::CacheGroupAnimVar(const std::string& index)
{
    auto* result = &cache_group_results[name + "_group[][" + index + "][" + varname + "]" + formattedargs];
    *result      = {name + "_group", "", index, varname};
    result->insert(result->end(), arguments.begin(), arguments.end());
}

bool nemesis::AnimVarPtr::Lexer::TryGetConstant(const std::string& line, DeqStr& components) const
{
    if (line.empty() || line.front() != '\"' || line.back() != '\"') return false;

    auto counter = std::count(line.begin(), line.end(), '\"');

    if (counter != 2) return false;

    components = {line};
    return true;
}

bool nemesis::AnimVarPtr::Lexer::TryGetCacheResult(const std::string& line,
                                                   const Map<std::string, DeqStr, alphanum_less>& cache,
                                                   std::string& match,
                                                   DeqStr& components,
                                                   size_t& refpos)
{
    for (auto& cache : cache)
    {
        size_t pos = line.find(cache.first);

        if (pos == NOT_FOUND) continue;

        size_t pos2 = pos + cache.first.length();

        if (pos2 < line.size() && line[pos2] == '[') continue;

        refpos     = pos;
        match      = cache.first;
        components = cache.second;
        return true;
    }

    return false;
}

bool nemesis::AnimVarPtr::Lexer::TryGetIndexGroupVar(std::string_view vline,
                                                     std::string& match,
                                                     DeqStr& components,
                                                     size_t& refpos) const
{
    size_t pos = vline.find(search_pref);

    if (pos == NOT_FOUND) return false;

    size_t pos2 = vline.find(search_suf);

    if (pos2 == NOT_FOUND) return false;

    pos2 += search_suf.length();

    if (pos2 < vline.size() && vline[pos2] == '[') return false;

    vline = vline.substr(pos, pos2 - pos);

    if (vline.empty()) return false;

    for (auto& ch : vline)
    {
        if (!isdigit(ch)) return false;
    }

    refpos     = pos;
    components = {name + "_group", "", std::string(vline), varname};
    components.insert(components.end(), arguments.begin(), arguments.end());
    match = name + "_group[][" + std::string(vline) + "][" + varname + "]" + formattedargs;
    return true;
}

nemesis::AnimVarPtr::Lexer::Iterator::Iterator(const Lexer& lexer, const std::string& refline)
    : refline(refline)
    , lexer(&lexer)
{
    line = refline;
    operator++();
    end = false;
}

bool nemesis::AnimVarPtr::Lexer::Iterator::operator==(Iterator iter)
{
    return end == iter.end;
}

bool nemesis::AnimVarPtr::Lexer::Iterator::operator!=(Iterator iter)
{
    return end != iter.end;
}

nemesis::AnimVarPtr::Lexer::Iterator& nemesis::AnimVarPtr::Lexer::Iterator::operator++()
{
    if (!lexer->TokenizeOnce(line, *this))
    {
        end = true;
        return *this;
    }
    
    line.erase(pos, match.length());
    end = false;
    return *this;
}

void nemesis::AnimVarPtr::Lexer::AddResultFunction(RtnFunc result_func) noexcept
{
    this->result_func = result_func;
}

nemesis::AnimVarPtr::RtnFunc nemesis::AnimVarPtr::Lexer::GetResultFunc() const noexcept
{
    return result_func;
}

bool nemesis::AnimVarPtr::Lexer::TokenizeOnce(const std::string& line,
                                              Pair<std::string, DeqStr>& output) const
{
    size_t pos  = line.find("$");
    size_t pos2 = line.find("$", pos + 1);

    while (pos != NOT_FOUND && pos2 != NOT_FOUND)
    {
        std::string temp_line = line.substr(pos, pos2 - pos);

        if (TryGetCacheResult(line, cache_group_results, output.first, output.second, pos)) return true;

        if (TryGetIndexGroupVar(line, output.first, output.second, pos)) return true;

        if (TryGetCacheResult(line, cache_results, output.first, output.second, pos)) return true;

        pos  = line.find("$", pos2 + 1);
        pos2 = line.find("$", pos + 1);
    }

    return false;
}

bool nemesis::AnimVarPtr::Lexer::TokenizeOnce(const std::string& line, Iterator& itr) const
{
    size_t pos = line.find("$");
    size_t pos2 = line.find("$", pos + 1);

    while (pos != NOT_FOUND && pos2 != NOT_FOUND)
    {
        std::string temp_line = line.substr(pos, pos2 - pos);

        if (TryGetCacheResult(temp_line, cache_group_results, itr.match, itr.groups, itr.pos)
            || TryGetIndexGroupVar(temp_line, itr.match, itr.groups, itr.pos)
            || TryGetCacheResult(temp_line, cache_results, itr.match, itr.groups, itr.pos))
        {
            itr.pos += pos;
            return true;
        }

        pos  = line.find("$", pos2 + 1);
        pos2 = line.find("$", pos + 1);
    }

    return false;
}

nemesis::AnimVarPtr::Lexer::Iterator nemesis::AnimVarPtr::Lexer::TokenizeString(const std::string& line) const
{
    return Iterator(*this, line);
}

nemesis::AnimVarPtr::Parser::Parser(DeqStr& components,
                                    size_t linenum, const nemesis::File& fileref)
    : components(components)
    , fileref(fileref)
{
    this->linenum     = linenum;
    animvar_uptr      = std::make_unique<nemesis::AnimVarPtr>();
    animvar_ptr       = animvar_uptr.get();
    auto& express_ref = animvar_ptr->raw_expression;
    express_ref       = components.front();

    for (size_t i = 1; i < components.size(); i++)
    {
        express_ref.push_back('[');
        express_ref.append(components[i]);
        express_ref.push_back(']');
    }
}

nemesis::AnimVarPtr::Parser::Parser(const std::string& expression,
                                    size_t linenum,
                                    const nemesis::File& fileref,
                                    AnimVarPtr* target)
    : components(_components)
    , fileref(fileref)
{
    animvar_ptr = target;
    animvar_ptr->raw_expression = expression;
    components
        = DeqStr(std::sregex_token_iterator(expression.begin(), expression.end(), lexing_rgx, -1), end);
    this->linenum = linenum;
}

void nemesis::AnimVarPtr::Parser::SetSelector(RtnFunc selector)
{
    b_custom_selector         = true;
    animvar_ptr->opt_selector = selector;
}

UPtr<nemesis::AnimVarPtr> nemesis::AnimVarPtr::Parser::Parse()
{
    try
    {
        TryPopulateConstant();

        if (TryPopulateAsModPatch()) return std::move(animvar_uptr);

        SetupTemplateClass();

        if (animvar_ptr->IsConstant()) return std::move(animvar_uptr);

        if (TryPopulateFromMaster()) return std::move(animvar_uptr);

        if (TryPopulateFromGroup()) return std::move(animvar_uptr);

        if (TryPopulateFromTemplate()) return std::move(animvar_uptr);

        if (TryAddAsBase()) return std::move(animvar_uptr);

        throw nemesis::AnimVarPtr::SYNTAX_ERROR;
    }
    catch (ErrorType ertype)
    {
        ErrorMessage(static_cast<int>(ertype), animvar_ptr->templatename, fileref.GetFilePath(), linenum);
    }

    return nullptr;
}

bool nemesis::AnimVarPtr::Parser::IsFileModPatch()
{
    return dynamic_cast<const nemesis::Patch*>(&fileref) != nullptr;
}

bool nemesis::AnimVarPtr::Parser::TryPopulateAsModPatch()
{
    if (!IsFileModPatch()) return false;

    animvar_ptr->b_modcode = true;
    return true;
}

void nemesis::AnimVarPtr::Parser::SetupTemplateClass()
{
    const nemesis::Template* templatefile = dynamic_cast<const nemesis::Template*>(&fileref);
    const nemesis::HkxBehavior* behavior  = nullptr;

    if (templatefile)
    {
        animvar_ptr->templateclass = &templatefile->GetTemplateClass();
        animvar_ptr->templatename  = animvar_ptr->templateclass->GetName();
        return;
    }

    behavior = dynamic_cast<const nemesis::HkxBehavior*>(&fileref);

    if (!behavior) throw std::runtime_error("Unsupported nemesis file object");

    if (animvar_ptr->IsConstant()) return;

    std::string name           = ParseClassName(components.front());
    animvar_ptr->templatename  = name;
    animvar_ptr->templateclass = behavior->GetAnimTemplate()->GetClass(name);

    if (!animvar_ptr->templateclass) ErrorMessage(1225, behavior->GetFilePath(), name);
}

bool nemesis::AnimVarPtr::Parser::TryPopulateConstant()
{
    if (components.size() != 1) return false;

    auto value = components.back();

    if (value.front() != '\"' || value.back() != '\"') return false;

    if (std::count(value.begin(), value.end(), '\"') != 2) return false;

    animvar_ptr->constant = std::make_unique<std::string>(value);
    return true;
}

bool nemesis::AnimVarPtr::Parser::TryPopulateFromMaster()
{
    auto* behavior = dynamic_cast<const nemesis::HkxBehavior*>(&fileref);

    if (!behavior) return false;

    if (!nemesis::iequals(animvar_ptr->templatename + "_master", components.front())) return false;

    if (components.size() > 1) ErrorMessage(1226, behavior->GetFilePath(), animvar_ptr->templatename);

    animvar_ptr->b_allmaster = true;
    return true;
}

bool nemesis::AnimVarPtr::Parser::TryPopulateFromGroup()
{
    if (!nemesis::iequals(animvar_ptr->templatename + "_group", components.front())) return false;

    components.pop_front();

    if (components.empty())
    {
        animvar_ptr->b_allgroup = true;
        return true;
    }

    PopulateFromGroup();
    return true;
}

void nemesis::AnimVarPtr::Parser::PopulateFromGroup()
{
    std::string& value = components.front();

    if (!value.empty())
    {
        if (!isOnlyNumber(value)) throw ErrorType::SYNTAX_ERROR;

        animvar_ptr->group          = std::make_unique<size_t>(stoi(value));
        animvar_ptr->grp_result_ptr = &nemesis::AnimVarPtr::GetResultFromGroup;
    }

    components.pop_front();

    if (components.empty()) return;

    PopulateFromTemplate();
}

bool nemesis::AnimVarPtr::Parser::TryPopulateFromTemplate()
{
    if (!nemesis::iequals(animvar_ptr->templatename, components.front())) return false;

    components.pop_front();

    if (components.empty())
    {
        animvar_ptr->b_allanim = true;
        return true;
    }

    PopulateFromTemplate();
    return true;
}

void nemesis::AnimVarPtr::Parser::PopulateFromTemplate()
{
    AddAnimValue();
    components.pop_front();

    if (components.empty()) return;

    if (b_custom_selector) return;

    if (TryAddAnimObject()) return;

    if (TryAddStateId()) return;

    PopulateFromOption(animvar_ptr->templateclass->GetOptionModels());
}

bool nemesis::AnimVarPtr::Parser::TryPopulateFromOption()
{
    auto& model       = animvar_ptr->templateclass->GetOptionModels();
    std::string value = components.front();

    if (value.back() == '*')
    {
        value.pop_back();
        animvar_ptr->b_linked = true;
    }

    if (!model.Contains(value)) return false;

    PopulateFromOption(model);
    return true;
}

void nemesis::AnimVarPtr::Parser::PopulateFromOption(const nemesis::OptionModelList& model)
{
    std::string value = components.front();

    if (value.back() == '*')
    {
        value.pop_back();
        animvar_ptr->b_linked = true;
    }

    if (b_custom_selector) return;

    if (value.empty() || !model.Contains(value)) throw ErrorType::UNKNOWN_OPTION;

    animvar_ptr->option = std::make_unique<std::string>(value);
    animvar_ptr->opt_result_ptr = &nemesis::AnimVarPtr::GetResultFromOption;
    components.pop_front();

    if (components.empty()) return;

    if (TryPopulateVariable(*model.GetModel(value))) return;

    PopulateElement();
}

bool nemesis::AnimVarPtr::Parser::TryPopulateVariable(const nemesis::OptionModel& model)
{
    std::string& value = components.front();

    if (isOnlyNumber(value)) return false;

    if (value.empty() || !model.GetVariablePtr(value)) return false;

    PopulateFromVariable(model);

    if (components.empty()) return true;

    PopulateElement();
    return true;
}

void nemesis::AnimVarPtr::Parser::PopulateFromVariable(const nemesis::OptionModel& model)
{
    std::string& value = components.front();

    if (value.empty() || !model.GetVariablePtr(value)) throw ErrorType::UNKNOWN_VARIABLE;

    animvar_ptr->optionvar = value;
    components.pop_front();
}

void nemesis::AnimVarPtr::Parser::PopulateElement()
{
    std::string& value = components.front();

    if (value.empty())
    {
        components.pop_front();
        return;
    }

    if (!isOnlyNumber(value)) throw ErrorType::SYNTAX_ERROR;

    animvar_ptr->optionindex          = std::make_unique<size_t>(stoi(value));
    animvar_ptr->opt_index_result_ptr = &nemesis::AnimVarPtr::GetResultFromOptionWithIndex;
    components.pop_front();
}

bool nemesis::AnimVarPtr::Parser::TestCustomSelector()
{
    return b_custom_selector;
}

bool nemesis::AnimVarPtr::Parser::TryAddAnimObject()
{
    const std::string& value = components.front();
    nemesis::smatch sm;

    if (!nemesis::regex_match(value, sm, animobj_rgx)) return false;

    AddAnimObject(stoi(sm.str(1)));
    return true;
}

void nemesis::AnimVarPtr::Parser::AddAnimObject(size_t groupnum)
{
    animvar_ptr->animobj_grp    = std::make_unique<size_t>(groupnum);
    animvar_ptr->opt_result_ptr = &nemesis::AnimVarPtr::GetResultFromAnimObj;
    components.pop_front();

    if (components.empty()) return;

    AddAnimObjectIndex();
}

void nemesis::AnimVarPtr::Parser::AddAnimObjectIndex()
{
    animvar_ptr->animobj_index = std::make_unique<size_t>(stoi(components.front()));
    components.pop_front();
}

bool nemesis::AnimVarPtr::Parser::TryAddStateId()
{
    const std::string& value = components.front();

    if (!value._Starts_with("S@")) return false;

    if (!isOnlyNumber(value.substr(2))) return false;

    AddStateId();
    return true;
}

void nemesis::AnimVarPtr::Parser::AddStateId()
{
    animvar_ptr->stateid        = std::make_unique<size_t>(std::stoi(components.front().substr(2)));
    animvar_ptr->opt_result_ptr = &nemesis::AnimVarPtr::GetResultFromStateId;
    components.pop_front();
}

bool nemesis::AnimVarPtr::Parser::TryAddAsBase()
{
    for (auto& func : base_functions)
    {
        if (!(this->*func)()) continue;

        animvar_ptr->b_base         = true;
        animvar_ptr->grp_result_ptr = &nemesis::AnimVarPtr::GetResultFromBase;
        break;
    }

    return animvar_ptr->b_base;
}

void nemesis::AnimVarPtr::Parser::AddAnimValue()
{
    auto& anim = animvar_ptr->anim_index;
    anim       = std::make_unique<AnimIndex>(components.front(), *animvar_ptr);

    if (!anim->HasValue()) return;

    animvar_ptr->anim_result_ptr = &nemesis::AnimVarPtr::GetResultFromAnim;
}

nemesis::AnimVarPtr::AnimIndex::AnimIndex(const std::string& animindex, nemesis::AnimVarPtr& animvar)
    : animvar(animvar)
{
    if (animindex.empty()) return;

    if (isOnlyNumber(animindex))
    {
        index = std::make_unique<size_t>(stoi(animindex));
        return;
    }

    if (animindex.length() != 1) throw ErrorType::SYNTAX_ERROR;

    const char ch = animindex[0];

    if (ch == 'F')
    {
        index = std::make_unique<size_t>(0);
        return;
    }

    auto itr = orderparser.find(ch);

    if (itr == orderparser.end()) throw ErrorType::SYNTAX_ERROR;

    order = itr->second;
}

bool nemesis::AnimVarPtr::AnimIndex::HasValue() const noexcept
{
    return index != nullptr || order != Order::NONE;
}

size_t nemesis::AnimVarPtr::AnimIndex::GetValue(nemesis::ScopeInfo& scopeinfo) const
{
    if (index) return *index;

    const nemesis::AnimQuery* animquery;

    switch (order)
    {
        case Order::NEXT:
            animquery = scopeinfo.GetNextAnim(animvar.templateclass);
            break;
        case Order::BACK:
            animquery = scopeinfo.GetBackAnim(animvar.templateclass);
            break;
        case Order::LAST:
            animquery = scopeinfo.GetLastAnim(animvar.templateclass);
            break;
        default:
            animquery = scopeinfo.GetAnim(animvar.templateclass);
            break;
    }

    return animquery->GetArrayNum();
}

std::string nemesis::AnimVarPtr::GetResultFromConstant(nemesis::ScopeInfo& scopeinfo) const
{
    return *constant;
}

std::string nemesis::AnimVarPtr::GetResultFromBase(nemesis::ScopeInfo& scopeinfo) const
{
    std::string rtn;
    scopeinfo.ExeTempBaseQuery(templateclass, [&]() { rtn = GetFromOption(scopeinfo); });
    return rtn;
}

std::string nemesis::AnimVarPtr::GetResultFromGroup(nemesis::ScopeInfo& scopeinfo) const
{
    std::string rtn;
    scopeinfo.ExeTempNumQuery(*group, templateclass, [&]() { rtn = GetFromAnim(scopeinfo); });
    return rtn;
}

std::string nemesis::AnimVarPtr::GetResultFromNoGroup(nemesis::ScopeInfo& scopeinfo) const
{
    return GetFromAnim(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetResultFromAnim(nemesis::ScopeInfo& scopeinfo) const
{
    std::string rtn;
    scopeinfo.ExeTempNumAnim(
        GetAnim(scopeinfo), templateclass, [&]() { rtn = GetFromOption(scopeinfo); });
    return rtn;
}

std::string nemesis::AnimVarPtr::GetResultFromNoAnim(nemesis::ScopeInfo& scopeinfo) const
{
    return GetFromOption(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetResultFromAnimObj(nemesis::ScopeInfo& scopeinfo) const
{
    return *scopeinfo.GetAnimObject(GetAnimObj(), templateclass);
}

std::string nemesis::AnimVarPtr::GetResultFromMainAnimEvent(nemesis::ScopeInfo& scopeinfo) const
{
    return std::string(scopeinfo.GetAnim(templateclass)->GetAnimationName());
}

std::string nemesis::AnimVarPtr::GetResultFromStateId(nemesis::ScopeInfo& scopeinfo) const
{
    std::string nodeid(scopeinfo.GetCurrentNodeId());
    auto* anim = scopeinfo.GetAnim(templateclass);
    return std::to_string(scopeinfo.GetStateIdManager()->GetStateId(*anim, GetStateId(), nodeid));
}

std::string nemesis::AnimVarPtr::GetResultFromOption(nemesis::ScopeInfo& scopeinfo) const
{
    return GetFromOptionIndex(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetResultFromNoOption(nemesis::ScopeInfo& scopeinfo) const
{
    std::stringstream ss;
    auto animptr        = scopeinfo.GetAnim(templateclass);
    const void* address = static_cast<const void*>(animptr);
    ss << address;
    return ss.str();
}

std::string nemesis::AnimVarPtr::GetResultFromOptionWithIndex(nemesis::ScopeInfo& scopeinfo) const
{
    auto optionlistptr = scopeinfo.GetOptionListPtr(GetOption(), templateclass);

    if (!optionlistptr || optionlistptr->empty()) return "";

    return optionlistptr->at(GetOptionIndex())->GetVariable(GetOptionVar());
}

std::string nemesis::AnimVarPtr::GetResultFromOptionWithNoIndex(nemesis::ScopeInfo& scopeinfo) const
{
    auto cur_option = scopeinfo.GetOption(GetOption(), templateclass);

    if (cur_option) return cur_option->GetVariable(GetOptionVar());

    auto optionlistptr = scopeinfo.GetOptionListPtr(GetOption(), templateclass);

    if (!optionlistptr || optionlistptr->empty()) return "";

    return optionlistptr->front()->GetVariable(GetOptionVar());
}

std::string nemesis::AnimVarPtr::GetFromAnim(nemesis::ScopeInfo& scopeinfo) const
{
    return (this->*anim_result_ptr)(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetFromOption(nemesis::ScopeInfo& scopeinfo) const
{
    return opt_selector(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetFromOptionIndex(nemesis::ScopeInfo& scopeinfo) const
{
    return (this->*opt_index_result_ptr)(scopeinfo);
}

nemesis::AnimVarPtr::AnimVarPtr(const std::string& expression,
                                size_t linenum, const nemesis::File& fileref)
{
    Parser parser(expression, linenum, fileref, this);
    parser.Parse();
}

nemesis::AnimVarPtr::AnimVarPtr(const std::string& expression,
                                size_t linenum,
                                const nemesis::File& fileref,
                                RtnFunc selector)
{
    Parser parser(expression, linenum, fileref, this);
    parser.SetSelector(selector);
    parser.Parse();
}

std::string nemesis::AnimVarPtr::GetResult(nemesis::ScopeInfo& scopeinfo) const
{
    if (IsModCode()) throw std::runtime_error("Mod code condition is being used for GetResult() function");

    return (this->*grp_result_ptr)(scopeinfo);
}

bool nemesis::AnimVarPtr::IsExist(nemesis::ScopeInfo& scopeinfo) const
{
    if (IsModCode()) throw std::runtime_error("Mod code condition is being used for IsExist() function");

    if (!IsAllMaster() && !IsAllGroup() && !IsAllAnim()) return IsGroupExist(scopeinfo);

    auto querylist = scopeinfo.GetTemplateAllQueries(templateclass);
    return querylist && !querylist->empty();
}

bool nemesis::AnimVarPtr::IsConstant() const noexcept
{
    return constant != nullptr;
}

bool nemesis::AnimVarPtr::IsAllMaster() const noexcept
{
    return b_allmaster;
}

bool nemesis::AnimVarPtr::IsAllGroup() const noexcept
{
    return b_allgroup;
}

bool nemesis::AnimVarPtr::IsAllAnim() const noexcept
{
    return b_allanim;
}

bool nemesis::AnimVarPtr::IsBase() const noexcept
{
    return b_base;
}

bool nemesis::AnimVarPtr::IsMainAnimEvent() const noexcept
{
    return b_mae;
}

bool nemesis::AnimVarPtr::IsModCode() const noexcept
{
    return b_modcode;
}

bool nemesis::AnimVarPtr::HasGroup() const noexcept
{
    return group != nullptr;
}

bool nemesis::AnimVarPtr::HasAnim() const noexcept
{
    return anim_index != nullptr && anim_index->HasValue();
}

bool nemesis::AnimVarPtr::HasOption() const noexcept
{
    return option != nullptr;
}

bool nemesis::AnimVarPtr::HasOptionVar() const noexcept
{
    return !optionvar.empty();
}

bool nemesis::AnimVarPtr::HasOptionIndex() const noexcept
{
    return optionindex != nullptr;
}

bool nemesis::AnimVarPtr::HasAnimObj() const noexcept
{
    return animobj_grp != nullptr;
}

bool nemesis::AnimVarPtr::HasAnimObjIndex() const noexcept
{
    return animobj_index != nullptr;
}

bool nemesis::AnimVarPtr::HasStateId() const noexcept
{
    return stateid != nullptr;
}

size_t nemesis::AnimVarPtr::GetGroup() const
{
    return *group;
}

size_t nemesis::AnimVarPtr::GetAnim(nemesis::ScopeInfo& scopeinfo) const
{
    return anim_index->GetValue(scopeinfo);
}

std::string nemesis::AnimVarPtr::GetOption() const
{
    return *option;
}

std::string nemesis::AnimVarPtr::GetOptionVar() const
{
    return optionvar;
}

size_t nemesis::AnimVarPtr::GetOptionIndex() const
{
    return *optionindex;
}

size_t nemesis::AnimVarPtr::GetAnimObj() const
{
    return *animobj_grp;
}

size_t nemesis::AnimVarPtr::GetAnimObjIndex() const
{
    return *animobj_index;
}

size_t nemesis::AnimVarPtr::GetStateId() const
{
    return *stateid;
}

const nemesis::TemplateClass* nemesis::AnimVarPtr::GetTemplateClass() const
{
    return templateclass;
}

std::string_view nemesis::AnimVarPtr::GetRawExpression() const
{
    return raw_expression;
}

bool nemesis::AnimVarPtr::IsGroupExist(nemesis::ScopeInfo& scopeinfo) const
{
    if (!HasGroup()) return IsAnimExist(scopeinfo);

    if (!scopeinfo.HasQuery(*group)) return false;

    bool rtn;
    scopeinfo.ExeTempNumQuery(*group, templateclass, [&]() { rtn = IsAnimExist(scopeinfo); });
    return rtn;
}

bool nemesis::AnimVarPtr::IsAnimExist(nemesis::ScopeInfo& scopeinfo) const
{
    if (!HasAnim())
    {
        return IsOptionExist(scopeinfo);
    }

    size_t anim_index = GetAnim(scopeinfo);

    if (!scopeinfo.HasAnim(anim_index, templateclass)) return false;

    bool rtn;
    scopeinfo.ExeTempNumAnim(anim_index, templateclass, [&]() { rtn = IsOptionExist(scopeinfo); });
    return rtn;
}

bool nemesis::AnimVarPtr::IsOptionExist(nemesis::ScopeInfo& scopeinfo) const
{
    if (!HasOption()) return true;

    if (!HasOptionIndex()) return scopeinfo.HasOption(*option, 0, templateclass);

    return scopeinfo.HasOption(*option, *optionindex, templateclass);
}

std::string nemesis::AnimVarPtr::ParseClassName(const std::string& first_component)
{
    std::string name;
    std::string lower        = nemesis::to_lower_copy(first_component);
    const std::string master = "_master";
    const std::string group  = "_group";

    if (StringEndWith(lower, master)) return first_component.substr(0, lower.length() - master.length());

    if (StringEndWith(lower, group)) return first_component.substr(0, lower.length() - group.length());

    return first_component;
}
