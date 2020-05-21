// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkModifierInternalLegData_1.h"

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


static const hkInternalClassMember hkbFootIkModifierInternalLegDataClass_Members[] =
{
   { "groundPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierInternalLegData,m_groundPosition) /*0*/,HK_NULL},
   { "footIkSolver",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifierInternalLegData,m_footIkSolver) /*16*/,HK_NULL},
};

// Signature:  e5ca3677
extern const hkClass hkbFootIkModifierInternalLegDataClass;
const hkClass hkbFootIkModifierInternalLegDataClass(
    "hkbFootIkModifierInternalLegData",
    HK_NULL, // parent
    sizeof(hkbFootIkModifierInternalLegData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbFootIkModifierInternalLegDataClass_Members), HK_COUNT_OF(hkbFootIkModifierInternalLegDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbFootIkModifierInternalLegData, hkbFootIkModifierInternalLegData);

