// testharness.hpp
/*
    Test harness for Levenshtein MySQL UDF.

    30 July 2019

    ... [Existing comments] ...
*/

// We do not have `#pragma once` because we need to be able to include this file
// multiple times.

#include <iostream>
#include "common.h" //gives IDE error but runs fine.

// Default definitions in case CMake does not define them
#ifndef LEV_FUNCTION
#define LEV_FUNCTION damlevconst
#endif

#ifndef LEV_ALGORITHM_NAME
#define LEV_ALGORITHM_NAME "damlevconst"
#endif

#ifndef LEV_ALGORITHM_COUNT
#define LEV_ALGORITHM_COUNT 2
#endif

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#ifndef PPCAT_NX
#define PPCAT_NX(A, B) A ## B
#endif
/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#ifndef PPCAT
#define PPCAT(A, B) PPCAT_NX(A, B)
#endif

#define LEV_ARGS PPCAT(LEV_FUNCTION,args)
#define LEV_INITID PPCAT(LEV_FUNCTION,initid)
#define LEV_MESSAGE PPCAT(LEV_FUNCTION,message)
#define LEV_CALL PPCAT(LEV_FUNCTION,_call)
#define LEV_INIT PPCAT(LEV_FUNCTION,_init)
#define LEV_DEINIT PPCAT(LEV_FUNCTION,_deinit)
#define LEV_SETUP PPCAT(LEV_FUNCTION,_setup)
#define LEV_TEARDOWN PPCAT(LEV_FUNCTION,_teardown)

// Use a "C" calling convention.
extern "C" {
int LEV_INIT(UDF_INIT *initid, UDF_ARGS *args, char *message);
long long LEV_FUNCTION(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void LEV_DEINIT(UDF_INIT *initid);
}

UDF_ARGS *LEV_ARGS;
UDF_INIT *LEV_INITID;
char *LEV_MESSAGE;

void LEV_SETUP(){
    LEV_ARGS = new UDF_ARGS;
    LEV_INITID = new UDF_INIT;
    LEV_MESSAGE = new char[512];

    // Initialize UDF_ARGS
    LEV_ARGS->arg_type = new Item_result[3];
    LEV_ARGS->args = new char*[3];
    LEV_ARGS->lengths = new unsigned long[3];
    // TODO: need to put a try for 3 or 2 here.
    LEV_ARGS->arg_count = LEV_ALGORITHM_COUNT;
    LEV_ARGS->arg_type[0] = STRING_RESULT;
    LEV_ARGS->arg_type[1] = STRING_RESULT;
    LEV_ARGS->arg_type[2] = INT_RESULT;
    // LEV_ARGS->arg_type[2] = DECIMAL_RESULT;
    // Initialize UDF_INIT
    int result = LEV_INIT(LEV_INITID, LEV_ARGS, LEV_MESSAGE);
    if(result == 1){
        std::cout << "Initialization Message: " << LEV_MESSAGE << std::endl;
    }
}

void LEV_TEARDOWN(){
    LEV_DEINIT(LEV_INITID);

    delete[] LEV_ARGS->arg_type;
    if(nullptr != LEV_ARGS->args){
        delete[] LEV_ARGS->args;
    }
    if(nullptr != LEV_ARGS->lengths){
        delete[] LEV_ARGS->lengths;
    }
    delete LEV_INITID;
    delete[] LEV_MESSAGE;
}

//long long LEV_CALL(char * subject, char * query){

long long LEV_CALL(char *subject, size_t subject_len, char *query, size_t query_len, long long max){
    long long result;

    LEV_ARGS->args[0] = subject;
    LEV_ARGS->lengths[0] = subject_len;
    LEV_ARGS->args[1] = query;
    LEV_ARGS->lengths[1] = query_len;
    LEV_ARGS->args[2] = (char *)(&max);
    LEV_ARGS->lengths[2] = sizeof(max);

    result = LEV_FUNCTION(LEV_INITID, LEV_ARGS, nullptr, LEV_MESSAGE);

    return result;
}
