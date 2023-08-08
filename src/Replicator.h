#ifndef REPLICATOR_H
#define REPLICATOR_H

#include <string>
#include <vector>
#include <random>
#include <ctime>

#include <iostream> // can be deleted later



class Replicator {


    private:
        double error_rate;
        char mutate(char base);

    public:
        Replicator(double error_rate);
        int replicate_with_errors(std::string &insert,
                                  std::vector<std::string> &replicates,
                                  int nb_replications,
                                  int nb_replications_variance,
                                  int &reps);
};







#endif // REPLICATOR_H