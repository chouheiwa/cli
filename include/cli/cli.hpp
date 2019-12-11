#pragma once


#include <iostream>
#include <memory>
#include "cli_types.h"
namespace kgt { namespace rpc {
    
    class wallet;
    /**
     *  Provides a simple wrapper for RPC calls to a given interface.
     */
    class cli
    {
    public:
        cli();
        int init(std::string& chain_id, std::string& ws_server, int timeout);
        int runCommand(api_type api,std::string& command,std::string& result);
        void setTimeout(int timeout);
        
    private:
        std::shared_ptr<kgt::rpc::wallet> _wallet;
    };
} }


