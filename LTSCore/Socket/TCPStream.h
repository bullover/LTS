#ifndef TCPSTREAM_H__2014_HEPHY
#define TCPSTREAM_H__2014_HEPHY

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <type_traits>
//#include <fstream>
class TCPStream
{
	private:
		/// socket descriptor
		int fSocketHandle;
		/// given port
		int fPort;
		/// string containing ip
		std::string fIP;

	public:
		enum class State : char
		{
			CLOSED = 0, TIMEOUT = -3, ERROR = -1
		};
		/// creates server stream, accepts in comming connection
		friend class TCPAcceptor;
		/// creates client stream, connects to given server stream
		friend class TCPConnector;
		/// diable copy ctor
		TCPStream(const TCPStream&) = delete;
		/// default move ctor
		TCPStream(TCPStream&& other) = default;

		/// dtor closes socket connection
		~TCPStream();

		/// sends data with given integral type.
		template<class DATA_t>
		inline ssize_t Send(std::vector<DATA_t>& rInput) noexcept;

        // sends string
        inline ssize_t Send(const std::string &rInput) noexcept;

		/// sends word with given integral type.
		template< class DATA_t>
		inline ssize_t SendWord(DATA_t word) noexcept;

		/// receives data with with given integral type.
		template<class DATA_t>
		inline ssize_t Receive(std::vector<DATA_t>& rOutput,
			unsigned int timeout = 0) noexcept;

        // resceives a string
        inline ssize_t Receive(std::string &rOutput, unsigned int timeout = 0, const char EOS = 0xd) noexcept;

		/// receive word with given type.
		template<typename DATA_t>
        inline ssize_t ReceiveWord(DATA_t& word, unsigned int timeout = 0) noexcept;

		/// Get given IP
		inline std::string& GetIP() noexcept { return this->fIP; }

		/// Get given port
		inline int GetPort() noexcept { return this->fPort; }

	private:
		/// ctor for TCPAcceptor and TCPConnector access only!
		TCPStream(int socketHandle, sockaddr_in* addr) noexcept;

		/// check if ready to read
		inline bool IsReadyToRead(const unsigned int TIMEOUT);
};

//_____________________________________________________________________________
template<class DATA_t>
inline ssize_t TCPStream::Send(std::vector<DATA_t>& rInput) noexcept
{
	size_t nrBytes = sizeof(DATA_t)*rInput.size();
	size_t nrSend = 0;

	while(0 < nrBytes) {
		ssize_t curSend = send(this->fSocketHandle,
			reinterpret_cast<char*>(rInput.data()) + nrSend/sizeof(char), nrBytes,
			MSG_NOSIGNAL);

		if (0 == curSend) return static_cast<int>(TCPStream::State::CLOSED);
		else if (0 > curSend) return static_cast<int>(TCPStream::State::ERROR);

		nrSend += curSend;
		nrBytes = ( (nrBytes - curSend) < 0 ? 0 : nrBytes - curSend);
	}

	return nrSend;
}

//_____________________________________________________________________________
inline ssize_t TCPStream::Send(const std::string& rInput) noexcept
{
    ssize_t nrBytes = sizeof(char)*rInput.size();
    size_t nrSend = 0;

    while(0 < nrBytes) {
        ssize_t curSend = send(this->fSocketHandle,
            (rInput.data()) + nrSend/sizeof(char), nrBytes,
            MSG_NOSIGNAL);

        if (0 == curSend) return static_cast<int>(TCPStream::State::CLOSED);
        else if (0 > curSend) return static_cast<int>(TCPStream::State::ERROR);

        nrSend += curSend;
        nrBytes = ( (nrBytes - curSend) < 0 ? 0 : nrBytes - curSend);
    }

    return nrSend;
}

