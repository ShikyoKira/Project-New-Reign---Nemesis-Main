#include "core/querymanager.h"

#include "utilities/animqueryfile.h"

void nemesis::QueryManager::AddFile(SPtr<const AnimQueryFile> fileptr)
{
    filelist.emplace_back(fileptr);
    fileptr_list.emplace_back(fileptr.get());

    for (auto& query : fileptr->GetList())
    {
        template_querylist[&query->GetAnimClass()].emplace_back(query);
    }
}

const Vec<const nemesis::AnimQuery*>*
nemesis::QueryManager::GetQueryList(const TemplateCategory* templtclass) const
{
    auto itr = template_querylist.find(templtclass);

    if (itr == template_querylist.end()) return nullptr;

    return &template_querylist.at(templtclass);
}

const Vec<const nemesis::AnimQueryFile*>& nemesis::QueryManager::GetFileList() const noexcept
{
    return fileptr_list;
}
