#include <stdio.h>
#include <stdlib.h>
#include "sheet.h"
#include "formula.h"
#include "file.h"
#include "error.h"

int main()
{
    Sheet* sheet = createSheet(5, 5);
    clear_error();

    int choice;
    while (1)
    {
        printf("\n1. Set cell value\n2. Print table\n");
        printf("3. Evaluate formula\n4. Save\n");
        printf("5. Load\n0. Exit\nChoose: ");
               
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
            {
                char addr[10];
                double value;
                printf("Enter cell (e.g., A1): ");
                scanf("%s", addr);
                printf("Enter value: ");
                scanf("%lf", &value);

                if (!setCellByAddress(sheet, addr, value))
                    printf("%s\n", message_error());
                break;
            }
        
            case 2:
            {
                printf("\n   ");
                for (int c = 0; c < sheet->cols; c++)
                printf(" %c  ", 'A' + c);
                printf("\n");

                for (int r = 0; r < sheet->rows; r++)
                {
                    printf("%d |", r + 1);
                    for (int c = 0; c < sheet->cols; c++)
                        printf(" %.1lf", sheet->cells[r][c]);
                    printf("\n");
                }
                break;
            }

            case 3:
            {
                char expr[100];
                printf("Enter formula (e.g., A1 + sin(A2)): ");
                fgets(expr, sizeof(expr), stdin);
                expr[strcspn(expr, "\n")] = 0;

                double result = evaluateFormula(sheet, expr);
                if (get_error() != No_error)
                    printf("%s\n", message_error());
                else
                    printf("Result = %.2lf\n", result);
		clear_error();
                break;
            }

            case 4:
            {
                char filename[50];
                printf("Enter filename to save: ");
                scanf("%s", filename);
                if (!saveSheetToFile(sheet, filename))
                    printf("%s\n", message_error());
                break;
            }

            case 5:
            {
                char filename[50];
                printf("Enter filename to load: ");
                scanf("%s", filename);
                if (!loadSheetFromFile(sheet, filename))
                    printf("%s\n", message_error());
                break;
            }
            
            case 0:
                return 0;

            default:
            {
                set_error(Syntax_error);
                break;
            }
        }
    }

    freeSheet(sheet);
    return 0;
}
