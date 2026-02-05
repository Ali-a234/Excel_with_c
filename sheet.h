#ifndef SHEET_H
#define SHEET_H

#include <stddef.h>

typedef struct {
    int rows;
    int cols;
    double **cells;
} Sheet;

// جدول دینامیک با تعداد سطر و ستون اولیه
Sheet* createSheet(int rows, int cols);

//آزاد کردن حافظه
void freeSheet(Sheet* sheet);

//دریافت مقدار سلول با آدرس
int getCellByAddress(Sheet* sheet, const char *addr, double *value);

// ست کردن مقدار سلول با آدرس
int setCellByAddress(Sheet* sheet, const char *addr, double value);

// اضافه کردن سطر یا ستون
int addRow(Sheet* sheet);
int addCol(Sheet* sheet);

#endif
