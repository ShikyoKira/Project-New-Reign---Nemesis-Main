// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbPoweredRagdollControlsModifier_5.h"

#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Version/hkVersionPatchManager.h>
#include <Common/Serialize/Data/Dict/hkDataObjectDict.h>
#include <Common/Serialize/Data/Native/hkDataObjectNative.h>
#include <Common/Serialize/Data/Util/hkDataObjectUtil.h>
#include <Common/Base/Reflection/Registry/hkDynamicClassNameRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Serialize/Util/hkSerializationCheckingUtils.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>


static const hkInternalClassMember hkbPoweredRagdollControlsModifierClass_Members[] =
{
   { "controlData",&hkbPoweredRagdollControlDataClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlsModifier,m_controlData) /*48*/,HK_NULL},
   { "bones",&hkbBoneIndexArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlsModifier,m_bones) /*80*/,HK_NULL},
   { "worldFromModelModeData",&hkbWorldFromModelModeDataClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlsModifier,m_worldFromModelModeData) /*84*/,HK_NULL},
   { "boneWeights",&hkbBoneWeightArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlsModifier,m_boneWeights) /*92*/,HK_NULL},
};

// Signature:  7cb54065
extern const hkClass hkbModifierClass;
extern const hkClass hkbPoweredRagdollControlsModifierClass;
const hkClass hkbPoweredRagdollControlsModifierClass(
    "hkbPoweredRagdollControlsModifier",
    &hkbModifierClass, // parent
    sizeof(hkbPoweredRagdollControlsModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbPoweredRagdollControlsModifierClass_Members), HK_COUNT_OF(hkbPoweredRagdollControlsModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    5 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbPoweredRagdollControlsModifier, hkbPoweredRagdollControlsModifier);

//#pragma warning (disable : 4430 )
//#define ASSERT_CONCAT_(a, b) a##b
//#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
///* These can't be used after statements in c89. */
//#ifdef __COUNTER__
//  /* microsoft */
//  #define STATIC_ASSERTMSG(e,m) \
//  namespace { enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(int)(!!(e)) }; }
//
//  #define STATIC_ASSERT(e) STATIC_ASSERTMSG(##e, #e)
//#else
//  /* This can't be used twice on the same line so ensure if using in headers
//   * that the headers are not included twice (by wrapping in #ifndef...#endif)
//   * Note it doesn't cause an issue when used on same line of separate modules
//   * compiled with gcc -combine -fwhole-program.  */
//  #define STATIC_ASSERT(e,m) \
//    enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }
//#endif
//namespace
//{
//   template<bool> struct StaticAssert;
//   template<> struct StaticAssert<true> {};
//   #define STATIC_ASSERT(condition) void func() { do { StaticAssert<(condition)>(); } while(0); }
//
//   STATIC_ASSERT(sizeof(hkbPoweredRagdollControlsModifier) == 96)
//}
