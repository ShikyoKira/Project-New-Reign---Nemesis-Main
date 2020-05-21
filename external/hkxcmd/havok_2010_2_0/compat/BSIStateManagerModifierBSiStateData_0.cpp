// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSIStateManagerModifierBSiStateData_0.h"

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


static const hkInternalClassMember BSIStateManagerModifierBSiStateDataClass_Members[] =
{
   { "pStateMachine",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIStateManagerModifierBSiStateData,m_pStateMachine) /*0*/,HK_NULL},
   { "StateID",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIStateManagerModifierBSiStateData,m_StateID) /*4*/,HK_NULL},
   { "iStateToSetAs",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIStateManagerModifierBSiStateData,m_iStateToSetAs) /*8*/,HK_NULL},
};

// Signature:  6b8a15fc
extern const hkClass BSIStateManagerModifierBSiStateDataClass;
const hkClass BSIStateManagerModifierBSiStateDataClass(
    "BSIStateManagerModifierBSiStateData",
    HK_NULL, // parent
    sizeof(BSIStateManagerModifierBSiStateData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSIStateManagerModifierBSiStateDataClass_Members), HK_COUNT_OF(BSIStateManagerModifierBSiStateDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(BSIStateManagerModifierBSiStateData, BSIStateManagerModifierBSiStateData);

