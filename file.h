#ifndef FILE_H
#define FILE_H

#include "sheet.h"

//ذخیره جدول
int saveSheetToFile(Sheet* sheet, const char* filename);

// بارگذاری جدول
int loadSheetFromFile(Sheet* sheet, const char* filename);

#endif
