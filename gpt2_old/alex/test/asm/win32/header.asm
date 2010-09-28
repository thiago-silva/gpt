%define WINDOWS_CUI
%define ONLY_NASM

;------------- begin nagoa --------------

%define wcall call_   ; comment this line  if you do not like "call" macro 


%imacro call_ 1-*  
%assign i	%0
	%if %0 > 1
		%rep %0 - 1
			%rotate -1  
			%assign i	i-1		                               
			%rotate -1
			%ifidni %1,"ADDR_"
			%rotate 1
			lea	eax,	[%1]
			push	eax
			%rotate -1
			%assign i	i-1
			%else		
			%rotate 1
			STDPUSH {%1}     
			%endif
			%if i <=1
			%exitrep
			%endif
		%endrep		
		%rotate -1
	%endif
	%ifid %1
		%ifdef %1_definida
			;é uma  funçao local
			CALL %1
		%else
			%ifdef ONLY_NASM
				CALL [%1]
			%else
				extern %1
				CALL [%1]
			%endif
		%endif
	%else
		CALL %1
	%endif
%endmacro


%macro STDPUSH 1				; macro to push argument list
	%ifnum %1				; argument is a number
						; check number to fit in signed byte
		%if %1 >= -128 && %1 <= 127
			push byte %1
		%else				; cannot be pushed as signed byte
			push dword %1
		%endif
	%else					; argument is not a number
		%ifstr %1			; argument is a string
			CONST %%str, db %1, 0	; define macro-local label
			push dword %%str
		%else				; neither string nor number (label or register)
			push dword %1
		%endif
	%endif
%endmacro

 %imacro CONST 2+	; macro to insert constant into code
	jmp near %%skip	; jump to macro-local label
	%1 %2		; mark label and define data
	%%skip:		; macro-local label
 %endm
 
;%ifdef ONLY_NASM
  BITS 32


; .................................................................................
;                                  CONSTANTS
; .................................................................................

  %define	_NATIVE      	1
  %define	_WINDOWS_GUI 	2
  %define	_WINDOWS_CUI 	3
  %define	_POSIX_CUI   	7

  %define	PAGE		1000h
  %define	SECTOR		200h

  %define	drvbase		00010000h
  %define 	dllbase		10000000h
  %define	exebase 	00400000h

  %define	HDRSIZE		hdr_end

  %define	CODEPAGES	( ( PAGE + (code_end - code - 1) ) / PAGE )
  %define	BSSPAGES	( ( PAGE + (bss_end  - bss  - 1) ) / PAGE )

; .................................................................................

  %define     PEFILE

	; ----- Number of objects

	%ifdef	BSS
	        %define	    	OBJECTS	2
	%else
	        %define	    	OBJECTS	1
	%endif

	; ----- define image base, section alignment, file alignment,
	;       code base, data base, characteristics, image size, 
	;       CODESIZE, DATASIZE and HDRSIZE

	%ifdef DRIVER
	   %define    IMAGEBASE		drvbase
	   %define    SECTION 		20h
	   %define    FILEAL		SECTION
	   %define    CODE_BASE		HDRSIZE
	   %define    DATA_BASE		__iat_
	   %define    CHARACTERISTICS	10Eh
	   %define    IMGSIZE		end_reloc
	   %define    CODESIZE		(code_end - code)   + (end_init  - init)
	   %define    DATASIZE		(end_iat  - __iat_) + (end_reloc - _reloc)
	   %define    CODEVSIZE		code_raw_end-code
	   %define    STACKRESERVE	40000h
	   %define    SUBSYS		NATIVE
	   %define    OBJECTS	  	4
	   %define    TSCHARACTERISTICS	068000020h
	%else
	   %ifdef DLL
	   	%define	 IMAGEBASE   			dllbase
	   	%define  CHARACTERISTICS		210Eh
	   	%define  __DLL
