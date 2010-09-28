%macro  begin 1
    push    ebp 
    mov     ebp,esp
  %if %1
    sub     esp,%1
  %endif
%endmacro

%macro return 0
    mov esp, ebp
    pop ebp  
    ret
%endmacro

%macro addarg 1
    push %1
%endmacro

%macro clargs 1
    add esp, 4 * %1
%endmacro


%macro exit 1
    mov eax, 1
    mov ebx, %1
    int 80h
%endmacro

%macro print_lf 0
  addarg 10
  call __print_c
  clargs 1
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%define MEMORY_SIZE  1048576
%define BUFFER_SIZE  1024

section .bss
    __mem    resb  MEMORY_SIZE

section .data
    __mem_ptr   dd 0

    __str_true  db 'verdadeiro',0
    __str_false db 'falso',0
    __str_no_mem_left db 'Não foi possível alocar memória.',0

section .text
global _start
 _start:
;     addarg __buffer1
;     call __atof
;     clargs 1
; 
;     addarg eax
;     call __imprima_real
;     clargs 1
; 
     exit 0

__imprima_literal:
    %define string ebp+8
    begin 0

    mov eax, [string]
    cmp eax, 0
    jz .end

    addarg dword [string]
    call __print
    clargs 1

    print_lf

    .end
      return

__imprima_inteiro:
    ;params
    %define num ebp+8

    ;local
    %define buffer   ebp-BUFFER_SIZE
    begin BUFFER_SIZE
    
    lea eax, [buffer]
    addarg eax
    addarg dword [num]    
    call __itoa
    clargs 2

    lea eax, [buffer]
    addarg eax
    call __print
    clargs 1

    print_lf

    return

__imprima_real:
    ;params
    %define num ebp+8

    ;local
    %define buffer   ebp-BUFFER_SIZE
    begin BUFFER_SIZE

    lea eax, [buffer]
    addarg eax
    addarg dword [num]    
    call __ftoa
    clargs 2

    lea eax, [buffer]
    addarg eax
    call __print
    clargs 1

    print_lf

    return
  
__print_c:
    ;params
    %define carac ebp+8

    begin 0

    lea eax, [carac]
    addarg eax
    call __print
    clargs 1

    return

__imprima_caractere:
    ;params 
    %define carac ebp+8
    begin 4
    
    addarg dword [carac]
    call __print_c
    clargs 1

    print_lf
    
    return

__imprima_logico:
    ;params 
    %define val ebp+8

    begin 0

    mov eax, [val]
    cmp eax, 0
    jz .false
    mov eax, __str_true
    jmp .print

    .false:
      mov eax, __str_false

    .print:
      addarg eax
      call __print
      clargs 1

      print_lf
  
    return

__strpos:
    ;params
    %define string ebp+12
    %define carac  ebp+8

    begin 0

    mov eax, [string]
    cmp eax, 0
    jnz .calc

    mov eax, 0
    return

    .calc:
      mov eax, 0
      .do:
        mov ebx, [string]
        mov ecx, [carac]
        cmp [ebx+eax], cl
        jz  .break

        cmp [ebx+eax], byte 0
        jz .notfound

        inc eax
        jnz .do
      .break:

    return

    .notfound
      mov eax, -1
      return

__strlen:
    ;params
    %define string ebp+8

    begin 0

    addarg dword [string]
    addarg 0
    call __strpos
    clargs 2

    return

__print:
    ;params
    %define string ebp+8

    begin 0

    addarg dword [string]
    call __strlen
    clargs 1

    mov edx, eax
    mov eax, 4
    mov ebx, 1
    mov ecx, [string]
    int 80h

    return

__is_num:
    ;params
    %define carac ebp+8

    begin 0

    cmp byte  [carac], 48
    jl .false

    cmp byte [carac], 57
    jg .false

    mov eax, 1
    return

    .false:
      mov eax, 0
      return

