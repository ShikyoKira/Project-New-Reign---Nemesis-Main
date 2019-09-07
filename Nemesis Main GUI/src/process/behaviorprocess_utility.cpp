#include "behaviorprocess_utility.h"

#include "add animation\furniture.h"
#include "add animation\grouptemplate.h"
#include "add animation\animationthread.h"

using namespace std;

void animThreadStart(shared_ptr<newAnimArgs> args)
{
	try
	{
		try
		{
			if (args->core)
			{
				args->atomicLock.lockCore();

				if (args->isCoreDone) return;
				else args->isCoreDone = true;

				args->atomicLock.releaseCore();
			}

			// getlines from newAnination
			args->dummyAnimation->setZeroEvent(args->ZeroEvent);
			args->dummyAnimation->setZeroVariable(args->ZeroVariable);
			args->dummyAnimation->GetFurnitureLine(args->allEditLines, args->lowerBehaviorFile, args->lastID, args->exportID, args->eventid, args->variableid, args->stateID,
				args->stateMultiplier, args->hasGroup, args->core, args->subFunctionIDs, args->singleFunctionIDs, args->atomicLock);
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, args->dummyAnimation->GetFormatName(), ex.what());
		}
	}
	catch (nemesis::exception&)
	{
		// resolved exception
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, args->dummyAnimation->GetFormatName(), "New animation: Unknown");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
}

void groupThreadStart(shared_ptr<newGroupArgs> args)
{
	try
	{
		try
		{
			args->groupTemp->setZeroEvent(args->ZeroEvent);
			args->groupTemp->setZeroVariable(args->ZeroVariable);
			args->groupTemp->getFunctionLines(args->allEditLines, args->lowerBehaviorFile, args->filename, args->stateID, args->groupFunctionIDs, args->groupAnimInfo,
				args->lastID, args->exportID, args->eventid, args->variableid, args->templateCode, args->atomicLock, args->groupCount);
		}
		catch (const exception& ex)
		{
			ErrorMessage(6002, args->filename, ex.what());
		}
	}
	catch (nemesis::exception&)
	{
		// resolved exception
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, args->filename, "New animation: Unknown");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
}

void elementUpdate(size_t& elementLine, int& counter, int& curID, map<int, vecstr>& catalystMap)
{
	if (elementLine != -1)
	{
		size_t position = catalystMap[curID][elementLine].find("numelements=\"") + 13;
		string oldElement = catalystMap[curID][elementLine].substr(position, catalystMap[curID][elementLine].
			find("\">", position) - position);

		if (oldElement != to_string(counter))
		{
			catalystMap[curID][elementLine].replace(catalystMap[curID][elementLine].find(oldElement), oldElement.length(),
				to_string(counter));
		}

		elementLine = -1;
	}
}

void processExistFuncID(vector<int>& funcIDs, string ZeroEvent, string ZeroVariable, map<int, vecstr>& catalystMap, shared_ptr<master> groupFunctionIDs,
	vector<vector<shared_ptr<animationInfo>>>& groupAnimInfo, string templateCode, ImportContainer& exportID, id& eventid, id& variableid, int lastID, bool hasMaster,
	bool hasGroup, setstr& templateGroup, bool ignoreGroup)
{
	for (int& functionID : funcIDs)
	{
		ExistingFunction exFunction;
		exFunction.setZeroEvent(ZeroEvent);
		exFunction.setZeroVariable(ZeroVariable);
		catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], groupFunctionIDs, groupAnimInfo, templateCode, exportID,
			eventid, variableid, lastID, hasMaster, hasGroup, templateGroup, ignoreGroup);

		if (error) throw nemesis::exception();

		if (catalystMap[functionID].size() == 0) ErrorMessage(1099);

		if (catalystMap[functionID].back().empty())
		{
			if (catalystMap[functionID].size() > 1 && catalystMap[functionID][catalystMap[functionID].size() - 2].empty()) catalystMap[functionID].pop_back();
		}
		else
		{
			catalystMap[functionID].push_back("");
		}

		if (error) throw nemesis::exception();
	}
}
