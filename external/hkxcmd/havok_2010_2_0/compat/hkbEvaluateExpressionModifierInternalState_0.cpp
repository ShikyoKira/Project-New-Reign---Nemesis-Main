// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEvaluateExpressionModifierInternalState_0.h"

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


static const hkInternalClassMember hkbEvaluateExpressionModifierInternalStateClass_Members[] =
{
   { "internalExpressionsData",&hkbEvaluateExpressionModifierInternalExpressionDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateExpressionModifierInternalState,m_internalExpressionsData) /*8*/,HK_NULL},
};

// Signature:  b414d58e
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbEvaluateExpressionModifierInternalStateClass;
const hkClass hkbEvaluateExpressionModifierInternalStateClass(
    "hkbEvaluateExpressionModifierInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbEvaluateExpressionModifierInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbEvaluateExpressionModifierInternalStateClass_Members), HK_COUNT_OF(hkbEvaluateExpressionModifierInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbEvaluateExpressionModifierInternalState, hkbEvaluateExpressionModifierInternalState);

