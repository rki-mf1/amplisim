// Purpose: Main file for the amplisim program.

#include "Primer.h"
#include "PrimerIndex.h"
#include "AmpliconGenerator.h"
#include "util.h"






int main(int argc, char const *argv[]){

    // check that the number of command line arguments is correct
    if (argc != 3){
        std::cerr << "Usage: amplisim <reference_genome.fa> <primers.bed>" << std::endl;
        return 1;
    }

    // print both input files to the user
    std::cout << "Reference genome: " << argv[1] << std::endl;
    std::cout << "Primer BED file: " << argv[2] << std::endl;

    // convert the reference genome file name to a string
    std::string ref_genome = argv[1];

    // create an unordered map to store the names (key) and sequences (value) of the chromosomes
    std::unordered_map<std::string, std::string> chromosomes;

    // call the read_reference function and catch the return value
    int ret = read_reference(ref_genome, chromosomes);
    // check if the function was executed correctly
    if (ret != 0){
        std::cerr << "Error reading the reference FASTA file." << std::endl;
        return 1;
    }

    // the second command line argument is the name of the BED file
    std::string bed_file = argv[2];

    // define a vector of Primer objects
    std::vector<Primer> primers;
    
    // call the read_primer function and catch the return value
    ret = read_primer(bed_file, primers);
    // check if the function was executed correctly
    if (ret != 0){
        std::cerr << "Error reading the primer BED file." << std::endl;
        return 1;
    }

    // create a PrimerIndex object
    PrimerIndex primer_index(primers);

    // for all chromosomes, print get_index and get_runlength from the PrimerIndex object (tab separated)
    for (auto const &chr : chromosomes){
        std::cout << chr.first << "\t" << primer_index.get_index(chr.first) << "\t" << primer_index.get_runlength(chr.first) << std::endl;
    }

    // create an empty vector of strings to store the amplicons
    std::vector<std::string> amplicons;

    // create an AmpliconGenerator object
    AmpliconGenerator amplicon_generator(primers, chromosomes, primer_index);

    // call the generate_amplicons function and catch the return value
    ret = amplicon_generator.generate_amplicons(amplicons);

    // check if the function was executed correctly
    if (ret != 0){
        std::cerr << "Error generating the amplicons." << std::endl;
        return 1;
    }

    // use the write_amplicons function to write the amplicons to a file
    ret = write_amplicons("amplicons.fa", amplicons);
    
    return 0;
}