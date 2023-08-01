#ifndef PRIMER_H
#define PRIMER_H

#include <string>


/**
 * @brief A class to store the information of a primer
 * 
 */
class Primer{
    public:
        std::string chr;
        int start_left;
        int end_left;
        int start_right;
        int end_right;
        Primer(std::string chr, int start_left, int end_left, int start_right, int end_right);
};


/**
 * @brief Construct a new Primer object
 * 
 * @param chr The chromosome of the primer.
 * @param start_left The start position of the left primer.
 * @param end_left The end position of the left primer.
 * @param start_right The start position of the right primer.
 * @param end_right The end position of the right primer.
 */
Primer::Primer(std::string chr, int start_left, int end_left, int start_right, int end_right){
    this->chr = chr;
    this->start_left = start_left;
    this->end_left = end_left;
    this->start_right = start_right;
    this->end_right = end_right;
}


#endif // PRIMER_H