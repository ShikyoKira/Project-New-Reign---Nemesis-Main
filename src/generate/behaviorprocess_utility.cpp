#include "nemesisinfo.h"

#include "utilities/algorithm.h"
#include "utilities/atomiclock.h"

#include "generate/behaviorgenerator.h"
#include "generate/behaviorprocess_utility.h"

#include "generate/animation/nodejoint.h"
#include "generate/animation/newanimation.h"
#include "generate/animation/grouptemplate.h"
#include "generate/animation/animationthread.h"

using namespace std;

extern bool SSE;
extern wstring stagePath;

void animThreadStart(shared_ptr<NewAnimArgs> args)
{
    if (error) return;

	try
	{
		try
		{
			if (args->core)
			{
                Lockless lock(args->atomicLock.coreLock);

				if (args->isCoreDone) return;

				args->isCoreDone = true;
			}

			// getlines from newAnination
			args->dummyAnimation->setZeroEvent(args->ZeroEvent);
			args->dummyAnimation->setZeroVariable(args->ZeroVariable);
			args->dummyAnimation->GetNewAnimationLine(args);
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
    if (error) return;

	try
	{
		try
		{
			args->groupTemp->setZeroEvent(args->ZeroEvent);
            args->groupTemp->setZeroVariable(args->ZeroVariable);
            args->groupTemp->getFunctionLines(args->allEditLines,
                                              args->lowerBehaviorFile,
                                              args->filename,
                                              args->stateID,
                                              args->groupFunctionIDs,
                                              args->groupAnimInfo,
                                              args->lastID,
                                              args->exportID,
                                              args->eventid,
                                              args->variableid,
                                              args->templateCode,
                                              args->atomicLock,
                                              args->groupCount);
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

void elementUpdate(size_t& elementLine, int& counter, int& curID, map<int, VecStr>& catalystMap)
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

void unpackToCatalyst(map<int, VecStr>& catalystMap, unordered_map<int, shared_ptr<NodeJoint>>& existingNodes)
{
	for (auto& node : existingNodes)
	{
		catalystMap[node.first] = node.second->unpack();
	}
}

int bonePatch(std::filesystem::path rigfile, int oribone, bool& newBone)
{
    int bonenum;

    if (SSE)
    {
        FILE* bonefile;
        _wfopen_s(&bonefile, rigfile.wstring().c_str(), L"r+b");

        if (bonefile)
        {
            uint16_t num = 0;
            vector<char> chlist;
            string line;

            while (!feof(bonefile))
            {
                char c = fgetc(bonefile);
                chlist.push_back(c);
                line.push_back(c);
            }

            fclose(bonefile);

            using uchar     = unsigned char;
            bool startCount = false;
            bool start      = true;
            uint pos        = line.find("NPC Root [Root]");

			if (pos != NOT_FOUND && pos > 64)
            {
                pos -= 64;
                char* ch = reinterpret_cast<char*>(&num);
                ch[0]    = chlist[pos];
                ch[1]    = chlist[pos + 1];
			}

            if (error) throw nemesis::exception();

            if (oribone < num)
            {
                bonenum = num - oribone;
                newBone = true;
            }
        }
        else
        {
            ErrorMessage(3002, rigfile);
        }
    }
    else
    {
        VecStr storeline;
        hkxcmdXmlInput(rigfile, storeline);
        string bonemap = "<hkparam name=\"parentIndices\" numelements=\"";

        for (auto& line : storeline)
        {
            if (line.find(bonemap) != NOT_FOUND)
            {
                size_t pos = line.find(bonemap) + bonemap.length();
                int num    = stoi(line.substr(pos, line.find("\">", pos) - pos));

                if (oribone < num)
                {
                    bonenum = num - oribone;
                    newBone = true;
                    break;
                }
            }
        }
    }

    if (error) throw nemesis::exception();

    return bonenum;
}

void processExistFuncID(std::vector<int>& funcIDs,
                        const std::string& ZeroEvent,
                        const std::string& ZeroVariable,
                        std::map<int, VecStr>& catalystMap,
                        const std::shared_ptr<master>& groupFunctionIDs,
                        const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo,
                        const std::string& templateCode,
                        ImportContainer& exportID,
                        ID& eventid,
                        ID& variableid,
                        int& lastID,
                        bool hasMaster,
                        bool hasGroup,
                        SetStr& templateGroup,
                        bool ignoreGroup,
                        const std::string& behaviorFile,
                        std::unordered_map<int, std::shared_ptr<NodeJoint>>& existingNodes)
{
	for (int& functionID : funcIDs)
	{
		unordered_map<int, shared_ptr<NodeJoint>>::iterator curNode = existingNodes.find(functionID);
		string filename = to_string(functionID);

		while (filename.length() < 4)
		{
			filename = "0" + filename;
		}

		filename = "#" + filename;

		if (curNode == existingNodes.end() || curNode->second == nullptr) existingNodes[functionID] = make_shared<NodeJoint>(catalystMap[functionID], templateCode, filename,
			behaviorFile, templateGroup);

		vector<vector<unordered_map<string, bool>>> optionPicked;
		SSMap IDExist;
		unordered_map<string, bool> otherAnimType;
		string strID = to_string(lastID);
		unsigned __int64 openRange;
		unsigned int elementCount = 0;
		bool negative = false;

		for (auto& groupInfo : groupAnimInfo)
		{
			vector<unordered_map<string, bool>> curGroupInfo;

			for (auto& animInfo : groupInfo)
			{
				curGroupInfo.push_back(animInfo->optionPicked);
			}

			if (curGroupInfo.size() > 0)
			{
				optionPicked.push_back(curGroupInfo);
			}
		}

		while (strID.length() < 4)
		{
			strID = "0" + strID;
		}

		existingNodes[functionID]->insertData(templateCode, filename, optionPicked, groupAnimInfo, -1, -1, -1, hasMaster, hasGroup, ignoreGroup, "", lastID, strID, IDExist,
			exportID, eventid, variableid, ZeroEvent, ZeroVariable, openRange, elementCount, nullptr, groupFunctionIDs, negative);

		if (error) throw nemesis::exception();
	}
}

void redirToStageDir(filesystem::path& outpath, const NemesisInfo* nemesisInfo)
{
    wstring wout = outpath.wstring();

    if (stagePath.length() > 0 && wordFind(outpath, nemesisInfo->GetDataPath()) == 0)
    {
        wout.replace(0, nemesisInfo->GetDataPath().length(), nemesisInfo->GetStagePath());
        outpath = wout;
    }
}

std::filesystem::path getTempBhvrPath(const NemesisInfo* nemesisInfo)
{
    if (stagePath.length() > 0)
    {
        return nemesisInfo->GetStagePath() + L"nemesis_engine\\temp_behaviors";
    }

	return "temp_behaviors";
}
