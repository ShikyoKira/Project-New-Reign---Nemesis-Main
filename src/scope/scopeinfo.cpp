#include "utilities/process.h"
#include "utilities/animqueryfile.h"

#include "core/animvarptr.h"

#include "scope/scopeinfo.h"

using namespace std;

using OptionUPtr   = UPtr<nemesis::Option>;

nemesis::ScopeInfo::ChoiceRange::ChoiceRange(size_t posA, size_t posB)
{
    front = posA;
    back  = posB;
}
//
//nemesis::ScopeInfo::AnimObjects::AnimObjects(VecStr& aolist)
//    : aolist(aolist)
//{
//}
//
//void nemesis::ScopeInfo::AnimObjects::SetTempAnimObject(SPtr<const std::string*>& animobj) noexcept
//{
//    animobj_temp = animobj;
//}
//
//void nemesis::ScopeInfo::AnimObjects::InsertAnimObject(const std::string& animobj)
//{
//    auto ptr = &animobj;
//    animobjlayers.emplace_back(ptr);
//}
//
//void nemesis::ScopeInfo::AnimObjects::DropAnimObject() noexcept
//{
//    animobjlayers.pop_back();
//}
//
//SPtr<const std::string*> nemesis::ScopeInfo::AnimObjects::GetTempAnimObject(size_t index) const
//{
//    return std::make_shared<const std::string*>(&aolist.at(index));
//}
//
//const std::string& nemesis::ScopeInfo::AnimObjects::GetAnimObject() const
//{
//    auto& ao = *animobjlayers.back();
//
//    if (animobj_temp.expired()) return ao;
//
//    return **animobj_temp.lock();
//}

const Vec<const nemesis::AnimQuery*>*
nemesis::ScopeInfo::GetTemplateAllQueries(const nemesis::TemplateCategory* templtclass) const
{
    auto query = GetQuery(templtclass);
    return query ? &query->GetQueryList().GetList() : exporter->GetQueriesByTemplate(templtclass->GetName());
}

const Vec<const nemesis::AnimQuery*>*
nemesis::ScopeInfo::GetTemplateAllAnim(const nemesis::Condition& condition) const
{
    auto& var         = condition.GetVariableA();
    auto* templtclass = var.GetTemplateCategory();

    if (!templtclass->HasGroup()) return GetTemplateAllQueries(templtclass);
    
    auto* query = GetQuery(templtclass);

    if (query) return &query->GetArray();

    ErrorMessage(1228, templtclass->GetName(), condition.GetFile().GetFilePath(), condition.GetLineNum());
    return nullptr;
}

const nemesis::Option* nemesis::ScopeInfo::GetCurrentOptionPtr(
    const std::string& optionname, size_t index, const nemesis::TemplateCategory* templtclass) const
{
    auto itr = scopelayer.find(templtclass);

    if (itr == scopelayer.end()) return nullptr;

    auto& optionlayers = itr->second.base_qdlayers.optionlayers;
    auto optlayer_itr  = optionlayers.find(optionname);

    if (optlayer_itr == optionlayers.end() || optlayer_itr->second.Empty()) return nullptr;

    return optlayer_itr->second.GetValue();
}

nemesis::Layers<const nemesis::Option*>&
nemesis::ScopeInfo::GetOptionLayers(const nemesis::Option& opt, const nemesis::TemplateCategory* templtclass)
{
    return scopelayer[templtclass].qdlayers_map[&opt.GetAnimQuery()].optionlayers[opt.GetName()];
}

nemesis::Layers<const std::string*>& nemesis::ScopeInfo::GetAnimObjListLayers(
    size_t animobj_grp, const nemesis::AnimQuery* animquery, const nemesis::TemplateCategory* templtclass)
{
    return scopelayer[templtclass].qdlayers_map[animquery].aolistlayers[animobj_grp];
}

const nemesis::ScopeInfo::ScopeLayer::QueryDataLayers*
nemesis::ScopeInfo::GetCurrentQueryDataLayers(const nemesis::TemplateCategory* templtclass) const
{
    auto itr = scopelayer.find(templtclass);

    if (itr == scopelayer.end()) return nullptr;

    auto anim         = GetAnim(templtclass);
    auto qdlayers_itr = itr->second.qdlayers_map.find(anim);

    if (qdlayers_itr == itr->second.qdlayers_map.end()) return nullptr;

    return &qdlayers_itr->second;
}

