/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDE_DEBUG_UTIL_H
#define HK_COLLIDE2_COLLIDE_DEBUG_UTIL_H

//#define HK_DISABLE_DEBUG_DISPLAY
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/hkDebugDisplayHandler.h>

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

/// A utility class for drawing OBBs and AABBs as debug lines. They can be
/// output to the generic debug display or sent directly to a display handler.
class hkpCollideDebugUtil
{
	public:
			/// Draws an OBB as a series of debug lines.
		static void HK_CALL drawObbLines(const hkTransform& t, const hkVector4& halfExtents, unsigned int colour, hkDebugDisplayHandler* displayHandler = HK_NULL, int id = 0, int tag = 0)
		{
#ifndef HK_DISABLE_DEBUG_DISPLAY
			// draw the lines

			hkVector4 points[8];
			// points 
			{
				for (int i = 0; i < 8; i++)
				{
					hkVector4 v = halfExtents;
					if ( i & 1 )	v(0) *= -1.0f;
					if ( i & 2 )	v(1) *= -1.0f;
					if ( i & 4 )	v(2) *= -1.0f;
					points[i].setTransformedPos(t, v);
				}
			}

			// edges
			
			{
				for (int i = 0; i < 8; i++)
				{
					for ( int bit = 1; bit < 8; bit <<= 1 )
					{
						int j = i ^ bit;
						if ( i < j )
						{
							hkVector4 start = points[i];
							hkVector4 end = points[j];

							if(displayHandler)
							{
								displayHandler->displayLine(start, end, colour, id, tag);
							}
							else
							{
								hkDebugDisplay::getInstance().displayLine(start, end, colour, id, tag);
							}
						}
					}
				}
			}
#endif
		}

			/// Draws an array of AABBs as a series of debug lines.
		static void HK_CALL displayAabbs(hkArray<hkAabb>& aabbs, unsigned int colour, hkDebugDisplayHandler* displayHandler = HK_NULL, int id = 0, int tag = 0)
		{
#ifndef HK_DISABLE_DEBUG_DISPLAY
			for(int i=0; i<aabbs.getSize(); i++)
			{
				hkAabb aabb = aabbs[i];

				{
					hkTransform t;
					t.setIdentity();
					hkVector4 centre = aabb.m_min;
					centre.add4(aabb.m_max);
					centre.mul4((const hkSimdReal&)hkQuadRealHalf);		
					t.setTranslation(centre);

					hkVector4 halfExtents = aabb.m_max;
					halfExtents.sub4(aabb.m_min);
					halfExtents.mul4((const hkSimdReal&)hkQuadRealHalf);		

					drawObbLines(t, halfExtents, colour, displayHandler, id, tag);
				}
			}
#endif
		}
};

#endif // HK_COLLIDE2_COLLIDE_DEBUG_UTIL_H

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
