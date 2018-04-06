//
//  cli_types.h
//  BitShares
//
//  Created by flh on 2018/4/6.
//

#ifndef cli_types_h
#define cli_types_h

#define Success 0
#define Chain_ID ""

namespace kgt { namespace rpc {
    enum api_type {
        wallet_api_type = 0,
        database_api_type = 1
    };
}}

#endif /* cli_types_h */