;	   	%define  DATASIZE			(end_iat - __iat_) + (end_reloc - _reloc)
	   %else 
	   	%define  IMAGEBASE 			exebase
	   	%define  CHARACTERISTICS		103h
	   %endif

	   ; --

	   %ifdef BSS
		%ifdef DLL
	 	 %define	BCHARACTERISTICS	0D0000080h
		%else
	 	 %define	BCHARACTERISTICS	0C0000080h
		%endif
		%define		BSS_SIZE 		BSSVSIZE
	   %else
	        %define		BSS_SIZE 		0
	   %endif

	   ; --

   	   %define    IMGSIZE	  	_vsize
	   %define    CODE_BASE		PAGE
	   %define    DATA_BASE		CODE_BASE
	   %define    SECTION 		PAGE
	   %define    FILEAL		SECTOR
	   %define    CODESIZE		_data_vsize
	   %define    CODEVSIZE		CODEPAGES*1000h
	   %define    BSSVSIZE		BSSPAGES*1000h
	   %define    TSCHARACTERISTICS	0E0000020h
	   %define    DATASIZE		CODESIZE
	%endif

	; ----- Subsystem

	%ifdef WINDOWS_CUI
	   %define  SUBSYS     _WINDOWS_CUI
	%elifdef NATIVE
	   %define  SUBSYS     _NATIVE
	%else
	   %define  SUBSYS     _WINDOWS_GUI
	%endif

	; ----- Relocation

	%define    RVADIFF      CODE_BASE-HDRSIZE
	%define    reloc	RVADIFF+IMAGEBASE
	%define	   reloc_bss	(CODE_BASE+CODEVSIZE+IMAGEBASE)-(code_end-code)

; ----------------------------------------------------------------------------------
  Section_setup:
; ----------------------------------------------------------------------------------
	section	.head 	vstart=IMAGEBASE	start=0
	_head_start:

	section	.text 	vfollows=.head		follows=.head	valign=SECTION
	_text_start:

	section	.data	vfollows=.text		follows=.text	valign=1	align=1
	_data_start:

	section	.bss  	vfollows=.data		valign=SECTION
	_bss_start:

; -------------------------------------------
; rva and macro: permits declare RVAs
; -------------------------------------------
%macro	   rva	1
	   	dd 	__rva(%1)
%endmacro

;-----------------------------------------------------------------------------
; Address calculation macros:
; Thanks to S_Tec of Northern Dragons
;-----------------------------------------------------------------------------

		; Round a physical size up to a virtual size:
%define _round(size) (( size + SECTION-1 ) & ~(SECTION-1))

		; Make an RVA out of a label in the .text section:
%define __rva(label) _head_vsize + label - _text_start

;-----------------------------------------------------------------------------

	; ----- Entry point

	%ifdef DRIVER
	  %define	ENTRY	DriverEntry
	%else
	  %define	ENTRY	__rva(_start)
	%endif

	; -----

; ----------------------------------------------------------------------------------
  header:
		section	.head
		bits	32
; ----------------------------------------------------------------------------------
  mz_header:
; ----------------------------------------------------------------------------------
	.magic                  	dw 	"MZ"
	.cblp                   	dw 	90h
	.cp                     	dw 	3	
	.crlc                   	dw 	0
	.cparhdr                	dw 	4
	.minalloc               	dw 	0
	.maxalloc               	dw 	-1
	.ss                     	dw 	0
	.sp                     	dw 	0B8h
	.csum                   	dw 	0
	.ip                     	dw 	0
	.cs                     	dw 	0
	.lfarlc                 	dw 	40h
	.ovno                   	dw 	0
	.res                    	times 	4 dw 0
	.oemid                  	dw 	0
	.oeminfo                	dw 	0
	.res2                   	times 	10 dw 0
	.lfanew                 	dd 	pe_header-IMAGEBASE
; ----------------------------------------------------------------------------------
  stub:
; ----------------------------------------------------------------------------------
					CALL 	.@W
					db 	"must on win32!$"
				.@W
					pop	dx
					mov	ah, 9
					int	21h

					mov	ah, 4ch
					int	21h

					align 	16, DB 0
; ----------------------------------------------------------------------------------
  pe_header:
; ----------------------------------------------------------------------------------
	.Signature              	dd 	"PE"
	.Machine                	dw 	14Ch
	.NumberOfSections       	dw 	OBJECTS
	.TimeDateStamp          	dd 	0
	.PointerToSymbolTable   	dd 	0
	.NumberOfSymbols        	dd 	0
	.SizeOfOptionalHeader   	dw 	0E0h
	.Characteristics        	dw 	CHARACTERISTICS
; ----------------------------------------------------------------------------------
  OptionalHeader:
