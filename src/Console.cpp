#include "../h/syscall_cpp.hpp"

char Console::getc() {
    // Call syscall_c.h getc() function
    return ::getc();
}
void Console::putc(char c) {
    // Call syscall_c.h putc(char) function
    ::putc(c);
}
