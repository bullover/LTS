#include "tobin.h"
#include "Error/LTSError.h"

ToBin::ToBin(const std::string& r_binFileName)
    :binFileName(r_binFileName),oFile(r_binFileName.c_str(),std::fstream::out | std::fstream::binary)
    {
    if(!oFile.is_open())
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"ToBin","Bin File not Open");
    }

}

 void ToBin::closeStream()
{
    this->oFile.close();
}

ToBin::~ToBin(){}
