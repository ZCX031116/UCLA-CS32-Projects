#include "Movie.h"
#include "User.h"
#include "treemm.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Recommender.h"
#include <iostream>
#include <string>
using namespace std;

const string USER_DATAFILE = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations) 
{
	vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
	
	if (recommendations.empty())
		cout << "We found no movies to recommend :(.\n";
	else 
	{
		for (int i = 0; i < recommendations.size(); i++) 
		{
			const MovieAndRank& mr = recommendations[i];
			Movie* m = md.get_movie_from_id(mr.movie_id);
			cout << i+1 << ". " << m->get_title() << " (" << m->get_release_year() << ")\n   Rating: " 
				<< m->get_rating() << "\n   Compatibility Score: " << mr.compatibility_score << "\n"<<"\n";
		}
	}
}
int main()
{
	UserDatabase udb;
	udb.load(USER_DATAFILE);
	MovieDatabase mdb;
	mdb.load(MOVIE_DATAFILE);
	Recommender r(udb, mdb);
	string email;
	int num;
	cout << "Enter a user's email for recommendations: ";
	cin >> email;
	cout << "Enter number of recommendations to provide: ";
	cin >> num;
	findMatches(r, mdb, email, num);
}