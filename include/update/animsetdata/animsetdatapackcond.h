#pragma once

#include <string>

#include "utilities/linkedvar.h"

struct AnimSetPack
{
    using AttackData = std::string;
    using ModCode = std::string;

private:
    nemesis::LinkedVar<std::string> name;
    nemesis::LinkedVar<std::string> unknown;
    std::vector<nemesis::LinkedVar<AttackData>> attackSetData;

public:
    AnimSetPack() {}
    AnimSetPack(const VecStr& storeline);
    AnimSetPack(const VecStr& storeline, const std::string& path, const ModCode& modcode);
    AnimSetPack(const std::string& _name,
                const std::string& _unknown,
                const std::vector<AttackData>& _attackSetData);

    void getlines(VecStr& storeline);

    friend struct AnimSetData;
};

void getLinkedLines(const nemesis::LinkedVar<AnimSetPack>& linkedtype, VecStr& storeline);
