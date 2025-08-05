#pragma once

#include "Core/Statement/Statement.h"

namespace nemesis
{
    struct LineModifier;

	struct CompositeStatement : public nemesis::Statement
    {
    private:
        const nemesis::AnimationRequest& GetAnimationRequest(const std::string& index_str,
                                                             const std::string& templt_name,
                                                             size_t templt_num,
                                                             nemesis::CompileState& state,
                                                             nemesis::SemanticManager& manager) const;

    protected:
        struct DynamicComponent
        {
        private:
            std::string RawValue;
            VecStr Tokens;
            Vec<SPtr<nemesis::LineModifier>> Modifiers;

        public:
            DynamicComponent(const std::string& component,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);

            std::string GetValue(nemesis::CompileState& state) const;
            bool IsDynamic() const;
        };

        Deq<DynamicComponent> DynamicComponents;

        static bool IsComplexComponent(const std::string& component);

        SPtr<std::function<bool(nemesis::CompileState&)>> CallbackTargetRequests(
            const nemesis::TemplateClass& templt_class,
            const nemesis::SemanticManager& manager,
            const std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest&)>& callback)
            override;

        SPtr<std::function<const nemesis::AnimationRequest&(nemesis::CompileState&)>>
        GetTargetRequest(const nemesis::TemplateClass& templt_class,
                         const nemesis::SemanticManager& manager) override;

    public:
        CompositeStatement(const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath);

        std::string Serialize() const override;
    };
}
