#include "sheet.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//ایجاد جدول با تعداد مشخص سطر و ستون
Sheet* createSheet(int rows, int cols);

//آزاد کردن حافظه جدول
void freeSheet(Sheet* sheet);

//تبدیل آدرس سلول به شماره سطر و ستون
static int parseAddress(const char *addr, int *row, int *col);

//خواندن مقدار سلولی مشخص
int getCellByAddress(Sheet* sheet, const char *addr, double *value);

//قرار دادن مقدار در سلولی مشخص
int setCellByAddress(Sheet* sheet, const char *addr, double value);

//اضافه کردن ردیف جدید
int addRow(Sheet* sheet);

//اضافه کردن ستون جدید
int addCol(Sheet* sheet);

Sheet* createSheet(int rows, int cols)
{
    Sheet* sheet = (Sheet*)malloc(sizeof(Sheet));
    sheet->rows = rows;
    sheet->cols = cols;

    sheet->cells = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++)
        sheet->cells[i] = (double*)calloc(cols, sizeof(double));

    return sheet;
}

void freeSheet(Sheet* sheet)
{
    if (!sheet) 
        return;
    for (int i = 0; i < sheet->rows; i++)
        free(sheet->cells[i]);
    free(sheet->cells);
    free(sheet);
}

static int parseAddress(const char *addr, int *row, int *col)
{
    if (!isalpha(addr[0])) 
        return 0;
    *col = toupper(addr[0]) - 'A';
    *row = 0;
    for (int i = 1; addr[i]; i++)
    {
        if (!isdigit(addr[i])) 
            return 0;
        *row = *row * 10 + (addr[i] - '1') + 1;
    }
    (*row)--;
    return 1;
}

int getCellByAddress(Sheet* sheet, const char *addr, double *value)
{
    int row, col;
    if (!parseAddress(addr, &row, &col) ||
        row < 0 || col < 0)
    {
        set_error(InvalidCell_error);
        return 0;
    }

    if (row >= sheet->rows || col >= sheet->cols)
    {
        *value = 0;
        return 1;
    }
    
    *value = sheet->cells[row][col];
    return 1;
}

int setCellByAddress(Sheet* sheet, const char *addr, double value)
{
    int row, col;
    if (!parseAddress(addr, &row, &col))
    {
        set_error(InvalidCell_error);
        return 0;
    }

    while (row >= sheet->rows)
        addRow(sheet);

    while (col >= sheet->cols)
        addCol(sheet);

    sheet->cells[row][col] = value;
    return 1;
}

int addRow(Sheet* sheet)
{
    double **newCells = (double**)realloc(sheet->cells, (sheet->rows + 1) * sizeof(double*));
    if (!newCells) 
        return 0;
    sheet->cells = newCells;
    sheet->cells[sheet->rows] = (double*)calloc(sheet->cols, sizeof(double));
    if (!sheet->cells[sheet->rows]) 
        return 0;
    sheet->rows++;
    return 1;
}

int addCol(Sheet* sheet)
{
    for (int i = 0; i < sheet->rows; i++)
    {
        double *newRow = (double*)realloc(sheet->cells[i], (sheet->cols + 1) * sizeof(double));
        if (!newRow) 
            return 0;
        newRow[sheet->cols] = 0;
        sheet->cells[i] = newRow;
    }
    sheet->cols++;
    return 1;
}
