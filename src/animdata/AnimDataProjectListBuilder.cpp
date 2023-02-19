#include "animdata/AnimDataProjectListBuilder.h"

UMap<nemesis::animdata::ReadingState,
     void (nemesis::animdata::AnimDataProjectListBuilder::*)(const nemesis::Line&)>
    nemesis::animdata::AnimDataProjectListBuilder::transition_map
    = {{ReadingState::ProjectSize, &AnimDataProjectListBuilder::AddProjectSizeLine},
       {ReadingState::BehaviorCounter, &AnimDataProjectListBuilder::AddBehaviorCounterLine},
       {ReadingState::Behavior, &AnimDataProjectListBuilder::AddBehaviorLine},
       {ReadingState::MovementDataActive, &AnimDataProjectListBuilder::AddMovementDataActiveLine},
       {ReadingState::AnimationData, &AnimDataProjectListBuilder::AddAnimationDataLine},
       {ReadingState::MovementDataSize, &AnimDataProjectListBuilder::AddMovementDataSizeLine},
       {ReadingState::MovementData, &AnimDataProjectListBuilder::AddMovementDataLine}};

nemesis::animdata::AnimDataProjectListBuilder::AnimDataProjectListBuilder(nemesis::File& file, nemesis::animdata::ReadingState start_state)
    : nemesis::Importer<nemesis::LinkedPreprocessLine>(file)
    , nemesis::StateMachine<AnimDataProjectListBuilder,
                            nemesis::animdata::ReadingState,
                            void,
                            const nemesis::Line&>(start_state)
{
    //stream.emplace_back(&project);
    trans_map_ptr = &transition_map;
}

bool nemesis::animdata::AnimDataProjectListBuilder::IsNewProject(const nemesis::Line& line)
{
    tempdata.emplace_back(line);

    switch (tempdata.size())
    {
        case 4:
        {
            if (hasAlpha(line))
            {
                AddNewProject();
                return true;
            }

            if (line.find(" ") != NOT_FOUND)
            {
                AddMovementData();
                return false;
            }
        }
        case 5:
        {
            if (line.empty() || line.find(" ") != NOT_FOUND)
            {
                AddMovementData();
                return false;
            }

            AddNewProject();
            return true;
        }

    }

    return false;
}

void nemesis::animdata::AnimDataProjectListBuilder::AddMovementData()
{

}

void nemesis::animdata::AnimDataProjectListBuilder::AddNewProject()
{

}

void nemesis::animdata::AnimDataProjectListBuilder::AddProjectSizeLine(const nemesis::Line& line)
{
    project = std::make_shared<nemesis::animdata::LinkedProject>();
}

void nemesis::animdata::AnimDataProjectListBuilder::AddBehaviorCounterLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataProjectListBuilder::AddBehaviorLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataProjectListBuilder::AddMovementDataActiveLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataProjectListBuilder::AddAnimationDataLine(const nemesis::Line& line)
{}

void nemesis::animdata::AnimDataProjectListBuilder::AddMovementDataSizeLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataProjectListBuilder::AddMovementDataLine(const nemesis::Line& line)
{}

void nemesis::animdata::AnimDataProjectListBuilder::Build()
{

}

void nemesis::animdata::AnimDataProjectListBuilder::ParseLines(const VecNstr& lines)
{
    throw std::runtime_error("AnimDataProjectListBuilder::ParseLines no implementation error");
}

void nemesis::animdata::AnimDataProjectListBuilder::AppendLine(const nemesis::Line& line)
{
    ParseLine(line);
}

SPtr<nemesis::animdata::LinkedProject> nemesis::animdata::AnimDataProjectListBuilder::ToProject() noexcept
{
    return project;
}