void nemesis::ScopeInfo::SetCurrentImport(nemesis::AnimImport* animimport_ptr)
{
    currentimport_ptr = animimport_ptr;
}

void nemesis::ScopeInfo::SetCurrentLine(nemesis::Line* lineptr)
{
    this->lineptr = lineptr;
}

void nemesis::ScopeInfo::SetCurrentLine(const nemesis::Line* lineptr)
{
    this->lineptr = lineptr;
}

void nemesis::ScopeInfo::ExeTempQuery(SPtr<const AnimQuery*>& shptr,
                                      const nemesis::TemplateCategory* templtclass,
                                      std::function<void()> callback)
{
    auto class_shr_ptr = std::make_shared<const nemesis::TemplateCategory*>(templtclass);
    SetTempTemplateCategory(class_shr_ptr);
    SetTempQuery(shptr, templtclass);
    callback();
}

void nemesis::ScopeInfo::ExeTempAnim(SPtr<const AnimQuery*>& shptr,
                                     const nemesis::TemplateCategory* templtclass,
                                     std::function<void()> callback)
{
    auto class_shr_ptr = std::make_shared<const nemesis::TemplateCategory*>(templtclass);
    SetTempTemplateCategory(class_shr_ptr);
    SetTempAnim(shptr, templtclass);
    callback();
}

bool nemesis::ScopeInfo::TrySetNodeId(nemesis::Line& line)
{
    auto pos = line.find(nodeid_search_sv1);

    if (pos == NOT_FOUND) return false;

    pos += nodeid_search_sv1.size();
    auto pos2 = line.find(nodeid_search_sv2, pos);

    if (pos2 == NOT_FOUND) return false;

    std::string_view vline = line.ToString();
    std::string id = line.substr(pos, pos2);

    if (!isOnlyNumber(id)) return false;

    nodeid_sv = vline.substr(pos, pos2);
    return true;
}

void nemesis::ScopeInfo::SetCurrentCondition(const nemesis::Condition* conditionptr)
{
    this->conditionptr = conditionptr;
}

void nemesis::ScopeInfo::SetCurrentProcess(nemesis::Process* processptr)
{
    this->processptr = processptr;
}

void nemesis::ScopeInfo::SetAnimCounter(size_t animcounter)
{
    this->animcounter = animcounter;
}

void nemesis::ScopeInfo::SetCurrentQuery(const nemesis::AnimQuery& query)
{
    basequery = &query;
    querylist = &query.GetQueryList();
}

void nemesis::ScopeInfo::SetTempTemplateCategory(SPtr<const nemesis::TemplateCategory*> templtclass_ptr) noexcept
{
    if (!temp_templtclass_ptr.expired()) return;

    temp_templtclass_ptr = templtclass_ptr;
}

void nemesis::ScopeInfo::SetTempQuery(SPtr<const nemesis::AnimQuery*>& index,
                                      const nemesis::TemplateCategory* templtclass) noexcept
{
    scopelayer[templtclass].querylayers.SetTemp(index);
}

void nemesis::ScopeInfo::SetTempAnim(SPtr<const nemesis::AnimQuery*>& anim,
                                     const nemesis::TemplateCategory* templtclass) noexcept
{
    scopelayer[templtclass].animlayers.SetTemp(anim);
}

void nemesis::ScopeInfo::SetTempOption(SPtr<const Option*>& opt,
                                       const nemesis::TemplateCategory* templtclass) noexcept
{
    auto& optionlayers = GetOptionLayers(**opt, templtclass);
    optionlayers.SetTemp(opt);
}

void nemesis::ScopeInfo::SetTempAnimObject(SPtr<const std::string*>& animobj,
                                           size_t animobj_grp,
                                           const nemesis::AnimQuery* animquery,
                                           const nemesis::TemplateCategory* templtclass) noexcept
{
    auto& aolistlayers = GetAnimObjListLayers(animobj_grp, animquery, templtclass);
    aolistlayers.SetTemp(animobj);
}

