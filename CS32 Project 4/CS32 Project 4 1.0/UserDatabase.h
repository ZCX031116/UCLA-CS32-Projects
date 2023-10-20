#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED
#include <fstream>
#include <string>
#include "User.h"
#include "treemm.h"

class User;
class UserDatabase
{
public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

private:
    TreeMultimap <std::string, User*> m_database; 
    bool loaded;
};

#endif