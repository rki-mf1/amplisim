#ifndef PRIMER_H
#define PRIMER_H

#include <string>


// define a data class Primer to store the information of a primer
// the class has the following attributes:
// - chr: the chromosome of the primer
// - start_left: the start position of the left primer
// - end_left: the end position of the left primer
// - start_right: the start position of the right primer
// - end_right: the end position of the right primer
class Primer{
    public:
        std::string chr;
        int start_left;
        int end_left;
        int start_right;
        int end_right;
        Primer(std::string chr, int start_left, int end_left, int start_right, int end_right);
};

// define the contructor of the Primer class
Primer::Primer(std::string chr, int start_left, int end_left, int start_right, int end_right){
    this->chr = chr;
    this->start_left = start_left;
    this->end_left = end_left;
    this->start_right = start_right;
    this->end_right = end_right;
}


#endif // PRIMER_H