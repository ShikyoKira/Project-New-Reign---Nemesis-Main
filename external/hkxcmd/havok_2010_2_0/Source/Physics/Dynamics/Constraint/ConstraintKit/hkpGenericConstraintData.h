/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_GENERIC_CONSTRAINT_H
#define HK_DYNAMICS2_GENERIC_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintScheme.h>

class hkpGenericConstraintDataParameters;

extern const hkClass hkpGenericConstraintDataClass;

/// A generic constraint for use with the hkpConstraintConstructionKit. A generic constraint
/// initially doesn't restrict any movement for its bodies - it must be configured using the kit.
class hkpGenericConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// A parameter index. The constraint construction kit returns an index each time you specify a pivot
			/// point, basis, or axis for the constraint, allowing you to access these later.
		typedef int hkpParameterIndex;

			/// Creates a new generic constraint.
		hkpGenericConstraintData();

		virtual ~hkpGenericConstraintData();


			/// Gets the parameter at the index returned during kit construction
		hkVector4* getParameters( hkpParameterIndex parameterIndex );

			/// Sets the parameters starting at the index returned during kit construction
		void setParameters( hkpParameterIndex parameterIndex, int numParameters, const hkVector4* newValues ); 

			/// Checks consistency of constraint members.
		virtual hkBool isValid() const;

		hkpGenericConstraintDataScheme* getScheme();

	public:

		struct hkpBridgeAtoms m_atoms;

	protected:

		// commands 

		// linear constraints

		void constrainAllLinearW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void constrainLinearW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void constrainToAngularW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void constrainAllAngularW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		// limits, friction, motors

		inline void setLinearLimitW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setAngularLimitW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setConeLimitW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setTwistLimitW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, const hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setAngularMotorW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setLinearMotorW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;

		inline void setAngularFrictionW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;
			
		void setLinearFrictionW( hkArray<int>::iterator& currentCommand, hkArray<hkVector4>::iterator& currentData, hkpGenericConstraintDataScheme& scheme, hkpGenericConstraintDataParameters& vars, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) const;
		
		// end commands

		void hatchScheme( hkpGenericConstraintDataScheme* scheme, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

		class hkpGenericConstraintDataScheme m_scheme;

	public:
		// Internal functions

		virtual void buildJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& info ) const;

		virtual int getType() const;

		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		hkpGenericConstraintData(hkFinishLoadedObjectFlag f);

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
