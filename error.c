#include "error.h"

// متغیر سراسری برای نگهداری خطای فعلی
static error_type current = No_error;

void set_error(error_type type)
{
    if (current == No_error)
        current = type;
}

error_type get_error(void)
{
    return current;
}

const char* message_error(void)
{
    switch (current)
    {
        case DivideByZero_error:
            return "Error: Can't divide by zero!";
        case InvalidCell_error:
            return "Error: Invalid cell reference!";
        case UnknownFunction_error:
            return "Error: This function is unknown!";
        case Syntax_error:
            return "Error: Invalid expression syntax!";
        case Domain_error:
            return "Error: Math domain error!";
        case File_error:
            return "Error: File is broken!"
        default:
            return "";
    }
}

void clear_error(void)
{
    current = No_error;
}
