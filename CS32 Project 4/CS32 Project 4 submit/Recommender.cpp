#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "treemm.h"
#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

bool compare(const MovieAndRank_& a, const MovieAndRank_& b)
{
    if (a.movie.compatibility_score != b.movie.compatibility_score)
        return a.movie.compatibility_score < b.movie.compatibility_score;
    else
    {
        if (a.rating != b.rating)
            return a.rating < b.rating;
        else
            return a.title > b.title;
    }
    return false;
}

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_user_database = &user_database;
    m_movie_database = &movie_database;
}
void Recommender::getMovie_helper(int type, vector<Movie*>& Tmp, string key) const
{
    if (type == 1)
    {
        Tmp = m_movie_database->get_movies_with_director(key);
        return;
    }
    else if (type == 2)
    {
        Tmp = m_movie_database->get_movies_with_actor(key);
        return;
    }
    else if (type==3)
    {
        Tmp = m_movie_database->get_movies_with_genre(key);
        return;
    }
    return;
}
void Recommender::recommend_helper(int type, int num, vector<string>& tmp, set<string>& container,
    TreeMultimap <string, string>& watching_history, vector<MovieAndRank_>& candidate) const
{
    int base;
    switch (type)
    {
    case 1:
        base = 20;
        break;
    case 2:
        base = 30;
        break;
    case 3:
        base = 1;
        break;
    default:
        base = 0;
    }
    
    vector<Movie*> Tmp;
    for (int i = 0; i < tmp.size(); i++)
    {
        if (container.find(tmp[i]) != container.end())
        {
            continue;
        }
        else
        {
            int num = 0;
            container.insert(tmp[i]);
            getMovie_helper(type, Tmp, tmp[i]);
            for (int j = 0; j < Tmp.size(); j++)
            {
                string Id = Tmp[j]->get_id();
                auto It = watching_history.find(Id);
                if (It.is_valid())
                    num++;
            }
            for (int j = 0; j < Tmp.size(); j++)
            {
                string Id = Tmp[j]->get_id();
                auto It = watching_history.find(Id);
                if (!It.is_valid())
                {
                    bool check = false;
                    for (int k = 0; k < candidate.size(); k++)
                    {
                        if (candidate[k].movie.movie_id == Id)
                        {
                            candidate[k].movie.compatibility_score += num * base;
                            check = true;
                            break;
                        }
                    }
                    if (!check)
                    {
                        MovieAndRank_ a(Id, num * base, Tmp[j]->get_title(), Tmp[j]->get_rating());
                        candidate.push_back(a);
                    }
                }
            }
        }
    }
}
vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    if (movie_count <= 0)
        return vector<MovieAndRank>();
    User* u = m_user_database->get_user_from_email(user_email);
    if (u == nullptr)
        return vector<MovieAndRank>();
    Movie* m;
    string id;
    set<string> directors;
    set<string> actors;
    set<string> genres;
    vector<string> tmp;
    vector<Movie*> Tmp;
    vector<MovieAndRank> result;
    vector<MovieAndRank_> candidate;
    TreeMultimap <string, string> watching_history;
    for (int l = 0; l < u->get_watch_history().size(); l++)
        watching_history.insert(u->get_watch_history()[l], u->get_watch_history()[l]);
    for (int l = 0; l < u->get_watch_history().size(); l++)
    {
        id.clear();
        id = u->get_watch_history()[l];
        m = m_movie_database->get_movie_from_id(id);
        tmp = m->get_directors();
        recommend_helper(1, 20, tmp, directors, watching_history, candidate);
        tmp.clear();
        tmp = m->get_actors();
        recommend_helper(2, 30, tmp, actors, watching_history, candidate);
        tmp.clear();
        tmp = m->get_genres();
        recommend_helper(3, 1, tmp, genres, watching_history, candidate);
        tmp.clear();
    }
    sort(candidate.begin(), candidate.end(), compare);
    int i = 0;
    while (!candidate.empty())
    {
        if (i == movie_count)
            break;
        result.push_back(candidate.back().movie);
        candidate.pop_back();
        i++;
    }
    return result;
}