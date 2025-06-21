#pragma once#include "Havok/Base/hkReferencedObject.h"namespace nemesis{    struct hkpContactMgr : nemesis::hkReferencedObject    {        static constexpr nemesis::hkClass Class{0xa6fa7e88,                                                "hkpContactMgr",                                                &nemesis::hkReferencedObject::Class,                                                20,                                                nullptr,                                                0,                                                {},                                                {},                                                nullptr,                                                nullptr,                                                nemesis::hkClass::FlagValues::FLAGS_NONE,                                                0};    private:
        enum Type
        {
            TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR,
            TYPE_REPORT_CONTACT_MGR,
            TYPE_CONVEX_LIST_CONTACT_MGR,
            TYPE_NULL_CONTACT_MGR,
            TYPE_USER_CONTACT_MGR,
            TYPE_MAX
        };

        Type m_type;    public:        hkpContactMgr(const nemesis::hkClass& cls);        void SerializeTo(nemesis::Serializer& serializer) const override;        void DeserializeFrom(nemesis::Deserializer& deserializer) override;    };    constexpr nemesis::hkClass nemesis::hkpContactMgr::Class;}