/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_GENERIC_CONSTRAINT_SCHEME_H
#define HK_DYNAMICS2_GENERIC_CONSTRAINT_SCHEME_H

#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Constraint/hkpConstraintData.h>

extern const class hkClass hkpGenericConstraintDataSchemeConstraintInfoClass;

class hkpConstraintModifier;
class hkpConstraintMotor;
class hkGenericConstraintDataCinfo;

extern const hkClass hkpGenericConstraintDataSchemeClass;

class hkpGenericConstraintDataScheme
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpGenericConstraintDataScheme );

		// serialization workaround
		struct ConstraintInfo
		{
		public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpGenericConstraintDataScheme::ConstraintInfo );
			HK_DECLARE_REFLECTION();
			int m_maxSizeOfSchema;
			int m_sizeOfSchemas;
			int m_numSolverResults;
			int m_numSolverElemTemps;
		}; 

		HK_DECLARE_REFLECTION();

		hkpGenericConstraintDataScheme() {} 

		enum 
		{	
			e_endScheme = 0, 
			e_setPivotA, e_setPivotB, 
			e_setLinearDofA, e_setLinearDofB, e_setLinearDofW, e_constrainLinearW, e_constrainAllLinearW,
			e_setAngularBasisA, e_setAngularBasisB, e_setAngularBasisAidentity, e_setAngularBasisBidentity,
			e_constrainToAngularW, e_constrainAllAngularW,e_setAngularMotor, e_setLinearMotor,
			e_setLinearLimit, e_setAngularLimit, e_setConeLimit, e_setTwistLimit, e_setAngularFriction, e_setLinearFriction,
			e_setStrength, e_restoreStrengh, e_doConstraintModifier, 
			e_numCommands 
		};  

		struct hkpConstraintInfo m_info; //+overridetype(struct hkpGenericConstraintDataScheme::ConstraintInfo) +nosave
	
		hkArray<hkVector4>				m_data;
		hkArray<int>					m_commands;
		hkArray<hkpConstraintModifier*>	m_modifiers; //+nosave
		hkArray<hkpConstraintMotor*>		m_motors;

	public:

		hkpGenericConstraintDataScheme(hkFinishLoadedObjectFlag f);

};


#endif

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
