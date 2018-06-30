                .text

                .global Int128Handler, Int129Handler, do_syscall_in_C


Int128Handler:  push %gs
                push %fs
                push %es
                push %ds
                push %ebp
                push %edi
                push %esi
                push %edx
                push %ecx
                push %ebx
                push %esp
                push %edx
                push %ecx
                push %eax
                call do_syscall_in_C
                add $12, %esp
                pop %esp
                pop %ebx
                pop %ecx
                pop %edx
                pop %esi
                pop %edi
                pop %ebp
                pop %ds
                pop %es
                pop %fs
                pop %gs
                iret

Int129Handler:  add     $20,    %esp      # throw iret data
                push    %eax              # save return value for now
                mov     $0x10,  %ax       # set data segments to kernel
                mov     %ax, %ds          # set data segments to kernel
                mov     %ax, %es          # set data segments to kernel
                mov     %ax, %fs          # set data segments to kernel
                mov     %ax, %gs          # set data segments to kernel
                pop     %eax              # retrieve return value
                pop     %edi              # restore registers
                pop     %esi              # restore registers
                pop     %ebx              # restore registers
                pop     %ebp              # restore registers
                ret                       # should return like from switchToUser
