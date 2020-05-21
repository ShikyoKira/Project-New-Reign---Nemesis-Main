/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE__VISUAL_DEBUGGER
#define HK_VISUALIZE__VISUAL_DEBUGGER

#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/hkProcessContext.h>

class hkStreamReader;
class hkStreamWriter;

static const hkUint32 HK_VISUAL_DEBUGGER_DEFAULT_PORT = 25001;

/// Client tracking.
///
/// This is the main visual debugger class on the server side. It takes care of all
/// clients attaching them as listeners to the world and creating the appropriate
/// display handlers to send the display information over the network. A plugin handler
/// is also created to allow the client to create and delete viewers remotely.
struct hkVisualDebuggerClient
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVisualDebuggerClient );

	class hkSocket* m_socket;
	class hkServerProcessHandler* m_processHandler; // one per client
};

// Object tracking (objects that can be inspected, which must have a hkClass)
struct hkVisualDebuggerTrackedObject
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVisualDebuggerTrackedObject );

	void* m_ptr;
	const class hkClass* m_class; 
	hkUlong m_tag;
};

// Class will be null on remove callback ( when 'added' == false )
typedef void (HK_CALL* hkVisualDebuggerTrackedObjectCallback)( void* ptr, const hkClass* klass, hkBool wasAdded, hkUlong tagId, void* userCallbackHandle );

	/// Visual Debugger server interface.
class hkVisualDebugger : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

			/// Creates a visual debugger for a given set of contexts
			/// A context is a container for hkWorlds, or user / animation data for instance
			/// Constructor. See the hkVisualDebugger::create.
			/// Will set the context owner as this VDB (assumes only one VDB)
			/// If you don
		hkVisualDebugger( const hkArray<hkProcessContext*>& contexts, const class hkVtableClassRegistry* classReg = HK_NULL );

			/// Instructs the visual debugger to start listening for client connections
			/// from the given TCP/IP port number. For the server to actually work the
			/// world associated with this hkVisualDebugger instance must have its
			/// hkpWorld::stepDeltaTime called as it uses the post simulation callback
			/// to poll for new clients.
		void serve(int listenPort = HK_VISUAL_DEBUGGER_DEFAULT_PORT);

			/// Instructs the visual debugger to start capturing the output of all
			/// of the default viewers to the specified filename. Note you do not
			/// have to call hkVisualDebugger::serve this is completely independent
			/// and can be used without any network serving.
		void capture(const char* captureFilename);

			/// Instructs the visual debugger to create a connection to a memory stream. After
			/// connected the debugger will communicate over these memory streams as if they
			/// were a network connection.
		void capture(hkMemoryTrack* outgoingMemory, hkMemoryTrack* incommingMemory);

			/// End (all) captures to file.
		void endCapture();

			/// Shutdown the VDB. Can call shutdown and then serve again later.
			/// It removes all clients and shuts down the server.
			/// Shutdown is called automatically by the dtor, but is safe to call more than once.
		void shutdown();

			/// Added a process to the list of default process. All process in this
			/// list will be selected by a client when a new connection is established.
		void addDefaultProcess(const char* processName);

			/// Removes a viewer from the list of default viewers. If a viewer is not
			/// in this list it will not be selected by a client when a new connection
			/// is established.
		void removeDefaultProcess(const char* viewerName);

			/// Added an object (with associated class) to the top level
			/// objects that can be inspected by clients.
			/// Group string currently ignored.
		void addTrackedObject(void* obj, const hkClass& klass, const char* group, hkUlong tagId );

			/// Removes an object from the top level
			/// objects that can be inspected by clients (removed by listeners
			/// in the code that added the object on deletion usually).
		void removeTrackedObject(void* obj);

			/// Called when the macro HK_STEP_DISPLAY(frameTimeInMs) is called, this
			/// macro must be called for the visual debugger to function properly.
		virtual void step(hkReal frameTimeInMs = 0); // advances all clients to the next frame

			/// Called at the end of a world simulation step.
			/// WAS postSimulationCallback(world)
		virtual void pollForNewClients();

			/// Destructor.
		virtual ~hkVisualDebugger();

			/// Get the contexts that the Processes can run under
			/// such as a context that lists hkWorlds, or one that has animation data for instance
			/// It is up to the Processes if they want to use the data in any to set themselves up
		const hkArray<hkProcessContext*>& getContexts() { return m_contexts; }

			/// Gets a list of process used by clients of the server.
		void getCurrentProcesses( hkArray< class hkProcess* >& process ) const;
		
			/// Searches for a process in the current process list by name. Returns HK_NULL if not found.
		hkProcess* getCurrentProcessByName( const char* name ) const;			

		// Internal used by the inspection process:
			/// Get the list of tracked objects. Use add and remove tracked object to change (e.g., upon deletion)
		const hkArray<hkVisualDebuggerTrackedObject>& getTrackedObjects() const { return m_trackedObjects; }
		void addTrackedObjectCallback( hkVisualDebuggerTrackedObjectCallback callback, void* userHandle);
		void removeTrackedObjectCallback( hkVisualDebuggerTrackedObjectCallback callback );
		inline const hkVtableClassRegistry* getClassReg() const { return m_classReg; }
		
			// By default ::step will use the wall clock time if frameTimeInMs is equal 
			// to 0.  Call disableOverrideFrameTimeIfZero to disable this behavior and use
			// the value of frameTimeInMs for the frame time.
		void disableOverrideFrameTimeIfZero() { m_overrideFrameTimeIfZero = false; }
		void SupressPollForNewClient(bool val);
	protected:

		void deleteClient(int i);
		void createClient( hkSocket* socket, hkStreamReader* reader, hkStreamWriter* writer );
		void writeStep(int i, float t);  // step cmd to given client id.

		// Server socket (we listen for new Clients on it)
		hkSocket* m_server;
		bool s_SuppressPollForNewClients;

		// All clients get a separate process handler.
		hkArray<hkVisualDebuggerClient> m_clients; 
		hkArray<hkProcessContext*> m_contexts;

		// Master list of currently tracked objects. Any changes raise the callbacks
		hkArray<hkVisualDebuggerTrackedObject> m_trackedObjects;
		hkArray<hkVisualDebuggerTrackedObjectCallback> m_trackCallbacks;
		hkArray<void*> m_trackCallbackHandles;
		const hkVtableClassRegistry* m_classReg;
		
		hkArray<hkStringPtr> m_defaultProcesses; // Process that will always service a Client (auto created)
		
		hkBool m_amTimingFrame;
		hkStopwatch m_frameTimer;
		hkBool m_overrideFrameTimeIfZero;
};

#endif // HK_VISUALIZE__VISUAL_DEBUGGER

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
