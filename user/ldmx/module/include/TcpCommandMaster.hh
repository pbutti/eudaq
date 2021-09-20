#include <rogue/interfaces/stream/Master.h>
#include <rogue/interfaces/stream/Frame.h>
#include <rogue/interfaces/stream/FrameIterator.h>

class TcpCommandMaster : public rogue::interfaces::stream::Master {
    
    std::string m_cmd;
    
public:
    
    //static std::shared_ptr<TcpCommandMaster> create();
    static std::shared_ptr<TcpCommandMaster> create() {
        static std::shared_ptr<TcpCommandMaster> ret =
            std::make_shared<TcpCommandMaster>();
        return(ret);
    }
    
    TcpCommandMaster();
    ~TcpCommandMaster();

    void send_cmd(const std::string& cmd);
};

typedef std::shared_ptr<TcpCommandMaster> TcpCommandMasterPtr;
    
