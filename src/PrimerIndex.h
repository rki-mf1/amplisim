#ifndef PRIMERINDEX_H
#define PRIMERINDEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cassert>

#include "Primer.h"


// define a class to build an index for the primers
// the class is called PrimerIndex
// the class has three private members: a pointer to a vector of Primer objects, an unordered map of indices, and an unordered map of runlengths
// the class has one public constructor, one public method to get a primer's index, and one public method to get a primer's runlength
class PrimerIndex{
    private:
        std::vector<Primer> *primers;
        std::unordered_map<std::string, int> indices;
        std::unordered_map<std::string, int> runlengths;
    public:
        PrimerIndex(std::vector<Primer> &primers);
        int get_index(std::string chr);
        int get_runlength(std::string chr);
    
    private:
        int create_primer_index();
};






#endif // PRIMERINDEX_H