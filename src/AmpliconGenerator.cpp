#include "AmpliconGenerator.h"


/**
 * @brief Construct a new AmpliconGenerator::AmpliconGenerator object
 * 
 * @param primers A vector of Primer objects.
 * @param chromosomes An unordered map of chromosome names (key) and sequences (value).
 * @param primer_index A PrimerIndex object.
 */
AmpliconGenerator::AmpliconGenerator(std::vector<Primer> &primers, std::unordered_map<std::string, std::string> &chromosomes, PrimerIndex &primer_index){
    this->primers = &primers;
    this->chromosomes = &chromosomes;
    this->primer_index = &primer_index;
}


/**
 * @brief Generate amplicons from a set of primers.
 * 
 * @param amplicons A vector of strings to store the amplicons.
 * @param verbose A boolean to print messages to the user.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
int AmpliconGenerator::generate_amplicons(std::vector<std::string> &amplicons, arguments &arguments){

    assert(amplicons.empty());

    // print a message to the user
    if (arguments.verbose){
        std::cout << "Generating amplicons..." << std::endl;
    }

    // iterate over the chromoseomes
    for(auto &chromosome : *this->chromosomes){

        // for the chromosome name give me the index from the primer index
        int index = this->primer_index->get_index(chromosome.first);

        // continue if the index is -1 (the chromosome name is not in the primer index)
        if (index == -1){
            continue;
        }

        // for the chromosome name give me the runlength from the primer index
        int runlength = this->primer_index->get_runlength(chromosome.first);

#ifdef DEBUG
        assert(runlength > 0);
#endif

        // iterate over the vector of primers from the index to the index plus the runlength
        for (int i = index; i < index + runlength; i++){

            // amplicon dropout chance here
            double p_dropout = (double) rand() / RAND_MAX; // [0,1]
            if (p_dropout < arguments.dropout) continue;

            // get the start and end position of the left primer
            int left_start = this->primers->at(i).start_left;
            int left_end = this->primers->at(i).end_left;

#ifdef DEBUG
            assert(left_start < left_end);
            assert(left_end < this->chromosomes->at(chromosome.first).length());
            assert(left_start > 0);
#endif

            // get the sequence of the left primer
            std::string left_primer = this->chromosomes->at(chromosome.first).substr(left_start, left_end - left_start);

            // get the start and end position of the right primer
            int right_start = this->primers->at(i).start_right;
            int right_end = this->primers->at(i).end_right;

#ifdef DEBUG
            assert(right_start < right_end);
            assert(right_end < this->chromosomes->at(chromosome.first).length());
            assert(right_start > 0);
#endif

            // get the sequence of the right primer
            std::string right_primer = this->chromosomes->at(chromosome.first).substr(right_start, right_end - right_start);

            // get the insert sequence of the amplicon
            // which is from the end of the left primer to the start of the right primer
            std::string insert = this->chromosomes->at(chromosome.first).substr(left_end, right_start - left_end);

            // create the amplicon
            // generate insert sequences with errors
            Replicator replicator(0.01);
            std::vector<std::string> replicates;
            int reps;
            int ret = replicator.replicate_with_errors(insert, replicates, arguments.mean, arguments.sd, reps);
            if (ret != 0){
                std::cerr << "Error replicating the insert sequence." << std::endl;
                return 1;
            }

            this->vec_reps.push_back(reps);

            // generate amplification products from the replicates
            for (auto &replicate : replicates){
                std::string amplicon = left_primer + replicate + right_primer;
                amplicons.push_back(amplicon);
            }

        }

    }

    // print a warning if the vector of amplicons is empty and return 1
    if (amplicons.empty()){
        std::cout << "WARNING: No amplicons were generated." << std::endl;
        return 1;
    }

    return 0;
}


/**
 * @brief Get the vector of replications.
 * 
 * @return std::vector<int> A vector of integers.
 */
std::vector<int> AmpliconGenerator::get_vec_reps(){
    return this->vec_reps;
}

