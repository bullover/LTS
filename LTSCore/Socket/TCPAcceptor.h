#ifndef TCPACCEPTOR_H
#define TCPACCEPTOR_H

#include <string>
#include "Socket/TCPStream.h"
class TCPAcceptor
{
	public:
		TCPAcceptor(const int port, const std::string& address);
		~TCPAcceptor();
        TCPStream* Accept(unsigned int timeout = 0);
		void Start();

		inline std::string& GetIP()
			{return this->fAddress;}

		inline int GetPort()
			{return this->fPort;}
	private:
		/// disable
		TCPAcceptor(const TCPAcceptor&);

		std::string fAddress;
		int fSocketHandle;
		int fPort;
};

#endif // TCPACCEPTOR_H
