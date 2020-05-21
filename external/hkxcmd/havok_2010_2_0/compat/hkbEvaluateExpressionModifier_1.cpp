// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEvaluateExpressionModifier_1.h"

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


static const hkInternalClassMember hkbEvaluateExpressionModifierClass_Members[] =
{
   { "expressions",&hkbExpressionDataArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateExpressionModifier,m_expressions) /*44*/,HK_NULL},
   { "compiledExpressionSet",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateExpressionModifier,m_compiledExpressionSet) /*48*/,HK_NULL},
   { "internalExpressionsData",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateExpressionModifier,m_internalExpressionsData) /*52*/,HK_NULL},
};

// Signature:  f900f6be
extern const hkClass hkbModifierClass;
extern const hkClass hkbEvaluateExpressionModifierClass;
const hkClass hkbEvaluateExpressionModifierClass(
    "hkbEvaluateExpressionModifier",
    &hkbModifierClass, // parent
    sizeof(hkbEvaluateExpressionModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbEvaluateExpressionModifierClass_Members), HK_COUNT_OF(hkbEvaluateExpressionModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbEvaluateExpressionModifier, hkbEvaluateExpressionModifier);

