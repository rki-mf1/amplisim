#ifndef ARGPARSER_H
#define ARGPARSER_H

/**
 * @file argp.h is the raw C header file for the GNU argp library.
 *       It is included here to avoid having to install the library
 *       on the system. The library is used to parse command line
 *       arguments.    
*/
#include <argp.h>



static char doc[] = "amplisim -- a program to simulate amplicon sequences from a reference genome";
static char args_doc[] = "REFERENCE PRIMERS";

static struct argp_option options[] = {
    {"output",  'o', "FILE", 0, "Output to FILE instead of standard output"},
    {"seed",    's', "INT" , 0, "Set a random seed"},
    {"mean",    'm', "INT" , 0, "Set the mean number of replicates per amplicon"},
    {"sd",      'n', "INT" , 0, "Set the standard deviation for the mean number of replicates per amplicon"},
    {"dropout", 'd', "INT" , 0, "Set the likelihood for an amplicon dropout [0,1)"},
    {0}
};

struct arguments {
    char *args[2];    // reference and primer files
    char *output_file;
    int seed;
    bool verbose = false;
    int mean;
    int sd;
    double dropout;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    struct arguments *arguments = (struct arguments *)state->input;

    switch (key){
        case 'o':
            arguments->output_file = arg;
            arguments->verbose = true;
            break;
        case 's':
            arguments->seed = atoi(arg);
            assert(arguments->seed > 0);
            break;
        case 'm':
            arguments->seed = atoi(arg);
            assert(arguments->mean > 0);
            break;
        case 'n':
            arguments->seed = atoi(arg);
            assert(arguments->sd >= 0);
            break;
        case 'd':
            arguments->seed = atof(arg);
            assert(arguments->dropout >= 0);
            assert(arguments->dropout < 1);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2){
                argp_usage(state);
            }
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 2){
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};






#endif // ARGPARSER_H