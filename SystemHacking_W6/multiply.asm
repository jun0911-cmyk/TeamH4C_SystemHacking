global main, _start

section .bss
buffer: resb 128
res: resb 10
increase: resb 10

section .data
fmt: db '%d', 0
equal: db '=', 0
mul: db '*', 0
end: db 10, 0

section .text
    extern printf
    extren scanf
    extren exit

multi:
    push rbp
    mov rbp, rsp
    xor rax, rax
    xor rbx, rbx
    inc rax
    inc rbx

_loop:
    mov rcx, 9
    cmp rbx, rcx
        jg +end
    mov rax, rbx
    mov rdi, [buffer]
    mul rdi
    mov [increase], rbx
    mov [res], rax
    call print_line

    inc rbx
    jmp _loop

_end:
    leave
    ret

print_line:
    push rbp,
    mov rbp, rsp
    mov rsi, [buffer]
    mov rdi, fmt
    mov rax, 0
    call printf
    mov rsi, [increase]
    mov rdi, fmt
    mov rax, 0
    call printf
    mov rsi, [res]
    mov rdi, 0
    call printf
    mov rdi, end
    mov rax, 0
    call printf
    leave ret

main:
_start:
    push rbp
    mov rsi, buffer
    mov rdi, fmt
    mov rax, 0
    call scanf
    call multi
    call exit
    pop rbp
    ret
