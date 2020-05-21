/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_LOGGER_H
#define HK_LOGGER_H

/// Interface to capturing log messages.
class hkLogger : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// 
		virtual ~hkLogger();

			/// Predefined message priority levels.
		enum LogLevel
		{
			LOG_ERROR = 40,
			LOG_WARNING = 30,
			LOG_INFO = 20,
			LOG_DEBUG = 10,
			LOG_NONE = 0
		};
			/// Message describing a serious problem.
		void error( const char* fmt, ... );

			/// Message describing a nonfatal error.
		void warning( const char* fmt, ... );

			/// Informational message which is not necessarily an error.
		void info( const char* fmt, ... );

			/// Debugging message for diagnosing problems.
		void debug( const char* fmt, ... );

			/// Custom level message.
		void log( int level, const char* fmt, ... );


			/// Enters a new (optionally) named scope.
		virtual void pushScope(const char* name) = 0;

			/// Exit the current scope.
		virtual void popScope() = 0;


			/// Set the level below which messages are ignored.
		virtual void setThreshold( int level ) = 0;

			/// Flush the internal buffers if any.
		virtual void flush() = 0;

			/// A logger which ignores all its messages.
		static hkLogger& nullLogger();

	protected:

			/// Override this method to handle messages.
		virtual void _log( int level, const char* fmt, hk_va_list args) = 0;
};

#endif //HK_LOGGER_H

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
