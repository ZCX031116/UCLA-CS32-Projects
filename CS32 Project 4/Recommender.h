#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <set>
#include "Movie.h"
#include "treemm.h"
using namespace std;
class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
        : movie_id(id), compatibility_score(score)
    {}
    std::string movie_id;
    int compatibility_score;
};
struct MovieAndRank_
{
    MovieAndRank_(const std::string& id, int score, const std::string& t, float r)
        : movie(id,score), title(t), rating(r)
    {}
    MovieAndRank movie;
    std::string title;
    float rating;
};
class Recommender
{
public:
    Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email, int movie_count) const;

private:
    const UserDatabase* m_user_database;
    const MovieDatabase* m_movie_database;
    void recommend_helper(int type, int num, vector<string>& tmp, set<string>& container,
        TreeMultimap <string, string>& watching_history, vector<MovieAndRank_>& candidate) const;
    void getMovie_helper(int type, vector<Movie*>& Tmp, string key) const;
};
#endif // RECOMMENDER_INCLUDED