#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkpPhantom.h"

#include "Havok/hkpRigidBody.h"
#include "Havok/hkpSimulation.h"
#include "Havok/hkpWorldDynamicsStepInfo.h"

namespace nemesis
{
    struct hkpWorld : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xaadcec37,
                                                "hkpWorld",
                                                &nemesis::hkReferencedObject::Class,
                                                1072,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpWorld);

        using BroadPhaseType = nemesis::hkInternalClassEnumItem<char>;
        using ContactPointGeneration = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkRefPtr<nemesis::hkpSimulation> m_simulation;
        nemesis::hkVector4 m_gravity;
        nemesis::hkRefPtr<nemesis::HavokObject> m_fixedIsland;
        nemesis::hkRefPtr<nemesis::hkpRigidBody> m_fixedRigidBody;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_activeSimulationIslands;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_inactiveSimulationIslands;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_dirtySimulationIslands;
        nemesis::hkRefPtr<nemesis::HavokObject> m_maintenanceMgr;
        nemesis::hkRefPtr<nemesis::HavokObject> m_memoryWatchDog;
        bool m_assertOnRunningOutOfSolverMemory{};
        nemesis::hkEnum<BroadPhaseType, char> m_broadPhaseType = {
            /// Three axis sweep and prune. This is the "legacy" broad phase, optimized
            /// for fast incremental updates which happen during normal rigid body simulation.
            /// However insertions and queries become slow for densely populated broad phases.
            {"BROADPHASE_TYPE_SAP",0},

            /// Spatial tree. This is optimized for insertions and queries, particularly
            /// for densely populated broad phases. It also offers some additional query
            /// methods beyond that of the sweep and prune version broadphase.
            /// However it is slower than sweep and prune for incremental updates.
            {"BROADPHASE_TYPE_TREE", 1},

            /// Hybrid. This combines the best of both SAP and TREE, using SAP for
            /// updates and TREE for queries. However there is some performance and memory
            /// overhead needed to keep both structures synchronized.
            /// This is the recommended broad phase type for general use cases.
            {"BROADPHASE_TYPE_HYBRID",2},
        };
        nemesis::hkRefPtr<nemesis::HavokObject> m_broadPhase;
        nemesis::hkRefPtr<nemesis::HavokObject> m_kdTreeManager;
        bool m_autoUpdateTree{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_broadPhaseDispatcher;
        nemesis::hkRefPtr<nemesis::HavokObject> m_phantomBroadPhaseListener;
        nemesis::hkRefPtr<nemesis::HavokObject> m_entityEntityBroadPhaseListener;
        nemesis::hkRefPtr<nemesis::HavokObject> m_broadPhaseBorderListener;
        nemesis::hkRefPtr<nemesis::HavokObject> m_multithreadedSimulationJobData;
        nemesis::hkRefPtr<nemesis::HavokObject> m_collisionInput;
        nemesis::hkRefPtr<nemesis::HavokObject> m_collisionFilter;
        nemesis::hkRefPtr<nemesis::HavokObject> m_collisionDispatcher;
        nemesis::hkRefPtr<nemesis::HavokObject> m_convexListFilter;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pendingOperations;
        int m_pendingOperationsCount{};
        int m_pendingBodyOperationsCount{};
        int m_criticalOperationsLockCount{};
        int m_criticalOperationsLockCountForPhantoms{};
        bool m_blockExecutingPendingOperations{};
        bool m_criticalOperationsAllowed{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_pendingOperationQueues;
        int m_pendingOperationQueueCount{};
        nemesis::hkMultiThreadCheck m_multiThreadCheck;
        bool m_processActionsInSingleThread{};
        bool m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob{};
        unsigned int m_minDesiredIslandSize{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_modifyConstraintCriticalSection;
        int m_isLocked{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_islandDirtyListCriticalSection;
        nemesis::hkRefPtr<nemesis::HavokObject> m_propertyMasterLock;
        bool m_wantSimulationIslands{};
        bool m_useHybridBroadphase{};
        float m_snapCollisionToConvexEdgeThreshold{};
        float m_snapCollisionToConcaveEdgeThreshold{};
        bool m_enableToiWeldRejection{};
        bool m_wantDeactivation{};
        bool m_shouldActivateOnRigidBodyTransformChange{};
        float m_deactivationReferenceDistance{};
        float m_toiCollisionResponseRotateNormal{};
        int m_maxSectorsPerMidphaseCollideTask{};
        int m_maxSectorsPerNarrowphaseCollideTask{};
        bool m_processToisMultithreaded{};
        int m_maxEntriesPerToiMidphaseCollideTask{};
        int m_maxEntriesPerToiNarrowphaseCollideTask{};
        int m_maxNumToiCollisionPairsSinglethreaded{};
        int m_simulationType{};
        float m_numToisTillAllowedPenetrationSimplifiedToi{};
        float m_numToisTillAllowedPenetrationToi{};
        float m_numToisTillAllowedPenetrationToiHigher{};
        float m_numToisTillAllowedPenetrationToiForced{};
        unsigned int m_lastEntityUid{};
        unsigned int m_lastIslandUid{};
        unsigned int m_lastConstraintUid{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpPhantom>> m_phantoms;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_actionListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_entityListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_phantomListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_constraintListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_worldDeletionListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_islandActivationListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_worldPostSimulationListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_worldPostIntegrateListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_worldPostCollideListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_islandPostIntegrateListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_islandPostCollideListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_contactListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_contactImpulseLimitBreachedListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_worldExtensions;
        nemesis::hkRefPtr<nemesis::HavokObject> m_violatedConstraintArray;
        nemesis::hkRefPtr<nemesis::HavokObject> m_broadPhaseBorder;
        nemesis::hkRefPtr<nemesis::HavokObject> m_destructionWorld;
        nemesis::hkRefPtr<nemesis::HavokObject> m_npWorld;
        nemesis::hkpWorldDynamicsStepInfo m_dynamicsStepInfo;
        std::array<nemesis::hkVector4, 2> m_broadPhaseExtents;
        int m_broadPhaseNumMarkers{};
        int m_sizeOfToiEventQueue{};
        int m_broadPhaseQuerySize{};
        int m_broadPhaseUpdateSize{};
        nemesis::hkEnum<ContactPointGeneration, char> m_contactPointGeneration = {
            /// Try to gather as many contact points as possible. This
            /// gives you the highest quality at the cost of some (up to 25%)
            /// extra CPU. You should use this setting if you want to stack
            /// very small objects in your game
            {"CONTACT_POINT_ACCEPT_ALWAYS", 0},

            /// Accept good contact points immediately and try to
            /// to reject the rest. This is a compromise
            {"CONTACT_POINT_REJECT_DUBIOUS", 1},

            /// Uses some optimistic algorithms to speed up contact point generation.
            /// This can seriously increase the performance of the engine.
            /// Note: Stacking small objects becomes very difficult with this option enabled
            {"CONTACT_POINT_REJECT_MANY", 2},
        };
        bool m_BETA_useSplitCollisionPipeline{};

    public:
        hkpWorld() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWorld::Class;
}