void nemesis::ScopeInfo::InsertQuery(const nemesis::AnimQuery& index,
                                     const nemesis::TemplateCategory* templtclass)
{
    Insert(scopelayer[templtclass].querylayers, &index);
}

void nemesis::ScopeInfo::InsertAnim(const nemesis::AnimQuery& anim, const nemesis::TemplateCategory* templtclass)
{
    Insert(scopelayer[templtclass].animlayers, &anim);
}

void nemesis::ScopeInfo::InsertOption(const nemesis::Option& opt, const nemesis::TemplateCategory* templtclass)
{
    auto& optionlayers = GetOptionLayers(opt, templtclass);
    Insert(optionlayers, &opt);
}

void nemesis::ScopeInfo::InsertAnimObject(const std::string& animobj,
                                          size_t animobj_grp,
                                          const nemesis::AnimQuery* animquery,
                                          const nemesis::TemplateCategory* templtclass)
{
    auto& aolistlayers = GetAnimObjListLayers(animobj_grp, animquery, templtclass);
    Insert(aolistlayers, &animobj);
}

void nemesis::ScopeInfo::DropLayer() noexcept
{
    auto& last = layerorder.back();
    last->DropLayer();
    layerorder.pop_back();
}

bool nemesis::ScopeInfo::IsTemplateActive(const std::string& name) const
{
    return exporter->IsTemplateActive(name);
}

bool nemesis::ScopeInfo::HasQuery(size_t index) const
{
    return index < querylist->GetListSize();
}

bool nemesis::ScopeInfo::HasAnim(size_t index, const nemesis::TemplateCategory* templtclass) const
{
    auto queryptr = GetQuery(templtclass);

    if (!queryptr) return false;

    return index < queryptr->GetArraySize();
}

bool nemesis::ScopeInfo::HasOption(std::string optionname,
                                   size_t index,
                                   const nemesis::TemplateCategory* templtclass) const
{
    auto anim = GetAnim(templtclass);

    if (!anim) return false;

    return anim->GetOptionPtr(optionname, index) != nullptr;
}

void nemesis::ScopeInfo::ExeTempNumQuery(size_t index,
                                         const nemesis::TemplateCategory* templtclass,
                                         std::function<void()> callback)
{
    auto queries = GetQuery(templtclass)->GetQueryList();

    if (index >= queries.GetListSize()) ErrorMessage(1165, GetFormat(), GetFile(), GetCurrentLineNum());

    auto shptr = std::make_shared<const nemesis::AnimQuery*>(&queries.GetQueryByIndex(index));
    ExeTempQuery(shptr, templtclass, callback);
}

void nemesis::ScopeInfo::ExeTempNumAnim(size_t index,
                                        const nemesis::TemplateCategory* templtclass,
                                        std::function<void()> callback)
{
    auto queries = GetQuery(templtclass);

    if (index >= queries->GetArraySize()) ErrorMessage(1166, GetFormat(), GetFile(), GetCurrentLineNum());

    auto shptr = std::make_shared<const nemesis::AnimQuery*>(&queries->GetArrayQuery(index));
    ExeTempAnim(shptr, templtclass, callback);
}

void nemesis::ScopeInfo::ExeTempOption(const nemesis::Option& opt,
                                       const nemesis::TemplateCategory* templtclass,
                                       std::function<void()> callback)
{
    auto shptr         = std::make_shared<const nemesis::Option*>(&opt);
    auto class_shr_ptr = std::make_shared<const nemesis::TemplateCategory*>(templtclass);
    SetTempTemplateCategory(class_shr_ptr);
    SetTempOption(shptr, templtclass);
    callback();
}

void nemesis::ScopeInfo::ExeTempAnimObject(size_t animobj_grp,
                                           size_t animobj_index,
                                           const nemesis::TemplateCategory* templtclass,
                                           std::function<void()> callback)
{
    auto anim          = GetAnim(templtclass);
    auto animobj       = &anim->GetAnimObject(animobj_grp).at(animobj_index);
    auto shptr         = std::make_shared<const std::string*>(animobj);
    auto class_shr_ptr = std::make_shared<const nemesis::TemplateCategory*>(templtclass);
    SetTempTemplateCategory(class_shr_ptr);
    SetTempAnimObject(shptr, animobj_grp, anim, templtclass);
    callback();
}

