/*

THE DESIGN PHILOSOPHY OF SCOPE INFO
- MUST BE CONST
- CONTAINS TEMPORARY INFORMATION
- ONLY EXIST 1 AT A TIME
- LIFE TIME LAST UNTIL IT GOES OUT OF SCOPE
- EXISTS ONLY IN TEMPLATE

The idea of scope info is to store temporary information that can be used
as parameters in nemesis functions within the scope, which includes
conditions that will create another nested scope and so on

*/
#pragma once

#include "core/animimport.h"
#include "core/multichoice.h"

#include "scope/layers.h"
#include "scope/stateidmanager.h"

#include "utilities/noncopyable.h"

#include "hkx/varid.h"
#include "hkx/eventid.h"

namespace nemesis
{
    struct Option;
    struct Process;
    struct Template;
    struct Exporter;
    struct AnimQuery;
    struct AnimQueryFile;
    struct AnimVarPtr;
    struct Condt;
    struct HkxBehavior;

	struct ScopeInfo
    {
    private:
        struct TemporaryScope
        {
        protected:
            ScopeInfo* scopeinfo;

        public:
            virtual ScopeInfo& GetScopeInfo() = 0;
        };

    public:
        struct ScopeIterator
        {
            virtual ~ScopeIterator() {}
            virtual bool IsEnd() = 0;
            virtual void Next()  = 0;
        };

        template<typename T>
        struct TempScopeLayer : public NonCopyableStruct, public TemporaryScope
        {
        private:
        public:
            TempScopeLayer(const SPtr<T>& tempvalue, ScopeInfo& scopeinfo)
            {
                this->scopeinfo = &scopeinfo;
            }

            ScopeInfo& GetScopeInfo() override
            {
                return *scopeinfo;
            }
        };

        struct ChoiceRange
        {
            size_t front;
            size_t back;

            ChoiceRange(size_t posA, size_t posB);
        };

        //struct AnimObjects
        //{
        //private:
        //    VecStr& aolist;

        //    // animation object layer
        //    Vec<const std::string*> animobjlayers;

        //    WPtr<const std::string*> animobj_temp;

        //public:
        //    AnimObjects(VecStr& aolist);

        //    void SetTempAnimObject(SPtr<const std::string*>& animobj) noexcept;
        //    void InsertAnimObject(const std::string& animobj);
        //    void DropAnimObject() noexcept;

        //    SPtr<const std::string*> GetTempAnimObject(size_t index) const;
        //    const std::string& GetAnimObject() const;
        //};

        /*
        CORRECT USAGE

        for (auto curindex = ExeQuery(scopeinfo); curindex.IsEnd(); curinedx.Next())
        {
            <do something>
        }
        */
        template<typename ValueType>
        struct ScopeIteratorValue : public ScopeIterator
        {
        private:
            size_t index = 0;
            const Vec<ValueType>* list_ptr = nullptr;
            nemesis::Layers<ValueType>* currentlayer = nullptr;

        public:
            ScopeIteratorValue() = default;
            ScopeIteratorValue(const Vec<ValueType>* list, nemesis::Layers<ValueType>& layer)
            {
                SetListPtr(list);
                SetCurrentLayer(layer);
            }
            
            ~ScopeIteratorValue()
            {
                if (!currentlayer) return;

                currentlayer->DropLayer();
            }

            bool IsEnd()
            {
                if (error) throw nemesis::exception();

                return !list_ptr || index >= list_ptr->size();
            }

            void Next()
            {
                index++;

                if (IsEnd()) return;

                currentlayer->UpdateLayer(list_ptr->at(index));
            }

            void SetListPtr(const Vec<ValueType>* list)
            {
                list_ptr = list;
            }

            void SetCurrentLayer(nemesis::Layers<ValueType>& layer)
            {
                currentlayer = &layer;
                currentlayer->AddLayer(list_ptr->at(index));
            }
        };

        struct ScopeLayer
        {
            struct QueryDataLayers
            {
                // option layer
                UMap<std::string, nemesis::Layers<const nemesis::Option*>> optionlayers;

                // animation object list layer
                UMap<size_t, nemesis::Layers<const std::string*>> aolistlayers;
                UMap<size_t, Vec<const std::string*>> aolist_cache;
            };

            // query layer
            nemesis::Layers<const nemesis::AnimQuery*> querylayers;

            // anim layer (within each query), specifically for grouped animations that use +
            nemesis::Layers<const nemesis::AnimQuery*> animlayers;

            QueryDataLayers base_qdlayers;
            UMap<const nemesis::AnimQuery*, QueryDataLayers> qdlayers_map;
        };

