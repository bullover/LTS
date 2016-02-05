#include "Socket/TuxServer.h"

TuxServer::TuxServer(const unsigned int PORT, const std::string& rIP)
        : fAcceptor(PORT, rIP), fpSocket(nullptr), fState(TuxServer::State::INIT)
{
}
