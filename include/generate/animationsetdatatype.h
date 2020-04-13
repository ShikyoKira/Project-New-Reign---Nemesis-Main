#ifndef ANIMATIONSETDATATYPE_H_
#define ANIMATIONSETDATATYPE_H_

struct attackdata
{
    bool isNew   = false;
    bool isMulti = false;
    std::string data;

    attackdata()
    {}
    attackdata(std::string _data)
    {
        data = _data;
    }
};

struct equip
{
    bool isNew   = false;
    bool isMulti = false;
    std::string name;
};

struct typepack
{
    bool isNew   = false;
    bool isMulti = false;
    std::string name;
    std::string equiptype1;
    std::string equiptype2;
};

struct animpack
{
    bool isNew   = false;
    bool isMulti = false;
    std::string eventname;
    std::string unknown;
    std::vector<attackdata> attackClip;
};

struct crc32
{
    bool isNew   = false;
    bool isMulti = false;
    std::string filepath;
    std::string filename;
    std::string fileformat;
};

#endif
