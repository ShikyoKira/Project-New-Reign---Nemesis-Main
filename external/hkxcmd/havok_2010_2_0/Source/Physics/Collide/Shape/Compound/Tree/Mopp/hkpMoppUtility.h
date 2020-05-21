/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_MOPP_UTILITY_H
#define HK_COLLIDE2_MOPP_UTILITY_H


#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>


class hkpShapeContainer;
class hkpMoppMediator;


/// This class provides useful functionality for managing, maintaining, and creating MOPPs.
class hkpMoppUtility
{
	public:
			/// Builds the MOPP for a given set of shapes with the specified fit tolerance requirements.
			/// The returned hkpMoppCode is a referenced object, so you just need to call removeReference() when you're finished with it -
			/// the system will look after deleting the object.
			/// Note: This method can take significant time to complete. MOPP code is a platform independent byte code and should
			/// be precomputed offline and loaded at runtime.
			/// If chunk building is used for SPUs then an array can be passed to collect the re-indexing information.
		static hkpMoppCode* HK_CALL buildCode(const hkpShapeContainer* shapeContainer, const hkpMoppCompilerInput& req, hkArray<hkpMoppCodeReindexedTerminal>* reindexInfoOut = HK_NULL);


	protected:

			// called by buildCode()
		static hkpMoppCode* HK_CALL buildCodeInternal(hkpMoppMediator& mediator, const hkpShapeContainer* shapeContainer, const hkpMoppCompilerInput& moppInput, hkArray<hkpMoppCodeReindexedTerminal>* reindexInfo);
};



#endif // HK_COLLIDE2_MOPP_UTILITY_H

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
