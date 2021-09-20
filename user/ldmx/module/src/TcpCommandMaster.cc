#include <TcpCommandMaster.hh>
#include <iostream>


TcpCommandMaster::TcpCommandMaster() : rogue::interfaces::stream::Master() { }
TcpCommandMaster::~TcpCommandMaster() {}


void TcpCommandMaster::send_config_cmd () {
    
    m_cmd = "configure";
    const uint size = m_cmd.length();
    //unsigned char buffer[size];
    //memcpy(buffer, m_cmd.data(), size);
    char buffer[size+1];
    std::strcpy(buffer, m_cmd.c_str());

    std::cout<<"size "<<size<<std::endl;
    std::cout<<"m_cmd.data() " << m_cmd.data()<<std::endl;
    std::cout<<"buffer " << buffer<<std::endl;
    
    
    rogue::interfaces::stream::FramePtr frame;
    rogue::interfaces::stream::FrameIterator it;
    
    //Request buffer 
    frame = reqFrame(size,true);
    
    // Update frame payload size
    frame->setPayload(size);
    
    // Get an iterator to the start of the Frame
    it = frame->begin();
    
    // Push the string
    rogue::interfaces::stream::toFrame(it,size,&buffer);
    
    
    std::cout<<"Sending frame... "<<buffer<<std::endl; 
    sendFrame(frame);
}
    
