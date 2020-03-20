/*
**	********************************************************************************
**
**	File		: RtpHeader.hpp
**	Description	: 
**	Modify		: 2019/12/25		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <cstdint>
#include <memory>
#include <cstring>

namespace Zilu {
namespace StreamResolver {

struct CRtpHeader
{
    // version, must be 2, 2bit
    uint8_t V;

    // padding flag, 1bit
    uint8_t P;

    // If the extension bit is set, the fixed header must be followed by exactly one header extension, 1bit
    uint8_t X;

    // CSRC count,4bit
    uint8_t CC;

    // market. for video, the end of frame. for audio, the start of frame
    uint8_t M;

    // payload type
    uint8_t PT;
    uint16_t seq;
    uint32_t ts;    //< timestamp
    uint32_t SSRC;

    // CSRC[0~15], nums according CC value.
    uint32_t CSRC[16];

    // if X == 1, header extension must exist
    uint16_t extension_profile;
    uint16_t extension_len;

//    std::shared_ptr<uint8_t> header_extension_data;

    /// 1 == P 存在填充标志
    uint16_t padding_len;

    CRtpHeader () : V(0), P(0), X(0), CC(0), M(0), PT(0), seq(0), ts(0), SSRC(0)
    {
        memset(CSRC, 0, sizeof(CSRC));
        extension_profile = 0;
        extension_len = 0;
        padding_len = 0;

        m_headsize = 0;
    }

public:
    uint32_t m_headsize;
public:
    int Decode(uint8_t *p, uint32_t dsize);
};

}
}
