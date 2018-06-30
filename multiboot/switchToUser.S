                .text
                .global switchToUser


switchToUser:   mov     $0x23,    %ax
                mov     %ax,      %ds
                mov     %ax,      %es
                mov     %ax,      %fs
                mov     %ax,      %gs

                mov     12(%esp), %eax          # pointer to kernel stack pointer is in eax now

                push    %ebp                    # save ebp to stack
                mov     %esp,     %ebp          # copy esp to ebp so we can use ebp for access function parameters

                push    %ebx                    # save registers
                push    %esi
                push    %edi

                mov     %esp,     (%eax)        # TSS stack pointer is now setup to kernel stack pointer of current value

                push    $0x23                   # stack segment
                push    12(%ebp)                # user stack pointer
                pushf                           # flags
                push    $0x1b                   # code segment
                push    8(%ebp)                 # destination of user text
                iret
