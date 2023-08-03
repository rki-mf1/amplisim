#ifndef AMPLICON_GENERATOR_H
#define AMPLICON_GENERATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "Primer.h"
#include "PrimerIndex.h"



/**
 * @brief Class to generate amplicons from a set of primers.
 * 
 */
class AmpliconGenerator{
    private:
        std::vector<Primer> *primers;
        std::unordered_map<std::string, std::string> *chromosomes;
        PrimerIndex *primer_index;
    public:
        AmpliconGenerator(std::vector<Primer> &primers, std::unordered_map<std::string, std::string> &chromosomes, PrimerIndex &primer_index);
        int generate_amplicons(std::vector<std::string> &amplicons);
};









#endif // AMPLICON_GENERATOR_H