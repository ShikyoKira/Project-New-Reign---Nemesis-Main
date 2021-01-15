#pragma once

#include <string>

#include "utilities/linkedvar.h"

struct AnimSetPack
{
    using AttackData = std::string;
    using ModCode = std::string;

private:
    class Parser
    {
    private:
        short type    = 0;
        int linenum   = 1;
        uint asdCount = 0;

        ModCode format;
        std::filesystem::path path;

        AnimSetPack& host;
        const VecStr& storeline;
        SPtr<nemesis::ConditionInfo> tobedeleted;

    public:
        Parser(AnimSetPack& base, const VecStr& storeline);

        void SetFormat(const ModCode& format);
        void SetPath(const std::filesystem::path& path);

        void AddAnimSetPack();
        void ImportAnimSetPack();

    private:

    };

    nemesis::LinkedVar<std::string> name;
    nemesis::LinkedVar<std::string> unknown;
    Vec<nemesis::LinkedVar<AttackData>> attackSetData;

public:
    AnimSetPack() = default;
    AnimSetPack(const VecStr& storeline);
    AnimSetPack(const VecStr& storeline, const std::string& path, const ModCode& modcode);
    AnimSetPack(const nemesis::Line& _name,
                const nemesis::Line& _unknown,
                const Vec<AttackData>& _attackSetData);

    bool AddName(const nemesis::Line& _name);
    bool AddUnknown(const nemesis::Line& _unknown);
    void AddAttackSetData(const nemesis::Line& _attackSetData);

    void getlines(VecStr& storeline);

    friend struct AnimSetData;
};

void getLinkedLines(const nemesis::LinkedVar<AnimSetPack>& linkedtype, VecStr& storeline);
