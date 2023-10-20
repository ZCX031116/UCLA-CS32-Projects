#include "User.h"
#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email,
    const vector<string>& watch_history)
{
    m_full_name = full_name;
    m_email = email;
    for (auto it=watch_history.begin();it!=watch_history.end();it++)
        m_watch_history.push_back(*it);
}

string User::get_full_name() const
{
    return m_full_name;
}

string User::get_email() const
{
    return m_email;
}

vector<string> User::get_watch_history() const
{
    return m_watch_history;
}