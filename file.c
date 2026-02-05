#include "file.h"
#include "sheet.h"
#include "error.h"
#include <stdio.h>

//ذخیره جدول در فایل
int saveSheetToFile(Sheet* sheet, const char* filename);

//خواندن جدول از فایل
int loadSheetFromFile(Sheet* sheet, const char* filename);

int saveSheetToFile(Sheet* sheet, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        set_error(File_error);
        return 0;
    }

    for (int i = 0; i < sheet->rows; i++)
    {
        for (int j = 0; j < sheet->cols; j++)
            fprintf(file, "%.2f ", sheet->cells[i][j]);
        fprintf(file, "\n");
    }

    fclose(file);
    return 1;
}

int loadSheetFromFile(Sheet* sheet, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        set_error(File_error);
        return 0;
    }

    for (int i = 0; i < sheet->rows; i++)
    {
        for (int j = 0; j < sheet->cols; j++)
        {
            if (fscanf(file, "%lf", &sheet->cells[i][j]) != 1)
            {
                set_error(File_error);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;
}