; ----------------------------------------------------------------------------------
	.Magic                  	dw 	10Bh
	.MajorLinkerVersion     	db 	0
	.MinorLinkerVersion     	db 	0
	.SizeOfCode             	dd 	CODESIZE	; sume of all code sections SizeOfRawData
	.SizeOfInitializedData  	dd 	DATASIZE	; size of all data sections SizeOfRawData
	.SizeOfUninitialzedData 	dd 	BSS_SIZE
	.AddressOfEntryPoint    	dd 	ENTRY
	.BaseOfCode             	dd 	CODE_BASE
	.BaseOfData             	dd 	DATA_BASE
	.ImageBase              	dd 	IMAGEBASE
	.SectionAlignment       	dd 	SECTION
	.FileAlignment          	dd 	FILEAL
	.MajorOperSystemVersion 	dw 	4
	.MinorOperSystemVersion 	dw 	0
	.MajorImageVersion      	dw 	0
	.MinorImageVersion      	dw 	0
	.MajorSubsystemVersion  	dw 	4
	.MinorSubsystemVersion  	dw 	0
	.Reserved1              	dd 	0
	.SizeOfImage            	dd 	_vsize
	.SizeOfHeaders          	dd 	_head_size
	.CheckSum               	dd 	0
	.Subsystem              	dw 	SUBSYS
	.DllCharacteristics     	dw 	0
	.SizeOfStackReserve     	dd 	100000h 
	.SizeOfStackCommit      	dd 	1000h   
	.SizeOfHeapReserve      	dd 	100000h 
	.SizeOfHeapCommit       	dd 	1000h   
	.LoaderFlags            	dd 	0
	.NumberOfRvaAndSizes    	dd 	10h
; ----------------------------------------------------------------------------------
  DataDirectories:
; ----------------------------------------------------------------------------------


	; ----- IMAGE_DIRECTORY_ENTRY_EXPORT


 	.export 		times	2 dd 0

	; ----- IMAGE_DIRECTORY_ENTRY_IMPORT -----


	.import 		dd	__rva(import), _end_import - import


	; ----- IMAGE_DIRECTORY_ENTRY_RESOURCE -----


	.rsrc			times	2 dd 0


	; ----- IMAGE_DIRECTORY_ENTRY_EXCEPTION -----


		.exception		times	2 dd 0


	; ----- IMAGE_DIRECTORY_ENTRY_SECURITY -----


		.certificate		times	2 dd 0


	; ----- IMAGE_DIRECTORY_ENTRY_BASERELOC


	.reloc			times	2 dd 0


	; -----


	.misc_sectionz  	times	10*2 dd 0

; ----------------------------------------
  SectionTables:
; ----------------------------------------

%define csize code_end - code 

text_section:
	.SectionName            	db 	".text", 0, 0, 0     
	.VirtualSize           		dd 	_data_vsize
	.VirtualAddress         	dd 	_head_vsize
	.SizeOfRawData          	dd 	_data_size
	.PointerToRawData       	dd 	_head_size
	.PointerToRelocations   	dd 	0
	.PointerToLinenumbers   	dd 	0
	.NumberOfRelocations    	dw 	0
	.NumberOfLinenumbers    	dw 	0
	.Characteristics		dd	TSCHARACTERISTICS

%ifdef BSS
bss_section:
	.SectionName            	db 	".bss", 0, 0, 0, 0
	.VirtualSize            	dd	BSSVSIZE
	.VirtualAddress         	dd 	CODEVSIZE+PAGE
	.SizeOfRawData          	dd 	0
	.PointerToRawData       	dd 	0
	.PointerToRelocations   	dd 	0
	.PointerToLinenumbers   	dd 	0
	.NumberOfRelocations    	dw 	0
	.NumberOfLinenumbers    	dw 	0
	.Characteristics        	dd 	BCHARACTERISTICS
%endif







;;;;;;;;;;;;;;------------- SIC END ----------------------;;;;;;;;;;;;


;-----------------------------------------------------------------------------
; Program end macro:
; Thanks to S_Tec of Northern Dragons
;-----------------------------------------------------------------------------

%imacro end 0
		align	FILEAL, db 0
		section	.head
		align	FILEAL, db 0
_head_size	equ	$-$$
_head_vsize	equ	_round(_head_size)
_head_end:


		section	.text
		align	4, db 0
_text_size	equ	$-$$
_text_end:


		section	.data
_data_size	equ	$-$$ + _text_size
_data_end:


		section	.bss
		alignb	SECTION
_bss_vsize	equ	$-$$
_bss_end:


