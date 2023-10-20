#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include "Movie.h"
#include "treemm.h"
class Movie;

class MovieDatabase
{
private:
    std::string toLower(std::string s) const
    {
        std::string tmp = "";
        for (int i = 0; i < s.size(); i++)
        {
            char c = s[i];
            if (std::isalpha(c))
                tmp += std::tolower(c);
            else
                tmp += c;
        }
        return tmp;
    }
    void convert_helper(std::vector<std::string>& v, std::string s)
    {
        std::string a;
        a.clear();
        v.clear();
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ',')
            {
                v.push_back(a);
                a.clear();
            }
            else
                a += s[i];
        }
        if (!a.empty())
            v.push_back(a);
    }
public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

private:
    TreeMultimap <std::string, Movie*> m_database;
    TreeMultimap <std::string, Movie*> m_director_database;
    TreeMultimap <std::string, Movie*> m_actor_database;
    TreeMultimap <std::string, Movie*> m_genre_database;    
};

#endif // MOVIEDATABASE_INCLUDED
