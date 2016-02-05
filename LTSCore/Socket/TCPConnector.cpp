#include "TCPConnector.h"
#include <iostream>

//_____________________________________________________________________________
TCPStream* TCPConnector::Connect(int port, std::string& server) noexcept
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	if ( 0 !=  ResolveHostName(server.data(), &(address.sin_addr)) )
		inet_pton(PF_INET, server.data(), &(address.sin_addr));

	int handle = socket(AF_INET, SOCK_STREAM, 0);

//    std::cout << port << " - port -" << handle << " - handle\n";

    if ( 0 != connect(handle,
        reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) )
            return nullptr;

//    auto retval = connect(handle,
//        reinterpret_cast<struct sockaddr*>(&address), sizeof(address));

//    std::cout << retval << " - retval" << std::endl;
//    if (0!=retval) return nullptr;
	return new TCPStream(handle, &address);
}