void nemesis::ScopeInfo::ExeTempBaseQuery(const nemesis::TemplateCategory* templtclass,
                                          std::function<void()> callback)
{
    auto queryptr = std::make_shared<const AnimQuery*>(basequery);
    ExeTempQuery(queryptr, templtclass, callback);
}

void nemesis::ScopeInfo::ExeTempBackAnim(const nemesis::TemplateCategory* templtclass,
                                         std::function<void()> callback)
{
    auto anim     = GetAnim(templtclass);
    auto queryptr = std::make_shared<const AnimQuery*>(&anim->GetBackInArray());
    ExeTempAnim(queryptr, templtclass, callback);
}

void nemesis::ScopeInfo::ExeTempNextAnim(const nemesis::TemplateCategory* templtclass,
                                         std::function<void()> callback)
{
    auto anim     = GetAnim(templtclass);
    auto queryptr = std::make_shared<const AnimQuery*>(&anim->GetNextInArray());
    ExeTempAnim(queryptr, templtclass, callback);
}

void nemesis::ScopeInfo::ExeTempLastAnim(const nemesis::TemplateCategory* templtclass,
                                         std::function<void()> callback)
{
    auto anim     = GetAnim(templtclass);
    auto queryptr = std::make_shared<const AnimQuery*>(&anim->GetLastInArray());
    ExeTempAnim(queryptr, templtclass, callback);
}

