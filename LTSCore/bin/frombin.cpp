#include "frombin.h"
#include "Error/LTSError.h"


FromBin::FromBin(const std::string &r_binFileName)
        :binFileName(r_binFileName),iFile(new std::fstream(r_binFileName.c_str(),std::fstream::in | std::fstream::binary))
{
    if(!iFile->is_open())
    {
        throw LTS::LTSError(LTS::LTSError::Error_t::FATAL,"formFile","Bin File not Open");
    }
}

void FromBin::closeStream()
{
		this->iFile->close();
}



bool FromBin::CheckifGood()
{
    if(this->iFile->good())
    {
        return true;
    }else
    {
        return false;
    }
}
FromBin::~FromBin(){}




















