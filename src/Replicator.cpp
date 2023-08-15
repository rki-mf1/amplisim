#include "Replicator.h"



/**
 * @brief Construct a new Replicator:: Replicator object
 * 
 * @param error_rate the error rate
 */
Replicator::Replicator(double error_rate) : error_rate(error_rate) {}


/**
 * @brief mutate a base
 * 
 * @param base the base to mutate
 * @return char the mutated base unequal to base
 */
char Replicator::mutate(const char base){

    // vector of possible bases
    std::vector<char> bases = {'A', 'C', 'G', 'T'};

    // remove the base from the vector of possible bases
    bases.erase(std::remove(bases.begin(), bases.end(), base), bases.end());

    // assuming std::random::srand() was called before
    
    // return a random base from the vector of possible bases
    return bases[rand() % bases.size()];
}


/**
 * @brief replicate a DNA sequence with errors
 * 
 * @param insert the DNA sequence to replicate
 * @param replicates a vector of strings to store the replicates
 * @param nb_replications the number of replications
 * @param nb_replications_variance the variance of the number of replications
 * @param reps in integer to store the number of replications for that specific insert
 * @return int 0 if the function was executed correctly, 1 otherwise
 */
int Replicator::replicate_with_errors(std::string &insert,
                                      std::vector<std::string> &replicates,
                                      int nb_replications,
                                      int nb_replications_variance,
                                      int &reps){

    // sanity check: the length of the insert must be at least 1
    if (insert.size() < 1) {
        std::cerr << "Error: the length of the insert must be at least 1." << std::endl;
        return 1;
    }

    // sanity check: the vector of replicates must be empty
    if (!replicates.empty()) {
        std::cerr << "Error: the vector of replicates must be empty." << std::endl;
        return 1;
    }

    // get a random integer from a normal distribution
    const unsigned int seed = rand() % UINT_MAX;
    std::mt19937 gen(seed);     /* Mersenne Twister pseudo-random generator */
    std::normal_distribution<> d(nb_replications, nb_replications_variance);
    int nb_replications_random = d(gen);

    // fallback: the number of replications must be at least 1
    if (nb_replications_random < 1) {
        nb_replications_random = 1;
    }

#ifdef DEBUG
    std::cout << "Number of replications: " << nb_replications_random+1 << std::endl;       // +1 to account for the original sequence
#endif
    reps = nb_replications_random;

    // insert the original sequence into the vector of replicates
    replicates.push_back(insert);

    // generate replicates
    for (int n = 0; n < nb_replications_random; ++n) {

        /* 
        Select a random insert as a template for further replications.
        This is to better reflect actual PCR, where the most abundant
        amplicon is the most likely to be replicated.
        */
        std::string rand_insert = replicates[rand() % replicates.size()];
        int len_rand_insert = rand_insert.size();

        std::string replicate = "";

        for (int i = 0; i < len_rand_insert; ++i) {
                
            double rand_num = (double) rand() / RAND_MAX;
            
            if (rand_num > this->error_rate) {
                replicate += rand_insert[i];
                continue;
            }

            rand_num = (double) rand() / RAND_MAX;
            
            if (rand_num < 0.8) {           // substitution, add random base
                replicate += this->mutate(rand_insert[i]);
            } else if (rand_num < 0.9) {    // insertion, add original base + random base
                replicate += rand_insert[i];
                replicate += "ACGT"[rand() % 4];
            } else {   // deletion, skip original base
                continue;
            }

        }

        replicates.push_back(replicate);
    }
    
    // we increase the number of replications by 1 to account for the original sequence
    reps++;

    return 0;
}