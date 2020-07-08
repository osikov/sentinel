#include "ProcUtils.hpp"
#include <stdio.h>
#include <string.h>

ProcUtils *ProcUtils::instance = NULL;

ProcUtils* ProcUtils::get()
{
    if (instance == NULL) instance = new ProcUtils();
    return instance;
}

ProcNetTcpRecord* ProcUtils::getTcpRecordBySocket(const char *id) const
{
    for(int i = 0; i < tcp.size(); i++)
    {
        if (!strcmp(tcp.at(i)->uid, id)) 
            return tcp.at(i);
    }
    
    return 0;
}


ProcUtils::ProcUtils()
{
    FILE *fh = fopen("/proc/net/tcp", "r");
    if (!fh) return;
    
    char buffer[1024];
    fgets(buffer, 1024, fh); // read header
    
    while(fgets(buffer, 1024, fh))
    {
        tcp.push_back(new ProcNetTcpRecord(buffer));
    }
    
    fclose(fh);
}


