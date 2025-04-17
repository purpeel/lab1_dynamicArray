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
    // ARRAY_ACCESS_ERROR = 6,
    // CONVERSION_ERROR = 7,
    // FILE_OPERATION_ERROR = 8,
    UNEXPECTED_ALPHA_ERROR = 9,
    // TYPE_MISMATCH_ERROR = 10,
    // ARGS_ERROR = 11,
    CMD_OUT_OF_CONTEXT_ERROR = 12,
    // STRING_PROCESSING_ERROR = 13,
    ZERO_LENGTH_INPUT_ERROR = 14,
    DOUBLE_INPUT_ERROR = 15,
    ARRAY_DATA_ALLOCATION_ERROR = 16
} Exception;


Exception charToInt( const char*, int * );
Exception charToDouble( const char*, double *);

double power( const double, int );


#endif