        nemesis::Exporter* exporter = nullptr;

        bool negative        = false;
        bool isend           = false;
        bool catchingelement = false;
        bool norelment       = false;
        bool hasduration     = false;

        int duration = 0;

        UMap<const nemesis::TemplateClass*, ScopeLayer> scopelayer;
        WPtr<const nemesis::TemplateClass*> temp_templtclass_ptr;

        Vec<SPtr<Vec<const nemesis::TemplateClass*>>> templtclass_cache;

        // class layer
        nemesis::Layers<const nemesis::TemplateClass*> classlayers;

        Vec<nemesis::ILayers*> layerorder;
        //Vec<AnimObjects*> ao_layerorder;

        Vec<const MultiChoice::Choice*> failed;
        const MultiChoice::Choice* captured;

        int elementcounter = 0;
        std::string* elementline = nullptr;
        std::string_view nodeid_sv;
        size_t animcounter;

        UPtr<nemesis::StateIdManager> stateidmanager;

        nemesis::AnimImport* currentimport_ptr = nullptr;

        const nemesis::AnimQueryFile* querylist = nullptr;
        const nemesis::AnimQuery* basequery     = nullptr;

        const nemesis::Line* lineptr     = nullptr;
        nemesis::Condition* conditionptr = nullptr;
        nemesis::Process* processptr     = nullptr;

        ScopeInfo() = default;

        template <typename T>
        UPtr<nemesis::ScopeInfo::TemporaryScope> GetTemp(SPtr<T>& shptr)
        {
            return std::make_unique<nemesis::ScopeInfo::TempScopeLayer<T>>(shptr, *this);
        }

        const Vec<const nemesis::AnimQuery*>*
        GetTemplateAllQueries(const nemesis::TemplateClass* templtclass) const;

        const Vec<const nemesis::AnimQuery*>*
        GetTemplateAllAnim(const nemesis::Condition& condition) const;

        
        const nemesis::Option* GetCurrentOptionPtr(const std::string& optionname,
                                                   size_t index,
                                                   const nemesis::TemplateClass* templtclass) const;
        nemesis::Layers<const nemesis::Option*>& GetOptionLayers(const nemesis::Option& opt,
                                                                 const nemesis::TemplateClass* templtclass);
        nemesis::Layers<const std::string*>& GetAnimObjListLayers(size_t animobj_grp,
                                                            const nemesis::AnimQuery* animquery,
                                                            const nemesis::TemplateClass* templtclass);
        const ScopeLayer::QueryDataLayers*
        GetCurrentQueryDataLayers(const nemesis::TemplateClass* templtclass) const;

        void ExeTempQuery(SPtr<const AnimQuery*>& shptr,
                          const nemesis::TemplateClass* templtclass,
                          std::function<void()> callback);
        void ExeTempAnim(SPtr<const AnimQuery*>& shptr,
                         const nemesis::TemplateClass* templtclass,
                         std::function<void()> callback);

    public:
        bool TrySetNodeId(nemesis::Line& line);

        void SetCurrentImport(nemesis::AnimImport* animimport_ptr);
        void SetCurrentLine(nemesis::Line* lineptr);
        void SetCurrentLine(const nemesis::Line* lineptr);
        void SetCurrentCondition(nemesis::Condition* conditionptr);
        void SetCurrentProcess(nemesis::Process* processptr);
        void SetAnimCounter(size_t animcounter);
        void SetCurrentQuery(const nemesis::AnimQuery& query);

        void SetTempTemplateClass(SPtr<const nemesis::TemplateClass*> templtclass_ptr) noexcept;
        void SetTempQuery(SPtr<const nemesis::AnimQuery*>& index,
                          const nemesis::TemplateClass* templtclass) noexcept;
        void SetTempAnim(SPtr<const nemesis::AnimQuery*>& anim,
                         const nemesis::TemplateClass* templtclass) noexcept;
        void SetTempOption(SPtr<const nemesis::Option*>& opt,
                           const nemesis::TemplateClass* templtclass) noexcept;
        void SetTempAnimObject(SPtr<const std::string*>& animobj,
                               size_t animobj_grp,
                               const nemesis::AnimQuery* animquery,
                               const nemesis::TemplateClass* templtclass) noexcept;
        
        template <typename T>
        void Insert(nemesis::Layers<T>& layers, T value)
        {
            layers.AddLayer(value);
            layerorder.emplace_back(&layers);
        }
        
