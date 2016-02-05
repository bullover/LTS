#ifndef TCPCONNECTOR_H
#define TCPCONNECTOR_H

#include "TCPStream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class TCPConnector
{
	public:
		TCPStream* Connect(int port, std::string& server) noexcept;
	private:
		inline int ResolveHostName(const std::string host, struct in_addr* addr);
};

//_____________________________________________________________________________
int TCPConnector::ResolveHostName(const std::string host, struct in_addr* addr)
{
	struct addrinfo* info = nullptr;
	int retval = getaddrinfo(host.data(), nullptr, nullptr, &info);

	if (!retval) memcpy(addr, &(reinterpret_cast<struct sockaddr_in*>
		(info->ai_addr))->sin_addr, sizeof(struct in_addr));

	if (info !=  nullptr) freeaddrinfo(info);
	return retval;
}

#endif // TCPCONNECTOR_H