__atoi:
    ;params
    %define string ebp+8
  
    ;locals
    %define num   ebp-4
    %define m     ebp-8
    %define i     ebp-12
    %define len   ebp-16
    %define negt  ebp-20
    begin 20
  
    mov dword [num], 0
    mov dword [m], 1
  
    addarg dword [string]
    call __strlen
    clargs 1

    cmp eax, dword 0
    jz .ret_zero

    mov dword [len], eax
    mov dword [i], 0

    ;checks for signal (+-)
    %define minus_sig 45 
    %define plus_sig 43
    mov eax, [string]
    xor ebx, ebx
    mov bl, [eax]

    cmp ebx, minus_sig
    jz .negative

    cmp ebx, plus_sig
    jz .positive

    jmp .conv

    .negative
      mov dword [negt], 1
      mov eax, [string]
      inc eax
      mov [string], eax
      jmp .conv

    .positive
      mov dword [negt], 0
      mov eax, [string]
      inc eax
      mov [string], eax

  .conv:

    mov eax, [len]
    .while:
      mov eax, dword [i]
      cmp eax, [len]
      jg .endwhile      
      
      mov ebx, [string]
      mov ecx, [i]        
      xor edx, edx
      mov dl, [ebx+ecx]      

      addarg edx
      call __is_num
      clargs 1
  
      cmp eax, 0
      jz .endwhile
  
      inc dword [i]
      jmp .while
    .endwhile:
  
    cmp dword [i], 0
    jz .ret_zero
  

    dec dword [i]    
    
    .while2:
      cmp dword [i], 0
      jl .endwhile2

      ; get the string[len-i]  
      mov ebx, [string]
      mov ecx, [i]
      xor eax, eax
      mov al, byte [ebx+ecx]

      ;(string[len-i] - 48) * m
      sub al, 48

      mul dword [m]      

      ;num += (string[len-i] - 48) * m
      add [num], eax

      dec dword [i]

      ;m *= 10
      mov eax, [m]
      mov ebx, 10
      mul ebx
      mov [m], eax

      jmp .while2
    .endwhile2:
  
    cmp dword [negt], 1
    jnz .return_pos

    mov eax, [num]
    neg eax
    return
   
    .return_pos
      mov eax, [num]
      return

    .ret_zero
      mov eax, 0
      return

__atof:
    ;params
    %define string ebp+8

    ;locals
    %define negt     ebp-4
    %define dotpos   ebp-8
    %define declen   ebp-12
    %define str_dec  ebp-16
    %define inte     ebp-20
    %define deci     ebp-24
    %define p        ebp-28
    %define float    ebp-32
    
    begin 32

    addarg dword [string]
    call __strlen
    clargs 1

    cmp eax, dword 0
    jz .ret_zero

    ;checks for signal (+-)
    %define minus_sig 45 
    %define plus_sig 43
    mov eax, [string]
    xor ebx, ebx
    mov bl, [eax]

    cmp ebx, minus_sig
    jz .negative

    cmp ebx, plus_sig
    jz .positive

    jmp .conv

    .negative
      mov dword [negt], 1
      mov eax, [string]
      inc eax
      mov [string], eax
      jmp .conv

    .positive
      mov dword [negt], 0
      mov eax, [string]
      inc eax
      mov [string], eax

  .conv:

    addarg dword [string]
    addarg '.'
    call __strpos
    clargs 2
    
    mov dword [dotpos], eax

    ;get decimal str
    mov eax, [string]
    add eax, [dotpos]
    inc eax

    mov [str_dec], eax

    addarg dword [str_dec]
    call __strlen
    clargs 1
    
    mov dword [declen], eax    

    addarg dword [str_dec]
    call __atoi
    clargs 1

    mov dword [deci], eax

    mov eax, [string]
    add eax, [dotpos]
    mov [eax], byte 0

    addarg dword [string]
    call __atoi
    clargs 1

    mov [inte], eax

    mov eax, dword [declen]
    
    addarg 10
    addarg eax
    call __pow
    clargs 2

    mov dword [p], eax

    finit
    fld   dword [deci]
    fdiv  dword [p]
    fiadd  dword [inte]
    fstp  dword [float]

    cmp dword [negt], 1
    jnz .return_pos

    mov eax, [float]
    or eax, 0x80000000
    return
   
    .return_pos
      mov eax, [float]
      return

    .ret_zero
      mov eax, 0
      return

    exit 0

__itoa:
    ;params
    %define buffer ebp+12
    %define num ebp+8

    ;local
    %define sig ebp-4
    begin 4

    ;sig = (num >> 31);
    mov eax, [num]
    mov [sig], eax
    shr eax, 31
    mov [sig], eax    

    cmp [sig], dword 0
    jz .endif
      neg dword [num]
    .endif:

    
    mov ecx, 0
    mov eax, [num] ;123
    .trans
      mov ebx, 10
      mov edx, 0
      idiv ebx   ;edx:eax / op: 123 / 10 -- eax = 12, edx = 3
      add edx, byte 48
      push edx
      inc ecx ;update stack size
    cmp eax, 0
    jnz .trans

    
    cmp [sig], dword 0
    jz .else1
      %define minus_sig 45
      mov edx, 1 ;addr offset
      mov eax, [buffer]
      mov [eax], byte minus_sig            
      jmp .endif1
    .else1:
      mov edx, 0 ;addr offset
    .endif1
    
    .reorder
      pop eax
      mov ebx, [buffer]
      mov [ebx+edx], eax ;[buffer+offset]
      dec ecx ;alg counter--
      inc edx ;offset++
    cmp ecx, 0 ;alg counter == 0? leave : reorder
    jnz .reorder

    return

