#ifndef TEMPLATETREE_H_
#define TEMPLATETERE_H_

struct single
{
    std::unordered_map<std::string, std::string> format;
};

struct group
{
    std::unordered_map<std::string, std::string> functionIDs;
    std::vector<std::shared_ptr<single>> singlelist;
};

struct master
{
    std::unordered_map<std::string, std::string> functionIDs;
    std::vector<std::shared_ptr<group>> grouplist;
};

#endif
