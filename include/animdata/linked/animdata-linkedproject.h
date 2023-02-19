#pragma once

#include "animdata/animdata-project.h"

#include "core/linked.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataProjectListBuilder;

        struct LinkedProject : nemesis::Linked<AnimProject, nemesis::animdata::LinkedProject>
        {
        public:
            LinkedProject() = default;
            LinkedProject(const nemesis::Line& line);

            void GetRawName(VecNstr& lines) const;

            void SetRawDataTo(VecNstr& lines) const;
            void SetNewConditionedDataTo(VecNstr& lines,
                                         const nemesis::Line& newline,
                                         const LinkedCondition& cond) const override;
            void SetRawConditionedDataTo(VecNstr& lines) const override;

            friend AnimDataProjectListBuilder;
        };
    }
}
