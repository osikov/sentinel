#include "PInfo.hpp"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


std::vector<std::string> knownIps;

void loadKnownIps()
{
    FILE *fh = fopen("/etc/sentinel.cfg","r");
    if (fh)
    {
        char buffer [1024];
        while (fgets(buffer, 1024, fh))
        {
            knownIps.push_back(strtok(buffer,"\r\n"));
        }
        fclose(fh);
    }
}

void __attribute__ ((constructor))  init()
{
    // run for bash only
    char exeLink[1024];
    sprintf(exeLink, "/proc/%d/exe", getpid());
    char exeName[1024];
    memset(exeName, 0, 1024);
    readlink(exeLink, exeName, 1024);
    if (strcmp(exeName, "/bin/bash") &&
        strcmp(exeName, "/bin/dash"))
            return;

    char timeAsString[1024];
    time_t curtime; 
    time(&curtime); 
    strcpy(timeAsString, ctime(&curtime));
    strtok(timeAsString, "\r\n");

    loadKnownIps();
    
    std::vector<std::string> connections;
    PInfo *p = new PInfo(getpid());
    p->init(connections);
    for(int i = 0; i < connections.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < knownIps.size() && !found; j++)
        {
            if (!connections.at(i).compare(knownIps.at(j)))
                found = true;
        }
        if (!found)
        {
            FILE *fh = fopen("/tmp/.sentinel.log", "a");
            if (fh)
            {
                fprintf(fh, "%s %s\n", timeAsString, connections.at(i).c_str());
            }
            fclose(fh);
        }
    }
}

