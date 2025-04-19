#ifndef H_UTILITIES
#define H_UTILITIES


typedef enum _comparisonResult {
    GREATER = -1,
    EQUAL = 0,
    LESS = 1
} ComparisonResult;


typedef enum {
    USER_EXIT,
    SYSTEM_EXIT,
    USER_CONTINUE
} menuDirective;


typedef enum execStatus {
    SUCCESSFUL_EXECUTION = 0,
    MEMORY_ALLOCATION_ERROR = 1,
    INPUT_OVERFLOW_ERROR = 2,
    EMPTY_STORAGE_ERROR = 3,
    ARRAYS_TYPEINFO_MISMATCH_ERROR = 4,
    TOO_FEW_ARRAYS_ERROR = 5,
    NULL_TYPEINFO_ERROR = 6,
    UNEXPECTED_ALPHA_ERROR = 9,
    CMD_OUT_OF_CONTEXT_ERROR = 12,
    ZERO_LENGTH_INPUT_ERROR = 14,
    DOUBLE_INPUT_ERROR = 15,
    ARRAY_DATA_ALLOCATION_ERROR = 16
} Exception;


Exception charToInt( const char*, int * );
Exception charToDouble( const char*, double *);


#endif