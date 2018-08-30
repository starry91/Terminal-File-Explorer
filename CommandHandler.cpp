#include "CommandHandler.h"
#include <sys/stat.h>
#include <syslog.h>
#include <fstream>
#include "file.h"

void setFilePerms(std::string file, mode_t perms)
{
    chmod(file.c_str(), perms);   
}

void CommandHandler::copyFile(std::string file, std::string dir)
{

    // FILE *from, *to;
    // char ch;
    std::string dest_file = dir + "/" + File(file).getFileName();
    syslog(0, "FileName: %s", File(file).getFileName().c_str());
    syslog(0, "Source File: %s",file.c_str());
    syslog(0, "Dest File: %s",dest_file.c_str());
    std::ifstream input(file, std::ios::binary );
    std::ofstream output(dest_file, std::ios::binary);
    std::copy( 
        std::istreambuf_iterator<char>(input), 
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(output));
    mode_t perms = File(file).getFilePerms();
    setFilePerms(dest_file, perms);
};


void CommandHandler::copyFiles(const std::vector<std::string>& files) {
    std::string dest = files[files.size()-1];
    for(int i = 1; i < files.size()-1; i++)
        copyFile(files[i], dest);
};