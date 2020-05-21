/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKSTOPWATCH_H 
#define HKBASE_HKSTOPWATCH_H

/// An hkStopwatch provides high resolution timing.
/// Time is stored internally as a 64 bit integer.
class hkStopwatch
{
	public:

        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, hkStopwatch);

			/// 
		static hkUint64 HK_CALL getTickCounter();

			/// 
		static hkUint64 HK_CALL getTicksPerSecond();

	public:

			/// Creates a new stopwatch with optional name
		hkStopwatch( const char* name=HK_NULL );

			/// Starts the stopwatch.
		void start();

			/// Stops the stopwatch. The split ticks are also stopped.
		void stop();

			/// Resets the counter and split counter to zero.
		void reset();
	
			/// Checks if the stopwatch is currently running.
		hkBool isRunning() const;

			/// Returns the name given in the constructor.
		const char* getName() const;

			/// Returns the elapsed time in seconds.
		hkReal getElapsedSeconds() const;

			/// Returns the split time in seconds.
			/// The split time is the amount of time elapsed since the last
			/// call to a getSplit method. Internally this calls getSplitTicks()
			/// and converts the value to seconds.
		hkReal getSplitSeconds();

			/// Returns the elapsed time in ticks.
			/// Ticks are a machine dependent quantity and may or may not
			/// map exactly to hardware cycles.
		hkUint64 getElapsedTicks() const;

			/// Returns the split time in ticks.
			/// The split time is the amount of time elapsed since the last
			/// call to a getSplit method.
		hkUint64 getSplitTicks();

			/// How many times has this stopwatch been stopped?
		int getNumTimings() const;
			
			/// Resumes the stopwatch.
		void resume();

	protected:

		static hkReal HK_CALL divide64(hkUint64, hkUint64);

		hkUint64	m_ticks_at_start;
		hkUint64	m_ticks_total;
		hkUint64	m_ticks_at_split;
		hkUint64	m_split_total;
		hkBool		m_running_flag;
		int			m_num_timings;
		const char*	m_name;
};

#include <Common/Base/System/Stopwatch/hkStopwatch.inl>

#endif // HKBASE_HKSTOPWATCH_H

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
