%define orgno 0x08048000
BITS 32             ;bin output defaults is 16bits. Change it!
ORG     orgno

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                Begin of the headers                                 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ehdr:                                            ; Elf32_Ehdr
              db      0x7F, "ELF", 1, 1, 1       ;   e_ident
      times 9 db      0
              dw      2                          ;   e_type
              dw      3                          ;   e_machine
              dd      1                          ;   e_version
              dd      _start                     ;   e_entry
              dd      phdrdata - $$              ;   e_phoff
              dd      0                          ;   e_shoff
              dd      0                          ;   e_flags
              dw      ehdrsize                   ;   e_ehsize
              dw      phdrsize                   ;   e_phentsize
              dw      2                          ;   e_phnum
              dw      0                          ;   e_shentsize
              dw      0                          ;   e_shnum
              dw      0                          ;   e_shstrndx
  
ehdrsize      equ     $ - ehdr

;;;;; data program header (read+write)
phdrdata:                           ; Elf32_Phdr
              dd      1             ;   p_type
              dd      0             ;   p_offset
              dd      orgno         ;   p_vaddr
              dd      orgno         ;   p_paddr
              dd      filesize      ;   p_filesz
              dd      filesize      ;   p_memsz
              dd      5             ;   p_flags (4 = r, 2 = w, 1 = exec)
              dd      0x1000        ;   p_align
phdrsize      equ     $ - phdrdata

;;;;; general program header (read+execute)
phdr_text:
              dd      1                 ;   p_type
              dd      _start_no-ehdr    ;   p_offset
              dd      _start            ;   p_vaddr
              dd      _start            ;   p_paddr
              dd      filesize          ;   p_filesz
              dd      filesize          ;   p_memsz
              dd      6                 ;   p_flags (4 = r, 2 = w, 1 = exec)
              dd      0x1000            ;   p_align


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 End of the headers                                 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .data
       hello:     db 'xla',10
       helloLen:  equ $-hello

section .text 
            _start_no equ $
_start:
        mov eax, hello
        mov [eax], byte 79   ; testing write to memory
        mov eax,4            ; The system call for write (sys_write)
        mov ebx,1            ; File descriptor 1 - standard output
        mov ecx,hello        ; Put the offset of hello in ecx
        mov edx,helloLen     ; helloLen is a constant, so we don't need to say
                             ;  mov edx,[helloLen] to get it's actual value
        int 80h              ; Call the kernel

        mov eax,1            ; The system call for exit (sys_exit)
        mov ebx,0            ; Exit with return code of 0 (no error)
        int 80h

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;               End of the program                                    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

filesize   equ     $ - $$

