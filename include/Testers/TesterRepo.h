#pragma once

#include "Testers/HkxNodeTester.h"
#include "Testers/NObjectTester.h"
#include "Testers/TemplateTester.h"
#include "Testers/ModifierTester.h"
#include "Testers/HkxBehaviorTester.h"
#include "Testers/MasterRepositoryTester.h"
#include "Testers/AnimationSetDataStateTester.h"
#include "Testers/AnimationSetDataProjectTester.h"
#include "Testers/AnimationSetDataSingleFileTester.h"
#include "Testers/AnimationDataSingleFileTester.h"

namespace nemesis
{
    void RunAllTest()
    {
        //nemesis::ModifierTester::Run();
        //nemesis::NObjectTester::Run();
        //nemesis::HkxNodeTester::Run();
        //nemesis::HkxBehaviorTester::Run();
        //nemesis::TemplateTester::Run();
        //nemesis::MasterRepositoryTester::Run();

        nemesis::AnimationSetDataStateTester::Run();
        nemesis::AnimationSetDataProjectTester::Run();
        nemesis::AnimationSetDataSingleFileTester::Run();

        nemesis::AnimationDataSingleFileTester::Run();
    }
}
