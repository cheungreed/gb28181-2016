/*
**	********************************************************************************
**
**	File		: ps2es.h
**	Description	:
**	Modify		: 2019/10/16		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <stdint.h>
#include <vector>
#include <cstring>
#include <memory>
#include "ps_def.hpp"

namespace Zilu {
namespace StreamResolver {

class CPsDemux {

public:
    /**
     * decode ps
     * @param ps
     * @param ps_size
     * @param ps_pack_header. free outside
     * @param ps_system_header. free outside
     * @param ps_system_map. free outside
     * @param es_array. should free outside
     * @return
     */
    int Decode(uint8_t *ps, int ps_size,
               PsPackHeader &ps_pack_header,
               PsSystemHeader &ps_system_header,
               PsSystemMap & ps_sysmap,
               std::vector<EsInfo> &es_array);

private:
    int decode_pack_header(uint8_t *ps, int ps_size, int &offset, PsPackHeader &pack_header);

    int decode_system_header(uint8_t*ps, int ps_size, int &offset, PsSystemHeader& sysheader);

    int decode_system_map(uint8_t *ps, int ps_size, int &offset, PsSystemMap& es);

    int decode_pes(uint8_t *ps, int ps_size, int &offset, EsInfo *es);

private:
    int ps_pkt_size( const uint8_t *p, int i_peek );


public:
    static bool ps_stream_id_peek(const uint8_t *bits, int expected);

    static int get_ps_unit_size(int &startIndex, int &psUnitSize, const uint8_t* pdata, uint32_t datasize);
};

}
}
