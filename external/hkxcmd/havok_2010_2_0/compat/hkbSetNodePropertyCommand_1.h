#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableValue_0.h"

class hkbSetNodePropertyCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSetNodePropertyCommand(void) {}
   HK_FORCE_INLINE hkbSetNodePropertyCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_nodeName(flag)
   , m_propertyName(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkStringPtr m_nodeName;
   hkStringPtr m_propertyName;
   hkbVariableValue m_propertyValue;
   hkInt32 m_padding;
};
extern const hkClass hkbSetNodePropertyCommandClass;

