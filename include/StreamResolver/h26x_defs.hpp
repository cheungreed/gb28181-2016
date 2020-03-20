/*
**	********************************************************************************
**
**	File		: h26x_defs.hpp
**	Description	: 
**	Modify		: 2020/1/15		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

namespace Zilu {
namespace StreamResolver {

//H264
#define H264_STD_TYPE_I (5)
#define H264_STD_TYPE_P (1)
#define H264_STD_TYPE_SPS (7)
#define H264_STD_TYPE_PPS (8)
#define H264_STD_TYPE_SEI (6)

//H265
#define H265_STD_TYPE_VPS (32)
#define H265_STD_TYPE_SPS (33)
#define H265_STD_TYPE_PPS (34)
#define H265_STD_TYPE_SEI (39)
#define H265_STD_TYPE_I   (19)

enum MediaFrameType{
    MFT_AUDIO       = 0,
    MFT_H264_H265_I = 1,
    MFT_H264_H265_P = 2,
    MFT_H264_H265_B = 3,
    MRT_USER_DATA   = 4,
};

struct VideoFrameInfo
{
    int codec_type;     //H265 or 264
    int width;          //ÊÓÆµ¿í¸ß
    int heigth;
    int fps;            //Ö¡ÂÊ

    VideoFrameInfo() {
        Clear();
    }
    void Clear() {
        codec_type = 0;
        width = 0;
        heigth = 0;
        fps = 0;
    }
};

struct FrameData
{
    std::shared_ptr<uint8_t> frame_data;
    int frame_type;
    int frame_size;

};

struct SpsInfo
{
    uint32_t width;
    uint32_t height;
    uint32_t fps;
    SpsInfo() {
        width = 0;
        height = 0;
        fps = 25;
    }
};

}
}
