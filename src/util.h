#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Primer.h"



/**
 * @brief Reverse complement a sequence.
 * 
 * @param seq The sequence to reverse complement.
 * @return std::string The reverse complement of the sequence.
 */
std::string reverse_complement(std::string seq){

    // create a string to store the reverse complement
    std::string rev_comp;

    // loop over the sequence in reverse order
    for (int i = seq.size() - 1; i >= 0; i--){

        // check if the current character is a nucleotide
        if (seq[i] == 'A' || seq[i] == 'a'){
            rev_comp += 'T';
        } else if (seq[i] == 'T' || seq[i] == 't'){
            rev_comp += 'A';
        } else if (seq[i] == 'C' || seq[i] == 'c'){
            rev_comp += 'G';
        } else if (seq[i] == 'G' || seq[i] == 'g'){
            rev_comp += 'C';
        } else {
            // print a warning message to the user if the current character is not a nucleotide
            std::cerr << "Warning: the sequence contains non-nucleotide characters." << std::endl;
            // if the current character is not a nucleotide, return an empty string
            return "N";
        }
    }

    return rev_comp;
}


/**
 * @brief Print the amplicons to the standard output.
 * 
 * @param primers A vector of Primer objects.
 * @param chromosomes An unordered map to store the names (key) and sequences (value) of the chromosomes.
 * @return void
 */
void print_amplicons(std::vector<Primer> &primers, std::unordered_map<std::string, std::string> &chromosomes){

    // loop over the primers
    for (auto p : primers){

        // check if the primer's chromosome name is in the chromosomes map
        if (chromosomes.find(p.chr) == chromosomes.end()){
            // print a warning message to the user if the chromosome name is not in the chromosomes map
            std::cerr << "Warning: the chromosome name " << p.chr << " is not in the reference FASTA file." << std::endl;
            // skip the current primer
            continue;
        }

        // print the chromosome name
        std::cout << p.chr << "\t";

        // print the start and end position of the left primer
        std::cout << p.start_left << "\t" << p.end_left << "\t";

        // print the sequence of the left primer
        std::cout << chromosomes[p.chr].substr(p.start_left, p.end_left - p.start_left) << std::endl;

        // print the chromosome name
        std::cout << p.chr << "\t";

        // print the start and end position of the right primer
        std::cout << p.start_right << "\t" << p.end_right << "\t";

        // print the reverse complement of the right primer
        std::cout << reverse_complement(chromosomes[p.chr].substr(p.start_right, p.end_right - p.start_right)) << std::endl;

    }

}





#endif // UTIL_H