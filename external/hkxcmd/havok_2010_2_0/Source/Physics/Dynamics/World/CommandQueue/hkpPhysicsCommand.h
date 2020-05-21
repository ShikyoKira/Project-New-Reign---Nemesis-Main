/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHYSICS_COMMAND_H
#define HK_DYNAMICS2_PHYSICS_COMMAND_H

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInfo.h>

	//
	//	The base class of all physics commands
	//
struct hkpPhysicsCommand
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPhysicsCommand );

	enum TYPE
	{
		TYPE_ADD_CONSTRAINT_TO_LOCKED_ISLAND,
		TYPE_REMOVE_CONSTRAINT_FROM_LOCKED_ISLAND,
		TYPE_END
	};
	hkpPhysicsCommand(TYPE type): m_type(type){}

	hkEnum<TYPE,hkUchar> m_type;
};


	//
	//	A small helper template class used for simple single object commands
	//
template<hkpPhysicsCommand::TYPE COMMAND_TYPE, typename OBJECT_TYPE>
struct hkpSingleObjectPhysicsCommand: public hkpPhysicsCommand
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpSingleObjectPhysicsCommand );

	HK_FORCE_INLINE hkpSingleObjectPhysicsCommand( OBJECT_TYPE object ): hkpPhysicsCommand( COMMAND_TYPE ), m_object(object){}
	OBJECT_TYPE m_object;
};

//
//	A small helper template class used for simple dual object commands
//
template<hkpPhysicsCommand::TYPE COMMAND_TYPE, typename OBJECT_TYPE0, typename OBJECT_TYPE1>
struct hkpDualObjectPhysicsCommand: public hkpPhysicsCommand
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpDualObjectPhysicsCommand );

	HK_FORCE_INLINE hkpDualObjectPhysicsCommand( OBJECT_TYPE0 object0, OBJECT_TYPE1 object1 ): hkpPhysicsCommand( COMMAND_TYPE ), m_object0(object0), m_object1(object1){}
	OBJECT_TYPE0 m_object0;
	OBJECT_TYPE1 m_object1;
};

struct hkpConstraintInfoExtended: public hkpConstraintInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintInfoExtended );

	hkpConstraintInstance* m_constraint;
};

	//
	//	Simple single objects commands
	//
typedef hkpDualObjectPhysicsCommand  <hkpPhysicsCommand::TYPE_ADD_CONSTRAINT_TO_LOCKED_ISLAND,      class hkpConstraintInstance*,int> hkpAddConstraintToCriticalLockedIslandPhysicsCommand;
typedef hkpSingleObjectPhysicsCommand<hkpPhysicsCommand::TYPE_REMOVE_CONSTRAINT_FROM_LOCKED_ISLAND, class hkpConstraintInstance*> hkpRemoveConstraintFromCriticalLockedIslandPhysicsCommand;


extern void HK_CALL hkPhysicsCommandMachineProcess( hkpWorld* world, hkpPhysicsCommand* begin, int sizeOfAllCommandsInBytes );

#endif // HK_DYNAMICS2_PHYSICS_COMMAND_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
