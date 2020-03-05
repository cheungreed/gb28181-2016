/*
**	********************************************************************************
**
**	File		: query_operation_defs.h
**	Description	: 
**	Modify		: 2020/3/2		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <list>
#include "control_defs.h"
#include "common_defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

struct manscdp_device_status_dialog_t
{
    struct request_t : public common_item_t{
        request_t() {
            cmdtype = "DeviceStatus";
        }
    };
    struct response_t : public common_item_t{
        struct item {
            string         devid;
            string         dutystatus;
        };
    public:
        response_t() {
            cmdtype = "DeviceStatus";
        }
        result_type_e      result;
        result_type_e      work_status;         ///<是否正常工作
        status_type_e      encode;
        status_type_e      record;
        string             dev_time;
        string             onoff_line;
        string             reason;
        vector<item>       alarm_status;
    };

public:
    request_t   request;
    response_t  response;
};

struct manscdp_cataloginfo_dialog_t
{
    struct request_t : public common_item_t{
    public:
        request_t() {
            cmdtype = "Catalog";
        }
        string      startime;
        string      endtime;
    };
    struct response_t : public common_item_t {
        struct item {
            struct info_t {
                uint8_t         ptztype;
                uint8_t         position_type;
                uint8_t         roomtype;
                uint8_t         usetype;
                uint8_t         supplylight_type;
                uint8_t         direction_type;
                string          resolution;
                string          business_groupid;
                string          download_speed;
                uint8_t         svc_space_supportmode;
                uint8_t         svc_time_supportmode;
            };
        public:
            string      devid;          ///< must
            string      name;           ///< must
            string      manufacturer;   ///< must
            string      model;          ///< must
            string      owner;          ///< must
            string      civilcode;      ///< must
            string      block;
            string      address;
            uint8_t     parental;
            string      parentid;
            uint8_t     safetyway;
            uint8_t     register_way;
            string      certnum;
            uint8_t     certifiable;
            uint8_t     errcode;
            string      endtime;
            uint8_t     secrecty;
            string      ipaddress;
            uint8_t     port;
            string      passwd;
            status_type_e   status;
            double      longitude;
            double      latitude;
            info_t      info;
        };
    public:
        response_t() {
            cmdtype = "Catalog";
        }

        uint32_t    sum;
        list<item>  devlist;
    }; ///< response_t

public:
    request_t           request;
    response_t          response;
};

struct manscdp_devinfo_dialog_t
{
    struct request_t : public common_item_t{
        request_t() {
            cmdtype = "DeviceInfo";
        }
    };
    struct response_t : public common_item_t{
        string          devname;
        result_type_e   result;
        string          manufacturer;
        string          model;
        string          firmware;
        uint8_t         channel;
        response_t() {
            cmdtype = "DeviceInfo";
        }
    };

public:
    request_t       request;
    response_t      response;
};

struct manscdp_recordinfo_dialog_t
{
    struct request_t : public common_item_t{
        string      starttime;
        string      endtime;
        string      filepath;
        string      address;
        uint8_t     secrecy;
        string      record_type;        ///< time/alarm/manual/all
        string      recordid;
        string      indistinct_query;
    public:
        request_t() {
            cmdtype = "RecordInfo";
        }
    };
    struct response_t : public common_item_t {
        struct item {
            string      devid;
            string      devname;
            string      filepath;
            string      address;
            string      starttime;
            string      endtime;
            uint8_t     secrecy;
            string      type;
            string      recordid;
            uint32_t    filesize;
        };
    public:
        uint32_t        sum;
        uint32_t        item_num;
        string          name;
        list<item>      recordlist;

        response_t() {
            cmdtype = "RecordInfo";
        }
    };

public:
    request_t           request;
    response_t          response;
};

struct manscdp_config_download_dialog_t
{
    struct request_t : public common_item_t{
        string          configtype;         ///< 多项 / 分隔

    public:
        request_t() {
            cmdtype = "ConfigDownload";
        }
    };

    struct response_t : public common_item_t {
        struct basic_param {
            uint8_t                     position_capability;
            double                      longitude;
            double                      latitude;
            manscdp_basicparam_cfg_t    basic_cfg;
        };
        struct video_param_opt {
            string                      download_speed;
            string                      resultion;
        };

    public:
        response_t() {
            cmdtype = "ConfigDownload";
        }
        result_type_e   result;
        basic_param     basic;
        video_param_opt video;
    };

public:
    request_t       request;
    response_t      response;
};

struct manscdp_preset_query_dialog_t
{
    struct request_t : public common_item_t{
        request_t() {
            cmdtype = "PresetQuery";
        }
    };
    struct response_t : public common_item_t {
        struct item {
            string      preset_id;
            string      preset_name;
        };
    public:
        response_t() {
            cmdtype = "PresetQuery";
        }
        uint32_t        item_num;
        list<item>      preset_list;
    };

public:
    request_t           request;
    response_t          response;
};

struct manscdp_alarm_query_dialog_t
{
    struct request_t : public common_item_t {
        string          start_alarmpriority;
        string          end_alarmpriority;
        string          alarm_method;
        string          alarmtype;
        string          start_alarmtime;
        string          end_alarmtime;
    public:
        request_t() {
            cmdtype = "Alarm";
        }
    };

public:
    request_t           request;
    //TODO response
};

}
}
}
