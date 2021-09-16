#include "eudaq/Producer.hh"
#include <iostream>
#include <fstream>
#include <ratio>
#include <chrono>
#include <thread>
#include <random>
#ifndef _WIN32
#include <sys/file.h>


//Add rogue dep
#include "rogue/utilities/Prbs.h"
#include "rogue/interfaces/stream/TcpClient.h"
#include "rogue/interfaces/stream/Slave.h"
#include <rogue/Helpers.h>

#endif
//----------DOC-MARK-----BEG*DEC-----DOC-MARK----------
class LdmxProducer : public eudaq::Producer {
public:
    LdmxProducer(const std::string & name, const std::string & runcontrol);
    void DoInitialise() override;
    void DoConfigure() override;
    void DoStartRun() override;
    void DoStopRun() override;
    void DoTerminate() override;
    void DoReset() override;
    void RunLoop() override;
  
    static const uint32_t m_id_factory = eudaq::cstr2hash("LdmxProducer");
private:
    bool m_flag_ts;
    bool m_flag_tg;
    uint32_t m_plane_id;
    FILE* m_file_lock;
    std::chrono::milliseconds m_ms_busy;
    bool m_exit_of_run;


    
    //--- The tcp client where to receive frames ---//
    rogue::interfaces::stream::TcpClientPtr m_tcpClient;
    std::string m_addr = "127.0.0.1";
    int         m_port = 8000;

    //--- The Prbs receiver ---//
    //Placeholder for the real HGCAL slave --//
    rogue::utilities::PrbsPtr m_prbs = rogue::utilities::Prbs::create();
    
};
//----------DOC-MARK-----END*DEC-----DOC-MARK----------
//----------DOC-MARK-----BEG*REG-----DOC-MARK----------
namespace{
    auto dummy0 = eudaq::Factory<eudaq::Producer>::
        Register<LdmxProducer, const std::string&, const std::string&>(LdmxProducer::m_id_factory);
}
//----------DOC-MARK-----END*REG-----DOC-MARK----------
//----------DOC-MARK-----BEG*CON-----DOC-MARK----------
LdmxProducer::LdmxProducer(const std::string & name, const std::string & runcontrol)
    :eudaq::Producer(name, runcontrol), m_file_lock(0), m_exit_of_run(false){  
}
//----------DOC-MARK-----BEG*INI-----DOC-MARK----------
void LdmxProducer::DoInitialise(){
    auto ini = GetInitConfiguration();
    std::string lock_path = ini->Get("EX0_DEV_LOCK_PATH", "ex0lockfile.txt");
    m_file_lock = fopen(lock_path.c_str(), "a");
#ifndef _WIN32
    if(flock(fileno(m_file_lock), LOCK_EX|LOCK_NB)){ //fail
        EUDAQ_THROW("unable to lock the lockfile: "+lock_path );
    }
#endif

    
    //Connection information to the remote rogue server
    //Address and Port are obtained from external configuration
 
    m_addr = ini->Get("LDMX_TCPCLIENT_ADDR","127.0.0.1");
    m_port = ini->Get("LDMX_TCPCLIENT_PORT",9999);
    
    EUDAQ_INFO("Setting up TcpClient addr: " + m_addr +" port: " +std::to_string(m_port));

    
    m_tcpClient = rogue::interfaces::stream::TcpClient::create(m_addr,m_port);
        
    //Connect rogueStreamConnect(master, slave)
    rogueStreamConnect(m_tcpClient, m_prbs);
        
    EUDAQ_INFO("Tcp stream connected to receiver...");
}

//----------DOC-MARK-----BEG*CONF-----DOC-MARK----------
void LdmxProducer::DoConfigure(){
    auto conf = GetConfiguration();
    conf->Print(std::cout);
    
    //... Send the configuration to the hardware ...
    
}
//----------DOC-MARK-----BEG*RUN-----DOC-MARK----------
void LdmxProducer::DoStartRun(){
    m_exit_of_run = false;
}
//----------DOC-MARK-----BEG*STOP-----DOC-MARK----------
void LdmxProducer::DoStopRun(){
    m_exit_of_run = true;
}
//----------DOC-MARK-----BEG*RST-----DOC-MARK----------
void LdmxProducer::DoReset(){
    m_exit_of_run = true;
    if(m_file_lock){
#ifndef _WIN32
        flock(fileno(m_file_lock), LOCK_UN);
#endif
        fclose(m_file_lock);
        m_file_lock = 0;
    }
    m_ms_busy = std::chrono::milliseconds();
    m_exit_of_run = false;
}
//----------DOC-MARK-----BEG*TER-----DOC-MARK----------
void LdmxProducer::DoTerminate(){
    m_exit_of_run = true;
    if(m_file_lock){
        fclose(m_file_lock);
        m_file_lock = 0;
    }
}
//----------DOC-MARK-----BEG*LOOP-----DOC-MARK----------
void LdmxProducer::RunLoop(){

    EUDAQ_INFO("In RunLoop() !!!");
    int rx_counts = 0;
    int i = 0;
    while(!m_exit_of_run){
        i+=1;
        rx_counts  = m_prbs->getRxCount();
        
        if (i % 10000 == 0)
            std::cout<<"rx_counts "<< rx_counts<<std::endl;
    }
}
//----------DOC-MARK-----END*IMP-----DOC-MARK----------
