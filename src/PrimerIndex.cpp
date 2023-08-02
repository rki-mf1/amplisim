#include "PrimerIndex.h"



/**
 * @brief Constructor of the PrimerIndex class.
 * 
 * @param primers A pointer to a vector of Primer objects.
 */
PrimerIndex::PrimerIndex(std::vector<Primer> &primers){
    this->primers = &primers;

    // check that the pointer points to valid memory
    assert(this->primers != nullptr);

    // create the index
    this->create_primer_index();
    }


/**
 * @brief Create an index for the primers.
 * 
 * @details The index is stored in two unordered maps: one for the indices and one for the runlengths.
 *          The indices unordered map stores the index of the first primer of each chromosome.
 *          The runlengths unordered map stores the number of primers of each chromosome.
 *          The chromosomes must be sorted in contiguous blocks in the BED file.
 * @return int 0 if the index is created successfully, 1 otherwise.
 *          If the vector of primers is empty, the function returns 1.
 *          If a chromosome appears in multiple discontinuous blocks in the BED file, the function returns 1.
 */
int PrimerIndex::create_primer_index(){

    // check that the vector of primers is not empty, return 1 otherwise
    if (this->primers->empty()){
        std::cerr << "Error: the vector of primers is empty." << std::endl;
        return 1;
    }

    // print a message to the user
    std::cout << "Creating the primer index..." << std::endl;

    // store an unordered set of chromosomes
    std::unordered_set<std::string> unique_chromosomes;

    // define a string to store the current chromosome
    std::string chr = "";

    int previousChromIndex = 0;

    // loop over the primers
    for (int i = 0; i < this->primers->size(); i++){

        // get the chromosome of the current primer
        std::string current_chr = this->primers->at(i).chr;

        // check if the chromosome of the current primer is different from the chromosome of the previous primer
        if (current_chr != chr){

            // check if the current chromosome is not in the unordered set of chromosomes, else return 1
            if (unique_chromosomes.find(current_chr) != unique_chromosomes.end()){
                std::cerr << "Error: the chromosome \'" << current_chr << "\' appears in multiple discontinuous blocks in the BED file." << std::endl;
                return 1;
            }

            // add the current chromosome to the unordered set of chromosomes
            unique_chromosomes.insert(current_chr);

            // store the index of the current primer in the indices unordered map
            this->indices[current_chr] = i;

            // store the runlength of the previous primer in the runlengths unordered map
            this->runlengths[chr] = i - previousChromIndex;

            // store the index of the current primer in the previousChromIndex variable
            previousChromIndex = i;

            // store the chromosome of the current primer in the chr string
            chr = current_chr;
        }
    }

    // store the runlength of the last primer in the runlengths unordered map
    this->runlengths[chr] = this->primers->size() - previousChromIndex;

    // assert that the indices and runlengths unordered maps are not empty
    assert(!this->indices.empty());
    assert(!this->runlengths.empty());
    // assert that both unordered maps have the same size
    assert(this->indices.size() == this->runlengths.size());

    return 0;
}


/**
 * @brief Get the index for a given chromosome.
 * 
 * @param chr The chromosome.
 * @return int The index of the first primer of the given chromosome.
 *         If the chromosome is not in the index, the function returns -1.
 */
int PrimerIndex::get_index(std::string chr){

    // check if the chromosome is in the index, return -1 otherwise
    if (this->indices.find(chr) == this->indices.end()){
        return -1;
    }

    // return the index of the chromosome
    return this->indices[chr];
}


/**
 * @brief Get the runlength for a given chromosome.
 * 
 * @param chr The chromosome.
 * @return int The number of primers of the given chromosome.
 *         If the chromosome is not in the index, the function returns -1.
 */
int PrimerIndex::get_runlength(std::string chr){

    // check if the chromosome is in the index, return -1 otherwise
    if (this->runlengths.find(chr) == this->runlengths.end()){
        return -1;
    }

    // return the runlength of the chromosome
    return this->runlengths[chr];
}
