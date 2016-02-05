#include "configreader.h"
#include <iostream>
#include <vector>
#include <math.h>

Configreader::Configreader(const std::string &rXmLFilePath)
{
    pugi::xml_parse_result result = m_XmlDoc.load_file(rXmLFilePath.c_str());

    if(result)
    {
        std::cout << "XML File loaded\n";
    }else
    {
        std::cout << "XML File NOT loaded\n" << rXmLFilePath;
        std::cout << result.description() << "\n";
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"Configxml","XML File");

    }

}


void Configreader::loadConfig(Config_t& rConfig)
{
    pugi::xml_node TuxOa = m_XmlDoc.child("CONFIG").child("TUXDAQ");

    std::cout << TuxOa.name() << "'''\n";

    rConfig.m_TuxDAQ.m_IP = TuxOa.child("IP").child_value();
    rConfig.m_TuxDAQ.m_Port = TuxOa.child("Port").text().as_uint();
    rConfig.m_TuxDAQ.m_NrUpdate = TuxOa.child("NrUpdate").text().as_uint();



    pugi::xml_node corvus = m_XmlDoc.child("CONFIG").child("CORVUS");

    std::vector<double> xvalues;
    std::vector<double> yvalues;
    std::vector<unsigned int> steps;


//    for (pugi::xml_node measurepts = corvus.child("POSITIONS").child("MEASURE"); measurepts; measurepts = measurepts.next_sibling("MEASURE"))
//    {
//        std::cout << "steps " << measurepts.child("steps").child_value()<< "\n";
//        std::cout << "x " << measurepts.child("x").child_value()<< "\n";
//        std::cout << "y " << measurepts.child("y").child_value()<< "\n";

//    }
    rConfig.m_Corvus.m_IPCorv = corvus.child("CLIENT").child("IP").child_value();
    rConfig.m_Corvus.m_PortCorv = corvus.child("CLIENT").child("Port").text().as_uint();
    rConfig.m_Corvus.m_TimeOutCorv = corvus.child("CLIENT").child("TimeOut").text().as_uint();



    steps.push_back(1);
    xvalues.push_back(0.0);
    yvalues.push_back(100.0);

    for (pugi::xml_node cmds = corvus.child("INITSETUP").child("CMD"); cmds; cmds = cmds.next_sibling("CMD"))
    {
        rConfig.m_Corvus.m_Configsetup.push_back(cmds.child_value());

    }

    for (pugi::xml_node measurepts = corvus.child("POSITIONS").child("MEASURE"); measurepts; measurepts = measurepts.next_sibling("MEASURE"))
    {
      steps.push_back(measurepts.child("steps").text().as_uint());
      xvalues.push_back(measurepts.child("x").text().as_double());
      yvalues.push_back(measurepts.child("y").text().as_double());


    }
    if(steps.size() == xvalues.size() && steps.size() == yvalues.size())
    {
        for(unsigned int i = 1 ; i<steps.size();i++)
        {
            if(steps[i]!=0)
            {
                Pos_t xysteps;
                Pos_t xystepsabs;
                xysteps.m_XPos = ((xvalues[i] - xvalues[i-1])/steps[i]);
                xysteps.m_YPos = ((yvalues[i] - yvalues[i-1])/steps[i]);

                double xbuff=(xvalues[i-1]);
                double ybuff=(yvalues[i-1]);
                for(unsigned i1 = 0; i1 < steps[i]; i1++)
                {
                rConfig.m_Corvus.m_allPos.push_back(xysteps);
                xbuff += xysteps.m_XPos;
                ybuff += xysteps.m_YPos;
                xystepsabs.m_XPos =  xbuff;
                xystepsabs.m_YPos = ybuff;
                rConfig.m_Corvus.m_allPosabsolut.push_back(xystepsabs);
                }
            }else
            {
                throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"Configxml","step cant be 0");
            }
        }
    }else
    {

     throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"Configxml","Positions are not correct");
    }

    rConfig.m_isValid = true;

}
