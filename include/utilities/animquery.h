#pragma once

#include "utilities/option.h"
#include "utilities/templatecategory.h"

namespace nemesis
{
    struct AnimQueryFile;

	struct AnimQuery : public std::enable_shared_from_this<AnimQuery>
    {
    private:
        static nemesis::regex animobj_rgx;

        const nemesis::AnimQueryFile& querylist;
        const nemesis::TemplateCategory& animclass;
        Vec<UPtr<nemesis::Option>> options;
        UMap<std::string, Vec<const nemesis::Option*>> optionsmap;

        // to keep objects in children alive 
        Vec<SPtr<const nemesis::AnimQuery>> sharedarray;
        SPtr<Vec<const nemesis::AnimQuery*>> array = std::make_shared<Vec<const nemesis::AnimQuery*>>();

        nemesis::Line fullquery;
        std::string animname;
        std::filesystem::path animpath;
        UMap<int, VecStr> animobjects;
        size_t index;
        size_t arrayindex = 0;
        size_t behaviorindex;

        VecStr motiondata;
        VecStr rotationdata;

        bool known = false;

        bool OptionValidation(const UPtr<nemesis::Option>& option);

    public:
        AnimQuery() = default;
        AnimQuery(const nemesis::Line& query,
                  const nemesis::AnimQueryFile& querylist,
                  const nemesis::TemplateCategory& animclass);

        void AddIndex(size_t index) noexcept;
        void AddBehaviorIndex(size_t index) noexcept;
        void AddAnimObjects(const std::string& animobject);
        void AddQueryToGroup(nemesis::AnimQuery& query);
        void AddOptions(const std::string& optionquery);
        void AddOption(const std::string& option);
        void AddAnimName(const std::string& animname) noexcept;
        void AddAnimPath(const std::filesystem::path& animpath) noexcept;
        void AddMotion(const std::string& translation);
        void AddRotation(const std::string& translation);

        void AddSelfToArray() noexcept;

        bool HasOption(const UPtr<nemesis::Option>& option) const noexcept;
        bool HasOption(const std::string& optionname) const noexcept;

        const Vec<const nemesis::Option*>* GetOptionListPtr(const std::string& optionname) const;
        const nemesis::Option* GetOptionPtr(const std::string& optionname, size_t offset = 0) const;

        const nemesis::Template* MatchTemplate(USet<const nemesis::Template*>& templateset) const noexcept;

        const VecStr& GetMotion() const;
        const VecStr& GetRotation() const;
        const std::filesystem::path& GetAnimPath() const;
        std::string_view GetAnimationName() const;

        const VecStr& GetAnimObject(size_t num) const;
        const std::string& GetAnimObject(size_t num, size_t index) const;

        size_t GetArrayNum() const noexcept;
        const nemesis::AnimQuery& GetNextInArray() const noexcept;
        const nemesis::AnimQuery& GetBackInArray() const noexcept;
        const nemesis::AnimQuery& GetLastInArray() const noexcept;
        const nemesis::AnimQuery& GetArrayQuery(size_t index) const;
        const Vec<const nemesis::AnimQuery*>& GetArray() const;
        size_t GetArraySize() const noexcept;

        size_t GetIndex() const noexcept;
        size_t GetArrayIndex() const noexcept;
        size_t GetBehaviorIndex() const noexcept;
        const nemesis::TemplateCategory& GetAnimClass() const noexcept;
        const nemesis::AnimQueryFile& GetQueryList() const noexcept;

        bool IsKnown() const noexcept;
        bool IsLast() const noexcept;
        bool IsFirst() const noexcept;
    };
}
