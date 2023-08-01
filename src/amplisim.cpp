// Purpose: Main file for the amplisim program.
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <htslib/faidx.h>

#include "Primer.h"



/**
 * @brief Read a FASTA file and store the names and sequences in vectors.
 * 
 * @param fasta_file The name of the FASTA file.
 * @param chromosomes An unordered map to store the names (key) and sequences (value) of the chromosomes.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
int read_fasta(std::string fasta_ref_gemome, std::unordered_map<std::string, std::string> &chromosomes){

    // open the fasta file
    faidx_t *fai = fai_load(fasta_ref_gemome.c_str());

    // check if the file was opened correctly
    if (fai == NULL){
        std::cerr << "Error opening the FASTA/FAI file." << std::endl;
        return 1;
    }

    // print a message to the user
    std::cout << "Reading the referece FASTA file..." << std::endl;

    // get the number of sequences in the fasta file
    int n_seqs = faidx_nseq(fai);

    // from the multi FASTA file get all the chromosome names and sequences and store them in vectors
    for (int i = 0; i < n_seqs; i++){

        // get the chromosome name
        const char *name = faidx_iseq(fai, i);

        // get chromosome end position
        int end = faidx_seq_len(fai, name);

        // get the chromosome sequence
        int len;
        char *seq = faidx_fetch_seq(fai, name, 0, end, &len);

        // store the chromosome name and sequence in the unordered map
        chromosomes[name] = seq;

        // free the memory
        free(seq);
    }

    // close the fasta file
    fai_destroy(fai);

    // assert that the number of names and sequences are the same
    assert(names.size() == seqs.size());

    return 0;

}


/**
 * @brief Read a BED file and store the information in a vector of Primer objects.
 * 
 * @param bed_file The name of the BED file.
 * @param primers A vector of Primer objects to store the information of the primers.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
int read_bed(std::string bed_file, std::vector<Primer> &primers){

    // open the BED file
    std::ifstream bed(bed_file);

    // check if the file was opened correctly
    if (!bed.is_open()){
        std::cerr << "Error opening the BED file." << std::endl;
        return 1;
    }

    // print a message to the user
    std::cout << "Reading the primer BED file..." << std::endl;

    // define a string to store the current line
    std::string line;

    /* Loop over the lines of the BED file (amplicaon primers).
       Every odd line            : information of the left primer
       Every even line           : information of the right primer
       Every two lines (odd line): create a Primer object and store it in the vector
    */
    int i = 0;
    std::string chrom;
    int left_start, left_end, right_start, right_end;

    while (std::getline(bed, line)){

        // split the line by tab
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, '\t')){
            fields.push_back(field);
        }

        // check if the line number is odd or even
        if (i & 1){
            // if the last bit of i is set, we are reading the second line of the primer
            // check if the chromosome is the same as the previous line
            if (fields[0] != chrom){
                std::cerr << "Error: the chromosome of the left and right primer are not the same." << std::endl;
                return 1;
            }            
            right_start = std::stoi(fields[1]);
            right_end = std::stoi(fields[2]);

            // create a Primer object and store it in the vector
            Primer p(chrom, left_start, left_end, right_start, right_end);
            primers.push_back(p);

        } else {
            // if the last bit of i is not set, we are reading the first line of the primer
            // store the information of the left primer
            chrom = fields[0];
            left_start = std::stoi(fields[1]);
            left_end = std::stoi(fields[2]);
        }

        // increase the line counter
        i++;
    }

    // close the BED file
    bed.close();

    return 0;

}


int main(int argc, char const *argv[]){

    // convert the reference genome file name to a string
    std::string ref_genome = argv[1];

    // create an unordered map to store the names (key) and sequences (value) of the chromosomes
    std::unordered_map<std::string, std::string> chromosomes;

    // call the read_fasta function and catch the return value
    int ret = read_fasta(ref_genome, chromosomes);
    // check if the function was executed correctly
    if (ret != 0){
        std::cerr << "Error reading the reference FASTA file." << std::endl;
        return 1;
    }

    // loop over the chromosomes and print the name and sequence
    for (auto it = chromosomes.begin(); it != chromosomes.end(); it++){
        std::cout << it->first << "\t" << it->second << std::endl;
    }

    // the second command line argument is the name of the BED file
    std::string bed_file = argv[2];

    // define a vector of Primer objects
    std::vector<Primer> primers;
    
    // call the read_bed function and catch the return value
    ret = read_bed(bed_file, primers);
    // check if the function was executed correctly
    if (ret != 0){
        std::cerr << "Error reading the primer BED file." << std::endl;
        return 1;
    }

    // loop over the primers and print the information
    for (auto it = primers.begin(); it != primers.end(); it++){
        std::cout << it->chr << "\t" << it->start_left << "\t" << it->end_left << "\t" << it->start_right << "\t" << it->end_right << std::endl;
    }

    return 0;


}