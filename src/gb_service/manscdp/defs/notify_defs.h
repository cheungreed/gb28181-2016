/*
**	********************************************************************************
**
**	File		: notify_dialogs_defs.h
**	Description	: 
**	Modify		: 2020/3/3		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "common_defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

struct manscdp_keepalive_dialogs_t
{
    struct request_t : public common_item_t {
        struct info {
            string      devid;
        };
    public:
        request_t() {
            cmdtype = "Keepalive";
        }
        result_type_e       result;
        list<info>          error_devlist;
    };
    struct response_t : public common_answer_t {
        response_t() {
            cmdtype = "Keepalive";
        }
    };
public:
    request_t       request;
    response_t      response;
};

struct manscdp_alarm_notify_dialogs_t
{
    struct request_t : public common_item_t {
        struct info {
            struct param {
                uint32_t        eventtype;
            };
        public:
            uint32_t        alarmtype;
            param           alarmtype_param;
        };
    public:
        request_t() {
            cmdtype = "Alarm";
        }
        string              alarm_priority;
        string              alarm_method;
        string              alarmtime;
        string              alarm_description;
        double              longitude;
        double              latitude;
        list<info>          info_list;
    };
    struct response_t : public common_answer_t {
        response_t() {
            cmdtype = "Alarm";
        }
    };

public:
    request_t           request;
    response_t          response;
};

struct manscdp_media_status_dialogs_t
{
    struct request_t : public common_item_t {
    public:
        request_t() {
            cmdtype = "MediaStatus";
        }
        string      notifytype;
    };
    struct response_t : public common_answer_t {
    public:
        response_t() {
            cmdtype = "MediaStatus";
        }
    };

public:
    request_t       request;
    response_t      response;
};

struct manscdp_broadcast_dialogs_t
{
    struct request_t : public common_item_t {
    public:
        request_t() {
            cmdtype = "Broadcast";
        }
        string          sourceid;
        string          targetid;
    };
    struct response_t : public common_answer_t {
    public:
        response_t() {
            cmdtype = "Broadcast";
        }
    };

public:
    request_t       request;
    response_t      response;
};

}
}
}