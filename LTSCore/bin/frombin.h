#ifndef FROMBIN_H
#define FROMBIN_H
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
//#include "TFile.h"
//#include "TTree.h"
//#include "structcol.h"

// added by Hao
#include <memory>

//struct ReducedBinEventread
//{
//		double pos;
//		double dEdx;
//		unsigned EventNumber;
//		unsigned NumberOfHits;


//};
//struct RedforLCIO
//{

//		unsigned Eventnr;
//		double Pos[3];
//		unsigned dEdx;
//		unsigned NumberOfComb;

//};

class FromBin
{
public:
		FromBin(const std::string &r_binFileName);
		~FromBin();

		FromBin (FromBin&& rOther) = default;
		FromBin& operator= (FromBin&& rOther) = default;

		template <typename data>
		 bool readBin(data &T);
				void closeStream();
                bool CheckifGood();
private:
				std::string binFileName;
				std::unique_ptr<std::fstream> iFile;
};


template <typename data>
bool FromBin::readBin(data &T)
{

		if(iFile->good())
		{
						this->iFile->read(reinterpret_cast<char*>(&T), sizeof(T));
						return true;
		}else
		{
				return false;
		}
//     } return true;

}



#endif // FROMBIN_H
