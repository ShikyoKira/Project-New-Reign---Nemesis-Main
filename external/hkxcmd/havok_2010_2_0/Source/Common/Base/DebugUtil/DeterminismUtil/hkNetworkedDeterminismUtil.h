/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_NETWORKED_DETERMINISM_UTIL_H
#define HK_NETWORKED_DETERMINISM_UTIL_H

#include <Common/Base/System/Io/Socket/hkSocket.h>
#include <Common/Base/Object/hkSingleton.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>

//#define HK_ENABLE_NETWORKED_DETERMINISM_UTIL

#if defined(HK_ENABLE_NETWORKED_DETERMINISM_UTIL) && !defined(HK_ENABLE_DETERMINISM_CHECKS)
#	error "Must enable HK_ENABLE_DETERMINISM_CHECKS when HK_ENABLE_NETWORKED_DETERMINISM_UTIL is used."
#endif

/// This is a utility that synchronizes one or multiple clients with one server.
/// The utility is plugged into the MenuDemo and synchronizes keyboard and gamePad input from the server to all clients.
/// This utility works in a synchronous way, all clients wait for data from server before processing.
/// And the server waits for clients to catch up.
class hkNetworkedDeterminismUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkNetworkedDeterminismUtil );
		hkNetworkedDeterminismUtil();
		~hkNetworkedDeterminismUtil();

		static void HK_CALL create();
		static void HK_CALL destroy();

		struct Command
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Command);
			enum Type
			{
				TYPE_CONTROL,
				TYPE_DETERMINISM_DATA
			};

			int m_type;
			hkArray<hkUint8> m_data;
		};

		class Server
		{
			private:
				Server();
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, Server );
				Server(int listeningPort, int maxNumClients);
				~Server();

				void pollForNewClients();

				void sendCommand(const Command& command);

				void synchronizeWithClients();

			private:
				hkSocket* m_listeningSocket;
				hkArray<hkSocket*> m_clients;

				int m_maxNumClients;
		};

		class Client
		{
			private:
				Client();
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, Client );
				Client(hkSocket* socket);
				~Client();
				
				void sendSynchronizationBytes();

				void processCommands(Command::Type expectedCommandType);

			public:
				hkSocket* m_socket;
		};

		struct ControlCommand : public Command
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, ControlCommand );
			ControlCommand() { m_type = TYPE_CONTROL; }
			ControlCommand(const void* data, int size);
			//void utiliseCommand(class hkDemoEnvironment* env) const;
		};
		
		struct DeterminismDataCommand : public Command
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, DeterminismDataCommand );
			DeterminismDataCommand(const char* buffer, int bufferSize);
		};


		static bool HK_CALL isServer() { return s_instance ? s_instance->m_server != HK_NULL : false; }
		static bool HK_CALL isClient() { return s_instance ? s_instance->m_client != HK_NULL : false; }

		// The server should simply cache the command here for sending at the end of the step.
		static void HK_CALL startStepDemo(ControlCommand& controlCommand) { if (s_instance) s_instance->startStepDemoImpl(controlCommand); }
		static void HK_CALL endStepDemo() { if (s_instance) s_instance->endStepDemoImpl(); }


		void startStepDemoImpl(ControlCommand& controlCommand);
		void endStepDemoImpl();

	private:

		bool tryToCreateClient();
		void createServer();

		static hkResult HK_CALL readFromSocket(hkSocket* socket, void* buffer, int size);
		static hkResult HK_CALL writeToSocket(hkSocket* socket, void* buffer, int size);

	private:

		static hkNetworkedDeterminismUtil* s_instance;

		hkStringPtr m_serverAddress;
		int m_serverPort;

		Server* m_server;
		Client* m_client;

		ControlCommand m_controlCommand;


};

#endif // HK_NETWORKED_DETERMINISM_UTIL_H

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
