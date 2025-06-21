#pragma once

#include "Havok/Base/hkLocalFrame.h"
#include "Havok/Base/hkpWorldObject.h"
#include "Havok/Base/hkpBreakableBody.h"

#include "Havok/hkConstraintInternal.h"
#include "Havok/hkpEntityExtendedListeners.h"
#include "Havok/hkpEntitySpuCollisionCallback.h"
#include "Havok/hkpMaterial.h"
#include "Havok/hkpMaxSizeMotion.h"

namespace nemesis
{
    struct hkpEntity : nemesis::hkpWorldObject
    {
        static constexpr nemesis::hkClass Class{0xa03c774b,
                                                "hkpEntity",
                                                &nemesis::hkpWorldObject::Class,
                                                720,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpEntity);

        nemesis::hkpMaterial m_material;
        nemesis::hkRefPtr<nemesis::HavokObject> m_limitContactImpulseUtilAndFlag;
        float m_damageMultiplier;
        nemesis::hkRefPtr<nemesis::hkpBreakableBody> m_breakableBody;
        unsigned int m_solverData;
        unsigned short m_storageIndex;
        unsigned short m_contactPointCallbackDelay;
        nemesis::hkSmallArray<nemesis::hkConstraintInternal> m_constraintsMaster;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConstraintInstance>> m_constraintsSlave;
        nemesis::hkArray<unsigned char> m_constraintRuntime;
        nemesis::hkRefPtr<nemesis::HavokObject> m_simulationIsland;
        char m_autoRemoveLevel;
        unsigned char m_numShapeKeysInContactPointProperties;
        unsigned char m_responseModifierFlags;
        unsigned int m_uid;
        nemesis::hkpEntitySpuCollisionCallback m_spuCollisionCallback;
        nemesis::hkpMaxSizeMotion m_motion;
        nemesis::hkSmallArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_contactListeners;
        nemesis::hkSmallArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_actions;
        nemesis::hkRefPtr<nemesis::hkLocalFrame> m_localFrame;
        nemesis::hkRefPtr<nemesis::hkpEntityExtendedListeners> m_extendedListeners;
        unsigned int m_npData;

    public:
        hkpEntity() noexcept;
        hkpEntity(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpEntity::Class;
}
