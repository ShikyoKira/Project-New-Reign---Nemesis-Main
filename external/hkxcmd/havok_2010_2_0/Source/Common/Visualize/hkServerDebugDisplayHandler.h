/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VISUALIZE_SERVER_DEBUG_DISPLAY_HANDLER
#define HK_VISUALIZE_SERVER_DEBUG_DISPLAY_HANDLER

#include <Common/Visualize/hkDebugDisplayHandler.h>
#include <Common/Visualize/hkProcess.h>
class hkDisplaySerializeOStream;

/// The server debug display handler is only a process as far as the command router is concerned.
class hkServerDebugDisplayHandler : public hkReferencedObject, public hkProcess, public hkDebugDisplayHandler
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

			/// Serializes hkDisplayHandler messages, the stream must be writable.
		hkServerDebugDisplayHandler( hkDisplaySerializeOStream* stream, hkDisplaySerializeIStream* inStream );

			/// Destructor.
		virtual ~hkServerDebugDisplayHandler();
		
		//
		// Display handler functions
		//
		using hkDebugDisplayHandler::addGeometry;

		virtual hkResult addGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint);
			/// Not supported.
		virtual hkResult addGeometryInstance(hkUlong originalInstanceID, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint);
		virtual hkResult setGeometryColor(int color, hkUlong id, int tag);
		virtual hkResult setGeometryTransparency(float alpha, hkUlong id, int tag);
		virtual hkResult updateGeometry(const hkTransform& transform, hkUlong id, int tag);
		virtual hkResult updateGeometry( const hkMatrix4& transform, hkUlong id, int tag );
		virtual hkResult skinGeometry(hkUlong* ids, int numIds, const hkMatrix4* poseModel, int numPoseModel, const hkMatrix4& worldFromModel, int tag );
		virtual hkResult removeGeometry(hkUlong id, int tag, hkUlong shapeIdHint);
				
		virtual hkResult updateCamera(const hkVector4& from, const hkVector4& to, const hkVector4& up, hkReal nearPlane, hkReal farPlane, hkReal fov, const char* name);

		virtual hkResult displayPoint(const hkVector4& position, int colour, int id, int tag);
		virtual hkResult displayLine(const hkVector4& start, const hkVector4& end, int color, int id, int tag);
		virtual hkResult displayTriangle(const hkVector4& a, const hkVector4& b, const hkVector4& c, int color, int id, int tag);

		virtual hkResult displayText(const char* text, int color, int id, int tag);
		virtual hkResult display3dText(const char* text, const hkVector4& pos, int color, int id, int tag);
		virtual hkResult displayAnnotation(const char* text, int id, int tag);
		virtual hkResult displayBone(const hkVector4& a, const hkVector4& b, const hkQuaternion& orientation, int color, int tag );

		virtual hkResult displayGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, const hkTransform& transform, int color, int id, int tag);
		virtual hkResult displayGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, int color, int id, int tag);

		virtual hkResult sendMemStatsDump(const char* data, int length);

		virtual hkResult holdImmediate();

		// Process interface.
		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands );
		virtual void consumeCommand( hkUint8 command );
			/// This class is only a process as far as the command router is concerned, so it
			/// doesn't need a proper tag.
		virtual int getProcessTag() { return -1; }

			/// Step calls buildAndSendGeometries.
		void step( hkReal frameTimeInMs );
	protected:
		/// A critical section to ensure that writing to m_outStream will be
		/// thread-safe.
		mutable class hkCriticalSection* m_outstreamLock;
		
		void sendGeometryData(const hkArrayBase<hkDisplayGeometry*>& geometries);

	public:
		// Support for sending geometries in parts.

			/// If supported by the client, builds and sends geometries in parts.
		virtual hkResult addGeometryLazily( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint );
		
		// Support for vdb geometry caching.
		
		virtual hkBool doesSupportHashes() const;
		virtual hkResult addGeometryHash( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const Hash& hash, const hkAabb& aabb, hkUint32 color, const hkTransform& transform, hkUlong id, int tag );		
		
	protected:
			/// Add some display geometries for a geometry.
			/// \param final is this the final (or possibly only) part of the geometry?
		hkResult addGeometryPart( const hkArrayBase<hkDisplayGeometry*>& geometries, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint, hkBool final );

			/// Build and send the geometry in parts.
		hkResult addGeometryInParts( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint );

			/// Move a geometry from the m_geometryAwaitingRequests array to the
			/// m_geometryAwaitingDeparture array.
		void processGeometryWithHashRequested( const Hash& hash );

			/// Give up references to the source and builder corresponding to the
			/// hash.
		void processGeometryWithHashNotRequested( const Hash& hash );

			/// Send some (or all) of the geometries in m_geometriesAwaitingDeparture.
		void buildAndSendGeometries();

			/// Send part of the geometries that correspond to the hash.
		hkResult useGeometryForHash( const hkArray<hkDisplayGeometry*>& geometries, const Hash& hash, hkBool final );

			/// Find an index in the m_geometriesAwaitingRequests array.
		int findIndexForGeometryAwaitingRequest( const Hash& m_hash ) const;

			/// Handle lifetimes of the source and builder objects.
		void addUseOfSourceAndGeometry( const hkReferencedObject* source, const hkDisplayGeometryBuilder* builder );
		
			/// Handle lifetimes of the source and builder objects.
		void removeUseOfSourceAndGeometry( const hkReferencedObject* source, const hkDisplayGeometryBuilder* builder );

	protected:
			/// Information which can be used to build a geometry when a hash
			/// value has been sent to the client.
		struct UnbuiltGeometryInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkServerDebugDisplayHandler::UnbuiltGeometryInfo);
			Hash m_hash;
			const hkReferencedObject* m_source;
			hkDisplayGeometryBuilder* m_builder;
		};

			/// Information about geometries we've sent to the client (in addGeometryHash
			/// messages), but haven't yet received a command telling us whether it needs them or
			/// not.
		hkArray<UnbuiltGeometryInfo> m_geometriesAwaitingRequests;

			/// Geometries that the client does want us to build and send.
			/// The first element of this list may have been partial built and send in the
			/// previous frame. This will be the case if m_continueData is non-null.
		hkArray<UnbuiltGeometryInfo> m_geometriesAwaitingDeparture;

			/// This is non-null if the first element in m_geometriesAwaitingDeparture was
			/// only partially sent in the previous frame. It is used to indicate the point
			/// at which processing in that element should resume.
			/// See hkDisplayGeometryBuilder.h for details.
		hkReferencedObject* m_continueData;

			/// References to one source and builder are kept for each hash, controlled
			/// by the usage count in this map. The algorithms in this class assume that
			/// only one do-request for a geometry corresponding to a particular hash can
			/// exist at one time (but many do-not-requests can co-exist with a do-request).
		typedef hkPointerMap<Hash, int> HashCountMap;
		HashCountMap m_hashCountMap;

			/// The number of simple shapes to divide shapes into when adding in parts.
			/// If this is 0, then the client does not want geometries sent in parts.
		int m_simpleShapesPerPart;

			/// The number of simple shapes to send per-frame.
			/// If this is 0, then the client does not want hashed geometries sent in parts.
		int m_simpleShapesPerFrame;

			/// Does the client want shapes sent as hashes?
		hkBool m_sendHashes;
};

#endif // HK_VISUALIZE_SERVER_DEBUG_DISPLAY_HANDLER

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
