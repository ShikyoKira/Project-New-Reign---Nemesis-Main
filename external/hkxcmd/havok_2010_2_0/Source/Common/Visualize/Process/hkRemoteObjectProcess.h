/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_REMOTE_OBJECT_PROCESS_H
#define HK_REMOTE_OBJECT_PROCESS_H

#include <Common/Visualize/hkProcess.h>

class hkClass;
class hkProcessContext;
class hkRemoteObjectProcess;

	/// Interface to receive a notification when objects are sent to the server from
	/// a client.
class hkRemoteObjectServerSideListener
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkRemoteObjectServerSideListener);
		virtual ~hkRemoteObjectServerSideListener() {}
		
			/// Callback raised when an object is received from a client.
		virtual void receiveObjectCallback( hkReferencedObject* object, const hkClass* klass ) = 0;
};

	/// Interface to receive a notification when objects are sent to a client from the
	/// server. Also used to send objects to the server from a client.
class hkRemoteObjectClientSideListener
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkRemoteObjectClientSideListener);
		virtual ~hkRemoteObjectClientSideListener() {}
		
			/// Declaration for function type used to send objects from the client to the
			/// server. This matches hkVdbSendObject which is used in the hkVdbClient.
		typedef void (HK_CALL *SendObject)(hkReferencedObject* object);
	
			/// Initializes the listener with a SendObject function. The function
			/// should be hkVdbSendObject in the VdbClient library.
		hkRemoteObjectClientSideListener( SendObject sendFunc ): m_sendFunc(sendFunc) {}
		
			/// Callback raised when an object is received from the VDB server.
		virtual void receiveObjectCallback( hkReferencedObject* object, const hkClass* klass ) = 0;
			/// Sends and object via the SendObject function used to initialize the listener.
		void sendObject( hkReferencedObject* object );
		
	private:
			/// Function to send an object.
		SendObject m_sendFunc;	
};

	/// The hkRemoteObjectProcess sends hkReferencedObjects between the VDB server and it's
	/// clients. Objects are sent bidirectionally as tagfiles and loaded onto the heap after
	/// receipt. Notifications of new objects are sent via hkRemoteObjectServerSideListeners
	/// which are be attached to the process.
class hkRemoteObjectProcess: public hkReferencedObject, public hkProcess
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);
				
			/// Creates the process.
		hkRemoteObjectProcess();
			/// Destructs the process.
		virtual ~hkRemoteObjectProcess();
		
			/// Adds a server side listener to the process.
		void addListener( hkRemoteObjectServerSideListener* listener );
			/// Removes a server side listener from the process.
		void removeListener( hkRemoteObjectServerSideListener* listener );

			/// Sends an object from the server to all clients using the hkRemoteObjectProcess protocol.
		static void sendObject( hkDisplaySerializeOStream* stream, hkReferencedObject* object );
		
			/// hkProcess interface
		virtual void init();
			/// hkProcess interface
		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands );
			/// hkProcess interface
		virtual void consumeCommand( hkUint8 command );
			/// hkProcess interface
		virtual int getProcessTag() { return m_tag; }

			/// Creates the instances of the process. Invoked by the hkProcessFactory.
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);
			/// Registers the process with hkProcessFactory.
		static void HK_CALL registerProcess();
			/// Gets the name of the process
		static inline const char* HK_CALL getName() { return "RemoteObject"; }
								
	protected:
			/// Array of listeners that are notified when objects are received.
		hkArray<hkRemoteObjectServerSideListener*> m_listeners;
			
			/// Unique ID for the process. Assigned when the process is registered.
		static int m_tag;
	
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
