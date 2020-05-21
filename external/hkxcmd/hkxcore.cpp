#include "stdafx.h"

#pragma region Havok Headers
//////////////////////////////////////////////////////////////////////////
// Havok Includes
//////////////////////////////////////////////////////////////////////////

#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/hkBase.h>

#include <Common/Base/Reflection/Registry/hkDefaultClassNameRegistry.h>
#include <Common/Serialize/Util/hkStaticClassNameRegistry.h>

#include <cstdio>
#include <iostream>

// Compatibility
#include <Common/Compat/hkCompat.h>

// Scene
#include <Common/SceneData/Scene/hkxScene.h>

// Serialize
#include <Common/Serialize/Data/hkDataObject.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Version/hkVersionPatchManager.h>

// Physics
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// Animation
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Ragdoll/Controller/PoweredConstraint/hkaRagdollPoweredConstraintController.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaRagdollRigidBodyController.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>

// Reflection
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMember.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>

#define RETURN_FAIL_IF(COND, MSG) \
    HK_MULTILINE_MACRO_BEGIN \
    if (COND) \
    { \
        HK_ERROR(0x53a6a026, MSG); \
        return 1; \
    } \
    HK_MULTILINE_MACRO_END

#pragma endregion

#pragma region Havok Footers
// [id=keycode]
#include <Common/Base/keycode.cxx>
// [id=productfeatures]
// We're using only physics - we undef products even if the keycode is present so
// that we don't get the usual initialization for these products.
#undef HK_FEATURE_PRODUCT_AI
//#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR

// We are using serialization, so we need ReflectedClasses.
// The objects are being saved and then loaded immediately so we know the version of the saved data is the same
// as the version the application is linked with. Because of this we don't need RegisterVersionPatches or SerializeDeprecatedPre700.
// If the demo was reading content saved from a previous version of the Havok content tools (common in real world Applications)
// RegisterVersionPatches and perhaps SerializeDeprecatedPre700 are needed.

#define HK_CLASSES_FILE "hkxreg.h"
#include <Common/Base/Config/hkProductFeatures.cxx>
#undef HK_CLASSES_FILE

#define DEFINE_CUSTOMPATCH_STUB(name) \
    static void HK_CALL name([[maybe_unused]] hkDataObject &obj) {}
DEFINE_CUSTOMPATCH_STUB(hkbMoveBoneTowardTargetModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbSplinePathGenerator_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbAttachmentModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbTimerModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbTargetRigidBodyModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbDetectCloseToGroundModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbVariableBindingSetBinding_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbVariableBindingSet_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbNode_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbPositionRelativeSelectorGenerator_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbCheckRagdollSpeedModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbFootIkModifierLeg_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbClipTrigger_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbClipGenerator_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbCatchFallModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbStateMachineStateInfo_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbBehaviorGraphData_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbPoweredRagdollControlsModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbStateMachine_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbKeyframeBonesModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbBlenderGeneratorChild_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbRigidBodyRagdollModifier_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbJigglerGroup_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbStateMachineStateInfo_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbDemoConfig_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbVariableBindingSet_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbClipGenerator_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbRagdollController_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbRagdollModifier_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbPoweredRagdollControlsModifier_3_to_4)
DEFINE_CUSTOMPATCH_STUB(hkbDemoConfig_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbGetUpModifier_1_to_2)
DEFINE_CUSTOMPATCH_STUB(hkbStateMachine_2_to_3)
DEFINE_CUSTOMPATCH_STUB(hkbSceneModifierList_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbBehaviorGraph_0_to_1)
DEFINE_CUSTOMPATCH_STUB(hkbStateMachineStateInfo_3_to_4)
DEFINE_CUSTOMPATCH_STUB(hkbLookAtModifier_2_to_3)
void HK_CALL registerBehaviorPatches() {}

// This automatically registers the patches from the HK_PATCHES_FILE
// with the given hkVersionPatchmanager
void HK_CALL CustomRegisterPatches(hkVersionPatchManager &man)
{
#define HK_PATCHES_FILE <Common/Compat/Patches/hkbPatches.cxx>
#include <Common/Serialize/Version/hkVersionPatchManager.cxx>
#undef HK_PATCHES_FILE
}

#define ASSERT_SIGNATURE(XXX, YYY) \
    if (XXX##Class.getSignature(0) != YYY) \
    { \
        std::cerr << "Class Signature Mismatch: " << #XXX << ' ' << XXX##Class.getSignature(0) << " != " << YYY \
                  << '\n'; \
    }

#define REGISTER_CLASS(XXX) \
    extern const hkClass XXX##Class; \
    extern const hkTypeInfo XXX##TypeInfo; \
    hkDefaultClassNameRegistry::getInstance().registerClass(&XXX##Class); \
    hkTypeInfoRegistry::getInstance().registerTypeInfo(&XXX##TypeInfo);

#define REGISTER_CLASS_W_SIG(XXX, YYY) \
    REGISTER_CLASS(XXX); \
    std::cout << "Class Registered: '" << #XXX << '\n'; \
    ASSERT_SIGNATURE(XXX, YYY);

void HK_CALL ValidateClassSignatures()
{
#define HKCLASS_VERIFY(XXX, YYY) \
    extern const hkClass XXX##Class; \
    ASSERT_SIGNATURE(XXX, YYY);

#include <hkxverify.inl>
#undef HKCLASS_VERIFY
}

void HK_CALL CustomRegisterDefaultClasses()
{
#define HKCLASS_VERIFY(XXX, YYY) REGISTER_CLASS_W_SIG(XXX, YYY)
#include <hkxverify.inl>
#undef HKCLASS_VERIFY
}

namespace hkHavok660r1Classes
{
    extern const hkStaticClassNameRegistry hkHavokDefaultClassRegistry;
}

void HK_CALL CustomRegisterOverrideClasses(hkDynamicClassNameRegistry &registry)
{
    const hkStaticClassNameRegistry **regList = hkVersionRegistry::StaticLinkedClassRegistries;
    for (const hkStaticClassNameRegistry **itr = regList; *itr != nullptr; ++itr)
    {
        const hkStaticClassNameRegistry *oldRegistry = (*itr);
        if (const hkClass *hclass = oldRegistry->getClassByName("hkClassEnum"))
        {
            //if ( hclass->getSignature() == 0x9617a10c )
            if (hclass->getSignature() == 0x9EE66732)
            {
                registry.registerClass(hclass);
                return;
            }
        }
    }
}

#pragma endregion
