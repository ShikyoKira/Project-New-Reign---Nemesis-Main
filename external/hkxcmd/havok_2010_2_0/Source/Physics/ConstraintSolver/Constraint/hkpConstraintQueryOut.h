/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_OUT_H
#define HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_OUT_H

class hkpJacobianSchema;

	/// A class containing a few pointers to buffers, where we expect
	/// the results of an hkpConstraintData::buildJacobian() call
class  hkpConstraintQueryOut
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpConstraintQueryOut );

			/// pointers to the schemas buffer
			/// the ConstraintData::buildJacobian() implementation has to increment this pointer
		hkPadSpu<hkpJacobianSchema*> m_jacobianSchemas;
};

#endif // HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_OUT_H

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
