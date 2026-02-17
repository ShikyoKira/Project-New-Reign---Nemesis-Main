#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkpCollisionFilter.h"
#include "Havok/Base/hkpConvexListFilter.h"
#include "Havok/Base/hkWorldMemoryAvailableWatchDog.h"

#include "Havok/hkAabb.h"

namespace nemesis
{
    struct hkpWorldCinfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa5255445,
                                                "hkpWorldCinfo",
                                                &nemesis::hkReferencedObject::Class,
                                                256,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpWorldCinfo);

        using BroadPhaseBorderBehaviour = nemesis::hkInternalClassEnumItem<char>;
        using TreeUpdateType = nemesis::hkInternalClassEnumItem<char>;
        using ContactPointGeneration = nemesis::hkInternalClassEnumItem<char>;
        using SimulationType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkVector4 m_gravity;
        int m_broadPhaseQuerySize{};
        float m_contactRestingVelocity{};
        nemesis::hkEnum<BroadPhaseBorderBehaviour, char> m_broadPhaseBorderBehaviour = {
            {"BROADPHASE_BORDER_ASSERT", 0},
            {"BROADPHASE_BORDER_FIX_ENTITY", 1},
            {"BROADPHASE_BORDER_REMOVE_ENTITY", 2},
            {"BROADPHASE_BORDER_DO_NOTHING", 3},
        };
        bool m_mtPostponeAndSortBroadPhaseBorderCallbacks{};
        nemesis::hkAabb m_broadPhaseWorldAabb;
        bool m_useKdTree{};
        bool m_useMultipleTree{};
        nemesis::hkEnum<TreeUpdateType, char> m_treeUpdateType = {
            {"REBUILD_ACTIVE", 0},
            {"REBUILD_ALL", 1},
        };
        bool m_autoUpdateKdTree{};
        float m_collisionTolerance{};
        nemesis::hkRefPtr<nemesis::hkpCollisionFilter> m_collisionFilter;
        nemesis::hkRefPtr<nemesis::hkpConvexListFilter> m_convexListFilter;
        float m_expectedMaxLinearVelocity{};
        int m_sizeOfToiEventQueue{};
        float m_expectedMinPsiDeltaTime{};
        nemesis::hkRefPtr<nemesis::hkWorldMemoryAvailableWatchDog> m_memoryWatchDog;
        int m_broadPhaseNumMarkers{};
        nemesis::hkEnum<ContactPointGeneration, char> m_contactPointGeneration = {
            {"CONTACT_POINT_ACCEPT_ALWAYS", 0},
            {"CONTACT_POINT_REJECT_DUBIOUS", 1},
            {"CONTACT_POINT_REJECT_MANY", 2},
        };
        bool m_allowToSkipConfirmedCallbacks{};
        bool m_useHybridBroadphase{};
        float m_solverTau{};
        float m_solverDamp{};
        int m_solverIterations{};
        int m_solverMicrosteps{};
        float m_maxConstraintViolation{};
        bool m_forceCoherentConstraintOrderingInSolver{};
        float m_snapCollisionToConvexEdgeThreshold{};
        float m_snapCollisionToConcaveEdgeThreshold{};
        bool m_enableToiWeldRejection{};
        bool m_enableDeprecatedWelding{};
        float m_iterativeLinearCastEarlyOutDistance{};
        int m_iterativeLinearCastMaxIterations{};
        unsigned char m_deactivationNumInactiveFramesSelectFlag0{};
        unsigned char m_deactivationNumInactiveFramesSelectFlag1{};
        unsigned char m_deactivationIntegrateCounter{};
        bool m_shouldActivateOnRigidBodyTransformChange{};
        float m_deactivationReferenceDistance{};
        float m_toiCollisionResponseRotateNormal{};
        bool m_BETA_useSplitCollisionPipeline{};
        bool m_useCompoundSpuElf{};
        int m_maxSectorsPerCollideTask{};
        int m_maxSectorsPerMidphaseCollideTask{};
        int m_maxSectorsPerNarrowphaseCollideTask{};
        bool m_processToisMultithreaded{};
        int m_maxEntriesPerToiCollideTask{};
        int m_maxEntriesPerToiMidphaseCollideTask{};
        int m_maxEntriesPerToiNarrowphaseCollideTask{};
        int m_maxNumToiCollisionPairsSinglethreaded{};
        float m_numToisTillAllowedPenetrationSimplifiedToi{};
        float m_numToisTillAllowedPenetrationToi{};
        float m_numToisTillAllowedPenetrationToiHigher{};
        float m_numToisTillAllowedPenetrationToiForced{};
        bool m_enableDeactivation{};
        nemesis::hkEnum<SimulationType, char> m_simulationType = {
            {"SIMULATION_TYPE_INVALID", 0},
            {"SIMULATION_TYPE_DISCRETE", 1},
            {"SIMULATION_TYPE_CONTINUOUS", 2},
            {"SIMULATION_TYPE_MULTITHREADED", 3},
        };
        bool m_enableSimulationIslands{};
        unsigned int m_minDesiredIslandSize{};
        bool m_processActionsInSingleThread{};
        bool m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob{};
        float m_frameMarkerPsiSnap{};
        bool m_fireCollisionCallbacks{};

    public:
        hkpWorldCinfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWorldCinfo::Class;
}
