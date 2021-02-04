//
// Created by Administrator on 2021/1/29.
//

#ifndef NDKFUNC_CONFIG_H
#define NDKFUNC_CONFIG_H

#include <string>

class Config {

public:
    Config();
    ~Config();

    bool parseConfigFile(const char* pcConfigFilePath);

private:
    bool open(const char* pcFileName);
    void close();
    bool isValid() {return (m_fp != NULL);}

    bool getTag(const char* pcLineData, const char* space, char* pcTag, int maxLen);

    bool getItem(const char* pcGroupName, const char* pcItemName, char* pcItem, int nMaxSize);

public:
    static const char* const FILE_PATH;

private:
    static const char* const TAG;

    std::string m_grp1_item1;
    std::string m_grp1_item2;
    std::string m_grp1_item3;

    std::string m_grp2_item1;
    std::string m_grp2_item2;
    std::string m_grp2_item3;

private:
    FILE* m_fp;
};


#endif //NDKFUNC_CONFIG_H
