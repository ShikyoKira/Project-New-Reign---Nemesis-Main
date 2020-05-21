// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbAttributeModifierAssignment_0.h"

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


static const hkInternalClassMember hkbAttributeModifierAssignmentClass_Members[] =
{
   { "attributeIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttributeModifierAssignment,m_attributeIndex) /*0*/,HK_NULL},
   { "attributeValue",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttributeModifierAssignment,m_attributeValue) /*4*/,HK_NULL},
};

// Signature:  48b8ad52
extern const hkClass hkbAttributeModifierAssignmentClass;
const hkClass hkbAttributeModifierAssignmentClass(
    "hkbAttributeModifierAssignment",
    HK_NULL, // parent
    sizeof(hkbAttributeModifierAssignment),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbAttributeModifierAssignmentClass_Members), HK_COUNT_OF(hkbAttributeModifierAssignmentClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbAttributeModifierAssignment, hkbAttributeModifierAssignment);

