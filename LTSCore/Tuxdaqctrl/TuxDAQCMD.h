#ifndef TUXDAQCMD_H
#define TUXDAQCMD_H

#include <cstdint>

namespace LTS
{
	namespace TuxDAQCMD
	{
		static const uint32_t gCONFIGURE	= 0x10000000;
		static const uint32_t gACK				= 0x11000000;

		static const uint32_t gEXCECUTE		= 0x20000000;
		static const uint32_t gPERFORMED	= 0x21000000;

		static const uint32_t gFINISH			= 0x30000000;

		static const uint32_t gSTOP				= 0x40000000;

		static const uint32_t gERROR			= 0x50000000;
	}
}

#endif // TUXDAQCMD_H