        void InsertQuery(const nemesis::AnimQuery& index, const nemesis::TemplateClass* templtclass);
        void InsertAnim(const nemesis::AnimQuery& anim, const nemesis::TemplateClass* templtclass);
        void InsertOption(const nemesis::Option& opt, const nemesis::TemplateClass* templtclass);
        void InsertAnimObject(const std::string& animobj,
                              size_t animobj_grp,
                              const nemesis::AnimQuery* animquery,
                              const nemesis::TemplateClass* templtclass);

        void DropLayer() noexcept;
        
        bool IsTemplateActive(const std::string& name) const;
        bool HasQuery(size_t index) const;
        bool HasAnim(size_t index, const nemesis::TemplateClass* templtclass) const;
        bool HasOption(std::string optionname, size_t index, const nemesis::TemplateClass* templtclass) const;

        void ExeTempNumQuery(size_t index,
                             const nemesis::TemplateClass* templtclass,
                             std::function<void()> callback);
        void
        ExeTempNumAnim(size_t index, const nemesis::TemplateClass* templtclass, std::function<void()> callback);
        void ExeTempOption(const nemesis::Option& opt,
                           const nemesis::TemplateClass* templtclass,
                           std::function<void()> callback);
        void ExeTempAnimObject(size_t animobj_grp,
                               size_t animobj_index,
                               const nemesis::TemplateClass* templtclass,
                               std::function<void()> callback);

        void ExeTempBaseQuery(const nemesis::TemplateClass* templtclass, std::function<void()> callback);

        void ExeTempBackAnim(const nemesis::TemplateClass* templtclass, std::function<void()> callback);
        void ExeTempNextAnim(const nemesis::TemplateClass* templtclass, std::function<void()> callback);
        void ExeTempLastAnim(const nemesis::TemplateClass* templtclass, std::function<void()> callback);

        const nemesis::TemplateClass* GetTemplateClass() const;
        const nemesis::AnimQuery* GetQuery(const nemesis::TemplateClass* templtclass) const;
        const nemesis::AnimQuery* GetAnim(const nemesis::TemplateClass* templtclass) const;
        const nemesis::Option* GetOption(const std::string& name,
                                         const nemesis::TemplateClass* templtclass) const;
        const std::string* GetAnimObject(size_t animobj_grp,
                                         const nemesis::TemplateClass* templtclass) const;

        const nemesis::AnimImport* GetCurrentImport() const;
        std::string_view GetFormat() const;
        std::filesystem::path GetFile() const;
        size_t GetCurrentLineNum() const;
        std::string_view GetCurrentNodeId() const;

        const nemesis::AnimQuery* GetLastAnim(const nemesis::TemplateClass* templtclass) const;
        const nemesis::AnimQuery* GetBackAnim(const nemesis::TemplateClass* templtclass) const;
        const nemesis::AnimQuery* GetNextAnim(const nemesis::TemplateClass* templtclass) const;
        const nemesis::AnimQuery* GetCurrentQuery() const;
        const nemesis::Option* GetBaseOptionPtr(const std::string& optionname,
                                                size_t index,
                                                const nemesis::TemplateClass* templtclass) const;
        const nemesis::Option* GetOptionPtr(const std::string& optionname,
                                            const nemesis::AnimQuery* animquery,
                                            const nemesis::TemplateClass* templtclass) const;
        const nemesis::Option* GetOptionPtr(const std::string& optionname,
                                            size_t index,
                                            const nemesis::TemplateClass* templtclass) const;
        const Vec<const nemesis::Option*>* GetOptionListPtr(const std::string& optionname,
                                                            const nemesis::TemplateClass* templtclass) const;
        
        void GenerateStateIdManager(const nemesis::HkxBehavior& behavior);
        nemesis::StateIdManager* GetStateIdManager();

        const nemesis::EventId& GetEventID(const std::string& eventname) const;
        const nemesis::VarId& GetVariableID(const std::string& varname) const;

        nemesis::Exporter* GetExporter();

        UPtr<ScopeIterator> ExeQuery(const Condition& condition);
        UPtr<ScopeIterator> ExeMasterQuery(const Condition& condition);
        UPtr<ScopeIterator> ExeGroupQuery(const Condition& condition);
        UPtr<ScopeIterator> ExeAnimQuery(const Condition& condition);
        UPtr<ScopeIterator> ExeAnimObjectQuery(const AnimVarPtr& var);
        
        #pragma region STATIC
        static constexpr std::string_view nodeid_search_sv1 = "<hkobject name=\"#";
        static constexpr std::string_view nodeid_search_sv2 = "\" class=\"";
        #pragma endregion STATIC

        friend Process;
        friend Exporter;
        friend Condition;
    };
}
