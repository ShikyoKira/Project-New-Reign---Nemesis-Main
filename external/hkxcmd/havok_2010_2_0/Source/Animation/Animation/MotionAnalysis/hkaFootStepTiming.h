/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FOOTSTEP_TIMING_H
#define HK_FOOTSTEP_TIMING_H

class hkaExpandAnimationUtility;

class hkaFootStepTiming
{
public:

	/// Default initialization
	hkaFootStepTiming();

	/// Initialize from an expanded animation
	hkaFootStepTiming( const hkArrayBase< hkBool >& isDown, hkReal duration );

	/// Initialize
	void init( const hkArrayBase< hkBool >& isDown, hkReal duration );

	/// \return The time that the foot first contacts the ground
	hkReal getDownTime() const;
	
	/// \return The time that the foot first lifts from the ground
	hkReal getUpTime() const;

	/// \return The midpoint for the foot down
	hkReal getMidDownTime() const;

	/// \return The midpoint for the foot down
	hkReal getMidUpTime() const;

	/// \return The length of time that the foot is down
	hkReal getDownDuration() const;

	/// \return The length of time that the foot is up
	hkReal getUpDuration() const;

	/// \return True if the foot is down at the specified time
	bool isDown( hkReal time ) const;

	/// \return True if the foot is up at the specified time
	bool isUp( hkReal time ) const;

	/// \return The midpoint in time between two events, takes wrapping into account
	hkReal getMidpoint( hkReal first, hkReal last ) const;


	/// Set the time that the foot strikes the ground
	inline void setDownTime( hkReal time ) { m_downTime = time; }

	/// Set the time that the foot releases from the ground
	inline void setUpTime( hkReal time ) { m_upTime = time; }

	/// Set the total duration of the cycle
	inline void setDuration( hkReal time ) { m_duration = time; }

private:

	hkReal m_downTime;
	hkReal m_upTime;
	hkReal m_duration;
};

#endif // HK_FOOTSTEP_TIMING_H

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
