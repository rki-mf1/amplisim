// Purpose: Main file for the amplisim program.
#include <iostream>
#include <vector>
#include <string>
#include <htslib/faidx.h>



/**
 * @brief Read a FASTA file and store the names and sequences in vectors.
 * 
 * @param fasta_file The name of the FASTA file.
 * @param names A vector of strings to store the names of the sequences.
 * @param seqs A vector of strings to store the sequences.
 * @return int 0 if the function was executed correctly, 1 otherwise.
 */
int read_fasta(std::string fasta_ref_gemome, std::vector<std::string> &names, std::vector<std::string> &seqs){

    // open the fasta file
    faidx_t *fai = fai_load(fasta_ref_gemome.c_str());

    // check if the file was opened correctly
    if (fai == NULL){
        std::cerr << "Error opening the FASTA/FAI file." << std::endl;
        return 1;
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

        // store the chromosome name and sequence in the vectors
        names.push_back(name);
        seqs.push_back(seq);

        // free the memory
        free(seq);
    }

    // close the fasta file
    fai_destroy(fai);

    // assert that the number of names and sequences are the same
    assert(names.size() == seqs.size());

    return 0;

}



int main(int argc, char const *argv[]){

    // convert the reference genome file name to a string
    std::string ref_genome = argv[1];

    // call the read_fasta function to read the reference genome and store names and sequences in vectors
    std::vector<std::string> names;
    std::vector<std::string> seqs;

    read_fasta(ref_genome, names, seqs);

    // loop over the chromosomes and print the name and sequence
    for (int i = 0; i < names.size(); i++){
        std::cout << names[i] << std::endl;
        std::cout << seqs[i] << std::endl;
    }

    
    

    return 0;


}