//_____________________________________________________________________________
template<class DATA_t>
inline ssize_t TCPStream::SendWord(DATA_t word) noexcept
{
	ssize_t nrSend = send(this->fSocketHandle,
		reinterpret_cast<char*>(&word), sizeof(word), MSG_NOSIGNAL);

	if (0 == nrSend) return static_cast<int>(TCPStream::State::CLOSED);
	else if (0 > nrSend) return static_cast<int>(TCPStream::State::ERROR);

	return nrSend;
}

//_____________________________________________________________________________
template<class DATA_t>
ssize_t TCPStream::Receive(std::vector<DATA_t>& rOutput, unsigned int timeout) noexcept
{
	ssize_t nrBytes = sizeof(DATA_t)*rOutput.size();
	size_t nrRead = 0;

	if (0 == timeout) {
		while (0 < nrBytes) {
			ssize_t curRead = read(this->fSocketHandle,
				reinterpret_cast<char*>(rOutput.data()) + nrRead/sizeof(char), nrBytes);

			if (0 == curRead) {
				return static_cast<ssize_t>(TCPStream::State::CLOSED);
			} else if ( 0 > curRead ) {
				return static_cast<ssize_t>(TCPStream::State::ERROR);
			}

			nrRead += curRead;
			nrBytes = ((nrBytes - curRead) < 0 ? nrBytes : nrBytes - curRead);
		}
		return nrRead;
	}

	while (0 < nrBytes) {
		if ( !(this->IsReadyToRead(timeout)) ) return static_cast<ssize_t>(TCPStream::State::TIMEOUT);
		/// fill data at the position of data[0] + offset created by the last read function
		ssize_t curRead = read(this->fSocketHandle, reinterpret_cast<char*>(rOutput.data())
			+ nrRead/sizeof(char), nrBytes);

		if (0 == curRead) {
			return static_cast<ssize_t>(TCPStream::State::CLOSED);
		} else if ( 0 > curRead ) {
			return static_cast<ssize_t>(TCPStream::State::ERROR);
		}

		nrRead += curRead;
		nrBytes = ((nrBytes - curRead) < 0 ? nrBytes : nrBytes - curRead);
	}
	return nrRead;
}
//_____________________________________________________________________________
//receives string

