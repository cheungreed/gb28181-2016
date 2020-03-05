/*
**	********************************************************************************
**
**	File		: defs.h
**	Description	: 
**	Modify		: 2020/2/29		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include <vector>
#include "control_defs.h"
#include "query_defs.h"
#include "notify_defs.h"
#include "tinyxml2/tinyxml2.h"

using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

enum manscdp_cmd_category_e
{
    MANSCDP_CMD_CATEGORY_CONTROL,
    MANSCDP_CMD_CATEGORY_QUERY,
    MANSCDP_CMD_CATEGORY_NOTIFY,
    MANSCDP_CMD_CATEGORY_RESPONSE,

    MANSCDP_CMD_CATEGORY_MAX,
    MANSCDP_CMD_CATEGORY_UNKNOWN = MANSCDP_CMD_CATEGORY_MAX
};

enum manscdp_cmdtype_e
{
    //< Control
    MANSCDP_CONTROL_CMD_DEVICE_CONTROL,      ///<设备控制
    MANSCDP_CONTROL_CMD_DEVICE_CONFIG,       ///<设备配置

    //< Query
    MANSCDP_QUERY_CMD_DEVICE_STATUS,        ///<设备控制
    MANSCDP_QUERY_CMD_CATALOG,              ///<设备目录查询
    MANSCDP_QUERY_CMD_DEVICE_INFO,          ///<设备信息查询
    MANSCDP_QUERY_CMD_RECORD_INFO,          ///<文件目录检索
    MANSCDP_QUERY_CMD_ALARM,                ///<报警查询
    MANSCDP_QUERY_CMD_CONFIG_DOWNLOAD,      ///<设备配置查询
    MANSCDP_QUERY_CMD_PRESET_QUERY,         ///<预置位查询
    MANSCDP_QUERY_CMD_MOBILE_POSITION,      ///<移动设备位置数据查询

    //< Notify
    MANSCDP_NOTIFY_CMD_KEEPALIVE,           ///<设备状态信息报送，保活
    MANSCDP_NOTIFY_CMD_ALARM,               ///<报警通知
    MANSCDP_NOTIFY_CMD_MEDIA_STATUS,        ///<媒体通知
    MANSCDP_NOTIFY_CMD_BROADCASE,           ///<语音广播通知
    MANSCDP_NOTIFY_CMD_MOBILE_POSITION,     ///<移动设备位置通知

    MANSCDP_CMD_TYPE_MAX,
    MANSCDP_CMD_TYPE_UNKNOWN = MANSCDP_CMD_TYPE_MAX
};

enum manscdp_devicecontrol_subcmd_e
{
     PTZCmd = 1,
     TeleBoot,
     RecordCmd,
     GuardCmd,
     AlarmCmd = 5,
     IFrameCmd,
     DragZoomIn,
     DragZoomOut,
     HomePosition
};

enum manscdp_switch_status_e
{
    SWITCH_ON,          ///< 状态切换  开关
    SWITCH_OFF
};

enum manscdp_deviceconfig_subcmd_e
{
    BasicParam = 1,
    SVACEncodeConfig,
    SVACDecodeConfig
};

typedef vector<manscdp_devicecontrol_subcmd_e> manscdp_devicecontrol_subcmd_t;
typedef vector<manscdp_deviceconfig_subcmd_e> manscdp_deviceconfig_subcmd_t;
typedef vector<string> manscdp_configdownload_subcmd_t;

typedef tinyxml2::XMLDocument   tinyxml_doc_t;
typedef tinyxml2::XMLElement    tinyxml_ele_t;

///< MANSCDP xml消息 必选项如下
/*
 * <?xml version="1.0"?>
 * <Query>
 * <CmdType>Catalog</CmdType>
 * <SN>27</SN>
 * <DeviceID>1234</DeviceID>
 * </Query>
 */
struct manscdp_msgbody_header_t
{
    manscdp_cmd_category_e              cmd_category;
    manscdp_cmdtype_e                   cmd_type;
    string                              sn;
    string                              devid;
    manscdp_deviceconfig_subcmd_t       devcfg_subcmd;
    manscdp_devicecontrol_subcmd_t      devctl_subcmd;
    manscdp_configdownload_subcmd_t     cfgdownload_subcmd;
};

}
}
}
