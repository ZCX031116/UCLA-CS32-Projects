#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include<iostream>
using namespace std;

bool compare(const MovieAndRank& a, const MovieAndRank& b)
{
    if (a.compatibility_score != b.compatibility_score)
        return a.compatibility_score < b.compatibility_score;
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
    vector<MovieAndRank> candidate;
    TreeMultimap <string, string> watching_history;
    for (int l = 0; l < u->get_watch_history().size(); l++)
    {
        watching_history.insert(u->get_watch_history()[l], u->get_watch_history()[l]);
    }
    for (int l = 0; l < u->get_watch_history().size(); l++)
    {
        id.clear();
        id = u->get_watch_history()[l];
        m = m_movie_database->get_movie_from_id(id);
        tmp = m->get_directors();
        for (int i = 0; i < tmp.size(); i++)
        {
            if (directors.find(tmp[i]) != directors.end())
            {
                continue;
            }
            else
            {
                int num = 0;
                directors.insert(tmp[i]);
                Tmp = m_movie_database->get_movies_with_director(tmp[i]);
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
                            if (candidate[k].movie_id == Id)
                            {
                                candidate[k].compatibility_score += num * 20;
                                //cout << candidate[k].title << " add " << num * 20 << endl << "current score: " << candidate[k].compatibility_score << endl << k << endl;
                                check = true;
                                break;
                            }
                        }
                        if (!check)
                        {
                            MovieAndRank a(Id, num * 20, Tmp[j]->get_title(), Tmp[j]->get_rating());
                            candidate.push_back(a);
                            //cout << a.title << " created and add " << num * 20 << endl << "current score: " << num * 20 << endl << k << endl;
                        }
                    }
                }
            }
        }
        tmp.clear();
        tmp = m->get_actors();
        for (int i = 0; i < tmp.size(); i++)
        {
            Tmp = m_movie_database->get_movies_with_actor(tmp[i]);
            if (actors.find(tmp[i]) != actors.end())
            {
                //cout << "actor exist " << tmp[i] << endl;
                continue;
            }
            else
            {
                actors.insert(tmp[i]);
                int num = 0;
                Tmp = m_movie_database->get_movies_with_actor(tmp[i]);
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
                            if (candidate[k].movie_id == Id)
                            {
                                candidate[k].compatibility_score += num * 30;
                                //cout << candidate[k].title << " add " << num * 30 << endl << "current score: " << candidate[k].compatibility_score << endl << k << endl;
                                check = true;
                                break;
                            }
                        }
                        if (!check)
                        {
                            MovieAndRank a(Id, num * 30, Tmp[j]->get_title(), Tmp[j]->get_rating());
                            candidate.push_back(a);
                            //cout << a.title << " created and add " << num * 30 << endl << "current score: " << num * 30 << endl << k << endl;
                        }
                    }
                }
            }
        }
        tmp.clear();
        tmp = m->get_genres();
        for (int i = 0; i < tmp.size(); i++)
        {
            Tmp = m_movie_database->get_movies_with_genre(tmp[i]);
            if (genres.find(tmp[i]) != genres.end())
            {
                continue;
                //cout << "genre exist " << tmp[i] << endl;
            }
            else
            {
                genres.insert(tmp[i]);
                int num = 0;
                Tmp = m_movie_database->get_movies_with_genre(tmp[i]);
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
                            if (candidate[k].movie_id == Id)
                            {
                                candidate[k].compatibility_score += num * 1;
                                //cout << candidate[k].title << " add " << num * 1 << "current score: " << endl << candidate[k].compatibility_score << endl << k << endl;
                                check = true;
                                break;
                            }
                        }
                        if (!check)
                        {
                            MovieAndRank a(Id, num * 1, Tmp[j]->get_title(), Tmp[j]->get_rating());
                            candidate.push_back(a);
                            //cout << a.title << " created and add " << num * 1 << endl << "current score: " << num * 1 << endl << k << endl;
                        }
                    }
                }
            }
        }
        tmp.clear();
    }
    sort(candidate.begin(), candidate.end(), compare);
    int i = 0;
    while (!candidate.empty())
    {
        if (i == movie_count)
            break;
        result.push_back(candidate.back());
        candidate.pop_back();
        i++;
    }
    return result;
}