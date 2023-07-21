#pragma once

#include "utilities/types.h"

namespace nemesis
{
    struct TemplateClass;
    struct TemplateObject;

	struct SemanticManager
    {
    private:
        const nemesis::TemplateClass* TemplateClass = nullptr;
        const nemesis::TemplateObject* TemplateObject = nullptr;

        VecStr OptionQueue;
        VecStr RequestQueue;
        VecStr MapQueue;

        USetStr ModInUsedList;

    public:
        void SetCurrentTemplateClass(const nemesis::TemplateClass* template_class) noexcept;
        const nemesis::TemplateClass* GetCurrentTemplateClass() const noexcept;

        void SetCurrentTemplate(const nemesis::TemplateObject* template_object) noexcept;
        const nemesis::TemplateObject* GetCurrentTemplate() const noexcept;

        bool TryAddOptionToQueue(const std::string& option_name, const std::string& option_syntax);
        bool HasOptionInQueue(const std::string& option_syntax) const;
        bool TryRemoveLastOption() noexcept;

        bool TryAddRequestToQueue(const std::string& group);
        bool HasRequestInQueue(const std::string& group) const;
        bool TryRemoveLastRequest() noexcept;

        bool TryAddMapToQueue(const std::string& key);
        bool HasMapInQueue(const std::string& key) const;
        bool TryRemoveLastMap(const std::string& key);

        void AddModInUsed(const std::string& mod_code);
        const USetStr& GetModInUsedList() const noexcept;
    };
}
