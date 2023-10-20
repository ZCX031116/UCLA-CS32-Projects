#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <set>
class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score, const std::string& t, float r)
        : movie_id(id), compatibility_score(score),title(t),rating(r)
    {}
    std::string movie_id;
    int compatibility_score;
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
};
#endif // RECOMMENDER_INCLUDED