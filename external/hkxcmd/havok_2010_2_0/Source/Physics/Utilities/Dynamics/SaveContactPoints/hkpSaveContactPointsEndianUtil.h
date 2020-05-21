/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SAVE_CONTACT_POINTS_ENDIAN_UTIL_H
#define HK_UTILITIES2_SAVE_CONTACT_POINTS_ENDIAN_UTIL_H

#include <Common/Base/hkBase.h>
#include <Common/Base/hkBase.h>


struct hkpAgentEntry;
struct hkpProcessCollisionInput;
struct hkpSerializedAgentNnEntry;
struct hkpSerializedTrack1nInfo;

struct hkpAgentNnMachineTimEntry;
struct hkpAgentNnMachinePaddedEntry; 
struct hkpAgent1nMachineTimEntry;
struct hkpAgent1nMachinePaddedEntry;

class hkpBoxBoxManifold;
struct hkpCapsuleTriangleCache3;
class hkpGskCache;
struct hkpGskManifold;
class hkpContactPointProperties;



class hkpSaveContactPointsEndianUtil
{
	public:
			// Swap endianness in a collision entry, including all recursive subtracks.
		static void HK_CALL swapEndianTypeInCollisionEntry( const hkpProcessCollisionInput* input, hkpSerializedAgentNnEntry& serializedEntry );
		static void HK_CALL swapEndianTypeInEntry( const hkpProcessCollisionInput* input, hkpAgentEntry* entry, hkpSerializedTrack1nInfo& serializedTrack, hkBool isNnEntry, int& sizeOfThisEntryOut, bool& trackUsedOut);
		static void HK_CALL swapEndianTypeIn1nTrack( const hkpProcessCollisionInput* input, hkpSerializedTrack1nInfo& serializedTrack );


		//
		// Helper functions
		//

			// Base endian swapping func
		static inline void reverseBytes(void* start, int count);

			// Basic types swapping
		static inline void swapEndianType(hkUint16& uint16);
		static inline void swapEndianType(hkUint32& uint32);
		static inline void swapEndianType(hkReal& real);
		static inline void swapEndianType(hkVector4& vec4);

			// Agent entry swapping
		static inline void swapEndianType(hkpAgentNnMachineTimEntry* entry);
		static inline void swapEndianType(hkpAgentNnMachinePaddedEntry* entry); 
		static inline void swapEndianType(hkpAgent1nMachineTimEntry* entry);
		static inline void swapEndianType(hkpAgent1nMachinePaddedEntry* entry);

			// Agent data swaping
		static inline void swapEndianType(hkpBoxBoxManifold* manifold);
		static inline void swapEndianType(hkpCapsuleTriangleCache3* cache);
		static inline void swapEndianType(hkpGskCache* cache);
		static inline void swapEndianType(hkpGskManifold* manifold);
		static inline void swapEndianType(hkpContactPointProperties* p);



};

#endif // HK_UTILITIES2_SAVE_CONTACT_POINTS_ENDIAN_UTIL_H

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
