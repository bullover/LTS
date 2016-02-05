#include <stdexcept>
#include <sstream>
#include <fcntl.h>
#include <cstring>

#include "Socket/TCPAcceptor.h"
#include "Socket/TCPStream.h"

//_____________________________________________________________________________
TCPAcceptor::TCPAcceptor(const int port, const std::string& address)
	: fAddress(address), fSocketHandle(0), fPort(port)
{}

//_____________________________________________________________________________
TCPAcceptor::~TCPAcceptor()
{
	if (0 != this->fSocketHandle) close(this->fSocketHandle);
}

//_____________________________________________________________________________
void TCPAcceptor::Start()
{
	(this->fSocketHandle) = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));

	address.sin_family = PF_INET;
	address.sin_port = htons(this->fPort);

	if ( !(this->fAddress.empty()) )
		inet_pton(PF_INET, (this->fAddress).c_str(), &(address.sin_addr));
	else address.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	setsockopt(this->fSocketHandle, SOL_SOCKET,
		SO_REUSEADDR, &optval, sizeof(optval));

	int retval = bind(this->fSocketHandle,
		reinterpret_cast<struct sockaddr*>(&address), sizeof(address));
	if (0 != retval) {
		std::stringstream ss;
		ss << "TCPAcceptor::ERROR: Can not bind to given address. Got "
			<< this->fPort << ":" << this->fAddress << '.';
		throw std::runtime_error(ss.str());
	}

	retval = listen(this->fSocketHandle, 1);
	if (0 != retval) {
		std::string errMsg = "TCPAcceptor::ERROR: Listening failed.";
		throw std::runtime_error(errMsg);
	}
}

//_____________________________________________________________________________
TCPStream* TCPAcceptor::Accept(unsigned int timeout)
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	socklen_t size = sizeof(address);

	if (timeout) {
		// set socket non blocking
		int retval = fcntl(this->fSocketHandle, F_GETFL, 0);
		if (retval < 0) {
			std::string errMsg = "TCPAcceptor::ERROR:Can not get listen socket flags.";
			throw std::runtime_error(errMsg);
		}

		if (0 != fcntl(this->fSocketHandle, F_SETFL, (retval|O_NONBLOCK))) {
			std::string errMsg = "TCPAcceptor::ERROR:Can not "
				"set listen socket flags to non blocking.";
			throw std::runtime_error(errMsg);
		}

		fd_set set;
		FD_ZERO(&set); /* clear the set */
		FD_SET(this->fSocketHandle, &set); /* add our file descriptor to the set */

		struct timeval time;
        time.tv_sec = 0;
        time.tv_usec = timeout;

		retval = select(1 + this->fSocketHandle, &set, 0, 0, &time);
		if (0 >= retval)
			throw std::runtime_error("Accept incomming connections timed out.");

		if (0 != fcntl(this->fSocketHandle, F_SETFL, (retval & (~O_NONBLOCK)))) {
			std::string errMsg = "TCPAcceptor::ERROR:Can not "
				"reset listen socket flags back to blocking.";
			throw std::runtime_error(errMsg);
		}
	}

	int socket_handle = accept(this->fSocketHandle,
		reinterpret_cast<struct sockaddr*> (&address), &size);

	if ( 0 > socket_handle ) return NULL;

	// set socket to non block
//	if (!isBlocking) {
//		int retval = fcntl(socket_handle, F_GETFL, 0);
//		if (retval < 0) {
//			std::string errMsg = "TCPAcceptor::ERROR: Can not get client socket flags.";
//			throw std::runtime_error(errMsg);
//		}

//		if (0 != fcntl(socket_handle, F_SETFL, (retval|O_NONBLOCK))) {
//			std::string errMsg = "TCPAcceptor::ERROR:Can not "
//				"set client socket flags to non blocking.";
//			throw std::runtime_error(errMsg);
//		}
//	}

    return new TCPStream(socket_handle, &address);
}


