        .text

        .global inb, outb
inb:    mov 4(%esp), %edx
        mov $0, %eax
        in %dx, %al
        ret

outb:   mov 4(%esp), %eax
        mov 8(%esp), %edx
        out %al, %dx
        ret
