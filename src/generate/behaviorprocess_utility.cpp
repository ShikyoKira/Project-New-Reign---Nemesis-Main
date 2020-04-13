#include "Global.h"

#include <boost/asio/thread_pool.hpp>

#include "generate/behaviorprocess_utility.h"

#include "generate/animation/animationthread.h"
#include "generate/animation/grouptemplate.h"
#include "generate/animation/newanimation.h"
#include "generate/animation/nodejoint.h"

using namespace std;

void animThreadStart(shared_ptr<NewAnimArgs> args, boost::asio::thread_pool* mt)
{
    try
    {
        try
        {
            if (args->core)
            {
                args->atomicLock.lockCore();

                if (args->isCoreDone)
                    return;
                else
                    args->isCoreDone = true;

                args->atomicLock.releaseCore();
            }

            // getlines from newAnination
            args->dummyAnimation->setZeroEvent(args->ZeroEvent);
            args->dummyAnimation->setZeroVariable(args->ZeroVariable);
            args->dummyAnimation->GetNewAnimationLine(args->allEditLines,
                                                      args->lowerBehaviorFile,
                                                      args->lastID,
                                                      args->exportID,
                                                      args->eventid,
                                                      args->variableid,
                                                      args->stateID,
                                                      args->stateMultiplier,
                                                      args->hasGroup,
                                                      args->core,
                                                      args->subFunctionIDs,
                                                      args->singleFunctionIDs,
                                                      args->atomicLock);
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

    if (error) { mt->stop(); }
}

void groupThreadStart(shared_ptr<newGroupArgs> args, boost::asio::thread_pool* mt)
{
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

    if (error) { mt->stop(); }
}

void elementUpdate(size_t& elementLine, int& counter, int& curID, map<int, VecStr>& catalystMap)
{
    if (elementLine != -1)
    {
        size_t position   = catalystMap[curID][elementLine].find("numelements=\"") + 13;
        string oldElement = catalystMap[curID][elementLine].substr(
            position, catalystMap[curID][elementLine].find("\">", position) - position);

        if (oldElement != to_string(counter))
        {
            catalystMap[curID][elementLine].replace(
                catalystMap[curID][elementLine].find(oldElement), oldElement.length(), to_string(counter));
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
        string filename                                             = to_string(functionID);

        while (filename.length() < 4)
        {
            filename = "0" + filename;
        }

        filename = "#" + filename;

        if (curNode == existingNodes.end() || curNode->second == nullptr)
            existingNodes[functionID] = make_shared<NodeJoint>(
                catalystMap[functionID], templateCode, filename, behaviorFile, templateGroup);

        vector<vector<unordered_map<string, bool>>> optionPicked;
        SSMap IDExist;
        unordered_map<string, bool> otherAnimType;
        string strID = to_string(lastID);
        unsigned __int64 openRange;
        uint elementCount = 0;
        bool negative     = false;

        for (auto& groupInfo : groupAnimInfo)
        {
            vector<unordered_map<string, bool>> curGroupInfo;

            for (auto& animInfo : groupInfo)
            {
                curGroupInfo.push_back(animInfo->optionPicked);
            }

            if (curGroupInfo.size() > 0) { optionPicked.push_back(curGroupInfo); }
        }

        while (strID.length() < 4)
        {
            strID = "0" + strID;
        }

        existingNodes[functionID]->insertData(templateCode,
                                              filename,
                                              optionPicked,
                                              groupAnimInfo,
                                              -1,
                                              -1,
                                              -1,
                                              hasMaster,
                                              hasGroup,
                                              ignoreGroup,
                                              "",
                                              lastID,
                                              strID,
                                              IDExist,
                                              exportID,
                                              eventid,
                                              variableid,
                                              ZeroEvent,
                                              ZeroVariable,
                                              openRange,
                                              elementCount,
                                              nullptr,
                                              groupFunctionIDs,
                                              negative);

        if (error) throw nemesis::exception();
    }
}
