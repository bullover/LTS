#ifndef CONFIG_T_H
#define CONFIG_T_H
#include <string>
#include <vector>


struct TuxDAQConfig_t
{
    TuxDAQConfig_t()
        : m_NrUpdate(0), m_Port(0), m_IP()
    {}

    unsigned int m_NrUpdate;
    unsigned int m_Port;
    std::string m_IP;
};

struct Pos_t
{
    double m_XPos;
    double m_YPos;
};

struct CorvusConfig_t
{
    std::string m_IPCorv;
    unsigned int m_PortCorv;
    unsigned int m_TimeOutCorv;
    std::vector<std::string> m_Configsetup;
    std::vector<Pos_t> m_allPos;
    std::vector<Pos_t> m_allPosabsolut;

};

struct Config_t
{
    Config_t():m_isValid(false)
    {}

    bool m_isValid;
    TuxDAQConfig_t m_TuxDAQ;
    CorvusConfig_t m_Corvus;

};





#endif // CONFIG_T_H
