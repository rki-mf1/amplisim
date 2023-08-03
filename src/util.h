#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>


/**
 * @brief Reverse complement a sequence.
 * 
 * @param seq The sequence to reverse complement.
 * @return std::string The reverse complement of the sequence.
 */
static std::string reverse_complement(std::string seq){

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



#endif // UTIL_H