#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

MovieDatabase::MovieDatabase()
{}

bool MovieDatabase::load(const string& filename)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open())
        return false;
    std::string key1;
    std::string key2;
    std::string key3;
    std::string key4;
    std::string line;
    std::string id;
    std::string title;
    std::string release_year;
    std::string directors;
    std::string actors;
    std::string genres;
    std::string Rating;
    std::vector<std::string> director;
    std::vector<std::string> actor;
    std::vector<std::string> genre;
    float rating;
    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            convert_helper(director, directors);
            convert_helper(actor, actors);
            convert_helper(genre, genres);
            rating = std::stof(Rating);
            Movie* m = new Movie(id,title,release_year,director,actor,genre,rating);
            key1 = toLower(id);
            m_database.insert(key1, m);
            for (int i = 0; i < director.size(); i++)
            {
                key2 = toLower(director[i]);
                m_director_database.insert(key2, m);
            }
            for (int i = 0; i < actor.size(); i++)
            {
                key3 = toLower(actor[i]);
                m_actor_database.insert(key3, m);
            }
               
            for (int i = 0; i < genre.size(); i++)
            {
                key4 = toLower(genre[i]);
                m_genre_database.insert(key4, m);
            }
            id.clear();
            title.clear();
            release_year.clear();
            directors.clear();
            actors.clear();
            genres.clear();
            Rating.clear();
        }
        else if (id.empty())
            id = line;
        else if (title.empty())
            title = line;
        else if (release_year.empty())
            release_year = line;
        else if (directors.empty())
            directors = line;
        else if (actors.empty())
            actors = line;
        else if (genres.empty())
            genres = line;
        else
            Rating = line;
    }
    if (!id.empty() && !title.empty() && !release_year.empty() && !directors.empty() && !actors.empty() && !genres.empty() && !Rating.empty())
    {
        convert_helper(director, directors);
        convert_helper(actor, actors);
        convert_helper(genre, genres);
        rating = std::stof(Rating);
        Movie* m = new Movie(id, title, release_year, director, actor, genre, rating);
        key1 = toLower(id);
        m_database.insert(key1, m);
        for (int i = 0; i < director.size(); i++)
        {
            key2 = toLower(director[i]);
            m_director_database.insert(key2, m);
        }
        for (int i = 0; i < actor.size(); i++)
        {
            key3 = toLower(actor[i]);
            m_actor_database.insert(key3, m);
        }

        for (int i = 0; i < genre.size(); i++)
        {
            key4 = toLower(genre[i]);
            m_genre_database.insert(key4, m);
        }
        id.clear();
        title.clear();
        release_year.clear();
        directors.clear();
        actors.clear();
        genres.clear();
        Rating.clear();
    }
    input_file.close();
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string Id = toLower(id);
    auto it= m_database.find(Id);
    if (!it.is_valid())
        return nullptr;
    return it.get_value(); 
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string Director = toLower(director);
    auto it = m_director_database.find(Director);
    vector<Movie*> tmp;
    tmp.clear();
    while (it.is_valid())
    {
        tmp.push_back(it.get_value());
        it.advance();
    }
    return tmp;  
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string Actor = toLower(actor);
    auto it = m_actor_database.find(Actor);
    vector<Movie*> tmp;
    tmp.clear();
    while (it.is_valid())
    {
        tmp.push_back(it.get_value());
        it.advance();
    }
    return tmp;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string Genre = toLower(genre);
    auto it = m_genre_database.find(Genre);
    vector<Movie*> tmp;
    tmp.clear();
    while (it.is_valid())
    {
        tmp.push_back(it.get_value());
        it.advance();
    }
    return tmp;
}