/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PREVAILING_WIND_H
#define HK_PREVAILING_WIND_H

#include <Physics/Utilities/Actions/Wind/hkpWind.h>

#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

/// A wind which can vary over time.
/// If oscillations are added, then the prevailing wind object must be added to the world
/// as a post simulation listener.
class hkpPrevailingWind : public hkpWind, public hkpWorldPostSimulationListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Constructor.
			/// \param mid the base wind vector.
		hkpPrevailingWind( const hkVector4& mid );

			/// Gets the wind at position pos due to wind.
		virtual void getWindVector( const hkVector4& pos, hkVector4& windOut ) const;

			/// Adds a sinusoidal oscillation.
			/// \param diff the maximum value of the vector at the positive extent.
			/// \param period the period of the oscillation in seconds.
			/// \param power the sin is raised to this power.
			///  Higher powers give gustier oscillations.
			/// \param phase the starting phase of the oscillation (between 0 and 1).
		void addOscillation( const hkVector4& diff, hkReal period, hkReal power = 1.0f, hkReal phase = 0.0f );

			/// Allows air properties to vary over time.
		void postSimulationCallback( hkpWorld* world );

			/// Destructor.
		virtual ~hkpPrevailingWind() { }

	public:

			/// Describes a value which varies over time sinusoidally.
		class Oscillator
		{
			public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpPrevailingWind::Oscillator);
					/// Constructor.
					/// \param diff the maximum value of the vector at the positive extent.
					/// \param period the period of the oscillation in seconds.
					/// \param phase the starting phase of the oscillation (between 0 and 1).
				Oscillator( hkReal period, hkReal phase = 0.0f );
				Oscillator() {}

					/// Gets the vector at the current state of the oscillation.
				inline hkReal getValue() const;

					/// Update by delta seconds.
				void update ( hkReal delta );

				virtual ~Oscillator() { }
			private:
					/// The period of oscillation.
				hkReal m_period;
					/// Values between 0 and 1.
				hkReal m_accumulator;
		};

	private:
			/// The base wind vector.
		hkVector4 m_mid;

			/// Combines a vector, an oscillator and a power.
		struct Triple {
			hkVector4 m_diff;
			Oscillator m_oscillator;
			hkReal m_power;

			Triple() {}
			Triple( const hkVector4& d, const Oscillator& o, hkReal p ) : m_diff( d ), m_oscillator( o ), m_power( p ) { }
		};

			/// The array of oscillating vectors.
		hkArray< Triple > m_oscillators;
			/// The current wind vector (redundant copy maintained so getWindVector queries are
			/// cheap).
		hkVector4 m_current;
};

#endif // HK_PREVAILING_WIND_H

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
