                .text

                .global syscall, end
syscall:        mov 4(%esp), %eax
                mov 8(%esp), %ecx
                mov 12(%esp), %edx
                int $128
                ret

end:            mov 4(%esp), %eax
                int $129
