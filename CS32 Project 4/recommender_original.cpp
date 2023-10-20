/* for (int i = 0; i < tmp.size(); i++)
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
       }*/
       /*for (int i = 0; i < tmp.size(); i++)
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
              }*/
              /*for (int i = 0; i < tmp.size(); i++)
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
                   }*/