#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "formula.h"
#include "sheet.h"
#include "error.h"

//محاسبه جمع و تفریق
double Expression(Sheet* sheet, char **s);

//محاسبه ضرب و تقسیم
double Term(Sheet* sheet, char **s);

//محاسبه پرانتز
double Factor(Sheet* sheet, char **s);

//خواندن عدد
double ReadNumber(char **s);

//محاسبه توابع
double Function(Sheet* sheet, char **s);

//خواندن مقدار سلول
double ReadCell(Sheet* sheet, char **s);

double evaluateFormula(Sheet* sheet, char *expr)
{
    char *p = expr;
    return Expression(sheet, &p);
}

double Expression(Sheet* sheet, char **s)
{
    double result = Term(sheet, s);

    while (**s == '+' || **s == '-')
    {
        char op = **s;
        (*s)++;
        double next = Term(sheet, s);

        if (op == '+') 
            result += next;
        else 
            result -= next;
    }

    return result;
}

double Term(Sheet* sheet, char **s)
{
    double result = Factor(sheet, s);

    while (**s == '*' || **s == '/')
    {
        char op = **s;
        (*s)++;
        double next = Factor(sheet, s);

        if (op == '*') 
            result *= next;
        else 
        {
            if (next == 0)
            {
                set_error(DivideByZero_error);
                return 0;
            }
            result /= next;
        }
    }

    return result;
}

double Factor(Sheet* sheet, char **s)
{
    while (**s == ' ') 
        (*s)++;

    if (**s == '(')
    {
        (*s)++;
        double result = Expression(sheet, s);
        if (**s == ')') 
            (*s)++;
        else
        {
            set_error(Syntax_error);
            return 0;
        }
        return result;
    }

    if (isalpha(**s))
    {
        if (isalpha(**s) && isdigit((*s)[1]))
            return ReadCell(sheet, s);
        else
            return Function(sheet, s);
    }

    return ReadNumber(s);
}

double ReadNumber(char **s)
{
    double num = 0;
    int hasDot = 0;
    double frac = 0.1;

    while (**s == ' ') 
        (*s)++;

    while ((**s >= '0' && **s <= '9') || **s == '.')
    {
        if (**s == '.') 
            hasDot = 1;
        else if (!hasDot) 
            num = num * 10 + (**s - '0');
        else
        {
            num += (**s - '0') * frac;
            frac *= 0.1;
        }
        (*s)++;
    }

    return num;
}

double ReadCell(Sheet* sheet, char **s)
{
    char addr[10];
    addr[0] = toupper(**s);
    (*s)++;
    int i = 1;

    while (isdigit(**s))
    {
        addr[i++] = **s;
        (*s)++;
    }
    addr[i] = '\0';

    double value;
    if (!getCellByAddress(sheet, addr, &value))
    {
        set_error(InvalidCell_error);
        return 0;
    }
    return value;
}

double Function(Sheet* sheet, char **s)
{
    char name[20];
    int i = 0;

    while (isalpha(**s))
    {
        name[i++] = **s;
        (*s)++;
    }
    name[i] = '\0';

    if (**s == '(') 
        (*s)++;

    double value = Expression(sheet, s);

    if (strcmp(name, "pow") == 0)
    {
        if (**s != ',')
        {
            set_error(Syntax_error);
            return 0;
        }
        (*s)++;

        double value2 = Expression(sheet, s);

        if (**s != ')')
        {
            set_error(Syntax_error);
            return 0;
         }
        (*s)++;

        return pow(value, value2);
    }

    if (**s != ')') 
    {
        set_error(Syntax_error);
        return 0;
    }
    (*s) ++;

    if (strcmp(name, "abs") == 0)
        return fabs(value);

    if (strcmp(name, "sin") == 0) 
        return sin(value);
    if (strcmp(name, "cos") == 0) 
        return cos(value);

    if (strcmp(name, "sinh") == 0)
        return sinh(value);
    if (strcmp(name, "cosh") == 0)
        return cosh(value);
    if (strcmp(name, "tanh") == 0)
        return tanh(value);

    if (strcmp(name, "tan") == 0)
    {
        if (cos(value) == 0)
        {
            set_error(Domain_error);
            return 0;
        } 
        return tan(value);
    }

    if (strcmp(name, "cot") == 0)
    {
        double t = tan(value);
        if (t == 0)
        {
            set_error(Domain_error);
            return 0;
        }
        return 1 / t;
    }


    if (strcmp(name, "sqrt") == 0)
    {
        if (value < 0) 
        { 
            set_error(Domain_error); 
            return 0; 
        }
        return sqrt(value);
    }

    if (strcmp(name, "exp") == 0)
        return exp(value);

    if ((strcmp(name, "log") == 0) || 
        (strcmp(name, "ln") == 0))
    {
        if (value <= 0) 
        { 
            set_error(Domain_error); 
            return 0; 
        }
        return log(value);
    }

    set_error(UnknownFunction_error);
    return 0;
}
