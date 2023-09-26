#ifndef AMPLICON_GENERATOR_H
#define AMPLICON_GENERATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "argparser.h"
#include "Primer.h"
#include "PrimerIndex.h"
#include "Replicator.h"



/**
 * @brief Class to generate amplicons from a set of primers.
 * 
 */
class AmpliconGenerator{
    private:
        std::vector<int> vec_reps;  // RAII (Resource Acquisition Is Initialization)
        std::vector<Primer> *primers;
        std::unordered_map<std::string, std::string> *chromosomes;
        PrimerIndex *primer_index;
    public:
        AmpliconGenerator(std::vector<Primer> &primers, std::unordered_map<std::string, std::string> &chromosomes, PrimerIndex &primer_index);
        int generate_amplicons(std::vector<std::string> &amplicons, arguments &arguments);
        std::vector<int> get_vec_reps();
};









#endif // AMPLICON_GENERATOR_H