#ifndef PRIMERINDEX_H
#define PRIMERINDEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cassert>

#include "Primer.h"


/**
 * @brief A class to store an index of primers.
 * 
 */
class PrimerIndex{
    private:
        bool verbose;
        std::vector<Primer> *primers;
        std::unordered_map<std::string, int> indices;
        std::unordered_map<std::string, int> runlengths;
    public:
        PrimerIndex(std::vector<Primer> &primers, const bool verbose = false);
        int get_index(std::string chr);
        int get_runlength(std::string chr);
    
    private:
        int create_primer_index();
};






#endif // PRIMERINDEX_H