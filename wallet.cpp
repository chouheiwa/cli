#include <cli/wallet.hpp>
#include <fc/thread/thread.hpp>

#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef HAVE_READLINE
# include <readline/readline.h>
# include <readline/history.h>
// I don't know exactly what version of readline we need.  I know the 4.2 version that ships on some macs is
// missing some functions we require.  We're developing against 6.3, but probably anything in the 6.x
// series is fine
# if RL_VERSION_MAJOR < 6
#  ifdef _MSC_VER
#   pragma message("You have an old version of readline installed that might not support some of the features we need")
#   pragma message("Readline support will not be compiled in")
#  else
#   warning "You have an old version of readline installed that might not support some of the features we need"
#   warning "Readline support will not be compiled in"
#  endif
#  undef HAVE_READLINE
# endif
# ifdef WIN32
#  include <io.h>
# endif
#endif

namespace kgt { namespace rpc {
    
    static std::vector<std::string>& wallet_commands()
    {
        static std::vector<std::string>* cmds = new std::vector<std::string>();
        return *cmds;
    }
    
    wallet::wallet() {
        
    }
    
    wallet::~wallet() {
    }
    
    variant wallet::send_call( api_id_type api_id, string method_name, variants args /* = variants() */ )
    {
        FC_ASSERT(false);
    }
    
    variant wallet::send_callback( uint64_t callback_id, variants args /* = variants() */ )
    {
        FC_ASSERT(false);
    }
    
    void wallet::send_notice( uint64_t callback_id, variants args /* = variants() */ )
    {
        FC_ASSERT(false);
    }
    
    int wallet::init(std::string& chain_id, std::string& ws_server, int timeout) {
        try{
            wdata = std::make_shared<wallet_data>();
            wdata->chain_id = chain_id_type(chain_id);
            wdata->ws_server = ws_server;
            client = std::make_shared<fc::http::websocket_client>();
            con  = client->connect( wdata->ws_server, fc::milliseconds(timeout) );
            apic = std::make_shared<fc::rpc::websocket_api_connection>(*con.get());
            fc::api<login_api> remote_api = apic->get_remote_api< login_api >(1);
            FC_ASSERT( remote_api->login( wdata->ws_user, wdata->ws_password ) );
            wapiptr = std::make_shared<wallet_api>( *wdata.get(), remote_api );
            
            wapi = std::make_shared<fc::api<wallet_api>>(wapiptr);
            for( auto& name_formatter : wapiptr->get_result_formatters() )
                this->format_result( name_formatter.first, name_formatter.second );
//            for( auto& name_formatter : dapiptr->get_result_formatters() )
//                this->format_result( name_formatter.first, name_formatter.second );
            
            this->register_api(*wapi.get());
            this->register_api(remote_api->database());
            this->start();
//            std::string str2;
//            str2 = "info";
//            std::string str3;
//            this->runCommand(0, str2, str3);
        }
        catch ( const fc::exception& e )
        {
            std::cout << e.to_detail_string() << "\n";
            return -1;
        }
        
        return 0;
    }
    
    void wallet::start()
    {
        wallet_commands() = get_method_names(0);
    }

void wallet::setTimeout(int timeout) {
    if (apic.get()) apic->setTimeout(timeout);
}
    
    void wallet::format_result( const string& method, std::function<string(variant,const variants&)> formatter)
    {
        _result_formatters[method] = formatter;
    }
    
    void wallet::setShowError(int showError)
    {
        _showError = showError;
    }
    
    void wallet::set_prompt( const string& prompt )
    {
        _prompt = prompt;
    }

    int wallet::runCommand(api_type api, string &line, string &results)
    {
        try
        {
            line += char(EOF);
            fc::variants args = fc::json::variants_from_string(line);;
            
            if( args.size() == 0 )
                return 0;
            
            const string& method = args[0].get_string();
            
            auto result = receive_call( api, method, variants( args.begin()+1,args.end() ) );
            results = fc::json::to_pretty_string( result );
            
            return 0;
        }
        catch ( const fc::exception& e )
        {
            std::cout << e.to_detail_string() << "\n";
//            if (_showError != 0) {
//                std::cout << e.to_detail_string() << "\n";
//            }
            return e.code();
        }
    }
    
    char * dupstr (const char* s) {
        char *r;
        
        r = (char*) malloc ((strlen (s) + 1));
        strcpy (r, s);
        return (r);
    }
    
    char* my_generator(const char* text, int state)
    {
        static int list_index, len;
        const char *name;
        
        if (!state) {
            list_index = 0;
            len = strlen (text);
        }
        
        auto& cmd = wallet_commands();
        
        while( list_index < cmd.size() )
        {
            name = cmd[list_index].c_str();
            list_index++;
            
            if (strncmp (name, text, len) == 0)
                return (dupstr(name));
        }
        
        /* If no names matched, then return NULL. */
        return ((char *)NULL);
    }
    
    void wallet::getline( const fc::string& prompt, fc::string& line)
    {
        std::cout << prompt;
        // sync_call( cin_thread, [&](){ std::getline( *input_stream, line ); }, "getline");
        fc::getline( fc::cin, line );
    }
    
} } // namespace fc::rpc

