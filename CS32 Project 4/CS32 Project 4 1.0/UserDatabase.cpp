#include "UserDatabase.h"
#include "User.h"
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
{
    loaded = false;
}

bool UserDatabase::load(const string& filename)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open())
        return false;
    if(loaded)
        return false;
    loaded = true;
    std::string line;
    std::string full_name;
    std::string email;
    std::string history_size;
    std::vector<std::string> watch_history;
    while (std::getline(input_file, line)) 
    {
        if (line.empty())
        {
            User* u = new User(full_name, email, watch_history);
            m_database.insert(email, u);
            full_name.clear();
            email.clear();
            watch_history.clear();
            history_size.clear();
        }
        else if (full_name.empty())
        {
            full_name = line;
        }
        else if (email.empty())
        {
            email = line;
        }
        else if (history_size.empty())
        {
            history_size = line;
        }
        else
        {
            watch_history.push_back(line);
        }
            
    }
    if (!full_name.empty() && !email.empty())
    {
        User* u= new User(full_name, email, watch_history);
        m_database.insert(email, u);
    }
    input_file.close();
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    auto it = m_database.find(email);
    if (it.is_valid())
        return it.get_value();
    return nullptr;
}