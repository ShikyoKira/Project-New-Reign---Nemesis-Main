#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbTwistModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbTwistModifier(void) {}
   HK_FORCE_INLINE hkbTwistModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_boneChainIndices(flag)
   , m_parentBoneIndices(flag)
      {}
   enum SetAngleMethod
   {
      LINEAR = 0,
      RAMPED = 1,
   };

   enum RotationAxisCoordinates
   {
      ROTATION_AXIS_IN_MODEL_COORDINATES = 0,
      ROTATION_AXIS_IN_LOCAL_COORDINATES = 1,
   };

   // Properties
   hkVector4 m_axisOfRotation;
   hkReal m_twistAngle;
   hkInt16 m_startBoneIndex;
   hkInt16 m_endBoneIndex;
   hkEnum<SetAngleMethod,hkInt8> m_setAngleMethod;
   hkEnum<RotationAxisCoordinates,hkInt8> m_rotationAxisCoordinates;
   hkBool m_isAdditive;
   hkArray<hkRefVariant> m_boneChainIndices;
   hkArray<hkRefVariant> m_parentBoneIndices;
};
extern const hkClass hkbTwistModifierClass;

