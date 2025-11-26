#ifndef TOURNAMENT_DRIVER_H
#define TOURNAMENT_DRIVER_H
using namespace std;
void testTournament(vector<string> mapFiles,
                    vector<string> playerStrategies,
                    int numGames,
                    int maxTurns);
bool argumentValidator(int argc, char *argv[],
                       vector<string> &mapFiles,
                       vector<string> &playerStrategies,
                       int &numGames,
                       int &maxTurns);
#endif