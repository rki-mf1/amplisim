#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <htslib/faidx.h>

#include "Primer.h"


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


/**
 * @brief Read a reference FASTA file and store the names and sequences in vectors.
 * 
 * @param fasta_file The name of the FASTA file.
 * @param chromosomes An unordered map to store the names (key) and sequences (value) of the chromosomes.
 * @param verbose A boolean to indicate if the function should print messages to the user.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
static int read_reference(std::string fasta_ref_gemome, std::unordered_map<std::string, std::string> &chromosomes, const bool verbose = false){

    // open the fasta file
    faidx_t *fai = fai_load(fasta_ref_gemome.c_str());

    // check if the file was opened correctly
    if (fai == NULL){
        std::cerr << "Error opening the FASTA/FAI file." << std::endl;
        return 1;
    }

    if (verbose){
        std::cout << "Reading the referece FASTA file..." << std::endl;
    }

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

    // assert that the chromosomes map is not empty
    assert(!chromosomes.empty());

    return 0;

}


/**
 * @brief Read a primer BED file and store the information in a vector of Primer objects.
 * 
 * @param bed_file The name of the BED file.
 * @param primers A vector of Primer objects to store the information of the primers.
 * @param verbose A boolean to indicate if the function should print messages to the user.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
static int read_primer(std::string bed_file, std::vector<Primer> &primers, const bool verbose = false){

    // open the BED file
    std::ifstream bed(bed_file);

    // check if the file was opened correctly
    if (!bed.is_open()){
        std::cerr << "Error opening the BED file." << std::endl;
        return 1;
    }

    if (verbose){
        std::cout << "Reading the primer BED file..." << std::endl;
    }
    
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

    while (std::getline(bed, line) && !line.empty()){       // avoid getting trapped in eof artefacts (empty lines)

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


/**
 * @brief Write the amplicons to a FASTA file or COUT.
 * 
 * @param amplicons_fasta The name of the FASTA file.
 * @param amplicons A vector of strings containing the amplicons.
 * @param vec_reps A vector of integers containing the number of replications for each amplicon.
 * @param write_to_stdout A boolean to indicate if the amplicons should be written to stdout.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
static int write_amplicons(const char*                    amplicons_fasta,
                           const std::vector<std::string> &amplicons,
                           const std::vector<int>         &vec_reps,
                           const bool                     write_to_stdout = true){

    // open the FASTA file
    std::ofstream fasta;

    if (!write_to_stdout){
        fasta = std::ofstream(amplicons_fasta);

        if (!fasta.is_open()){
            std::cerr << "Error opening the FASTA file." << std::endl;
            return 1;
        }

        std::cout << "Writing the amplicons to a FASTA file..." << std::endl;
    }

    // cast the size of the amplicons vector an int
    int n_amplicons = amplicons.size();

    // insert idx
    int idx_insert = 0;
    int count_per_insert = 1;

    // loop over the amplicons and write them to the FASTA file
    for (int idx_amplicon = 0; idx_amplicon < n_amplicons; ++idx_amplicon){

        if (!write_to_stdout){
            fasta << ">amplicon_" << idx_insert << "_" << idx_amplicon << std::endl;
            fasta << amplicons[idx_amplicon] << std::endl;
        }
        else{
            std::cout << ">amplicon_" << idx_insert << "_" << idx_amplicon << std::endl;
            std::cout << amplicons[idx_amplicon] << std::endl;
        }

        // check if the current amplicon is the last one of the current insert
        if (count_per_insert == vec_reps[idx_insert]){
            // if the current amplicon is the last one of the current insert, reset the counter and increase the insert index
            count_per_insert = 1;
            idx_insert++;
        } else {
            // if the current amplicon is not the last one of the current insert, increase the counter
            count_per_insert++;
        }
    }

    if (!write_to_stdout){
        // close the FASTA file
        fasta.close();
    }

    return 0;
}




#endif // UTIL_H