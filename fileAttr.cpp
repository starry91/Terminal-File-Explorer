#include "fileAttr.h"
#include <iomanip>

fileAttr::fileAttr(std::string perm, std::string usr_name,
                   std::string grp_name, long long size,
                   std::string last_modified, std::string name)
{
    this->permission = perm;
    this->usr_name = usr_name;
    this->grp_name = grp_name;
    this->size = size;
    this->last_modified = last_modified;
    this->file_type = (perm[0] == 'd') ? 'd' : 'f';
    if (this->file_type == 'd')
        this->name = "\033[35;10m" + name + "\033[0m";
    else
        this->name = name;
}

std::ostream &operator<<(std::ostream &os, const fileAttr &obj)
{
    os << obj.permission << " ";
    os << obj.usr_name << " ";
    os << obj.grp_name << " ";
    os << std::right << std::setw(6) << obj.size << " ";
    os << obj.last_modified << " ";
    os << obj.name << " ";
}
