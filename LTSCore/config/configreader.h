#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include "pugi/pugixml.hpp"
#include "Error/LTSError.h"
#include <string>
#include "Config_t.h"




class Configreader
{
public:
    Configreader(const std::string& rXmLFilePath);
    //void loadConfig(Config_t &rConfig);
    void loadConfig(Config_t &rConfig);
private:
    pugi::xml_document m_XmlDoc;
};

#endif // CONFIGREADER_H
