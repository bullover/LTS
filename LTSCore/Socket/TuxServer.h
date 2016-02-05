#ifndef TUXSERVER_H
#define TUXSERVER_H

#include <memory>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Socket/TCPAcceptor.h"

class TuxServer
{
	public:
		enum State {
			INIT, STARTED, CONNECTED, CLOSED, ERROR
		};

		static const uint32_t fgEOR = 0xffffffff;

		TuxServer(const unsigned int PORT, const std::string& rIP);
		inline void Start();
        inline bool Accept(unsigned int timeout = 0);
		template <typename DATA_t>
		inline ssize_t Send(std::vector<DATA_t> rInput);
        inline ssize_t Recv(std::string& rOutput, unsigned int Tout);
		inline void SendEOF();
	private:
		TCPAcceptor fAcceptor;
                std::unique_ptr<TCPStream> fpSocket;

		State fState;
};

inline void TuxServer::Start()
{
	if (this->fState == TuxServer::State::INIT) {
		// throws runtime error
		(this->fAcceptor).Start();
		this->fState = TuxServer::State::STARTED;
	} else {
		std::stringstream ss;
		ss << "Server has already been started. IP: "
			<< (this->fAcceptor).GetIP()
			<< " Port: " << (this->fAcceptor).GetPort();
		throw std::runtime_error(ss.str());
	}
}

inline bool TuxServer::Accept(unsigned int timeout)
{
	if ((TuxServer::State::STARTED == this->fState) ||
		(TuxServer::State::CLOSED == this->fState))
	{
        (this->fpSocket).reset((this->fAcceptor).Accept(timeout));
		this->fState = (NULL != (this->fpSocket).get())
			? TuxServer::State::CONNECTED : this->fState;
		return NULL != (this->fpSocket).get();
	} else {
		std::stringstream ss;
		ss << "TuxServer can not accept incomming connections in the current state: "
			<< static_cast<int>(this->fState) << '.';
		throw std::runtime_error(ss.str());
	}
}

template <typename DATA_t>
inline ssize_t TuxServer::Send(std::vector<DATA_t> rInput)
{
	ssize_t retval = (this->fpSocket)->Send(rInput);
	// check for error
	if (0 >= retval) {
		(this->fState) = (retval == 0) ?
			TuxServer::State::CLOSED : TuxServer::State::ERROR;
		(this->fpSocket).reset(NULL);
	}
	return retval;
}

//template <typename DATA_t>
inline ssize_t TuxServer::Recv(std::string& rOutput,unsigned int Tout)
{
    rOutput = std::string(100,'\0');
    ssize_t retval = (this->fpSocket)->Receive(rOutput,Tout);
    // check for error
    if (0 >= retval) {
        (this->fState) = (retval == 0) ?
            TuxServer::State::CLOSED : TuxServer::State::ERROR;
        (this->fpSocket).reset(NULL);
    }
    return retval;
}


inline void TuxServer::SendEOF() {
	std::vector<uint32_t> eof;
	// eof.push_back(TuxServer::fgEOR);
	eof.push_back(0xffffffff);

	size_t retval = 0;
	while ( 0 == retval ) {
		retval = (this->fpSocket)->Send(eof);
		if (0 >= retval) {
			(this->fState) = (retval == 0) ?
				TuxServer::State::CLOSED : TuxServer::State::ERROR;
			(this->fpSocket).reset(NULL);
			return void();
		}
		std::cerr << "EOR send : " << retval << std::endl;
	}
}

#endif // TUXSERVER_H
