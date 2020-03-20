/*
**	********************************************************************************
**
**	File		: rtp_demux.hpp
**	Description	: 
**	Modify		: 2019/12/22		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <vector>
#include <cstdint>
#include "rtp_header.hpp"
#include "rtp_defs.hpp"

#define RTP_PAYLOAD_TYPE_PS (96)

#define TIMESTAMP_KIND 3

namespace Zilu {
namespace StreamResolver {

class CRtpDemux
{
public:
    CRtpDemux();
    ~CRtpDemux();

    /*
     * Handle RTP packet, extract payload
     * @param pRaw
     */
    int DecodeRawData(uint8_t *pRaw, uint32_t nSize, RtpPayload& payload);

    int AppendPayload(RtpPayload &payload);

    ///< 预处理rtp负载，重新排序序列，判断是否丢帧，主动丢掉缺失的相同时间戳的数据
    ///< 调用FetchFrame前，先调用，只调用一次
    int PreprocessRtpPayload();

    int FetchFrame(RtpPayload& payload);

    ///< 判断此帧数据是否可以Append，true-可以Append，false-需要先取数据处理再Append
    bool IsAppendData(RtpPayload &payload);

    ///判断是否丢失P帧，丢失后，之后的P帧需全部丢弃
    bool IsLostPframe();

private:
    int clean_by_timestamp(uint32_t timestamp);

private:
    bool                m_bLostPframe;         //P帧丢失，后面所有P帧丢弃，等待下个I帧
    int32_t             m_nPreSequence;       //记录上一个比较单元最后一个sequence
    CRtpHeader           m_rtpHeader;
    std::list<RtpPayload> m_lRtpPayload;      //存放三个不同时间戳的数据,作为一个比较单元
    std::vector<uint32_t> m_vTimestamp;         //计数时间戳个数，3个不同时间戳
#ifdef DEBUG_
    uint16_t    m_nLastSequence;
#endif
};

}
}
