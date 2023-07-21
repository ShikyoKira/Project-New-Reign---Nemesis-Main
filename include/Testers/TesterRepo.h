#pragma once

#include "Testers/HkxNodeTester.h"
#include "Testers/NObjectTester.h"
#include "Testers/TemplateTester.h"
#include "Testers/ModifierTester.h"
#include "Testers/HkxBehaviorTester.h"
#include "Testers/MasterRepositoryTester.h"

namespace nemesis
{
    void RunAllTest()
    {
        //nemesis::ModifierTester::Run();
        //nemesis::NObjectTester::Run();
        //nemesis::HkxNodeTester::Run();
        //nemesis::HkxBehaviorTester::Run();
        nemesis::TemplateTester::Run();
        nemesis::MasterRepositoryTester::Run();
    }
}
