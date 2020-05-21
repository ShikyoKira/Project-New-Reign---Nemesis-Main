// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbIntVariableSequencedData_0.h"

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


static const hkInternalClassMember hkbIntVariableSequencedDataClass_Members[] =
{
   { "samples",&hkbIntVariableSequencedDataSampleClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbIntVariableSequencedData,m_samples) /*8*/,HK_NULL},
   { "variableIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbIntVariableSequencedData,m_variableIndex) /*20*/,HK_NULL},
};

// Signature:  7bfc518a
extern const hkClass hkbSequencedDataClass;
extern const hkClass hkbIntVariableSequencedDataClass;
const hkClass hkbIntVariableSequencedDataClass(
    "hkbIntVariableSequencedData",
    &hkbSequencedDataClass, // parent
    sizeof(hkbIntVariableSequencedData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbIntVariableSequencedDataClass_Members), HK_COUNT_OF(hkbIntVariableSequencedDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbIntVariableSequencedData, hkbIntVariableSequencedData);

