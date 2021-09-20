#include <TcpCommandMaster.hh>
#include <iostream>


TcpCommandMaster::TcpCommandMaster() : rogue::interfaces::stream::Master() { }
TcpCommandMaster::~TcpCommandMaster() {}


void TcpCommandMaster::send_cmd(const std::string& cmd) {

    const uint size = cmd.length();
    char buffer[size+1];
    std::strcpy(buffer, cmd.c_str());
    rogue::interfaces::stream::FramePtr frame;
    rogue::interfaces::stream::FrameIterator it;
    //Request buffer 
    frame = reqFrame(size,true);

    //std::cout<<"size "<<size<<std::endl;
    //std::cout<<"m_cmd.data() " << m_cmd.data()<<std::endl;
    //std::cout<<"buffer " << buffer<<std::endl;
    
    // Update frame payload size
    frame->setPayload(size);
    
    // Get an iterator to the start of the Frame
    it = frame->begin();
    
    // Push the string
    rogue::interfaces::stream::toFrame(it,size,&buffer);
    sendFrame(frame);
}