const nemesis::TemplateCategory* nemesis::ScopeInfo::GetTemplateCategory() const
{
    if (temp_templtclass_ptr.expired()) return nullptr;

    return *temp_templtclass_ptr.lock().get();
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetQuery(const nemesis::TemplateCategory* templtclass) const
{
    auto itr = scopelayer.find(templtclass);

    if (itr == scopelayer.end() || itr->second.querylayers.Empty()) return nullptr;

    return itr->second.querylayers.GetValue();
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetAnim(const nemesis::TemplateCategory* templtclass) const
{
    auto itr = scopelayer.find(templtclass);

    if (itr == scopelayer.end() || itr->second.animlayers.Empty()) return GetQuery(templtclass);

    return itr->second.animlayers.GetValue();
}

const nemesis::Option* nemesis::ScopeInfo::GetOption(const std::string& name,
                                                     const nemesis::TemplateCategory* templtclass) const
{
    auto qdlayers_ptr = GetCurrentQueryDataLayers(templtclass);

    if (!qdlayers_ptr) return nullptr;

    auto& optionlayers = qdlayers_ptr->optionlayers;
    auto optlayers_itr = optionlayers.find(name);

    if (optlayers_itr == optionlayers.end() || optlayers_itr->second.Empty()) return nullptr;

    return optlayers_itr->second.GetValue();
}

const std::string* nemesis::ScopeInfo::GetAnimObject(size_t animobj_grp,
                                                     const nemesis::TemplateCategory* templtclass) const
{
    auto qdlayers_ptr = GetCurrentQueryDataLayers(templtclass);

    if (!qdlayers_ptr) return nullptr;

    auto& aolistlayers    = qdlayers_ptr->aolistlayers;
    auto aolistlayers_itr = aolistlayers.find(animobj_grp);

    if (aolistlayers_itr == aolistlayers.end() || aolistlayers_itr->second.Empty()) return nullptr;

    return aolistlayers_itr->second.GetValue();
}

const nemesis::AnimImport* nemesis::ScopeInfo::GetCurrentImport() const
{
    return currentimport_ptr;
}

std::string_view nemesis::ScopeInfo::GetFormat() const
{
    return processptr->GetFormat();
}

std::filesystem::path nemesis::ScopeInfo::GetFile() const
{
    return processptr->GetBehaviorFile();
}

size_t nemesis::ScopeInfo::GetCurrentLineNum() const
{
    if (lineptr) return lineptr->GetLineNumber();

    if (conditionptr) return conditionptr->GetLineNum();

    if (processptr) return processptr->GetCurrentLineNum();

    throw std::runtime_error("Failed to get current line number");
}

std::string_view nemesis::ScopeInfo::GetCurrentNodeId() const
{
    return nodeid_sv;
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetLastAnim(const nemesis::TemplateCategory* templtclass) const
{
    return &GetAnim(templtclass)->GetLastInArray();
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetBackAnim(const nemesis::TemplateCategory* templtclass) const
{
    return &GetAnim(templtclass)->GetBackInArray();
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetNextAnim(const nemesis::TemplateCategory* templtclass) const
{
    return &GetAnim(templtclass)->GetNextInArray();
}

const nemesis::AnimQuery* nemesis::ScopeInfo::GetCurrentQuery() const
{
    return basequery;
}

const nemesis::Option* nemesis::ScopeInfo::GetBaseOptionPtr(const std::string& optionname,
                                                            size_t index,
                                                            const nemesis::TemplateCategory* templtclass) const
{
    auto optptr = GetCurrentOptionPtr(optionname, index, templtclass);

    if (optptr) return optptr;

    auto optlist = basequery->GetOptionListPtr(optionname);

    if (!optlist) return nullptr;

    return optlist->at(index);
}

const nemesis::Option* nemesis::ScopeInfo::GetOptionPtr(const std::string& optionname,
                                                        const nemesis::AnimQuery* animquery,
                                                        const nemesis::TemplateCategory* templtclass) const
{
    auto itr = scopelayer.find(templtclass);

    if (itr == scopelayer.end()) return nullptr;

    auto qdlayer_itr = itr->second.qdlayers_map.find(animquery);

    if (qdlayer_itr == itr->second.qdlayers_map.end()) return nullptr;

    auto& optionlayers = qdlayer_itr->second.optionlayers;
    auto optlayer_itr  = optionlayers.find(optionname);

    if (optlayer_itr == optionlayers.end() || optlayer_itr->second.Empty()) return nullptr;

    return optlayer_itr->second.GetValue();
}

const nemesis::Option* nemesis::ScopeInfo::GetOptionPtr(const std::string& optionname,
                                                        size_t index,
                                                        const nemesis::TemplateCategory* templtclass) const
{
    auto anim = GetAnim(templtclass);

    if (!anim) return nullptr;

    return anim->GetOptionPtr(optionname, index);
}

const Vec<const nemesis::Option*>*
nemesis::ScopeInfo::GetOptionListPtr(const std::string& optionname,
                                     const nemesis::TemplateCategory* templtclass) const
{
    auto anim = GetAnim(templtclass);

    if (!anim) return nullptr;

    return anim->GetOptionListPtr(optionname);
}

void nemesis::ScopeInfo::GenerateStateIdManager(const nemesis::HkxBehaviorFile& behavior)
{
    stateidmanager = std::make_unique<nemesis::StateIdManager>(behavior);
}

nemesis::StateIdManager* nemesis::ScopeInfo::GetStateIdManager()
{
    return stateidmanager.get();
}

const nemesis::HkxEvent& nemesis::ScopeInfo::GetHkxEvent(const std::string& eventname) const
{
    auto HkxEvent = exporter->GetHkxEventPtr(eventname);

    if (!HkxEvent)
    {
        ErrorMessage(
            1131, lineptr->GetClassName(), lineptr->GetFilePath(), lineptr->GetLineNumber(), eventname);
    }
    
    return *HkxEvent;
}

const nemesis::HkxVariable& nemesis::ScopeInfo::GetVariableID(const std::string& varname) const
{
    auto HkxVariable = exporter->GetVariableIDPtr(varname);

    if (!HkxVariable)
    {
        ErrorMessage(
            1132, lineptr->GetClassName(), lineptr->GetFilePath(), lineptr->GetLineNumber(), varname);
    }
    
    return *HkxVariable;
}

nemesis::Exporter* nemesis::ScopeInfo::GetExporter()
{
    return exporter;
}

UPtr<nemesis::ScopeInfo::ScopeIterator> nemesis::ScopeInfo::ExeQuery(const Condition& condition)
{
    auto& var = condition.GetVariableA();

    if (var.IsBase())
    {
        UPtr<nemesis::ScopeInfo::ScopeIterator> rtn;
        ExeTempBaseQuery(var.GetTemplateCategory(), [&]() { rtn = ExeAnimQuery(condition); });
        return std::move(rtn);
    }

    if (var.IsAllMaster())
    {
        auto* tempclass      = var.GetTemplateCategory();
        auto templtclass_ptr = std::make_shared<Vec<const nemesis::TemplateCategory*>>();
        templtclass_ptr->emplace_back(tempclass);
        templtclass_cache.emplace_back(templtclass_ptr);
        return std::make_unique<ScopeIteratorValue<const TemplateCategory*>>(templtclass_ptr.get(), classlayers);
    }

    return std::move(ExeMasterQuery(condition));
}

UPtr<nemesis::ScopeInfo::ScopeIterator> nemesis::ScopeInfo::ExeMasterQuery(const Condition& condition)
{
    auto& var = condition.GetVariableA();

    if (var.IsAllGroup())
    {
        auto templtclass = var.GetTemplateCategory();
        auto* list       = GetTemplateAllQueries(templtclass);

        if (!list) return nullptr;

        auto& querylayers = scopelayer[templtclass].querylayers;
        return std::make_unique<ScopeIteratorValue<const AnimQuery*>>(list, querylayers);
    }

    if (!var.HasGroup()) return std::move(ExeGroupQuery(condition));

    UPtr<nemesis::ScopeInfo::ScopeIterator> rtn;
    ExeTempNumQuery(var.GetGroup(), var.GetTemplateCategory(), [&]() { rtn = ExeGroupQuery(condition); });
    return std::move(rtn);
}

UPtr<nemesis::ScopeInfo::ScopeIterator> nemesis::ScopeInfo::ExeGroupQuery(const Condition& condition)
{
    auto& var = condition.GetVariableA();

    if (var.IsAllAnim())
    {
        auto* templtclass = var.GetTemplateCategory();
        auto* list        = GetTemplateAllAnim(condition);

        if (!list) return nullptr;

        auto& animlayers  = scopelayer[templtclass].animlayers;
        return std::make_unique<ScopeIteratorValue<const AnimQuery*>>(list, animlayers);
    }

    if (!var.HasAnim()) return std::move(ExeAnimQuery(condition));

    UPtr<nemesis::ScopeInfo::ScopeIterator> rtn;
    ExeTempNumAnim(var.GetAnim(*this), var.GetTemplateCategory(), [&]() { rtn = ExeAnimQuery(condition); });
    return std::move(rtn);
}

UPtr<nemesis::ScopeInfo::ScopeIterator> nemesis::ScopeInfo::ExeAnimQuery(const Condition& condition)
{
    auto& var = condition.GetVariableA();

    if (var.HasAnimObj()) return std::move(ExeAnimObjectQuery(var));

    if (!var.HasOption()) return nullptr;

    auto templtclass = var.GetTemplateCategory();
    auto anim        = GetAnim(templtclass);
    std::string name = var.GetOption();
    auto* optionlist = anim->GetOptionListPtr(name);

    if (!optionlist || optionlist->empty()) return std::make_unique<ScopeIteratorValue<std::string>>();

    auto& qdlayers = scopelayer[templtclass].qdlayers_map[anim];
    return std::make_unique<ScopeIteratorValue<const Option*>>(optionlist, qdlayers.optionlayers[name]);
}

UPtr<nemesis::ScopeInfo::ScopeIterator> nemesis::ScopeInfo::ExeAnimObjectQuery(const AnimVarPtr& var)
{
    auto templtclass   = var.GetTemplateCategory();
    auto anim          = GetAnim(templtclass);
    size_t grp_num       = var.GetAnimObj();
    auto* animobj_list = &anim->GetAnimObject(grp_num);

    if (animobj_list->empty()) return nullptr;

    auto& qdlayers = scopelayer[templtclass].qdlayers_map[anim];

    if (qdlayers.aolist_cache.find(grp_num) == qdlayers.aolist_cache.end())
    {
        Vec<const std::string*>& aolist = qdlayers.aolist_cache[grp_num];

        for (auto& animobj : *animobj_list)
        {
            aolist.emplace_back(&animobj);
        }
    }

    return std::make_unique<ScopeIteratorValue<const std::string*>>(&qdlayers.aolist_cache.at(grp_num),
                                                                    qdlayers.aolistlayers[grp_num]);
}