/*inline ssize_t TCPStream::Send(const std::string& rInput) noexcept
{
    size_t nrBytes = sizeof(char)*rInput.size();
    size_t nrSend = 0;

    while(0 < nrBytes) {
        ssize_t curSend = send(this->fSocketHandle,
            (rInput.data()) + nrSend/sizeof(char), nrBytes,
            MSG_NOSIGNAL);

        if (0 == curSend) return static_cast<int>(TCPStream::State::CLOSED);
        else if (0 > curSend) return static_cast<int>(TCPStream::State::ERROR);

        nrSend += curSend;
        nrBytes = ( (nrBytes - curSend) < 0 ? 0 : nrBytes - curSend);
    }

    return nrSend;
}*/
ssize_t TCPStream::Receive(std::string& rOutput, unsigned int timeout, const char EOS) noexcept
{
    /*//--------------Read in char by char--------------------
    ssize_t nrBytes = sizeof(char)*rOutput.size();
    size_t nrRead = 0;

    char item = '\n';
    std::vector<char> buffer;

    std::cout << nrBytes << " - nrBytes"<< std::endl;
    while(item != '\n'){

        if ( !(this->IsReadyToRead(timeout)) ) return static_cast<ssize_t>(TCPStream::State::TIMEOUT);
        /// fill data at the position of data[0] + offset created by the last read function

        //std::cout<<"kommt bis zu curread"<<std::endl;
        ssize_t curRead = read(this->fSocketHandle,&item, sizeof(item));

        if (0 == curRead) {
            return static_cast<ssize_t>(TCPStream::State::CLOSED);
        } else if ( 0 > curRead ) {
            return static_cast<ssize_t>(TCPStream::State::ERROR);
        }
        buffer.push_back(item);
  }

    rOutput.resize(buffer.size());
    rOutput.assign(buffer.data());
    std::cout << rOutput << " - routput" << std::endl;
    return nrRead;

    //-------------------------------------*/


    ssize_t nrBytes = 50 ;//sizeof(char)*rOutput.size();
    char curChar = '\0';
    //std::stringstream ss;
    ssize_t nrRead = 0;
//    std::cout << nrBytes << " - nrBytes"<< std::endl;
//    if (0 == timeout) {
//        for(auto i = 0u ;  i < rOutput.size() ; i++ )
//        {

//            ssize_t curRead = read(this->fSocketHandle,
//              &curChar , sizeof(curChar));


//             std::cout << nrBytes << " - nrBytes in loop"<< std::endl;
//            if (0 == curRead) {
//                return static_cast<ssize_t>(TCPStream::State::CLOSED);
//            } else if ( 0 > curRead ) {
//                return static_cast<ssize_t>(TCPStream::State::ERROR);
//            }
//            rOutput[i]= curChar;
//            nrBytes -= sizeof(curChar);
//        }
//        return nrRead;
//    }

    //-----------------------------------



//------- this part is heavly modified ----------------------------------------

 //  std::ofstream ofs ("/mnt/storage/Documents/LTS/LTS_v2/LTS/test.bin", std::ofstream::out);
    //unsigned nrBytesdone;
   for(auto i = 0u ;  i < 50; i++ )
   {
         if ( !(this->IsReadyToRead(timeout)) ) return static_cast<ssize_t>(TCPStream::State::TIMEOUT);
        /// fill data at the position of data[0] + offset created by the last read function

        ssize_t curRead = read(this->fSocketHandle,
          &curChar , sizeof(curChar));

       //    ofs << curChar;
        //   std::cout << std::hex << "0x" << (int)curChar << std::endl;
           if(curChar == EOS)
           {
               nrRead=i;
               break;
           }
        //std::cout << nrBytes << " - nrBytes in loop \n";//<< std::endl;
        //std::cout << i << " - nrBytes in loop done \n";//<< std::endl;
        if (0 == curRead) {
            return static_cast<ssize_t>(TCPStream::State::CLOSED);
        } else if ( 0 > curRead ) {
            return static_cast<ssize_t>(TCPStream::State::ERROR);
        }
        rOutput.push_back(curChar);
        nrBytes -= sizeof(curChar);
    }
  //  ofs.close();
    return nrRead;
}



//_____________________________________________________________________________
template<class DATA_t>
inline ssize_t TCPStream::ReceiveWord(DATA_t& word, unsigned int timeout) noexcept
{
	if (0 == timeout) {
		ssize_t size = read(this->fSocketHandle, reinterpret_cast<char*>(&word),
			sizeof(word));
		std::cout << "work: 0x" << std::hex << word << std::dec << '\n';
		return size;
	}

	if (IsReadyToRead(timeout)) {
		size_t size = read(this->fSocketHandle,
			reinterpret_cast<char*>(&word), sizeof(word));
		std::cout << "work: 0x" << std::hex << word << std::dec << '\n';
		return size;
	}

	return static_cast<ssize_t>(TCPStream::State::TIMEOUT);

//	return (this->IsReadyToRead(timeout)) ?
//		read(this->fSocketHandle, reinterpret_cast<char*>(&word), sizeof(word))
//		: static_cast<ssize_t>(TCPStream::State::TIMEOUT);
}

//_____________________________________________________________________________
inline bool TCPStream::IsReadyToRead(const unsigned int TIMEOUT )
{
	fd_set socket_handels;
	struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = TIMEOUT*1000;

	FD_ZERO(&socket_handels);
	FD_SET(this->fSocketHandle, &socket_handels);

	return
		(select(this->fSocketHandle + 1, &socket_handels, nullptr, nullptr, &tv) > 0)
		? true : false;
}

#endif // TCPSTREAM_H

