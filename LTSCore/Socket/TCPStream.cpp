#include "TCPStream.h"

//_____________________________________________________________________________
TCPStream::TCPStream(int socketHandle, struct sockaddr_in* addr) noexcept
	: fSocketHandle(socketHandle), fPort(0), fIP(50, ' ')
{
	inet_ntop(PF_INET, reinterpret_cast<struct in_addr*>(&(addr->sin_addr.s_addr)),
		&(this->fIP)[0], fIP.size() - 1);
	this->fPort = ntohs(addr->sin_port);
}

//_____________________________________________________________________________
TCPStream::~TCPStream()
{
	close(this->fSocketHandle);
}
