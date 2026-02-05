#ifndef ERROR_H
#define ERROR_H

//تعریف انواع خطاهای ممکن
typedef enum
{
    No_error,               
    DivideByZero_error,
    InvalidCell_error,
    UnknownFunction_error,
    Syntax_error,
    Domain_error,
    File_error
} error_type;

//تابع ثبت خطا اگر قبلا خطایی رخ نداده باشد
void set_error(error_type type);

//تابع خواندن خطای فعلی
error_type get_error(void);

//تابع نمایش پیام خطا به کاربر
const char* message_error(void);

//تابع پاک کردن خطای فعلی بعد از بررسی
void clear_error(void);

#endif
