/*
**	********************************************************************************
**
**	File		: sdp.cpp
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#include "sdp.h"
#include "osipparser2/sdp_message.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {


CSdpHandle::CSdpHandle()
{

}

CSdpHandle::~CSdpHandle()
{

}

int CSdpHandle::ParseSdp(const char *buf, sdp_description_t &sdp_desc)
{
    if (buf == nullptr) {
        return -1;
    }

    sdp_message_t* osip_sdp;
    sdp_message_init(&osip_sdp);
    int r = sdp_message_parse(osip_sdp, buf);
    if (r != 0) {
        sdp_message_free(osip_sdp);
        return -2;
    }

    sdp_desc.v_version = osip_sdp->v_version;
    sdp_desc.o_addr = osip_sdp->o_addr;
    sdp_desc.o_addrtype = osip_sdp->o_addrtype;
    sdp_desc.o_nettype = osip_sdp->o_nettype;
    sdp_desc.o_sess_version = osip_sdp->o_sess_version;
    sdp_desc.o_sessid = osip_sdp->o_sess_id;
    sdp_desc.s_sess_name = osip_sdp->s_name;
    sdp_desc.u_uri = osip_sdp->u_uri;
    sdp_desc.c_addr = osip_sdp->c_connection->c_addr;
    sdp_desc.c_addrtype = osip_sdp->c_connection->c_addrtype;
    sdp_desc.c_nettype = osip_sdp->c_connection->c_nettype;

    sdp_time_descr_t* t_ele = (sdp_time_descr_t*)osip_list_get(&(osip_sdp->t_descrs), 0);
    if (t_ele != nullptr) {
        sdp_desc.t_startime = t_ele->t_start_time;
        sdp_desc.t_endtime = t_ele->t_stop_time;
    }

    sdp_media_t* m_ele = (sdp_media_t*)osip_list_get(&(osip_sdp->m_medias), 0);
    if (m_ele != nullptr) {
        sdp_desc.m_media = m_ele->m_media;
        sdp_desc.m_tmode = m_ele->m_proto;
        sdp_desc.m_port = m_ele->m_port;
        ///< TODO maybe need to reparse "m" oneself;
    }

    for (uint32_t i = 0; i < osip_sdp->a_attributes.nb_elt; ++i)
    {
        sdp_attribute_t* a_ele = (sdp_attribute_t*)osip_list_get(&(osip_sdp->a_attributes), i);
        if (a_ele != nullptr) {
            sdp_desc.a_attrimap.insert(make_pair(a_ele->a_att_field, a_ele->a_att_value));
        }
    }

    ///< TODO parse y= f=

    sdp_desc.print();

    sdp_message_free(osip_sdp);

    return 0;
}

int CSdpHandle::AssembleSdp(string &sdp, const sdp_description_t &sdp_desc)
{
    return 0;
}

}
}
}
