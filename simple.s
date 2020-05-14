global _start

section .text

_start:         nop
                nop
                nop
                sub rsp, 6
                mov byte [rsp], 'H'
                mov byte [rsp + 1], 'e'
                mov byte [rsp + 2], 'l'
                mov byte [rsp + 3], 'l'
                mov byte [rsp + 4], 'o'
                mov byte [rsp + 5], 0x0A
                mov rax, 1              ; syscall Write(int fd, const void* buff, size_t count)
                mov rdi, 1
                mov rsi, rsp
                mov rdx, 6
                syscall
                mov rax, 0x3C           ; syscall Exit(int exit_code)
                mov rdi, 0
                syscall
                nop
                nop
                nop