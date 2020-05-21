#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableValue_0.h"
#include "hkbVariableInfo_1.h"

class hkbSetWordVariableCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSetWordVariableCommand(void) {}
   HK_FORCE_INLINE hkbSetWordVariableCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_quadValue;
   hkUint64 m_characterId;
   hkInt32 m_variableId;
   hkbVariableValue m_value;
   hkEnum<hkbVariableInfo::VariableType,hkUint8> m_type;
   hkBool m_global;
};
extern const hkClass hkbSetWordVariableCommandClass;

