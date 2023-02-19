#include "animdata/AnimDataSingleFileBuilder.h"
#include "animdata/animdata-singlefile.h"

UMap<nemesis::animdata::ReadingState, void (nemesis::animdata::AnimDataSingleFileBuilder::*)(const nemesis::Line&)>
    nemesis::animdata::AnimDataSingleFileBuilder::transition_map
    = {{ReadingState::Project, &AnimDataSingleFileBuilder::AddProjectLine},
       {ReadingState::ProjectActive, &AnimDataSingleFileBuilder::AddProjectActiveLine},
       {ReadingState::BehaviorCounter, &AnimDataSingleFileBuilder::AddBehaviorCounterLine},
       {ReadingState::Behavior, &AnimDataSingleFileBuilder::AddBehaviorLine},
       {ReadingState::MovementDataActive, &AnimDataSingleFileBuilder::AddMovementDataActiveLine},
       {ReadingState::ClipName, &AnimDataSingleFileBuilder::AddClipNameLine},
       {ReadingState::ClipCode, &AnimDataSingleFileBuilder::AddClipCodeLine},
       {ReadingState::PlaybackSpeed, &AnimDataSingleFileBuilder::AddPlaybackSpeedLine},
       {ReadingState::CropStart, &AnimDataSingleFileBuilder::AddCropStartLine},
       {ReadingState::CropEnd, &AnimDataSingleFileBuilder::AddCropEndLine},
       {ReadingState::EventCounter, &AnimDataSingleFileBuilder::AddEventCountLine},
       {ReadingState::EventName, &AnimDataSingleFileBuilder::AddEventLine},
       {ReadingState::MovementCode, &AnimDataSingleFileBuilder::AddMovementCodeLine},
       {ReadingState::Duration, &AnimDataSingleFileBuilder::AddDurationLine},
       {ReadingState::MovementDataSize, &AnimDataSingleFileBuilder::AddMovementCounterLine},
       {ReadingState::MovementData, &AnimDataSingleFileBuilder::AddMovementLine},
       {ReadingState::RotationDataSize, &AnimDataSingleFileBuilder::AddRotationCounterLine},
       {ReadingState::RotationData, &AnimDataSingleFileBuilder::AddRotationLine}};

nemesis::animdata::AnimDataSingleFileBuilder::AnimDataSingleFileBuilder(nemesis::File& file)
    : nemesis::StateMachine<AnimDataSingleFileBuilder, ReadingState, void, const nemesis::Line&>(nemesis::animdata::ReadingState::Project)
{
    singlefile     = std::make_unique<nemesis::animdata::SingleFile>();
    trans_map_ptr = &transition_map;
    projectlist_buidler = std::make_unique<nemesis::animdata::AnimDataProjectListBuilder>(file);
}

void nemesis::animdata::AnimDataSingleFileBuilder::AppendLine(const nemesis::Line& line)
{
    Execute(line);
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddProjectLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddProjectActiveLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddBehaviorCounterLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddBehaviorLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddMovementDataActiveLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddClipNameLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddClipCodeLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddPlaybackSpeedLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddCropStartLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddCropEndLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddEventCountLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddEventLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddMovementCodeLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddDurationLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddMovementCounterLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddMovementLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddRotationCounterLine(const nemesis::Line& line)
{
}

void nemesis::animdata::AnimDataSingleFileBuilder::AddRotationLine(const nemesis::Line& line)
{
}
