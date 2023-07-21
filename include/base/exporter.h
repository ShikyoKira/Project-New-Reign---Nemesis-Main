#pragma once

#include <regex>
#include <chrono>

#include "base/file.h"

#include "utilities/types.h"
#include "utilities/atomic.h"

#include "hkx/HkxVariableList.h"
#include "hkx/HkxAttributeList.h"
#include "hkx/HkxEventList.h"
#include "hkx/numelement.h"

#include "core/animimport.h"

namespace nemesis
{
    struct Process;
    struct AnimQuery;
    struct ScopeInfo;
    struct TemplateCategory;
    struct Template;
    struct AnimTemplate;

	struct Exporter
    {
    private:
        static std::regex lexing_rgx;

    protected:
        struct NodeIdList
        {
        private:
            mutable UMap<std::string, std::string> nodeidlist;

            std::string GetNewId(nemesis::Exporter& exporter) const;

        public:
            std::string_view GetNodeId(const std::string& key, nemesis::Exporter& exporter) const;
        };

        struct NodeIdByQuery
        {
        private:
            mutable UMap<const nemesis::AnimQuery*, NodeIdList> nodemap;

        public:
            const NodeIdList* GetByQuery(const nemesis::AnimQuery* animquery) const;
        };

        struct NodeIdByFile
        {
        private:
            UMap<const nemesis::File*, NodeIdByQuery> nodemap;

        public:
            const NodeIdByQuery* GetByFile(const nemesis::File* file);
            const NodeIdByQuery* GetByFile(const nemesis::File* file) const;
        };

        UPtr<nemesis::HkxEventList> events;
        UPtr<nemesis::HkxVariableList> variables;
        UPtr<nemesis::HkxAttributeList> attibutes;

        Vec<UPtr<nemesis::NumElement>> numelement_layer;
        Vec<Pair<std::string, UPtr<nemesis::AnimImport>>> animimportpair_list;

        const nemesis::File& file;
        nemesis::Atomic<NodeIdByFile> nodeidlist;
        nemesis::Atomic<NodeIdList> import_nodeidlist;
        nemesis::Atomic<size_t> topid;

        SPtr<nemesis::ScopeInfo> scopeinfo;

        Exporter(const nemesis::File& file);

        void CheckNumElement(std::string& line);
        void RenewScopeInfo();
        bool ContainsAnimImport(const std::string& key);

    public:
        void SetCurrentQuery(const nemesis::AnimQuery& query);
        void AddEvents();
        void AddVariables();
        void AddAttributes();
        void AddAnimImport(const std::string& key, UPtr<nemesis::AnimImport>& animimport_ptr);
        void SetInnerExporter(nemesis::Exporter& exporter);
        
        bool IsBehavior() const;

        std::string_view
        GetID(const std::string& key, const nemesis::File* file, const nemesis::AnimQuery* animquery);
        std::string_view GetImportID(const std::string& key);
        const nemesis::HkxEvent* GetHkxEventPtr(const std::string& eventname) const;
        const nemesis::HkxVariable* GetVariableIDPtr(const std::string& varname) const;
        const nemesis::File& GetFile() const;

        virtual const nemesis::AnimTemplate* GetAnimTemplate()                                      = 0;
        virtual const nemesis::TemplateCategory* GetTemplateCategory(const std::string& name)             = 0;
        virtual void Export()                                                                       = 0;
        virtual bool IsTemplateActive(const std::string& name)                                      = 0;
        virtual const Vec<const nemesis::AnimQuery*>* GetQueriesByTemplate(const std::string& name) = 0;

        friend Process;
    };
}
