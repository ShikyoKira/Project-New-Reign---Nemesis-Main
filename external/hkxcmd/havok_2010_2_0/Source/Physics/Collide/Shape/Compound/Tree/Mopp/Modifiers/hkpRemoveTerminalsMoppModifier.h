/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
//
//

#ifndef HK_COLLIDE2_MOPP_REMOVE_TERMINALS_MODIFIER_H
#define HK_COLLIDE2_MOPP_REMOVE_TERMINALS_MODIFIER_H

#include <Physics/Internal/Collide/Mopp/Machine/hkpMoppModifier.h>

class hkpMoppCode;
class hkpShapeCollection;
class hkBitField;

extern const hkClass hkpRemoveTerminalsMoppModifierClass;

/// This class allows you to efficiently remove terminals from a MOPP.
/// You can use many instances of this class so long as each instance removes different
/// terminals from the MOPP. This class queries the MOPP with the AABB of all terminals
/// being removed to speed up the building of the internal tables. To get good performance,
/// the triangles to remove should be all in the same area (so that the AABB is small).
/// The construction of a hkpRemoveTerminalsMoppModifier does the AABB query described
/// above, so is not as fast as the actual removal, or you can use a pre-calculated AABB.
/// Since applying a modifier (via the applyRemoveTerminals() method) modifies the MOPP
/// code in-place, to 'undo' a removal (for example on level restart/reload) you must use
/// the same modifier and call the undoRemoveTerminals() method.
/// N.B. Modifiers must be 'undone' in the *reverse order* to that in which they were applied.
class hkpRemoveTerminalsMoppModifier: public hkReferencedObject, public hkpMoppModifier
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MOPP );
		HK_DECLARE_REFLECTION();

			/// Build this modifier (which does an AABB query on the MOPP) but does not
			/// alter the moppCode yet, so you can pre-construct this object.
			/// It calculates the query AABB based on the shapes in the shapesContainer using keysToRemove.
		hkpRemoveTerminalsMoppModifier( const hkpMoppCode* moppCode, const hkpShapeContainer* shapeContainer, const hkArray<hkpShapeKey>& keysToRemove );

		/// Build this modifier (which does an AABB query on the MOPP) but does not
		/// alter the moppCode yet, so you can pre-construct this object. This version
		/// uses a pre-calculated AABB and is roughly 2-10 times faster than the other constructor.
		hkpRemoveTerminalsMoppModifier( const hkpMoppCode* moppCode, const hkAabb& precalculatedAabb, const hkArray<hkpShapeKey>& keysToRemove );

		virtual ~hkpRemoveTerminalsMoppModifier();

		hkpRemoveTerminalsMoppModifier (hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_removeInfo(f) {}

			/// Actually remove the terminals from the moppCode. This function is very fast.
		void applyRemoveTerminals( hkpMoppCode* moppCode );

			/// Undo the removal of the terminals.
			/// N.B. Modifiers must be 'undone' in the reverse order to that in which they
			/// were 'applied'. This function is very fast.
		void undoRemoveTerminals( hkpMoppCode* moppCode );

	protected:
		hkpRemoveTerminalsMoppModifier(): m_tempShapesToRemove(HK_NULL) { }

			// hkpMoppModifier interface implementation
		virtual hkBool shouldTerminalBeRemoved( hkUint32 id, const hkUint32 *properties );

			// hkpMoppModifier interface implementation
		virtual void addTerminalRemoveInfo( hkInt32 relativeMoppAddress );

	protected:

			/// This member stores history of what has been removed from the MOPP code.
			///     - Bits 0-7: The original MOPP code instruction.
			///     - Bits 8-31: The offset in the MOPP where the instruction was located.
		hkArray<hkUint32> m_removeInfo;

			/// for construction only
		const hkArray<hkpShapeKey>* m_tempShapesToRemove; //+nosave
};		

class hkpRemoveTerminalsMoppModifier2: public hkpRemoveTerminalsMoppModifier
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Scan the entire MOPP and remove everything which is not set in enabledKeys.
		hkpRemoveTerminalsMoppModifier2( const hkpMoppCode* moppCode, const hkBitField& enabledKeys );

	protected:
			// hkpMoppModifier interface implementation
		virtual hkBool shouldTerminalBeRemoved( hkUint32 id, const hkUint32 *properties );


	protected:
		const hkBitField* m_enabledKeys;

};
#endif // HK_COLLIDE2_MOPP_REMOVE_TERMINALS_MODIFIER_H

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
