                .text
                .global setGdt, setTSS, setIdt

setIdt:         mov     4(%esp), %eax
                lidt    (%eax)
                ret

setGdt:         mov     4(%esp),  %eax
                lgdt    (%eax)
                mov     $0x10,    %ax
                mov     %ax,      %ds
                mov     %ax,      %es
                mov     %ax,      %fs
                mov     %ax,      %gs
                mov     %ax,      %ss
                jmp     $0x08,    $reload_CS

reload_CS:      ret

setTSS:         mov     $0x2B,    %ax
                ltr     %ax
                ret
