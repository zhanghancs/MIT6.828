// Simple implementation of cprintf console output for the kernel,
// based on printfmt() and the kernel console's cputchar().

#include "inc/string.h"
#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/stdarg.h>

static void
putch(int ch, int* cnt) {
    cputchar(ch);
    *cnt++;
}

int vcprintf(const char* fmt, va_list ap) {
    int cnt = 0;

    vprintfmt((void*)putch, &cnt, fmt, ap);
    return cnt;
}

int cprintf(const char* fmt, ...) {
    va_list ap;
    int     cnt;
    char    new_fmt[1024];

    strcpy(new_fmt, "\033[31m"); // 将 ANSI escape code 复制到新的字符串
    strcat(new_fmt, fmt);        // 将原始格式字符串连接到新的字符串
    strcat(new_fmt, "\033[0m");

    va_start(ap, fmt);
    cnt = vcprintf(new_fmt, ap);
    va_end(ap);

    return cnt;
}
