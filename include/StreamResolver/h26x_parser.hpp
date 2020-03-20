/*
**	********************************************************************************
**
**	File		: h26x_Parser.hpp
**	Description	: 
**	Modify		: 2020/1/15		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <vector>
//#include "../Src/h26x/MediaBuffer.h"
#include "h26x_defs.hpp"

namespace Zilu {
namespace StreamResolver {

class CH26xParser
{
public:
    CH26xParser();
    ~CH26xParser();
#if 0
    int PushFrameData(const uint8_t *p_data, uint32_t i_size);
    int Decode(std::vector<FrameData> &frame, int codecType);
    int GetFrameInfo(VideoFrameInfo& finfo);
    void Clear();
#endif
    int GetOneNalu(int &endIndex, const uint8_t *p_data, uint32_t i_size);
    int PeekVideoCodecType(const uint8_t *p_data, uint32_t i_size);
    int PeekFrameTypeH264(const uint8_t *pFrame, uint32_t nFrameSize);
    int PeekFrameTypeH265(const uint8_t *pFrame, uint32_t nFrameSize);

    void ParseSpsInfo(int frameType, SpsInfo &sps, const uint8_t *p_data, uint32_t i_size);

#if 0
private:
    VideoFrameInfo  m_frameinfo;
    CMediaBuffer    m_cacheSPS;
    CMediaBuffer    m_cachePPS;
    CMediaBuffer    m_cacheVPS;
    CMediaBuffer    m_cacheSEI;
    CMediaBuffer    m_videoFrame;
#endif
};

}
}
