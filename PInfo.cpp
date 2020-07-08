#include "PInfo.hpp"
#include "ProcUtils.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>



PInfo::PInfo(int _pid)
{
    pid = _pid;
    parent = 0;
}

bool PInfo::init(std::vector<std::string>& connections)
{
    char fname [255];
    sprintf(fname, "/proc/%d/stat", pid);
    FILE *fh = fopen(fname, "r");
    if (!fh) return false;
    char buff[1024];
    if (!fgets ( buff, 1024, fh ))
        return false;
    fclose(fh);    
    char *lastBr = strrchr(buff, ')'); // closing bracer of the process name
        // it is 2nd arg
    if (lastBr == 0) return false;
    if (!strtok(lastBr, " ")) // )
        return false;
    if (!strtok(0, " ")) // 3rd value: status
        return false;
    char *sppid = strtok(0, " "); // 4th value: parent pid
    if (!sppid) return false;
    int ppid = atoi(sppid);
    if (ppid != 0)
    {
        PInfo *p = new PInfo(ppid);
        if (!p->init(connections))
            return false;
        if (!p->checkNetInfo(connections))
            return false;
        parent = p;
        
    }
    return true;
}


bool PInfo::checkNetInfo(std::vector<std::string>& connections)
{
    char dname [1024];
    sprintf(dname, "/proc/%d/fd", pid);
    DIR *dr;
    struct dirent *de;
    dr = opendir(dname);
    if (dr)
    {
        while((de = readdir(dr)) != NULL)
        {
            char buffer[1024];
            memset(buffer, 0, 1024);
            sprintf(dname, "/proc/%d/fd/%s", pid, de->d_name);
            readlink(dname, buffer, 1024);
            if (!strncmp(buffer, "socket", 6))
            {
                strtok(buffer, "[]");
                char *ptr = strtok(0, "[]"); // socket uid
                ProcNetTcpRecord *rec = ProcUtils::get()->getTcpRecordBySocket(ptr);
                if (rec)
                {
                    if (strcmp(rec->st, "01")) // status connected only
                        continue;
                    int ip[4];
                    sscanf(rec->rem_address, "%02X", &ip[3]);
                    sscanf(rec->rem_address+2, "%02X", &ip[2]);
                    sscanf(rec->rem_address+4, "%02X", &ip[1]);
                    sscanf(rec->rem_address+6, "%02X", &ip[0]);
                    char sip[20];
                    sprintf(sip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
                    connections.push_back(sip);
                }
            }
        }
    }
    return true;
}
