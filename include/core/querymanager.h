#pragma once

namespace nemesis
{
    struct AnimQuery;
    struct AnimQueryFile;
    struct TemplateClass;

	struct QueryManager
    {
    private:
        Vec<SPtr<const AnimQueryFile>> filelist;
        Vec<const AnimQueryFile*> fileptr_list;
        Map<const TemplateClass*, Vec<const AnimQuery*>> template_querylist;

    public:
        void AddFile(SPtr<const AnimQueryFile> fileptr);

        const Vec<const AnimQuery*>* GetQueryList(const TemplateClass* templtclass) const;
        const Vec<const AnimQueryFile*>& GetFileList() const noexcept;
    };
}
