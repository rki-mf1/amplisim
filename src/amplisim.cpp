// Purpose: Main file for the amplisim program.

#include "Primer.h"
#include "PrimerIndex.h"
#include "AmpliconGenerator.h"
#include "util.h"
#include "argparser.h"



int main(int argc, char *argv[]){

    struct arguments arguments;
    arguments.output_file = NULL;
    arguments.seed = -1;
    
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // if seed unset, use time(NULL)
    if (arguments.seed == -1){
        arguments.seed = time(NULL);
    }
    srand( (unsigned) arguments.seed );

    std::string ref_genome = arguments.args[0];
    std::string bed_file   = arguments.args[1];

    // print a message to the user
    std::cout << "Running amplisim..." << std::endl;
    std::cout << "=== Arguments =====" << std::endl;
    std::cout << "Reference genome: " << ref_genome << std::endl;
    std::cout << "Primer BED file : " << bed_file   << std::endl;
    std::cout << "Seed            : " << arguments.seed << std::endl;
    if (arguments.output_file == NULL){
        std::cout << "Output file     : amplicons.fa" << std::endl;
    }
    else{
        std::cout << "Output file     : " << arguments.output_file << std::endl;
    }
    std::cout << "===================" << std::endl;

    // create an unordered map to store the names (key) and sequences (value) of the chromosomes
    std::unordered_map<std::string, std::string> chromosomes;

    int ret = read_reference(ref_genome, chromosomes);
    if (ret != 0){
        std::cerr << "Error reading the reference FASTA file." << std::endl;
        return 1;
    }

    // define a vector of Primer objects
    std::vector<Primer> primers;

    ret = read_primer(bed_file, primers);
    if (ret != 0){
        std::cerr << "Error reading the primer BED file." << std::endl;
        return 1;
    }

    // create a PrimerIndex object
    PrimerIndex primer_index(primers);

    // [COUT] for all chromosomes, print get_index and get_runlength from the PrimerIndex object (tab separated)
    for (auto const &chr : chromosomes){
        std::cout << chr.first << "\t" << primer_index.get_index(chr.first) << "\t" << primer_index.get_runlength(chr.first) << std::endl;
    }

    // create an empty vector of strings to store the amplicons
    std::vector<std::string> amplicons;

    // create an amplicons
    AmpliconGenerator amplicon_generator(primers, chromosomes, primer_index);
    ret = amplicon_generator.generate_amplicons(amplicons);
    if (ret != 0){
        std::cerr << "Error generating the amplicons." << std::endl;
        return 1;
    }

    // vec_reps is a vector of ints that stores the number of amplifications for each amplicon template sequence
    std::vector<int> vec_reps = amplicon_generator.get_vec_reps();
    assert(vec_reps.size() > 0);
    assert(std::accumulate(vec_reps.begin(), vec_reps.end(), 0) == amplicons.size());

    // use the write_amplicons function to write the amplicons to a file
    ret = write_amplicons("amplicons.fa", amplicons, vec_reps);
    

    return 0;
}