__ftoa:
    %define BIAS 127

    ;params
    %define buffer ebp+12
    %define num ebp+8

    ;local
    %define exp    ebp-4
    %define mant   ebp-8
    %define left   ebp-12
    %define sig    ebp-16    
    %define offset ebp-20
    %define r      ebp-24
    %define term   ebp-28
    %define acc    ebp-32
    %define tmp    ebp-36
    %define right  ebp-40
    ;last ebp localvar offset
    %define lst    40    
  
    begin lst

    ;sig = (num >> 31);
    mov eax, [num]
    mov [sig], eax
    shr eax, 31
    mov [sig], eax

    ;num = num & 0x7FFFFFFF;
    mov eax, [num]
    and eax, 0x7FFFFFFF
    mov [num], eax

    ;exp = num >> 23;
    mov eax, [num]
    shr eax, 23
    mov [exp], eax

    ;mant = num & 0x7FFFFF;
    mov eax, [num]
    and eax, 0x7FFFFF
    mov [mant], eax

    ;if((exp | mant) == 0) return "0.0";
    mov eax, [exp]      
    mov ebx, [mant]
    or eax, ebx      
      cmp eax, 0
      jnz .endif
      mov eax, 0x00302E30 ;"0.0"
      jmp .end
    .endif:

    ;exp -= BIAS;
    sub [exp], dword BIAS
  
    ;mant |= 0x800000;       //+ hidden (1.)
    or [mant], dword 0x800000

    ;if(((24+1)-exp) < 32) { //not sure if this is right (ocurrs on 0.001)
    ;  left = mant >> ((24-1)-exp);//24 - (hidden) - exp
    ;} else {
    ;  left = 0;
    ;}
    mov eax, 25
    sub eax, [exp]
      cmp eax, 32
      jg .else1   
      mov eax, [mant]
      mov ecx, 23
      sub ecx, [exp]
      shr eax, cl
      mov [left], eax
      jmp .endif1
    .else1:
      mov [left], dword 0
    .endif1:

    ;if(8 + 1 + exp > 0) {//not sure about this eighter
    ;  mant = mant << (8 + 1 + exp); //hidden + exp
    ;} else { 
    ;  exp *= -1;
    ;  mant = mant >> (exp - (8+1));
    ;}
    mov eax, 9
    add eax, [exp]
      cmp eax, 0
      jl .else2
      mov ecx, 9
      add ecx, [exp]
      mov eax, [mant]
      shl eax, cl
      mov [mant], eax
      jmp .endif2
    .else2:
      mov ecx, [exp]
      sub ecx, 9
      mov eax, [mant]
      shr eax, cl          
    .endif2:


    ;int offset = 31;
    mov [offset], dword 31

    ;int r = mant;
    mov eax, [mant]
    mov [r], eax

    ;int term = 1;
    mov [term], dword 1

    ;double acc = 0;
    mov [acc], dword 0


    finit

    ;while(offset != 1) {  
    .while:    
      mov eax, [offset]
      cmp eax, 1
      jz .endwhile
  
      ;if((r >> offset) & 1) {
      mov eax, [r]
      mov ecx, [offset]
      shr eax, cl
      and eax, 1
        cmp eax, 0
        jz .endif3
  
        ;acc += pow(2, -(term));
        mov eax, 2
        mov ebx, [term]
        neg ebx

        addarg eax
        addarg ebx
        call __pow ;res on eax
        clargs 2

        mov [tmp], eax
  
        fld  dword [acc] ;st0 = acc      
        fadd dword [tmp]
        fstp dword [acc]
      .endif3:

      ;term++;
      inc dword [term]

      ;offset--;
      dec dword [offset]
      jmp .while
    .endwhile:

    finit

    ;;acc *=100;
    fld dword [acc] ;st0 = acc
    mov [tmp], dword 100
    fimul dword [tmp]
    ;round acc
    frndint
    ;get the integer part and put it on acc
    fist dword [acc]

    mov eax, [acc]
    mov [right], eax


    ;create string "left.[0]right"

    cmp [sig], dword 0
    jz .endif5
      neg dword [left]
    .endif5:

    addarg dword [buffer]
    addarg dword [left]
    call __itoa
    clargs 2
    ;buffer has "left"
    
    addarg dword [buffer]
    call __strlen
    clargs 1

    add eax, dword [buffer]
    mov [eax], byte '.'
    inc eax
    mov [tmp], eax    

    ;make room
    %define buffer2 ebp-lst+4
    %define bfsize  1024
    sub  esp, bfsize

    addarg dword [buffer2]
    addarg dword [right]
    call __itoa
    clargs 2
    ;buffer2 has "right"

    addarg dword [buffer2]
    call __strlen
    clargs 1

    ;check if decimal has 2 digits
    cmp eax, 2
    jz .endif4
      %define zero 48
      xor eax, eax
      mov ebx, [buffer2]
      mov al, [ebx]
      mov [ebx], byte zero
      mov byte [ebx+1], al
    .endif4:

    mov eax, [buffer2]
    mov ebx, [tmp]
    .append:
      cmp [eax], byte 0
      jz .endappend
      
      xor ecx, ecx
      mov cl, [eax]
      mov [ebx], cl
      inc eax
      inc ebx
      jmp .append
    .endappend:

    mov [ebx], byte 0

  .end
    return

__pow:
    ;params
    %define base ebp+12
    %define exp  ebp+8

    ;local
    %define res  ebp-4
    %define i    ebp-8
    %define sig  ebp-12
    %define tmp  ebp-16
    begin 14

    ;int neg = exp < 0;
    mov eax, [exp]
    mov ecx, 31
    shr eax, cl
    mov [sig], eax


    ;if(neg) exp *= -1;
    mov eax, [sig]
    cmp eax, 0
    jz .endif
      mov eax, [exp]
      neg eax
      mov [exp], eax
    .endif:

    ;res = 1;
    mov [res], dword 1

    ;for(i = 0; i < exp; i++) {
    mov [i], dword 0
    .while:
      mov eax, [i]
      cmp eax, [exp]
      jge .endwhile

      ;res *= base;
      mov eax, [res]
      mul dword [base]
      mov [res], eax

      ;i++
      inc dword [i]
      jmp .while
    .endwhile:

    ;if(neg) res = 1.0 / res;
    cmp [sig], dword 0
    jz .noneg
    .negative:
      finit
      mov [tmp], dword 0x3F800000 ; 1.0
  
      fld   dword [tmp]
      fidiv dword [res]
      fstp  dword [res]
    .noneg:

    mov eax, [res]

    return

matrix_init__:
    ;params
    %define matrix      ebp+16
    %define is_string   ebp+12
    %define msize       ebp+8

    ;local
    %define total     ebp-4
    begin 4

    cmp [is_string], dword 0
    jnz .else
      mov eax, [msize]
      mov [total], eax
      jmp .endif
    .else:
      mov eax, [msize]
      mov ebx, 32 ;bits
      mul ebx
      mov [total], eax
    .endif:
  
    mov eax, 0
    .while:
      cmp eax, [total]
      jge .endwhile
      mov ecx, [matrix]
      add ecx, eax
      mov [ecx], byte 0
      inc eax
      jmp .while
    .endwhile:

    return

__matrix_cpy:
    ;params
    %define src       ebp+20
    %define dest      ebp+16
    %define is_string ebp+12
    %define msize     ebp-8

    ;local
    %define total     ebp-4
    begin 4

    cmp [is_string], dword 0
    jnz .else
      mov eax, [msize]
      mov [total], eax
      jmp .endif
    .else:
      mov eax, [msize]
      mov ebx, 32 ;bits
      mul ebx
      mov [total], eax
    .endif:


    mov eax, 0
    .while:
      cmp eax, [total]
      jge .endwhile
      mov ebx, [src]
      add ebx, eax
      mov ecx, [dest]
      add ecx, eax
      mov edx, [ecx]
      mov [ecx], edx
      inc eax
      jmp .while
    .endwhile:

    return

__strcpy:
    ;params
    %define src    ebp+12
    %define dest   ebp+8

    ;local
    %define src_len     ebp-4
    begin 4

    addarg dword [src]
    call __strlen
    clargs 1

    mov [src_len], eax

    cmp eax, [src_len]
    jnz .end
    
    mov eax, 0
    mov edx, [dest]
    .while:
      cmp eax, [src_len]
      jge .endwhile

      mov ebx, [src]
      xor ecx, ecx
      mov cl, [ebx+eax]

      mov [edx+eax], cl

      inc eax
      jmp .while
    .endwhile:

    mov [edx+eax], byte 0

    .end:
      return

__strcmp:
    ;params
    %define left    ebp+12
    %define right   ebp+8

    ;local
    %define left_len     ebp-4
    %define l            ebp-8
    %define r            ebp-12
    begin 12

    addarg dword [left]
    call __strlen
    clargs 1

    mov [left_len], eax

    addarg dword [right]
    call __strlen
    clargs 1

    cmp eax, [left_len]
    jnz .false

    mov eax, 0
    .while:
      cmp eax, [left_len]
      jge .endwhile

      mov ebx, [left]
      xor edx, edx
      mov dl, [ebx+eax]

      mov ebx, [right]
      xor ecx, ecx
      mov cl, [ebx+eax]
      
      cmp edx, ecx
      jnz .false

      inc eax
      jmp .while
    .endwhile:

    mov eax, 1
    return

  .false
    mov eax, 0 
    return

__malloc:
    ;params
    %define size   ebp+8

    begin 0

    cmp [size], dword 0
    jnz .alloc

    mov eax, 0
    return

    .alloc:
      mov eax, __mem        ;base       +
      add eax, [__mem_ptr]  ;allocated  +
      add eax, [size]       ;requested  = new allocated chunk
  
      mov ebx,  __mem      ;base  +
      add ebx, MEMORY_SIZE ;total  = chunk limit address

      cmp eax, ebx
      jg .no_memory_left

      ;return val
      mov eax, __mem
      add eax, [__mem_ptr]
  
      ;updates __mem_ptr
      mov ebx, [__mem_ptr]
      add ebx, [size]
      mov [__mem_ptr], ebx

      return
    
   .no_memory_left:
      addarg __str_no_mem_left
      call __imprima_literal
      clargs 1
      exit 1

__leia_caractere:
    %define buffer   ebp-BUFFER_SIZE
    begin BUFFER_SIZE
    
    ;read from stdin
    lea eax, [buffer]
    addarg eax
    addarg BUFFER_SIZE
    call __readline
    clargs 2
    
    lea ebx, [buffer]
    xor eax, eax
    mov al, [ebx]

    return

__leia_real:
    %define buffer   ebp-BUFFER_SIZE    
    begin BUFFER_SIZE
    
    ;read from stdin
    lea eax, [buffer]
    addarg eax
    addarg BUFFER_SIZE
    call __readline
    clargs 2
    
    lea eax, [buffer]
    addarg eax
    call __atof
    clargs 1

    return

__leia_inteiro:
    %define buffer   ebp-BUFFER_SIZE
    begin BUFFER_SIZE
    
    ;read from stdin
    lea eax, [buffer]
    addarg eax
    addarg BUFFER_SIZE
    call __readline
    clargs 2
    
    lea eax, [buffer]
    addarg eax
    call __atoi
    clargs 1

    return

__leia_logico:
    %define zero_str ebp-4
    %define buffer   ebp-4-BUFFER_SIZE
    begin (4+BUFFER_SIZE)
    
    mov [zero_str], dword 0x00000030 ;"0\0"

    ;read from stdin
    lea eax, [buffer]
    addarg eax
    addarg BUFFER_SIZE
    call __readline
    clargs 2

    ;buffer == "falso"?
    lea eax, [buffer]
    addarg eax
    addarg __str_false
    call __strcmp
    clargs 2

    cmp eax, 1
    jz .false

    ;buffer == "0"?
    lea eax, [zero_str]
    lea ebx, [buffer]
    addarg eax
    addarg ebx
    call __strcmp
    clargs 2

    cmp eax, 1
    jz .false

    mov eax, 1
    return

    .false:
      mov eax, 0
      return

__leia_literal:
    %define string ebp-4
    begin 4

    addarg BUFFER_SIZE
    call __malloc
    clargs 1

    mov [string], eax

    addarg dword [string]
    addarg BUFFER_SIZE
    call __readline
    clargs 2

    mov eax, [string]
    return

__readline:
    ;params
    %define buffer   ebp+12
    %define size     ebp+8
    begin 0

    ;call read()
    mov eax, 3 ;the syscall "read"
    mov ebx, 0 ;the fd (stdin)
    mov ecx, [buffer]
    mov edx, [size];the length 
    int 80h

    ;removes the \n
    mov ebx, [buffer]
    mov [ebx+eax-1], byte 0

    return