_data_vsize	equ	_round(_text_size + _data_size) + _bss_vsize
_vsize		equ	_head_vsize + _data_vsize
%endmacro


		; ''''''''''''''''''''''''''''''''''''''''
		; MACROS TO DEFINE PE IMPORT TABLES      '
		; numit_or: 03/2003                      '
		; ''''''''''''''''''''''''''''''''''''''''
		; Use:                                   '
		; MODULES                                '
		;	LIBRARIES kernel32, user32       '
		; 	LIBRARIES gdi32                  '
		; ENDMOD                                 '
		;                                        '
		; BEGIN_IMPORT kernel32                  '
		;  __api_ GetModuleHandleA, ExitProcess  '
		; END_IMPORT                             '
		;                                        '
		; BEGIN_IMPORT user32                    '
		;  __api_ DialogBoxParamA, MessageBoxA   '
		;  __api_ CloseDialog                    '
		; END_IMPORT                             '
		;                                        '
		; BEGIN_IMPORT gdi32                     '
		;  __api_ BitBlt                         '
		; END_IMPORT                             '
		;                                        '
		; Nice!                                  '
		; ''''''''''''''''''''''''''''''''''''''''

; --------------------------------------------------------------------
; library macro: constructs the directory of imported modules:
; a array of IMAGE_IMPORT_DESCRIPTOR structures.
; Use two parameters:
;	· The addres where is a string with the name of the imported
;	  module.
;	· The address of a directory (The Import Address Table: IAT)
;	  that is a array whish entries are IMAGE_THUNK_DATA. Each 
;	  IMAGE_THUNK_DATA is an RVA to a IMAGE_IMPORT_BY_NAME 
; 	  structure which describes the imported function.
; --------------------------------------------------------------------
%macro	library 2
	   dd   0
           dd   0
           dd   -1
	   rva  %1
           rva  %2
%endmacro

; -----------------------------------------------------------------
; macro enlibrary: constructs a null IMAGE_IMPORT_DESCRIPTOR
; that indicates the end of the IMAGE_IMPORT_DESCRIPTOR's array.
; -----------------------------------------------------------------
%define endlibrary times 5 dd 0 

; -----------------------------------------------------------------
; macro api_import: constructs the IMAGE_THUNK_DATA that points to 
; the IMAGE_IMPORT_BY_NAME structure. Uses tree parameters:
; 	· The name of the imported function
;	· The address of the IMAGE_IMPORT_BY_NAME struct for the
;	  imported function
;	· A integer (0 or 1) that indicates if this is the last
;	  entry. Pass 1 for the last entry.
; -----------------------------------------------------------------
%macro  api_import 3
	%1 rva %2
	   %if  %3=1
		dw   0
	   %endif
%endmacro

	; -----

%macro _MOD 2-4
  %rep %0/2
    ModuleNames %1, %2
    %rotate 2
  %endrep
%endmacro

	; -----

%macro ModuleNames 2
 %1_i DB %2, 0
%endmacro



; --------------------------
; IF we use the IAT in KMDs
; --------------------------
%macro IAT 0
 raw_code_end:
 align 20h, DB 0 
 code_end:
 BEGIN_IMPORT ntoskrnl.exe
%endmacro

	; -----

%macro END_IAT 0
 dd 0
end_raw_iat:
align 4, DB 0
%ifdef _RDATA_
align 20h, DB 0
%endif
end_iat:
%undef _RDATA_
%endmacro

; -------------------------------------
; Use:
; MODULES
;  LIB kernel32.dll, "kernel32.dll"
;  LIB user32.dll, "user32.dll"
; ENDM
; -------------------------------------
%macro MODULES 0
 %ifnctx module
  ALIGN 16, DB 0
  %push module
  import:
 %endif
%endmacro

	; -----

%macro LIB 2
 %ifnctx lib
  %assign %$_z 0
  %push lib
 %endif
 %define fi_%$_z %1
 %define gi_%$_z %2
 %assign %$_z %$_z+1
%endmacro

	; -----

%macro	__LIBS_ 2-*
MODULES
	%rep	%0/2
		LIB	%1, %2
		%rotate 2
	%endrep
ENDM
%endmacro

	; -----

%macro ENDM 0
 %ifctx lib
  %define _LIB_
  %assign %$_y 0
  %rep %$_z
     LIBRARY fi_%$_y
     %assign %$_y %$_y+1
  %endrep
     times 5 DD 0
     end_import_desc:
  %assign %$_y 0
  %rep %$_z
     _MOD fi_%$_y, gi_%$_y
     %assign %$_y %$_y+1
  %endrep
  %pop
 %else
  ; %error Error using LIB macro
 %endif
 END_MOD
%endmacro

	; -----

%macro LIBRARY 1-2
 %ifdef _LIB_
   %ifctx lib
     IMPDESCRIPTOR %1
     %if %0=2
       times 5 DD 0
       end_import_desc:
     %endif
   %endif
 %else
   %ifctx module
     IMPDESCRIPTOR %1
     %if %0=2
       times 5 DD 0
       end_import_desc:
     %endif
   %endif
 %endif
%endmacro

	; -----

%macro LIBRARIES 2-3
 %ifdef _LIB_
 %ifctx lib
    %rep 2
     IMPDESCRIPTOR %1
     %rotate 1
    %endrep
    %if %0=3
     times 5 DD 0
    %endif 
   %rotate 1
 %endif
 %else
 %ifctx module
    %rep 2
     IMPDESCRIPTOR %1
     %rotate 1
    %endrep
    %if %0=3
     times 5 DD 0
    %endif 
   %rotate 1
 %endif
 %endif
%endmacro

	; -----

%macro IMPDESCRIPTOR 1
  %ifdef _DRV_
   dd   %1_c
  %else 
   dd   0
  %endif
   dd   0
   dd   0
   rva  %1_i
   rva  %1
%endmacro  

	; -----

%macro END_MOD 0
 %ifdef _LIB_
   %undef _LIB_
 %endif
 %ifdef _DRV_
   ALIGN 4, DB 0
   end_raw_init:
 %endif
 %ifctx module
   %pop
 %else
   ; %error Error using the MODULES macro
 %endif
%endmacro

; --------------------------------------------------------
; USE:
; BEGIN_IMPORT kernel32.dll
;	FUNCTION GetModuleHandleA, "GetModuleHandleA", 0
;	FUNCTION ExitProcess, "ExitProcess", 0
; ENDI
; --------------------------------------------------------
%macro BEGIN_IMPORT 1
 %ifdef _DRV_
   __iat_:
 %endif
 %push import
    %1:
%endmacro

	; -----

%macro FUNCTION 3
 %ifnctx fun
  %assign %$_z 0
  %push fun
 %endif
 %define ai_%$_z %1
 %define bi_%$_z %2
 %assign ci_%$_z %3
 %assign %$_z %$_z+1
%endmacro

	; -----

%macro ENDI 0
 %ifctx fun
  %assign %$_y 0
  %rep %$_z
   __api_ ai_%$_y
   %assign %$_y %$_y+1
  %endrep
  %assign %$_y 0
  %rep %$_z
   _IMPORT ai_%$_y, bi_%$_y, ci_%$_y
   %assign %$_y %$_y+1
  %endrep
  %pop
 %endif
END_IMPORT
%endmacro 

	; -----

%macro  __api_ 1-*
%ifctx import
  %rep %0
	%ifdef NORMAL
		%1_oo: rva %1_k
		%1 equ %1_oo+reloc
	%else
     		%1: rva %1_k
	%endif
   	%rotate 1
  %endrep
%endif
%endmacro

	; -----

%macro iat 3
	%1_k	DW	%3
		DB 	%2, 0
		ALIGN   2, DB 0
%endmacro

	; -----

%macro hints 1-*
  %rep %0
    rva %1_k
    %rotate 1
  %endrep
%endmacro

	; -----

%macro	__api_ord 2-*
%ifctx import
  %rep %0/2
    %1:	dd %2+80000000h
    %rotate 2
  %endrep
%endif
%ifndef _ORD_
  %define _ORD_
%endif
%endmacro

	; -----

%macro _IMPORT 3-*
   %ifndef endi
     DW 0
     %define endi
   %endif
  %rep %0/3
    iat %1, %2, %3
    %rotate 3
  %endrep
%endmacro

	; -----

%macro __IMPORT_ 4-*
  BEGIN_IMPORT %1
  %rotate 1
  %assign %%a %0
  %assign %%a (%%a-1)/3
  %rep %%a
	__api_ %1
	%if %0 > 4
		%rotate 3
	%endif
  %endrep
  %if %0 > 4
  	%rotate 1
  %endif
  %rep %%a
	_IMPORT %1, %2, %3
	%if %0 > 4
		%rotate 3
	%endif
  %endrep
  END_IMPORT
%endmacro

	; -----

%macro END_IMPORT 0
 %ifdef endi
  %undef endi
 %endif
 %ifdef _ORD_
   %undef _ORD
   DD 0
 %endif
 %ifctx import
  %pop
 %endif
%endmacro

	; -----

%define __BEGIN_IMPORT section .text

%macro __END_IMPORT 0
	_end_import:
	%define	import_size _end_import - import
	__SECT__
%endmacro




; :::: PE MACROS   nguga ::::::::::::::
;===========================================

%macro IMPORT_END 0-*  
_end_import:
%define	import_size _end_import - import
end
%endmacro

%define IMPORT_BEGIN section .text

%define LAST_BEGIN     section .text
%define LAST_END       IMPORT_END
