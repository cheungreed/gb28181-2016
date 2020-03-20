/*
**	********************************************************************************
**
**	File		: call_defs.h
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include <map>

using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

enum media_operate_type_e
{
    MOT_UNKNOWN  =0,
    MOT_PLAY,
    MOT_DOWNLOAD,
    MOT_PLAYBACK,
    MOT_TALK,
};

struct sdp_description_t
{
    string      v_version;

    ///< o=34020000002000000001 B8975BC37429408E83BBAB32535D07B2 0 IN IP4 172.18.3.203
    string      o_username;         ///< owner/creator
    string      o_sessid;
    string      o_sess_version;
    string      o_nettype;
    string      o_addrtype;
    string      o_addr;

    ///< s=Play
    string      s_sess_name;        ///< ¶ÔÓ¦media_operate_type_e

    ///< u=34020000001320000002
    string      u_uri;

    ///< c=IN IP4 172.18.3.204
    string      c_nettype;
    string      c_addrtype;
    string      c_addr;

    ///< t= 0 0
    string      t_startime;
    string      t_endtime;

    ///< m=video 38930 RTP/AVP 96 98 97
    string      m_media;            ///< media type
    string      m_port;
    string      m_tmode;            ///< TCP(TCP/RTP/AVP) or UDP£¨RTP/AVP£©media transmit mode
    string      m_payloadtype;      ///< payload type

    ///< a=recvonly
    ///< a=rtpmap:96 PS/90000
    map<string, string> a_attrimap; ///< a×Ö¶Î

    ///< y=0200003000
    string      y_ssrc;

    ///< f=v/1/5/3///a///
    string      f_media_desc;

    void print() {
        LOG_DEBUG << "o_username: " << o_username;
        LOG_DEBUG << "o_sessid: " << o_sessid;
        LOG_DEBUG << "o_sess_version: " << o_sess_version;
        LOG_DEBUG << "o_nettype: " << o_nettype;
        LOG_DEBUG << "o_addrtype: " << o_addrtype;
        LOG_DEBUG << "o_addr: " << o_addr;
        LOG_DEBUG << "s_sess_name: " << s_sess_name;
        LOG_DEBUG << "u_uri: " << u_uri;
        LOG_DEBUG << "c_nettype: " << c_nettype;
        LOG_DEBUG << "c_addrtype: " << c_addrtype;
        LOG_DEBUG << "c_addr: " << c_addr;
        LOG_DEBUG << "t_startime: " << t_startime;
        LOG_DEBUG << "t_endtime: " << t_endtime;
        LOG_DEBUG << "m_media: " << m_media;
        LOG_DEBUG << "m_port: " << m_port;
        LOG_DEBUG << "m_tmode: " << m_tmode;
        LOG_DEBUG << "m_payloadtype: " << m_payloadtype;
        LOG_DEBUG << "y_ssrc: " << y_ssrc;
        LOG_DEBUG << "f_media_desc: " << f_media_desc;

        for (auto a : a_attrimap) {
            LOG_DEBUG << "attri key: " << a.first << ", value: " << a.second;
        }
    }
};

}
}
}
