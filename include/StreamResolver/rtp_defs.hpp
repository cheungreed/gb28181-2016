/*
**	********************************************************************************
**
**	File		: rtp_defs.hpp
**	Description	: 
**	Modify		: 2020/1/17		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <memory>
#include <bits/shared_ptr.h>

namespace Zilu {
namespace StreamResolver {

struct RtpPayload
{
    uint8_t  ptype;         //< Payload type
    uint16_t seq;
    uint32_t ssrc;
    uint64_t ts;
    std::shared_ptr<uint8_t> payload;
    uint32_t p_len;         //< Payload length

public:
    RtpPayload() : ts(0), seq(0), ssrc(0), payload(nullptr), p_len(0) {}

    bool operator<(const RtpPayload& rhs) const {
        return this->seq < rhs.seq;
    }
    //比较时间戳，有小到大
    static bool CompareSequence(const RtpPayload& lhs, const RtpPayload& rhs) {
        //处理sequence溢出的场景 65534 65535 0 1 2 不参加排序
        if (lhs.seq > (uint16_t(-1) - 2) && rhs.seq < 2)
            return true;

        return lhs.seq < rhs.seq;
    }
};

struct block_t
{
    uint8_t     *p_buffer; //< Payload start
    uint32_t    i_buffer;  //< Payload length
};

}
}
