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




#endif // PRIMER_H