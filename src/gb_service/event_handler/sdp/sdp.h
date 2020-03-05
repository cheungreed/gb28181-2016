/*
**	********************************************************************************
**
**	File		: sdp.h
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "call_defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CSdpHandle
{
public:
    CSdpHandle();
    ~CSdpHandle();

    int ParseSdp(const char *buf, sdp_description_t &sdp_desc);
    int AssembleSdp(string &sdp, const sdp_description_t &sdp_desc);
};

}
}
}