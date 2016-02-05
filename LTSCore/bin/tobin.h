#ifndef TOBIN_H
#define TOBIN_H
#include <string>
#include <iostream>
#include <fstream>




class ToBin
{
public:
    ToBin(const std::string &r_binFileName);
    ~ToBin();

template <typename data>
    void writeBin(data &T);

    void closeStream();

private:
    std::string binFileName;
    std::fstream oFile;
};

template <typename data>
void ToBin::writeBin(data &T)
{

    this->oFile.write(reinterpret_cast<char*>(&T), sizeof(T));
}





#endif // TOBIN_H
