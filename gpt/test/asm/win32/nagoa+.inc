; NAGOA made by  PedroGC Aka  Nguga
;-----------------------------------------------------------------------------------
; 12 Março  2005 NAGOA+.INC        http://visual-assembler.pt.vu
;-----------------------------------------------------------------------------------
; nagoa.inc is part of NAGOA RAD IDE  
; place this file on   c:\lab\vasm\inc  
;-----------------------------------------------------------------------------------
; NAGOA made by  PedroGC Aka  Nguga
;-----------------------------------------------------------------------------------
; Greetings to everyone on that helps build NAGOA.inc   here the people  :
;=====================================================
; Sven B. Schreiber, Philippe Auphelle, Gij, Iczelion,
; Steve Hutchesson, Barry Kauler, Wayne Radburn, Kaproncai Tamás
; Anton Eremeev ,nuMIT_or , David Arnoldo Ortiz Lozano ,
; mastercpp , Luckystar , Frank Barret Kotler , Vecna , William Swanson ,highenergy
;-----------------------------------------------------------------------------------

;======================================================
;  CALL is real assembly instruction "call" is a macro but if you do not like use instead
;  call_ or invoke or stdcall  and then   CALL = call  assembly instruction , 
;  for that just comment this line above .
;-------------------------------------------------------------------------------------
%define wcall call_   ; comment this line  if you do not like "call" macro 
;-------------------------------------------------------------------------------------
;======================================================




;---------------------------------------------------------------------------------------

%idefine elif		elseif
%idefine nasm		call_
%idefine stdcall	                  call_
%idefine invoke		call_
%idefine local		stack
%idefine addr_		"ADDR_",
%idefine addr		"ADDR_",
%define false		0
%define true		1
; ----------------------------------------------------------------------------------
;
; 			(c) numit_or 2002-2004: numit_or@cantv.net
; 		 	http://mipagina.cantv.net/numator/
;
; 			Thanks a lot to:
;				vecna
;				blaz
;				DKZ DKZ
;				S_Tec of Northern Dragons
;
; ----------------------------------------------------------------------------------
;   edit for NAGOA by PedroGC    22-fev-2004
;-----------------------------------------------------------------------------------

%ifdef ONLY_NASM

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
	  %define	ENTRY	__rva(__start)
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


	%ifdef DLL
		.export			dd	__rva(export), import - export
	%else
	 	.export 		times	2 dd 0
	%endif


	; ----- IMAGE_DIRECTORY_ENTRY_IMPORT -----


	%ifdef DRIVER
		.import			dd	__rva(import), end_import_desc - import
	%else
		.import 		dd	__rva(import), _end_import - import
	%endif


	; ----- IMAGE_DIRECTORY_ENTRY_RESOURCE -----


	%ifdef RSRC
		.rsrc			dd	__rva(resources), import - resources
		; %ifndef RESOURCE
		;	%include "\lab\vasm\inc\resource.inc"
		; %endif
	%else
		.rsrc			times	2 dd 0
	%endif


	; ----- IMAGE_DIRECTORY_ENTRY_EXCEPTION -----


		.exception		times	2 dd 0


	; ----- IMAGE_DIRECTORY_ENTRY_SECURITY -----


		.certificate		times	2 dd 0


	; ----- IMAGE_DIRECTORY_ENTRY_BASERELOC


	%ifdef DRIVER
		.reloc			dd	_reloc, end_raw_reloc1 - _reloc
;	%elifdef DLL
;		.reloc			dd	_reloc + RVADIFF, end_raw_reloc1 - _reloc
	%else
		.reloc			times	2 dd 0
	%endif


	; -----


	%ifdef	DRIVER
		.misc_section1		times	6*2 dd 0
		.iat			dd	__iat_, end_raw_iat - __iat_
		.misc_section2		times 	3*2 dd 0
	%else
		.misc_sectionz  	times	10*2 dd 0
	%endif

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

%ifdef DRIVER
 rdata_section:
	.SectionName            	db 	".rdata", 0, 0
	.VirtualSize            	dd 	end_raw_iat - __iat_
	.VirtualAddress         	dd 	__iat_
	.SizeOfRawData          	dd 	end_iat - __iat_
	.PointerToRawData       	dd 	__iat_
	.PointerToRelocations   	dd 	0
	.PointerToLinenumbers   	dd 	0
	.NumberOfRelocations    	dw 	0
	.NumberOfLinenumbers    	dw 	0
	.Characteristics        	dd 	048000040h

init_section:
	.SectionName            	db 	"INIT", 0, 0, 0, 0
	.VirtualSize            	dd 	end_raw_init - init
	.VirtualAddress         	dd 	init
	.SizeOfRawData          	dd 	end_init - init
	.PointerToRawData       	dd 	init
	.PointerToRelocations   	dd 	0
	.PointerToLinenumbers   	dd 	0
	.NumberOfRelocations    	dw 	0
	.NumberOfLinenumbers    	dw 	0
	.Characteristics        	dd 	0E2000020h

reloc_section:
	.SectionName            	db 	".reloc", 0, 0
	.VirtualSize           		dd 	end_raw_reloc2 - _reloc
	.VirtualAddress         	dd 	_reloc
	.SizeOfRawData          	dd 	end_reloc - _reloc
	.PointerToRawData       	dd 	_reloc
	.PointerToRelocations   	dd 	0
	.PointerToLinenumbers   	dd 	0
	.NumberOfRelocations    	dw 	0
	.NumberOfLinenumbers    	dw 	0
	.Characteristics        	dd 	042000040h
%endif


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

	; -----

%define _RDATA_
%assign __relocs 0
%assign _mrelocs 0
%assign _nrelocs 0
%assign _orelocs 0
%assign _prelocs 0
%assign _qrelocs 0

	; -----


%macro _INIT_ 0
	init:
%endmacro

	; -----

%macro _DATA_ 0
	align	16, DB 0
	data:
%endmacro

	; -----

%macro __BSS 0
	raw_code_end:
	align	200h, DB 0
	code_end:

	%ifdef	__DLL
 		%undef __DLL
		__RELOC_
	%endif

	%define _BSS_
	section .bss
	bss:
%endmacro

	; -----

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

; --------------------------------------------------------------------
; apicall macro: permits to do direct calls to api functions
; --------------------------------------------------------------------
%macro     apicall 1
%ifdef _DRV_
	%assign __relocs __relocs+1
	__add_ %%tag, ..@rels, __relocs
	%%tag:
%elifdef __DLL
	%assign __relocs __relocs+1
	__add_ %%tag, ..@rels, __relocs
	%%tag:
%endif
	%ifndef NORMAL
	   CALL dword [%1+IMAGEBASE+RVADIFF]
	%endif
%endmacro

%macro __add_ 3
 %define %1 %{2}%3
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


; <----------------------------- MACROS FOR RESOURCES -------------------------------->

; --------------------------------------------------------------------------
; USE:
; RESOURCES MENU, m, DIALOG, n		; m is the number of menus
;                                       ; n is the number of dialogs
; BEGIN_RES_MENU
;    RES_MENU IDM_K
;    ...
;    RES_MENU IDM_L
; END_RES_MENU
;
; BEGIN_RES_DIALOG
;    RES_DIALOG IDD_K
;    ...
;    RES_DIALOG IDD_L
; END_RES_DIALOG
;
; BEGIN_RES_BITMAP
;    RES_BITMAP IDD_K
;    ...
;    RES_BITMAP IDD_L
; END_RES_BITMAP
;
; -------------------------------------------------------------------------
; Resource Types EQUs
; -----------------------------------------------------------------

 %ifndef WIN32N
	%define	DIFFERENCE EQU 11

	%define	RT_CURSOR 1
	%define	RT_BITMAP 2
	%define	RT_ICON 3
	%define	RT_MENU 4
	%define	RT_DIALOG 5
	%define	RT_STRING 6
	%define	RT_FONTDIR 7
	%define	RT_FONT 8
	%define	RT_ACCELERATOR 9
	%define	RT_RCDATA 10
	%define	RT_MESSAGETABLE 11
	%define	RT_GROUP_CURSOR RT_CURSOR + DIFFERENCE
	%define	RT_GROUP_ICON RT_ICON + DIFFERENCE
	%define	RT_VERSION 16
	%define	RT_DLGINCLUDE 17
	%define	RT_PLUGPLAY 19
	%define	RT_VXD 20
	%define	RT_ANICURSOR 21
	%define	RT_ANIICON 22
 %endif
	%define	BMP	RT_BITMAP    ; was BITMAP conflit with STRUC ..
	%define	ICON	RT_ICON
	%define	MENU	RT_MENU
	%define	DIALOG	RT_DIALOG
	%define	GROUP_ICON RT_GROUP_ICON

; -----------------------------------------------------------------
; Type of control
; -----------------------------------------------------------------
    %define    BUTTON              0x80 
    %define    EDIT                0x81 
    %define    STATIC              0x82 
    %define    LISTBOX             0x83 
    %define    SCROLLBAR           0x84 
    %define    COMBOBOX            0x85
; -----------------------------------------------------------------
; Equs for menus
; -----------------------------------------------------------------
    %define POPUP		0x0010
    %define MENUBREAK       	0x0040
    %define ENDMENU		0x0080
    %define DS_SETFONT		0x0040
; -----------------------------------------------------------------
; macro RESOURCE_DIRECTORY:
; -----------------------------------------------------------------
%macro	RESOURCE_DIRECTORY 1
	DD 0
	DD 0
	DW 0
	DW 0
	DW 0
	DW %1
%endmacro
; -----------------------------------------------------------------
; macro RESOURCE_DIRECTORY_ENTRY:
; -----------------------------------------------------------------
%macro	RESOURCE_DIRECTORY_ENTRY 2-3
	DD	%1
	%if %0=2
		DD	(%2-resources)+0x80000000
	%else
		DD	%2-resources
	%endif
%endmacro
; -----------------------------------------------------------------
; macro RESOURCE_DATA_ENTRY:
; -----------------------------------------------------------------
%macro RESOURCE_DATA_ENTRY 2
	rva	%1
	DD	%2
	DD	0
	DD	0
%endmacro
; -----------------------------------------------------------------
; RESOURCES macro:
; -----------------------------------------------------------------
%macro	RESOURCES 2-*
ALIGN 16, DB 0
resources:
	RESOURCE_DIRECTORY %0/2
	%assign _a 0
	%assign _b 0
	%assign _c 0
	%assign _d 0
	%assign _e 0
	%assign _f 0
	%assign _x 0
	%rep %0/2
		%if %1=4
			%assign _a %2
		%elif %1=5
			%assign _b %2
		%elif %1=2
			%assign _c %2
		%elif %1=3
			%assign _d %2
		%elif %1=11+3
			%assign _e %2
		%elif %1=1
			%assign _f %2
		%endif
		%rotate 2
	%endrep
	%if _f>0
		RESOURCE_DIRECTORY_ENTRY 1, subdir_cursor
	%endif
	%if _c>0
		RESOURCE_DIRECTORY_ENTRY 2, subdir_bitmap
	%endif
	%if _d>0
		RESOURCE_DIRECTORY_ENTRY 3, subdir_icon
	%endif
	%if _a>0
		RESOURCE_DIRECTORY_ENTRY 4, subdir_menu
	%endif
	%if _b>0
		RESOURCE_DIRECTORY_ENTRY 5, subdir_dialog
	%endif
	%if _e>0
		RESOURCE_DIRECTORY_ENTRY 11+3, subdir_grpicon
	%endif
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_CURSOR macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_CURSOR 0
 subdir_cursor:
 %push res_cursor
 %assign %$_c 0
 RESOURCE_DIRECTORY _f
%endmacro
; -----------------------------------------------------------------
; RES_CURSOR macro
; -----------------------------------------------------------------
%macro  RES_CURSOR 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, rcursor_%$_c
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_BITMAP macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_BITMAP 0
 subdir_bitmap:
 %push res_bitmap
 %assign %$_c 0
 RESOURCE_DIRECTORY _c
%endmacro
; -----------------------------------------------------------------
; RES_BITMAP macro
; -----------------------------------------------------------------
%macro  RES_BITMAP 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, rbitmap_%$_c
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_ICON macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_ICON 0
 subdir_icon:
 %push res_icon
 %assign %$_c 0
 RESOURCE_DIRECTORY _d
%endmacro
; -----------------------------------------------------------------
; RES_ICON macro
; -----------------------------------------------------------------
%macro  RES_ICON 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, ricon_%$_c
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_MENU macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_MENU 0
 subdir_menu:
 %push res_menu
 %assign %$_c 0
 RESOURCE_DIRECTORY _a
%endmacro
; -----------------------------------------------------------------
; RES_MENU macro
; -----------------------------------------------------------------
%macro  RES_MENU 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, rmenu_%$_c
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_DIALOG macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_DIALOG 0
 subdir_dialog:
 %push res_dialog
 RESOURCE_DIRECTORY _b
 %assign %$_c 0
%endmacro
; -----------------------------------------------------------------
; RES_DIALOG macro
; -----------------------------------------------------------------
%macro	RES_DIALOG 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, rdialog_%$_c
%endmacro
; -----------------------------------------------------------------
; BEGIN_RES_GROUP_ICON macro
; -----------------------------------------------------------------
%macro	BEGIN_RES_GROUP_ICON 0
 subdir_grpicon:
 %push res_grpicon
 %assign %$_c 0
 RESOURCE_DIRECTORY _e
%endmacro
; -----------------------------------------------------------------
; RES_GROUP_ICON macro
; -----------------------------------------------------------------
%macro  RES_GROUP_ICON 1				; Parameter is the res ID
	%assign	%$_c %$_c+1
	RESOURCE_DIRECTORY_ENTRY %1, rgrpicon_%$_c
%endmacro
; -----------------------------------------------------------------
; END_RES_CURSOR  macro
; -----------------------------------------------------------------
%macro	END_RES_CURSOR 0
	%assign %$_c 1
	%rep _f
 	rcursor_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A,  rcursor_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _f
	rcursor_data_%$_c:
		RESOURCE_DATA_ENTRY cursor_%$_c, end_cursor_%$_c-cursor_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; END_RES_BITMAP  macro
; -----------------------------------------------------------------
%macro	END_RES_BITMAP 0
	%assign %$_c 1
	%rep _c
 	rbitmap_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A,  rbitmap_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _c
	rbitmap_data_%$_c:
		RESOURCE_DATA_ENTRY bitmap_%$_c, end_bitmap_%$_c-bitmap_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; END_RES_ICON  macro
; -----------------------------------------------------------------
%macro	END_RES_ICON 0
	%assign %$_c 1
	%rep _d
 	ricon_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A,  ricon_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _d
	ricon_data_%$_c:
		RESOURCE_DATA_ENTRY icon_%$_c, end_icon_%$_c-icon_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; END_RES_MENU  macro
; -----------------------------------------------------------------
%macro	END_RES_MENU 0
	%assign %$_c 1
	%rep _a
 	rmenu_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A,  rmenu_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _a
	rmenu_data_%$_c:
		RESOURCE_DATA_ENTRY menu_%$_c, end_menu_%$_c-menu_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; END_RES_DIALOG macro
; -----------------------------------------------------------------
%macro	END_RES_DIALOG 0
	%assign %$_c 1
	%rep _b
	rdialog_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A, rdialog_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _b
	rdialog_data_%$_c:
		RESOURCE_DATA_ENTRY dlg_%$_c, end_dlg_%$_c-dlg_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; END_RES_GROUP_ICON  macro
; -----------------------------------------------------------------
%macro	END_RES_GROUP_ICON 0
	%assign %$_c 1
	%rep _e
 	rgrpicon_%$_c:
		RESOURCE_DIRECTORY 1
		RESOURCE_DIRECTORY_ENTRY 0x040A,  rgrpicon_data_%$_c, 0
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%rep _e
	rgrpicon_data_%$_c:
		RESOURCE_DATA_ENTRY grpicon_%$_c, end_grpicon_%$_c-grpicon_%$_c
		%assign	%$_c %$_c+1
	%endrep
	%assign %$_c 1
	%pop
%endmacro
; -----------------------------------------------------------------
; macro RESOURCE_DIR_STRING_U:
; -----------------------------------------------------------------
%macro RESOURCE_DIR_STRING_U 1
	DB	%1, 0
%endmacro
; -----------------------------------------------------------------
; macro _dialog_first_pars_
; -----------------------------------------------------------------
%macro	_dialog_first_pars_ 7
	%%lStyle		DD  %1 
	%%lExtendedStyle	DD  %2    	; new for NT 
	%%NumberOfItems		DW  %3 

	%%x			DW   %4 
	%%y			DW   %5

	%%cx			DW   %6
	%%cy			DW   %7
%endmacro

; ---------------------------------------------------------------
; macro BEGIN_DIALOG 
; ---------------------------------------------------------------
;
; ---------------------------------------------------------------
; Sintax to define dialogs
; BEGIN_DIALOG Style, ExtendedStyle, NumberOfItems, x, y, cx, cy
;     CAPTION "Caption Text"
;     FONT    size, "Font Name"
; END_DIALOG
;
; NumberOfItems is the number of controls (Buttons, Edit, etc.)
;
; All parameters are needed, except FONT
; BEGIN_DIALOG, CAPTION, FONT, END_DIALOG, are case sensitive
; --------------------------------------------------------------
%macro BEGIN_DIALOG 8
	%push dialog
	_dialog_first_pars_ %1, %2, %3, %4, %5, %6, %7
	%%MenuName		DW  %8
	%%ClassName		DW   0
	
	%%szCaption
	%IFNDEF		CAPTION
		%DEFINE		CAPTION DO_UNICODEZ
	%ENDIF

	%IFNDEF		Font
		%DEFINE		Font DO_UNICODEZ
	%ENDIF
%endmacro
; ---------------------------------------------------------------
; macro END_DIALOG
; ---------------------------------------------------------------
%macro END_DIALOG 0
	%ifctx dialog
		%UNDEF CAPTION
		%UNDEF Font
		%pop
	%endif
%endmacro
; ---------------------------------------------------------------
; macro FontSize
; ---------------------------------------------------------------
%macro FontSize 1
	%ifctx dialog
	  %%FontPt	DW   %1
	%endif
%endmacro
; ---------------------------------------------------------------
; macro FontSize
; ---------------------------------------------------------------
%macro FONT 2
	%ifctx dialog
	 FontSize %1
	 Font %2
	%endif
%endmacro
; ---------------------------------------------------------------
; macro BeginControl
; ---------------------------------------------------------------

; -- NOTE -------------------------------------------------------
; This macro use DO_UNICODEZ macro in madcros.inc. If you don't
; include madcros.inc before, this macro will not work :(
; ---------------------------------------------------------------
%DEFINE		UTEXT   DO_UNICODEZ
; ---------------------------------------------------------------
; Sintaxis for controls
; ---------------------------------------------------------------
; BeginControl Style, ExtendedStyle, x, y, cx, cy, ID, Class
;	UTEXT   "The text"
; EndControl
;
; If you won't use text:
; BeginControl
;	NOTEXT
; EndControl
; --------------------------------------------------------------
%macro BeginControl	8
ALIGN 4, DB 0
  %%lStyle		DD	%1
  %%lExtendedStyle	DD 	%2
  %%x			DW	%3
  %%y			DW	%4 
  %%cx			DW  	%5
  %%cy			DW  	%6
  %%wId			DW 	%7
  %%ClassId		DW	-1
			DW      %8
%endmacro
; --------------------------------------------------------------
 %DEFINE		NOTEXT  DW 0
; -----------------------------------------------------------------
; macro EndControl 
; -----------------------------------------------------------------
%macro EndControl	0
	%%nExtraStuff		DW 	0
%endmacro
; -----------------------------------------------------------------
; macro MenuHeader
; -----------------------------------------------------------------
%define BeginOfMenu 	DW	0, 0
; -----------------------------------------------------------------
; macro PopupMenuItem
; -----------------------------------------------------------------
%macro PopupMenuItem	1-2
  %if %0=2
   %if %2=0
    %%fItemFlags	DW	90h
   %endif
  %else
    %%fItemFlags	DW	POPUP
  %endif
  %%szItemText
  UTEXT			%1
%endmacro
; -----------------------------------------------------------------
; macro NormalMenuItem
; -----------------------------------------------------------------
%macro NormalMenuItem 	3+
  %%fItemFlags		DW	%1	; 
  %%wMenuID		DW	%2	; 
  %%szItemText
  %ifstr %3
   UTEXT		%3
  %else
   DW 0
  %endif
%endmacro
; -----------------------------------------------------------------


; ----------------------------------------------------------
; MACROS to build DLLs using NASM without linker
;
; USE:
;
; NAME "DLL.DLL"
;
; BEGIN_EXPORT n
;   EXPORT_FUNCTIONS funct1, funct2, funct3
;   EXPORT_NAMES     funct1, funct2, funct3
;   EXPORT_STRINGS   funct1, "funct1", funct2, "funct2", funct3, "funct3"
; END_EXPORT
;
; where n is the number of exported functions
;
; (c) numit_or 2003
; numit_or@cantv.net
; http://mipagina.cantv.net
; -------------------------------------------------------


%macro NAME 1
 %ifnstr %1
  ; %error The module name must to be between " "
 %else
  %DEFINE name %1
 %endif
%endmacro

	; -----

%macro BEGIN_EXPORT 1
 GLOBAL export
ALIGN 16, DB 0
 export:
  .Characteristics           DD      0
  .TimeDateStamp             DD      0
  .MajorVersion              DW      0
  .MinorVersion              DW      0
  .nName                     DD      pName-IMAGEBASE
  .nBase                     DD      1
  .NumberOfFunctions         DD      %1
  .NumberOfNames             DD      %1
  .AddressOfFunctions        DD      functions-IMAGEBASE
  .AddressOfNames            DD      names-IMAGEBASE
  .AddressOfNameOrdinals     DD      ordinals-IMAGEBASE
  %define _j %1
  names:
 %push export
%endmacro

	; -----

%macro EXPORT_FUNCTIONS 1-*
 %ifctx export
   %rep %0
     DD %1_u-IMAGEBASE
     %rotate 1
   %endrep
 %endif
%endmacro

	; -----

%macro EXPORT_NAMES 1-*
 %ifnctx export_names
   functions:
   %push export_names
 %endif
 %rep %0
    DD %1-IMAGEBASE
    %rotate 1
 %endrep
%endmacro

	; -----

%macro EXPORT_STRINGS 2-*
    %ifctx export_names
      %pop
    %endif
    %rep %0/2
     %ifnstr %2
       ; %error The module name must to be betwen " "
       %exitrep
     %else
	%1_u:
	DB %2, 0
     %endif
     %rotate 2
    %endrep
%endmacro

	; -----

%macro ORDINALS 1
 %ifctx export
  ordinals:
  DW 0
  %assign _i 1
  %rep %1-1
	DW _i
	%assign _i _i+1
  %endrep
 %endif
 %assign _i 0
%endmacro

	; -----

%macro END_EXPORT 0
 %ifctx export
	ORDINALS _j
	%pop
 %endif
 %assign _j 0
pName:
 DB name, 0
 %undef name
%endmacro

	; -----

%macro __EXPORTS_ 3-*
NAME	%1

%assign %%a %0-1

	; -----

BEGIN_EXPORT %%a/2
%rotate 1
%rep %%a/2
	EXPORT_FUNCTIONS %1
	%rotate 2
%endrep

%rotate 1
%rep %%a/2
	EXPORT_NAMES %1
	%rotate 2
%endrep

%rotate 1
%rep %%a/2
	EXPORT_STRINGS %1, %2
	%rotate 2
%endrep
END_EXPORT
%endmacro


; --------------- Macros for .reloc section ------------------------
;
; use: __RELOC_
;

struc IMAGE_BASE_RELOCATION
  .VirtualAddress resd 1
  .SizeOfBlock    resd 1
endstruc


%macro __RELOC_ 0
;%ifdef PE_
 _reloc:
%ifdef _DRV_
   .VirtualAddress dd 0
%else
   .VirtualAddress dd 1000h
%endif
   .SizeOfBlock    dd ((__relocs+_mrelocs+_nrelocs+_orelocs+_prelocs+_qrelocs)*2)+IMAGE_BASE_RELOCATION_size
 trailing_relocations:
   set_relocs __relocs
   %ifdef _movdr_
       set_mrelocs _mrelocs
       %undef _movdr_
   %endif
   %ifdef _rmov_
       set_nrelocs _nrelocs
       %undef _rmov_
   %endif
   %ifdef _rpush_
   set_orelocs _orelocs
       %undef _rpush_
   %endif
   %ifdef _rpop_
       set_prelocs _prelocs
       %undef _rpop_
   %endif
   %ifdef _rlea_
       set_qrelocs _qrelocs
       %undef _rlea_
   %endif
 end_raw_reloc1:
   align 10h, DB 0
 end_raw_reloc2:
   align 20h, DB 0
 end_reloc
;%endif
%endmacro

	; -----

%macro set_relocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@rels, %%_i
   setrel %%tag, 2
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro set_mrelocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@mrels, %%_i
   setrel %%tag, 3			; <-- opcode size: movdr 
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro set_nrelocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@nrels, %%_i
   setrel %%tag, 1			; <-- opcode size: rmov 
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro set_orelocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@orels, %%_i
   setrel %%tag, 2			; <-- opcode size: rpush 
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro set_prelocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@prels, %%_i
   setrel %%tag, 2			; <-- opcode size: rpop  
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro set_qrelocs 1
  %assign %%_i 1
  %rep %1
   __add_ %%tag, ..@qrels, %%_i
   setrel %%tag, 2			; <-- opcode size: relea  
   %assign %%_i %%_i+1
  %endrep
%endmacro

	; -----

%macro setrel 2
   DW %1+%2+3000h
%endmacro



; --------------- Additional DLL macros ----------------------------

%macro DllEntry 3
 %if %0!=3
   ; %error The number of parameters does not match. Must to be three.
 %else
   __start:
   PROC _entry@3, %1, %2, %3
 %endif
%endmacro

	; -----

%macro EndDllEntry 0
  ENDP
%endmacro


; ----------- macros to reference  global data -----------------

%define		UNICODE_STRING_SIZE 8

; mov from a global relative address to a absolute address

 %MACRO movdr 2
   %ifndef _movdr_
	%define _movdr_
   %endif

  %ifdef PE_
   %assign _mrelocs _mrelocs+1
   __add_ %%tag, ..@mrels, _mrelocs
   %%tag:
   movd %1, %2+IMAGEBASE
  %elifdef __DLL
   %assign _mrelocs _mrelocs+1
   __add_ %%tag, ..@mrels, _mrelocs
   %%tag:
   movd %1, %2
  %else
   movd %1, %2
  %endif
 %ENDMACRO

	; -----

; mov from a global relative address to a register

%macro rmov 2
   %ifndef _rmov_
	%define _rmov_
   %endif

   %ifdef PE_
     %assign _nrelocs _nrelocs+1
     __add_ %%tag, ..@nrels, _nrelocs
     %%tag:
       mov %1, %2+IMAGEBASE
   %elifdef __DLL
     %assign _nrelocs _nrelocs+1
     __add_ %%tag, ..@nrels, _nrelocs
     %%tag:
       mov %1, %2
   %else
     Mov %1, %2
   %endif
%endmacro

	; -----

%macro rpush 1
   %ifndef _rpush_
	%define _rpush_
   %endif

   %ifdef PE_
     %assign _orelocs _orelocs+1
     __add_ %%tag, ..@orels, _orelocs
     %%tag:
       push %1+IMAGEBASE
   %elifdef __DLL
     %assign _orelocs _orelocs+1
     __add_ %%tag, ..@orels, _orelocs
     %%tag:
       push dword %1
   %else
     Push dword %1
   %endif
%endmacro

	; -----

%macro rpop 1
   %ifndef _rpop_
	%define _rpop_
   %endif

   %ifdef PE_
     %assign _prelocs _prelocs+1
     __add_ %%tag, ..@prels, _prelocs
     %%tag:
        pop %1+IMAGEBASE
  %elifdef __DLL
     %assign _prelocs _prelocs+1
     __add_ %%tag, ..@prels, _prelocs
     %%tag:
        pop dword %1
  %else
     Pop dword %1
   %endif
%endmacro

	; -----

%macro rlea 2
   %ifndef _rlea_
	%define _rlea_
   %endif

   %ifdef PE_
     %assign _qrelocs _qrelocs+1
     __add_ %%tag, ..@qrels, _qrelocs
     %%tag:
        Lea %1, %2+IMAGEBASE
  %elifdef __DLL
     %assign _qrelocs _qrelocs+1
     __add_ %%tag, ..@qrels, _qrelocs
     %%tag:
        Lea %1, %2
  %else
     Lea dword %1, %2
   %endif
%endmacro

	; -----

%macro is_reg 2
   %ifidn   %1, eax
	    %%assign %2 1
   %elifidn %1, ebx
	    %%assign %2 1
   %elifidn %1, ecx
	    %%assign %2 1
   %elifidn %1, edx
	    %%assign %2 1
   %elifidn %1, esi
	    %%assign %2 1
   %elifidn %1, edi
	    %%assign %2 1
   %elifidn %1, ebp
	    %%assign %2 1
   %elifidn %1, esp
	    %%assign %2 1
   %elifidn %1, eip
	    %%assign %2 1
   %else
	    %%assign %2 0
   %endif
%endmacro


%ifdef _DRV_
 %define _bss(a) a
 %define _data(a) a
%else
 %define _bss(a)  reloc_bss+a
 %define _data(a) reloc+a
%endif

;%define	addr(a) {dword a+reloc}

%define _code(a) _data(a)
%define ofs(a) _code(a)

%macro  pPointer 1+
	push dword (IMAGEBASE+RVADIFF)+%1
%endmacro

; -----------------------------------------------------




%endif




; ------------------- Resource Compiler Header File -------------------- 

;  This file supplies the constants used by the resource compiler for
;         various 32 bit resource components in .RC script files. 

; ---------------------------------------------------------------------- 



%DEFINE RESOURCE

%ifndef WIN32N
    %define DIFFERENCE	    	11

    %define RT_CURSOR	    	1
    %define RT_BITMAP	    	2
    %define RT_ICON	    	3
    %define RT_MENU	    	4
    %define RT_DIALOG	    	5
    %define RT_STRING	    	6
    %define RT_FONTDIR	    	7
    %define RT_FONT	    	8
    %define RT_ACCELERATOR	9
    %define RT_RCDATA	    	10
    %define RT_MESSAGETABLE	11
    %define RT_GROUP_CURSOR	RT_CURSOR + DIFFERENCE
    %define RT_GROUP_ICON	RT_ICON + DIFFERENCE
    %define RT_VERSION	    	16
    %define RT_DLGINCLUDE	17
    %define RT_PLUGPLAY	    	19
    %define RT_VXD	    	20
    %define RT_ANICURSOR	21
    %define RT_ANIICON	    	22
%endif

    %define WM_DDE_FIRST	    	0x03E0
    %define WM_DDE_INITIATE     	(WM_DDE_FIRST)
    %define WM_DDE_TERMINATE    	(WM_DDE_FIRST+1)
    %define WM_DDE_ADVISE	    	(WM_DDE_FIRST+2)
    %define WM_DDE_UNADVISE     	(WM_DDE_FIRST+3)
    %define WM_DDE_ACK          	(WM_DDE_FIRST+4)
    %define WM_DDE_DATA         	(WM_DDE_FIRST+5)
    %define WM_DDE_REQUEST		(WM_DDE_FIRST+6)
    %define WM_DDE_POKE         	(WM_DDE_FIRST+7)
    %define WM_DDE_EXECUTE	    	(WM_DDE_FIRST+8)
    %define WM_DDE_LAST         	(WM_DDE_FIRST+8)
    
    %define HDS_HORZ                0x0000
    %define HDS_BUTTONS             0x0002
    %define HDS_HOTTRACK            0x0004
    %define HDS_HIDDEN              0x0008
    %define HDS_DRAGDROP            0x0040
    %define HDS_FULLDRAG            0x0080
    %define RBS_TOOLTIPS            0x0100
    %define RBS_VARHEIGHT           0x0200
    %define RBS_BANDBORDERS         0x0400
    %define RBS_FIXEDORDER          0x0800
    %define RBS_REGISTERDROP        0x1000
    %define RBS_AUTOSIZE            0x2000
    %define RBS_VERTICALGRIPPER     0x4000
    %define RBS_DBLCLKTOGGLE        0x8000
    
    %define TTS_ALWAYSTIP           0x01
    %define TTS_NOPREFIX            0x02
    
    %define SBARS_SIZEGRIP          0x0100
    
    %define TBS_AUTOTICKS           0x0001
    %define TBS_VERT                0x0002
    %define TBS_HORZ                0x0000
    %define TBS_TOP                 0x0004
    %define TBS_BOTTOM              0x0000
    %define TBS_LEFT                0x0004
    %define TBS_RIGHT               0x0000
    %define TBS_BOTH                0x0008
    %define TBS_NOTICKS             0x0010
    %define TBS_ENABLESELRANGE      0x0020
    %define TBS_FIXEDLENGTH         0x0040
    %define TBS_NOTHUMB             0x0080
    %define TBS_TOOLTIPS            0x0100
    
    %define UDS_WRAP                0x0001
    %define UDS_SETBUDDYINT         0x0002
    %define UDS_ALIGNRIGHT          0x0004
    %define UDS_ALIGNLEFT           0x0008
    %define UDS_AUTOBUDDY           0x0010
    %define UDS_ARROWKEYS           0x0020
    %define UDS_HORZ                0x0040
    %define UDS_NOTHOUSANDS         0x0080
    %define UDS_HOTTRACK            0x0100
    
    %define PBS_SMOOTH              0x01
    %define PBS_VERTICAL            0x04
    
; --------------------- Common Control Styles ------------------------- 

    %define CCS_TOP                 0x00000001
    %define CCS_NOMOVEY             0x00000002
    %define CCS_BOTTOM              0x00000003
    %define CCS_NORESIZE            0x00000004
    %define CCS_NOPARENTALIGN       0x00000008
    %define CCS_ADJUSTABLE          0x00000020
    %define CCS_NODIVIDER           0x00000040
    %define CCS_VERT                0x00000080
    %define CCS_LEFT                (CCS_VERT | CCS_TOP)
    %define CCS_RIGHT               (CCS_VERT | CCS_BOTTOM)
    %define CCS_NOMOVEX             (CCS_VERT | CCS_NOMOVEY)
    
    %define LVS_ICON                0x0000
    %define LVS_REPORT              0x0001
    %define LVS_SMALLICON           0x0002
    %define LVS_LIST                0x0003
    %define LVS_TYPEMASK            0x0003
    %define LVS_SINGLESEL           0x0004
    %define LVS_SHOWSELALWAYS       0x0008
    %define LVS_SORTASCENDING       0x0010
    %define LVS_SORTDESCENDING      0x0020
    %define LVS_SHAREIMAGELISTS     0x0040
    %define LVS_NOLABELWRAP         0x0080
    %define LVS_AUTOARRANGE         0x0100
    %define LVS_EDITLABELS          0x0200
    %define LVS_OWNERDATA           0x1000
    %define LVS_NOSCROLL            0x2000
    %define LVS_TYPESTYLEMASK       0xfc00
    %define LVS_ALIGNTOP            0x0000
    %define LVS_ALIGNLEFT           0x0800
    %define LVS_ALIGNMASK           0x0c00
    %define LVS_OWNERDRAWFIXED      0x0400
    %define LVS_NOCOLUMNHEADER      0x4000
    %define LVS_NOSORTHEADER        0x8000
    %define TVS_HASBUTTONS          0x0001
    %define TVS_HASLINES            0x0002
    %define TVS_LINESATROOT         0x0004
    %define TVS_EDITLABELS          0x0008
    %define TVS_DISABLEDRAGDROP     0x0010
    %define TVS_SHOWSELALWAYS       0x0020
    %define TVS_RTLREADING          0x0040
    %define TVS_NOTOOLTIPS          0x0080
    %define TVS_CHECKBOXES          0x0100
    %define TVS_TRACKSELECT         0x0200
    %define TVS_SINGLEEXPAND        0x0400
    %define TVS_INFOTIP             0x0800
    %define TVS_FULLROWSELECT       0x1000
    %define TVS_NOSCROLL            0x2000
    %define TVS_NONEVENHEIGHT       0x4000
    
    %define TCS_SCROLLOPPOSITE      0x0001
    %define TCS_BOTTOM              0x0002
    %define TCS_RIGHT               0x0002
    %define TCS_MULTISELECT         0x0004
    %define TCS_FLATBUTTONS         0x0008
    %define TCS_FORCEICONLEFT       0x0010
    %define TCS_FORCELABELLEFT      0x0020
    %define TCS_HOTTRACK            0x0040
    %define TCS_VERTICAL            0x0080
    %define TCS_TABS                0x0000
    %define TCS_BUTTONS             0x0100
    %define TCS_SINGLELINE          0x0000
    %define TCS_MULTILINE           0x0200
    %define TCS_RIGHTJUSTIFY        0x0000
    %define TCS_FIXEDWIDTH          0x0400
    %define TCS_RAGGEDRIGHT         0x0800
    %define TCS_FOCUSONBUTTONDOWN   0x1000
    %define TCS_OWNERDRAWFIXED      0x2000
    %define TCS_TOOLTIPS            0x4000
    %define TCS_FOCUSNEVER          0x8000
    
    %define ACS_CENTER              0x0001
    %define ACS_TRANSPARENT         0x0002
    %define ACS_AUTOPLAY            0x0004
    %define ACS_TIMER               0x0008
    
    %define DTS_UPDOWN          0x0001
    %define DTS_SHOWNONE        0x0002
    %define DTS_SHORTDATEFORMAT 0x0000
    %define DTS_LONGDATEFORMAT  0x0004
    %define DTS_TIMEFORMAT      0x0009
    %define DTS_APPCANPARSE     0x0010
    %define DTS_RIGHTALIGN      0x0020
    
    %define PGS_VERT                0x00000000
    %define PGS_HORZ                0x00000001
    %define PGS_AUTOSCROLL          0x00000002
    %define PGS_DRAGNDROP           0x00000004
    
    ; style definition 

    %define NFS_EDIT                0x0001
    %define NFS_STATIC              0x0002
    %define NFS_LISTCOMBO           0x0004
    %define NFS_BUTTON              0x0008
    %define NFS_ALL                 0x0010

    ; ShowWindow() Commands 

    %define SW_HIDE             0
    %define SW_SHOWNORMAL       1
    %define SW_NORMAL           1
    %define SW_SHOWMINIMIZED    2
    %define SW_SHOWMAXIMIZED    3
    %define SW_MAXIMIZE         3
    %define SW_SHOWNOACTIVATE   4
    %define SW_SHOW             5
    %define SW_MINIMIZE         6
    %define SW_SHOWMINNOACTIVE  7
    %define SW_SHOWNA           8
    %define SW_RESTORE          9
    %define SW_SHOWDEFAULT      10
    %define SW_FORCEMINIMIZE    11
    %define SW_MAX              11
    
    ; Old ShowWindow() Commands
     
    %define HIDE_WINDOW         0
    %define SHOW_OPENWINDOW     1
    %define SHOW_ICONWINDOW     2
    %define SHOW_FULLSCREEN     3
    %define SHOW_OPENNOACTIVATE 4
    
    ; Identifiers for the WM_SHOWWINDOW message
     
    %define SW_PARENTCLOSING    1
    %define SW_OTHERZOOM        2
    %define SW_PARENTOPENING    3
    %define SW_OTHERUNZOOM      4
    
    ; Virtual Keys, Standard Set
     
    %define VK_LBUTTON        0x01
    %define VK_RBUTTON        0x02
    %define VK_CANCEL         0x03
    %define VK_MBUTTON        0x04
    
    %define VK_BACK           0x08
    %define VK_TAB            0x09
    
    %define VK_CLEAR          0x0C
    %define VK_RETURN         0x0D
    
    %define VK_SHIFT          0x10
    %define VK_CONTROL        0x11
    %define VK_MENU           0x12
    %define VK_PAUSE          0x13
    %define VK_CAPITAL        0x14
    
    %define VK_KANA           0x15
    %define VK_HANGEUL        0x15
    %define VK_HANGUL         0x15
    %define VK_JUNJA          0x17
    %define VK_FINAL          0x18
    %define VK_HANJA          0x19
    %define VK_KANJI          0x19
    
    %define VK_ESCAPE         0x1B
    
    %define VK_CONVERT        0x1C
    %define VK_NONCONVERT     0x1D
    %define VK_ACCEPT         0x1E
    %define VK_MODECHANGE     0x1F
    
    %define VK_SPACE          0x20
    %define VK_PRIOR          0x21
    %define VK_NEXT           0x22
    %define VK_END            0x23
    %define VK_HOME           0x24
    %define VK_LEFT           0x25
    %define VK_UP             0x26
    %define VK_RIGHT          0x27
    %define VK_DOWN           0x28
    %define VK_SELECT         0x29
    %define VK_PRINT          0x2A
    %define VK_EXECUTE        0x2B
    %define VK_SNAPSHOT       0x2C
    %define VK_INSERT         0x2D
    %define VK_DELETE         0x2E
    %define VK_HELP           0x2F
    
    ; VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) 
    ; VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) 
    
    %define VK_LWIN           0x5B
    %define VK_RWIN           0x5C
    %define VK_APPS           0x5D
    
    %define VK_NUMPAD0        0x60
    %define VK_NUMPAD1        0x61
    %define VK_NUMPAD2        0x62
    %define VK_NUMPAD3        0x63
    %define VK_NUMPAD4        0x64
    %define VK_NUMPAD5        0x65
    %define VK_NUMPAD6        0x66
    %define VK_NUMPAD7        0x67
    %define VK_NUMPAD8        0x68
    %define VK_NUMPAD9        0x69
    %define VK_MULTIPLY       0x6A
    %define VK_ADD            0x6B
    %define VK_SEPARATOR      0x6C
    %define VK_SUBTRACT       0x6D
    %define VK_DECIMAL        0x6E
    %define VK_DIVIDE         0x6F
    %define VK_F1             0x70
    %define VK_F2             0x71
    %define VK_F3             0x72
    %define VK_F4             0x73
    %define VK_F5             0x74
    %define VK_F6             0x75
    %define VK_F7             0x76
    %define VK_F8             0x77
    %define VK_F9             0x78
    %define VK_F10            0x79
    %define VK_F11            0x7A
    %define VK_F12            0x7B
    %define VK_F13            0x7C
    %define VK_F14            0x7D
    %define VK_F15            0x7E
    %define VK_F16            0x7F
    %define VK_F17            0x80
    %define VK_F18            0x81
    %define VK_F19            0x82
    %define VK_F20            0x83
    %define VK_F21            0x84
    %define VK_F22            0x85
    %define VK_F23            0x86
    %define VK_F24            0x87
    %define VK_NUMLOCK        0x90
    %define VK_SCROLL         0x91
    %define VK_LSHIFT         0xA0
    %define VK_RSHIFT         0xA1
    %define VK_LCONTROL       0xA2
    %define VK_RCONTROL       0xA3
    %define VK_LMENU          0xA4
    %define VK_RMENU          0xA5
    %define VK_PROCESSKEY     0xE5
    %define VK_ATTN           0xF6
    %define VK_CRSEL          0xF7
    %define VK_EXSEL          0xF8
    %define VK_EREOF          0xF9
    %define VK_PLAY           0xFA
    %define VK_ZOOM           0xFB
    %define VK_NONAME         0xFC
    %define VK_PA1            0xFD
    %define VK_OEM_CLEAR      0xFE
    
    %define WM_NULL                         0x0000
    %define WM_CREATE                       0x0001
    %define WM_DESTROY                      0x0002
    %define WM_MOVE                         0x0003
    %define WM_SIZE                         0x0005
    %define WM_ACTIVATE                     0x0006

    ; WM_ACTIVATE state values 

    %define     WA_INACTIVE     0
    %define     WA_ACTIVE       1
    %define     WA_CLICKACTIVE  2
    
    %define WM_SETFOCUS                     0x0007
    %define WM_KILLFOCUS                    0x0008
    %define WM_ENABLE                       0x000A
    %define WM_SETREDRAW                    0x000B
    %define WM_SETTEXT                      0x000C
    %define WM_GETTEXT                      0x000D
    %define WM_GETTEXTLENGTH                0x000E
    %define WM_PAINT                        0x000F
    %define WM_CLOSE                        0x0010
    %define WM_QUERYENDSESSION              0x0011
    %define WM_QUIT                         0x0012
    %define WM_QUERYOPEN                    0x0013
    %define WM_ERASEBKGND                   0x0014
    %define WM_SYSCOLORCHANGE               0x0015
    %define WM_ENDSESSION                   0x0016
    %define WM_SHOWWINDOW                   0x0018
    %define WM_WININICHANGE                 0x001A
    %define WM_SETTINGCHANGE                WM_WININICHANGE
    
    %define WM_DEVMODECHANGE                0x001B
    %define WM_ACTIVATEAPP                  0x001C
    %define WM_FONTCHANGE                   0x001D
    %define WM_TIMECHANGE                   0x001E
    %define WM_CANCELMODE                   0x001F
    %define WM_SETCURSOR                    0x0020
    %define WM_MOUSEACTIVATE                0x0021
    %define WM_CHILDACTIVATE                0x0022
    %define WM_QUEUESYNC                    0x0023
    
    %define WM_GETMINMAXINFO                0x0024
    %define WM_PAINTICON                    0x0026
    %define WM_ICONERASEBKGND               0x0027
    %define WM_NEXTDLGCTL                   0x0028
    %define WM_SPOOLERSTATUS                0x002A
    %define WM_DRAWITEM                     0x002B
    %define WM_MEASUREITEM                  0x002C
    %define WM_DELETEITEM                   0x002D
    %define WM_VKEYTOITEM                   0x002E
    %define WM_CHARTOITEM                   0x002F
    %define WM_SETFONT                      0x0030
    %define WM_GETFONT                      0x0031
    %define WM_SETHOTKEY                    0x0032
    %define WM_GETHOTKEY                    0x0033
    %define WM_QUERYDRAGICON                0x0037
    %define WM_COMPAREITEM                  0x0039
    %define WM_GETOBJECT                    0x003D
    %define WM_COMPACTING                   0x0041
    %define WM_COMMNOTIFY                   0x0044
    %define WM_WINDOWPOSCHANGING            0x0046
    %define WM_WINDOWPOSCHANGED             0x0047
    %define WM_POWER                        0x0048

    ; wParam for WM_POWER window message and DRV_POWER driver notification 

    %define PWR_OK              1
    %define PWR_FAIL            (-1)
    %define PWR_SUSPENDREQUEST  1
    %define PWR_SUSPENDRESUME   2
    %define PWR_CRITICALRESUME  3
    
    %define WM_COPYDATA                     0x004A
    %define WM_CANCELJOURNAL                0x004B
    
    %define WM_NOTIFY                       0x004E
    %define WM_INPUTLANGCHANGEREQUEST       0x0050
    %define WM_INPUTLANGCHANGE              0x0051
    %define WM_TCARD                        0x0052
    %define WM_HELP                         0x0053
    %define WM_USERCHANGED                  0x0054
    %define WM_NOTIFYFORMAT                 0x0055
    
    %define NFR_ANSI                             1
    %define NFR_UNICODE                          2
    %define NF_QUERY                             3
    %define NF_REQUERY                           4
    
    %define WM_CONTEXTMENU                  0x007B
    %define WM_STYLECHANGING                0x007C
    %define WM_STYLECHANGED                 0x007D
    %define WM_DISPLAYCHANGE                0x007E
    %define WM_GETICON                      0x007F
    %define WM_SETICON                      0x0080
    
    %define WM_NCCREATE                     0x0081
    %define WM_NCDESTROY                    0x0082
    %define WM_NCCALCSIZE                   0x0083
    %define WM_NCHITTEST                    0x0084
    %define WM_NCPAINT                      0x0085
    %define WM_NCACTIVATE                   0x0086
    %define WM_GETDLGCODE                   0x0087
    %define WM_SYNCPAINT                    0x0088
    %define WM_NCMOUSEMOVE                  0x00A0
    %define WM_NCLBUTTONDOWN                0x00A1
    %define WM_NCLBUTTONUP                  0x00A2
    %define WM_NCLBUTTONDBLCLK              0x00A3
    %define WM_NCRBUTTONDOWN                0x00A4
    %define WM_NCRBUTTONUP                  0x00A5
    %define WM_NCRBUTTONDBLCLK              0x00A6
    %define WM_NCMBUTTONDOWN                0x00A7
    %define WM_NCMBUTTONUP                  0x00A8
    %define WM_NCMBUTTONDBLCLK              0x00A9
    
    %define WM_KEYFIRST                     0x0100
    %define WM_KEYDOWN                      0x0100
    %define WM_KEYUP                        0x0101
    %define WM_CHAR                         0x0102
    %define WM_DEADCHAR                     0x0103
    %define WM_SYSKEYDOWN                   0x0104
    %define WM_SYSKEYUP                     0x0105
    %define WM_SYSCHAR                      0x0106
    %define WM_SYSDEADCHAR                  0x0107
    %define WM_KEYLAST                      0x0108
    
    %define WM_IME_STARTCOMPOSITION         0x010D
    %define WM_IME_ENDCOMPOSITION           0x010E
    %define WM_IME_COMPOSITION              0x010F
    %define WM_IME_KEYLAST                  0x010F
    
    %define WM_INITDIALOG                   0x0110
    %define WM_COMMAND                      0x0111
    %define WM_SYSCOMMAND                   0x0112
    %define WM_TIMER                        0x0113
    %define WM_HSCROLL                      0x0114
    %define WM_VSCROLL                      0x0115
    %define WM_INITMENU                     0x0116
    %define WM_INITMENUPOPUP                0x0117
    %define WM_MENUSELECT                   0x011F
    %define WM_MENUCHAR                     0x0120
    %define WM_ENTERIDLE                    0x0121
    %define WM_MENURBUTTONUP                0x0122
    %define WM_MENUDRAG                     0x0123
    %define WM_MENUGETOBJECT                0x0124
    %define WM_UNINITMENUPOPUP              0x0125
    %define WM_MENUCOMMAND                  0x0126
    
    %define WM_CTLCOLORMSGBOX               0x0132
    %define WM_CTLCOLOREDIT                 0x0133
    %define WM_CTLCOLORLISTBOX              0x0134
    %define WM_CTLCOLORBTN                  0x0135
    %define WM_CTLCOLORDLG                  0x0136
    %define WM_CTLCOLORSCROLLBAR            0x0137
    %define WM_CTLCOLORSTATIC               0x0138
    
    %define WM_MOUSEFIRST                   0x0200
    %define WM_MOUSEMOVE                    0x0200
    %define WM_LBUTTONDOWN                  0x0201
    %define WM_LBUTTONUP                    0x0202
    %define WM_LBUTTONDBLCLK                0x0203
    %define WM_RBUTTONDOWN                  0x0204
    %define WM_RBUTTONUP                    0x0205
    %define WM_RBUTTONDBLCLK                0x0206
    %define WM_MBUTTONDOWN                  0x0207
    %define WM_MBUTTONUP                    0x0208
    %define WM_MBUTTONDBLCLK                0x0209
    
    %define WHEEL_DELTA                     120
    %define WHEEL_PAGESCROLL                (UINT_MAX)
    
    %define WM_PARENTNOTIFY                 0x0210
    %define WM_ENTERMENULOOP                0x0211
    %define WM_EXITMENULOOP                 0x0212
    
    %define WM_NEXTMENU                     0x0213
    %define WM_SIZING                       0x0214
    %define WM_CAPTURECHANGED               0x0215
    %define WM_MOVING                       0x0216
    %define WM_POWERBROADCAST               0x0218      
    %define WM_DEVICECHANGE                 0x0219
    %define WM_MDICREATE                    0x0220
    %define WM_MDIDESTROY                   0x0221
    %define WM_MDIACTIVATE                  0x0222
    %define WM_MDIRESTORE                   0x0223
    %define WM_MDINEXT                      0x0224
    %define WM_MDIMAXIMIZE                  0x0225
    %define WM_MDITILE                      0x0226
    %define WM_MDICASCADE                   0x0227
    %define WM_MDIICONARRANGE               0x0228
    %define WM_MDIGETACTIVE                 0x0229
    
    %define WM_MDISETMENU                   0x0230
    %define WM_ENTERSIZEMOVE                0x0231
    %define WM_EXITSIZEMOVE                 0x0232
    %define WM_DROPFILES                    0x0233
    %define WM_MDIREFRESHMENU               0x0234
    
    %define WM_IME_SETCONTEXT               0x0281
    %define WM_IME_NOTIFY                   0x0282
    %define WM_IME_CONTROL                  0x0283
    %define WM_IME_COMPOSITIONFULL          0x0284
    %define WM_IME_SELECT                   0x0285
    %define WM_IME_CHAR                     0x0286
    %define WM_IME_REQUEST                  0x0288
    %define WM_IME_KEYDOWN                  0x0290
    %define WM_IME_KEYUP                    0x0291
    
    %define WM_MOUSEHOVER                   0x02A1
    %define WM_MOUSELEAVE                   0x02A3
    
    %define WM_CUT                          0x0300
    %define WM_COPY                         0x0301
    %define WM_PASTE                        0x0302
    %define WM_CLEAR                        0x0303
    %define WM_UNDO                         0x0304
    %define WM_RENDERFORMAT                 0x0305
    %define WM_RENDERALLFORMATS             0x0306
    %define WM_DESTROYCLIPBOARD             0x0307
    %define WM_DRAWCLIPBOARD                0x0308
    %define WM_PAINTCLIPBOARD               0x0309
    %define WM_VSCROLLCLIPBOARD             0x030A
    %define WM_SIZECLIPBOARD                0x030B
    %define WM_ASKCBFORMATNAME              0x030C
    %define WM_CHANGECBCHAIN                0x030D
    %define WM_HSCROLLCLIPBOARD             0x030E
    %define WM_QUERYNEWPALETTE              0x030F
    %define WM_PALETTEISCHANGING            0x0310
    %define WM_PALETTECHANGED               0x0311
    %define WM_HOTKEY                       0x0312
    
    %define WM_PRINT                        0x0317
    %define WM_PRINTCLIENT                  0x0318
    
    %define WM_HANDHELDFIRST                0x0358
    %define WM_HANDHELDLAST                 0x035F
    
    %define WM_AFXFIRST                     0x0360
    %define WM_AFXLAST                      0x037F
    
    %define WM_PENWINFIRST                  0x0380
    %define WM_PENWINLAST                   0x038F
    %define WM_APP                          0x8000
    
    %define WM_USER                         0x0400
    
    ;  wParam for WM_SIZING message  

    %define WMSZ_LEFT           1
    %define WMSZ_RIGHT          2
    %define WMSZ_TOP            3
    %define WMSZ_TOPLEFT        4
    %define WMSZ_TOPRIGHT       5
    %define WMSZ_BOTTOM         6
    %define WMSZ_BOTTOMLEFT     7
    %define WMSZ_BOTTOMRIGHT    8

    ; WM_NCHITTEST and MOUSEHOOKSTRUCT Mouse Position Codes 

    %define HTERROR             (-2)
    %define HTTRANSPARENT       (-1)
    %define HTNOWHERE           0
    %define HTCLIENT            1
    %define HTCAPTION           2
    %define HTSYSMENU           3
    %define HTGROWBOX           4
    %define HTSIZE              HTGROWBOX
    %define HTMENU              5
    %define HTHSCROLL           6
    %define HTVSCROLL           7
    %define HTMINBUTTON         8
    %define HTMAXBUTTON         9
    %define HTLEFT              10
    %define HTRIGHT             11
    %define HTTOP               12
    %define HTTOPLEFT           13
    %define HTTOPRIGHT          14
    %define HTBOTTOM            15
    %define HTBOTTOMLEFT        16
    %define HTBOTTOMRIGHT       17
    %define HTBORDER            18
    %define HTREDUCE            HTMINBUTTON
    %define HTZOOM              HTMAXBUTTON
    %define HTSIZEFIRST         HTLEFT
    %define HTSIZELAST          HTBOTTOMRIGHT
    %define HTOBJECT            19
    %define HTCLOSE             20
    %define HTHELP              21
    
    %define SMTO_NORMAL         0x0000
    %define SMTO_BLOCK          0x0001
    %define SMTO_ABORTIFHUNG    0x0002
    %define SMTO_NOTIMEOUTIFNOTHUNG 0x0008

    ; WM_MOUSEACTIVATE Return Codes 

    %define MA_ACTIVATE         1
    %define MA_ACTIVATEANDEAT   2
    %define MA_NOACTIVATE       3
    %define MA_NOACTIVATEANDEAT 4

    ; WM_SETICON / WM_GETICON Type Codes 

    %define ICON_SMALL          0
    %define ICON_BIG            1

    ; WM_SIZE message wParam values 

    %define SIZE_RESTORED       0
    %define SIZE_MINIMIZED      1
    %define SIZE_MAXIMIZED      2
    %define SIZE_MAXSHOW        3
    %define SIZE_MAXHIDE        4

    ; Obsolete constant names 

    %define SIZENORMAL          SIZE_RESTORED
    %define SIZEICONIC          SIZE_MINIMIZED
    %define SIZEFULLSCREEN      SIZE_MAXIMIZED
    %define SIZEZOOMSHOW        SIZE_MAXSHOW
    %define SIZEZOOMHIDE        SIZE_MAXHIDE

    ; WM_NCCALCSIZE "window valid rect" return values 

    %define WVR_ALIGNTOP        0x0010
    %define WVR_ALIGNLEFT       0x0020
    %define WVR_ALIGNBOTTOM     0x0040
    %define WVR_ALIGNRIGHT      0x0080
    %define WVR_HREDRAW         0x0100
    %define WVR_VREDRAW         0x0200
    %define WVR_REDRAW         (WVR_HREDRAW | \
                                WVR_VREDRAW)

    ; Key State Masks for Mouse Messages 

    %define MK_LBUTTON          0x0001
    %define MK_RBUTTON          0x0002
    %define MK_SHIFT            0x0004
    %define MK_CONTROL          0x0008
    %define MK_MBUTTON          0x0010
    
    %define TME_HOVER       0x00000001
    %define TME_LEAVE       0x00000002
    %define TME_QUERY       0x40000000
    %define TME_CANCEL      0x80000000
    
    %define HOVER_DEFAULT   0xFFFFFFFF

    ; Window Styles 

    %define WS_OVERLAPPED       0x00000000
    %define WS_POPUP            0x80000000
    %define WS_CHILD            0x40000000
    %define WS_MINIMIZE         0x20000000
    %define WS_VISIBLE          0x10000000
    %define WS_DISABLED         0x08000000
    %define WS_CLIPSIBLINGS     0x04000000
    %define WS_CLIPCHILDREN     0x02000000
    %define WS_MAXIMIZE         0x01000000
    %define WS_CAPTION          0x00C00000
    %define WS_BORDER           0x00800000
    %define WS_DLGFRAME         0x00400000
    %define WS_VSCROLL          0x00200000
    %define WS_HSCROLL          0x00100000
    %define WS_SYSMENU          0x00080000
    %define WS_THICKFRAME       0x00040000
    %define WS_GROUP            0x00020000
    %define WS_TABSTOP          0x00010000
    
    %define WS_MINIMIZEBOX      0x00020000
    %define WS_MAXIMIZEBOX      0x00010000

    %define WS_TILED            WS_OVERLAPPED
    %define WS_ICONIC           WS_MINIMIZE
    %define WS_SIZEBOX          WS_THICKFRAME
    %define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW

    ; Common Window Styles 

    %define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                                 WS_CAPTION        | \
                                 WS_SYSMENU        | \
                                 WS_THICKFRAME     | \
                                 WS_MINIMIZEBOX    | \
                                 WS_MAXIMIZEBOX)
    
    %define WS_POPUPWINDOW      (WS_POPUP          | \
                                 WS_BORDER         | \
                                 WS_SYSMENU)
    
    %define WS_CHILDWINDOW      (WS_CHILD)

    ; Extended Window Styles 

    %define WS_EX_DLGMODALFRAME     0x00000001
    %define WS_EX_NOPARENTNOTIFY    0x00000004
    %define WS_EX_TOPMOST           0x00000008L
    %define WS_EX_ACCEPTFILES       0x00000010
    %define WS_EX_TRANSPARENT       0x00000020
    %define WS_EX_MDICHILD          0x00000040
    %define WS_EX_TOOLWINDOW        0x00000080
    %define WS_EX_WINDOWEDGE        0x00000100
    %define WS_EX_CLIENTEDGE        0x00000200
    %define WS_EX_CONTEXTHELP       0x00000400
    %define WS_EX_RIGHT             0x00001000
    %define WS_EX_LEFT              0x00000000
    %define WS_EX_RTLREADING        0x00002000
    %define WS_EX_LTRREADING        0x00000000
    %define WS_EX_LEFTSCROLLBAR     0x00004000
    %define WS_EX_RIGHTSCROLLBAR    0x00000000
    %define WS_EX_CONTROLPARENT     0x00010000
    %define WS_EX_STATICEDGE        0x00020000
    %define WS_EX_APPWINDOW         0x00040000
    %define WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
    %define WS_EX_PALETTEWINDOW     (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)

    ; Class styles 

    %define CS_VREDRAW          0x0001
    %define CS_HREDRAW          0x0002
    %define CS_DBLCLKS          0x0008
    %define CS_OWNDC            0x0020
    %define CS_CLASSDC          0x0040
    %define CS_PARENTDC         0x0080
    %define CS_NOCLOSE          0x0200
    %define CS_SAVEBITS         0x0800
    %define CS_BYTEALIGNCLIENT  0x1000
    %define CS_BYTEALIGNWINDOW  0x2000
    %define CS_GLOBALCLASS      0x4000
    
    %define CS_IME              0x00010000

    ; Predefined Clipboard Formats 

    %define CF_TEXT             1
    %define CF_BITMAP           2
    %define CF_METAFILEPICT     3
    %define CF_SYLK             4
    %define CF_DIF              5
    %define CF_TIFF             6
    %define CF_OEMTEXT          7
    %define CF_DIB              8
    %define CF_PALETTE          9
    %define CF_PENDATA          10
    %define CF_RIFF             11
    %define CF_WAVE             12
    %define CF_UNICODETEXT      13
    %define CF_ENHMETAFILE      14
    %define CF_HDROP            15
    %define CF_LOCALE           16
    %define CF_MAX              17
    %define CF_OWNERDISPLAY     0x0080
    %define CF_DSPTEXT          0x0081
    %define CF_DSPBITMAP        0x0082
    %define CF_DSPMETAFILEPICT  0x0083
    %define CF_DSPENHMETAFILE   0x008E

    ; "Private" formats don't get GlobalFree()'d 

    %define CF_PRIVATEFIRST     0x0200
    %define CF_PRIVATELAST      0x02FF

    ; "GDIOBJ" formats do get DeleteObject()'d 

    %define CF_GDIOBJFIRST      0x0300
    %define CF_GDIOBJLAST       0x03FF

    ; Menu flags for Add/Check/EnableMenuItem() 

    %define MF_INSERT           0x00000000
    %define MF_CHANGE           0x00000080
    %define MF_APPEND           0x00000100
    %define MF_DELETE           0x00000200
    %define MF_REMOVE           0x00001000
    
    %define MF_BYCOMMAND        0x00000000
    %define MF_BYPOSITION       0x00000400
    
    %define MF_SEPARATOR        0x00000800
    
    %define MF_ENABLED          0x00000000
    %define MF_GRAYED           0x00000001
    %define MF_DISABLED         0x00000002
    
    %define MF_UNCHECKED        0x00000000
    %define MF_CHECKED          0x00000008L
    %define MF_USECHECKBITMAPS  0x00000200
    
    %define MF_STRING           0x00000000
    %define MF_BITMAP           0x00000004
    %define MF_OWNERDRAW        0x00000100
    
    %define MF_POPUP            0x00000010
    %define MF_MENUBARBREAK     0x00000020
    %define MF_MENUBREAK        0x00000040
    
    %define MF_UNHILITE         0x00000000
    %define MF_HILITE           0x00000080
    
    %define MF_DEFAULT          0x00001000
    %define MF_SYSMENU          0x00002000
    %define MF_HELP             0x00004000
    %define MF_RIGHTJUSTIFY     0x00004000
    
    %define MF_MOUSESELECT      0x00008000
    %define MF_END              0x00000080
    
    %define MFT_STRING          MF_STRING
    %define MFT_BITMAP          MF_BITMAP
    %define MFT_MENUBARBREAK    MF_MENUBARBREAK
    %define MFT_MENUBREAK       MF_MENUBREAK
    %define MFT_OWNERDRAW       MF_OWNERDRAW
    %define MFT_RADIOCHECK      0x00000200
    %define MFT_SEPARATOR       MF_SEPARATOR
    %define MFT_RIGHTORDER      0x00002000
    %define MFT_RIGHTJUSTIFY    MF_RIGHTJUSTIFY
    
    ; Menu flags for Add/Check/EnableMenuItem() 

    %define MFS_GRAYED          0x00000003
    %define MFS_DISABLED        MFS_GRAYED
    %define MFS_CHECKED         MF_CHECKED
    %define MFS_HILITE          MF_HILITE
    %define MFS_ENABLED         MF_ENABLED
    %define MFS_UNCHECKED       MF_UNCHECKED
    %define MFS_UNHILITE        MF_UNHILITE
    %define MFS_DEFAULT         MF_DEFAULT
    %define MFS_MASK            0x0000108BL
    %define MFS_HOTTRACKDRAWN   0x10000000
    %define MFS_CACHEDBMP       0x20000000
    %define MFS_BOTTOMGAPDROP   0x40000000
    %define MFS_TOPGAPDROP      0x80000000
    %define MFS_GAPDROP         0xC0000000
    
    %define MF_END             0x00000080

    ; System Menu Command Values 

    %define SC_SIZE         0xF000
    %define SC_MOVE         0xF010
    %define SC_MINIMIZE     0xF020
    %define SC_MAXIMIZE     0xF030
    %define SC_NEXTWINDOW   0xF040
    %define SC_PREVWINDOW   0xF050
    %define SC_CLOSE        0xF060
    %define SC_VSCROLL      0xF070
    %define SC_HSCROLL      0xF080
    %define SC_MOUSEMENU    0xF090
    %define SC_KEYMENU      0xF100
    %define SC_ARRANGE      0xF110
    %define SC_RESTORE      0xF120
    %define SC_TASKLIST     0xF130
    %define SC_SCREENSAVE   0xF140
    %define SC_HOTKEY       0xF150
    %define SC_DEFAULT      0xF160
    %define SC_MONITORPOWER 0xF170
    %define SC_CONTEXTHELP  0xF180
    %define SC_SEPARATOR    0xF00F

    ; Obsolete names 

    %define SC_ICON         SC_MINIMIZE
    %define SC_ZOOM         SC_MAXIMIZE

    ; OEM Resource Ordinal Numbers 

    %define OBM_CLOSE           32754
    %define OBM_UPARROW         32753
    %define OBM_DNARROW         32752
    %define OBM_RGARROW         32751
    %define OBM_LFARROW         32750
    %define OBM_REDUCE          32749
    %define OBM_ZOOM            32748
    %define OBM_RESTORE         32747
    %define OBM_REDUCED         32746
    %define OBM_ZOOMD           32745
    %define OBM_RESTORED        32744
    %define OBM_UPARROWD        32743
    %define OBM_DNARROWD        32742
    %define OBM_RGARROWD        32741
    %define OBM_LFARROWD        32740
    %define OBM_MNARROW         32739
    %define OBM_COMBO           32738
    %define OBM_UPARROWI        32737
    %define OBM_DNARROWI        32736
    %define OBM_RGARROWI        32735
    %define OBM_LFARROWI        32734
    
    %define OBM_OLD_CLOSE       32767
    %define OBM_SIZE            32766
    %define OBM_OLD_UPARROW     32765
    %define OBM_OLD_DNARROW     32764
    %define OBM_OLD_RGARROW     32763
    %define OBM_OLD_LFARROW     32762
    %define OBM_BTSIZE          32761
    %define OBM_CHECK           32760
    %define OBM_CHECKBOXES      32759
    %define OBM_BTNCORNERS      32758
    %define OBM_OLD_REDUCE      32757
    %define OBM_OLD_ZOOM        32756
    %define OBM_OLD_RESTORE     32755

    %define OCR_NORMAL          32512
    %define OCR_IBEAM           32513
    %define OCR_WAIT            32514
    %define OCR_CROSS           32515
    %define OCR_UP              32516
    %define OCR_SIZE            32640
    %define OCR_ICON            32641
    %define OCR_SIZENWSE        32642
    %define OCR_SIZENESW        32643
    %define OCR_SIZEWE          32644
    %define OCR_SIZENS          32645
    %define OCR_SIZEALL         32646
    %define OCR_ICOCUR          32647
    %define OCR_NO              32648
    %define OCR_HAND            32649
    %define OCR_APPSTARTING     32650
    
    %define OIC_SAMPLE          32512
    %define OIC_HAND            32513
    %define OIC_QUES            32514
    %define OIC_BANG            32515
    %define OIC_NOTE            32516
    %define OIC_WINLOGO         32517
    %define OIC_WARNING         OIC_BANG
    %define OIC_ERROR           OIC_HAND
    %define OIC_INFORMATION     OIC_NOTE

    ; Standard Icon IDs 

    %define IDI_APPLICATION     32512
    %define IDI_HAND            32513
    %define IDI_QUESTION        32514
    %define IDI_EXCLAMATION     32515
    %define IDI_ASTERISK        32516
    %define IDI_WINLOGO         32517
    
    %define IDI_WARNING     IDI_EXCLAMATION
    %define IDI_ERROR       IDI_HAND
    %define IDI_INFORMATION IDI_ASTERISK

    ; Dialog Box Command IDs 

    %define IDOK                1
    %define IDCANCEL            2
    %define IDABORT             3
    %define IDRETRY             4
    %define IDIGNORE            5
    %define IDYES               6
    %define IDNO                7
    %define IDCLOSE         8
    %define IDHELP          9
    
    ; Edit Control Styles 

    %define ES_LEFT             0x0000
    %define ES_CENTER           0x0001
    %define ES_RIGHT            0x0002
    %define ES_MULTILINE        0x0004
    %define ES_UPPERCASE        0x0008L
    %define ES_LOWERCASE        0x0010
    %define ES_PASSWORD         0x0020
    %define ES_AUTOVSCROLL      0x0040
    %define ES_AUTOHSCROLL      0x0080
    %define ES_NOHIDESEL        0x0100
    %define ES_OEMCONVERT       0x0400
    %define ES_READONLY         0x0800
    %define ES_WANTRETURN       0x1000
    %define ES_NUMBER           0x2000
    
    ; Edit Control Messages 

    %define EM_GETSEL               0x00B0
    %define EM_SETSEL               0x00B1
    %define EM_GETRECT              0x00B2
    %define EM_SETRECT              0x00B3
    %define EM_SETRECTNP            0x00B4
    %define EM_SCROLL               0x00B5
    %define EM_LINESCROLL           0x00B6
    %define EM_SCROLLCARET          0x00B7
    %define EM_GETMODIFY            0x00B8
    %define EM_SETMODIFY            0x00B9
    %define EM_GETLINECOUNT         0x00BA
    %define EM_LINEINDEX            0x00BB
    %define EM_SETHANDLE            0x00BC
    %define EM_GETHANDLE            0x00BD
    %define EM_GETTHUMB             0x00BE
    %define EM_LINELENGTH           0x00C1
    %define EM_REPLACESEL           0x00C2
    %define EM_GETLINE              0x00C4
    %define EM_LIMITTEXT            0x00C5
    %define EM_CANUNDO              0x00C6
    %define EM_UNDO                 0x00C7
    %define EM_FMTLINES             0x00C8
    %define EM_LINEFROMCHAR         0x00C9
    %define EM_SETTABSTOPS          0x00CB
    %define EM_SETPASSWORDCHAR      0x00CC
    %define EM_EMPTYUNDOBUFFER      0x00CD
    %define EM_GETFIRSTVISIBLELINE  0x00CE
    %define EM_SETREADONLY          0x00CF
    %define EM_SETWORDBREAKPROC     0x00D0
    %define EM_GETWORDBREAKPROC     0x00D1
    %define EM_GETPASSWORDCHAR      0x00D2
    %define EM_SETMARGINS           0x00D3
    %define EM_GETMARGINS           0x00D4
    %define EM_SETLIMITTEXT         EM_LIMITTEXT
    %define EM_GETLIMITTEXT         0x00D5
    %define EM_POSFROMCHAR          0x00D6
    %define EM_CHARFROMPOS          0x00D7
    
    %define EM_SETIMESTATUS         0x00D8
    %define EM_GETIMESTATUS         0x00D9
    
    ; Button Control Styles 

    %define BS_PUSHBUTTON       0x00000000
    %define BS_DEFPUSHBUTTON    0x00000001
    %define BS_CHECKBOX         0x00000002
    %define BS_AUTOCHECKBOX     0x00000003
    %define BS_RADIOBUTTON      0x00000004
    %define BS_3STATE           0x00000005
    %define BS_AUTO3STATE       0x00000006
    %define BS_GROUPBOX         0x00000007
    %define BS_USERBUTTON       0x00000008
    %define BS_AUTORADIOBUTTON  0x00000009
    %define BS_OWNERDRAW        0x0000000B
    %define BS_LEFTTEXT         0x00000020
    %define BS_TEXT             0x00000000
    %define BS_ICON             0x00000040
    %define BS_BITMAP           0x00000080
    %define BS_LEFT             0x00000100
    %define BS_RIGHT            0x00000200
    %define BS_CENTER           0x00000300
    %define BS_TOP              0x00000400
    %define BS_BOTTOM           0x00000800
    %define BS_VCENTER          0x00000C00
    %define BS_PUSHLIKE         0x00001000
    %define BS_MULTILINE        0x00002000
    %define BS_NOTIFY           0x00004000
    %define BS_FLAT             0x00008000
    %define BS_RIGHTBUTTON      BS_LEFTTEXT

    ; User Button Notification Codes 

    %define BN_CLICKED          0
    %define BN_PAINT            1
    %define BN_HILITE           2
    %define BN_UNHILITE         3
    %define BN_DISABLE          4
    %define BN_DOUBLECLICKED    5
    %define BN_PUSHED           BN_HILITE
    %define BN_UNPUSHED         BN_UNHILITE
    %define BN_DBLCLK           BN_DOUBLECLICKED
    %define BN_SETFOCUS         6
    %define BN_KILLFOCUS        7

    ; Button Control Messages 

    %define BM_GETCHECK        0x00F0
    %define BM_SETCHECK        0x00F1
    %define BM_GETSTATE        0x00F2
    %define BM_SETSTATE        0x00F3
    %define BM_SETSTYLE        0x00F4
    %define BM_CLICK           0x00F5
    %define BM_GETIMAGE        0x00F6
    %define BM_SETIMAGE        0x00F7
    
    %define BST_UNCHECKED      0x0000
    %define BST_CHECKED        0x0001
    %define BST_INDETERMINATE  0x0002
    %define BST_PUSHED         0x0004
    %define BST_FOCUS          0x0008

    ; Static Control Constants 

    %define SS_LEFT             0x00000000
    %define SS_CENTER           0x00000001
    %define SS_RIGHT            0x00000002
    %define SS_ICON             0x00000003
    %define SS_BLACKRECT        0x00000004
    %define SS_GRAYRECT         0x00000005
    %define SS_WHITERECT        0x00000006
    %define SS_BLACKFRAME       0x00000007
    %define SS_GRAYFRAME        0x00000008
    %define SS_WHITEFRAME       0x00000009
    %define SS_USERITEM         0x0000000A
    %define SS_SIMPLE           0x0000000B
    %define SS_LEFTNOWORDWRAP   0x0000000C
    %define SS_OWNERDRAW        0x0000000D
    %define SS_BITMAP           0x0000000E
    %define SS_ENHMETAFILE      0x0000000F
    %define SS_ETCHEDHORZ       0x00000010
    %define SS_ETCHEDVERT       0x00000011
    %define SS_ETCHEDFRAME      0x00000012
    %define SS_TYPEMASK         0x0000001F
    %define SS_NOPREFIX         0x00000080
    %define SS_NOTIFY           0x00000100
    %define SS_CENTERIMAGE      0x00000200
    %define SS_RIGHTJUST        0x00000400
    %define SS_REALSIZEIMAGE    0x00000800
    %define SS_SUNKEN           0x00001000
    %define SS_ENDELLIPSIS      0x00004000
    %define SS_PATHELLIPSIS     0x00008000
    %define SS_WORDELLIPSIS     0x0000C000
    %define SS_ELLIPSISMASK     0x0000C000

    ; Dialog Styles 

    %define DS_ABSALIGN         0x01
    %define DS_SYSMODAL         0x02
    %define DS_LOCALEDIT        0x20
    %define DS_SETFONT          0x40
    %define DS_MODALFRAME       0x80
    %define DS_NOIDLEMSG        0x100
    %define DS_SETFOREGROUND    0x200
    
    %define DS_3DLOOK           	0x0004
    %define DS_FIXEDSYS         	0x0008
    %define DS_NOFAILCREATE     	0x0010
    %define DS_CONTROL          	0x0400
    %define DS_CENTER           	0x0800
    %define DS_CENTERMOUSE      	0x1000
    %define DS_CONTEXTHELP      	0x2000

    ; Listbox Styles 

    %define LBS_NOTIFY            0x0001
    %define LBS_SORT              0x0002
    %define LBS_NOREDRAW          0x0004
    %define LBS_MULTIPLESEL       0x0008
    %define LBS_OWNERDRAWFIXED    0x0010
    %define LBS_OWNERDRAWVARIABLE 0x0020
    %define LBS_HASSTRINGS        0x0040
    %define LBS_USETABSTOPS       0x0080
    %define LBS_NOINTEGRALHEIGHT  0x0100
    %define LBS_MULTICOLUMN       0x0200
    %define LBS_WANTKEYBOARDINPUT 0x0400
    %define LBS_EXTENDEDSEL       0x0800
    %define LBS_DISABLENOSCROLL   0x1000
    %define LBS_NODATA            0x2000
    %define LBS_NOSEL             0x4000
    %define LBS_STANDARD          (LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER)

    ; Combo Box styles 

    %define CBS_SIMPLE            	0x0001
    %define CBS_DROPDOWN          	0x0002
    %define CBS_DROPDOWNLIST      	0x0003
    %define CBS_OWNERDRAWFIXED    	0x0010
    %define CBS_OWNERDRAWVARIABLE 	0x0020
    %define CBS_AUTOHSCROLL       	0x0040
    %define CBS_OEMCONVERT        	0x0080
    %define CBS_SORT              	0x0100
    %define CBS_HASSTRINGS        	0x0200
    %define CBS_NOINTEGRALHEIGHT  	0x0400
    %define CBS_DISABLENOSCROLL   	0x0800
    %define CBS_UPPERCASE           0x2000
    %define CBS_LOWERCASE           0x4000

    ; Scroll Bar Styles 

    %define SBS_HORZ                    0x0000
    %define SBS_VERT                    0x0001
    %define SBS_TOPALIGN                0x0002
    %define SBS_LEFTALIGN               0x0002
    %define SBS_BOTTOMALIGN             0x0004
    %define SBS_RIGHTALIGN              0x0004
    %define SBS_SIZEBOXTOPLEFTALIGN     0x0002
    %define SBS_SIZEBOXBOTTOMRIGHTALIGN 0x0004
    %define SBS_SIZEBOX                 0x0008
    %define SBS_SIZEGRIP                0x0010

    ; Commands to pass to WinHelp() 

    %define HELP_CONTEXT      0x0001
    %define HELP_QUIT         0x0002
    %define HELP_INDEX        0x0003
    %define HELP_CONTENTS     0x0003
    %define HELP_HELPONHELP   0x0004
    %define HELP_SETINDEX     0x0005
    %define HELP_SETCONTENTS  0x0005
    %define HELP_CONTEXTPOPUP 0x0008
    %define HELP_FORCEFILE    0x0009
    %define HELP_KEY          0x0101
    %define HELP_COMMAND      0x0102
    %define HELP_PARTIALKEY   0x0105
    %define HELP_MULTIKEY     0x0201
    %define HELP_SETWINPOS    0x0203
    %define HELP_CONTEXTMENU  0x000a
    %define HELP_FINDER       0x000b
    %define HELP_WM_HELP      0x000c
    %define HELP_SETPOPUP_POS 0x000d
    
    %define HELP_TCARD              0x8000
    %define HELP_TCARD_DATA         0x0010
    %define HELP_TCARD_OTHER_CALLER 0x0011
    
    ; These are in winhelp.h in Win95. 

    %define IDH_NO_HELP                     28440
    %define IDH_MISSING_CONTEXT             28441
    %define IDH_GENERIC_HELP_BUTTON         28442
    %define IDH_OK                          28443
    %define IDH_CANCEL                      28444
    %define IDH_HELP                        28445
    
    
    
%define ACHAR         BYTE  ;ansi character
%define ATOM          DWORD ;string atom
%define BOOL          DWORD ;boolean variable
%define COLORREF      DWORD ;rgb color
%define DWORDLONG     QWORD ;long double word
%define GLOBALHANDLE  DWORD ;global handle
%define HACCEL        DWORD ;accelerator handle
%define HANDLE        DWORD ;unspecified handle
%define HBITMAP       DWORD ;bitmap handle
%define HBRUSH        DWORD ;brush handle
%define HCOLORSPACE   DWORD ;color space handle
%define HCURSOR       DWORD ;cursor handle
%define HDC           DWORD ;device context handle
%define HDWP          DWORD ;defer win pos handle
%define HENHMETAFILE  DWORD ;enh. metafile handle
%define HFILE         DWORD ;file handle
%define HFONT         DWORD ;font handle
%define HGLOBAL       DWORD ;global handle
%define HHOOK         DWORD ;hook handle
%define HICON         DWORD ;icon handle
%define HINSTANCE     DWORD ;instance handle
%define HINTERNET     DWORD ;internet handle
%define HLOCAL        DWORD ;local handle
%define HMENU         DWORD ;menu handle
%define HMETAFILE     DWORD ;metafile handle
%define HPALETTE      DWORD ;palette handle
%define HPEN          DWORD ;pen handle
%define HRGN          DWORD ;region handle
%define HRSRC         DWORD ;resource handle
%define HSTR          DWORD ;string handle
%define HTASK         DWORD ;task handle
%define HTREEITEM     DWORD ;tree view item handle
%define HWND          DWORD ;window handle
%define INTEGER       DWORD ;standard integer
%define LOCALHANDLE   DWORD ;local handle
%define LONG          DWORD ;long integer
%define LONGINT       DWORD ;long integer
%define LPARAM        DWORD ;long parameter
%define LPBOOL        DWORD ;long ptr to boolean
%define LPBYTE        DWORD ;long ptr to byte
%define LPCSTR        DWORD ;long ptr to string
%define LPCTSTR       DWORD ;long ptr to string
%define LPCVOID       DWORD ;long ptr to buffer
%define LPDWORD       DWORD ;long ptr to dword
%define LPFN          DWORD ;long ptr to function
%define LPINT         DWORD ;long ptr to integer
%define LPLONG        DWORD ;long ptr to long int
%define LPMSG         DWORD ;long pointer to message
%define LPPAINTSTRUCT DWORD ;long pointer to paint structure
%define LPRECT        DWORD ;long pointer to rectangle
%define LPSTR         DWORD ;long ptr to string
%define LPTSTR        DWORD ;long ptr to string
%define LPVOID        DWORD ;long ptr to buffer
%define LPWORD        DWORD ;long ptr to word
%define LRESULT       DWORD ;long result
%define POINTER       DWORD ;pointer to anything
%define PVOID         DWORD ;pointer to buffer
%define SHORTINT      WORD  ;short integer
%define UINT          DWORD ;unsigned integer
%define WCHAR         WORD  ;unicode character
%define WNDPROC       DWORD ;window procedure
%define WPARAM        DWORD ;word parameter



;-------------------------win32api equates-------------------------------

%define FOURCC     DWORD
%define WINAPI 1
%define TRUE 1
%define FALSE 0
%define NULL 0
%define Normal 000000h
%define ReadOnly 000001h
%define Hidden 000010h
%define System 000100h
%define vLabel 001000h
%define SubDir 010000h
%define Archive 100000h
%define Black 000000h
%define Blue 0FF0000h
%define Green 00FF00h
%define Cyan 0FFFF00h
%define Red 0000FFh
%define Magenta 0FF00FFh
%define Yellow 00FFFFh
%define White 0FFFFFFh
%define Gray 080808h
%define ANYSIZE_ARRAY 1
%define INVALID_HANDLE_VALUE -1
%define DELETE 10000h
%define READ_CONTROL 20000h
%define WRITE_DAC 40000h
%define WRITE_OWNER 80000h
%define SYNCHRONIZE 100000h
%define STANDARD_RIGHTS_READ READ_CONTROL
%define STANDARD_RIGHTS_WRITE READ_CONTROL
%define STANDARD_RIGHTS_EXECUTE READ_CONTROL
%define STANDARD_RIGHTS_REQUIRED 0F0000h
%define STANDARD_RIGHTS_ALL 1F0000h
%define SPECIFIC_RIGHTS_ALL 0FFFFh
%define SID_REVISION 1
%define SID_MAX_SUB_AUTHORITIES 15
%define SID_RECOMMENDED_SUB_AUTHORITIES 1
%define SidTypeUser 1
%define SidTypeGroup 2
%define SidTypeDomain 3
%define SidTypeAlias 4
%define SidTypeWellKnownGroup 5
%define SidTypeDeletedAccount 6
%define SidTypeInvalid 7
%define SidTypeUnknown 8
%define SECURITY_NULL_RID 0h
%define SECURITY_WORLD_RID 0h
%define SECURITY_LOCAL_RID 0h
%define SECURITY_CREATOR_OWNER_RID 0h
%define SECURITY_CREATOR_GROUP_RID 1h
%define SECURITY_DIALUP_RID 1h
%define SECURITY_NETWORK_RID 2h
%define SECURITY_BATCH_RID 3h
%define SECURITY_INTERACTIVE_RID 4h
%define SECURITY_SERVICE_RID 6h
%define SECURITY_ANONYMOUS_LOGON_RID 7h
%define SECURITY_LOGON_IDS_RID 5h
%define SECURITY_LOCAL_SYSTEM_RID 12h
%define SECURITY_NT_NON_UNIQUE 15h
%define SECURITY_BUILTIN_DOMAIN_RID 20h
%define DOMAIN_USER_RID_ADMIN 1F4h
%define DOMAIN_USER_RID_GUEST 1F5h
%define DOMAIN_GROUP_RID_ADMINS 200h
%define DOMAIN_GROUP_RID_USERS 201h
%define DOMAIN_GROUP_RID_GUESTS 202h
%define DOMAIN_ALIAS_RID_ADMINS 220h
%define DOMAIN_ALIAS_RID_USERS 221h
%define DOMAIN_ALIAS_RID_GUESTS 222h
%define DOMAIN_ALIAS_RID_POWER_USERS 223h
%define DOMAIN_ALIAS_RID_ACCOUNT_OPS 224h
%define DOMAIN_ALIAS_RID_SYSTEM_OPS 225h
%define DOMAIN_ALIAS_RID_PRINT_OPS 226h
%define DOMAIN_ALIAS_RID_BACKUP_OPS 227h
%define DOMAIN_ALIAS_RID_REPLICATOR 228h
%define SE_GROUP_MANDATORY 1h
%define SE_GROUP_ENABLED_BY_DEFAULT 2h
%define SE_GROUP_ENABLED 4h
%define SE_GROUP_OWNER 8h
%define SE_GROUP_LOGON_ID 0C0000000h
%define FILE_BEGIN 0
%define FILE_CURRENT 1
%define FILE_END 2
%define FILE_FLAG_WRITE_THROUGH 80000000h
%define FILE_FLAG_OVERLAPPED 40000000h
%define FILE_FLAG_NO_BUFFERING 20000000h
%define FILE_FLAG_RANDOM_ACCESS 10000000h
%define FILE_FLAG_SEQUENTIAL_SCAN 8000000h
%define FILE_FLAG_DELETE_ON_CLOSE 4000000h
%define FILE_FLAG_BACKUP_SEMANTICS 2000000h
%define FILE_FLAG_POSIX_SEMANTICS 1000000h
%define CREATE_NEW 1
%define CREATE_ALWAYS 2
%define OPEN_EXISTING 3
%define OPEN_ALWAYS 4
%define TRUNCATE_EXISTING 5
%define PIPE_ACCESS_INBOUND 1h
%define PIPE_ACCESS_OUTBOUND 2h
%define PIPE_ACCESS_DUPLEX 3h
%define PIPE_CLIENT_END 0h
%define PIPE_SERVER_END 1h
%define PIPE_WAIT 0h
%define PIPE_NOWAIT 1h
%define PIPE_READMODE_BYTE 0h
%define PIPE_READMODE_MESSAGE 2h
%define PIPE_TYPE_BYTE 0h
%define PIPE_TYPE_MESSAGE 4h
%define PIPE_UNLIMITED_INSTANCES 255
%define SECURITY_CONTEXT_TRACKING 40000h
%define SECURITY_EFFECTIVE_ONLY 80000h
%define SECURITY_SQOS_PRESENT 100000h
%define SECURITY_VALID_SQOS_FLAGS 1F0000h
%define SP_SERIALCOMM 1h
%define PST_UNSPECIFIED 0h
%define PST_RS232 1h
%define PST_PARALLELPORT 2h
%define PST_RS422 3h
%define PST_RS423 4h
%define PST_RS449 5h
%define PST_FAX 21h
%define PST_SCANNER 22h
%define PST_NETWORK_BRIDGE 100h
%define PST_LAT 101h
%define PST_TCPIP_TELNET 102h
%define PST_X25 103h
%define PCF_DTRDSR 1h
%define PCF_RTSCTS 2h
%define PCF_RLSD 4h
%define PCF_PARITY_CHECK 8h
%define PCF_XONXOFF 10h
%define PCF_SETXCHAR 20h
%define PCF_TOTALTIMEOUTS 40h
%define PCF_INTTIMEOUTS 80h
%define PCF_SPECIALCHARS 100h
%define PCF_16BITMODE 200h
%define DLL_PROCESS_DETACH 0
%define DLL_PROCESS_ATTACH 1
%define DLL_THREAD_ATTACH 2
%define DLL_THREAD_DETACH 3
%define SP_PARITY 1h
%define SP_BAUD 2h
%define SP_DATABITS 4h
%define SP_STOPBITS 8h
%define SP_HANDSHAKING 10h
%define SP_PARITY_CHECK 20h
%define SP_RLSD 40h
%define BAUD_075 1h
%define BAUD_110 2h
%define BAUD_134_5 4h
%define BAUD_150 8h
%define BAUD_300 10h
%define BAUD_600 20h
%define BAUD_1200 40h
%define BAUD_1800 80h
%define BAUD_2400 100h
%define BAUD_4800 200h
%define BAUD_7200 400h
%define BAUD_9600 800h
%define BAUD_14400 1000h
%define BAUD_19200 2000h
%define BAUD_38400 4000h
%define BAUD_56K 8000h
%define BAUD_128K 10000h
%define BAUD_115200 20000h
%define BAUD_57600 40000h
%define BAUD_USER 10000000h
%define DATABITS_5 1h
%define DATABITS_6 2h
%define DATABITS_7 4h
%define DATABITS_8 8h
%define DATABITS_16 10h
%define DATABITS_16X 20h
%define STOPBITS_10 1h
%define STOPBITS_15 2h
%define STOPBITS_20 4h
%define PARITY_NONE 100h
%define PARITY_ODD 200h
%define PARITY_EVEN 400h
%define PARITY_MARK 800h
%define PARITY_SPACE 1000h
%define DTR_CONTROL_DISABLE 0h
%define DTR_CONTROL_ENABLE 1h
%define DTR_CONTROL_HANDSHAKE 2h
%define RTS_CONTROL_DISABLE 0h
%define RTS_CONTROL_ENABLE 1h
%define RTS_CONTROL_HANDSHAKE 2h
%define RTS_CONTROL_TOGGLE 3h
%define GMEM_FIXED 0h
%define GMEM_MOVEABLE 2h
%define GMEM_NOCOMPACT 10h
%define GMEM_NODISCARD 20h
%define GMEM_ZEROINIT 40h
%define GMEM_MODIFY 80h
%define GMEM_DISCARDABLE 100h
%define GMEM_NOT_BANKED 1000h
%define GMEM_SHARE 2000h
%define GMEM_DDESHARE 2000h
%define GMEM_NOTIFY 4000h
%define GMEM_LOWER GMEM_NOT_BANKED
%define GMEM_VALID_FLAGS 7F72h
%define GMEM_INVALID_HANDLE 8000h
%define GMEM_DISCARDED 4000h
%define GMEM_LOCKCOUNT 0FFh
%define GHND GMEM_MOVEABLE|GMEM_ZEROINIT
%define GPTR GMEM_FIXED|GMEM_ZEROINIT
%define LMEM_FIXED 0h
%define LMEM_MOVEABLE 2h
%define LMEM_NOCOMPACT 10h
%define LMEM_NODISCARD 20h
%define LMEM_ZEROINIT 40h
%define LMEM_MODIFY 80h
%define LMEM_DISCARDABLE 0F00h
%define LMEM_VALID_FLAGS 0F72h
%define LMEM_INVALID_HANDLE 8000h
%define LHND LMEM_MOVEABLE+LMEM_ZEROINIT
%define LPTR LMEM_FIXED+LMEM_ZEROINIT
%define NONZEROLHND LMEM_MOVEABLE
%define NONZEROLPTR LMEM_FIXED
%define LMEM_DISCARDED 4000h
%define LMEM_LOCKCOUNT 0FFh
%define DEBUG_PROCESS 1h
%define DEBUG_ONLY_THIS_PROCESS 2h
%define CREATE_SUSPENDED 4h
%define DETACHED_PROCESS 8h
%define CREATE_NEW_CONSOLE 10h
%define NORMAL_PRIORITY_CLASS 20h
%define IDLE_PRIORITY_CLASS 40h
%define HIGH_PRIORITY_CLASS 80h
%define REALTIME_PRIORITY_CLASS 100h
%define CREATE_NEW_PROCESS_GROUP 200h
%define CREATE_NO_WINDOW 8000000h
%define PROFILE_USER 10000000h
%define PROFILE_KERNEL 20000000h
%define PROFILE_SERVER 40000000h
%define MAXLONG 7FFFFFFFh
%define THREAD_BASE_PRIORITY_MIN -2
%define THREAD_BASE_PRIORITY_MAX 2
%define THREAD_BASE_PRIORITY_LOWRT 15
%define THREAD_BASE_PRIORITY_IDLE -15
%define THREAD_PRIORITY_LOWEST THREAD_BASE_PRIORITY_MIN
%define THREAD_PRIORITY_BELOW_NORMAL THREAD_PRIORITY_LOWEST+1
%define THREAD_PRIORITY_NORMAL 0
%define THREAD_PRIORITY_HIGHEST THREAD_BASE_PRIORITY_MAX
%define THREAD_PRIORITY_ABOVE_NORMAL THREAD_PRIORITY_HIGHEST-1
%define THREAD_PRIORITY_ERROR_RETURN MAXLONG
%define THREAD_PRIORITY_TIME_CRITICAL THREAD_BASE_PRIORITY_LOWRT
%define THREAD_PRIORITY_IDLE THREAD_BASE_PRIORITY_IDLE
%define APPLICATION_ERROR_MASK 20000000h
%define ERROR_SEVERITY_SUCCESS 0h
%define ERROR_SEVERITY_INFORMATIONAL 40000000h
%define ERROR_SEVERITY_WARNING 80000000h
%define ERROR_SEVERITY_ERROR 0C0000000h
%define MINCHAR 80h
%define MAXCHAR 7Fh
%define MINSHORT 8000h
%define MAXSHORT 7FFFh
%define MINLONG 80000000h
%define MAXBYTE 0FFh
%define MAXWORD 0FFFFh
%define MAXDWORD 0FFFFFFFFh
%define LANG_NEUTRAL 0h
%define LANG_BULGARIAN 2h
%define LANG_CHINESE 4h
%define LANG_CROATIAN 1Ah
%define LANG_CZECH 5h
%define LANG_DANISH 6h
%define LANG_DUTCH 13h
%define LANG_ENGLISH 9h
%define LANG_FINNISH 0Bh
%define LANG_FRENCH 0Ch
%define LANG_GERMAN 7h
%define LANG_GREEK 8h
%define LANG_HUNGARIAN 0Eh
%define LANG_ICELANDIC 0Fh
%define LANG_ITALIAN 10h
%define LANG_JAPANESE 11h
%define LANG_KOREAN 12h
%define LANG_NORWEGIAN 14h
%define LANG_POLISH 15h
%define LANG_PORTUGUESE 16h
%define LANG_ROMANIAN 18h
%define LANG_RUSSIAN 19h
%define LANG_SLOVAK 1Bh
%define LANG_SLOVENIAN 24h
%define LANG_SPANISH 0Ah
%define LANG_SWEDISH 1Dh
%define LANG_TURKISH 1Fh
%define SUBLANG_NEUTRAL 0h
%define SUBLANG_DEFAULT 1h
%define SUBLANG_SYS_DEFAULT 2h
%define SUBLANG_CHINESE_TRADITIONAL 1h
%define SUBLANG_CHINESE_SIMPLIFIED 2h
%define SUBLANG_CHINESE_HONGKONG 3h
%define SUBLANG_CHINESE_SINGAPORE 4h
%define SUBLANG_DUTCH 1h
%define SUBLANG_DUTCH_BELGIAN 2h
%define SUBLANG_ENGLISH_US 1h
%define SUBLANG_ENGLISH_UK 2h
%define SUBLANG_ENGLISH_AUS 3h
%define SUBLANG_ENGLISH_CAN 4h
%define SUBLANG_ENGLISH_NZ 5h
%define SUBLANG_ENGLISH_EIRE 6h
%define SUBLANG_FRENCH 1h
%define SUBLANG_FRENCH_BELGIAN 2h
%define SUBLANG_FRENCH_CANADIAN 3h
%define SUBLANG_FRENCH_SWISS 4h
%define SUBLANG_GERMAN 1h
%define SUBLANG_GERMAN_SWISS 2h
%define SUBLANG_GERMAN_AUSTRIAN 3h
%define SUBLANG_ITALIAN 1h
%define SUBLANG_ITALIAN_SWISS 2h
%define SUBLANG_NORWEGIAN_BOKMAL 1h
%define SUBLANG_NORWEGIAN_NYNORSK 2h
%define SUBLANG_PORTUGUESE 2h
%define SUBLANG_PORTUGUESE_BRAZILIAN 1h
%define SUBLANG_SPANISH 1h
%define SUBLANG_SPANISH_MEXICAN 2h
%define SUBLANG_SPANISH_MODERN 3h
%define SORT_DEFAULT 0h
%define SORT_JAPANESE_XJIS 0h
%define SORT_JAPANESE_UNICODE 1h
%define SORT_CHINESE_BIG5 0h
%define SORT_CHINESE_UNICODE 1h
%define SORT_KOREAN_KSC 0h
%define SORT_KOREAN_UNICODE 1h
%define FILE_READ_DATA 1h
%define FILE_LIST_DIRECTORY 1h
%define FILE_WRITE_DATA 2h
%define FILE_ADD_FILE 2h
%define FILE_APPEND_DATA 4h
%define FILE_ADD_SUBDIRECTORY 4h
%define FILE_CREATE_PIPE_INSTANCE 4h
%define FILE_READ_EA 8h
%define FILE_READ_PROPERTIES FILE_READ_EA
%define FILE_WRITE_EA 10h
%define FILE_WRITE_PROPERTIES FILE_WRITE_EA
%define FILE_EXECUTE 20h
%define FILE_TRAVERSE 20h
%define FILE_DELETE_CHILD 40h
%define FILE_READ_ATTRIBUTES 80h
%define FILE_WRITE_ATTRIBUTES 100h
%define FILE_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|1FFh
%define FILE_GENERIC_READ STANDARD_RIGHTS_READ|FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|SYNCHRONIZE
%define FILE_GENERIC_WRITE STANDARD_RIGHTS_WRITE|FILE_WRITE_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_APPEND_DATA|SYNCHRONIZE
%define FILE_GENERIC_EXECUTE STANDARD_RIGHTS_EXECUTE|FILE_READ_ATTRIBUTES|FILE_EXECUTE|SYNCHRONIZE
%define FILE_SHARE_READ 1h
%define FILE_SHARE_DELETE               4
%define FILE_SHARE_WRITE 2h
%define FILE_ATTRIBUTE_READONLY 1h
%define FILE_ATTRIBUTE_HIDDEN 2h
%define FILE_ATTRIBUTE_SYSTEM 4h
%define FILE_ATTRIBUTE_DIRECTORY 10h
%define FILE_ATTRIBUTE_ARCHIVE 20h
%define FILE_ATTRIBUTE_NORMAL 80h
%define FILE_ATTRIBUTE_TEMPORARY 100h
%define FILE_ATTRIBUTE_COMPRESSED 800h
%define FILE_NOTIFY_CHANGE_FILE_NAME 1h
%define FILE_NOTIFY_CHANGE_DIR_NAME 2h
%define FILE_NOTIFY_CHANGE_ATTRIBUTES 4h
%define FILE_NOTIFY_CHANGE_SIZE 8h
%define FILE_NOTIFY_CHANGE_LAST_WRITE 10h
%define FILE_NOTIFY_CHANGE_SECURITY 100h
%define MAILSLOT_NO_MESSAGE -1
%define MAILSLOT_WAIT_FOREVER -1
%define FILE_CASE_SENSITIVE_SEARCH 1h
%define FILE_CASE_PRESERVED_NAMES 2h
%define FILE_UNICODE_ON_DISK 4h
%define FILE_PERSISTENT_ACLS 8h
%define FILE_FILE_COMPRESSION 10h
%define FILE_VOLUME_IS_COMPRESSED 8000h
%define IO_COMPLETION_MODIFY_STATE 2h
%define IO_COMPLETION_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|3h
%define DUPLICATE_CLOSE_SOURCE 1h
%define DUPLICATE_SAME_ACCESS 2h
%define ACCESS_SYSTEM_SECURITY 1000000h
%define MAXIMUM_ALLOWED 2000000h
%define GENERIC_READ 80000000h
%define GENERIC_WRITE 40000000h
%define GENERIC_EXECUTE 20000000h
%define GENERIC_ALL 10000000h
%define ACL_REVISION 2
%define ACL_REVISION1 1
%define ACL_REVISION2 2
%define ACCESS_ALLOWED_ACE_TYPE 0h
%define ACCESS_DENIED_ACE_TYPE 1h
%define SYSTEM_AUDIT_ACE_TYPE 2h
%define SYSTEM_ALARM_ACE_TYPE 3h
%define HELPINFO_WINDOW 1
%define HELPINFO_MENUITEM 2
%define OBJECT_INHERIT_ACE 1h
%define CONTAINER_INHERIT_ACE 2h
%define NO_PROPAGATE_INHERIT_ACE 4h
%define INHERIT_ONLY_ACE 8h
%define VALID_INHERIT_FLAGS 0Fh
%define SUCCESSFUL_ACCESS_ACE_FLAG 40h
%define FAILED_ACCESS_ACE_FLAG 80h
%define AclRevisionInformation 1
%define AclSizeInformation 2
%define SECURITY_DESCRIPTOR_REVISION 1
%define SECURITY_DESCRIPTOR_REVISION1 1
%define SECURITY_DESCRIPTOR_MIN_LENGTH 20
%define SE_OWNER_DEFAULTED 1h
%define SE_GROUP_DEFAULTED 2h
%define SE_DACL_PRESENT 4h
%define SE_DACL_DEFAULTED 8h
%define SE_SACL_PRESENT 10h
%define SE_SACL_DEFAULTED 20h
%define SE_SELF_RELATIVE 8000h
%define SE_PRIVILEGE_ENABLED_BY_DEFAULT 1h
%define SE_PRIVILEGE_ENABLED 2h
%define SE_PRIVILEGE_USED_FOR_ACCESS 80000000h
%define PRIVILEGE_SET_ALL_NECESSARY 1
%define SecurityAnonymous 1
%define SecurityIdentification 2
%define REG_OPTION_RESERVED 0
%define REG_OPTION_NON_VOLATILE 0
%define REG_OPTION_VOLATILE 1
%define REG_OPTION_CREATE_LINK 2
%define REG_OPTION_BACKUP_RESTORE 4
%define REG_NONE 0
%define REG_SZ 1
%define REG_EXPAND_SZ 2
%define REG_BINARY 3
%define REG_DWORD 4
%define REG_DWORD_LITTLE_ENDIAN 4
%define REG_DWORD_BIG_ENDIAN 5
%define REG_LINK 6
%define REG_MULTI_SZ 7
%define REG_RESOURCE_LIST 8
%define REG_FULL_RESOURCE_DESCRIPTOR 9
%define REG_RESOURCE_REQUIREMENTS_LIST 10
%define REG_CREATED_NEW_KEY 1h
%define REG_OPENED_EXISTING_KEY 2h
%define REG_WHOLE_HIVE_VOLATILE 1h
%define REG_REFRESH_HIVE 2h
%define REG_NOTIFY_CHANGE_NAME 1h
%define REG_NOTIFY_CHANGE_ATTRIBUTES 2h
%define REG_NOTIFY_CHANGE_LAST_SET 4h
%define REG_NOTIFY_CHANGE_SECURITY 8h
%define REG_LEGAL_CHANGE_FILTER REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET|REG_NOTIFY_CHANGE_SECURITY
%define REG_LEGAL_OPTION REG_OPTION_RESERVED|REG_OPTION_NON_VOLATILE|REG_OPTION_VOLATILE|REG_OPTION_CREATE_LINK|REG_OPTION_BACKUP_RESTORE
%define KEY_QUERY_VALUE 1h
%define KEY_SET_VALUE 2h
%define KEY_CREATE_SUB_KEY 4h
%define KEY_ENUMERATE_SUB_KEYS 8h
%define KEY_NOTIFY 10h
%define KEY_CREATE_LINK 20h
%define KEY_READ STANDARD_RIGHTS_READ|KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_NOTIFY&(-1-SYNCHRONIZE)
%define KEY_WRITE STANDARD_RIGHTS_WRITE|KEY_SET_VALUE|KEY_CREATE_SUB_KEY|SYNCHRONIZE&(-1-SYNCHRONIZE)
%define KEY_EXECUTE KEY_READ
%define KEY_ALL_ACCESS STANDARD_RIGHTS_ALL|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_CREATE_SUB_KEY|KEY_ENUMERATE_SUB_KEYS|KEY_NOTIFY|KEY_CREATE_LINK&(-1-SYNCHRONIZE)
%define EXCEPTION_DEBUG_EVENT 1
%define CREATE_THREAD_DEBUG_EVENT 2
%define CREATE_PROCESS_DEBUG_EVENT 3
%define EXIT_THREAD_DEBUG_EVENT 4
%define EXIT_PROCESS_DEBUG_EVENT 5
%define LOAD_DLL_DEBUG_EVENT 6
%define UNLOAD_DLL_DEBUG_EVENT 7
%define OUTPUT_DEBUG_STRING_EVENT 8
%define RIP_EVENT 9
%define EXCEPTION_MAXIMUM_PARAMETERS 15
%define DRIVE_REMOVABLE 2
%define DRIVE_FIXED 3
%define DRIVE_REMOTE 4
%define DRIVE_CDROM 5
%define DRIVE_RAMDISK 6
%define FILE_TYPE_UNKNOWN 0h
%define FILE_TYPE_DISK 1h
%define FILE_TYPE_CHAR 2h
%define FILE_TYPE_PIPE 3h
%define FILE_TYPE_REMOTE 8000h
%define STD_INPUT_HANDLE -10
%define STD_OUTPUT_HANDLE -11
%define STD_ERROR_HANDLE -12
%define NOPARITY 0
%define ODDPARITY 1
%define EVENPARITY 2
%define MARKPARITY 3
%define SPACEPARITY 4
%define ONESTOPBIT 0
%define ONE5STOPBITS 1
%define TWOSTOPBITS 2
%define IGNORE 0
%define INFINITE 0FFFFh
%define CBR_110 110
%define CBR_300 300
%define CBR_600 600
%define CBR_1200 1200
%define CBR_2400 2400
%define CBR_4800 4800
%define CBR_9600 9600
%define CBR_14400 14400
%define CBR_19200 19200
%define CBR_38400 38400
%define CBR_56000 56000
%define CBR_57600 57600
%define CBR_115200 115200
%define CBR_128000 128000
%define CBR_256000 256000
%define CE_RXOVER 1h
%define CE_OVERRUN 2h
%define CE_RXPARITY 4h
%define CE_FRAME 8h
%define CE_BREAK 10h
%define CE_TXFULL 100h
%define CE_PTO 200h
%define CE_IOE 400h
%define CE_DNS 800h
%define CE_OOP 1000h
%define CE_MODE 8000h
%define IE_BADID -1
%define IE_OPEN -2
%define IE_NOPEN -3
%define IE_MEMORY -4
%define IE_DEFAULT -5
%define IE_HARDWARE -10
%define IE_BYTESIZE -11
%define IE_BAUDRATE -12
%define EV_RXCHAR 1h
%define EV_RXFLAG 2h
%define EV_TXEMPTY 4h
%define EV_CTS 8h
%define EV_DSR 10h
%define EV_RLSD 20h
%define EV_BREAK 40h
%define EV_ERR 80h
%define EV_RING 100h
%define EV_PERR 200h
%define EV_RX80FULL 400h
%define EV_EVENT1 800h
%define EV_EVENT2 1000h
%define SETXOFF 1
%define SETXON 2
%define SETRTS 3
%define CLRRTS 4
%define SETDTR 5
%define CLRDTR 6
%define RESETDEV 7
%define SETBREAK 8
%define CLRBREAK 9
%define PURGE_TXABORT 1h
%define PURGE_RXABORT 2h
%define PURGE_TXCLEAR 4h
%define PURGE_RXCLEAR 8h
%define LPTx 80h
%define MS_CTS_ON 10h
%define MS_DSR_ON 20h
%define MS_RING_ON 40h
%define MS_RLSD_ON 80h
%define S_QUEUEEMPTY 0
%define S_THRESHOLD 1
%define S_ALLTHRESHOLD 2
%define S_NORMAL 0
%define S_LEGATO 1
%define S_STACCATO 2
%define S_PERIOD512 0
%define S_PERIOD1024 1
%define S_PERIOD2048 2
%define S_PERIODVOICE 3
%define S_WHITE512 4
%define S_WHITE1024 5
%define S_WHITE2048 6
%define S_WHITEVOICE 7
%define S_SERDVNA -1
%define S_SEROFM -2
%define S_SERMACT -3
%define S_SERQFUL -4
%define S_SERBDNT -5




;	ReadEventLog, ReportEvent
%assign	EVENTLOG_FORWARDS_READ		4
%assign	EVENTLOG_BACKWARDS_READ		8
%assign	EVENTLOG_SEEK_READ		2
%assign	EVENTLOG_SEQUENTIAL_READ	1
%assign	EVENTLOG_ERROR_TYPE		1
%assign	EVENTLOG_WARNING_TYPE		2
%assign	EVENTLOG_INFORMATION_TYPE	4
%assign	EVENTLOG_AUDIT_SUCCESS		8
%assign	EVENTLOG_AUDIT_FAILURE		16
;	QueryServiceObjectSecurity
%assign	OWNER_SECURITY_INFORMATION	0x1
%assign	GROUP_SECURITY_INFORMATION	0x2
%assign	DACL_SECURITY_INFORMATION	0x4
%assign	SACL_SECURITY_INFORMATION	0x8


%define S_SERDLN -6
%define S_SERDCC -7
%define S_SERDTP -8
%define S_SERDVL -9
%define S_SERDMD -10
%define S_SERDSH -11
%define S_SERDPT -12
%define S_SERDFQ -13
%define S_SERDDR -14
%define S_SERDSR -15
%define S_SERDST -16
%define NMPWAIT_WAIT_FOREVER 0FFFFh
%define NMPWAIT_NOWAIT 1h
%define NMPWAIT_USE_DEFAULT_WAIT 0h
%define FS_CASE_IS_PRESERVED FILE_CASE_PRESERVED_NAMES
%define FS_CASE_SENSITIVE FILE_CASE_SENSITIVE_SEARCH
%define FS_UNICODE_STORED_ON_DISK FILE_UNICODE_ON_DISK
%define FS_PERSISTENT_ACLS FILE_PERSISTENT_ACLS
%define SECTION_QUERY 1h
%define SECTION_MAP_WRITE 2h
%define SECTION_MAP_READ 4h
%define SECTION_MAP_EXECUTE 8h
%define SECTION_EXTEND_SIZE 10h
%define SECTION_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|SECTION_MAP_WRITE|SECTION_MAP_READ|SECTION_MAP_EXECUTE|SECTION_EXTEND_SIZE
%define FILE_MAP_COPY SECTION_QUERY
%define FILE_MAP_WRITE SECTION_MAP_WRITE
%define FILE_MAP_READ SECTION_MAP_READ
%define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS
%define OF_READ 0h
%define OF_WRITE 1h
%define OF_READWRITE 2h
%define OF_SHARE_COMPAT 0h
%define OF_SHARE_EXCLUSIVE 10h
%define OF_SHARE_DENY_WRITE 20h
%define OF_SHARE_DENY_READ 30h
%define OF_SHARE_DENY_NONE 40h
%define OF_PARSE 100h
%define OF_DELETE 200h
%define OF_VERIFY 400h
%define OF_CANCEL 800h
%define OF_CREATE 1000h
%define OF_PROMPT 2000h
%define OF_EXIST 4000h
%define OF_REOPEN 8000h
%define OFS_MAXPATHNAME 128
%define DONT_RESOLVE_DLL_REFERENCES 1h
%define TC_NORMAL 0
%define TC_HARDERR 1
%define TC_GP_TRAP 2
%define TC_SIGNAL 3
%define MAX_LEADBYTES 12
%define MB_PRECOMPOSED 1h
%define MB_COMPOSITE 2h
%define MB_USEGLYPHCHARS 4h
%define WC_DEFAULTCHECK 100h
%define WC_COMPOSITECHECK 200h
%define WC_DISCARDNS 10h
%define WC_SEPCHARS 20h
%define WC_DEFAULTCHAR 40h
%define CT_CTYPE1 1h
%define CT_CTYPE2 2h
%define CT_CTYPE3 4h
%define C1_UPPER 1h
%define C1_LOWER 2h
%define C1_DIGIT 4h
%define C1_SPACE 8h
%define C1_PUNCT 10h
%define C1_CNTRL 20h
%define C1_BLANK 40h
%define C1_XDIGIT 80h
%define C1_ALPHA 100h
%define C2_LEFTTORIGHT 1h
%define C2_RIGHTTOLEFT 2h
%define C2_EUROPENUMBER 3h
%define C2_EUROPESEPARATOR 4h
%define C2_EUROPETERMINATOR 5h
%define C2_ARABICNUMBER 6h
%define C2_COMMONSEPARATOR 7h
%define C2_BLOCKSEPARATOR 8h
%define C2_SEGMENTSEPARATOR 9h
%define C2_WHITESPACE 0Ah
%define C2_OTHERNEUTRAL 0Bh
%define C2_NOTAPPLICABLE 0h
%define C3_NONSPACING 1h
%define C3_DIACRITIC 2h
%define C3_VOWELMARK 4h
%define C3_SYMBOL 8h
%define C3_NOTAPPLICABLE 0h
%define NORM_IGNORECASE 1h
%define NORM_IGNORENONSPACE 2h
%define NORM_IGNORESYMBOLS 4h
%define MAP_FOLDCZONE 10h
%define MAP_PRECOMPOSED 20h
%define MAP_COMPOSITE 40h
%define MAP_FOLDDIGITS 80h
%define LCMAP_LOWERCASE 100h
%define LCMAP_UPPERCASE 200h
%define LCMAP_SORTKEY 400h
%define LCMAP_BYTEREV 800h
%define SORT_STRINGSORT 1000h
%define CP_ACP 0
%define CP_OEMCP 1
%define CTRY_DEFAULT 0
%define CTRY_AUSTRALIA 61
%define CTRY_AUSTRIA 43
%define CTRY_BELGIUM 32
%define CTRY_BRAZIL 55
%define CTRY_CANADA 2
%define CTRY_DENMARK 45
%define CTRY_FINLAND 358
%define CTRY_FRANCE 33
%define CTRY_GERMANY 49
%define CTRY_ICELAND 354
%define CTRY_IRELAND 353
%define CTRY_ITALY 39
%define CTRY_JAPAN 81
%define CTRY_MEXICO 52
%define CTRY_NETHERLANDS 31
%define CTRY_NEW_ZEALAND 64
%define CTRY_NORWAY 47
%define CTRY_PORTUGAL 351
%define CTRY_PRCHINA 86
%define CTRY_SOUTH_KOREA 82
%define CTRY_SPAIN 34
%define CTRY_SWEDEN 46
%define CTRY_SWITZERLAND 41
%define CTRY_TAIWAN 886
%define CTRY_UNITED_KINGDOM 44
%define CTRY_UNITED_STATES 1
%define LOCALE_NOUSEROVERRIDE 80000000h
%define LOCALE_USER_DEFAULT 0000h
%define LOCALE_ILANGUAGE 1h
%define LOCALE_SLANGUAGE 2h
%define LOCALE_SENGLANGUAGE 1001h
%define LOCALE_SABBREVLANGNAME 3h
%define LOCALE_SNATIVELANGNAME 4h
%define LOCALE_ICOUNTRY 5h
%define LOCALE_SCOUNTRY 6h
%define LOCALE_SENGCOUNTRY 1002h
%define LOCALE_SABBREVCTRYNAME 7h
%define LOCALE_SNATIVECTRYNAME 8h
%define LOCALE_IDEFAULTLANGUAGE 9h
%define LOCALE_IDEFAULTCOUNTRY 0Ah
%define LOCALE_IDEFAULTCODEPAGE 0Bh
%define LOCALE_SLIST 0Ch
%define LOCALE_IMEASURE 0Dh
%define LOCALE_SDECIMAL 0Eh
%define LOCALE_STHOUSAND 0Fh
%define LOCALE_SGROUPING 10h
%define LOCALE_IDIGITS 11h
%define LOCALE_ILZERO 12h
%define LOCALE_SNATIVEDIGITS 13h
%define LOCALE_SCURRENCY 14h
%define LOCALE_SINTLSYMBOL 15h
%define LOCALE_SMONDECIMALSEP 16h
%define LOCALE_SMONTHOUSANDSEP 17h
%define LOCALE_SMONGROUPING 18h
%define LOCALE_ICURRDIGITS 19h
%define LOCALE_IINTLCURRDIGITS 1Ah
%define LOCALE_ICURRENCY 1Bh
%define LOCALE_INEGCURR 1Ch
%define LOCALE_SDATE 1Dh
%define LOCALE_STIME 1Eh
%define LOCALE_SSHORTDATE 1Fh
%define LOCALE_SLONGDATE 20h
%define LOCALE_STIMEFORMAT 1003h
%define LOCALE_IDATE 21h
%define LOCALE_ILDATE 22h
%define LOCALE_ITIME 23h
%define LOCALE_ICENTURY 24h
%define LOCALE_ITLZERO 25h
%define LOCALE_IDAYLZERO 26h
%define LOCALE_IMONLZERO 27h
%define LOCALE_S1159 28h
%define LOCALE_S2359 29h
%define LOCALE_SDAYNAME1 2Ah
%define LOCALE_SDAYNAME2 2Bh
%define LOCALE_SDAYNAME3 2Ch
%define LOCALE_SDAYNAME4 2Dh
%define LOCALE_SDAYNAME5 2Eh
%define LOCALE_SDAYNAME6 2Fh
%define LOCALE_SDAYNAME7 30h
%define LOCALE_SABBREVDAYNAME1 31h
%define LOCALE_SABBREVDAYNAME2 32h
%define LOCALE_SABBREVDAYNAME3 33h
%define LOCALE_SABBREVDAYNAME4 34h
%define LOCALE_SABBREVDAYNAME5 35h
%define LOCALE_SABBREVDAYNAME6 36h
%define LOCALE_SABBREVDAYNAME7 37h
%define LOCALE_SMONTHNAME1 38h
%define LOCALE_SMONTHNAME2 39h
%define LOCALE_SMONTHNAME3 3Ah
%define LOCALE_SMONTHNAME4 3Bh
%define LOCALE_SMONTHNAME5 3Ch
%define LOCALE_SMONTHNAME6 3Dh
%define LOCALE_SMONTHNAME7 3Eh
%define LOCALE_SMONTHNAME8 3Fh
%define LOCALE_SMONTHNAME9 40h
%define LOCALE_SMONTHNAME10 41h
%define LOCALE_SMONTHNAME11 42h
%define LOCALE_SMONTHNAME12 43h
%define LOCALE_SABBREVMONTHNAME1 44h
%define LOCALE_SABBREVMONTHNAME2 45h
%define LOCALE_SABBREVMONTHNAME3 46h
%define LOCALE_SABBREVMONTHNAME4 47h
%define LOCALE_SABBREVMONTHNAME5 48h
%define LOCALE_SABBREVMONTHNAME6 49h
%define LOCALE_SABBREVMONTHNAME7 4Ah
%define LOCALE_SABBREVMONTHNAME8 4Bh
%define LOCALE_SABBREVMONTHNAME9 4Ch
%define LOCALE_SABBREVMONTHNAME10 4Dh
%define LOCALE_SABBREVMONTHNAME11 4Eh
%define LOCALE_SABBREVMONTHNAME12 4Fh
%define LOCALE_SABBREVMONTHNAME13 100Fh
%define LOCALE_SPOSITIVESIGN 50h
%define LOCALE_SNEGATIVESIGN 51h
%define LOCALE_IPOSSIGNPOSN 52h
%define LOCALE_INEGSIGNPOSN 53h
%define LOCALE_IPOSSYMPRECEDES 54h
%define LOCALE_IPOSSEPBYSPACE 55h
%define LOCALE_INEGSYMPRECEDES 56h
%define LOCALE_INEGSEPBYSPACE 57h
%define TIME_NOMINUTESORSECONDS 1h
%define TIME_NOSECONDS 2h
%define TIME_NOTIMEMARKER 4h
%define TIME_FORCE24HOURFORMAT 8h

;       GetTimeZoneInformation
%assign TIME_ZONE_ID_UNKNOWN            0
%assign TIME_ZONE_ID_STANDARD           1
%assign TIME_ZONE_ID_DAYLIGHT           2
;       GetUserObjectInformation
%assign UOI_FLAGS                       1
%assign UOI_NAME                        2
%assign UOI_TYPE                        3
;       GetVolumeInformation
%assign FS_CASE_IS_PRESERVED            2
%assign FS_CASE_SENSITIVE               1
%assign FS_UNICODE_STORED_ON_DISK       4
%assign FS_PERSISTENT_ACLS              8
%assign FS_FILE_COMPRESSION             16
%assign FS_VOL_IS_COMPRESSED            32768

%define DATE_SHORTDATE 1h
%define DATE_LONGDATE 2h
%define TF_FORCEDRIVE 80h
%define LOCKFILE_FAIL_IMMEDIATELY 1h
%define LOCKFILE_EXCLUSIVE_LOCK 2h
%define LNOTIFY_OUTOFMEM 0
%define LNOTIFY_MOVE 1
%define LNOTIFY_DISCARD 2
%define SLE_ERROR 1h
%define SLE_MINORERROR 2h
%define SLE_WARNING 3h
%define SEM_FAILCRITICALERRORS 1h
%define SEM_NOGPFAULTERRORBOX 2h
%define SEM_NOOPENFILEERRORBOX 8000h
%define RT_CURSOR 1
%define RT_BITMAP 2
%define RT_ICON 3
%define RT_MENU 4
%define RT_DIALOG 5
%define RT_STRING 6
%define RT_FONTDIR 7
%define RT_FONT 8
%define RT_ACCELERATOR 9
%define RT_RCDATA 10
%define DFC_CAPTION 1
%define DFC_MENU 2
%define DFC_SCROLL 3
%define DFC_BUTTON 4
%define DFCS_CAPTIONCLOSE 0000h
%define DFCS_CAPTIONMIN 0001h
%define DFCS_CAPTIONMAX 0002h
%define DFCS_CAPTIONRESTORE 0003h
%define DFCS_CAPTIONHELP 0004h
%define DFCS_MENUARROW 0000h
%define DFCS_MENUCHECK 0001h
%define DFCS_MENUBULLET 0002h
%define DFCS_MENUARROWRIGHT 0004h
%define DFCS_SCROLLUP 0000h
%define DFCS_SCROLLDOWN 0001h
%define DFCS_SCROLLLEFT 0002h
%define DFCS_SCROLLRIGHT 0003h
%define DFCS_SCROLLCOMBOBOX 0005h
%define DFCS_SCROLLSIZEGRIP 0008h
%define DFCS_SCROLLSIZEGRIPRIGHT 0010h
%define DFCS_BUTTONCHECK 0000h
%define DFCS_BUTTONRADIOIMAGE 0001h
%define DFCS_BUTTONRADIOMASK 0002h
%define DFCS_BUTTONRADIO 0004h
%define DFCS_BUTTON3STATE 0008h
%define DFCS_BUTTONPUSH 0010h
%define DFCS_INACTIVE 0100h
%define DFCS_PUSHED 0200h
%define DFCS_CHECKED 0400h
%define DFCS_ADJUSTRECT 2000h
%define DFCS_FLAT 4000h
%define DFCS_MONO 8000h
%define DDD_RAW_TARGET_PATH 1h
%define DDD_REMOVE_DEFINITION 2h
%define DDD_EXACT_MATCH_ON_REMOVE 4h
%define MAX_PATH 32
%define MOVEFILE_REPLACE_EXISTING 1h
%define MOVEFILE_COPY_ALLOWED 2h
%define MOVEFILE_DELAY_UNTIL_REBOOT 4h

;	MsgWaitForMultipleObjects, WaitForMultipleObjectsEx
%assign	WAIT_OBJECT_0			0
%assign	WAIT_ABANDONED_0		0x80
%assign	WAIT_TIMEOUT			0x102
%assign	WAIT_IO_COMPLETION		0xC0
%assign	WAIT_ABANDONED			0x80
%assign	WAIT_FAILED			0xFFFFFFFF
%assign	MAXIMUM_WAIT_OBJECTS		0x40
%assign	MAXIMUM_SUSPEND_COUNT		0x7F
;	MultiByteToWideChar
%assign	MB_PRECOMPOSED			1
%assign	MB_COMPOSITE			2
%assign	MB_ERR_INVALID_CHARS		8
%assign	MB_USEGLYPHCHARS		4
;	OpenProcessToken
%assign	TOKEN_ADJUST_DEFAULT		128
%assign	TOKEN_ADJUST_GROUPS		64
%assign	TOKEN_ADJUST_PRIVILEGES		32
%assign	TOKEN_ALL_ACCESS		0xF00FF
%assign	TOKEN_ASSIGN_PRIMARY		1
%assign	TOKEN_DUPLICATE			2
%assign	TOKEN_EXECUTE			0x20000
%assign	TOKEN_IMPERSONATE		4
%assign	TOKEN_QUERY			8
%assign	TOKEN_QUERY_SOURCE		16
%assign	TOKEN_READ			0x20008
%assign	TOKEN_WRITE			0x200E0


%define TokenUser 1
%define TokenGroups 2
%define TokenPrivileges 3
%define TokenOwner 4
%define TokenPrimaryGroup 5
%define TokenDefaultDacl 6
%define TokenSource 7
%define TokenType 8
%define TokenImpersonationLevel 9
%define TokenStatistics 10
%define GET_TAPE_MEDIA_INFORMATION 0
%define GET_TAPE_DRIVE_INFORMATION 1
%define SET_TAPE_MEDIA_INFORMATION 0
%define SET_TAPE_DRIVE_INFORMATION 1
%define FORMAT_MESSAGE_ALLOCATE_BUFFER 100h
%define FORMAT_MESSAGE_IGNORE_INSERTS 200h
%define FORMAT_MESSAGE_FROM_STRING 400h
%define FORMAT_MESSAGE_FROM_HMODULE 800h
%define FORMAT_MESSAGE_FROM_SYSTEM 1000h
%define FORMAT_MESSAGE_ARGUMENT_ARRAY 2000h
%define FORMAT_MESSAGE_MAX_WIDTH_MASK 0FFh
%define TLS_OUT_OF_INDEXES 0FFFFh
%define BACKUP_DATA 1h
%define BACKUP_EA_DATA 2h
%define BACKUP_SECURITY_DATA 3h
%define BACKUP_ALTERNATE_DATA 4h
%define BACKUP_LINK 5h
%define STREAM_MODIFIED_WHEN_READ 1h
%define STREAM_CONTAINS_SECURITY 2h
%define STARTF_USESHOWWINDOW 1h
%define STARTF_USESIZE 2h
%define STARTF_USEPOSITION 4h
%define STARTF_USECOUNTCHARS 8h
%define STARTF_USEFILLATTRIBUTE 10h
%define STARTF_RUNFULLSCREEN 20h
%define STARTF_FORCEONFEEDBACK 40h
%define STARTF_FORCEOFFFEEDBACK 80h
%define STARTF_USESTDHANDLES 100h
%define SHUTDOWN_NORETRY 1h
%define MAX_DEFAULTCHAR 2
%define CAL_ICALINTVALUE 1h
%define CAL_SCALNAME 2h
%define CAL_IYEAROFFSETRANGE 3h
%define CAL_SERASTRING 4h
%define CAL_SSHORTDATE 5h
%define CAL_SLONGDATE 6h
%define CAL_SDAYNAME1 7h
%define CAL_SDAYNAME2 8h
%define CAL_SDAYNAME3 9h
%define CAL_SDAYNAME4 0Ah
%define CAL_SDAYNAME5 0Bh
%define CAL_SDAYNAME6 0Ch
%define CAL_SDAYNAME7 0Dh
%define CAL_SABBREVDAYNAME1 0Eh
%define CAL_SABBREVDAYNAME2 0Fh
%define CAL_SABBREVDAYNAME3 10h
%define CAL_SABBREVDAYNAME4 11h
%define CAL_SABBREVDAYNAME5 12h
%define CAL_SABBREVDAYNAME6 13h
%define CAL_SABBREVDAYNAME7 14h
%define CAL_SMONTHNAME1 15h
%define CAL_SMONTHNAME2 16h
%define CAL_SMONTHNAME3 17h
%define CAL_SMONTHNAME4 18h
%define CAL_SMONTHNAME5 19h
%define CAL_SMONTHNAME6 1Ah
%define CAL_SMONTHNAME7 1Bh
%define CAL_SMONTHNAME8 1Ch
%define CAL_SMONTHNAME9 1Dh
%define CAL_SMONTHNAME10 1Eh
%define CAL_SMONTHNAME11 1Fh
%define CAL_SMONTHNAME12 20h
%define CAL_SMONTHNAME13 21h
%define CAL_SABBREVMONTHNAME1 22h
%define CAL_SABBREVMONTHNAME2 23h
%define CAL_SABBREVMONTHNAME3 24h
%define CAL_SABBREVMONTHNAME4 25h
%define CAL_SABBREVMONTHNAME5 26h
%define CAL_SABBREVMONTHNAME6 27h
%define CAL_SABBREVMONTHNAME7 28h
%define CAL_SABBREVMONTHNAME8 29h
%define CAL_SABBREVMONTHNAME9 2Ah
%define CAL_SABBREVMONTHNAME10 2Bh
%define CAL_SABBREVMONTHNAME11 2Ch
%define CAL_SABBREVMONTHNAME12 2Dh
%define CAL_SABBREVMONTHNAME13 2Eh
%define ENUM_ALL_CALENDARS 0FFFFh
%define CAL_GREGORIAN 1
%define CAL_GREGORIAN_US 2
%define CAL_JAPAN 3
%define CAL_TAIWAN 4
%define CAL_KOREA 5
%define RIGHT_ALT_PRESSED 1h
%define LEFT_ALT_PRESSED 2h
%define RIGHT_CTRL_PRESSED 4h
%define LEFT_CTRL_PRESSED 8h
%define SHIFT_PRESSED 10h
%define NUMLOCK_ON 20h
%define SCROLLLOCK_ON 40h
%define CAPSLOCK_ON 80h
%define ENHANCED_KEY 100h
%define FROM_LEFT_1ST_BUTTON_PRESSED 1h
%define RIGHTMOST_BUTTON_PRESSED 2h
%define FROM_LEFT_2ND_BUTTON_PRESSED 4h
%define FROM_LEFT_3RD_BUTTON_PRESSED 8h
%define FROM_LEFT_4TH_BUTTON_PRESSED 10h
%define MOUSE_MOVED 1h
%define DOUBLE_CLICK 2h
%define KEY_EVENT 1h
%define mouse_eventC 2h
%define WINDOW_BUFFER_SIZE_EVENT 4h
%define MENU_EVENT 8h
%define FOCUS_EVENT 10h
%define FOREGROUND_BLUE 1h
%define FOREGROUND_GREEN 2h
%define FOREGROUND_RED 4h
%define FOREGROUND_INTENSITY 8h
%define BACKGROUND_BLUE 10h
%define BACKGROUND_GREEN 20h
%define BACKGROUND_RED 40h
%define BACKGROUND_INTENSITY 80h
%define CTRL_C_EVENT 0
%define CTRL_BREAK_EVENT 1
%define CTRL_CLOSE_EVENT 2
%define CTRL_LOGOFF_EVENT 5
%define CTRL_SHUTDOWN_EVENT 6
%define ENABLE_PROCESSED_INPUT 1h
%define ENABLE_LINE_INPUT 2h
%define ENABLE_ECHO_INPUT 4h
%define ENABLE_WINDOW_INPUT 8h
%define ENABLE_MOUSE_INPUT 10h
%define ENABLE_PROCESSED_OUTPUT 1h
%define ENABLE_WRAP_AT_EOL_OUTPUT 2h
%define CONSOLE_TEXTMODE_BUFFER 1
%define R2_BLACK 1
%define R2_NOTMERGEPEN 2
%define R2_MASKNOTPEN 3
%define R2_NOTCOPYPEN 4
%define R2_MASKPENNOT 5
%define R2_NOT 6
%define R2_XORPEN 7
%define R2_NOTMASKPEN 8
%define R2_MASKPEN 9
%define R2_NOTXORPEN 10
%define R2_NOP 11
%define R2_MERGENOTPEN 12
%define R2_COPYPEN 13
%define R2_MERGEPENNOT 14
%define R2_MERGEPEN 15
%define R2_WHITE 16
%define R2_LAST 16
%define SRCCOPY 0CC0020h
%define SRCPAINT 0EE0086h
%define SRCAND 8800C6h
%define SRCINVERT 660046h
%define SRCERASE 440328h
%define NOTSRCCOPY 330008h
%define NOTSRCERASE 1100A6h
%define MERGECOPY 0C000CAh
%define MERGEPAINT 0BB0226h
%define PATCOPY 0F00021h
%define PATPAINT 0FB0A09h
%define PATINVERT 5A0049h
%define DSTINVERT 550009h
%define BLACKNESS 42h
%define WHITENESS 0FF0062h
%define GDI_ERROR 0FFFFh
%define HGDI_ERROR 0FFFFh
%define ERRORAPI 0
%define NULLREGION 1
%define SIMPLEREGION 2
%define COMPLEXREGION 3
%define RGN_AND 1
%define RGN_OR 2
%define RGN_XOR 3
%define RGN_DIFF 4
%define RGN_COPY 5
%define RGN_MIN RGN_AND
%define RGN_MAX RGN_COPY
%define BLACKONWHITE 1
%define WHITEONBLACK 2
%define COLORONCOLOR 3
%define HALFTONE 4
%define MAXSTRETCHBLTMODE 4
%define ALTERNATE 1
%define WINDING 2
%define POLYFILL_LAST 2
%define TA_NOUPDATECP 0
%define TA_UPDATECP 1
%define TA_LEFT 0
%define TA_RIGHT 2
%define TA_CENTER 6
%define TA_TOP 0
%define TA_BOTTOM 8
%define TA_BASELINE 24
%define TA_MASK TA_BASELINE+TA_CENTER+TA_UPDATECP
%define VTA_BASELINE TA_BASELINE
%define VTA_LEFT TA_BOTTOM
%define VTA_RIGHT TA_TOP
%define VTA_CENTER TA_CENTER
%define VTA_BOTTOM TA_RIGHT
%define VTA_TOP TA_LEFT
%define ETO_GRAYED 1
%define ETO_OPAQUE 2
%define ETO_CLIPPED 4
%define ASPECT_FILTERING 1h
%define DCB_RESET 1h
%define DCB_ACCUMULATE 2h
%define DCB_DIRTY DCB_ACCUMULATE
%define DCB_SET DCB_RESET|DCB_ACCUMULATE
%define DCB_ENABLE 4h
%define DCB_DISABLE 8h
%define META_SETBKCOLOR 201h
%define META_SETBKMODE 102h
%define META_SETMAPMODE 103h
%define META_SETROP2 104h
%define META_SETRELABS 105h
%define META_SETPOLYFILLMODE 106h
%define META_SETSTRETCHBLTMODE 107h
%define META_SETTEXTCHAREXTRA 108h
%define META_SETTEXTCOLOR 209h
%define META_SETTEXTJUSTIFICATION 20Ah
%define META_SETWINDOWORG 20Bh
%define META_SETWINDOWEXT 20Ch
%define META_SETVIEWPORTORG 20Dh
%define META_SETVIEWPORTEXT 20Eh
%define META_OFFSETWINDOWORG 20Fh
%define META_SCALEWINDOWEXT 410h
%define META_OFFSETVIEWPORTORG 211h
%define META_SCALEVIEWPORTEXT 412h
%define META_LINETO 213h
%define META_MOVETO 214h
%define META_EXCLUDECLIPRECT 415h
%define META_INTERSECTCLIPRECT 416h
%define META_ARC 817h
%define META_ELLIPSE 418h
%define META_FLOODFILL 419h
%define META_PIE 81Ah
%define META_RECTANGLE 41Bh
%define META_ROUNDRECT 61Ch
%define META_PATBLT 61Dh
%define META_SAVEDC 1Eh
%define META_SETPIXEL 41Fh
%define META_OFFSETCLIPRGN 220h
%define META_TEXTOUT 521h
%define META_BITBLT 922h
%define META_STRETCHBLT 0B23h
%define META_POLYGON 324h
%define META_POLYLINE 325h
%define META_ESCAPE 626h
%define META_RESTOREDC 127h
%define META_FILLREGION 228h
%define META_FRAMEREGION 429h
%define META_INVERTREGION 12Ah
%define META_PAINTREGION 12Bh
%define META_SELECTCLIPREGION 12Ch
%define META_SELECTOBJECT 12Dh
%define META_SETTEXTALIGN 12Eh
%define META_CHORD 830h
%define META_SETMAPPERFLAGS 231h
%define META_EXTTEXTOUT 0A32h
%define META_SETDIBTODEV 0D33h
%define META_SELECTPALETTE 234h
%define META_REALIZEPALETTE 35h
%define META_ANIMATEPALETTE 436h
%define META_SETPALENTRIES 37h
%define META_POLYPOLYGON 538h
%define META_RESIZEPALETTE 139h
%define META_DIBBITBLT 940h
%define META_DIBSTRETCHBLT 0B41h
%define META_DIBCREATEPATTERNBRUSH 142h
%define META_STRETCHDIB 0F43h
%define META_EXTFLOODFILL 548h
%define META_DELETEOBJECT 1F0h
%define META_CREATEPALETTE 0F7h
%define META_CREATEPATTERNBRUSH 1F9h
%define META_CREATEPENINDIRECT 2FAh
%define META_CREATEFONTINDIRECT 2FBh
%define META_CREATEBRUSHINDIRECT 2FCh
%define META_CREATEREGION 6FFh
%define NEWFRAME 1
%define AbortDocC 2
%define NEXTBAND 3
%define SETCOLORTABLE 4
%define GETCOLORTABLE 5
%define FLUSHOUTPUT 6
%define DRAFTMODE 7
%define QUERYESCSUPPORT 8
%define SETABORTPROC 9
%define StartDocC 10
%define EndDocC 11
%define GETPHYSPAGESIZE 12
%define GETPRINTINGOFFSET 13
%define GETSCALINGFACTOR 14
%define MFCOMMENT 15
%define GETPENWIDTH 16
%define SETCOPYCOUNT 17
%define SELECTPAPERSOURCE 18
%define DEVICEDATA 19
%define PASSTHROUGH 19
%define GETTECHNOLGY 20
%define GETTECHNOLOGY 20
%define SETLINECAP 21
%define SETLINEJOIN 22
%define SetMiterLimitC 23
%define BANDINFO 24
%define DRAWPATTERNRECT 25
%define GETVECTORPENSIZE 26
%define GETVECTORBRUSHSIZE 27
%define ENABLEDUPLEX 28
%define GETSETPAPERBINS 29
%define GETSETPRINTORIENT 30
%define ENUMPAPERBINS 31
%define SETDIBSCALING 32
%define EPSPRINTING 33
%define ENUMPAPERMETRICS 34
%define GETSETPAPERMETRICS 35
%define POSTSCRIPT_DATA 37
%define POSTSCRIPT_IGNORE 38
%define MOUSETRAILS 39
%define GETDEVICEUNITS 42
%define GETEXTENDEDTEXTMETRICS 256
%define GETEXTENTTABLE 257
%define GETPAIRKERNTABLE 258
%define GETTRACKKERNTABLE 259
%define ExtTextOutC 512
%define GETFACENAME 513
%define DOWNLOADFACE 514
%define ENABLERELATIVEWIDTHS 768
%define ENABLEPAIRKERNING 769
%define SETKERNTRACK 770
%define SETALLJUSTVALUES 771
%define SETCHARSET 772
%define StretchBltC 2048
%define GETSETSCREENPARAMS 3072
%define BEGIN_PATH 4096
%define CLIP_TO_PATH 4097
%define END_PATH 4098
%define EXT_DEVICE_CAPS 4099
%define RESTORE_CTM 4100
%define SAVE_CTM 4101
%define SET_ARC_DIRECTION 4102
%define SET_BACKGROUND_COLOR 4103
%define SET_POLY_MODE 4104
%define SET_SCREEN_ANGLE 4105
%define SET_SPREAD 4106
%define TRANSFORM_CTM 4107
%define SET_CLIP_BOX 4108
%define SET_BOUNDS 4109
%define SET_MIRROR_MODE 4110
%define OPENCHANNEL 4110
%define DOWNLOADHEADER 4111
%define CLOSECHANNEL 4112
%define POSTSCRIPT_PASSTHROUGH 4115
%define ENCAPSULATED_POSTSCRIPT 4116
%define SP_NOTREPORTED 4000h
%define SP_ERROR -1
%define SP_APPABORT -2
%define SP_USERABORT -3
%define SP_OUTOFDISK -4
%define SP_OUTOFMEMORY -5
%define PR_JOBSTATUS 0h
%define OBJ_PEN 1
%define OBJ_BRUSH 2
%define OBJ_DC 3
%define OBJ_METADC 4
%define OBJ_PAL 5
%define OBJ_FONT 6
%define OBJ_BITMAP 7
%define OBJ_REGION 8
%define OBJ_METAFILE 9
%define OBJ_MEMDC 10
%define OBJ_EXTPEN 11
%define OBJ_ENHMETADC 12
%define OBJ_ENHMETAFILE 13
%define MWT_IDENTITY 1
%define MWT_LEFTMULTIPLY 2
%define MWT_RIGHTMULTIPLY 3
%define MWT_MIN MWT_IDENTITY
%define MWT_MAX MWT_RIGHTMULTIPLY
%define BI_RGB 0
%define BI_RLE8 1
%define BI_RLE4 2
%define BI_bitfields 3
;	BITMAPINFOHEADER structure
%assign	BI_RGB				0
%assign	BI_RLE8				1
%assign	BI_RLE4				2
%assign	BI_BITFIELDS			3
%define NTM_REGULAR 40h
%define NTM_BOLD 20h
%define NTM_ITALIC 1h
%define TMPF_FIXED_PITCH 1h
%define TMPF_VECTOR 2h
%define TMPF_DEVICE 8h
%define TMPF_TRUETYPE 4h
%define LF_FACESIZE 32
%define LF_FULLFACESIZE 64
%define OUT_DEFAULT_PRECIS 0
%define OUT_STRING_PRECIS 1
%define OUT_CHARACTER_PRECIS 2
%define OUT_STROKE_PRECIS 3
%define OUT_TT_PRECIS 4
%define OUT_DEVICE_PRECIS 5
%define OUT_RASTER_PRECIS 6
%define OUT_TT_ONLY_PRECIS 7
%define OUT_OUTLINE_PRECIS 8
%define CLIP_DEFAULT_PRECIS 0
%define CLIP_CHARACTER_PRECIS 1
%define CLIP_STROKE_PRECIS 2
%define CLIP_MASK 0Fh
%define CLIP_LH_ANGLES 16
%define CLIP_TT_ALWAYS 32
%define CLIP_EMBEDDED 128
%define DEFAULT_QUALITY 0
%define DRAFT_QUALITY 1
%define PROOF_QUALITY 2
%define DEFAULT_PITCH 0
%define FIXED_PITCH 1
%define VARIABLE_PITCH 2
%define ANSI_CHARSET 0
%define DEFAULT_CHARSET 1
%define SYMBOL_CHARSET 2
%define SHIFTJIS_CHARSET 128
%define HANGEUL_CHARSET 129
%define CHINESEBIG5_CHARSET 136
%define OEM_CHARSET 255
%define FF_DONTCARE 0
%define FF_ROMAN 16
%define FF_SWISS 32
%define FF_MODERN 48
%define FF_SCRIPT 64
%define FF_DECORATIVE 80
%define FW_DONTCARE 0
%define FW_THIN 100
%define FW_EXTRALIGHT 200
%define FW_LIGHT 300
%define FW_NORMAL 400
%define FW_MEDIUM 500
%define FW_SEMIBOLD 600
%define FW_BOLD 700
%define FW_EXTRABOLD 800
%define FW_HEAVY 900
%define FW_ULTRALIGHT FW_EXTRALIGHT
%define FW_REGULAR FW_NORMAL
%define FW_DEMIBOLD FW_SEMIBOLD
%define FW_ULTRABOLD FW_EXTRABOLD
%define FW_BLACK FW_HEAVY
%define PANOSE_COUNT 10
%define PAN_FAMILYTYPE_INDEX 0
%define PAN_SERIFSTYLE_INDEX 1
%define PAN_WEIGHT_INDEX 2
%define PAN_PROPORTION_INDEX 3
%define PAN_CONTRAST_INDEX 4
%define PAN_STROKEVARIATION_INDEX 5
%define PAN_ARMSTYLE_INDEX 6
%define PAN_LETTERFORM_INDEX 7
%define PAN_MIDLINE_INDEX 8
%define PAN_XHEIGHT_INDEX 9
%define PAN_CULTURE_LATIN 0
%define PAN_ANY 0
%define PAN_NO_FIT 1
%define PAN_FAMILY_TEXT_DISPLAY 2
%define PAN_FAMILY_SCRIPT 3
%define PAN_FAMILY_DECORATIVE 4
%define PAN_FAMILY_PICTORIAL 5
%define PAN_SERIF_COVE 2
%define PAN_SERIF_OBTUSE_COVE 3
%define PAN_SERIF_SQUARE_COVE 4
%define PAN_SERIF_OBTUSE_SQUARE_COVE 5
%define PAN_SERIF_SQUARE 6
%define PAN_SERIF_THIN 7
%define PAN_SERIF_BONE 8
%define PAN_SERIF_EXAGGERATED 9
%define PAN_SERIF_TRIANGLE 10
%define PAN_SERIF_NORMAL_SANS 11
%define PAN_SERIF_OBTUSE_SANS 12
%define PAN_SERIF_PERP_SANS 13
%define PAN_SERIF_FLARED 14
%define PAN_SERIF_ROUNDED 15
%define PAN_WEIGHT_VERY_LIGHT 2
%define PAN_WEIGHT_LIGHT 3
%define PAN_WEIGHT_THIN 4
%define PAN_WEIGHT_BOOK 5
%define PAN_WEIGHT_MEDIUM 6
%define PAN_WEIGHT_DEMI 7
%define PAN_WEIGHT_BOLD 8
%define PAN_WEIGHT_HEAVY 9
%define PAN_WEIGHT_BLACK 10
%define PAN_WEIGHT_NORD 11
%define PAN_PROP_OLD_STYLE 2
%define PAN_PROP_MODERN 3
%define PAN_PROP_EVEN_WIDTH 4
%define PAN_PROP_EXPANDED 5
%define PAN_PROP_CONDENSED 6
%define PAN_PROP_VERY_EXPANDED 7
%define PAN_PROP_VERY_CONDENSED 8
%define PAN_PROP_MONOSPACED 9
%define PAN_CONTRAST_NONE 2
%define PAN_CONTRAST_VERY_LOW 3
%define PAN_CONTRAST_LOW 4
%define PAN_CONTRAST_MEDIUM_LOW 5
%define PAN_CONTRAST_MEDIUM 6
%define PAN_CONTRAST_MEDIUM_HIGH 7
%define PAN_CONTRAST_HIGH 8
%define PAN_CONTRAST_VERY_HIGH 9
%define PAN_STROKE_GRADUAL_DIAG 2
%define PAN_STROKE_GRADUAL_TRAN 3
%define PAN_STROKE_GRADUAL_VERT 4
%define PAN_STROKE_GRADUAL_HORZ 5
%define PAN_STROKE_RAPID_VERT 6
%define PAN_STROKE_RAPID_HORZ 7
%define PAN_STROKE_INSTANT_VERT 8
%define PAN_STRAIGHT_ARMS_HORZ 2
%define PAN_STRAIGHT_ARMS_WEDGE 3
%define PAN_STRAIGHT_ARMS_VERT 4
%define PAN_STRAIGHT_ARMS_SINGLE_SERIF 5
%define PAN_STRAIGHT_ARMS_DOUBLE_SERIF 6
%define PAN_BENT_ARMS_HORZ 7
%define PAN_BENT_ARMS_WEDGE 8
%define PAN_BENT_ARMS_VERT 9
%define PAN_BENT_ARMS_SINGLE_SERIF 10
%define PAN_BENT_ARMS_DOUBLE_SERIF 11
%define PAN_LETT_NORMAL_CONTACT 2
%define PAN_LETT_NORMAL_WEIGHTED 3
%define PAN_LETT_NORMAL_BOXED 4
%define PAN_LETT_NORMAL_FLATTENED 5
%define PAN_LETT_NORMAL_ROUNDED 6
%define PAN_LETT_NORMAL_OFF_CENTER 7
%define PAN_LETT_NORMAL_SQUARE 8
%define PAN_LETT_OBLIQUE_CONTACT 9
%define PAN_LETT_OBLIQUE_WEIGHTED 10
%define PAN_LETT_OBLIQUE_BOXED 11
%define PAN_LETT_OBLIQUE_FLATTENED 12
%define PAN_LETT_OBLIQUE_ROUNDED 13
%define PAN_LETT_OBLIQUE_OFF_CENTER 14
%define PAN_LETT_OBLIQUE_SQUARE 15
%define PAN_MIDLINE_STANDARD_TRIMMED 2
%define PAN_MIDLINE_STANDARD_POINTED 3
%define PAN_MIDLINE_STANDARD_SERIFED 4
%define PAN_MIDLINE_HIGH_TRIMMED 5
%define PAN_MIDLINE_HIGH_POINTED 6
%define PAN_MIDLINE_HIGH_SERIFED 7
%define PAN_MIDLINE_CONSTANT_TRIMMED 8
%define PAN_MIDLINE_CONSTANT_POINTED 9
%define PAN_MIDLINE_CONSTANT_SERIFED 10
%define PAN_MIDLINE_LOW_TRIMMED 11
%define PAN_MIDLINE_LOW_POINTED 12
%define PAN_MIDLINE_LOW_SERIFED 13
%define PAN_XHEIGHT_CONSTANT_SMALL 2
%define PAN_XHEIGHT_CONSTANT_STD 3
%define PAN_XHEIGHT_CONSTANT_LARGE 4
%define PAN_XHEIGHT_DUCKING_SMALL 5
%define PAN_XHEIGHT_DUCKING_STD 6
%define PAN_XHEIGHT_DUCKING_LARGE 7
%define ELF_VENDOR_SIZE 4
%define ELF_VERSION 0
%define ELF_CULTURE_LATIN 0
%define RASTER_FONTTYPE 1h
%define DEVICE_FONTTYPE 2h
%define TRUETYPE_FONTTYPE 4h
%define PC_RESERVED 1h
%define PC_EXPLICIT 2h
%define PC_NOCOLLAPSE 4h
%define TRANSPARENT 1
%define OPAQUE 2
%define BKMODE_LAST 2
%define GM_COMPATIBLE 1
%define GM_ADVANCED 2
%define GM_LAST 2
%define PT_CLOSEFIGURE 1h
%define PT_LINETO 2h
%define PT_BEZIERTO 4h
%define PT_MOVETO 6h
%define MM_TEXT 1
%define MM_LOMETRIC 2
%define MM_HIMETRIC 3
%define MM_LOENGLISH 4
%define MM_HIENGLISH 5
%define MM_TWIPS 6
%define MM_ISOTROPIC 7
%define MM_ANISOTROPIC 8
%define MM_MIN MM_TEXT
%define MM_MAX MM_ANISOTROPIC
%define MM_MAX_FIXEDSCALE MM_TWIPS
%define _ABSOLUTE 1
%define RELATIVE 2
%define WHITE_BRUSH 0
%define LTGRAY_BRUSH 1
%define GRAY_BRUSH 2
%define DKGRAY_BRUSH 3
%define BLACK_BRUSH 4
%define NULL_BRUSH 5
%define HOLLOW_BRUSH NULL_BRUSH
%define WHITE_PEN 6
%define BLACK_PEN 7
%define NULL_PEN 8
%define OEM_FIXED_FONT 10
%define ANSI_FIXED_FONT 11
%define ANSI_VAR_FONT 12
%define SYSTEM_FONT 13
%define DEVICE_DEFAULT_FONT 14
%define DEFAULT_PALETTE 15
%define SYSTEM_FIXED_FONT 16
%define STOCK_LAST 16
%define CLR_INVALID 0FFFFh
%define BS_SOLID 0
%define BS_NULL 1
%define BS_HOLLOW BS_NULL
%define BS_HATCHED 2
%define BS_PATTERN 3
%define BS_INDEXED 4
%define BS_DIBPATTERN 5
%define BS_DIBPATTERNPT 6
%define BS_PATTERN8X8 7
%define BS_DIBPATTERN8X8 8
%define HS_HORIZONTAL 0
%define HS_VERTICAL 1
%define HS_FDIAGONAL 2
%define HS_BDIAGONAL 3
%define HS_CROSS 4
%define HS_DIAGCROSS 5
%define HS_FDIAGONAL1 6
%define HS_BDIAGONAL1 7
%define HS_SOLID 8
%define HS_DENSE1 9
%define HS_DENSE2 10
%define HS_DENSE3 11
%define HS_DENSE4 12
%define HS_DENSE5 13
%define HS_DENSE6 14
%define HS_DENSE7 15
%define HS_DENSE8 16
%define HS_NOSHADE 17
%define HS_HALFTONE 18
%define HS_SOLIDCLR 19
%define HS_DITHEREDCLR 20
%define HS_SOLIDTEXTCLR 21
%define HS_DITHEREDTEXTCLR 22
%define HS_SOLIDBKCLR 23
%define HS_DITHEREDBKCLR 24
%define HS_API_MAX 25
%define PS_SOLID 0
%define PS_DASH 1
%define PS_DOT 2
%define PS_DASHDOT 3
%define PS_DASHDOTDOT 4
%define PS_NULL 5
%define PS_INSIDEFRAME 6
%define PS_USERSTYLE 7
%define PS_ALTERNATE 8
%define PS_STYLE_MASK 0Fh
%define PS_ENDCAP_ROUND 0h
%define PS_ENDCAP_SQUARE 100h
%define PS_ENDCAP_FLAT 200h
%define PS_ENDCAP_MASK 0F00h
%define PS_JOIN_ROUND 0h
%define PS_JOIN_BEVEL 1000h
%define PS_JOIN_MITER 2000h
%define PS_JOIN_MASK 0F000h
%define PS_COSMETIC 0h
%define PS_GEOMETRIC 10000h
%define PS_TYPE_MASK 0F0000h
%define AD_COUNTERCLOCKWISE 1
%define AD_CLOCKWISE 2
%define PRF_CHECKVISIBLE 00000001h
%define PRF_NONCLIENT 00000002h
%define PRF_CLIENT 00000004h
%define PRF_ERASEBKGND 00000008h
%define PRF_CHILDREN 00000010h
%define PRF_OWNED 00000020h
%define BDR_RAISEDOUTER 0001h
%define BDR_SUNKENOUTER 0002h
%define BDR_RAISEDINNER 0004h
%define BDR_SUNKENINNER 0008h
%define BDR_OUTER 0003h
%define BDR_INNER 000Ch
%define BDR_RAISED 0005h
%define BDR_SUNKEN 000Ah
%define EDGE_RAISED BDR_RAISEDOUTER|BDR_RAISEDINNER
%define EDGE_SUNKEN BDR_SUNKENOUTER|BDR_SUNKENINNER
%define EDGE_ETCHED BDR_SUNKENOUTER|BDR_RAISEDINNER
%define EDGE_BUMP BDR_RAISEDOUTER|BDR_SUNKENINNER
%define BF_LEFT 0001h
%define BF_TOP 0002h
%define BF_RIGHT 0004h
%define BF_BOTTOM 0008h
%define BF_TOPLEFT BF_TOP|BF_LEFT
%define BF_TOPRIGHT BF_TOP|BF_RIGHT
%define BF_BOTTOMLEFT BF_BOTTOM|BF_LEFT
%define BF_BOTTOMRIGHT BF_BOTTOM|BF_RIGHT
%define BF_RECT BF_LEFT|BF_TOP|BF_RIGHT|BF_BOTTOM
%define BF_DIAGONAL 0010h
%define BF_DIAGONAL_ENDTOPRIGHT BF_DIAGONAL|BF_TOP|BF_RIGHT
%define BF_DIAGONAL_ENDTOPLEFT BF_DIAGONAL|BF_TOP|BF_LEFT
%define BF_DIAGONAL_ENDBOTTOMLEFT BF_DIAGONAL|BF_BOTTOM|BF_LEFT
%define BF_DIAGONAL_ENDBOTTOMRIGHT BF_DIAGONAL|BF_BOTTOM|BF_RIGHT
%define BF_MIDDLE 0800h
%define BF_SOFT 1000h
%define BF_ADJUST 2000h
%define BF_FLAT 4000h
%define BF_MONO 8000h
%define DRIVERVERSION 0
%define TECHNOLOGY 2
%define HORZSIZE 4
%define VERTSIZE 6
%define HORZRES 8
%define VERTRES 10
%define BITSPIXEL 12
%define PLANES 14
%define NUMBRUSHES 16
%define NUMPENS 18
%define NUMMARKERS 20
%define NUMFONTS 22
%define NUMCOLORS 24
%define PDEVICESIZE 26
%define CURVECAPS 28
%define LINECAPS 30
%define POLYGONALCAPS 32
%define TEXTCAPS 34
%define CLIPCAPS 36
%define RASTERCAPS 38
%define ASPECTX 40
%define ASPECTY 42
%define ASPECTXY 44
%define LOGPIXELSX 88
%define LOGPIXELSY 90
%define SIZEPALETTE 104
%define NUMRESERVED 106
%define COLORRES 108
%define PHYSICALWIDTH 110
%define PHYSICALHEIGHT 111
%define PHYSICALOFFSETX 112
%define PHYSICALOFFSETY 113
%define SCALINGFACTORX 114
%define SCALINGFACTORY 115
%define DT_PLOTTER 0
%define DT_RASDISPLAY 1
%define DT_RASPRINTER 2
%define DT_RASCAMERA 3
%define DT_CHARSTREAM 4
%define DT_METAFILE 5
%define DT_DISPFILE 6
%define CC_NONE 0
%define CC_CIRCLES 1
%define CC_PIE 2
%define CC_CHORD 4
%define CC_ELLIPSES 8
%define CC_WIDE 16
%define CC_STYLED 32
%define CC_WIDESTYLED 64
%define CC_INTERIORS 128
%define CC_ROUNDRECT 256
%define LC_NONE 0
%define LC_POLYLINE 2
%define LC_MARKER 4
%define LC_POLYMARKER 8
%define LC_WIDE 16
%define LC_STYLED 32
%define LC_WIDESTYLED 64
%define LC_INTERIORS 128
%define PC_NONE 0
%define PC_POLYGON 1
%define PC_RECTANGLE 2
%define PC_WINDPOLYGON 4
%define PC_TRAPEZOID 4
%define PC_SCANLINE 8
%define PC_WIDE 16
%define PC_STYLED 32
%define PC_WIDESTYLED 64
%define PC_INTERIORS 128
%define CP_NONE 0
%define CP_RECTANGLE 1
%define CP_REGION 2
%define TC_OP_CHARACTER 1h
%define TC_OP_STROKE 2h
%define TC_CP_STROKE 4h
%define TC_CR_90 8h
%define TC_CR_ANY 10h
%define TC_SF_X_YINDEP 20h
%define TC_SA_DOUBLE 40h
%define TC_SA_INTEGER 80h
%define TC_SA_CONTIN 100h
%define TC_EA_DOUBLE 200h
%define TC_IA_ABLE 400h
%define TC_UA_ABLE 800h
%define TC_SO_ABLE 1000h
%define TC_RA_ABLE 2000h
%define TC_VA_ABLE 4000h
%define TC_RESERVED 8000h
%define TC_SCROLLBLT 10000h
%define RC_NONE 0
%define RC_BITBLT 1
%define RC_BANDING 2
%define RC_SCALING 4
%define RC_BITMAP64 8
%define RC_GDI20_OUTPUT 10h
%define RC_GDI20_STATE 20h
%define RC_SAVEBITMAP 40h
%define RC_DI_BITMAP 80h
%define RC_PALETTE 100h
%define RC_DIBTODEV 200h
%define RC_BIGFONT 400h
%define RC_STRETCHBLT 800h
%define RC_FLOODFILL 1000h
%define RC_STRETCHDIB 2000h
%define RC_OP_DX_OUTPUT 4000h
%define RC_DEVBITS 8000h
%define DIB_RGB_COLORS 0
%define DIB_PAL_COLORS 1
%define DIB_PAL_INDICES 2
%define DIB_PAL_PHYSINDICES 2
%define DIB_PAL_LOGINDICES 4
%define SYSPAL_ERROR 0
%define SYSPAL_STATIC 1
%define SYSPAL_NOSTATIC 2
%define CBM_CREATEDIB 2h
%define CBM_INIT 4h
%define FLOODFILLBORDER 0
%define FLOODFILLSURFACE 1
%define CCHDEVICENAME 32
%define CCHFORMNAME 32
%define DM_SPECVERSION 320h
%define DM_ORIENTATION 1h
%define DM_PAPERSIZE 2h
%define DM_PAPERLENGTH 4h
%define DM_PAPERWIDTH 8h
%define DM_SCALE 10h
%define DM_COPIES 100h
%define DM_DEFAULTSOURCE 200h
%define DM_PRINTQUALITY 400h
%define DM_COLOR 800h
%define DM_DUPLEX 1000h
%define DM_YRESOLUTION 2000h
%define DM_TTOPTION 4000h
%define DM_COLLATE 8000h
%define DM_FORMNAME 10000h
%define DMORIENT_PORTRAIT 1
%define DMORIENT_LANDSCAPE 2
%define DMPAPER_LETTER 1
%define DMPAPER_FIRST DMPAPER_LETTER
%define DMPAPER_LETTERSMALL 2
%define DMPAPER_TABLOID 3
%define DMPAPER_LEDGER 4
%define DMPAPER_LEGAL 5
%define DMPAPER_STATEMENT 6
%define DMPAPER_EXECUTIVE 7
%define DMPAPER_A3 8
%define DMPAPER_A4 9
%define DMPAPER_A4SMALL 10
%define DMPAPER_A5 11
%define DMPAPER_B4 12
%define DMPAPER_B5 13
%define DMPAPER_FOLIO 14
%define DMPAPER_QUARTO 15
%define DMPAPER_10X14 16
%define DMPAPER_11X17 17
%define DMPAPER_NOTE 18
%define DMPAPER_ENV_9 19
%define DMPAPER_ENV_10 20
%define DMPAPER_ENV_11 21
%define DMPAPER_ENV_12 22
%define DMPAPER_ENV_14 23
%define DMPAPER_CSHEET 24
%define DMPAPER_DSHEET 25
%define DMPAPER_ESHEET 26
%define DMPAPER_ENV_DL 27
%define DMPAPER_ENV_C5 28
%define DMPAPER_ENV_C3 29
%define DMPAPER_ENV_C4 30
%define DMPAPER_ENV_C6 31
%define DMPAPER_ENV_C65 32
%define DMPAPER_ENV_B4 33
%define DMPAPER_ENV_B5 34
%define DMPAPER_ENV_B6 35
%define DMPAPER_ENV_ITALY 36
%define DMPAPER_ENV_MONARCH 37
%define DMPAPER_ENV_PERSONAL 38
%define DMPAPER_FANFOLD_US 39
%define DMPAPER_FANFOLD_STD_GERMAN 40
%define DMPAPER_FANFOLD_LGL_GERMAN 41
%define DMPAPER_LAST DMPAPER_FANFOLD_LGL_GERMAN
%define DMPAPER_USER 256
%define DMBIN_UPPER 1
%define DMBIN_FIRST DMBIN_UPPER
%define DMBIN_ONLYONE 1
%define DMBIN_LOWER 2
%define DMBIN_MIDDLE 3
%define DMBIN_MANUAL 4
%define DMBIN_ENVELOPE 5
%define DMBIN_ENVMANUAL 6
%define DMBIN_AUTO 7
%define DMBIN_TRACTOR 8
%define DMBIN_SMALLFMT 9
%define DMBIN_LARGEFMT 10
%define DMBIN_LARGECAPACITY 11
%define DMBIN_CASSETTE 14
%define DMBIN_LAST DMBIN_CASSETTE
%define DMBIN_USER 256
%define DMRES_DRAFT -1
%define DMRES_LOW -2
%define DMRES_MEDIUM -3
%define DMRES_HIGH -4
%define DMCOLOR_MONOCHROME 1
%define DMCOLOR_COLOR 2
%define DMDUP_SIMPLEX 1
%define DMDUP_VERTICAL 2
%define DMDUP_HORIZONTAL 3
%define DMTT_BITMAP 1
%define DMTT_DOWNLOAD 2
%define DMTT_SUBDEV 3
%define DMCOLLATE_FALSE 0
%define DMCOLLATE_TRUE 1
%define DM_GRAYSCALE 1h
%define DM_INTERLACED 2h
%define RDH_RECTANGLES 1
%define GGO_METRICS 0
%define GGO_BITMAP 1
%define GGO_NATIVE 2
%define TT_POLYGON_TYPE 24
%define TT_PRIM_LINE 1
%define TT_PRIM_QSPLINE 2
%define TT_AVAILABLE 1h
%define TT_ENABLED 2h
%define DM_UPDATE 1
%define DM_COPY 2
%define DM_PROMPT 4
%define DM_MODIFY 8
%define DM_IN_BUFFER DM_MODIFY
%define DM_IN_PROMPT DM_PROMPT
%define DM_OUT_BUFFER DM_COPY
%define DM_OUT_DEFAULT DM_UPDATE
%define DC_FIELDS 1
%define DC_PAPERS 2
%define DC_PAPERSIZE 3
%define DC_MINEXTENT 4
%define DC_MAXEXTENT 5
%define DC_BINS 6
%define DC_DUPLEX 7
%define DC_SIZE 8
%define DC_EXTRA 9
%define DC_VERSION 10
%define DC_DRIVER 11
%define DC_BINNAMES 12
%define DC_ENUMRESOLUTIONS 13
%define DC_FILEDEPENDENCIES 14
%define DC_TRUETYPE 15
%define DC_PAPERNAMES 16
%define DC_ORIENTATION 17
%define DC_COPIES 18
%define DCTT_BITMAP 1h
%define DCTT_DOWNLOAD 2h
%define DCTT_SUBDEV 4h
%define CA_NEGATIVE 1h
%define CA_LOG_FILTER 2h
%define ILLUMINANT_DEVICE_DEFAULT 0
%define ILLUMINANT_A 1
%define ILLUMINANT_B 2
%define ILLUMINANT_C 3
%define ILLUMINANT_D50 4
%define ILLUMINANT_D55 5
%define ILLUMINANT_D65 6
%define ILLUMINANT_D75 7
%define ILLUMINANT_F2 8
%define ILLUMINANT_MAX_INDEX ILLUMINANT_F2
%define ILLUMINANT_TUNGSTEN ILLUMINANT_A
%define ILLUMINANT_DAYLIGHT ILLUMINANT_C
%define ILLUMINANT_FLUORESCENT ILLUMINANT_F2
%define ILLUMINANT_NTSC ILLUMINANT_C
%define RGB_GAMMA_MIN 2500
%define RGB_GAMMA_MAX 65000
%define REFERENCE_WHITE_MIN 6000
%define REFERENCE_WHITE_MAX 10000
%define REFERENCE_BLACK_MIN 0
%define REFERENCE_BLACK_MAX 4000
%define COLOR_ADJ_MIN -100
%define COLOR_ADJ_MAX 100
%define FONTMAPPER_MAX 10
%define ENHMETA_SIGNATURE 464D4520h
%define ENHMETA_STOCK_OBJECT 80000000h
%define EMR_HEADER 1
%define EMR_POLYBEZIER 2
%define EMR_POLYGON 3
%define EMR_POLYLINE 4
%define EMR_POLYBEZIERTO 5
%define EMR_POLYLINETO 6
%define EMR_POLYPOLYLINE 7
%define EMR_POLYPOLYGON 8
%define EMR_SETWINDOWEXTEX 9
%define EMR_SETWINDOWORGEX 10
%define EMR_SETVIEWPORTEXTEX 11
%define EMR_SETVIEWPORTORGEX 12
%define EMR_SETBRUSHORGEX 13
%define EMR_EOF 14
%define EMR_SETPIXELV 15
%define EMR_SETMAPPERFLAGS 16
%define EMR_SETMAPMODE 17
%define EMR_SETBKMODE 18
%define EMR_SETPOLYFILLMODE 19
%define EMR_SETROP2 20
%define EMR_SETSTRETCHBLTMODE 21
%define EMR_SETTEXTALIGN 22
%define EMR_SETCOLORADJUSTMENT 23
%define EMR_SETTEXTCOLOR 24
%define EMR_SETBKCOLOR 25
%define EMR_OFFSETCLIPRGN 26
%define EMR_MOVETOEX 27
%define EMR_SETMETARGN 28
%define EMR_EXCLUDECLIPRECT 29
%define EMR_INTERSECTCLIPRECT 30
%define EMR_SCALEVIEWPORTEXTEX 31
%define EMR_SCALEWINDOWEXTEX 32
%define EMR_SAVEDC 33
%define EMR_RESTOREDC 34
%define EMR_SETWORLDTRANSFORM 35
%define EMR_MODIFYWORLDTRANSFORM 36
%define EMR_SELECTOBJECT 37
%define EMR_CREATEPEN 38
%define EMR_CREATEBRUSHINDIRECT 39
%define EMR_DELETEOBJECT 40
%define EMR_ANGLEARC 41
%define EMR_ELLIPSE 42
%define EMR_RECTANGLE 43
%define EMR_ROUNDRECT 44
%define EMR_ARC 45
%define EMR_CHORD 46
%define EMR_PIE 47
%define EMR_SELECTPALETTE 48
%define EMR_CREATEPALETTE 49
%define EMR_SETPALETTEENTRIES 50
%define EMR_RESIZEPALETTE 51
%define EMR_REALIZEPALETTE 52
%define EMR_EXTFLOODFILL 53
%define EMR_LINETO 54
%define EMR_ARCTO 55
%define EMR_POLYDRAW 56
%define EMR_SETARCDIRECTION 57
%define EMR_SETMITERLIMIT 58
%define EMR_BEGINPATH 59
%define EMR_ENDPATH 60
%define EMR_CLOSEFIGURE 61
%define EMR_FILLPATH 62
%define EMR_STROKEANDFILLPATH 63
%define EMR_STROKEPATH 64
%define EMR_FLATTENPATH 65
%define EMR_WIDENPATH 66
%define EMR_SELECTCLIPPATH 67
%define EMR_ABORTPATH 68
%define EMR_GDICOMMENT 70
%define EMR_FILLRGN 71
%define EMR_FRAMERGN 72
%define EMR_INVERTRGN 73
%define EMR_PAINTRGN 74
%define EMR_EXTSELECTCLIPRGN 75
%define EMR_BITBLT 76
%define EMR_STRETCHBLT 77
%define EMR_MASKBLT 78
%define EMR_PLGBLT 79
%define EMR_SETDIBITSTODEVICE 80
%define EMR_STRETCHDIBITS 81
%define EMR_EXTCREATEFONTINDIRECTW 82
%define EMR_EXTTEXTOUTA 83
%define EMR_EXTTEXTOUTW 84
%define EMR_POLYBEZIER16 85
%define EMR_POLYGON16 86
%define EMR_POLYLINE16 87
%define EMR_POLYBEZIERTO16 88
%define EMR_POLYLINETO16 89
%define EMR_POLYPOLYLINE16 90
%define EMR_POLYPOLYGON16 91
%define EMR_POLYDRAW16 92
%define EMR_CREATEMONOBRUSH 93
%define EMR_CREATEDIBPATTERNBRUSHPT 94
%define EMR_EXTCREATEPEN 95
%define EMR_POLYTEXTOUTA 96
%define EMR_POLYTEXTOUTW 97
%define EMR_MIN 1
%define EMR_MAX 97
%define STRETCH_ANDSCANS 1
%define STRETCH_ORSCANS 2
%define STRETCH_DELETESCANS 3
%define STRETCH_HALFTONE 4
%define TCI_SRCCHARSET 1
%define TCI_SRCCODEPAGE 2
%define TCI_SRCFONTSIG 3
%define MONO_FONT 8
%define JOHAB_CHARSET 130
%define HEBREW_CHARSET 177
%define ARABIC_CHARSET 178
%define GREEK_CHARSET 161
%define TURKISH_CHARSET 162
%define THAI_CHARSET 222
%define EASTEUROPE_CHARSET 238
%define RUSSIAN_CHARSET 204
%define MAC_CHARSET 77
%define BALTIC_CHARSET 186
%define FS_LATIN1 1h
%define FS_LATIN2 2h
%define FS_CYRILLIC 4h
%define FS_GREEK 8h
%define FS_TURKISH 10h
%define FS_HEBREW 20h
%define FS_ARABIC 40h
%define FS_BALTIC 80h
%define FS_THAI 10000h
%define FS_JISJAPAN 20000h
%define FS_CHINESESIMP 40000h
%define FS_WANSUNG 80000h
%define FS_CHINESETRAD 100000h
%define FS_JOHAB 200000h
%define FS_SYMBOL 80000000h
%define DEFAULT_GUI_FONT 17
%define DM_RESERVED1 800000h
%define DM_RESERVED2 1000000h
%define DM_ICMMETHOD 2000000h
%define DM_ICMINTENT 4000000h
%define DM_MEDIATYPE 8000000h
%define DM_DITHERTYPE 10000000h
%define DMPAPER_ISO_B4 42
%define DMPAPER_JAPANESE_POSTCARD 43
%define DMPAPER_9X11 44
%define DMPAPER_10X11 45
%define DMPAPER_15X11 46
%define DMPAPER_ENV_INVITE 47
%define DMPAPER_RESERVED_48 48
%define DMPAPER_RESERVED_49 49
%define DMPAPER_LETTER_EXTRA 50
%define DMPAPER_LEGAL_EXTRA 51
%define DMPAPER_TABLOID_EXTRA 52
%define DMPAPER_A4_EXTRA 53
%define DMPAPER_LETTER_TRANSVERSE 54
%define DMPAPER_A4_TRANSVERSE 55
%define DMPAPER_LETTER_EXTRA_TRANSVERSE 56
%define DMPAPER_A_PLUS 57
%define DMPAPER_B_PLUS 58
%define DMPAPER_LETTER_PLUS 59
%define DMPAPER_A4_PLUS 60
%define DMPAPER_A5_TRANSVERSE 61
%define DMPAPER_B5_TRANSVERSE 62
%define DMPAPER_A3_EXTRA 63
%define DMPAPER_A5_EXTRA 64
%define DMPAPER_B5_EXTRA 65
%define DMPAPER_A2 66
%define DMPAPER_A3_TRANSVERSE 67
%define DMPAPER_A3_EXTRA_TRANSVERSE 68
%define DMTT_DOWNLOAD_OUTLINE 4
%define DMICMMETHOD_NONE 1
%define DMICMMETHOD_SYSTEM 2
%define DMICMMETHOD_DRIVER 3
%define DMICMMETHOD_DEVICE 4
%define DMICMMETHOD_USER 256
%define DMICM_SATURATE 1
%define DMICM_CONTRAST 2
%define DMICM_COLORMETRIC 3
%define DMICM_USER 256
%define DMMEDIA_STANDARD 1
%define DMMEDIA_GLOSSY 2
%define DMMEDIA_TRANSPARENCY 3
%define DMMEDIA_USER 256
%define DMDITHER_NONE 1
%define DMDITHER_COARSE 2
%define DMDITHER_FINE 3
%define DMDITHER_LINEART 4
%define DMDITHER_GRAYSCALE 5
%define DMDITHER_USER 256
%define GGO_GRAY2_BITMAP 4
%define GGO_GRAY4_BITMAP 5
%define GGO_GRAY8_BITMAP 6
%define GGO_GLYPH_INDEX 80h
%define GCP_DBCS 1h
%define GCP_REORDER 2h
%define GCP_USEKERNING 8h
%define GCP_GLYPHSHAPE 10h
%define GCP_LIGATE 20h
%define GCP_DIACRITIC 100h
%define GCP_KASHIDA 400h
%define GCP_ERROR 8000h
%define FLI_MASK 103Bh
%define GCP_JUSTIFY 10000h
%define GCP_NODIACRITICS 20000h
%define FLI_GLYPHS 40000h
%define GCP_CLASSIN 80000h
%define GCP_MAXEXTENT 100000h
%define GCP_JUSTIFYIN 200000h
%define GCP_DISPLAYZWG 400000h
%define GCP_SYMSWAPOFF 800000h
%define GCP_NUMERICOVERRIDE 1000000h
%define GCP_NEUTRALOVERRIDE 2000000h
%define GCP_NUMERICSLATIN 4000000h
%define GCP_NUMERICSLOCAL 8000000h
%define GCPCLASS_LATIN 1
%define GCPCLASS_HEBREW 2
%define GCPCLASS_ARABIC 2
%define GCPCLASS_NEUTRAL 3
%define GCPCLASS_LOCALNUMBER 4
%define GCPCLASS_LATINNUMBER 5
%define GCPCLASS_LATINNUMERICTERMINATOR 6
%define GCPCLASS_LATINNUMERICSEPARATOR 7
%define GCPCLASS_NUMERICSEPARATOR 8
%define GCPCLASS_PREBOUNDRTL 80h
%define GCPCLASS_PREBOUNDLTR 40h
%define DC_BINADJUST 19
%define DC_EMF_COMPLIANT 20
%define DC_DATATYPE_PRODUCED 21
%define DC_COLLATE 22
%define DCTT_DOWNLOAD_OUTLINE 8h
%define DCBA_FACEUPNONE 0h
%define DCBA_FACEUPCENTER 1h
%define DCBA_FACEUPLEFT 2h
%define DCBA_FACEUPRIGHT 3h
%define DCBA_FACEDOWNNONE 100h
%define DCBA_FACEDOWNCENTER 101h
%define DCBA_FACEDOWNLEFT 102h
%define DCBA_FACEDOWNRIGHT 103h
%define ICM_OFF 1
%define ICM_ON 2
%define ICM_QUERY 3
%define EMR_SETICMMODE 98
%define EMR_CREATECOLORSPACE 99
%define EMR_SETCOLORSPACE 100
%define EMR_DELETECOLORSPACE 101
%define SB_HORZ 0
%define SB_VERT 1
%define SB_CTL 2
%define SB_BOTH 3
%define SB_LINEUP 0
%define SB_LINELEFT 0
%define SB_LINEDOWN 1
%define SB_LINERIGHT 1
%define SB_PAGEUP 2
%define SB_PAGELEFT 2
%define SB_PAGEDOWN 3
%define SB_PAGERIGHT 3
%define SB_THUMBPOSITION 4
%define SB_THUMBTRACK 5
%define SB_TOP 6
%define SB_LEFT 6
%define SB_BOTTOM 7
%define SB_RIGHT 7
%define SB_ENDSCROLL 8
%define SBM_SETSCROLLINFO 00E9h
%define SBM_GETSCROLLINFO 00EAh
%define SIF_RANGE 0001h
%define SIF_PAGE 0002h
%define SIF_POS 0004h
%define SIF_DISABLENOSCROLL 0008h
%define SIF_TRACKPOS 0010h
%define SIF_ALL SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS
%define SW_HIDE 0
%define SW_SHOWNORMAL 1
%define SW_NORMAL 1
%define SW_SHOWMINIMIZED 2
%define SW_SHOWMAXIMIZED 3
%define SW_MAXIMIZE 3
%define SW_SHOWNOACTIVATE 4
%define SW_SHOW 5
%define SW_MINIMIZE 6
%define SW_SHOWMINNOACTIVE 7
%define SW_SHOWNA 8
%define SW_RESTORE 9
%define SW_SHOWDEFAULT 10
%define SW_MAX 10
%define HIDE_WINDOW 0
%define SHOW_OPENWINDOW 1
%define SHOW_ICONWINDOW 2
%define SHOW_FULLSCREEN 3
%define SHOW_OPENNOACTIVATE 4
%define SW_PARENTCLOSING 1
%define SW_OTHERZOOM 2
%define SW_PARENTOPENING 3
%define SW_OTHERUNZOOM 4
%define KF_EXTENDED 100h
%define KF_DLGMODE 800h
%define KF_MENUMODE 1000h
%define KF_ALTDOWN 2000h
%define KF_REPEAT 4000h
%define KF_UP 8000h
%define VK_BACK 8h
%define VK_CANCEL 3h
%define VK_CAPITAL 14h
%define VK_CLEAR 0Ch
%define VK_CONTROL 11h
%define VK_DELETE 2Eh
%define VK_DOWN 28h
%define VK_END 23h
%define VK_ESCAPE 1Bh
%define VK_EXECUTE 2Bh
%define VK_HELP 2Fh
%define VK_HOME 24h
%define VK_INSERT 2Dh
%define VK_LBUTTON 1h
%define VK_LEFT 25h
%define VK_MBUTTON 4h
%define VK_MENU 12h
%define VK_NEXT 22h
%define VK_PAUSE 13h
%define VK_PGDN 22h
%define VK_PGUP 21h
%define VK_PRINT 2Ah
%define VK_PRIOR 21h
%define VK_RBUTTON 2h
%define VK_RETURN 0Dh
%define VK_RIGHT 27h
%define VK_SELECT 29h
%define VK_SHIFT 10h
%define VK_SNAPSHOT 2Ch
%define VK_SPACE 20h
%define VK_TAB 9h
%define VK_UP 26h
%define VK_NUMPAD0 60h
%define VK_NUMPAD1 61h
%define VK_NUMPAD2 62h
%define VK_NUMPAD3 63h
%define VK_NUMPAD4 64h
%define VK_NUMPAD5 65h
%define VK_NUMPAD6 66h
%define VK_NUMPAD7 67h
%define VK_NUMPAD8 68h
%define VK_NUMPAD9 69h
%define VK_MULTIPLY 6Ah
%define VK_ADD 6Bh
%define VK_SEPARATOR 6Ch
%define VK_SUBTRACT 6Dh
%define VK_DECIMAL 6Eh
%define VK_DIVIDE 6Fh
%define VK_F1 70h
%define VK_F2 71h
%define VK_F3 72h
%define VK_F4 73h
%define VK_F5 74h
%define VK_F6 75h
%define VK_F7 76h
%define VK_F8 77h
%define VK_F9 78h
%define VK_F10 79h
%define VK_F11 7Ah
%define VK_F12 7Bh
%define VK_F13 7Ch
%define VK_F14 7Dh
%define VK_F15 7Eh
%define VK_F16 7Fh
%define VK_F17 80h
%define VK_F18 81h
%define VK_F19 82h
%define VK_F20 83h
%define VK_F21 84h
%define VK_F22 85h
%define VK_F23 86h
%define VK_F24 87h
%define VK_NUMLOCK 90h
%define VK_SCROLL 91h
%define VK_LSHIFT 0A0h
%define VK_RSHIFT 0A1h
%define VK_LCONTROL 0A2h
%define VK_RCONTROL 0A3h
%define VK_LMENU 0A4h
%define VK_RMENU 0A5h
%define VK_ATTN 0F6h
%define VK_CRSEL 0F7h
%define VK_EXSEL 0F8h
%define VK_EREOF 0F9h
%define VK_PLAY 0FAh
%define VK_ZOOM 0FBh
%define VK_NONAME 0FCh
%define VK_PA1 0FDh
%define VK_OEM_CLEAR 0FEh
%define WH_MIN -1
%define WH_MSGFILTER -1
%define WH_JOURNALRECORD 0
%define WH_JOURNALPLAYBACK 1
%define WH_KEYBOARD 2
%define WH_GETMESSAGE 3
%define WH_CALLWNDPROC 4
%define WH_CBT 5
%define WH_SYSMSGFILTER 6
%define WH_MOUSE 7
%define WH_HARDWARE 8
%define WH_DEBUG 9
%define WH_SHELL 10
%define WH_FOREGROUNDIDLE 11
%define WH_MAX 11
%define HC_ACTION 0
%define HC_GETNEXT 1
%define HC_SKIP 2
%define HC_NOREMOVE 3
%define HC_NOREM HC_NOREMOVE
%define HC_SYSMODALON 4
%define HC_SYSMODALOFF 5
%define HCBT_MOVESIZE 0
%define HCBT_MINMAX 1
%define HCBT_QS 2
%define HCBT_CREATEWND 3
%define HCBT_DESTROYWND 4
%define HCBT_ACTIVATE 5
%define HCBT_CLICKSKIPPED 6
%define HCBT_KEYSKIPPED 7
%define HCBT_SYSCOMMAND 8
%define HCBT_SETFOCUS 9
%define HSHELL_WINDOWCREATED 1
%define HSHELL_WINDOWDESTROYED 2
%define HSHELL_ACTIVATESHELLWINDOW 3
%define HKL_PREV 0
%define HKL_NEXT 1
%define KLF_ACTIVATE 1h
%define KLF_SUBSTITUTE_OK 2h
%define KLF_UNLOADPREVIOUS 4h
%define KLF_REORDER 8h
%define KL_NAMELENGTH 9
%define DESKTOP_READOBJECTS 1h
%define DESKTOP_CREATEWINDOW 2h
%define DESKTOP_CREATEMENU 4h
%define DESKTOP_HOOKCONTROL 8h
%define DESKTOP_JOURNALRECORD 10h
%define DESKTOP_JOURNALPLAYBACK 20h
%define DESKTOP_ENUMERATE 40h
%define DESKTOP_WRITEOBJECTS 80h
%define WINSTA_ENUMDESKTOPS 1h
%define WINSTA_READATTRIBUTES 2h
%define WINSTA_ACCESSCLIPBOARD 4h
%define WINSTA_CREATEDESKTOP 8h
%define WINSTA_WRITEATTRIBUTES 10h
%define WINSTA_ACCESSPUBLICATOMS 20h
%define WINSTA_EXITWINDOWS 40h
%define WINSTA_ENUMERATE 100h
%define WINSTA_READSCREEN 200h
%define GWL_WNDPROC -4
%define GWL_HINSTANCE -6
%define GWL_HWNDPARENT -8
%define GWL_STYLE -16
%define GWL_EXSTYLE -20
%define GWL_USERDATA -21
%define GWL_ID -12
%define GCL_MENUNAME -8
%define GCL_HBRBACKGROUND -10
%define GCL_HCURSOR -12
%define GCL_HICON -14
%define GCL_HMODULE -16
%define GCL_CBWNDEXTRA -18
%define GCL_CBCLSEXTRA -20
%define GCL_WNDPROC -24
%define GCL_STYLE -26
%define GCW_ATOM -32
%define WM_USER 400h
%define WM_NULL 0h
%define WM_CREATE 1h
%define WM_DESTROY 2h
%define WM_MOVE 3h
%define WM_SIZE 5h
%define WM_ACTIVATE 6h
%define WA_INACTIVE 0
%define WA_ACTIVE 1
%define WA_CLICKACTIVE 2
%define WM_SETFOCUS 7h
%define WM_KILLFOCUS 08h
%define WM_ENABLE 0Ah
%define WM_SETREDRAW 0Bh
%define WM_SETTEXT 0Ch
%define WM_GETTEXT 0Dh
%define WM_GETTEXTLENGTH 0Eh
%define WM_PAINT 0Fh
%define WM_CLOSE 10h
%define WM_QUERYENDSESSION 11h
%define WM_QUIT 12h
%define WM_QUERYOPEN 13h
%define WM_ERASEBKGND 14h
%define WM_SYSCOLORCHANGE 15h
%define WM_ENDSESSION 16h
%define WM_SHOWWINDOW 18h
%define WM_WININICHANGE 1Ah
%define WM_DEVMODECHANGE 1Bh
%define WM_ACTIVATEAPP 1Ch
%define WM_FONTCHANGE 1Dh
%define WM_TIMECHANGE 1Eh
%define WM_CANCELMODE 1Fh
%define WM_SETCURSOR 20h
%define WM_MOUSEACTIVATE 21h
%define WM_CHILDACTIVATE 22h
%define WM_QUEUESYNC 23h
%define WM_GETMINMAXINFO 24h
%define WM_PAINTICON 26h
%define WM_ICONERASEBKGND 27h
%define WM_NEXTDLGCTL 28h
%define WM_SPOOLERSTATUS 2Ah
%define WM_DRAWITEM 2Bh
%define WM_MEASUREITEM 2Ch
%define WM_DELETEITEM 2Dh
%define WM_VKEYTOITEM 2Eh
%define WM_CHARTOITEM 2Fh
%define WM_SETFONT 30h
%define WM_GETFONT 31h
%define WM_SETHOTKEY 32h
%define WM_GETHOTKEY 33h
%define WM_QUERYDRAGICON 37h
%define WM_COMPAREITEM 39h
%define WM_COMPACTING 41h
%define WM_OTHERWINDOWCREATED 42h
%define WM_OTHERWINDOWDESTROYED 43h
%define WM_COMMNOTIFY 44h
%define CN_RECEIVE 1h
%define CN_TRANSMIT 2h
%define CN_EVENT 4h
%define WM_WINDOWPOSCHANGING 46h
%define WM_WINDOWPOSCHANGED 47h
%define WM_POWER 48h
%define PWR_OK 1
%define PWR_FAIL -1
%define PWR_SUSPENDREQUEST 1
%define PWR_SUSPENDRESUME 2
%define PWR_CRITICALRESUME 3
%define WM_COPYDATA 4Ah
%define WM_CANCELJOURNAL 4Bh
%define WM_NOTIFY 4Eh
%define WM_INPUTLANGUAGECHANGEREQUEST 50h
%define WM_INPUTLANGUAGECHANGE 51h
%define WM_TCARD 52h
%define WM_HELP 53h
%define WM_USERCHANGED 54h
%define WM_NOTIFYFORMAT 55h
%define WM_CONTEXTMENU 7Bh
%define WM_STYLECHANGING 7Ch
%define WM_STYLECHANGED 7Dh
%define WM_DISPLAYCHANGE 7Eh
%define WM_GETICON 7Fh
%define WM_SETICON 80h
%define WM_NCCREATE 81h
%define WM_NCDESTROY 82h
%define WM_NCCALCSIZE 83h
%define WM_NCHITTEST 84h
%define WM_NCPAINT 85h
%define WM_NCACTIVATE 86h
%define WM_GETDLGCODE 87h
%define WM_NCMOUSEMOVE 0A0h
%define WM_NCLBUTTONDOWN 0A1h
%define WM_NCLBUTTONUP 0A2h
%define WM_NCLBUTTONDBLCLK 0A3h
%define WM_NCRBUTTONDOWN 0A4h
%define WM_NCRBUTTONUP 0A5h
%define WM_NCRBUTTONDBLCLK 0A6h
%define WM_NCMBUTTONDOWN 0A7h
%define WM_NCMBUTTONUP 0A8h
%define WM_NCMBUTTONDBLCLK 0A9h
%define WM_KEYFIRST 100h
%define WM_KEYDOWN 100h
%define WM_KEYUP 101h
%define WM_CHAR 102h
%define WM_DEADCHAR 103h
%define WM_SYSKEYDOWN 104h
%define WM_SYSKEYUP 105h
%define WM_SYSCHAR 106h
%define WM_SYSDEADCHAR 107h
%define WM_KEYLAST 108h
%define WM_INITDIALOG 110h
%define WM_COMMAND 111h
%define WM_SYSCOMMAND 112h
%define WM_TIMER 113h
%define WM_HSCROLL 114h
%define WM_VSCROLL 115h
%define WM_INITMENU 116h
%define WM_INITMENUPOPUP 117h
%define WM_MENUSELECT 11Fh
%define WM_MENUCHAR 120h
%define WM_ENTERIDLE 121h
%define WM_CTLCOLORMSGBOX 132h
%define WM_CTLCOLOREDIT 133h
%define WM_CTLCOLORLISTBOX 134h
%define WM_CTLCOLORBTN 135h
%define WM_CTLCOLORDLG 136h
%define WM_CTLCOLORSCROLLBAR 137h
%define WM_CTLCOLORSTATIC 138h
%define WM_MOUSEFIRST 200h
%define WM_MOUSEMOVE 200h
%define WM_LBUTTONDOWN 201h
%define WM_LBUTTONUP 202h
%define WM_LBUTTONDBLCLK 203h
%define WM_RBUTTONDOWN 204h
%define WM_RBUTTONUP 205h
%define WM_RBUTTONDBLCLK 206h
%define WM_MBUTTONDOWN 207h
%define WM_MBUTTONUP 208h
%define WM_MBUTTONDBLCLK 209h
%define WM_MOUSELAST 209h
%define WM_PARENTNOTIFY 210h
%define WM_ENTERMENULOOP 211h
%define WM_EXITMENULOOP 212h
%define WM_MDICREATE 220h
%define WM_MDIDESTROY 221h
%define WM_MDIACTIVATE 222h
%define WM_MDIRESTORE 223h
%define WM_MDINEXT 224h
%define WM_MDIMAXIMIZE 225h
%define WM_MDITILE 226h
%define WM_MDICASCADE 227h
%define WM_MDIICONARRANGE 228h
%define WM_MDIGETACTIVE 229h
%define WM_MDISETMENU 230h
%define WM_DROPFILES 233h
%define WM_MDIREFRESHMENU 234h
%define WM_CUT 300h
%define WM_COPY 301h
%define WM_PASTE 302h
%define WM_CLEAR 303h
%define WM_UNDO 304h
%define WM_RENDERFORMAT 305h
%define WM_RENDERALLFORMATS 306h
%define WM_DESTROYCLIPBOARD 307h
%define WM_DRAWCLIPBOARD 308h
%define WM_PAINTCLIPBOARD 309h
%define WM_VSCROLLCLIPBOARD 30Ah
%define WM_SIZECLIPBOARD 30Bh
%define WM_ASKCBFORMATNAME 30Ch
%define WM_CHANGECBCHAIN 30Dh
%define WM_HSCROLLCLIPBOARD 30Eh
%define WM_QUERYNEWPALETTE 30Fh
%define WM_PALETTEISCHANGING 310h
%define WM_PALETTECHANGED 311h
%define WM_HOTKEY 312h
%define WM_PRINTCLIENT 318h
%define WM_PENWINFIRST 380h
%define WM_PENWINLAST 38Fh
%define ST_BEGINSWP 0
%define ST_ENDSWP 1
%define HTERROR -2
%define HTTRANSPARENT -1
%define HTNOWHERE 0
%define HTCLIENT 1
%define HTCAPTION 2
%define HTSYSMENU 3
%define HTGROWBOX 4
%define HTSIZE HTGROWBOX
%define HTMENU 5
%define HTHSCROLL 6
%define HTVSCROLL 7
%define HTMINBUTTON 8
%define HTMAXBUTTON 9
%define HTLEFT 10
%define HTRIGHT 11
%define HTTOP 12
%define HTTOPLEFT 13
%define HTTOPRIGHT 14
%define HTBOTTOM 15
%define HTBOTTOMLEFT 16
%define HTBOTTOMRIGHT 17
%define HTBORDER 18
%define HTREDUCE HTMINBUTTON
%define HTZOOM HTMAXBUTTON
%define HTSIZEFIRST HTLEFT
%define HTSIZELAST HTBOTTOMRIGHT
%define SMTO_NORMAL 0h
%define SMTO_BLOCK 1h
%define SMTO_ABORTIFHUNG 2h
%define MA_ACTIVATE 1
%define MA_ACTIVATEANDEAT 2
%define MA_NOACTIVATE 3
%define MA_NOACTIVATEANDEAT 4
%define SIZE_RESTORED 0
%define SIZE_MINIMIZED 1
%define SIZE_MAXIMIZED 2
%define SIZE_MAXSHOW 3
%define SIZE_MAXHIDE 4
%define SIZENORMAL SIZE_RESTORED
%define SIZEICONIC SIZE_MINIMIZED
%define SIZEFULLSCREEN SIZE_MAXIMIZED
%define SIZEZOOMSHOW SIZE_MAXSHOW
%define SIZEZOOMHIDE SIZE_MAXHIDE
%define WVR_ALIGNTOP 10h
%define WVR_ALIGNLEFT 20h
%define WVR_ALIGNBOTTOM 40h
%define WVR_ALIGNRIGHT 80h
%define WVR_HREDRAW 100h
%define WVR_VREDRAW 200h
%define WVR_REDRAW WVR_HREDRAW|WVR_VREDRAW
%define WVR_VALIDRECTS 400h
%define MK_LBUTTON 1h
%define MK_RBUTTON 2h
%define MK_SHIFT 4h
%define MK_CONTROL 8h
%define MK_MBUTTON 10h
%define WS_OVERLAPPED 0h
%define WS_POPUP 80000000h
%define WS_CHILD 40000000h
%define WS_MINIMIZE 20000000h
%define WS_VISIBLE 10000000h
%define WS_DISABLED 8000000h
%define WS_CLIPSIBLINGS 4000000h
%define WS_CLIPCHILDREN 2000000h
%define WS_MAXIMIZE 1000000h
%define WS_CAPTION 0C00000h
%define WS_BORDER 800000h
%define WS_DLGFRAME 400000h
%define WS_VSCROLL 200000h
%define WS_HSCROLL 100000h
%define WS_SYSMENU 80000h
%define WS_THICKFRAME 40000h
%define WS_GROUP 20000h
%define WS_TABSTOP 10000h
%define WS_MINIMIZEBOX 20000h
%define WS_MAXIMIZEBOX 10000h
%define WS_TILED WS_OVERLAPPED
%define WS_ICONIC WS_MINIMIZE
%define WS_SIZEBOX WS_THICKFRAME
%define WS_OVERLAPPEDWINDOW WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX
%define WS_TILEDWINDOW WS_OVERLAPPEDWINDOW
%define WS_POPUPWINDOW WS_POPUP|WS_BORDER|WS_SYSMENU
%define WS_CHILDWINDOW WS_CHILD
%define WS_EX_DLGMODALFRAME 1h
%define WS_EX_NOPARENTNOTIFY 4h
%define WS_EX_TOPMOST 8h
%define WS_EX_ACCEPTFILES 10h
%define WS_EX_TRANSPARENT 20h
%define WS_EX_MDICHILD 00000040h
%define WS_EX_TOOLWINDOW 00000080h
%define WS_EX_WINDOWEDGE 00000100h
%define WS_EX_CLIENTEDGE 00000200h
%define WS_EX_CONTEXTHELP 00000400h
%define WS_EX_RIGHT 00001000h
%define WS_EX_LEFT 00000000h
%define WS_EX_RTLREADING 00002000h
%define WS_EX_LTRREADING 00000000h
%define WS_EX_LEFTSCROLLBAR 00004000h
%define WS_EX_RIGHTSCROLLBAR 00000000h
%define WS_EX_CONTROLPARENT 00010000h
%define WS_EX_STATICEDGE 00020000h
%define WS_EX_APPWINDOW 00040000h
%define WS_EX_OVERLAPPEDWINDOW WS_EX_WINDOWEDGE|WS_EX_CLIENTEDGE
%define WS_EX_PALETTEWINDOW WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST
%define CS_VREDRAW 1h
%define CS_HREDRAW 2h
%define CS_KEYCVTWINDOW 4h
%define CS_DBLCLKS 8h
%define CS_OWNDC 20h
%define CS_CLASSDC 40h
%define CS_PARENTDC 80h
%define CS_NOKEYCVT 100h
%define CS_NOCLOSE 200h
%define CS_SAVEBITS 800h
%define CS_BYTEALIGNCLIENT 1000h
%define CS_BYTEALIGNWINDOW 2000h
%define CS_PUBLICCLASS 4000h
%define CS_GLOBALCLASS CS_PUBLICCLASS
%define CF_TEXT 1
%define CF_BITMAP 2
%define CF_METAFILEPICT 3
%define CF_SYLK 4
%define CF_DIF 5
%define CF_TIFF 6
%define CF_OEMTEXT 7
%define CF_DIB 8
%define CF_PALETTE 9
%define CF_PENDATA 10
%define CF_RIFF 11
%define CF_WAVE 12
%define CF_OWNERDISPLAY 80h
%define CF_DSPTEXT 81h
%define CF_DSPBITMAP 82h
%define CF_DSPMETAFILEPICT 83h
%define CF_DSPENHMETAFILE 8Eh
%define CF_PRIVATEFIRST 200h
%define CF_PRIVATELAST 2FFh
%define CF_GDIOBJFIRST 300h
%define CF_GDIOBJLAST 3FFh
%define FVIRTKEY 1h
%define FNOINVERT 2h
%define FSHIFT 4h
%define FCONTROL 8h
%define FALT 10h
%define WPF_SETMINPOSITION 1h
%define WPF_RESTORETOMAXIMIZED 2h
%define ODT_MENU 1
%define ODT_LISTBOX 2
%define ODT_COMBOBOX 3
%define ODT_BUTTON 4
%define ODA_DRAWENTIRE 1h
%define ODA_SELECT 2h
%define ODA_FOCUS 4h
%define ODS_SELECTED 1h
%define ODS_GRAYED 2h
%define ODS_DISABLED 4h
%define ODS_CHECKED 8h
%define ODS_FOCUS 10h
%define PM_NOREMOVE 0h
%define PM_REMOVE 1h
%define PM_NOYIELD 2h
%define MOD_ALT 1h
%define MOD_CONTROL 2h
%define MOD_SHIFT 4h
%define IDHOT_SNAPWINDOW -1
%define IDHOT_SNAPDESKTOP -2
%define EWX_LOGOFF 0
%define EWX_SHUTDOWN 1
%define EWX_REBOOT 2
%define EWX_FORCE 4
%define EW_RESTARTWINDOWS 42h
%define READAPI 0
%define WRITEAPI 1
%define READ_WRITE 2
%define HWND_BROADCAST 0FFFFh
%define CW_USEDEFAULT 80000000h
%define HWND_DESKTOP 0
%define SWP_NOSIZE 1h
%define SWP_NOMOVE 2h
%define SWP_NOZORDER 4h
%define SWP_NOREDRAW 8h
%define SWP_NOACTIVATE 10h
%define SWP_FRAMECHANGED 20h
%define SWP_SHOWWINDOW 40h
%define SWP_HIDEWINDOW 80h
%define SWP_NOCOPYBITS 100h
%define SWP_NOOWNERZORDER 200h
%define SWP_DRAWFRAME SWP_FRAMECHANGED
%define SWP_NOREPOSITION SWP_NOOWNERZORDER
%define HWND_TOP 0
%define HWND_BOTTOM 1
%define HWND_TOPMOST -1
%define HWND_NOTOPMOST -2
%define DLGWINDOWEXTRA 30
%define KEYEVENTF_EXTENDEDKEY 1h
%define KEYEVENTF_KEYUP 2h

;	PropSheetPageProc
%assign	PSPCB_CREATE			2
%assign	PSPCB_RELEASE			1
;	PropertySheet
%assign	IS_PSREBOOTSYSTEM		3
%assign	IS_PSRESTARTWINDOWS		2
;	PrepareTape
%assign	TAPE_FORMAT			0x5
%assign	TAPE_LOAD			0
%assign	TAPE_LOCK			0x3
%assign	TAPE_TENSION			0x2
%assign	TAPE_UNLOAD			0x1
%assign	TAPE_UNLOCK			0x4

%define MOUSEEVENTF_MOVE 1h
%define MOUSEEVENTF_LEFTDOWN 2h
%define MOUSEEVENTF_LEFTUP 4h
%define MOUSEEVENTF_RIGHTDOWN 8h
%define MOUSEEVENTF_RIGHTUP 10h
%define MOUSEEVENTF_MIDDLEDOWN 20h
%define MOUSEEVENTF_MIDDLEUP 40h
%define MOUSEEVENTF_ABSOLUTE 8000h
%define QS_KEY 1h
%define QS_MOUSEMOVE 2h
%define QS_MOUSEBUTTON 4h
%define QS_POSTMESSAGE 8h
%define QS_TIMER 10h
%define QS_PAINT 20h
%define QS_SENDMESSAGE 40h
%define QS_HOTKEY 80h
%define QS_MOUSE QS_MOUSEMOVE|QS_MOUSEBUTTON
%define QS_INPUT QS_MOUSE|QS_KEY
%define QS_ALLEVENTS QS_INPUT|QS_POSTMESSAGE|QS_TIMER|QS_PAINT|QS_HOTKEY
%define QS_ALLINPUT QS_SENDMESSAGE|QS_PAINT|QS_TIMER|QS_POSTMESSAGE|QS_MOUSEBUTTON|QS_MOUSEMOVE|QS_HOTKEY|QS_KEY
%define SM_CXSCREEN 0
%define SM_CYSCREEN 1
%define SM_CXVSCROLL 2
%define SM_CYHSCROLL 3
%define SM_CYCAPTION 4
%define SM_CXBORDER 5
%define SM_CYBORDER 6
%define SM_CXDLGFRAME 7
%define SM_CYDLGFRAME 8
%define SM_CYVTHUMB 9
%define SM_CXHTHUMB 10
%define SM_CXICON 11
%define SM_CYICON 12
%define SM_CXCURSOR 13
%define SM_CYCURSOR 14
%define SM_CYMENU 15
%define SM_CXFULLSCREEN 16
%define SM_CYFULLSCREEN 17
%define SM_CYKANJIWINDOW 18
%define SM_MOUSEPRESENT 19
%define SM_CYVSCROLL 20
%define SM_CXHSCROLL 21
%define SM_DEBUG 22
%define SM_SWAPBUTTON 23
%define SM_RESERVED1 24
%define SM_RESERVED2 25
%define SM_RESERVED3 26
%define SM_RESERVED4 27
%define SM_CXMIN 28
%define SM_CYMIN 29
%define SM_CXSIZE 30
%define SM_CYSIZE 31
%define SM_CXFRAME 32
%define SM_CYFRAME 33
%define SM_CXMINTRACK 34
%define SM_CYMINTRACK 35
%define SM_CXDOUBLECLK 36
%define SM_CYDOUBLECLK 37
%define SM_CXICONSPACING 38
%define SM_CYICONSPACING 39
%define SM_MENUDROPALIGNMENT 40
%define SM_PENWINDOWS 41
%define SM_DBCSENABLED 42
%define SM_CMOUSEBUTTONS 43
%define SM_CXFIXEDFRAME SM_CXDLGFRAME
%define SM_CYFIXEDFRAME SM_CYDLGFRAME
%define SM_CXSIZEFRAME SM_CXFRAME
%define SM_CYSIZEFRAME SM_CYFRAME
%define SM_SECURE 44
%define SM_CXEDGE 45
%define SM_CYEDGE 46
%define SM_CXMINSPACING 47
%define SM_CYMINSPACING 48
%define SM_CXSMICON 49
%define SM_CYSMICON 50
%define SM_CYSMCAPTION 51
%define SM_CXSMSIZE 52
%define SM_CYSMSIZE 53
%define SM_CXMENUSIZE 54
%define SM_CYMENUSIZE 55
%define SM_ARRANGE 56
%define SM_CXMINIMIZED 57
%define SM_CYMINIMIZED 58
%define SM_CXMAXTRACK 59
%define SM_CYMAXTRACK 60
%define SM_CXMAXIMIZED 61
%define SM_CYMAXIMIZED 62
%define SM_NETWORK 63
%define SM_CLEANBOOT 67
%define SM_CXDRAG 68
%define SM_CYDRAG 69
%define SM_SHOWSOUNDS 70
%define SM_CXMENUCHECK 71
%define SM_CYMENUCHECK 72
%define SM_SLOWMACHINE 73
%define SM_MIDEASTENABLED 74
%define SM_CMETRICS 75
%define TPM_LEFTBUTTON 0h
%define TPM_RIGHTBUTTON 2h
%define TPM_LEFTALIGN 0h
%define TPM_CENTERALIGN 4h
%define TPM_RIGHTALIGN 8h
%define DT_TOP 0h
%define DT_LEFT 0h
%define DT_CENTER 1h
%define DT_RIGHT 2h
%define DT_VCENTER 4h
%define DT_BOTTOM 8h
%define DT_WORDBREAK 10h
%define DT_SINGLELINE 20h
%define DT_EXPANDTABS 40h
%define DT_TABSTOP 80h
%define DT_NOCLIP 100h
%define DT_EXTERNALLEADING 200h
%define DT_CALCRECT 400h
%define DT_NOPREFIX 800h
%define DT_INTERNAL 1000h
%define DCX_WINDOW 1h
%define DCX_CACHE 2h
%define DCX_NORESETATTRS 4h
%define DCX_CLIPCHILDREN 8h
%define DCX_CLIPSIBLINGS 10h
%define DCX_PARENTCLIP 20h
%define DCX_EXCLUDERGN 40h
%define DCX_INTERSECTRGN 80h
%define DCX_EXCLUDEUPDATE 100h
%define DCX_INTERSECTUPDATE 200h
%define DCX_LOCKWINDOWUPDATE 400h
%define DCX_NORECOMPUTE 100000h
%define DCX_VALIDATE 200000h
%define RDW_INVALIDATE 1h
%define RDW_INTERNALPAINT 2h
%define RDW_ERASE 4h
%define RDW_VALIDATE 8h
%define RDW_NOINTERNALPAINT 10h
%define RDW_NOERASE 20h
%define RDW_NOCHILDREN 40h
%define RDW_ALLCHILDREN 80h
%define RDW_UPDATENOW 100h
%define RDW_ERASENOW 200h
%define RDW_FRAME 400h
%define RDW_NOFRAME 800h
%define SW_SCROLLCHILDREN 1h
%define SW_INVALIDATE 2h
%define SW_ERASE 4h
%define ESB_ENABLE_BOTH 0h
%define ESB_DISABLE_BOTH 3h
%define ESB_DISABLE_LEFT 1h
%define ESB_DISABLE_RIGHT 2h
%define ESB_DISABLE_UP 1h
%define ESB_DISABLE_DOWN 2h
%define ESB_DISABLE_LTUP ESB_DISABLE_LEFT
%define ESB_DISABLE_RTDN ESB_DISABLE_RIGHT
%define MB_OK 0h
%define MB_OKCANCEL 1h
%define MB_ABORTRETRYIGNORE 2h
%define MB_YESNOCANCEL 3h
%define MB_YESNO 4h
%define MB_RETRYCANCEL 5h
%define MB_ICONHAND 10h
%define MB_ICONQUESTION 20h
%define MB_ICONEXCLAMATION 30h
%define MB_ICONASTERISK 40h
%define MB_ICONERROR MB_ICONHAND
%define MB_ICONINFORMATION MB_ICONASTERISK
%define MB_ICONSTOP MB_ICONHAND
%define MB_ICONWARNING MB_ICONEXCLAMATION
%define MB_DEFBUTTON1 0h
%define MB_DEFBUTTON2 100h
%define MB_DEFBUTTON3 200h
%define MB_APPLMODAL 0h
%define MB_SYSTEMMODAL 1000h
%define MB_TASKMODAL 2000h
%define MB_NOFOCUS 8000h
%define MB_SETFOREGROUND 10000h
%define MB_DEFAULT_DESKTOP_ONLY 20000h
%define MB_TYPEMASK 0Fh
%define MB_ICONMASK 0F0h
%define MB_DEFMASK 0F00h
%define MB_MODEMASK 3000h
%define MB_MISCMASK 0C000h
%define CTLCOLOR_MSGBOX 0
%define CTLCOLOR_EDIT 1
%define CTLCOLOR_LISTBOX 2
%define CTLCOLOR_BTN 3
%define CTLCOLOR_DLG 4
%define CTLCOLOR_SCROLLBAR 5
%define CTLCOLOR_STATIC 6
%define CTLCOLOR_MAX 8
%define COLOR_SCROLLBAR 0
%define COLOR_BACKGROUND 1
%define COLOR_ACTIVECAPTION 2
%define COLOR_INACTIVECAPTION 3
%define COLOR_MENU 4
%define COLOR_WINDOW 5
%define COLOR_WINDOWFRAME 6
%define COLOR_MENUTEXT 7
%define COLOR_WINDOWTEXT 8
%define COLOR_CAPTIONTEXT 9
%define COLOR_ACTIVEBORDER 10
%define COLOR_INACTIVEBORDER 11
%define COLOR_APPWORKSPACE 12
%define COLOR_HIGHLIGHT 13
%define COLOR_HIGHLIGHTTEXT 14
%define COLOR_BTNFACE 15
%define COLOR_BTNSHADOW 16
%define COLOR_GRAYTEXT 17
%define COLOR_BTNTEXT 18
%define COLOR_INACTIVECAPTIONTEXT 19
%define COLOR_BTNHIGHLIGHT 20
%define COLOR_3DDKSHADOW 21
%define COLOR_3DLIGHT 22
%define COLOR_INFOTEXT 23
%define COLOR_INFOBK 24
%define COLOR_DESKTOP COLOR_BACKGROUND
%define COLOR_3DFACE COLOR_BTNFACE
%define COLOR_3DSHADOW COLOR_BTNSHADOW
%define COLOR_3DHIGHLIGHT COLOR_BTNHIGHLIGHT
%define COLOR_3DHILIGHT COLOR_BTNHIGHLIGHT
%define COLOR_BTNHILIGHT COLOR_BTNHIGHLIGHT
%define GW_HWNDFIRST 0
%define GW_HWNDLAST 1
%define GW_HWNDNEXT 2
%define GW_HWNDPREV 3
%define GW_OWNER 4
%define GW_CHILD 5
%define GW_MAX 5
%define MF_INSERT 0h
%define MF_CHANGE 80h
%define MF_APPEND 100h
%define MF_DELETE 200h
%define MF_REMOVE 1000h
%define MF_BYCOMMAND 0h
%define MF_BYPOSITION 400h
%define MF_SEPARATOR 800h
%define MF_ENABLED 0h
%define MF_GRAYED 1h
%define MF_DISABLED 2h
%define MF_UNCHECKED 0h
%define MF_CHECKED 8h
%define MF_USECHECKBITMAPS 200h
%define MF_STRING 0h
%define MF_BITMAP 4h
%define MF_OWNERDRAW 100h
%define MF_POPUP 10h
%define MF_MENUBARBREAK 20h
%define MF_MENUBREAK 40h
%define MF_UNHILITE 0h
%define MF_HILITE 80h
%define MF_SYSMENU 2000h
%define MF_HELP 4000h
%define MF_MOUSESELECT 8000h
%define MF_END 80h
%define SC_SIZE 0F000h
%define SC_MOVE 0F010h
%define SC_MINIMIZE 0F020h
%define SC_MAXIMIZE 0F030h
%define SC_NEXTWINDOW 0F040h
%define SC_PREVWINDOW 0F050h
%define SC_CLOSE 0F060h
%define SC_VSCROLL 0F070h
%define SC_HSCROLL 0F080h
%define SC_MOUSEMENU 0F090h
%define SC_KEYMENU 0F100h
%define SC_ARRANGE 0F110h
%define SC_RESTORE 0F120h
%define SC_TASKLIST 0F130h
%define SC_SCREENSAVE 0F140h
%define SC_HOTKEY 0F150h
%define SC_ICON SC_MINIMIZE
%define SC_ZOOM SC_MAXIMIZE
%define IDC_ARROW 32512
%define IDC_IBEAM 32513
%define IDC_WAIT 32514
%define IDC_CROSS 32515
%define IDC_UPARROW 32516
%define IDC_SIZE 32640
%define IDC_ICON 32641
%define IDC_SIZENWSE 32642
%define IDC_SIZENESW 32643
%define IDC_SIZEWE 32644
%define IDC_SIZENS 32645
%define IDC_SIZEALL 32646
%define IDC_NO 32648
%define IDC_APPSTARTING 32650
%define OBM_CLOSE 32754
%define OBM_UPARROW 32753
%define OBM_DNARROW 32752
%define OBM_RGARROW 32751
%define OBM_LFARROW 32750
%define OBM_REDUCE 32749
%define OBM_ZOOM 32748
%define OBM_RESTORE 32747
%define OBM_REDUCED 32746
%define OBM_ZOOMD 32745
%define OBM_RESTORED 32744
%define OBM_UPARROWD 32743
%define OBM_DNARROWD 32742
%define OBM_RGARROWD 32741
%define OBM_LFARROWD 32740
%define OBM_MNARROW 32739
%define OBM_COMBO 32738
%define OBM_UPARROWI 32737
%define OBM_DNARROWI 32736
%define OBM_RGARROWI 32735
%define OBM_LFARROWI 32734
%define OBM_OLD_CLOSE 32767
%define OBM_SIZE 32766
%define OBM_OLD_UPARROW 32765
%define OBM_OLD_DNARROW 32764
%define OBM_OLD_RGARROW 32763
%define OBM_OLD_LFARROW 32762
%define OBM_BTSIZE 32761
%define OBM_CHECK 32760
%define OBM_CHECKBOXES 32759
%define OBM_BTNCORNERS 32758
%define OBM_OLD_REDUCE 32757
%define OBM_OLD_ZOOM 32756

;	SetTapePosition
%assign	TAPE_ABSOLUTE_BLOCK		0x1
%assign	TAPE_LOGICAL_BLOCK		0x2
%assign	TAPE_REWIND			0
%assign	TAPE_SPACE_END_OF_DATA		0x4
%assign	TAPE_SPACE_FILEMARKS		0x6
%assign	TAPE_SPACE_RELATIVE_BLOCKS	0x5
%assign	TAPE_SPACE_SEQUENTIAL_FMKS	0x7
%assign	TAPE_SPACE_SEQUENTIAL_SMKS	0x9
%assign	TAPE_SPACE_SETMARKS		0x8
;	LoadLibraryEx
%assign	DONT_RESOLVE_DLL_REFERENCES	1
%assign	LOAD_LIBRARY_AS_DATAFILE	2
%assign	LOAD_WITH_ALTERED_SEARCH_PATH	8

%define OBM_OLD_RESTORE 32755
%define OCR_NORMAL 32512
%define OCR_IBEAM 32513
%define OCR_WAIT 32514
%define OCR_CROSS 32515
%define OCR_UP 32516
%define OCR_SIZE 32640
%define OCR_ICON 32641
%define OCR_SIZENWSE 32642
%define OCR_SIZENESW 32643
%define OCR_SIZEWE 32644
%define OCR_SIZENS 32645
%define OCR_SIZEALL 32646
%define OCR_ICOCUR 32647
%define OCR_NO 32648
%define OIC_SAMPLE 32512
%define OIC_HAND 32513
%define OIC_QUES 32514
%define OIC_BANG 32515
%define OIC_NOTE 32516
%define ORD_LANGDRIVER 1
%define IDI_APPLICATION 32512
%define IDI_HAND 32513
%define IDI_QUESTION 32514
%define IDI_EXCLAMATION 32515
%define IDI_ASTERISK 32516
%define IDOK 1
%define IDCANCEL 2
%define IDABORT 3
%define IDRETRY 4
%define IDIGNORE 5
%define IDYES 6
%define IDNO 7
%define ES_LEFT 0h
%define ES_CENTER 1h
%define ES_RIGHT 2h
%define ES_MULTILINE 4h
%define ES_UPPERCASE 8h
%define ES_LOWERCASE 10h
%define ES_PASSWORD 20h
%define ES_AUTOVSCROLL 40h
%define ES_AUTOHSCROLL 80h
%define ES_NOHIDESEL 100h
%define ES_OEMCONVERT 400h
%define ES_READONLY 800h
%define ES_WANTRETURN 1000h
%define EN_SETFOCUS 100h
%define EN_KILLFOCUS 200h
%define EN_CHANGE 300h
%define EN_UPDATE 400h
%define EN_ERRSPACE 500h
%define EN_MAXTEXT 501h
%define EN_HSCROLL 601h
%define EN_VSCROLL 602h
%define EM_GETSEL 0B0h
%define EM_SETSEL 0B1h
%define EM_GETRECT 0B2h
%define EM_SETRECT 0B3h
%define EM_SETRECTNP 0B4h
%define EM_SCROLL 0B5h
%define EM_LINESCROLL 0B6h
%define EM_SCROLLCARET 0B7h
%define EM_GETMODIFY 0B8h
%define EM_SETMODIFY 0B9h
%define EM_GETLINECOUNT 0BAh
%define EM_LINEINDEX 0BBh
%define EM_SETHANDLE 0BCh
%define EM_GETHANDLE 0BDh
%define EM_GETTHUMB 0BEh
%define EM_LINELENGTH 0C1h
%define EM_REPLACESEL 0C2h
%define EM_GETLINE 0C4h
%define EM_LIMITTEXT 0C5h
%define EM_CANUNDO 0C6h
%define EM_UNDO 0C7h
%define EM_FMTLINES 0C8h
%define EM_LINEFROMCHAR 0C9h
%define EM_SETTABSTOPS 0CBh
%define EM_SETPASSWORDCHAR 0CCh
%define EM_EMPTYUNDOBUFFER 0CDh
%define EM_GETFIRSTVISIBLELINE 0CEh
%define EM_SETREADONLY 0CFh
%define EM_SETWORDBREAKPROC 0D0h
%define EM_GETWORDBREAKPROC 0D1h
%define EM_GETPASSWORDCHAR 0D2h
%define EM_SETMARGINS 0D3h
%define EM_GETMARGINS 0D4h
%define EM_SETLIMITTEXT EM_LIMITTEXT
%define EM_GETLIMITTEXT 0D5h
%define EM_POSFROMCHAR 0D6h
%define EM_CHARFROMPOS 0D7h
%define WB_LEFT 0
%define WB_RIGHT 1
%define WB_ISDELIMITER 2
%define BS_PUSHBUTTON 0h
%define BS_DEFPUSHBUTTON 1h
%define BS_CHECKBOX 2h
%define BS_AUTOCHECKBOX 3h
%define BS_RADIOBUTTON 4h
%define BS_3STATE 5h
%define BS_AUTO3STATE 6h
%define BS_GROUPBOX 7h
%define BS_USERBUTTON 8h
%define BS_AUTORADIOBUTTON 9h
%define BS_OWNERDRAW 0Bh
%define BS_LEFTTEXT 20h
%define BS_BITMAP 80h
%define BS_ICON 40h
%define BN_CLICKED 0
%define BN_PAINT 1
%define BN_HILITE 2
%define BN_UNHILITE 3
%define BN_DISABLE 4
%define BN_DOUBLECLICKED 5
%define BN_SETFOCUS 6
%define BN_KILLFOCUS 7
%define BST_UNCHECKED 00h
%define BST_CHECKED 01h
%define BST_INDETERMINATE 02h
%define BST_PUSHED 04h
%define BM_GETCHECK 0F0h
%define BM_SETCHECK 0F1h
%define BM_GETSTATE 0F2h
%define BM_SETSTATE 0F3h
%define BM_SETSTYLE 0F4h
%define BM_CLICK 0F5h
%define BM_GETIMAGE 0F6h
%define BM_SETIMAGE 0F7h
%define SS_LEFT 0h
%define SS_CENTER 1h
%define SS_RIGHT 2h
%define SS_ICON 3h
%define SS_BLACKRECT 4h
%define SS_GRAYRECT 5h
%define SS_WHITERECT 6h
%define SS_BLACKFRAME 7h
%define SS_GRAYFRAME 8h
%define SS_WHITEFRAME 9h
%define SS_USERITEM 0Ah
%define SS_SIMPLE 0Bh
%define SS_LEFTNOWORDWRAP 0Ch
%define SS_NOPREFIX 80h
%define STM_SETICON 170h
%define STM_GETICON 171h
%define STM_MSGMAX 172h
%define WC_DIALOG 8002
%define DWL_MSGRESULT 0
%define DWL_DLGPROC 4
%define DWL_USER 8

;       GlobalAlloc, GlobalFlags
%assign GMEM_FIXED                      0
%assign GMEM_MOVEABLE                   2
%assign GPTR                            64
%assign GHND                            66
%assign GMEM_DDESHARE                   8192
%assign GMEM_DISCARDABLE                256
%assign GMEM_LOWER                      4096
%assign GMEM_NOCOMPACT                  16
%assign GMEM_NODISCARD                  32
%assign GMEM_NOT_BANKED                 4096
%assign GMEM_NOTIFY                     16384
%assign GMEM_SHARE                      8192
%assign GMEM_ZEROINIT                   64
%assign GMEM_DISCARDED                  16384
%assign GMEM_INVALID_HANDLE             32768
%assign GMEM_LOCKCOUNT                  255

;       HeapAlloc, HeapReAlloc
%assign HEAP_GENERATE_EXCEPTIONS        4
%assign HEAP_NO_SERIALIZE               1
%assign HEAP_ZERO_MEMORY                8
%assign STATUS_NO_MEMORY                0xC0000017
%assign STATUS_ACCESS_VIOLATION         0xC0000005
%assign HEAP_REALLOC_IN_PLACE_ONLY      16


%define DDL_READWRITE 0h
%define DDL_READONLY 1h
%define DDL_HIDDEN 2h
%define DDL_SYSTEM 4h
%define DDL_DIRECTORY 10h
%define DDL_ARCHIVE 20h
%define DDL_POSTMSGS 2000h
%define DDL_DRIVES 4000h
%define DDL_EXCLUSIVE 8000h
%define DS_ABSALIGN 0001h
%define DS_SYSMODAL 0002h
%define DS_3DLOOK 0004h
%define DS_FIXEDSYS 0008h
%define DS_NOFAILCREATE 0010h
%define DS_LOCALEDIT 0020h
%define DS_SETFONT 0040h
%define DS_MODALFRAME 0080h
%define DS_NOIDLEMSG 0100h
%define DS_SETFOREGROUND 0200h
%define DS_CONTROL 0400h
%define DS_CENTER 0800h
%define DS_CENTERMOUSE 1000h
%define DS_CONTEXTHELP 2000h
%define DM_GETDEFID WM_USER+0
%define DM_SETDEFID WM_USER+1
%define DC_HASDEFID 534h
%define DLGC_WANTARROWS 1h
%define DLGC_WANTTAB 2h
%define DLGC_WANTALLKEYS 4h
%define DLGC_WANTMESSAGE 4h
%define DLGC_HASSETSEL 8h
%define DLGC_DEFPUSHBUTTON 10h
%define DLGC_UNDEFPUSHBUTTON 20h
%define DLGC_RADIOBUTTON 40h
%define DLGC_WANTCHARS 80h
%define DLGC_STATIC 100h
%define DLGC_BUTTON 2000h
%define LB_CTLCODE 0
%define LB_OKAY 0
%define LB_ERR -1
%define LB_ERRSPACE -2
%define LBN_ERRSPACE -2
%define LBN_SELCHANGE 1
%define LBN_DBLCLK 2
%define LBN_SELCANCEL 3
%define LBN_SETFOCUS 4
%define LBN_KILLFOCUS 5
%define LB_ADDSTRING 180h
%define LB_INSERTSTRING 181h
%define LB_DELETESTRING 182h
%define LB_SELITEMRANGEEX 183h
%define LB_RESETCONTENT 184h
%define LB_SETSEL 185h
%define LB_SETCURSEL 186h
%define LB_GETSEL 187h
%define LB_GETCURSEL 188h
%define LB_GETTEXT 189h
%define LB_GETTEXTLEN 18Ah
%define LB_GETCOUNT 18Bh
%define LB_SELECTSTRING 18Ch
%define LB_DIR 18Dh
%define LB_GETTOPINDEX 18Eh
%define LB_FINDSTRING 18Fh
%define LB_GETSELCOUNT 190h
%define LB_GETSELITEMS 191h
%define LB_SETTABSTOPS 192h
%define LB_GETHORIZONTALEXTENT 193h
%define LB_SETHORIZONTALEXTENT 194h
%define LB_SETCOLUMNWIDTH 195h
%define LB_ADDFILE 196h
%define LB_SETTOPINDEX 197h
%define LB_GETITEMRECT 198h
%define LB_GETITEMDATA 199h
%define LB_SETITEMDATA 19Ah
%define LB_SELITEMRANGE 19Bh
%define LB_SETANCHORINDEX 19Ch
%define LB_GETANCHORINDEX 19Dh
%define LB_SETCARETINDEX 19Eh
%define LB_GETCARETINDEX 19Fh
%define LB_SETITEMHEIGHT 1A0h
%define LB_GETITEMHEIGHT 1A1h
%define LB_FINDSTRINGEXACT 1A2h
%define LB_SETLOCALE 1A5h
%define LB_GETLOCALE 1A6h
%define LB_SETCOUNT 1A7h
%define LB_MSGMAX 1A8h
%define LBS_NOTIFY 1h
%define LBS_SORT 2h
%define LBS_NOREDRAW 4h
%define LBS_MULTIPLESEL 8h
%define LBS_OWNERDRAWFIXED 10h
%define LBS_OWNERDRAWVARIABLE 20h
%define LBS_HASSTRINGS 40h
%define LBS_USETABSTOPS 80h
%define LBS_NOINTEGRALHEIGHT 100h
%define LBS_MULTICOLUMN 200h
%define LBS_WANTKEYBOARDINPUT 400h
%define LBS_EXTENDEDSEL 800h
%define LBS_DISABLENOSCROLL 1000h
%define LBS_NODATA 2000h
%define LBS_STANDARD LBS_NOTIFY|LBS_SORT|WS_VSCROLL|WS_BORDER
%define CB_OKAY 0
%define CB_ERR -1
%define CB_ERRSPACE -2
%define CBN_ERRSPACE -1
%define CBN_SELCHANGE 1
%define CBN_DBLCLK 2
%define CBN_SETFOCUS 3
%define CBN_KILLFOCUS 4
%define CBN_EDITCHANGE 5
%define CBN_EDITUPDATE 6
%define CBN_DROPDOWN 7
%define CBN_CLOSEUP 8
%define CBN_SELENDOK 9
%define CBN_SELENDCANCEL 10
%define CBS_SIMPLE 1h
%define CBS_DROPDOWN 2h
%define CBS_DROPDOWNLIST 3h
%define CBS_OWNERDRAWFIXED 10h
%define CBS_OWNERDRAWVARIABLE 20h
%define CBS_AUTOHSCROLL 40h
%define CBS_OEMCONVERT 80h
%define CBS_SORT 100h
%define CBS_HASSTRINGS 200h
%define CBS_NOINTEGRALHEIGHT 400h
%define CBS_DISABLENOSCROLL 800h
%define CB_GETEDITSEL 140h
%define CB_LIMITTEXT 141h
%define CB_SETEDITSEL 142h
%define CB_ADDSTRING 143h
%define CB_DELETESTRING 144h
%define CB_DIR 145h
%define CB_GETCOUNT 146h
%define CB_GETCURSEL 147h
%define CB_GETLBTEXT 148h
%define CB_GETLBTEXTLEN 149h
%define CB_INSERTSTRING 14Ah
%define CB_RESETCONTENT 14Bh
%define CB_FINDSTRING 14Ch
%define CB_SELECTSTRING 14Dh
%define CB_SETCURSEL 14Eh
%define CB_SHOWDROPDOWN 14Fh
%define CB_GETITEMDATA 150h
%define CB_SETITEMDATA 151h
%define CB_GETDROPPEDCONTROLRECT 152h
%define CB_SETITEMHEIGHT 153h
%define CB_GETITEMHEIGHT 154h
%define CB_SETEXTENDEDUI 155h
%define CB_GETEXTENDEDUI 156h
%define CB_GETDROPPEDSTATE 157h
%define CB_FINDSTRINGEXACT 158h
%define CB_SETLOCALE 159h
%define CB_GETLOCALE 15Ah
%define CB_GETTOPINDEX 15Bh
%define CB_SETTOPINDEX 15Ch
%define CB_GETHORIZONTALEXTENT 15Dh
%define CB_SETHORIZONTALEXTENT 15Eh
%define CB_GETDROPPEDWIDTH 15Fh
%define CB_SETDROPPEDWIDTH 160h
%define CB_INITSTORAGE 161h
%define CB_MSGMAX 162h
%define SBS_HORZ 0h
%define SBS_VERT 1h
%define SBS_TOPALIGN 2h
%define SBS_LEFTALIGN 2h
%define SBS_BOTTOMALIGN 4h
%define SBS_RIGHTALIGN 4h
%define SBS_SIZEBOXTOPLEFTALIGN 2h
%define SBS_SIZEBOXBOTTOMRIGHTALIGN 4h
%define SBS_SIZEBOX 8h
%define SBS_SIZEGRIP 10h
%define SBM_SETPOS 0E0h
%define SBM_GETPOS 0E1h
%define SBM_SETRANGE 0E2h
%define SBM_SETRANGEREDRAW 0E6h
%define SBM_GETRANGE 0E3h
%define SBM_ENABLE_ARROWS 0E4h
%define MDIS_ALLCHILDSTYLES 1h
%define MDITILE_VERTICAL 0h
%define MDITILE_HORIZONTAL 1h
%define MDITILE_SKIPDISABLED 2h
%define HELP_CONTEXT 1h
%define HELP_QUIT 2h
%define HELP_INDEX 3h
%define HELP_CONTENTS 3h
%define HELP_HELPONHELP 4h
%define HELP_SETINDEX 5h
%define HELP_SETCONTENTS 5h
%define HELP_CONTEXTPOPUP 8h
%define HELP_FORCEFILE 9h
%define HELP_KEY 101h
%define HELP_COMMAND 102h
%define HELP_PARTIALKEY 105h
%define HELP_MULTIKEY 201h
%define HELP_SETWINPOS 203h
%define HELP_CONTEXTMENU 000Ah
%define HELP_FINDER 000Bh
%define HELP_WM_HELP 000Ch
%define HELP_SETPOPUP_POS 000Dh
%define HELP_TCARD 8000h
%define HELP_TCARD_DATA 0010h
%define HELP_TCARD_OTHER_CALLER 0011h
%define IDH_NO_HELP 28440
%define IDH_MISSING_CONTEXT 28441
%define IDH_GENERIC_HELP_BUTTON 28442
%define IDH_OK 28443
%define IDH_CANCEL 28444
%define IDH_HELP 28445
%define SPI_GETBEEP 1
%define SPI_SETBEEP 2
%define SPI_GETMOUSE 3
%define SPI_SETMOUSE 4
%define SPI_GETBORDER 5
%define SPI_SETBORDER 6
%define SPI_GETKEYBOARDSPEED 10
%define SPI_SETKEYBOARDSPEED 11
%define SPI_LANGDRIVER 12
%define SPI_ICONHORIZONTALSPACING 13
%define SPI_GETSCREENSAVETIMEOUT 14
%define SPI_SETSCREENSAVETIMEOUT 15
%define SPI_GETSCREENSAVEACTIVE 16
%define SPI_SETSCREENSAVEACTIVE 17
%define SPI_GETGRIDGRANULARITY 18
%define SPI_SETGRIDGRANULARITY 19
%define SPI_SETDESKWALLPAPER 20
%define SPI_SETDESKPATTERN 21
%define SPI_GETKEYBOARDDELAY 22
%define SPI_SETKEYBOARDDELAY 23
%define SPI_ICONVERTICALSPACING 24
%define SPI_GETICONTITLEWRAP 25
%define SPI_SETICONTITLEWRAP 26
%define SPI_GETMENUDROPALIGNMENT 27
%define SPI_SETMENUDROPALIGNMENT 28
%define SPI_SETDOUBLECLKWIDTH 29
%define SPI_SETDOUBLECLKHEIGHT 30
%define SPI_GETICONTITLELOGFONT 31
%define SPI_SETDOUBLECLICKTIME 32
%define SPI_SETMOUSEBUTTONSWAP 33
%define SPI_SETICONTITLELOGFONT 34
%define SPI_GETFASTTASKSWITCH 35
%define SPI_SETFASTTASKSWITCH 36
%define SPI_SETDRAGFULLWINDOWS 37
%define SPI_GETDRAGFULLWINDOWS 38
%define SPI_GETNONCLIENTMETRICS 41
%define SPI_SETNONCLIENTMETRICS 42
%define SPI_GETMINIMIZEDMETRICS 43
%define SPI_SETMINIMIZEDMETRICS 44
%define SPI_GETICONMETRICS 45
%define SPI_SETICONMETRICS 46
%define SPI_SETWORKAREA 47
%define SPI_GETWORKAREA 48
%define SPI_SETPENWINDOWS 49
%define SPI_GETFILTERKEYS 50
%define SPI_SETFILTERKEYS 51
%define SPI_GETTOGGLEKEYS 52
%define SPI_SETTOGGLEKEYS 53
%define SPI_GETMOUSEKEYS 54
%define SPI_SETMOUSEKEYS 55
%define SPI_GETSHOWSOUNDS 56
%define SPI_SETSHOWSOUNDS 57
%define SPI_GETSTICKYKEYS 58
%define SPI_SETSTICKYKEYS 59
%define SPI_GETACCESSTIMEOUT 60
%define SPI_SETACCESSTIMEOUT 61
%define SPI_GETSERIALKEYS 62
%define SPI_SETSERIALKEYS 63
%define SPI_GETSOUNDSENTRY 64
%define SPI_SETSOUNDSENTRY 65
%define SPI_GETHIGHCONTRAST 66
%define SPI_SETHIGHCONTRAST 67
%define SPI_GETKEYBOARDPREF 68
%define SPI_SETKEYBOARDPREF 69
%define SPI_GETSCREENREADER 70
%define SPI_SETSCREENREADER 71
%define SPI_GETANIMATION 72
%define SPI_SETANIMATION 73
%define SPI_GETFONTSMOOTHING 74
%define SPI_SETFONTSMOOTHING 75
%define SPI_SETDRAGWIDTH 76
%define SPI_SETDRAGHEIGHT 77
%define SPI_SETHANDHELD 78
%define SPI_GETLOWPOWERTIMEOUT 79
%define SPI_GETPOWEROFFTIMEOUT 80
%define SPI_SETLOWPOWERTIMEOUT 81
%define SPI_SETPOWEROFFTIMEOUT 82
%define SPI_GETLOWPOWERACTIVE 83
%define SPI_GETPOWEROFFACTIVE 84
%define SPI_SETLOWPOWERACTIVE 85
%define SPI_SETPOWEROFFACTIVE 86
%define SPI_SETCURSORS 87
%define SPI_SETICONS 88
%define SPI_GETDEFAULTINPUTLANG 89
%define SPI_SETDEFAULTINPUTLANG 90
%define SPI_SETLANGTOGGLE 91
%define SPI_GETWINDOWSEXTENSION 92
%define SPI_SETMOUSETRAILS 93
%define SPI_GETMOUSETRAILS 94
%define SPI_SCREENSAVERRUNNING 97
%define SPIF_UPDATEINIFILE 1h
%define SPIF_SENDWININICHANGE 2h
%define WM_DDE_FIRST 3E0h
%define WM_DDE_INITIATE WM_DDE_FIRST
%define WM_DDE_TERMINATE WM_DDE_FIRST+1
%define WM_DDE_ADVISE WM_DDE_FIRST+2
%define WM_DDE_UNADVISE WM_DDE_FIRST+3
%define WM_DDE_ACK WM_DDE_FIRST+4
%define WM_DDE_DATA WM_DDE_FIRST+5
%define WM_DDE_REQUEST WM_DDE_FIRST+6
%define WM_DDE_POKE WM_DDE_FIRST+7
%define WM_DDE_EXECUTE WM_DDE_FIRST+8
%define WM_DDE_LAST WM_DDE_FIRST+8
%define XST_NULL 0
%define XST_INCOMPLETE 1
%define XST_CONNECTED 2
%define XST_INIT1 3
%define XST_INIT2 4
%define XST_REQSENT 5
%define XST_DATARCVD 6
%define XST_POKESENT 7
%define XST_POKEACKRCVD 8
%define XST_EXECSENT 9
%define XST_EXECACKRCVD 10
%define XST_ADVSENT 11
%define XST_UNADVSENT 12
%define XST_ADVACKRCVD 13
%define XST_UNADVACKRCVD 14
%define XST_ADVDATASENT 15
%define XST_ADVDATAACKRCVD 16
%define CADV_LATEACK 0FFFFh
%define ST_CONNECTED 1h
%define ST_ADVISE 2h
%define ST_ISLOCAL 4h
%define ST_BLOCKED 8h
%define ST_CLIENT 10h
%define ST_TERMINATED 20h
%define ST_INLIST 40h
%define ST_BLOCKNEXT 80h
%define ST_ISSELF 100h
%define DDE_FACK 8000h
%define DDE_FBUSY 4000h
%define DDE_FDEFERUPD 4000h
%define DDE_FACKREQ 8000h
%define DDE_FRELEASE 2000h
%define DDE_FREQUESTED 1000h
%define DDE_FAPPSTATUS 0FFh
%define DDE_FNOTPROCESSED 0h
%define DDE_FACKRESERVED (-1-DDE_FACK)|DDE_FBUSY|DDE_FAPPSTATUS
%define DDE_FADVRESERVED (-1-DDE_FACKREQ)|DDE_FDEFERUPD
%define DDE_FDATRESERVED (-1-DDE_FACKREQ)|DDE_FRELEASE|DDE_FREQUESTED
%define DDE_FPOKRESERVED (-1-DDE_FRELEASE)
%define CP_WINANSI 1004
%define CP_WINUNICODE 1200
%define XTYPF_NOBLOCK 2h
%define XTYPF_NODATA 4h
%define XTYPF_ACKREQ 8h
%define XCLASS_MASK 0FC00h
%define XCLASS_BOOL 1000h
%define XCLASS_DATA 2000h
%define XCLASS_FLAGS 4000h
%define XCLASS_NOTIFICATION 8000h
%define XTYP_ERROR 0h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define XTYP_ADVDATA 10h|XCLASS_FLAGS
%define XTYP_ADVREQ 20h|XCLASS_DATA|XTYPF_NOBLOCK
%define XTYP_ADVSTART 30h|XCLASS_BOOL
%define XTYP_ADVSTOP 40h|XCLASS_NOTIFICATION
%define XTYP_EXECUTE 50h|XCLASS_FLAGS
%define XTYP_CONNECT 60h|XCLASS_BOOL|XTYPF_NOBLOCK
%define XTYP_CONNECT_CONFIRM 70h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define XTYP_XACT_COMPLETE 80h|XCLASS_NOTIFICATION
%define XTYP_POKE 90h|XCLASS_FLAGS
%define XTYP_REGISTER 0A0h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define XTYP_REQUEST 0B0h|XCLASS_DATA
%define XTYP_DISCONNECT 0C0h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define XTYP_UNREGISTER 0D0h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define XTYP_WILDCONNECT 0E0h|XCLASS_DATA|XTYPF_NOBLOCK
%define XTYP_MASK 0F0h
%define XTYP_SHIFT 4
%define TIMEOUT_ASYNC 0FFFFh
%define QID_SYNC 0FFFFh
%define CBR_BLOCK 0FFFFh
%define CBF_FAIL_SELFCONNECTIONS 1000h
%define CBF_FAIL_CONNECTIONS 2000h
%define CBF_FAIL_ADVISES 4000h
%define CBF_FAIL_EXECUTES 8000h
%define CBF_FAIL_POKES 10000h
%define CBF_FAIL_REQUESTS 20000h
%define CBF_FAIL_ALLSVRXACTIONS 3F000h
%define CBF_SKIP_CONNECT_CONFIRMS 40000h
%define CBF_SKIP_REGISTRATIONS 80000h
%define CBF_SKIP_UNREGISTRATIONS 100000h
%define CBF_SKIP_DISCONNECTS 200000h
%define CBF_SKIP_ALLNOTIFICATIONS 3C0000h
%define APPCMD_CLIENTONLY 10h
%define APPCMD_FILTERINITS 20h
%define APPCMD_MASK 0FF0h
%define APPCLASS_STANDARD 0h
%define APPCLASS_MASK 0Fh
%define EC_ENABLEALL 0
%define EC_ENABLEONE ST_BLOCKNEXT
%define EC_DISABLE ST_BLOCKED
%define EC_QUERYWAITING 2
%define DNS_REGISTER 1h
%define DNS_UNREGISTER 2h
%define DNS_FILTERON 4h
%define DNS_FILTEROFF 8h
%define HDATA_APPOWNED 1h
%define DMLERR_NO_ERROR 0
%define DMLERR_FIRST 4000h
%define DMLERR_ADVACKTIMEOUT 4000h
%define DMLERR_BUSY 4001h
%define DMLERR_DATAACKTIMEOUT 4002h
%define DMLERR_DLL_NOT_INITIALIZED 4003h
%define DMLERR_DLL_USAGE 4004h
%define DMLERR_EXECACKTIMEOUT 4005h
%define DMLERR_INVALIDPARAMETER 4006h
%define DMLERR_LOW_MEMORY 4007h
%define DMLERR_MEMORY_ERROR 4008h
%define DMLERR_NOTPROCESSED 4009h
%define DMLERR_NO_CONV_ESTABLISHED 400Ah
%define DMLERR_POKEACKTIMEOUT 400Bh
%define DMLERR_POSTMSG_FAILED 400Ch
%define DMLERR_REENTRANCY 400Dh
%define DMLERR_SERVER_DIED 400Eh
%define DMLERR_SYS_ERROR 400Fh
%define DMLERR_UNADVACKTIMEOUT 4010h
%define DMLERR_UNFOUND_QUEUE_ID 4011h
%define DMLERR_LAST 4011h
%define MH_CREATE 1
%define MH_KEEP 2
%define MH_DELETE 3
%define MH_CLEANUP 4
%define MAX_MONITORS 4
%define APPCLASS_MONITOR 1h
%define XTYP_MONITOR 0F0h|XCLASS_NOTIFICATION|XTYPF_NOBLOCK
%define MF_HSZ_INFO 1000000h
%define MF_SENDMSGS 2000000h
%define MF_POSTMSGS 4000000h
%define MF_CALLBACKS 8000000h
%define MF_ERRORS 10000000h
%define MF_LINKS 20000000h
%define MF_CONV 40000000h
%define MF_MASK 0FF000000h
%define NO_ERROR 0
%define ERROR_SUCCESS 0
%define ERROR_INVALID_FUNCTION 1
%define ERROR_FILE_NOT_FOUND 2
%define ERROR_PATH_NOT_FOUND 3
%define ERROR_TOO_MANY_OPEN_FILES 4
%define ERROR_ACCESS_DENIED 5
%define ERROR_INVALID_HANDLE 6
%define ERROR_ARENA_TRASHED 7
%define ERROR_NOT_ENOUGH_MEMORY 8
%define ERROR_INVALID_BLOCK 9
%define ERROR_BAD_ENVIRONMENT 10
%define ERROR_BAD_FORMAT 11
%define ERROR_INVALID_ACCESS 12
%define ERROR_INVALID_DATA 13
%define ERROR_OUTOFMEMORY 14
%define ERROR_INVALID_DRIVE 15
%define ERROR_CURRENT_DIRECTORY 16
%define ERROR_NOT_SAME_DEVICE 17
%define ERROR_NO_MORE_FILES 18
%define ERROR_WRITE_PROTECT 19
%define ERROR_BAD_UNIT 20
%define ERROR_NOT_READY 21
%define ERROR_BAD_COMMAND 22
%define ERROR_CRC 23
%define ERROR_BAD_LENGTH 24
%define ERROR_SEEK 25
%define ERROR_NOT_DOS_DISK 26
%define ERROR_SECTOR_NOT_FOUND 27
%define ERROR_OUT_OF_PAPER 28
%define ERROR_WRITE_FAULT 29
%define ERROR_READ_FAULT 30
%define ERROR_GEN_FAILURE 31
%define ERROR_SHARING_VIOLATION 32
%define ERROR_LOCK_VIOLATION 33
%define ERROR_WRONG_DISK 34
%define ERROR_SHARING_BUFFER_EXCEEDED 36
%define ERROR_HANDLE_EOF 38
%define ERROR_HANDLE_DISK_FULL 39
%define ERROR_NOT_SUPPORTED 50
%define ERROR_REM_NOT_LIST 51
%define ERROR_DUP_NAME 52
%define ERROR_BAD_NETPATH 53
%define ERROR_NETWORK_BUSY 54
%define ERROR_DEV_NOT_EXIST 55
%define ERROR_TOO_MANY_CMDS 56
%define ERROR_ADAP_HDW_ERR 57
%define ERROR_BAD_NET_RESP 58
%define ERROR_UNEXP_NET_ERR 59
%define ERROR_BAD_REM_ADAP 60
%define ERROR_PRINTQ_FULL 61
%define ERROR_NO_SPOOL_SPACE 62
%define ERROR_PRINT_CANCELLED 63
%define ERROR_NETNAME_DELETED 64
%define ERROR_NETWORK_ACCESS_DENIED 65
%define ERROR_BAD_DEV_TYPE 66
%define ERROR_BAD_NET_NAME 67
%define ERROR_TOO_MANY_NAMES 68
%define ERROR_TOO_MANY_SESS 69
%define ERROR_SHARING_PAUSED 70
%define ERROR_REQ_NOT_ACCEP 71
%define ERROR_REDIR_PAUSED 72
%define ERROR_FILE_EXISTS 80
%define ERROR_CANNOT_MAKE 82
%define ERROR_FAIL_I24 83
%define ERROR_OUT_OF_STRUCTURES 84
%define ERROR_ALREADY_ASSIGNED 85
%define ERROR_INVALID_PASSWORD 86
%define ERROR_INVALID_PARAMETER 87
%define ERROR_NET_WRITE_FAULT 88
%define ERROR_NO_PROC_SLOTS 89
%define ERROR_TOO_MANY_SEMAPHORES 100
%define ERROR_EXCL_SEM_ALREADY_OWNED 101
%define ERROR_SEM_IS_SET 102
%define ERROR_TOO_MANY_SEM_REQUESTS 103
%define ERROR_INVALID_AT_INTERRUPT_TIME 104
%define ERROR_SEM_OWNER_DIED 105
%define ERROR_SEM_USER_LIMIT 106
%define ERROR_DISK_CHANGE 107
%define ERROR_DRIVE_LOCKED 108
%define ERROR_BROKEN_PIPE 109
%define ERROR_OPEN_FAILED 110
%define ERROR_BUFFER_OVERFLOW 111
%define ERROR_DISK_FULL 112
%define ERROR_NO_MORE_SEARCH_HANDLES 113
%define ERROR_INVALID_TARGET_HANDLE 114
%define ERROR_INVALID_CATEGORY 117
%define ERROR_INVALID_VERIFY_SWITCH 118
%define ERROR_BAD_DRIVER_LEVEL 119
%define ERROR_CALL_NOT_IMPLEMENTED 120
%define ERROR_SEM_TIMEOUT 121
%define ERROR_INSUFFICIENT_BUFFER 122
%define ERROR_INVALID_NAME 123
%define ERROR_INVALID_LEVEL 124
%define ERROR_NO_VOLUME_LABEL 125
%define ERROR_MOD_NOT_FOUND 126
%define ERROR_PROC_NOT_FOUND 127
%define ERROR_WAIT_NO_CHILDREN 128
%define ERROR_CHILD_NOT_COMPLETE 129
%define ERROR_DIRECT_ACCESS_HANDLE 130
%define ERROR_NEGATIVE_SEEK 131
%define ERROR_SEEK_ON_DEVICE 132
%define ERROR_IS_JOIN_TARGET 133
%define ERROR_IS_JOINED 134
%define ERROR_IS_SUBSTED 135
%define ERROR_NOT_JOINED 136
%define ERROR_NOT_SUBSTED 137
%define ERROR_JOIN_TO_JOIN 138
%define ERROR_SUBST_TO_SUBST 139
%define ERROR_JOIN_TO_SUBST 140
%define ERROR_SUBST_TO_JOIN 141
%define ERROR_BUSY_DRIVE 142
%define ERROR_SAME_DRIVE 143
%define ERROR_DIR_NOT_ROOT 144
%define ERROR_DIR_NOT_EMPTY 145
%define ERROR_IS_SUBST_PATH 146
%define ERROR_IS_JOIN_PATH 147
%define ERROR_PATH_BUSY 148
%define ERROR_IS_SUBST_TARGET 149
%define ERROR_SYSTEM_TRACE 150
%define ERROR_INVALID_EVENT_COUNT 151
%define ERROR_TOO_MANY_MUXWAITERS 152
%define ERROR_INVALID_LIST_FORMAT 153
%define ERROR_LABEL_TOO_LONG 154
%define ERROR_TOO_MANY_TCBS 155
%define ERROR_SIGNAL_REFUSED 156
%define ERROR_DISCARDED 157
%define ERROR_NOT_LOCKED 158
%define ERROR_BAD_THREADID_ADDR 159
%define ERROR_BAD_ARGUMENTS 160
%define ERROR_BAD_PATHNAME 161
%define ERROR_SIGNAL_PENDING 162
%define ERROR_MAX_THRDS_REACHED 164
%define ERROR_LOCK_FAILED 167
%define ERROR_BUSY 170
%define ERROR_CANCEL_VIOLATION 173
%define ERROR_ATOMIC_LOCKS_NOT_SUPPORTED 174
%define ERROR_INVALID_SEGMENT_NUMBER 180
%define ERROR_INVALID_ORDINAL 182
%define ERROR_ALREADY_EXISTS 183
%define ERROR_INVALID_FLAG_NUMBER 186
%define ERROR_SEM_NOT_FOUND 187
%define ERROR_INVALID_STARTING_CODESEG 188
%define ERROR_INVALID_STACKSEG 189
%define ERROR_INVALID_MODULETYPE 190
%define ERROR_INVALID_EXE_SIGNATURE 191
%define ERROR_EXE_MARKED_INVALID 192
%define ERROR_BAD_EXE_FORMAT 193
%define ERROR_ITERATED_DATA_EXCEEDS_64k 194
%define ERROR_INVALID_MINALLOCSIZE 195
%define ERROR_DYNLINK_FROM_INVALID_RING 196
%define ERROR_IOPL_NOT_ENABLED 197
%define ERROR_INVALID_SEGDPL 198
%define ERROR_AUTODATASEG_EXCEEDS_64k 199
%define ERROR_RING2SEG_MUST_BE_MOVABLE 200
%define ERROR_RELOC_CHAIN_XEEDS_SEGLIM 201
%define ERROR_INFLOOP_IN_RELOC_CHAIN 202
%define ERROR_ENVVAR_NOT_FOUND 203
%define ERROR_NO_SIGNAL_SENT 205
%define ERROR_FILENAME_EXCED_RANGE 206
%define ERROR_RING2_STACK_IN_USE 207
%define ERROR_META_EXPANSION_TOO_LONG 208
%define ERROR_INVALID_SIGNAL_NUMBER 209
%define ERROR_THREAD_1_INACTIVE 210
%define ERROR_LOCKED 212
%define ERROR_TOO_MANY_MODULES 214
%define ERROR_NESTING_NOT_ALLOWED 215
%define ERROR_BAD_PIPE 230
%define ERROR_PIPE_BUSY 231
%define ERROR_NO_DATA 232
%define ERROR_PIPE_NOT_CONNECTED 233
%define ERROR_MORE_DATA 234
%define ERROR_VC_DISCONNECTED 240
%define ERROR_INVALID_EA_NAME 254
%define ERROR_EA_LIST_INCONSISTENT 255
%define ERROR_NO_MORE_ITEMS 259
%define ERROR_CANNOT_COPY 266
%define ERROR_DIRECTORY 267
%define ERROR_EAS_DIDNT_FIT 275
%define ERROR_EA_FILE_CORRUPT 276
%define ERROR_EA_TABLE_FULL 277
%define ERROR_INVALID_EA_HANDLE 278
%define ERROR_EAS_NOT_SUPPORTED 282
%define ERROR_NOT_OWNER 288
%define ERROR_TOO_MANY_POSTS 298
%define ERROR_MR_MID_NOT_FOUND 317
%define ERROR_INVALID_ADDRESS 487
%define ERROR_ARITHMETIC_OVERFLOW 534
%define ERROR_PIPE_CONNECTED 535
%define ERROR_PIPE_LISTENING 536
%define ERROR_EA_ACCESS_DENIED 994
%define ERROR_OPERATION_ABORTED 995
%define ERROR_IO_INCOMPLETE 996
%define ERROR_IO_PENDING 997
%define ERROR_NOACCESS 998
%define ERROR_SWAPERROR 999
%define ERROR_STACK_OVERFLOW 1001
%define ERROR_INVALID_MESSAGE 1002
%define ERROR_CAN_NOT_COMPLETE 1003
%define ERROR_INVALID_FLAGS 1004
%define ERROR_UNRECOGNIZED_VOLUME 1005
%define ERROR_FILE_INVALID 1006
%define ERROR_FULLSCREEN_MODE 1007
%define ERROR_NO_TOKEN 1008
%define ERROR_BADDB 1009
%define ERROR_BADKEY 1010
%define ERROR_CANTOPEN 1011
%define ERROR_CANTREAD 1012
%define ERROR_CANTWRITE 1013
%define ERROR_REGISTRY_RECOVERED 1014
%define ERROR_REGISTRY_CORRUPT 1015
%define ERROR_REGISTRY_IO_FAILED 1016
%define ERROR_NOT_REGISTRY_FILE 1017
%define ERROR_KEY_DELETED 1018
%define ERROR_NO_LOG_SPACE 1019
%define ERROR_KEY_HAS_CHILDREN 1020
%define ERROR_CHILD_MUST_BE_VOLATILE 1021
%define ERROR_NOTIFY_ENUM_DIR 1022
%define ERROR_DEPENDENT_SERVICES_RUNNING 1051
%define ERROR_INVALID_SERVICE_CONTROL 1052
%define ERROR_SERVICE_REQUEST_TIMEOUT 1053
%define ERROR_SERVICE_NO_THREAD 1054
%define ERROR_SERVICE_DATABASE_LOCKED 1055
%define ERROR_SERVICE_ALREADY_RUNNING 1056
%define ERROR_INVALID_SERVICE_ACCOUNT 1057
%define ERROR_SERVICE_DISABLED 1058
%define ERROR_CIRCULAR_DEPENDENCY 1059
%define ERROR_SERVICE_DOES_NOT_EXIST 1060
%define ERROR_SERVICE_CANNOT_ACCEPT_CTRL 1061
%define ERROR_SERVICE_NOT_ACTIVE 1062
%define ERROR_FAILED_SERVICE_CONTROLLER_CONNECT 1063
%define ERROR_EXCEPTION_IN_SERVICE 1064
%define ERROR_DATABASE_DOES_NOT_EXIST 1065
%define ERROR_SERVICE_SPECIFIC_ERROR 1066
%define ERROR_PROCESS_ABORTED 1067
%define ERROR_SERVICE_DEPENDENCY_FAIL 1068
%define ERROR_SERVICE_LOGON_FAILED 1069
%define ERROR_SERVICE_START_HANG 1070
%define ERROR_INVALID_SERVICE_LOCK 1071
%define ERROR_SERVICE_MARKED_FOR_DELETE 1072
%define ERROR_SERVICE_EXISTS 1073
%define ERROR_ALREADY_RUNNING_LKG 1074
%define ERROR_SERVICE_DEPENDENCY_DELETED 1075
%define ERROR_BOOT_ALREADY_ACCEPTED 1076
%define ERROR_SERVICE_NEVER_STARTED 1077
%define ERROR_DUPLICATE_SERVICE_NAME 1078
%define ERROR_END_OF_MEDIA 1100
%define ERROR_FILEMARK_DETECTED 1101
%define ERROR_BEGINNING_OF_MEDIA 1102
%define ERROR_SETMARK_DETECTED 1103
%define ERROR_NO_DATA_DETECTED 1104
%define ERROR_PARTITION_FAILURE 1105
%define ERROR_INVALID_BLOCK_LENGTH 1106
%define ERROR_DEVICE_NOT_PARTITIONED 1107
%define ERROR_UNABLE_TO_LOCK_MEDIA 1108
%define ERROR_UNABLE_TO_UNLOAD_MEDIA 1109
%define ERROR_MEDIA_CHANGED 1110
%define ERROR_BUS_RESET 1111
%define ERROR_NO_MEDIA_IN_DRIVE 1112
%define ERROR_NO_UNICODE_TRANSLATION 1113
%define ERROR_DLL_INIT_FAILED 1114
%define ERROR_SHUTDOWN_IN_PROGRESS 1115
%define ERROR_NO_SHUTDOWN_IN_PROGRESS 1116
%define ERROR_IO_DEVICE 1117
%define ERROR_SERIAL_NO_DEVICE 1118
%define ERROR_IRQ_BUSY 1119
%define ERROR_MORE_WRITES 1120
%define ERROR_COUNTER_TIMEOUT 1121
%define ERROR_FLOPPY_ID_MARK_NOT_FOUND 1122
%define ERROR_FLOPPY_WRONG_CYLINDER 1123
%define ERROR_FLOPPY_UNKNOWN_ERROR 1124
%define ERROR_FLOPPY_BAD_REGISTERS 1125
%define ERROR_DISK_RECALIBRATE_FAILED 1126
%define ERROR_DISK_OPERATION_FAILED 1127
%define ERROR_DISK_RESET_FAILED 1128
%define ERROR_EOM_OVERFLOW 1129
%define ERROR_NOT_ENOUGH_SERVER_MEMORY 1130
%define ERROR_POSSIBLE_DEADLOCK 1131
%define ERROR_MAPPED_ALIGNMENT 1132
%define ERROR_INVALID_PIXEL_FORMAT 2000
%define ERROR_BAD_DRIVER 2001
%define ERROR_INVALID_WINDOW_STYLE 2002
%define ERROR_METAFILE_NOT_SUPPORTED 2003
%define ERROR_TRANSFORM_NOT_SUPPORTED 2004
%define ERROR_CLIPPING_NOT_SUPPORTED 2005
%define ERROR_UNKNOWN_PRINT_MONITOR 3000
%define ERROR_PRINTER_DRIVER_IN_USE 3001
%define ERROR_SPOOL_FILE_NOT_FOUND 3002
%define ERROR_SPL_NO_STARTDOC 3003
%define ERROR_SPL_NO_ADDJOB 3004
%define ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED 3005
%define ERROR_PRINT_MONITOR_ALREADY_INSTALLED 3006
%define ERROR_WINS_INTERNAL 4000
%define ERROR_CAN_NOT_DEL_LOCAL_WINS 4001
%define ERROR_STATIC_INIT 4002
%define ERROR_INC_BACKUP 4003
%define ERROR_FULL_BACKUP 4004
%define ERROR_REC_NON_EXISTENT 4005
%define ERROR_RPL_NOT_ALLOWED 4006
%define SEVERITY_SUCCESS 0
%define SEVERITY_ERROR 1
%define FACILITY_NT_BIT 10000000h
%define NOERROR 0
%define E_UNEXPECTED 8000FFFFh
%define E_NOTIMPL 80004001h
%define E_OUTOFMEMORY 8007000Eh
%define E_INVALIDARG 80070057h
%define E_NOINTERFACE 80004002h
%define E_POINTER 80004003h
%define E_HANDLE 80070006h
%define E_ABORT 80004004h
%define E_FAIL 80004005h
%define E_ACCESSDENIED 80070005h
%define CO_E_INIT_TLS 80004006h
%define CO_E_INIT_SHARED_ALLOCATOR 80004007h
%define CO_E_INIT_MEMORY_ALLOCATOR 80004008h
%define CO_E_INIT_CLASS_CACHE 80004009h
%define CO_E_INIT_RPC_CHANNEL 8000400Ah
%define CO_E_INIT_TLS_SET_CHANNEL_CONTROL 8000400Bh
%define CO_E_INIT_TLS_CHANNEL_CONTROL 8000400Ch
%define CO_E_INIT_UNACCEPTED_USER_ALLOCATOR 8000400Dh
%define CO_E_INIT_SCM_MUTEX_EXISTS 8000400Eh
%define CO_E_INIT_SCM_FILE_MAPPING_EXISTS 8000400Fh
%define CO_E_INIT_SCM_MAP_VIEW_OF_FILE 80004010h
%define CO_E_INIT_SCM_EXEC_FAILURE 80004011h
%define CO_E_INIT_ONLY_SINGLE_THREADED 80004012h
%define S_OK 0h
%define S_FALSE 1h
%define OLE_E_FIRST 80040000h
%define OLE_E_LAST 800400FFh
%define OLE_S_FIRST 40000h
%define OLE_S_LAST 400FFh
%define OLE_E_OLEVERB 80040000h
%define OLE_E_ADVF 80040001h
%define OLE_E_ENUM_NOMORE 80040002h
%define OLE_E_ADVISENOTSUPPORTED 80040003h
%define OLE_E_NOCONNECTION 80040004h
%define OLE_E_NOTRUNNING 80040005h
%define OLE_E_NOCACHE 80040006h
%define OLE_E_BLANK 80040007h
%define OLE_E_CLASSDIFF 80040008h
%define OLE_E_CANT_GETMONIKER 80040009h
%define OLE_E_CANT_BINDTOSOURCE 8004000Ah
%define OLE_E_STATIC 8004000Bh
%define OLE_E_PROMPTSAVECANCELLED 8004000Ch
%define OLE_E_INVALIDRECT 8004000Dh
%define OLE_E_WRONGCOMPOBJ 8004000Eh
%define OLE_E_INVALIDHWND 8004000Fh
%define OLE_E_NOT_INPLACEACTIVE 80040010h
%define OLE_E_CANTCONVERT 80040011h
%define OLE_E_NOSTORAGE 80040012h
%define DV_E_FORMATETC 80040064h
%define DV_E_DVTARGETDEVICE 80040065h
%define DV_E_STGMEDIUM 80040066h
%define DV_E_STATDATA 80040067h
%define DV_E_LINDEX 80040068h
%define DV_E_TYMED 80040069h
%define DV_E_CLIPFORMAT 8004006Ah
%define DV_E_DVASPECT 8004006Bh
%define DV_E_DVTARGETDEVICE_SIZE 8004006Ch
%define DV_E_NOIVIEWOBJECT 8004006Dh
%define DRAGDROP_E_FIRST 80040100h
%define DRAGDROP_E_LAST 8004010Fh
%define DRAGDROP_S_FIRST 40100h
%define DRAGDROP_S_LAST 4010Fh
%define DRAGDROP_E_NOTREGISTERED 80040100h
%define DRAGDROP_E_ALREADYREGISTERED 80040101h
%define DRAGDROP_E_INVALIDHWND 80040102h
%define CLASSFACTORY_E_FIRST 80040110h
%define CLASSFACTORY_E_LAST 8004011Fh
%define CLASSFACTORY_S_FIRST 40110h
%define CLASSFACTORY_S_LAST 4011Fh
%define CLASS_E_NOAGGREGATION 80040110h
%define CLASS_E_CLASSNOTAVAILABLE 80040111h
%define MARSHAL_E_FIRST 80040120h
%define MARSHAL_E_LAST 8004012Fh
%define MARSHAL_S_FIRST 40120h
%define MARSHAL_S_LAST 4012Fh
%define DATA_E_FIRST 80040130h
%define DATA_E_LAST 8004013Fh
%define DATA_S_FIRST 40130h
%define DATA_S_LAST 4013Fh
%define VIEW_E_FIRST 80040140h
%define VIEW_E_LAST 8004014Fh
%define VIEW_S_FIRST 40140h
%define VIEW_S_LAST 4014Fh
%define VIEW_E_DRAW 80040140h
%define REGDB_E_FIRST 80040150h
%define REGDB_E_LAST 8004015Fh
%define REGDB_S_FIRST 40150h
%define REGDB_S_LAST 4015Fh
%define REGDB_E_READREGDB 80040150h
%define REGDB_E_WRITEREGDB 80040151h
%define REGDB_E_KEYMISSING 80040152h
%define REGDB_E_INVALIDVALUE 80040153h
%define REGDB_E_CLASSNOTREG 80040154h
%define REGDB_E_IIDNOTREG 80040155h
%define CACHE_E_FIRST 80040170h
%define CACHE_E_LAST 8004017Fh
%define CACHE_S_FIRST 40170h
%define CACHE_S_LAST 4017Fh
%define CACHE_E_NOCACHE_UPDATED 80040170h
%define OLEOBJ_E_FIRST 80040180h
%define OLEOBJ_E_LAST 8004018Fh
%define OLEOBJ_S_FIRST 40180h
%define OLEOBJ_S_LAST 4018Fh
%define OLEOBJ_E_NOVERBS 80040180h
%define OLEOBJ_E_INVALIDVERB 80040181h
%define CLIENTSITE_E_FIRST 80040190h
%define CLIENTSITE_E_LAST 8004019Fh
%define CLIENTSITE_S_FIRST 40190h
%define CLIENTSITE_S_LAST 4019Fh
%define INPLACE_E_NOTUNDOABLE 800401A0h
%define INPLACE_E_NOTOOLSPACE 800401A1h
%define INPLACE_E_FIRST 800401A0h
%define INPLACE_E_LAST 800401AFh
%define INPLACE_S_FIRST 401A0h
%define INPLACE_S_LAST 401AFh
%define ENUM_E_FIRST 800401B0h
%define ENUM_E_LAST 800401BFh
%define ENUM_S_FIRST 401B0h
%define ENUM_S_LAST 401BFh
%define CONVERT10_E_FIRST 800401C0h
%define CONVERT10_E_LAST 800401CFh
%define CONVERT10_S_FIRST 401C0h
%define CONVERT10_S_LAST 401CFh
%define CONVERT10_E_OLESTREAM_GET 800401C0h
%define CONVERT10_E_OLESTREAM_PUT 800401C1h
%define CONVERT10_E_OLESTREAM_FMT 800401C2h
%define CONVERT10_E_OLESTREAM_BITMAP_TO_DIB 800401C3h
%define CONVERT10_E_STG_FMT 800401C4h
%define CONVERT10_E_STG_NO_STD_STREAM 800401C5h
%define CONVERT10_E_STG_DIB_TO_BITMAP 800401C6h
%define CLIPBRD_E_FIRST 800401D0h
%define CLIPBRD_E_LAST 800401DFh
%define CLIPBRD_S_FIRST 401D0h
%define CLIPBRD_S_LAST 401DFh
%define CLIPBRD_E_CANT_OPEN 800401D0h
%define CLIPBRD_E_CANT_EMPTY 800401D1h
%define CLIPBRD_E_CANT_SET 800401D2h
%define CLIPBRD_E_BAD_DATA 800401D3h
%define CLIPBRD_E_CANT_CLOSE 800401D4h
%define MK_E_FIRST 800401E0h
%define MK_E_LAST 800401EFh
%define MK_S_FIRST 401E0h
%define MK_S_LAST 401EFh
%define MK_E_CONNECTMANUALLY 800401E0h
%define MK_E_EXCEEDEDDEADLINE 800401E1h
%define MK_E_NEEDGENERIC 800401E2h
%define MK_E_UNAVAILABLE 800401E3h
%define MK_E_SYNTAX 800401E4h
%define MK_E_NOOBJECT 800401E5h
%define MK_E_INVALIDEXTENSION 800401E6h
%define MK_E_INTERMEDIATEINTERFACENOTSUPPORTED 800401E7h
%define MK_E_NOTBINDABLE 800401E8h
%define MK_E_NOTBOUND 800401E9h
%define MK_E_CANTOPENFILE 800401EAh
%define MK_E_MUSTBOTHERUSER 800401EBh
%define MK_E_NOINVERSE 800401ECh
%define MK_E_NOSTORAGE 800401EDh
%define MK_E_NOPREFIX 800401EEh
%define MK_E_ENUMERATION_FAILED 800401EFh
%define CO_E_FIRST 800401F0h
%define CO_E_LAST 800401FFh
%define CO_S_FIRST 401F0h
%define CO_S_LAST 401FFh
%define CO_E_NOTINITIALIZED 800401F0h
%define CO_E_ALREADYINITIALIZED 800401F1h
%define CO_E_CANTDETERMINECLASS 800401F2h
%define CO_E_CLASSSTRING 800401F3h
%define CO_E_IIDSTRING 800401F4h
%define CO_E_APPNOTFOUND 800401F5h
%define CO_E_APPSINGLEUSE 800401F6h
%define CO_E_ERRORINAPP 800401F7h
%define CO_E_DLLNOTFOUND 800401F8h
%define CO_E_ERRORINDLL 800401F9h
%define CO_E_WRONGOSFORAPP 800401FAh
%define CO_E_OBJNOTREG 800401FBh
%define CO_E_OBJISREG 800401FCh
%define CO_E_OBJNOTCONNECTED 800401FDh
%define CO_E_APPDIDNTREG 800401FEh
%define CO_E_RELEASED 800401FFh
%define OLE_S_USEREG 40000h
%define OLE_S_STATIC 40001h
%define OLE_S_MAC_CLIPFORMAT 40002h
%define DRAGDROP_S_DROP 40100h
%define DRAGDROP_S_CANCEL 40101h
%define DRAGDROP_S_USEDEFAULTCURSORS 40102h
%define DATA_S_SAMEFORMATETC 40130h
%define VIEW_S_ALREADY_FROZEN 40140h
%define CACHE_S_FORMATETC_NOTSUPPORTED 40170h
%define CACHE_S_SAMECACHE 40171h
%define CACHE_S_SOMECACHES_NOTUPDATED 40172h
%define OLEOBJ_S_INVALIDVERB 40180h
%define OLEOBJ_S_CANNOT_DOVERB_NOW 40181h
%define OLEOBJ_S_INVALIDHWND 40182h
%define INPLACE_S_TRUNCATED 401A0h
%define CONVERT10_S_NO_PRESENTATION 401C0h
%define MK_S_REDUCED_TO_SELF 401E2h
%define MK_S_ME 401E4h
%define MK_S_HIM 401E5h
%define MK_S_US 401E6h
%define MK_S_MONIKERALREADYREGISTERED 401E7h
%define CO_E_CLASS_CREATE_FAILED 80080001h
%define CO_E_SCM_ERROR 80080002h
%define CO_E_SCM_RPC_FAILURE 80080003h
%define CO_E_BAD_PATH 80080004h
%define CO_E_SERVER_EXEC_FAILURE 80080005h
%define CO_E_OBJSRV_RPC_FAILURE 80080006h
%define MK_E_NO_NORMALIZED 80080007h
%define CO_E_SERVER_STOPPING 80080008h
%define MEM_E_INVALID_ROOT 80080009h
%define MEM_E_INVALID_LINK 80080010h
%define MEM_E_INVALID_SIZE 80080011h
%define DISP_E_UNKNOWNINTERFACE 80020001h
%define DISP_E_MEMBERNOTFOUND 80020003h
%define DISP_E_PARAMNOTFOUND 80020004h
%define DISP_E_TYPEMISMATCH 80020005h
%define DISP_E_UNKNOWNNAME 80020006h
%define DISP_E_NONAMEDARGS 80020007h
%define DISP_E_BADVARTYPE 80020008h
%define DISP_E_EXCEPTION 80020009h
%define DISP_E_OVERFLOW 8002000Ah
%define DISP_E_BADINDEX 8002000Bh
%define DISP_E_UNKNOWNLCID 8002000Ch
%define DISP_E_ARRAYISLOCKED 8002000Dh
%define DISP_E_BADPARAMCOUNT 8002000Eh
%define DISP_E_PARAMNOTOPTIONAL 8002000Fh
%define DISP_E_BADCALLEE 80020010h
%define DISP_E_NOTACOLLECTION 80020011h
%define TYPE_E_BUFFERTOOSMALL 80028016h
%define TYPE_E_INVDATAREAD 80028018h
%define TYPE_E_UNSUPFORMAT 80028019h
%define TYPE_E_REGISTRYACCESS 8002801Ch
%define TYPE_E_LIBNOTREGISTERED 8002801Dh
%define TYPE_E_UNDEFINEDTYPE 80028027h
%define TYPE_E_QUALIFIEDNAMEDISALLOWED 80028028h
%define TYPE_E_INVALIDSTATE 80028029h
%define TYPE_E_WRONGTYPEKIND 8002802Ah
%define TYPE_E_ELEMENTNOTFOUND 8002802Bh
%define TYPE_E_AMBIGUOUSNAME 8002802Ch
%define TYPE_E_NAMECONFLICT 8002802Dh
%define TYPE_E_UNKNOWNLCID 8002802Eh
%define TYPE_E_DLLFUNCTIONNOTFOUND 8002802Fh
%define TYPE_E_BADMODULEKIND 800288BDh
%define TYPE_E_SIZETOOBIG 800288C5h
%define TYPE_E_DUPLICATEID 800288C6h
%define TYPE_E_INVALIDID 800288CFh
%define TYPE_E_TYPEMISMATCH 80028CA0h
%define TYPE_E_OUTOFBOUNDS 80028CA1h
%define TYPE_E_IOERROR 80028CA2h
%define TYPE_E_CANTCREATETMPFILE 80028CA3h
%define TYPE_E_CANTLOADLIBRARY 80029C4Ah
%define TYPE_E_INCONSISTENTPROPFUNCS 80029C83h
%define TYPE_E_CIRCULARTYPE 80029C84h
%define STG_E_INVALIDFUNCTION 80030001h
%define STG_E_FILENOTFOUND 80030002h
%define STG_E_PATHNOTFOUND 80030003h
%define STG_E_TOOMANYOPENFILES 80030004h
%define STG_E_ACCESSDENIED 80030005h
%define STG_E_INVALIDHANDLE 80030006h
%define STG_E_INSUFFICIENTMEMORY 80030008h
%define STG_E_INVALIDPOINTER 80030009h
%define STG_E_NOMOREFILES 80030012h
%define STG_E_DISKISWRITEPROTECTED 80030013h
%define STG_E_SEEKERROR 80030019h
%define STG_E_WRITEFAULT 8003001Dh
%define STG_E_READFAULT 8003001Eh
%define STG_E_SHAREVIOLATION 80030020h
%define STG_E_LOCKVIOLATION 80030021h
%define STG_E_FILEALREADYEXISTS 80030050h
%define STG_E_INVALIDPARAMETER 80030057h
%define STG_E_MEDIUMFULL 80030070h
%define STG_E_ABNORMALAPIEXIT 800300FAh
%define STG_E_INVALIDHEADER 800300FBh
%define STG_E_INVALIDNAME 800300FCh
%define STG_E_UNKNOWN 800300FDh
%define STG_E_UNIMPLEMENTEDFUNCTION 800300FEh
%define STG_E_INVALIDFLAG 800300FFh
%define STG_E_INUSE 80030100h
%define STG_E_NOTCURRENT 80030101h
%define STG_E_REVERTED 80030102h
%define STG_E_CANTSAVE 80030103h
%define STG_E_OLDFORMAT 80030104h
%define STG_E_OLDDLL 80030105h
%define STG_E_SHAREREQUIRED 80030106h
%define STG_E_NOTFILEBASEDSTORAGE 80030107h
%define STG_E_EXTANTMARSHALLINGS 80030108h
%define STG_S_CONVERTED 30200h
%define RPC_E_CALL_REJECTED 80010001h
%define RPC_E_CALL_CANCELED 80010002h
%define RPC_E_CANTPOST_INSENDCALL 80010003h
%define RPC_E_CANTCALLOUT_INASYNCCALL 80010004h
%define RPC_E_CANTCALLOUT_INEXTERNALCALL 80010005h
%define RPC_E_CONNECTION_TERMINATED 80010006h
%define RPC_E_SERVER_DIED 80010007h
%define RPC_E_CLIENT_DIED 80010008h
%define RPC_E_INVALID_DATAPACKET 80010009h
%define RPC_E_CANTTRANSMIT_CALL 8001000Ah
%define RPC_E_CLIENT_CANTMARSHAL_DATA 8001000Bh
%define RPC_E_CLIENT_CANTUNMARSHAL_DATA 8001000Ch
%define RPC_E_SERVER_CANTMARSHAL_DATA 8001000Dh
%define RPC_E_SERVER_CANTUNMARSHAL_DATA 8001000Eh
%define RPC_E_INVALID_DATA 8001000Fh
%define RPC_E_INVALID_PARAMETER 80010010h
%define RPC_E_CANTCALLOUT_AGAIN 80010011h
%define RPC_E_SERVER_DIED_DNE 80010012h
%define RPC_E_SYS_CALL_FAILED 80010100h
%define RPC_E_OUT_OF_RESOURCES 80010101h
%define RPC_E_ATTEMPTED_MULTITHREAD 80010102h
%define RPC_E_NOT_REGISTERED 80010103h
%define RPC_E_FAULT 80010104h
%define RPC_E_SERVERFAULT 80010105h
%define RPC_E_CHANGED_MODE 80010106h
%define RPC_E_INVALIDMETHOD 80010107h
%define RPC_E_DISCONNECTED 80010108h
%define RPC_E_RETRY 80010109h
%define RPC_E_SERVERCALL_RETRYLATER 8001010Ah
%define RPC_E_SERVERCALL_REJECTED 8001010Bh
%define RPC_E_INVALID_CALLDATA 8001010Ch
%define RPC_E_CANTCALLOUT_ININPUTSYNCCALL 8001010Dh
%define RPC_E_WRONG_THREAD 8001010Eh
%define RPC_E_THREAD_NOT_INIT 8001010Fh
%define RPC_E_UNEXPECTED 8001FFFFh
%define ERROR_BAD_USERNAME 2202
%define ERROR_NOT_CONNECTED 2250
%define ERROR_OPEN_FILES 2401
%define ERROR_DEVICE_IN_USE 2404
%define ERROR_BAD_DEVICE 1200
%define ERROR_CONNECTION_UNAVAIL 1201
%define ERROR_DEVICE_ALREADY_REMEMBERED 1202
%define ERROR_NO_NET_OR_BAD_PATH 1203
%define ERROR_BAD_PROVIDER 1204
%define ERROR_CANNOT_OPEN_PROFILE 1205

%define ERROR_BAD_PROFILE 1206
%define ERROR_NOT_CONTAINER 1207
%define ERROR_EXTENDED_ERROR 1208
%define ERROR_INVALID_GROUPNAME 1209
%define ERROR_INVALID_COMPUTERNAME 1210
%define ERROR_INVALID_EVENTNAME 1211
%define ERROR_INVALID_DOMAINNAME 1212
%define ERROR_INVALID_SERVICENAME 1213
%define ERROR_INVALID_NETNAME 1214
%define ERROR_INVALID_SHARENAME 1215
%define ERROR_INVALID_PASSWORDNAME 1216
%define ERROR_INVALID_MESSAGENAME 1217
%define ERROR_INVALID_MESSAGEDEST 1218
%define ERROR_SESSION_CREDENTIAL_CONFLICT 1219
%define ERROR_REMOTE_SESSION_LIMIT_EXCEEDED 1220
%define ERROR_DUP_DOMAINNAME 1221
%define ERROR_NO_NETWORK 1222
%define ERROR_NOT_ALL_ASSIGNED 1300
%define ERROR_SOME_NOT_MAPPED 1301
%define ERROR_NO_QUOTAS_FOR_ACCOUNT 1302
%define ERROR_LOCAL_USER_SESSION_KEY 1303
%define ERROR_NULL_LM_PASSWORD 1304
%define ERROR_UNKNOWN_REVISION 1305
%define ERROR_REVISION_MISMATCH 1306
%define ERROR_INVALID_OWNER 1307
%define ERROR_INVALID_PRIMARY_GROUP 1308
%define ERROR_NO_IMPERSONATION_TOKEN 1309
%define ERROR_CANT_DISABLE_MANDATORY 1310
%define ERROR_NO_LOGON_SERVERS 1311
%define ERROR_NO_SUCH_LOGON_SESSION 1312
%define ERROR_NO_SUCH_PRIVILEGE 1313
%define ERROR_PRIVILEGE_NOT_HELD 1314
%define ERROR_INVALID_ACCOUNT_NAME 1315
%define ERROR_USER_EXISTS 1316
%define ERROR_NO_SUCH_USER 1317
%define ERROR_GROUP_EXISTS 1318
%define ERROR_NO_SUCH_GROUP 1319
%define ERROR_MEMBER_IN_GROUP 1320
%define ERROR_MEMBER_NOT_IN_GROUP 1321
%define ERROR_LAST_ADMIN 1322
%define ERROR_WRONG_PASSWORD 1323
%define ERROR_ILL_FORMED_PASSWORD 1324
%define ERROR_PASSWORD_RESTRICTION 1325
%define ERROR_LOGON_FAILURE 1326
%define ERROR_ACCOUNT_RESTRICTION 1327
%define ERROR_INVALID_LOGON_HOURS 1328
%define ERROR_INVALID_WORKSTATION 1329
%define ERROR_PASSWORD_EXPIRED 1330
%define ERROR_ACCOUNT_DISABLED 1331
%define ERROR_NONE_MAPPED 1332
%define ERROR_TOO_MANY_LUIDS_REQUESTED 1333
%define ERROR_LUIDS_EXHAUSTED 1334
%define ERROR_INVALID_SUB_AUTHORITY 1335
%define ERROR_INVALID_ACL 1336
%define ERROR_INVALID_SID 1337
%define ERROR_INVALID_SECURITY_DESCR 1338
%define ERROR_BAD_INHERITANCE_ACL 1340
%define ERROR_SERVER_DISABLED 1341
%define ERROR_SERVER_NOT_DISABLED 1342
%define ERROR_INVALID_ID_AUTHORITY 1343
%define ERROR_ALLOTTED_SPACE_EXCEEDED 1344
%define ERROR_INVALID_GROUP_ATTRIBUTES 1345
%define ERROR_BAD_IMPERSONATION_LEVEL 1346
%define ERROR_CANT_OPEN_ANONYMOUS 1347
%define ERROR_BAD_VALIDATION_CLASS 1348
%define ERROR_BAD_TOKEN_TYPE 1349
%define ERROR_NO_SECURITY_ON_OBJECT 1350
%define ERROR_CANT_ACCESS_DOMAIN_INFO 1351
%define ERROR_INVALID_SERVER_STATE 1352
%define ERROR_INVALID_DOMAIN_STATE 1353
%define ERROR_INVALID_DOMAIN_ROLE 1354
%define ERROR_NO_SUCH_DOMAIN 1355
%define ERROR_DOMAIN_EXISTS 1356
%define ERROR_DOMAIN_LIMIT_EXCEEDED 1357
%define ERROR_INTERNAL_DB_CORRUPTION 1358
%define ERROR_INTERNAL_ERROR 1359
%define ERROR_GENERIC_NOT_MAPPED 1360
%define ERROR_BAD_DESCRIPTOR_FORMAT 1361
%define ERROR_NOT_LOGON_PROCESS 1362
%define ERROR_LOGON_SESSION_EXISTS 1363
%define ERROR_NO_SUCH_PACKAGE 1364
%define ERROR_BAD_LOGON_SESSION_STATE 1365
%define ERROR_LOGON_SESSION_COLLISION 1366
%define ERROR_INVALID_LOGON_TYPE 1367
%define ERROR_CANNOT_IMPERSONATE 1368
%define ERROR_RXACT_INVALID_STATE 1369
%define ERROR_RXACT_COMMIT_FAILURE 1370
%define ERROR_SPECIAL_ACCOUNT 1371
%define ERROR_SPECIAL_GROUP 1372
%define ERROR_SPECIAL_USER 1373
%define ERROR_MEMBERS_PRIMARY_GROUP 1374
%define ERROR_TOKEN_ALREADY_IN_USE 1375
%define ERROR_NO_SUCH_ALIAS 1376
%define ERROR_MEMBER_NOT_IN_ALIAS 1377
%define ERROR_MEMBER_IN_ALIAS 1378
%define ERROR_ALIAS_EXISTS 1379
%define ERROR_LOGON_NOT_GRANTED 1380
%define ERROR_TOO_MANY_SECRETS 1381
%define ERROR_SECRET_TOO_LONG 1382
%define ERROR_INTERNAL_DB_ERROR 1383
%define ERROR_TOO_MANY_CONTEXT_IDS 1384
%define ERROR_LOGON_TYPE_NOT_GRANTED 1385
%define ERROR_NT_CROSS_ENCRYPTION_REQUIRED 1386
%define ERROR_NO_SUCH_MEMBER 1387
%define ERROR_INVALID_MEMBER 1388
%define ERROR_TOO_MANY_SIDS 1389
%define ERROR_LM_CROSS_ENCRYPTION_REQUIRED 1390
%define ERROR_NO_INHERITANCE 1391
%define ERROR_FILE_CORRUPT 1392
%define ERROR_DISK_CORRUPT 1393
%define ERROR_NO_USER_SESSION_KEY 1394
%define ERROR_INVALID_WINDOW_HANDLE 1400
%define ERROR_INVALID_MENU_HANDLE 1401
%define ERROR_INVALID_CURSOR_HANDLE 1402
%define ERROR_INVALID_ACCEL_HANDLE 1403
%define ERROR_INVALID_HOOK_HANDLE 1404
%define ERROR_INVALID_DWP_HANDLE 1405
%define ERROR_TLW_WITH_WSCHILD 1406
%define ERROR_CANNOT_FIND_WND_CLASS 1407
%define ERROR_WINDOW_OF_OTHER_THREAD 1408
%define ERROR_HOTKEY_ALREADY_REGISTERED 1409
%define ERROR_CLASS_ALREADY_EXISTS 1410
%define ERROR_CLASS_DOES_NOT_EXIST 1411
%define ERROR_CLASS_HAS_WINDOWS 1412
%define ERROR_INVALID_INDEX 1413
%define ERROR_INVALID_ICON_HANDLE 1414
%define ERROR_PRIVATE_DIALOG_INDEX 1415
%define ERROR_LISTBOX_ID_NOT_FOUND 1416
%define ERROR_NO_WILDCARD_CHARACTERS 1417
%define ERROR_CLIPBOARD_NOT_OPEN 1418
%define ERROR_HOTKEY_NOT_REGISTERED 1419
%define ERROR_WINDOW_NOT_DIALOG 1420
%define ERROR_CONTROL_ID_NOT_FOUND 1421
%define ERROR_INVALID_COMBOBOX_MESSAGE 1422
%define ERROR_WINDOW_NOT_COMBOBOX 1423
%define ERROR_INVALID_EDIT_HEIGHT 1424
%define ERROR_DC_NOT_FOUND 1425
%define ERROR_INVALID_HOOK_FILTER 1426
%define ERROR_INVALID_FILTER_PROC 1427
%define ERROR_HOOK_NEEDS_HMOD 1428
%define ERROR_PUBLIC_ONLY_HOOK 1429
%define ERROR_JOURNAL_HOOK_SET 1430
%define ERROR_HOOK_NOT_INSTALLED 1431
%define ERROR_INVALID_LB_MESSAGE 1432
%define ERROR_SETCOUNT_ON_BAD_LB 1433
%define ERROR_LB_WITHOUT_TABSTOPS 1434
%define ERROR_DESTROY_OBJECT_OF_OTHER_THREAD 1435
%define ERROR_CHILD_WINDOW_MENU 1436
%define ERROR_NO_SYSTEM_MENU 1437
%define ERROR_INVALID_MSGBOX_STYLE 1438
%define ERROR_INVALID_SPI_VALUE 1439
%define ERROR_SCREEN_ALREADY_LOCKED 1440
%define ERROR_HWNDS_HAVE_DIFF_PARENT 1441
%define ERROR_NOT_CHILD_WINDOW 1442
%define ERROR_INVALID_GW_COMMAND 1443
%define ERROR_INVALID_THREAD_ID 1444
%define ERROR_NON_MDICHILD_WINDOW 1445
%define ERROR_POPUP_ALREADY_ACTIVE 1446
%define ERROR_NO_SCROLLBARS 1447
%define ERROR_INVALID_SCROLLBAR_RANGE 1448
%define ERROR_INVALID_SHOWWIN_COMMAND 1449
%define ERROR_EVENTLOG_FILE_CORRUPT 1500
%define ERROR_EVENTLOG_CANT_START 1501
%define ERROR_LOG_FILE_FULL 1502
%define ERROR_EVENTLOG_FILE_CHANGED 1503
%define RPC_S_INVALID_STRING_BINDING 1700
%define RPC_S_WRONG_KIND_OF_BINDING 1701
%define RPC_S_INVALID_BINDING 1702
%define RPC_S_PROTSEQ_NOT_SUPPORTED 1703
%define RPC_S_INVALID_RPC_PROTSEQ 1704
%define RPC_S_INVALID_STRING_UUID 1705
%define RPC_S_INVALID_ENDPOINT_FORMAT 1706
%define RPC_S_INVALID_NET_ADDR 1707
%define RPC_S_NO_ENDPOINT_FOUND 1708
%define RPC_S_INVALID_TIMEOUT 1709
%define RPC_S_OBJECT_NOT_FOUND 1710
%define RPC_S_ALREADY_REGISTERED 1711
%define RPC_S_TYPE_ALREADY_REGISTERED 1712
%define RPC_S_ALREADY_LISTENING 1713
%define RPC_S_NO_PROTSEQS_REGISTERED 1714
%define RPC_S_NOT_LISTENING 1715
%define RPC_S_UNKNOWN_MGR_TYPE 1716
%define RPC_S_UNKNOWN_IF 1717
%define RPC_S_NO_BINDINGS 1718
%define RPC_S_NO_PROTSEQS 1719
%define RPC_S_CANT_CREATE_ENDPOINT 1720
%define RPC_S_OUT_OF_RESOURCES 1721
%define RPC_S_SERVER_UNAVAILABLE 1722
%define RPC_S_SERVER_TOO_BUSY 1723
%define RPC_S_INVALID_NETWORK_OPTIONS 1724
%define RPC_S_NO_CALL_ACTIVE 1725
%define RPC_S_CALL_FAILED 1726
%define RPC_S_CALL_FAILED_DNE 1727
%define RPC_S_PROTOCOL_ERROR 1728
%define RPC_S_UNSUPPORTED_TRANS_SYN 1730
%define RPC_S_UNSUPPORTED_TYPE 1732
%define RPC_S_INVALID_TAG 1733
%define RPC_S_INVALID_BOUND 1734
%define RPC_S_NO_ENTRY_NAME 1735
%define RPC_S_INVALID_NAME_SYNTAX 1736
%define RPC_S_UNSUPPORTED_NAME_SYNTAX 1737
%define RPC_S_UUID_NO_ADDRESS 1739
%define RPC_S_DUPLICATE_ENDPOINT 1740
%define RPC_S_UNKNOWN_AUTHN_TYPE 1741
%define RPC_S_MAX_CALLS_TOO_SMALL 1742
%define RPC_S_STRING_TOO_LONG 1743
%define RPC_S_PROTSEQ_NOT_FOUND 1744
%define RPC_S_PROCNUM_OUT_OF_RANGE 1745
%define RPC_S_BINDING_HAS_NO_AUTH 1746
%define RPC_S_UNKNOWN_AUTHN_SERVICE 1747
%define RPC_S_UNKNOWN_AUTHN_LEVEL 1748
%define RPC_S_INVALID_AUTH_IDENTITY 1749
%define RPC_S_UNKNOWN_AUTHZ_SERVICE 1750
%define EPT_S_INVALID_ENTRY 1751
%define EPT_S_CANT_PERFORM_OP 1752
%define EPT_S_NOT_REGISTERED 1753
%define RPC_S_NOTHING_TO_EXPORT 1754
%define RPC_S_INCOMPLETE_NAME 1755
%define RPC_S_INVALID_VERS_OPTION 1756
%define RPC_S_NO_MORE_MEMBERS 1757
%define RPC_S_NOT_ALL_OBJS_UNEXPORTED 1758
%define RPC_S_INTERFACE_NOT_FOUND 1759
%define RPC_S_ENTRY_ALREADY_EXISTS 1760
%define RPC_S_ENTRY_NOT_FOUND 1761
%define RPC_S_NAME_SERVICE_UNAVAILABLE 1762
%define RPC_S_INVALID_NAF_ID 1763
%define RPC_S_CANNOT_SUPPORT 1764
%define RPC_S_NO_CONTEXT_AVAILABLE 1765
%define RPC_S_INTERNAL_ERROR 1766
%define RPC_S_ZERO_DIVIDE 1767
%define RPC_S_ADDRESS_ERROR 1768
%define RPC_S_FP_DIV_ZERO 1769
%define RPC_S_FP_UNDERFLOW 1770
%define RPC_S_FP_OVERFLOW 1771
%define RPC_X_NO_MORE_ENTRIES 1772
%define RPC_X_SS_CHAR_TRANS_OPEN_FAIL 1773
%define RPC_X_SS_CHAR_TRANS_SHORT_FILE 1774
%define RPC_X_SS_IN_NULL_CONTEXT 1775
%define RPC_X_SS_CONTEXT_DAMAGED 1777
%define RPC_X_SS_HANDLES_MISMATCH 1778
%define RPC_X_SS_CANNOT_GET_CALL_HANDLE 1779
%define RPC_X_NULL_REF_POINTER 1780
%define RPC_X_ENUM_VALUE_OUT_OF_RANGE 1781
%define RPC_X_BYTE_COUNT_TOO_SMALL 1782
%define RPC_X_BAD_STUB_DATA 1783
%define ERROR_INVALID_USER_BUFFER 1784
%define ERROR_UNRECOGNIZED_MEDIA 1785
%define ERROR_NO_TRUST_LSA_SECRET 1786
%define ERROR_NO_TRUST_SAM_ACCOUNT 1787
%define ERROR_TRUSTED_DOMAIN_FAILURE 1788
%define ERROR_TRUSTED_RELATIONSHIP_FAILURE 1789
%define ERROR_TRUST_FAILURE 1790
%define RPC_S_CALL_IN_PROGRESS 1791
%define ERROR_NETLOGON_NOT_STARTED 1792
%define ERROR_ACCOUNT_EXPIRED 1793
%define ERROR_REDIRECTOR_HAS_OPEN_HANDLES 1794
%define ERROR_PRINTER_DRIVER_ALREADY_INSTALLED 1795
%define ERROR_UNKNOWN_PORT 1796
%define ERROR_UNKNOWN_PRINTER_DRIVER 1797
%define ERROR_UNKNOWN_PRINTPROCESSOR 1798
%define ERROR_INVALID_SEPARATOR_FILE 1799
%define ERROR_INVALID_PRIORITY 1800
%define ERROR_INVALID_PRINTER_NAME 1801
%define ERROR_PRINTER_ALREADY_EXISTS 1802
%define ERROR_INVALID_PRINTER_COMMAND 1803
%define ERROR_INVALID_DATATYPE 1804
%define ERROR_INVALID_ENVIRONMENT 1805
%define RPC_S_NO_MORE_BINDINGS 1806
%define ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT 1807
%define ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT 1808
%define ERROR_NOLOGON_SERVER_TRUST_ACCOUNT 1809
%define ERROR_DOMAIN_TRUST_INCONSISTENT 1810
%define ERROR_SERVER_HAS_OPEN_HANDLES 1811
%define ERROR_RESOURCE_DATA_NOT_FOUND 1812
%define ERROR_RESOURCE_TYPE_NOT_FOUND 1813
%define ERROR_RESOURCE_NAME_NOT_FOUND 1814
%define ERROR_RESOURCE_LANG_NOT_FOUND 1815
%define ERROR_NOT_ENOUGH_QUOTA 1816
%define RPC_S_GROUP_MEMBER_NOT_FOUND 1898
%define EPT_S_CANT_CREATE 1899
%define RPC_S_INVALID_OBJECT 1900
%define ERROR_INVALID_TIME 1901
%define ERROR_INVALID_FORM_NAME 1902
%define ERROR_INVALID_FORM_SIZE 1903
%define ERROR_ALREADY_WAITING 1904
%define ERROR_PRINTER_DELETED 1905
%define ERROR_INVALID_PRINTER_STATE 1906
%define ERROR_NO_BROWSER_SERVERS_FOUND 6118
%define MAXPNAMELEN 32
%define MAXERRORLENGTH 128
%define TIME_MS 1h
%define TIME_SAMPLES 2h
%define TIME_BYTES 4h
%define TIME_SMPTE 8h
%define TIME_MIDI 10h
%define MM_JOY1MOVE 3A0h
%define MM_JOY2MOVE 3A1h
%define MM_JOY1ZMOVE 3A2h
%define MM_JOY2ZMOVE 3A3h
%define MM_JOY1BUTTONDOWN 3B5h
%define MM_JOY2BUTTONDOWN 3B6h
%define MM_JOY1BUTTONUP 3B7h
%define MM_JOY2BUTTONUP 3B8h
%define MM_MCINOTIFY 3B9h
%define MM_MCISYSTEM_STRING 3CAh
%define MM_WOM_OPEN 3BBh
%define MM_WOM_CLOSE 3BCh
%define MM_WOM_DONE 3BDh
%define MM_WIM_OPEN 3BEh
%define MM_WIM_CLOSE 3BFh
%define MM_WIM_DATA 3C0h
%define MM_MIM_OPEN 3C1h
%define MM_MIM_CLOSE 3C2h
%define MM_MIM_DATA 3C3h
%define MM_MIM_LONGDATA 3C4h
%define MM_MIM_ERROR 3C5h
%define MM_MIM_LONGERROR 3C6h
%define MM_MOM_OPEN 3C7h
%define MM_MOM_CLOSE 3C8h
%define MM_MOM_DONE 3C9h
%define MMSYSERR_BASE 0
%define WAVERR_BASE 32
%define MIDIERR_BASE 64
%define TIMERR_BASE 96
%define JOYERR_BASE 160
%define MCIERR_BASE 256
%define MCI_STRING_OFFSET 512
%define MCI_VD_OFFSET 1024
%define MCI_CD_OFFSET 1088
%define MCI_WAVE_OFFSET 1152MCI_OPEN                    equ 0803h
MCI_CLOSE                   equ 0804h
MCI_PLAY                    equ 0806h
MCI_SEEK                    equ 0807h
MCI_STOP                    equ 0808h
MCI_PAUSE                   equ 0809h
MCI_STEP                    equ 080Eh
MCI_RECORD                  equ 080Fh
MCI_SAVE                    equ 0813h
MCI_CUT                     equ 0851h
MCI_COPY                    equ 0852h
MCI_PASTE                   equ 0853h
MCI_RESUME                  equ 0855h
MCI_DELETE                  equ 0856h
MCI_MODE_NOT_READY          equ 524
MCI_MODE_STOP               equ 525
MCI_MODE_PLAY               equ 526
MCI_MODE_RECORD             equ 527
MCI_MODE_SEEK               equ 528
MCI_MODE_PAUSE              equ 529
MCI_MODE_OPEN               equ 530
%define MCI_SEQ_OFFSET 1216

%define MMSYSERR_NOERROR 0
%define MMSYSERR_ERROR MMSYSERR_BASE+1
%define MMSYSERR_BADDEVICEID MMSYSERR_BASE+2
%define MMSYSERR_NOTENABLED MMSYSERR_BASE+3
%define MMSYSERR_ALLOCATED MMSYSERR_BASE+4
%define MMSYSERR_INVALHANDLE MMSYSERR_BASE+5
%define MMSYSERR_NODRIVER MMSYSERR_BASE+6
%define MMSYSERR_NOMEM MMSYSERR_BASE+7
%define MMSYSERR_NOTSUPPORTED MMSYSERR_BASE+8
%define MMSYSERR_BADERRNUM MMSYSERR_BASE+9
%define MMSYSERR_INVALFLAG MMSYSERR_BASE+10
%define MMSYSERR_INVALPARAM MMSYSERR_BASE+11
%define MMSYSERR_HANDLEBUSY MMSYSERR_BASE+12
%define MMSYSERR_INVALIDALIAS MMSYSERR_BASE+13
%define MMSYSERR_LASTERROR MMSYSERR_BASE+13
%define MM_MOM_POSITIONCB 3CAh
%define MM_MCISIGNAL 3CBh
%define MM_MIM_MOREDATA 3CCh
%define MIDICAPS_STREAM 8h
%define MEVT_F_SHORT 0h
%define MEVT_F_LONG 80000000h
%define MEVT_F_CALLBACK 40000000h
%define MIDISTRM_ERROR -2
%define MIDIPROP_SET 80000000h
%define MIDIPROP_GET 40000000h
%define MIDIPROP_TIMEDIV 1h
%define MIDIPROP_TEMPO 2h
%define MIXER_SHORT_NAME_CHARS 16
%define MIXER_LONG_NAME_CHARS 64
%define MIXERR_BASE 1024
%define MIXERR_INVALLINE MIXERR_BASE+0
%define MIXERR_INVALCONTROL MIXERR_BASE+1
%define MIXERR_INVALVALUE MIXERR_BASE+2
%define MIXERR_LASTERROR MIXERR_BASE+2
%define MIXER_OBJECTF_HANDLE 80000000h
%define MIXER_OBJECTF_MIXER 0h
%define MIXER_OBJECTF_HMIXER MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIXER
%define MIXER_OBJECTF_WAVEOUT 10000000h
%define MIXER_OBJECTF_HWAVEOUT MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_WAVEOUT
%define MIXER_OBJECTF_WAVEIN 20000000h
%define MIXER_OBJECTF_HWAVEIN MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_WAVEIN
%define MIXER_OBJECTF_MIDIOUT 30000000h
%define MIXER_OBJECTF_HMIDIOUT MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIDIOUT
%define MIXER_OBJECTF_MIDIIN 40000000h
%define MIXER_OBJECTF_HMIDIIN MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIDIIN
%define MIXER_OBJECTF_AUX 50000000h
%define MIXERLINE_LINEF_ACTIVE 1h
%define MIXERLINE_LINEF_DISCONNECTED 8000h
%define MIXERLINE_LINEF_SOURCE 80000000h
%define MIXERLINE_COMPONENTTYPE_DST_FIRST 0h
%define MIXERLINE_COMPONENTTYPE_DST_UNDEFINED MIXERLINE_COMPONENTTYPE_DST_FIRST+0
%define MIXERLINE_COMPONENTTYPE_DST_DIGITAL MIXERLINE_COMPONENTTYPE_DST_FIRST+1
%define MIXERLINE_COMPONENTTYPE_DST_LINE MIXERLINE_COMPONENTTYPE_DST_FIRST+2
%define MIXERLINE_COMPONENTTYPE_DST_MONITOR MIXERLINE_COMPONENTTYPE_DST_FIRST+3
%define MIXERLINE_COMPONENTTYPE_DST_SPEAKERS MIXERLINE_COMPONENTTYPE_DST_FIRST+4
%define MIXERLINE_COMPONENTTYPE_DST_HEADPHONES MIXERLINE_COMPONENTTYPE_DST_FIRST+5
%define MIXERLINE_COMPONENTTYPE_DST_TELEPHONE MIXERLINE_COMPONENTTYPE_DST_FIRST+6
%define MIXERLINE_COMPONENTTYPE_DST_WAVEIN MIXERLINE_COMPONENTTYPE_DST_FIRST+7
%define MIXERLINE_COMPONENTTYPE_DST_VOICEIN MIXERLINE_COMPONENTTYPE_DST_FIRST+8
%define MIXERLINE_COMPONENTTYPE_DST_LAST MIXERLINE_COMPONENTTYPE_DST_FIRST+8
%define MIXERLINE_COMPONENTTYPE_SRC_FIRST 1000h
%define MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED MIXERLINE_COMPONENTTYPE_SRC_FIRST+0
%define MIXERLINE_COMPONENTTYPE_SRC_DIGITAL MIXERLINE_COMPONENTTYPE_SRC_FIRST+1
%define MIXERLINE_COMPONENTTYPE_SRC_LINE MIXERLINE_COMPONENTTYPE_SRC_FIRST+2
%define MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE MIXERLINE_COMPONENTTYPE_SRC_FIRST+3
%define MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER MIXERLINE_COMPONENTTYPE_SRC_FIRST+4
%define MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC MIXERLINE_COMPONENTTYPE_SRC_FIRST+5
%define MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE MIXERLINE_COMPONENTTYPE_SRC_FIRST+6
%define MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER MIXERLINE_COMPONENTTYPE_SRC_FIRST+7
%define MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT MIXERLINE_COMPONENTTYPE_SRC_FIRST+8
%define MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY MIXERLINE_COMPONENTTYPE_SRC_FIRST+9
%define MIXERLINE_COMPONENTTYPE_SRC_ANALOG MIXERLINE_COMPONENTTYPE_SRC_FIRST+10
%define MIXERLINE_COMPONENTTYPE_SRC_LAST MIXERLINE_COMPONENTTYPE_SRC_FIRST+10
%define MIXERLINE_TARGETTYPE_UNDEFINED 0
%define MIXERLINE_TARGETTYPE_WAVEOUT 1
%define MIXERLINE_TARGETTYPE_WAVEIN 2
%define MIXERLINE_TARGETTYPE_MIDIOUT 3
%define MIXERLINE_TARGETTYPE_MIDIIN 4
%define MIXERLINE_TARGETTYPE_AUX 5
%define MIXER_GETLINEINFOF_DESTINATION 0h
%define MIXER_GETLINEINFOF_SOURCE 1h
%define MIXER_GETLINEINFOF_LINEID 2h
%define MIXER_GETLINEINFOF_COMPONENTTYPE 3h
%define MIXER_GETLINEINFOF_TARGETTYPE 4h
%define MIXER_GETLINEINFOF_QUERYMASK 0Fh
%define MIXERCONTROL_CONTROLF_UNIFORM 1h
%define MIXERCONTROL_CONTROLF_MULTIPLE 2h
%define MIXERCONTROL_CONTROLF_DISABLED 80000000h
%define MIXERCONTROL_CT_CLASS_MASK 0F0000000h
%define MIXERCONTROL_CT_CLASS_CUSTOM 0h
%define MIXERCONTROL_CT_CLASS_METER 10000000h
%define MIXERCONTROL_CT_CLASS_SWITCH 20000000h
%define MIXERCONTROL_CT_CLASS_NUMBER 30000000h
%define MIXERCONTROL_CT_CLASS_SLIDER 40000000h
%define MIXERCONTROL_CT_CLASS_FADER 50000000h
%define MIXERCONTROL_CT_CLASS_TIME 60000000h
%define MIXERCONTROL_CT_CLASS_LIST 70000000h
%define MIXERCONTROL_CT_SUBCLASS_MASK 0F000000h
%define MIXERCONTROL_CT_SC_SWITCH_BOOLEAN 0h
%define MIXERCONTROL_CT_SC_SWITCH_BUTTON 1000000h
%define MIXERCONTROL_CT_SC_METER_POLLED 0h
%define MIXERCONTROL_CT_SC_TIME_MICROSECS 0h
%define MIXERCONTROL_CT_SC_TIME_MILLISECS 1000000h
%define MIXERCONTROL_CT_SC_LIST_SINGLE 0h
%define MIXERCONTROL_CT_SC_LIST_MULTIPLE 1000000h
%define MIXERCONTROL_CT_UNITS_MASK 0FF0000h
%define MIXERCONTROL_CT_UNITS_CUSTOM 0h
%define MIXERCONTROL_CT_UNITS_BOOLEAN 10000h
%define MIXERCONTROL_CT_UNITS_SIGNED 20000h
%define MIXERCONTROL_CT_UNITS_UNSIGNED 30000h
%define MIXERCONTROL_CT_UNITS_DECIBELS 40000h
%define MIXERCONTROL_CT_UNITS_PERCENT 50000h
%define MIXERCONTROL_CONTROLTYPE_CUSTOM MIXERCONTROL_CT_CLASS_CUSTOM|MIXERCONTROL_CT_UNITS_CUSTOM
%define MIXERCONTROL_CONTROLTYPE_BOOLEANMETER MIXERCONTROL_CT_CLASS_METER|MIXERCONTROL_CT_SC_METER_POLLED|MIXERCONTROL_CT_UNITS_BOOLEAN
%define MIXERCONTROL_CONTROLTYPE_SIGNEDMETER MIXERCONTROL_CT_CLASS_METER|MIXERCONTROL_CT_SC_METER_POLLED|MIXERCONTROL_CT_UNITS_SIGNED
%define MIXERCONTROL_CONTROLTYPE_PEAKMETER MIXERCONTROL_CONTROLTYPE_SIGNEDMETER+1
%define MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER MIXERCONTROL_CT_CLASS_METER|MIXERCONTROL_CT_SC_METER_POLLED|MIXERCONTROL_CT_UNITS_UNSIGNED
%define MIXERCONTROL_CONTROLTYPE_BOOLEAN MIXERCONTROL_CT_CLASS_SWITCH|MIXERCONTROL_CT_SC_SWITCH_BOOLEAN|MIXERCONTROL_CT_UNITS_BOOLEAN
%define MIXERCONTROL_CONTROLTYPE_ONOFF MIXERCONTROL_CONTROLTYPE_BOOLEAN+1
%define MIXERCONTROL_CONTROLTYPE_MUTE MIXERCONTROL_CONTROLTYPE_BOOLEAN+2
%define MIXERCONTROL_CONTROLTYPE_MONO MIXERCONTROL_CONTROLTYPE_BOOLEAN+3
%define MIXERCONTROL_CONTROLTYPE_LOUDNESS MIXERCONTROL_CONTROLTYPE_BOOLEAN+4
%define MIXERCONTROL_CONTROLTYPE_STEREOENH MIXERCONTROL_CONTROLTYPE_BOOLEAN+5
%define MIXERCONTROL_CONTROLTYPE_BUTTON MIXERCONTROL_CT_CLASS_SWITCH|MIXERCONTROL_CT_SC_SWITCH_BUTTON|MIXERCONTROL_CT_UNITS_BOOLEAN
%define MIXERCONTROL_CONTROLTYPE_DECIBELS MIXERCONTROL_CT_CLASS_NUMBER|MIXERCONTROL_CT_UNITS_DECIBELS
%define MIXERCONTROL_CONTROLTYPE_SIGNED MIXERCONTROL_CT_CLASS_NUMBER|MIXERCONTROL_CT_UNITS_SIGNED
%define MIXERCONTROL_CONTROLTYPE_UNSIGNED MIXERCONTROL_CT_CLASS_NUMBER|MIXERCONTROL_CT_UNITS_UNSIGNED
%define MIXERCONTROL_CONTROLTYPE_PERCENT MIXERCONTROL_CT_CLASS_NUMBER|MIXERCONTROL_CT_UNITS_PERCENT
%define MIXERCONTROL_CONTROLTYPE_SLIDER MIXERCONTROL_CT_CLASS_SLIDER|MIXERCONTROL_CT_UNITS_SIGNED
%define MIXERCONTROL_CONTROLTYPE_PAN MIXERCONTROL_CONTROLTYPE_SLIDER+1
%define MIXERCONTROL_CONTROLTYPE_QSOUNDPAN MIXERCONTROL_CONTROLTYPE_SLIDER+2
%define MIXERCONTROL_CONTROLTYPE_FADER MIXERCONTROL_CT_CLASS_FADER|MIXERCONTROL_CT_UNITS_UNSIGNED
%define MIXERCONTROL_CONTROLTYPE_VOLUME MIXERCONTROL_CONTROLTYPE_FADER+1
%define MIXERCONTROL_CONTROLTYPE_BASS MIXERCONTROL_CONTROLTYPE_FADER+2
%define MIXERCONTROL_CONTROLTYPE_TREBLE MIXERCONTROL_CONTROLTYPE_FADER+3
%define MIXERCONTROL_CONTROLTYPE_EQUALIZER MIXERCONTROL_CONTROLTYPE_FADER+4
%define MIXERCONTROL_CONTROLTYPE_SINGLESELECT MIXERCONTROL_CT_CLASS_LIST|MIXERCONTROL_CT_SC_LIST_SINGLE|MIXERCONTROL_CT_UNITS_BOOLEAN
%define MIXERCONTROL_CONTROLTYPE_MUX MIXERCONTROL_CONTROLTYPE_SINGLESELECT+1
%define MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT MIXERCONTROL_CT_CLASS_LIST|MIXERCONTROL_CT_SC_LIST_MULTIPLE|MIXERCONTROL_CT_UNITS_BOOLEAN
%define MIXERCONTROL_CONTROLTYPE_MIXER MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT+1
%define MIXERCONTROL_CONTROLTYPE_MICROTIME MIXERCONTROL_CT_CLASS_TIME|MIXERCONTROL_CT_SC_TIME_MICROSECS|MIXERCONTROL_CT_UNITS_UNSIGNED
%define MIXERCONTROL_CONTROLTYPE_MILLITIME MIXERCONTROL_CT_CLASS_TIME|MIXERCONTROL_CT_SC_TIME_MILLISECS|MIXERCONTROL_CT_UNITS_UNSIGNED
%define MIXER_GETLINECONTROLSF_ALL 0h
%define MIXER_GETLINECONTROLSF_ONEBYID 1h
%define MIXER_GETLINECONTROLSF_ONEBYTYPE 2h
%define MIXER_GETLINECONTROLSF_QUERYMASK 0Fh
%define MIXER_GETCONTROLDETAILSF_VALUE 0h
%define MIXER_GETCONTROLDETAILSF_LISTTEXT 1h
%define MIXER_GETCONTROLDETAILSF_QUERYMASK 0Fh
%define MIXER_SETCONTROLDETAILSF_VALUE 0h
%define MIXER_SETCONTROLDETAILSF_CUSTOM 1h
%define MIXER_SETCONTROLDETAILSF_QUERYMASK 0Fh
%define JOY_BUTTON5 10h
%define JOY_BUTTON6 20h
%define JOY_BUTTON7 40h
%define JOY_BUTTON8 80h
%define JOY_BUTTON9 100h
%define JOY_BUTTON10 200h
%define JOY_BUTTON11 400h
%define JOY_BUTTON12 800h
%define JOY_BUTTON13 1000h
%define JOY_BUTTON14 2000h
%define JOY_BUTTON15 4000h
%define JOY_BUTTON16 8000h
%define JOY_BUTTON17 10000h
%define JOY_BUTTON18 20000h
%define JOY_BUTTON19 40000h
%define JOY_BUTTON20 80000h
%define JOY_BUTTON21 100000h
%define JOY_BUTTON22 200000h
%define JOY_BUTTON23 400000h
%define JOY_BUTTON24 800000h
%define JOY_BUTTON25 1000000h
%define JOY_BUTTON26 2000000h
%define JOY_BUTTON27 4000000h
%define JOY_BUTTON28 8000000h
%define JOY_BUTTON29 10000000h
%define JOY_BUTTON30 20000000h
%define JOY_BUTTON31 40000000h
%define JOY_BUTTON32 80000000h
%define JOY_POVCENTERED -1
%define JOY_POVFORWARD 0
%define JOY_POVRIGHT 9000
%define JOY_POVBACKWARD 18000
%define JOY_POVLEFT 27000
%define JOY_RETURNX 1h
%define JOY_RETURNY 2h
%define JOY_RETURNZ 4h
%define JOY_RETURNR 8h
%define JOY_RETURNU 10h
%define JOY_RETURNV 20h
%define JOY_RETURNPOV 40h
%define JOY_RETURNBUTTONS 80h
%define JOY_RETURNRAWDATA 100h
%define JOY_RETURNPOVCTS 200h
%define JOY_RETURNCENTERED 400h
%define JOY_USEDEADZONE 800h
%define JOY_RETURNALL JOY_RETURNX|JOY_RETURNY|JOY_RETURNZ|JOY_RETURNR|JOY_RETURNU|JOY_RETURNV|JOY_RETURNPOV|JOY_RETURNBUTTONS
%define JOY_CAL_READALWAYS 10000h
%define JOY_CAL_READXYONLY 20000h
%define JOY_CAL_READ3 40000h
%define JOY_CAL_READ4 80000h
%define JOY_CAL_READXONLY 100000h
%define JOY_CAL_READYONLY 200000h
%define JOY_CAL_READ5 400000h
%define JOY_CAL_READ6 800000h
%define JOY_CAL_READZONLY 1000000h
%define JOY_CAL_READRONLY 2000000h
%define JOY_CAL_READUONLY 4000000h
%define JOY_CAL_READVONLY 8000000h
%define WAVE_FORMAT_QUERY 1h
%define SND_PURGE 40h
%define SND_APPLICATION 80h
%define WAVE_MAPPED 4h
%define WAVE_FORMAT_DIRECT 8h
%define WAVE_FORMAT_DIRECT_QUERY WAVE_FORMAT_QUERY|WAVE_FORMAT_DIRECT
%define MIM_MOREDATA MM_MIM_MOREDATA
%define MOM_POSITIONCB MM_MOM_POSITIONCB
%define MIDI_IO_STATUS 20h
%define DRV_LOAD 1h
%define DRV_ENABLE 2h
%define DRV_OPEN 3h
%define DRV_CLOSE 4h
%define DRV_DISABLE 5h
%define DRV_FREE 6h
%define DRV_CONFIGURE 7h
%define DRV_QUERYCONFIGURE 8h
%define DRV_INSTALL 9h
%define DRV_REMOVE 0Ah
%define DRV_EXITSESSION 0Bh
%define DRV_POWER 0Fh
%define DRV_RESERVED 800h
%define DRV_USER 4000h
%define DRVCNF_CANCEL 0h
%define DRVCNF_OK 1h
%define DRVCNF_RESTART 2h
%define DRV_CANCEL DRVCNF_CANCEL
%define DRV_OK DRVCNF_OK
%define DRV_RESTART DRVCNF_RESTART
%define DRV_MCI_FIRST DRV_RESERVED
%define DRV_MCI_LAST DRV_RESERVED+0FFFh
%define CALLBACK_TYPEMASK 70000h
%define CALLBACK_NULL 0h
%define CALLBACK_WINDOW 10000h
%define CALLBACK_TASK 20000h
%define CALLBACK_FUNCTION 30000h
%define MM_MICROSOFT 1
%define MM_MIDI_MAPPER 1
%define MM_WAVE_MAPPER 2
%define MM_SNDBLST_MIDIOUT 3
%define MM_SNDBLST_MIDIIN 4
%define MM_SNDBLST_SYNTH 5
%define MM_SNDBLST_WAVEOUT 6
%define MM_SNDBLST_WAVEIN 7
%define MM_ADLIB 9
%define MM_MPU401_MIDIOUT 10
%define MM_MPU401_MIDIIN 11
%define MM_PC_JOYSTICK 12
%define SND_SYNC 0h
%define SND_ASYNC 1h
%define SND_NODEFAULT 2h
%define SND_MEMORY 4h
%define SND_ALIAS 10000h
%define SND_FILENAME 20000h
%define SND_RESOURCE 40004h
%define SND_ALIAS_ID 110000h
%define SND_ALIAS_START 0
%define SND_LOOP 8h
%define SND_NOSTOP 10h
%define SND_VALID 1Fh
%define SND_NOWAIT 2000h
%define SND_VALIDFLAGS 17201Fh
%define SND_RESERVED 0FF000000h
%define SND_TYPE_MASK 170007h
%define WAVERR_BADFORMAT WAVERR_BASE+0
%define WAVERR_STILLPLAYING WAVERR_BASE+1
%define WAVERR_UNPREPARED WAVERR_BASE+2
%define WAVERR_SYNC WAVERR_BASE+3
%define WAVERR_LASTERROR WAVERR_BASE+3
%define WOM_OPEN MM_WOM_OPEN
%define WOM_CLOSE MM_WOM_CLOSE
%define WOM_DONE MM_WOM_DONE
%define WIM_OPEN MM_WIM_OPEN
%define WIM_CLOSE MM_WIM_CLOSE
%define WIM_DATA MM_WIM_DATA
%define WAVE_MAPPER -1
%define WAVE_ALLOWSYNC 2h
%define WAVE_VALID 3h
%define WHDR_DONE 1h
%define WHDR_PREPARED 2h
%define WHDR_BEGINLOOP 4h
%define WHDR_ENDLOOP 8h
%define WHDR_INQUEUE 10h
%define WHDR_VALID 1Fh
%define WAVECAPS_PITCH 1h
%define WAVECAPS_PLAYBACKRATE 2h
%define WAVECAPS_VOLUME 4h
%define WAVECAPS_LRVOLUME 8h
%define WAVECAPS_SYNC 10h
%define WAVE_INVALIDFORMAT 0h
%define WAVE_FORMAT_1M08 1h
%define WAVE_FORMAT_1S08 2h
%define WAVE_FORMAT_1M16 4h
%define WAVE_FORMAT_1S16 8h
%define WAVE_FORMAT_2M08 10h
%define WAVE_FORMAT_2S08 20h
%define WAVE_FORMAT_2M16 40h
%define WAVE_FORMAT_2S16 80h
%define WAVE_FORMAT_4M08 100h
%define WAVE_FORMAT_4S08 200h
%define WAVE_FORMAT_4M16 400h
%define WAVE_FORMAT_4S16 800h
%define WAVE_FORMAT_PCM 1
%define MIDIERR_UNPREPARED MIDIERR_BASE+0
%define MIDIERR_STILLPLAYING MIDIERR_BASE+1
%define MIDIERR_NOMAP MIDIERR_BASE+2
%define MIDIERR_NOTREADY MIDIERR_BASE+3
%define MIDIERR_NODEVICE MIDIERR_BASE+4
%define MIDIERR_INVALIDSETUP MIDIERR_BASE+5
%define MIDIERR_LASTERROR MIDIERR_BASE+5
%define MIM_OPEN MM_MIM_OPEN
%define MIM_CLOSE MM_MIM_CLOSE
%define MIM_DATA MM_MIM_DATA
%define MIM_LONGDATA MM_MIM_LONGDATA
%define MIM_ERROR MM_MIM_ERROR
%define MIM_LONGERROR MM_MIM_LONGERROR
%define MOM_OPEN MM_MOM_OPEN
%define MOM_CLOSE MM_MOM_CLOSE
%define MOM_DONE MM_MOM_DONE
%define MIDIMAPPER -1
%define MIDI_MAPPER -1
%define MIDI_CACHE_ALL 1
%define MIDI_CACHE_BESTFIT 2
%define MIDI_CACHE_QUERY 3
%define MIDI_UNCACHE 4
%define MIDI_CACHE_VALID MIDI_CACHE_ALL|MIDI_CACHE_BESTFIT|MIDI_CACHE_QUERY|MIDI_UNCACHE
%define MOD_MIDIPORT 1
%define MOD_SYNTH 2
%define MOD_SQSYNTH 3
%define MOD_FMSYNTH 4
%define MOD_MAPPER 5
%define MIDICAPS_VOLUME 1h
%define MIDICAPS_LRVOLUME 2h
%define MIDICAPS_CACHE 4h
%define MHDR_DONE 1h
%define MHDR_PREPARED 2h
%define MHDR_INQUEUE 4h
%define MHDR_VALID 7h
%define AUX_MAPPER -1
%define AUXCAPS_CDAUDIO 1
%define AUXCAPS_AUXIN 2
%define AUXCAPS_VOLUME 1h
%define AUXCAPS_LRVOLUME 2h
%define TIMERR_NOERROR 0
%define TIMERR_NOCANDO TIMERR_BASE+1
%define TIMERR_STRUCT TIMERR_BASE+33
%define TIME_ONESHOT 0
%define TIME_PERIODIC 1
%define JOYERR_NOERROR 0
%define JOYERR_PARMS JOYERR_BASE+5
%define JOYERR_NOCANDO JOYERR_BASE+6
%define JOYERR_UNPLUGGED JOYERR_BASE+7
%define JOY_BUTTON1 1h
%define JOY_BUTTON2 2h
%define JOY_BUTTON3 4h
%define JOY_BUTTON4 8h
%define JOY_BUTTON1CHG 100h
%define JOY_BUTTON2CHG 200h
%define JOY_BUTTON3CHG 400h
%define JOY_BUTTON4CHG 800h
%define JOYSTICKID1 0
%define JOYSTICKID2 1
%define MMIOERR_BASE 256
%define MMIOERR_FILENOTFOUND MMIOERR_BASE+1
%define MMIOERR_OUTOFMEMORY MMIOERR_BASE+2
%define MMIOERR_CANNOTOPEN MMIOERR_BASE+3
%define MMIOERR_CANNOTCLOSE MMIOERR_BASE+4
%define MMIOERR_CANNOTREAD MMIOERR_BASE+5
%define MMIOERR_CANNOTWRITE MMIOERR_BASE+6
%define MMIOERR_CANNOTSEEK MMIOERR_BASE+7
%define MMIOERR_CANNOTEXPAND MMIOERR_BASE+8
%define MMIOERR_CHUNKNOTFOUND MMIOERR_BASE+9
%define MMIOERR_UNBUFFERED MMIOERR_BASE+10
%define MMIO_RWMODE 3h
%define MMIO_SHAREMODE 70h
%define MMIO_CREATE 1000h
%define MMIO_PARSE 100h
%define MMIO_DELETE 200h
%define MMIO_EXIST 4000h
%define MMIO_ALLOCBUF 10000h
%define MMIO_GETTEMP 20000h
%define MMIO_DIRTY 10000000h
%define MMIO_OPEN_VALID 3FFFFh
%define MMIO_READ 0h
%define MMIO_WRITE 1h
%define MMIO_READWRITE 2h
%define MMIO_COMPAT 0h
%define MMIO_EXCLUSIVE 10h
%define MMIO_DENYWRITE 20h
%define MMIO_DENYREAD 30h
%define MMIO_DENYNONE 40h
%define MMIO_FHOPEN 10h
%define MMIO_EMPTYBUF 10h
%define MMIO_TOUPPER 10h
%define MMIO_INSTALLPROC 10000h
%define MMIO_PUBLICPROC 10000000h
%define MMIO_UNICODEPROC 1000000h
%define MMIO_REMOVEPROC 20000h
%define MMIO_FINDPROC 40000h
%define MMIO_FINDCHUNK 10h
%define MMIO_FINDRIFF 20h
%define MMIO_FINDLIST 40h
%define MMIO_CREATERIFF 20h
%define MMIO_CREATELIST 40h
%define MMIO_VALIDPROC 11070000h
%define MMIOM_READ MMIO_READ
%define MMIOM_WRITE MMIO_WRITE
%define MMIOM_SEEK 2
%define MMIOM_OPEN 3
%define MMIOM_CLOSE 4
%define MMIOM_WRITEFLUSH 5
%define MMIOM_RENAME 6
%define MMIOM_USER 8000h
%define SEEK_SET 0
%define SEEK_CUR 1
%define SEEK_END 2
%define MMIO_DEFAULTBUFFER 8192
%define MCIERR_INVALID_DEVICE_ID MCIERR_BASE+1
%define MCIERR_UNRECOGNIZED_KEYWORD MCIERR_BASE+3
%define MCIERR_UNRECOGNIZED_COMMAND MCIERR_BASE+5
%define MCIERR_HARDWARE MCIERR_BASE+6
%define MCIERR_INVALID_DEVICE_NAME MCIERR_BASE+7
%define MCIERR_OUT_OF_MEMORY MCIERR_BASE+8
%define MCIERR_DEVICE_OPEN MCIERR_BASE+9
%define MCIERR_CANNOT_LOAD_DRIVER MCIERR_BASE+10
%define MCIERR_MISSING_COMMAND_STRING MCIERR_BASE+11
%define MCIERR_PARAM_OVERFLOW MCIERR_BASE+12
%define MCIERR_MISSING_STRING_ARGUMENT MCIERR_BASE+13
%define MCIERR_BAD_INTEGER MCIERR_BASE+14
%define MCIERR_PARSER_INTERNAL MCIERR_BASE+15
%define MCIERR_DRIVER_INTERNAL MCIERR_BASE+16
%define MCIERR_MISSING_PARAMETER MCIERR_BASE+17
%define MCIERR_UNSUPPORTED_FUNCTION MCIERR_BASE+18
%define MCIERR_FILE_NOT_FOUND MCIERR_BASE+19
%define MCIERR_DEVICE_NOT_READY MCIERR_BASE+20
%define MCIERR_INTERNAL MCIERR_BASE+21
%define MCIERR_DRIVER MCIERR_BASE+22
%define MCIERR_CANNOT_USE_ALL MCIERR_BASE+23
%define MCIERR_MULTIPLE MCIERR_BASE+24
%define MCIERR_EXTENSION_NOT_FOUND MCIERR_BASE+25
%define MCIERR_OUTOFRANGE MCIERR_BASE+26
%define MCIERR_FLAGS_NOT_COMPATIBLE MCIERR_BASE+28
%define MCIERR_FILE_NOT_SAVED MCIERR_BASE+30
%define MCIERR_DEVICE_TYPE_REQUIRED MCIERR_BASE+31
%define MCIERR_DEVICE_LOCKED MCIERR_BASE+32
%define MCIERR_DUPLICATE_ALIAS MCIERR_BASE+33
%define MCIERR_BAD_CONSTANT MCIERR_BASE+34
%define MCIERR_MUST_USE_SHAREABLE MCIERR_BASE+35
%define MCIERR_MISSING_DEVICE_NAME MCIERR_BASE+36
%define MCIERR_BAD_TIME_FORMAT MCIERR_BASE+37
%define MCIERR_NO_CLOSING_QUOTE MCIERR_BASE+38
%define MCIERR_DUPLICATE_FLAGS MCIERR_BASE+39
%define MCIERR_INVALID_FILE MCIERR_BASE+40
%define MCIERR_NULL_PARAMETER_BLOCK MCIERR_BASE+41
%define MCIERR_UNNAMED_RESOURCE MCIERR_BASE+42
%define MCIERR_NEW_REQUIRES_ALIAS MCIERR_BASE+43
%define MCIERR_NOTIFY_ON_AUTO_OPEN MCIERR_BASE+44
%define MCIERR_NO_ELEMENT_ALLOWED MCIERR_BASE+45
%define MCIERR_NONAPPLICABLE_FUNCTION MCIERR_BASE+46
%define MCIERR_ILLEGAL_FOR_AUTO_OPEN MCIERR_BASE+47
%define MCIERR_FILENAME_REQUIRED MCIERR_BASE+48
%define MCIERR_EXTRA_CHARACTERS MCIERR_BASE+49
%define MCIERR_DEVICE_NOT_INSTALLED MCIERR_BASE+50
%define MCIERR_GET_CD MCIERR_BASE+51
%define MCIERR_SET_CD MCIERR_BASE+52
%define MCIERR_SET_DRIVE MCIERR_BASE+53
%define MCIERR_DEVICE_LENGTH MCIERR_BASE+54
%define MCIERR_DEVICE_ORD_LENGTH MCIERR_BASE+55
%define MCIERR_NO_INTEGER MCIERR_BASE+56
%define MCIERR_WAVE_OUTPUTSINUSE MCIERR_BASE+64
%define MCIERR_WAVE_SETOUTPUTINUSE MCIERR_BASE+65
%define MCIERR_WAVE_INPUTSINUSE MCIERR_BASE+66
%define MCIERR_WAVE_SETINPUTINUSE MCIERR_BASE+67
%define MCIERR_WAVE_OUTPUTUNSPECIFIED MCIERR_BASE+68
%define MCIERR_WAVE_INPUTUNSPECIFIED MCIERR_BASE+69
%define MCIERR_WAVE_OUTPUTSUNSUITABLE MCIERR_BASE+70
%define MCIERR_WAVE_SETOUTPUTUNSUITABLE MCIERR_BASE+71
%define MCIERR_WAVE_INPUTSUNSUITABLE MCIERR_BASE+72
%define MCIERR_WAVE_SETINPUTUNSUITABLE MCIERR_BASE+73
%define MCIERR_SEQ_DIV_INCOMPATIBLE MCIERR_BASE+80
%define MCIERR_SEQ_PORT_INUSE MCIERR_BASE+81
%define MCIERR_SEQ_PORT_NONEXISTENT MCIERR_BASE+82
%define MCIERR_SEQ_PORT_MAPNODEVICE MCIERR_BASE+83
%define MCIERR_SEQ_PORT_MISCERROR MCIERR_BASE+84
%define MCIERR_SEQ_TIMER MCIERR_BASE+85
%define MCIERR_SEQ_PORTUNSPECIFIED MCIERR_BASE+86
%define MCIERR_SEQ_NOMIDIPRESENT MCIERR_BASE+87
%define MCIERR_NO_WINDOW MCIERR_BASE+90
%define MCIERR_CREATEWINDOW MCIERR_BASE+91
%define MCIERR_FILE_READ MCIERR_BASE+92
%define MCIERR_FILE_WRITE MCIERR_BASE+93
%define MCIERR_CUSTOM_DRIVER_BASE MCIERR_BASE+256
%define MCI_FIRST 800h
%define MCI_OPEN 803h
%define MCI_CLOSE 804h
%define MCI_ESCAPE 805h
%define MCI_PLAY 806h
%define MCI_SEEK 807h
%define MCI_STOP 808h
%define MCI_PAUSE 809h
%define MCI_INFO 80Ah
%define MCI_GETDEVCAPS 80Bh
%define MCI_SPIN 80Ch
%define MCI_SET 80Dh
%define MCI_STEP 80Eh
%define MCI_RECORD 80Fh
%define MCI_SYSINFO 810h
%define MCI_BREAK 811h
%define MCI_SOUND 812h
%define MCI_SAVE 813h
%define MCI_STATUS 814h
%define MCI_CUE 830h
%define MCI_REALIZE 840h
%define MCI_WINDOW 841h
%define MCI_PUT 842h
%define MCI_WHERE 843h
%define MCI_FREEZE 844h
%define MCI_UNFREEZE 845h
%define MCI_LOAD 850h
%define MCI_CUT 851h
%define MCI_COPY 852h
%define MCI_PASTE 853h
%define MCI_UPDATE 854h
%define MCI_RESUME 855h
%define MCI_DELETE 856h
%define MCI_LAST 0FFFh
%define MCI_USER_MESSAGES 400h+MCI_FIRST
%define MCI_ALL_DEVICE_ID -1
%define MCI_DEVTYPE_VCR 513
%define MCI_DEVTYPE_VIDEODISC 514
%define MCI_DEVTYPE_OVERLAY 515
%define MCI_DEVTYPE_CD_AUDIO 516
%define MCI_DEVTYPE_DAT 517
%define MCI_DEVTYPE_SCANNER 518
%define MCI_DEVTYPE_ANIMATION 519
%define MCI_DEVTYPE_DIGITAL_VIDEO 520
%define MCI_DEVTYPE_OTHER 521
%define MCI_DEVTYPE_WAVEFORM_AUDIO 522
%define MCI_DEVTYPE_SEQUENCER 523
%define MCI_DEVTYPE_FIRST MCI_DEVTYPE_VCR
%define MCI_DEVTYPE_LAST MCI_DEVTYPE_SEQUENCER
%define MCI_DEVTYPE_FIRST_USER 1000h
%define MCI_MODE_NOT_READY MCI_STRING_OFFSET+12
%define MCI_MODE_STOP MCI_STRING_OFFSET+13
%define MCI_MODE_PLAY MCI_STRING_OFFSET+14
%define MCI_MODE_RECORD MCI_STRING_OFFSET+15
%define MCI_MODE_SEEK MCI_STRING_OFFSET+16
%define MCI_MODE_PAUSE MCI_STRING_OFFSET+17
%define MCI_MODE_OPEN MCI_STRING_OFFSET+18
%define MCI_FORMAT_MILLISECONDS 0
%define MCI_FORMAT_HMS 1
%define MCI_FORMAT_MSF 2
%define MCI_FORMAT_FRAMES 3
%define MCI_FORMAT_SMPTE_24 4
%define MCI_FORMAT_SMPTE_25 5
%define MCI_FORMAT_SMPTE_30 6
%define MCI_FORMAT_SMPTE_30DROP 7
%define MCI_FORMAT_BYTES 8
%define MCI_FORMAT_SAMPLES 9
%define MCI_FORMAT_TMSF 10
%define MCI_NOTIFY_SUCCESSFUL 1h
%define MCI_NOTIFY_SUPERSEDED 2h
%define MCI_NOTIFY_ABORTED 4h
%define MCI_NOTIFY_FAILURE 8h
%define MCI_NOTIFY 1h
%define MCI_WAIT 2h
%define MCI_FROM 4h
%define MCI_TO 8h
%define MCI_TRACK 10h
%define MCI_OPEN_SHAREABLE 100h
%define MCI_OPEN_ELEMENT 200h
%define MCI_OPEN_ALIAS 400h
%define MCI_OPEN_ELEMENT_ID 800h
%define MCI_OPEN_TYPE_ID 1000h
%define MCI_OPEN_TYPE 2000h
%define MCI_SEEK_TO_START 100h
%define MCI_SEEK_TO_END 200h
%define MCI_STATUS_ITEM 100h
%define MCI_STATUS_START 200h
%define MCI_STATUS_LENGTH 1h
%define MCI_STATUS_POSITION 2h
%define MCI_STATUS_NUMBER_OF_TRACKS 3h
%define MCI_STATUS_MODE 4h
%define MCI_STATUS_MEDIA_PRESENT 5h
%define MCI_STATUS_TIME_FORMAT 6h
%define MCI_STATUS_READY 7h
%define MCI_STATUS_CURRENT_TRACK 8h
%define MCI_INFO_PRODUCT 100h
%define MCI_INFO_FILE 200h
%define MCI_GETDEVCAPS_ITEM 100h
%define MCI_GETDEVCAPS_CAN_RECORD 1h
%define MCI_GETDEVCAPS_HAS_AUDIO 2h
%define MCI_GETDEVCAPS_HAS_VIDEO 3h
%define MCI_GETDEVCAPS_DEVICE_TYPE 4h
%define MCI_GETDEVCAPS_USES_FILES 5h
%define MCI_GETDEVCAPS_COMPOUND_DEVICE 6h
%define MCI_GETDEVCAPS_CAN_EJECT 7h
%define MCI_GETDEVCAPS_CAN_PLAY 8h
%define MCI_GETDEVCAPS_CAN_SAVE 9h
%define MCI_SYSINFO_QUANTITY 100h
%define MCI_SYSINFO_OPEN 200h
%define MCI_SYSINFO_NAME 400h
%define MCI_SYSINFO_INSTALLNAME 800h
%define MCI_SET_DOOR_OPEN 100h
%define MCI_SET_DOOR_CLOSED 200h
%define MCI_SET_TIME_FORMAT 400h
%define MCI_SET_AUDIO 800h
%define MCI_SET_VIDEO 1000h
%define MCI_SET_ON 2000h
%define MCI_SET_OFF 4000h
%define MCI_SET_AUDIO_ALL 4001h
%define MCI_SET_AUDIO_LEFT 4002h
%define MCI_SET_AUDIO_RIGHT 4003h
%define MCI_BREAK_KEY 100h
%define MCI_BREAK_HWND 200h
%define MCI_BREAK_OFF 400h
%define MCI_RECORD_INSERT 100h
%define MCI_RECORD_OVERWRITE 200h
%define MCI_SOUND_NAME 100h
%define MCI_SAVE_FILE 100h
%define MCI_LOAD_FILE 100h
%define MCI_VD_MODE_PARK MCI_VD_OFFSET+1
%define MCI_VD_MEDIA_CLV MCI_VD_OFFSET+2
%define MCI_VD_MEDIA_CAV MCI_VD_OFFSET+3
%define MCI_VD_MEDIA_OTHER MCI_VD_OFFSET+4
%define MCI_VD_FORMAT_TRACK 4001h
%define MCI_VD_PLAY_REVERSE 10000h
%define MCI_VD_PLAY_FAST 20000h
%define MCI_VD_PLAY_SPEED 40000h
%define MCI_VD_PLAY_SCAN 80000h
%define MCI_VD_PLAY_SLOW 100000h
%define MCI_VD_SEEK_REVERSE 10000h
%define MCI_VD_STATUS_SPEED 4002h
%define MCI_VD_STATUS_FORWARD 4003h
%define MCI_VD_STATUS_MEDIA_TYPE 4004h
%define MCI_VD_STATUS_SIDE 4005h
%define MCI_VD_STATUS_DISC_SIZE 4006h
%define MCI_VD_GETDEVCAPS_CLV 10000h
%define MCI_VD_GETDEVCAPS_CAV 20000h
%define MCI_VD_SPIN_UP 10000h
%define MCI_VD_SPIN_DOWN 20000h
%define MCI_VD_GETDEVCAPS_CAN_REVERSE 4002h
%define MCI_VD_GETDEVCAPS_FAST_RATE 4003h
%define MCI_VD_GETDEVCAPS_SLOW_RATE 4004h
%define MCI_VD_GETDEVCAPS_NORMAL_RATE 4005h
%define MCI_VD_STEP_FRAMES 10000h
%define MCI_VD_STEP_REVERSE 20000h
%define MCI_VD_ESCAPE_STRING 100h
%define MCI_WAVE_PCM MCI_WAVE_OFFSET+0
%define MCI_WAVE_MAPPER MCI_WAVE_OFFSET+1
%define MCI_WAVE_OPEN_BUFFER 10000h
%define MCI_WAVE_SET_FORMATTAG 10000h
%define MCI_WAVE_SET_CHANNELS 20000h
%define MCI_WAVE_SET_SAMPLESPERSEC 40000h
%define MCI_WAVE_SET_AVGBYTESPERSEC 80000h
%define MCI_WAVE_SET_BLOCKALIGN 100000h
%define MCI_WAVE_SET_BITSPERSAMPLE 200000h
%define MCI_WAVE_INPUT 400000h
%define MCI_WAVE_OUTPUT 800000h
%define MCI_WAVE_STATUS_FORMATTAG 4001h
%define MCI_WAVE_STATUS_CHANNELS 4002h
%define MCI_WAVE_STATUS_SAMPLESPERSEC 4003h
%define MCI_WAVE_STATUS_AVGBYTESPERSEC 4004h
%define MCI_WAVE_STATUS_BLOCKALIGN 4005h
%define MCI_WAVE_STATUS_BITSPERSAMPLE 4006h
%define MCI_WAVE_STATUS_LEVEL 4007h
%define MCI_WAVE_SET_ANYINPUT 4000000h
%define MCI_WAVE_SET_ANYOUTPUT 8000000h
%define MCI_WAVE_GETDEVCAPS_INPUTS 4001h
%define MCI_WAVE_GETDEVCAPS_OUTPUTS 4002h
%define MCI_SEQ_DIV_PPQN 0+MCI_SEQ_OFFSET
%define MCI_SEQ_DIV_SMPTE_24 1+MCI_SEQ_OFFSET
%define MCI_SEQ_DIV_SMPTE_25 2+MCI_SEQ_OFFSET
%define MCI_SEQ_DIV_SMPTE_30DROP 3+MCI_SEQ_OFFSET
%define MCI_SEQ_DIV_SMPTE_30 4+MCI_SEQ_OFFSET
%define MCI_SEQ_FORMAT_SONGPTR 4001h
%define MCI_SEQ_FILE 4002h
%define MCI_SEQ_MIDI 4003h
%define MCI_SEQ_SMPTE 4004h
%define MCI_SEQ_NONE 65533
%define MCI_SEQ_MAPPER 65535
%define MCI_SEQ_STATUS_TEMPO 4002h
%define MCI_SEQ_STATUS_PORT 4003h
%define MCI_SEQ_STATUS_SLAVE 4007h
%define MCI_SEQ_STATUS_MASTER 4008h
%define MCI_SEQ_STATUS_OFFSET 4009h
%define MCI_SEQ_STATUS_DIVTYPE 400Ah
%define MCI_SEQ_SET_TEMPO 10000h
%define MCI_SEQ_SET_PORT 20000h
%define MCI_SEQ_SET_SLAVE 40000h
%define MCI_SEQ_SET_MASTER 80000h
%define MCI_SEQ_SET_OFFSET 1000000h
%define MCI_ANIM_OPEN_WS 10000h
%define MCI_ANIM_OPEN_PARENT 20000h
%define MCI_ANIM_OPEN_NOSTATIC 40000h
%define MCI_ANIM_PLAY_SPEED 10000h
%define MCI_ANIM_PLAY_REVERSE 20000h
%define MCI_ANIM_PLAY_FAST 40000h
%define MCI_ANIM_PLAY_SLOW 80000h
%define MCI_ANIM_PLAY_SCAN 100000h
%define MCI_ANIM_STEP_REVERSE 10000h
%define MCI_ANIM_STEP_FRAMES 20000h
%define MCI_ANIM_STATUS_SPEED 4001h
%define MCI_ANIM_STATUS_FORWARD 4002h
%define MCI_ANIM_STATUS_HWND 4003h
%define MCI_ANIM_STATUS_HPAL 4004h
%define MCI_ANIM_STATUS_STRETCH 4005h
%define MCI_ANIM_INFO_TEXT 10000h
%define MCI_ANIM_GETDEVCAPS_CAN_REVERSE 4001h
%define MCI_ANIM_GETDEVCAPS_FAST_RATE 4002h
%define MCI_ANIM_GETDEVCAPS_SLOW_RATE 4003h
%define MCI_ANIM_GETDEVCAPS_NORMAL_RATE 4004h
%define MCI_ANIM_GETDEVCAPS_PALETTES 4006h
%define MCI_ANIM_GETDEVCAPS_CAN_STRETCH 4007h
%define MCI_ANIM_GETDEVCAPS_MAX_WINDOWS 4008h
%define MCI_ANIM_REALIZE_NORM 10000h
%define MCI_ANIM_REALIZE_BKGD 20000h
%define MCI_ANIM_WINDOW_HWND 10000h
%define MCI_ANIM_WINDOW_STATE 40000h
%define MCI_ANIM_WINDOW_TEXT 80000h
%define MCI_ANIM_WINDOW_ENABLE_STRETCH 100000h
%define MCI_ANIM_WINDOW_DISABLE_STRETCH 200000h
%define MCI_ANIM_WINDOW_DEFAULT 0h
%define MCI_ANIM_RECT 10000h
%define MCI_ANIM_PUT_SOURCE 20000h
%define MCI_ANIM_PUT_DESTINATION 40000h
%define MCI_ANIM_WHERE_SOURCE 20000h
%define MCI_ANIM_WHERE_DESTINATION 40000h
%define MCI_ANIM_UPDATE_HDC 20000h
%define MCI_OVLY_OPEN_WS 10000h
%define MCI_OVLY_OPEN_PARENT 20000h
%define MCI_OVLY_STATUS_HWND 4001h
%define MCI_OVLY_STATUS_STRETCH 4002h
%define MCI_OVLY_INFO_TEXT 10000h
%define MCI_OVLY_GETDEVCAPS_CAN_STRETCH 4001h
%define MCI_OVLY_GETDEVCAPS_CAN_FREEZE 4002h
%define MCI_OVLY_GETDEVCAPS_MAX_WINDOWS 4003h
%define MCI_OVLY_WINDOW_HWND 10000h
%define MCI_OVLY_WINDOW_STATE 40000h
%define MCI_OVLY_WINDOW_TEXT 80000h
%define MCI_OVLY_WINDOW_ENABLE_STRETCH 100000h
%define MCI_OVLY_WINDOW_DISABLE_STRETCH 200000h
%define MCI_OVLY_WINDOW_DEFAULT 0h
%define MCI_OVLY_RECT 10000h
%define MCI_OVLY_PUT_SOURCE 20000h
%define MCI_OVLY_PUT_DESTINATION 40000h
%define MCI_OVLY_PUT_FRAME 80000h
%define MCI_OVLY_PUT_VIDEO 100000h
%define MCI_OVLY_WHERE_SOURCE 20000h
%define MCI_OVLY_WHERE_DESTINATION 40000h
%define MCI_OVLY_WHERE_FRAME 80000h
%define MCI_OVLY_WHERE_VIDEO 100000h
%define CAPS1 94
%define C1_TRANSPARENT 1h
%define NEWTRANSPARENT 3
%define QUERYROPSUPPORT 40
%define SELECTDIB 41
%define SE_ERR_SHARE 26
%define SE_ERR_ASSOCINCOMPLETE 27
%define SE_ERR_DDETIMEOUT 28
%define SE_ERR_DDEFAIL 29
%define SE_ERR_DDEBUSY 30
%define SE_ERR_NOASSOC 31
%define PRINTER_CONTROL_PAUSE 1
%define PRINTER_CONTROL_RESUME 2
%define PRINTER_CONTROL_PURGE 3
%define PRINTER_STATUS_PAUSED 1h
%define PRINTER_STATUS_ERROR 2h
%define PRINTER_STATUS_PENDING_DELETION 4h
%define PRINTER_STATUS_PAPER_JAM 8h
%define PRINTER_STATUS_PAPER_OUT 10h
%define PRINTER_STATUS_MANUAL_FEED 20h
%define PRINTER_STATUS_PAPER_PROBLEM 40h
%define PRINTER_STATUS_OFFLINE 80h
%define PRINTER_STATUS_IO_ACTIVE 100h
%define PRINTER_STATUS_BUSY 200h
%define PRINTER_STATUS_PRINTING 400h
%define PRINTER_STATUS_OUTPUT_BIN_FULL 800h
%define PRINTER_STATUS_NOT_AVAILABLE 1000h
%define PRINTER_STATUS_WAITING 2000h
%define PRINTER_STATUS_PROCESSING 4000h
%define PRINTER_STATUS_INITIALIZING 8000h
%define PRINTER_STATUS_WARMING_UP 10000h
%define PRINTER_STATUS_TONER_LOW 20000h
%define PRINTER_STATUS_NO_TONER 40000h
%define PRINTER_STATUS_PAGE_PUNT 80000h
%define PRINTER_STATUS_USER_INTERVENTION 100000h
%define PRINTER_STATUS_OUT_OF_MEMORY 200000h
%define PRINTER_STATUS_DOOR_OPEN 400000h
%define PRINTER_ATTRIBUTE_QUEUED 1h
%define PRINTER_ATTRIBUTE_DIRECT 2h
%define PRINTER_ATTRIBUTE_DEFAULT 4h
%define PRINTER_ATTRIBUTE_SHARED 8h
%define PRINTER_ATTRIBUTE_NETWORK 10h
%define PRINTER_ATTRIBUTE_HIDDEN 20h
%define PRINTER_ATTRIBUTE_LOCAL 40h
%define NO_PRIORITY 0
%define MAX_PRIORITY 99
%define MIN_PRIORITY 1
%define DEF_PRIORITY 1
%define JOB_CONTROL_PAUSE 1
%define JOB_CONTROL_RESUME 2
%define JOB_CONTROL_CANCEL 3
%define JOB_CONTROL_RESTART 4
%define JOB_STATUS_PAUSED 1h
%define JOB_STATUS_ERROR 2h
%define JOB_STATUS_DELETING 4h
%define JOB_STATUS_SPOOLING 8h
%define JOB_STATUS_PRINTING 10h
%define JOB_STATUS_OFFLINE 20h
%define JOB_STATUS_PAPEROUT 40h
%define JOB_STATUS_PRINTED 80h
%define JOB_POSITION_UNSPECIFIED 0
%define FORM_BUILTIN 1h
%define PRINTER_CONTROL_SET_STATUS 4
%define PRINTER_ATTRIBUTE_WORK_OFFLINE 400h
%define PRINTER_ATTRIBUTE_ENABLE_BIDI 800h
%define JOB_CONTROL_DELETE 5
%define JOB_STATUS_USER_INTERVENTION 10000h
%define DI_CHANNEL 1
%define DI_READ_SPOOL_JOB 3
%define PORT_TYPE_WRITE 1h
%define PORT_TYPE_READ 2h
%define PORT_TYPE_REDIRECTED 4h
%define PORT_TYPE_NET_ATTACHED 8h
%define PRINTER_ENUM_DEFAULT 1h
%define PRINTER_ENUM_LOCAL 2h
%define PRINTER_ENUM_CONNECTIONS 4h
%define PRINTER_ENUM_FAVORITE 4h
%define PRINTER_ENUM_NAME 8h
%define PRINTER_ENUM_REMOTE 10h
%define PRINTER_ENUM_SHARED 20h
%define PRINTER_ENUM_NETWORK 40h
%define PRINTER_ENUM_EXPAND 4000h
%define PRINTER_ENUM_CONTAINER 8000h
%define PRINTER_ENUM_ICONMASK 0FF0000h
%define PRINTER_ENUM_ICON1 10000h
%define PRINTER_ENUM_ICON2 20000h
%define PRINTER_ENUM_ICON3 40000h
%define PRINTER_ENUM_ICON4 80000h
%define PRINTER_ENUM_ICON5 100000h
%define PRINTER_ENUM_ICON6 200000h
%define PRINTER_ENUM_ICON7 400000h
%define PRINTER_ENUM_ICON8 800000h
%define PRINTER_CHANGE_ADD_PRINTER 1h
%define PRINTER_CHANGE_SET_PRINTER 2h
%define PRINTER_CHANGE_DELETE_PRINTER 4h
%define PRINTER_CHANGE_PRINTER 0FFh
%define PRINTER_CHANGE_ADD_JOB 100h
%define PRINTER_CHANGE_SET_JOB 200h
%define PRINTER_CHANGE_DELETE_JOB 400h
%define PRINTER_CHANGE_WRITE_JOB 800h
%define PRINTER_CHANGE_JOB 0FF00h
%define PRINTER_CHANGE_ADD_FORM 10000h
%define PRINTER_CHANGE_SET_FORM 20000h
%define PRINTER_CHANGE_DELETE_FORM 40000h
%define PRINTER_CHANGE_FORM 70000h
%define PRINTER_CHANGE_ADD_PORT 100000h
%define PRINTER_CHANGE_CONFIGURE_PORT 200000h
%define PRINTER_CHANGE_DELETE_PORT 400000h
%define PRINTER_CHANGE_PORT 700000h
%define PRINTER_CHANGE_ADD_PRINT_PROCESSOR 1000000h
%define PRINTER_CHANGE_DELETE_PRINT_PROCESSOR 4000000h
%define PRINTER_CHANGE_PRINT_PROCESSOR 7000000h
%define PRINTER_CHANGE_ADD_PRINTER_DRIVER 10000000h
%define PRINTER_CHANGE_DELETE_PRINTER_DRIVER 40000000h
%define PRINTER_CHANGE_PRINTER_DRIVER 70000000h
%define PRINTER_CHANGE_TIMEOUT 80000000h
%define PRINTER_CHANGE_ALL 7777FFFFh
%define PRINTER_ERROR_INFORMATION 80000000h
%define PRINTER_ERROR_WARNING 40000000h
%define PRINTER_ERROR_SEVERE 20000000h
%define PRINTER_ERROR_OUTOFPAPER 1h
%define PRINTER_ERROR_JAM 2h
%define PRINTER_ERROR_OUTOFTONER 4h
%define SERVER_ACCESS_ADMINISTER 1h
%define SERVER_ACCESS_ENUMERATE 2h
%define PRINTER_ACCESS_ADMINISTER 4h
%define PRINTER_ACCESS_USE 8h
%define JOB_ACCESS_ADMINISTER 10h
%define SERVER_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SERVER_ACCESS_ADMINISTER|SERVER_ACCESS_ENUMERATE
%define SERVER_READ STANDARD_RIGHTS_READ|SERVER_ACCESS_ENUMERATE
%define SERVER_WRITE STANDARD_RIGHTS_WRITE|SERVER_ACCESS_ADMINISTER|SERVER_ACCESS_ENUMERATE
%define SERVER_EXECUTE STANDARD_RIGHTS_EXECUTE|SERVER_ACCESS_ENUMERATE
%define PRINTER_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|PRINTER_ACCESS_ADMINISTER|PRINTER_ACCESS_USE
%define PRINTER_READ STANDARD_RIGHTS_READ|PRINTER_ACCESS_USE
%define PRINTER_WRITE STANDARD_RIGHTS_WRITE|PRINTER_ACCESS_USE
%define PRINTER_EXECUTE STANDARD_RIGHTS_EXECUTE|PRINTER_ACCESS_USE
%define JOB_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|JOB_ACCESS_ADMINISTER
%define JOB_READ STANDARD_RIGHTS_READ|JOB_ACCESS_ADMINISTER
%define JOB_WRITE STANDARD_RIGHTS_WRITE|JOB_ACCESS_ADMINISTER
%define JOB_EXECUTE STANDARD_RIGHTS_EXECUTE|JOB_ACCESS_ADMINISTER
%define RESOURCE_CONNECTED 1h
%define RESOURCE_PUBLICNET 2h
%define RESOURCE_GLOBALNET 2h
%define RESOURCE_REMEMBERED 3h
%define RESOURCE_RECENT 4h
%define RESOURCE_CONTEXT 5h
%define RESOURCETYPE_ANY 0h
%define RESOURCETYPE_DISK 1h
%define RESOURCETYPE_PRINT 2h
%define RESOURCETYPE_UNKNOWN 0FFFFh
%define RESOURCEUSAGE_CONNECTABLE 1h
%define RESOURCEUSAGE_CONTAINER 2h
%define RESOURCEUSAGE_RESERVED 80000000h
%define RESOURCEDISPLAYTYPE_GENERIC 0h
%define RESOURCEDISPLAYTYPE_DOMAIN 1h
%define RESOURCEDISPLAYTYPE_SERVER 2h
%define RESOURCEDISPLAYTYPE_SHARE 3h
%define RESOURCEDISPLAYTYPE_FILE 4h
%define RESOURCEDISPLAYTYPE_GROUP 5h
%define CONNECT_UPDATE_PROFILE 1h
%define WN_SUCCESS NO_ERROR
%define WN_NOT_SUPPORTED ERROR_NOT_SUPPORTED
%define WN_NET_ERROR ERROR_UNEXP_NET_ERR
%define WN_MORE_DATA ERROR_MORE_DATA
%define WN_BAD_POINTER ERROR_INVALID_ADDRESS
%define WN_BAD_VALUE ERROR_INVALID_PARAMETER
%define WN_BAD_PASSWORD ERROR_INVALID_PASSWORD
%define WN_ACCESS_DENIED ERROR_ACCESS_DENIED
%define WN_FUNCTION_BUSY ERROR_BUSY
%define WN_WINDOWS_ERROR ERROR_UNEXP_NET_ERR
%define WN_BAD_USER ERROR_BAD_USERNAME
%define WN_OUT_OF_MEMORY ERROR_NOT_ENOUGH_MEMORY
%define WN_NO_NETWORK ERROR_NO_NETWORK
%define WN_EXTENDED_ERROR ERROR_EXTENDED_ERROR
%define WN_NOT_CONNECTED ERROR_NOT_CONNECTED
%define WN_OPEN_FILES ERROR_OPEN_FILES
%define WN_DEVICE_IN_USE ERROR_DEVICE_IN_USE
%define WN_BAD_NETNAME ERROR_BAD_NET_NAME
%define WN_BAD_LOCALNAME ERROR_BAD_DEVICE
%define WN_ALREADY_CONNECTED ERROR_ALREADY_ASSIGNED
%define WN_DEVICE_ERROR ERROR_GEN_FAILURE
%define WN_CONNECTION_CLOSED ERROR_CONNECTION_UNAVAIL
%define WN_NO_NET_OR_BAD_PATH ERROR_NO_NET_OR_BAD_PATH
%define WN_BAD_PROVIDER ERROR_BAD_PROVIDER
%define WN_CANNOT_OPEN_PROFILE ERROR_CANNOT_OPEN_PROFILE
%define WN_BAD_PROFILE ERROR_BAD_PROFILE
%define WN_BAD_HANDLE ERROR_INVALID_HANDLE
%define WN_NO_MORE_ENTRIES ERROR_NO_MORE_ITEMS
%define WN_NOT_CONTAINER ERROR_NOT_CONTAINER
%define WN_NO_ERROR NO_ERROR
%define NCBNAMSZ 16
%define MAX_LANA 254
%define NAME_FLAGS_MASK 87h
%define GROUP_NAME 80h
%define UNIQUE_NAME 0h
%define REGISTERING 0h
%define REGISTERED 4h
%define DEREGISTERED 5h
%define DUPLICATE 6h
%define DUPLICATE_DEREG 7h
%define LISTEN_OUTSTANDING 1h
%define CALL_PENDING 2h
%define SESSION_ESTABLISHED 3h
%define HANGUP_PENDING 4h
%define HANGUP_COMPLETE 5h
%define SESSION_ABORTED 6h
%define NCBCALL 10h
%define NCBLISTEN 11h
%define NCBHANGUP 12h
%define NCBSEND 14h
%define NCBRECV 15h
%define NCBRECVANY 16h
%define NCBCHAINSEND 17h
%define NCBDGSEND 20h
%define NCBDGRECV 21h
%define NCBDGSENDBC 22h
%define NCBDGRECVBC 23h
%define NCBADDNAME 30h
%define NCBDELNAME 31h
%define NCBRESET 32h
%define NCBASTAT 33h
%define NCBSSTAT 34h
%define NCBCANCEL 35h
%define NCBADDGRNAME 36h
%define NCBENUM 37h
%define NCBUNLINK 70h
%define NCBSENDNA 71h
%define NCBCHAINSENDNA 72h
%define NCBLANSTALERT 73h
%define NCBACTION 77h
%define NCBFINDNAME 78h
%define NCBTRACE 79h
%define ASYNCH 80h
%define NRC_GOODRET 0h
%define NRC_BUFLEN 1h
%define NRC_ILLCMD 3h
%define NRC_CMDTMO 5h
%define NRC_INCOMP 6h
%define NRC_BADDR 7h
%define NRC_SNUMOUT 8h
%define NRC_NORES 9h
%define NRC_SCLOSED 0Ah
%define NRC_CMDCAN 0Bh
%define NRC_DUPNAME 0Dh
%define NRC_NAMTFUL 0Eh
%define NRC_ACTSES 0Fh
%define NRC_LOCTFUL 11h
%define NRC_REMTFUL 12h
%define NRC_ILLNN 13h
%define NRC_NOCALL 14h
%define NRC_NOWILD 15h
%define NRC_INUSE 16h
%define NRC_NAMERR 17h
%define NRC_SABORT 18h
%define NRC_NAMCONF 19h
%define NRC_IFBUSY 21h
%define NRC_TOOMANY 22h
%define NRC_BRIDGE 23h
%define NRC_CANOCCR 24h
%define NRC_CANCEL 26h
%define NRC_DUPENV 30h
%define NRC_ENVNOTDEF 34h

;	SetWindowPos, DeferWindowPos
%assign	HWND_BOTTOM			1
%assign	HWND_NOTOPMOST			-2
%assign	HWND_TOP			0
%assign	HWND_TOPMOST			-1
%assign	SWP_DRAWFRAME			32
%assign	SWP_FRAMECHANGED		32
%assign	SWP_HIDEWINDOW			128
%assign	SWP_NOACTIVATE			16
%assign	SWP_NOCOPYBITS			256
%assign	SWP_NOMOVE			2
%assign	SWP_NOSIZE			1
%assign	SWP_NOREDRAW			8
%assign	SWP_NOZORDER			4
%assign	SWP_SHOWWINDOW			64
%assign	SWP_NOOWNERZORDER		512
%assign	SWP_NOREPOSITION		512
%assign	SWP_NOSENDCHANGING		1024



%define NRC_OSRESNOTAV 35h
%define NRC_MAXAPPS 36h
%define NRC_NOSAPS 37h
%define NRC_NORESOURCES 38h
%define NRC_INVADDRESS 39h
%define NRC_INVDDID 3Bh
%define NRC_LOCKFAIL 3Ch
%define NRC_OPENERR 3Fh
%define NRC_SYSTEM 40h
%define NRC_PENDING 0FFh
%define EXCEPTION_EXECUTE_HANDLER 1
%define EXCEPTION_CONTINUE_SEARCH 0
%define EXCEPTION_CONTINUE_EXECUTION -1
%define ctlFirst 400h
%define ctlLast 4FFh
%define psh1 400h
%define psh2 401h
%define psh3 402h
%define psh4 403h
%define psh5 404h
%define psh6 405h
%define psh7 406h
%define psh8 407h
%define psh9 408h
%define psh10 409h
%define psh11 40Ah
%define psh12 40Bh
%define psh13 40Ch
%define psh14 40Dh
%define psh15 40Eh
%define pshHelp psh15
%define psh16 40Fh
%define chx1 410h
%define chx2 411h
%define chx3 412h
%define chx4 413h
%define chx5 414h
%define chx6 415h
%define chx7 416h
%define chx8 417h
%define chx9 418h
%define chx10 419h
%define chx11 41Ah
%define chx12 41Bh
%define chx13 41Ch
%define chx14 41Dh
%define chx15 41Eh
%define chx16 41Dh
%define rad1 420h
%define rad2 421h
%define rad3 422h
%define rad4 423h
%define rad5 424h
%define rad6 425h
%define rad7 426h
%define rad8 427h
%define rad9 428h
%define rad10 429h
%define rad11 42Ah
%define rad12 42Bh
%define rad13 42Ch
%define rad14 42Dh
%define rad15 42Eh
%define rad16 42Fh
%define grp1 430h
%define grp2 431h
%define grp3 432h
%define grp4 433h
%define frm1 434h
%define frm2 435h
%define frm3 436h
%define frm4 437h
%define rct1 438h
%define rct2 439h
%define rct3 43Ah
%define rct4 43Bh
%define ico1 43Ch
%define ico2 43Dh
%define ico3 43Eh
%define ico4 43Fh
%define stc1 440h
%define stc2 441h
%define stc3 442h
%define stc4 443h
%define stc5 444h
%define stc6 445h
%define stc7 446h
%define stc8 447h
%define stc9 448h
%define stc10 449h
%define stc11 44Ah
%define stc12 44Bh
%define stc13 44Ch
%define stc14 44Dh
%define stc15 44Eh
%define stc16 44Fh
%define stc17 450h
%define stc18 451h
%define stc19 452h
%define stc20 453h
%define stc21 454h
%define stc22 455h
%define stc23 456h
%define stc24 457h
%define stc25 458h
%define stc26 459h
%define stc27 45Ah
%define stc28 45Bh
%define stc29 45Ch
%define stc30 45Dh
%define stc31 45Eh
%define stc32 45Fh
%define lst1 460h
%define lst2 461h
%define lst3 462h
%define lst4 463h
%define lst5 464h
%define lst6 465h
%define lst7 466h
%define lst8 467h
%define lst9 468h
%define lst10 469h
%define lst11 46Ah
%define lst12 46Bh
%define lst13 46Ch
%define lst14 46Dh
%define lst15 46Eh
%define lst16 46Fh
%define cmb1 470h
%define cmb2 471h
%define cmb3 472h
%define cmb4 473h
%define cmb5 474h
%define cmb6 475h
%define cmb7 476h
%define cmb8 477h
%define cmb9 478h
%define cmb10 479h
%define cmb11 47Ah
%define cmb12 47Bh
%define cmb13 47Ch
%define cmb14 47Dh
%define cmb15 47Eh
%define cmb16 47Fh
%define edt1 480h
%define edt2 481h
%define edt3 482h
%define edt4 483h
%define edt5 484h
%define edt6 485h
%define edt7 486h
%define edt8 487h
%define edt9 488h
%define edt10 489h
%define edt11 48Ah
%define edt12 48Bh
%define edt13 48Ch
%define edt14 48Dh
%define edt15 48Eh
%define edt16 48Fh
%define scr1 490h
%define scr2 491h
%define scr3 492h
%define scr4 493h
%define scr5 494h
%define scr6 495h
%define scr7 496h
%define scr8 497h
%define FILEOPENORD 1536
%define MULTIFILEOPENORD 1537
%define PRINTDLGORD 1538
%define PRNSETUPDLGORD 1539
%define FINDDLGORD 1540
%define REPLACEDLGORD 1541
%define FONTDLGORD 1542
%define FORMATDLGORD31 1543
%define FORMATDLGORD30 1544
%define HKEY_CLASSES_ROOT 80000000h
%define HKEY_CURRENT_USER 80000001h
%define HKEY_LOCAL_MACHINE 80000002h
%define HKEY_USERS 80000003h
%define HKEY_PERFORMANCE_DATA 80000004h
%define HKEY_CURRENT_CONFIG 80000005h
%define HKEY_DYN_DATA 80000006h
%define SERVICE_NO_CHANGE 0FFFFh
%define SERVICE_ACTIVE 1h
%define SERVICE_INACTIVE 2h
%define SERVICE_STATE_ALL SERVICE_ACTIVE|SERVICE_INACTIVE
%define SERVICE_CONTROL_STOP 1h
%define SERVICE_CONTROL_PAUSE 2h
%define SERVICE_CONTROL_CONTINUE 3h
%define SERVICE_CONTROL_INTERROGATE 4h
%define SERVICE_CONTROL_SHUTDOWN 5h
%define SERVICE_STOPPED 1h
%define SERVICE_START_PENDING 2h
%define SERVICE_STOP_PENDING 3h
%define SERVICE_RUNNING 4h
%define SERVICE_CONTINUE_PENDING 5h
%define SERVICE_PAUSE_PENDING 6h
%define SERVICE_PAUSED 7h
%define SERVICE_ACCEPT_STOP 1h
%define SERVICE_ACCEPT_PAUSE_CONTINUE 2h
%define SERVICE_ACCEPT_SHUTDOWN 4h
%define SC_MANAGER_CONNECT 1h
%define SC_MANAGER_CREATE_SERVICE 2h
%define SC_MANAGER_ENUMERATE_SERVICE 4h
%define SC_MANAGER_LOCK 8h
%define SC_MANAGER_QUERY_LOCK_STATUS 10h
%define SC_MANAGER_MODIFY_BOOT_CONFIG 20h
%define SC_MANAGER_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SC_MANAGER_CONNECT|SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE|SC_MANAGER_LOCK
%define SERVICE_QUERY_CONFIG 1h
%define SERVICE_CHANGE_CONFIG 2h
%define SERVICE_QUERY_STATUS 4h
%define SERVICE_ENUMERATE_DEPENDENTS 8h
%define SERVICE_START 10h
%define SERVICE_STOP 20h
%define SERVICE_PAUSE_CONTINUE 40h
%define SERVICE_INTERROGATE 80h
%define SERVICE_USER_DEFINED_CONTROL 100h
%define SERVICE_ALL_ACCESS STANDARD_RIGHTS_REQUIRED|SERVICE_QUERY_CONFIG|SERVICE_CHANGE_CONFIG|SERVICE_QUERY_STATUS
%define PERF_DATA_VERSION 1
%define PERF_DATA_REVISION 1
%define PERF_NO_INSTANCES -1
%define PERF_SIZE_DWORD 0h
%define PERF_SIZE_LARGE 100h
%define PERF_SIZE_ZERO 200h
%define PERF_SIZE_VARIABLE_LEN 300h
%define PERF_TYPE_NUMBER 0h
%define PERF_TYPE_COUNTER 400h
%define PERF_TYPE_TEXT 800h
%define PERF_TYPE_ZERO 0C00h
%define PERF_NUMBER_HEX 0h
%define PERF_NUMBER_DECIMAL 10000h
%define PERF_NUMBER_DEC_1000 20000h
%define PERF_COUNTER_VALUE 0h
%define PERF_COUNTER_RATE 10000h
%define PERF_COUNTER_FRACTION 20000h
%define PERF_COUNTER_BASE 30000h
%define PERF_COUNTER_ELAPSED 40000h
%define PERF_COUNTER_QUEUELEN 50000h
%define PERF_COUNTER_HISTOGRAM 60000h
%define PERF_TEXT_UNICODE 0h
%define PERF_TEXT_ASCII 10000h
%define PERF_TIMER_TICK 0h
%define PERF_TIMER_100NS 100000h
%define PERF_OBJECT_TIMER 200000h
%define PERF_DELTA_COUNTER 400000h
%define PERF_DELTA_BASE 800000h
%define PERF_INVERSE_COUNTER 1000000h
%define PERF_MULTI_COUNTER 2000000h
%define PERF_DISPLAY_NO_SUFFIX 0h
%define PERF_DISPLAY_PER_SEC 10000000h
%define PERF_DISPLAY_PERCENT 20000000h
%define PERF_DISPLAY_SECONDS 30000000h
%define PERF_DISPLAY_NOSHOW 40000000h
%define PERF_COUNTER_COUNTER PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_DISPLAY_PER_SEC
%define PERF_COUNTER_TIMER PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_COUNTER_QUEUELEN_TYPE PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_QUEUELEN|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_DISPLAY_NO_SUFFIX
%define PERF_COUNTER_BULK_COUNT PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_DISPLAY_PER_SEC
%define PERF_COUNTER_TEXT PERF_SIZE_VARIABLE_LEN|PERF_TYPE_TEXT|PERF_TEXT_UNICODE|PERF_DISPLAY_NO_SUFFIX
%define PERF_COUNTER_RAWCOUNT PERF_SIZE_DWORD|PERF_TYPE_NUMBER|PERF_NUMBER_DECIMAL|PERF_DISPLAY_NO_SUFFIX
%define PERF_SAMPLE_FRACTION PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_FRACTION|PERF_DELTA_COUNTER|PERF_DELTA_BASE|PERF_DISPLAY_PERCENT
%define PERF_SAMPLE_COUNTER PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_DISPLAY_NO_SUFFIX
%define PERF_COUNTER_NODATA PERF_SIZE_ZERO|PERF_DISPLAY_NOSHOW
%define PERF_COUNTER_TIMER_INV PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_TICK|PERF_DELTA_COUNTER|PERF_INVERSE_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_SAMPLE_BASE PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_BASE|PERF_DISPLAY_NOSHOW|1h
%define PERF_AVERAGE_TIMER PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_FRACTION|PERF_DISPLAY_SECONDS
%define PERF_AVERAGE_BASE PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_BASE|PERF_DISPLAY_NOSHOW|2h
%define PERF_AVERAGE_BULK PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_FRACTION|PERF_DISPLAY_NOSHOW
%define PERF_100NSEC_TIMER PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_100NS|PERF_DELTA_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_100NSEC_TIMER_INV PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_100NS|PERF_DELTA_COUNTER|PERF_INVERSE_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_COUNTER_MULTI_TIMER PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_DELTA_COUNTER|PERF_TIMER_TICK|PERF_MULTI_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_COUNTER_MULTI_TIMER_INV PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_RATE|PERF_DELTA_COUNTER|PERF_MULTI_COUNTER|PERF_TIMER_TICK|PERF_INVERSE_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_COUNTER_MULTI_BASE PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_BASE|PERF_MULTI_COUNTER|PERF_DISPLAY_NOSHOW
%define PERF_100NSEC_MULTI_TIMER PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_DELTA_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_100NS|PERF_MULTI_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_100NSEC_MULTI_TIMER_INV PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_DELTA_COUNTER|PERF_COUNTER_RATE|PERF_TIMER_100NS|PERF_MULTI_COUNTER|PERF_INVERSE_COUNTER|PERF_DISPLAY_PERCENT
%define PERF_RAW_FRACTION PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_FRACTION|PERF_DISPLAY_PERCENT
%define PERF_RAW_BASE PERF_SIZE_DWORD|PERF_TYPE_COUNTER|PERF_COUNTER_BASE|PERF_DISPLAY_NOSHOW|3h
%define PERF_ELAPSED_TIME PERF_SIZE_LARGE|PERF_TYPE_COUNTER|PERF_COUNTER_ELAPSED|PERF_OBJECT_TIMER|PERF_DISPLAY_SECONDS
%define PERF_COUNTER_HISTOGRAM_TYPE 80000000h
%define PERF_DETAIL_NOVICE 100
%define PERF_DETAIL_ADVANCED 200
%define PERF_DETAIL_EXPERT 300
%define PERF_DETAIL_WIZARD 400
%define PERF_NO_UNIQUE_ID -1
%define LZERROR_BADINHANDLE -1
%define LZERROR_BADOUTHANDLE -2
%define LZERROR_READ -3
%define LZERROR_WRITE -4
%define LZERROR_PUBLICLOC -5
%define LZERROR_GLOBLOCK -6
%define LZERROR_BADVALUE -7
%define LZERROR_UNKNOWNALG -8
%define VK_PROCESSKEY 0E5h
%define STYLE_DESCRIPTION_SIZE 32
%define WM_CONVERTREQUESTEX 108h
%define WM_IME_STARTCOMPOSITION 10Dh
%define WM_IME_ENDCOMPOSITION 10Eh
%define WM_IME_COMPOSITION 10Fh
%define WM_IME_KEYLAST 10Fh
%define WM_IME_SETCONTEXT 281h
%define WM_IME_NOTIFY 282h
%define WM_IME_CONTROL 283h
%define WM_IME_COMPOSITIONFULL 284h
%define WM_IME_SELECT 285h
%define WM_IME_CHAR 286h
%define WM_IME_KEYDOWN 290h
%define WM_IME_KEYUP 291h
%define IMC_GETCANDIDATEPOS 7h
%define IMC_SETCANDIDATEPOS 8h
%define IMC_GETCOMPOSITIONFONT 9h
%define IMC_SETCOMPOSITIONFONT 0Ah
%define IMC_GETCOMPOSITIONWINDOW 0Bh
%define IMC_SETCOMPOSITIONWINDOW 0Ch
%define IMC_GETSTATUSWINDOWPOS 0Fh
%define IMC_SETSTATUSWINDOWPOS 10h
%define IMC_CLOSESTATUSWINDOW 21h
%define IMC_OPENSTATUSWINDOW 22h
%define NI_OPENCANDIDATE 10h
%define NI_CLOSECANDIDATE 11h
%define NI_SELECTCANDIDATESTR 12h
%define NI_CHANGECANDIDATELIST 13h
%define NI_FINALIZECONVERSIONRESULT 14h
%define NI_COMPOSITIONSTR 15h
%define NI_SETCANDIDATE_PAGESTART 16h
%define NI_SETCANDIDATE_PAGESIZE 17h
%define ISC_SHOWUICANDIDATEWINDOW 1h
%define ISC_SHOWUICOMPOSITIONWINDOW 80000000h
%define ISC_SHOWUIGUIDELINE 40000000h
%define ISC_SHOWUIALLCANDIDATEWINDOW 0Fh
%define ISC_SHOWUIALL 0C000000Fh
%define CPS_COMPLETE 1h
%define CPS_CONVERT 2h
%define CPS_REVERT 3h
%define CPS_CANCEL 4h

;	IsTextUnicode
%assign	IS_TEXT_UNICODE_ASCII16			1
%assign	IS_TEXT_UNICODE_REVERSE_ASCII16		16
%assign	IS_TEXT_UNICODE_STATISTICS		2
%assign	IS_TEXT_UNICODE_REVERSE_STATISTICS	32
%assign	IS_TEXT_UNICODE_CONTROLS		4
%assign	IS_TEXT_UNICODE_REVERSE_CONTROLS	64
%assign	IS_TEXT_UNICODE_SIGNATURE		8
%assign	IS_TEXT_UNICODE_REVERSE_SIGNATURE	128
%assign	IS_TEXT_UNICODE_ILLEGAL_CHARS		256
%assign	IS_TEXT_UNICODE_ODD_LENGTH		512
%assign	IS_TEXT_UNICODE_NULL_BYTES		4096
%assign	IS_TEXT_UNICODE_UNICODE_MASK		15
%assign	IS_TEXT_UNICODE_REVERSE_MASK		240
%assign	IS_TEXT_UNICODE_NOT_UNICODE_MASK	3840
%assign	IS_TEXT_UNICODE_NOT_ASCII_MASK		61440

%define IME_CHOTKEY_IME_NONIME_TOGGLE 10h
%define IME_CHOTKEY_SHAPE_TOGGLE 11h
%define IME_CHOTKEY_SYMBOL_TOGGLE 12h
%define IME_JHOTKEY_CLOSE_OPEN 30h
%define IME_KHOTKEY_SHAPE_TOGGLE 50h
%define IME_KHOTKEY_HANJACONVERT 51h
%define IME_KHOTKEY_ENGLISH 52h
%define IME_THOTKEY_IME_NONIME_TOGGLE 70h
%define IME_THOTKEY_SHAPE_TOGGLE 71h
%define IME_THOTKEY_SYMBOL_TOGGLE 72h
%define IME_HOTKEY_DSWITCH_FIRST 100h
%define IME_HOTKEY_DSWITCH_LAST 11Fh
%define IME_ITHOTKEY_RESEND_RESULTSTR 200h
%define IME_ITHOTKEY_PREVIOUS_COMPOSITION 201h
%define IME_ITHOTKEY_UISTYLE_TOGGLE 202h
%define GCS_COMPREADSTR 1h
%define GCS_COMPREADATTR 2h
%define GCS_COMPREADCLAUSE 4h
%define GCS_COMPSTR 8h
%define GCS_COMPATTR 10h
%define GCS_COMPCLAUSE 20h
%define GCS_CURSORPOS 80h
%define GCS_DELTASTART 100h
%define GCS_RESULTREADSTR 200h
%define GCS_RESULTREADCLAUSE 400h
%define GCS_RESULTSTR 800h
%define GCS_RESULTCLAUSE 1000h
%define CS_INSERTCHAR 2000h
%define CS_NOMOVECARET 4000h
%define IME_PROP_AT_CARET 10000h
%define IME_PROP_SPECIAL_UI 20000h
%define IME_PROP_CANDLIST_START_FROM_1 40000h
%define IME_PROP_UNICODE 80000h
%define UI_CAP_2700 1h
%define UI_CAP_ROT90 2h
%define UI_CAP_ROTANY 4h
%define SCS_CAP_COMPSTR 1h
%define SCS_CAP_MAKEREAD 2h
%define SELECT_CAP_CONVERSION 1h
%define SELECT_CAP_SENTENCE 2h
%define GGL_LEVEL 1h
%define GGL_INDEX 2h
%define GGL_STRING 3h
%define GGL_PRIVATE 4h
%define GL_LEVEL_NOGUIDELINE 0h
%define GL_LEVEL_FATAL 1h
%define GL_LEVEL_ERROR 2h
%define GL_LEVEL_WARNING 3h
%define GL_LEVEL_INFORMATION 4h
%define GL_ID_UNKNOWN 0h
%define GL_ID_NOMODULE 1h
%define GL_ID_NODICTIONARY 10h
%define GL_ID_CANNOTSAVE 11h
%define GL_ID_NOCONVERT 20h
%define GL_ID_TYPINGERROR 21h
%define GL_ID_TOOMANYSTROKE 22h
%define GL_ID_READINGCONFLICT 23h
%define GL_ID_INPUTREADING 24h
%define GL_ID_INPUTRADICAL 25h
%define GL_ID_INPUTCODE 26h
%define GL_ID_INPUTSYMBOL 27h
%define GL_ID_CHOOSECANDIDATE 28h
%define GL_ID_REVERSECONVERSION 29h
%define GL_ID_PRIVATE_FIRST 8000h
%define GL_ID_PRIVATE_LAST 0FFFFh
%define IGP_PROPERTY 4h
%define IGP_CONVERSION 8h
%define IGP_SENTENCE 0Ch
%define IGP_UI 10h
%define IGP_SETCOMPSTR 14h
%define IGP_SELECT 18h
%define SCS_SETSTR GCS_COMPREADSTR|GCS_COMPSTR
%define SCS_CHANGEATTR GCS_COMPREADATTR|GCS_COMPATTR
%define SCS_CHANGECLAUSE GCS_COMPREADCLAUSE|GCS_COMPCLAUSE
%define ATTR_INPUT 0h
%define ATTR_TARGET_CONVERTED 1h
%define ATTR_CONVERTED 2h
%define ATTR_TARGET_NOTCONVERTED 3h
%define ATTR_INPUT_ERROR 4h
%define CFS_DEFAULT 0h
%define CFS_RECT 1h
%define CFS_POINT 2h
%define CFS_SCREEN 4h
%define CFS_FORCE_POSITION 20h
%define CFS_CANDIDATEPOS 40h
%define CFS_EXCLUDE 80h
%define GCL_CONVERSION 1h
%define GCL_REVERSECONVERSION 2h
%define GCL_REVERSE_LENGTH 3h
%define IME_CMODE_ALPHANUMERIC 0h
%define IME_CMODE_NATIVE 1h
%define IME_CMODE_CHINESE IME_CMODE_NATIVE
%define IME_CMODE_HANGEUL IME_CMODE_NATIVE
%define IME_CMODE_JAPANESE IME_CMODE_NATIVE
%define IME_CMODE_KATAKANA 2h
%define IME_CMODE_LANGUAGE 3h
%define IME_CMODE_FULLSHAPE 8h
%define IME_CMODE_ROMAN 10h
%define IME_CMODE_CHARCODE 20h
%define IME_CMODE_HANJACONVERT 40h
%define IME_CMODE_SOFTKBD 80h
%define IME_CMODE_NOCONVERSION 100h
%define IME_CMODE_EUDC 200h
%define IME_CMODE_SYMBOL 400h
%define IME_SMODE_NONE 0h
%define IME_SMODE_PLAURALCLAUSE 1h
%define IME_SMODE_SINGLECONVERT 2h
%define IME_SMODE_AUTOMATIC 4h
%define IME_SMODE_PHRASEPREDICT 8h
%define IME_CAND_UNKNOWN 0h
%define IME_CAND_READ 1h
%define IME_CAND_CODE 2h
%define IME_CAND_MEANING 3h
%define IME_CAND_RADICAL 4h
%define IME_CAND_STROKE 5h
%define IMN_CLOSESTATUSWINDOW 1h
%define IMN_OPENSTATUSWINDOW 2h
%define IMN_CHANGECANDIDATE 3h
%define IMN_CLOSECANDIDATE 4h
%define IMN_OPENCANDIDATE 5h
%define IMN_SETCONVERSIONMODE 6h
%define IMN_SETSENTENCEMODE 7h
%define IMN_SETOPENSTATUS 8h
%define IMN_SETCANDIDATEPOS 9h
%define IMN_SETCOMPOSITIONFONT 0Ah
%define IMN_SETCOMPOSITIONWINDOW 0Bh
%define IMN_SETSTATUSWINDOWPOS 0Ch
%define IMN_GUIDELINE 0Dh
%define IMN_PRIVATE 0Eh
%define IMM_ERROR_NODATA -1
%define IMM_ERROR_GENERAL -2
%define IME_CONFIG_GENERAL 1
%define IME_CONFIG_REGISTERWORD 2
%define IME_CONFIG_SELECTDICTIONARY 3
%define IME_ESC_QUERY_SUPPORT 3h
%define IME_ESC_RESERVED_FIRST 4h
%define IME_ESC_RESERVED_LAST 7FFh
%define IME_ESC_PRIVATE_FIRST 800h
%define IME_ESC_PRIVATE_LAST 0FFFh
%define IME_ESC_SEQUENCE_TO_INTERNAL 1001h
%define IME_ESC_GET_EUDC_DICTIONARY 1003h
%define IME_ESC_SET_EUDC_DICTIONARY 1004h
%define IME_ESC_MAX_KEY 1005h
%define IME_ESC_IME_NAME 1006h
%define IME_ESC_SYNC_HOTKEY 1007h
%define IME_ESC_HANJA_MODE 1008h
%define IME_REGWORD_STYLE_EUDC 1h
%define IME_REGWORD_STYLE_USER_FIRST 80000000h
%define IME_REGWORD_STYLE_USER_LAST 0FFFFh
%define SOFTKEYBOARD_TYPE_T1 1h
%define SOFTKEYBOARD_TYPE_C1 2h
%define DIALOPTION_BILLING 40h
%define DIALOPTION_QUIET 80h
%define DIALOPTION_DIALTONE 100h
%define MDMVOLFLAG_LOW 1h
%define MDMVOLFLAG_MEDIUM 2h
%define MDMVOLFLAG_HIGH 4h
%define MDMVOL_LOW 0h
%define MDMVOL_MEDIUM 1h
%define MDMVOL_HIGH 2h
%define MDMSPKRFLAG_OFF 1h
%define MDMSPKRFLAG_DIAL 2h
%define MDMSPKRFLAG_ON 4h
%define MDMSPKRFLAG_CALLSETUP 8h
%define MDMSPKR_OFF 0h
%define MDMSPKR_DIAL 1h
%define MDMSPKR_ON 2h
%define MDMSPKR_CALLSETUP 3h
%define MDM_COMPRESSION 1h
%define MDM_ERROR_CONTROL 2h
%define MDM_FORCED_EC 4h
%define MDM_CELLULAR 8h
%define MDM_FLOWCONTROL_HARD 10h
%define MDM_FLOWCONTROL_SOFT 20h
%define MDM_CCITT_OVERRIDE 40h
%define MDM_SPEED_ADJUST 80h
%define MDM_TONE_DIAL 100h
%define MDM_BLIND_DIAL 200h
%define MDM_V23_OVERRIDE 400h
%define ABM_NEW 0h
%define ABM_REMOVE 1h
%define ABM_QUERYPOS 2h
%define ABM_SETPOS 3h
%define ABM_GETSTATE 4h
%define ABM_GETTASKBARPOS 5h
%define ABM_ACTIVATE 6h
%define ABM_GETAUTOHIDEBAR 7h
%define ABM_SETAUTOHIDEBAR 8h
%define ABM_WINDOWPOSCHANGED 9h
%define ABN_STATECHANGE 0h
%define ABN_POSCHANGED 1h
%define ABN_FULLSCREENAPP 2h
%define ABN_WINDOWARRANGE 3h
%define ABS_AUTOHIDE 1h
%define ABS_ALWAYSONTOP 2h
%define ABE_LEFT 0
%define ABE_TOP 1
%define ABE_RIGHT 2
%define ABE_BOTTOM 3
%define EIRESID -1
%define FO_MOVE 1h
%define FO_COPY 2h
%define FO_DELETE 3h
%define FO_RENAME 4h
%define FOF_MULTIDESTFILES 1h
%define FOF_CONFIRMMOUSE 2h
%define FOF_SILENT 4h
%define FOF_RENAMEONCOLLISION 8h
%define FOF_NOCONFIRMATION 10h
%define FOF_WANTMAPPINGHANDLE 20h
%define FOF_ALLOWUNDO 40h
%define FOF_FILESONLY 80h
%define FOF_SIMPLEPROGRESS 100h
%define FOF_NOCONFIRMMKDIR 200h
%define PO_DELETE 13h
%define PO_RENAME 14h
%define PO_PORTCHANGE 20h
%define PO_REN_PORT 34h
%define SE_ERR_FNF 2
%define SE_ERR_PNF 3
%define SE_ERR_ACCESSDENIED 5
%define SE_ERR_OOM 8
%define SE_ERR_DLLNOTFOUND 32
%define SEE_MASK_CLASSNAME 1h
%define SEE_MASK_CLASSKEY 3h
%define SEE_MASK_IDLIST 4h
%define SEE_MASK_INVOKEIDLIST 0Ch
%define SEE_MASK_ICON 10h
%define SEE_MASK_HOTKEY 20h
%define SEE_MASK_NOCLOSEPROCESS 40h
%define SEE_MASK_CONNECTNETDRV 80h
%define SEE_MASK_FLAG_DDEWAIT 100h
%define SEE_MASK_DOENVSUBST 200h
%define SEE_MASK_FLAG_NO_UI 400h
%define NIM_ADD 0h
%define NIM_MODIFY 1h
%define NIM_DELETE 2h
%define NIF_MESSAGE 1h
%define NIF_ICON 2h
%define NIF_TIP 4h
%define SHGFI_ICON 100h
%define SHGFI_DISPLAYNAME 200h
%define SHGFI_TYPENAME 400h
%define SHGFI_ATTRIBUTES 800h
%define SHGFI_ICONLOCATION 1000h
%define SHGFI_EXETYPE 2000h
%define SHGFI_SYSICONINDEX 4000h
%define SHGFI_LINKOVERLAY 8000h
%define SHGFI_SELECTED 10000h
%define SHGFI_LARGEICON 0h
%define SHGFI_SMALLICON 1h
%define SHGFI_OPENICON 2h
%define SHGFI_SHELLICONSIZE 4h
%define SHGFI_PIDL 8h
%define SHGFI_USEFILEATTRIBUTES 10h
%define SHGNLI_PIDL 1h
%define SHGNLI_PREFIXNAME 2h
%define VS_VERSION_INFO 1
%define VS_USER_DEFINED 100
%define VS_FFI_SIGNATURE 0FEEF04BDh
%define VS_FFI_STRUCVERSION 10000h
%define VS_FFI_FILEFLAGSMASK 3Fh
%define VS_FF_DEBUG 1h
%define VS_FF_PRERELEASE 2h
%define VS_FF_PATCHED 4h
%define VS_FF_PRIVATEBUILD 8h
%define VS_FF_INFOINFERRED 10h
%define VS_FF_SPECIALBUILD 20h
%define VOS_UNKNOWN 0h
%define VOS_DOS 10000h
%define VOS_OS216 20000h
%define VOS_OS232 30000h
%define VOS_NT 40000h
%define VOS__BASE 0h
%define VOS__WINDOWS16 1h
%define VOS__PM16 2h
%define VOS__PM32 3h
%define VOS__WINDOWS32 4h
%define VOS_DOS_WINDOWS16 10001h
%define VOS_DOS_WINDOWS32 10004h
%define VOS_OS216_PM16 20002h
%define VOS_OS232_PM32 30003h
%define VOS_NT_WINDOWS32 40004h
%define VFT_UNKNOWN 0h
%define VFT_APP 1h
%define VFT_DLL 2h
%define VFT_DRV 3h
%define VFT_FONT 4h
%define VFT_VXD 5h
%define VFT_STATIC_LIB 7h
%define VFT2_UNKNOWN 0h
%define VFT2_DRV_PRINTER 1h
%define VFT2_DRV_KEYBOARD 2h
%define VFT2_DRV_LANGUAGE 3h
%define VFT2_DRV_DISPLAY 4h
%define VFT2_DRV_MOUSE 5h
%define VFT2_DRV_NETWORK 6h
%define VFT2_DRV_SYSTEM 7h
%define VFT2_DRV_INSTALLABLE 8h
%define VFT2_DRV_SOUND 9h
%define VFT2_DRV_COMM 0Ah
%define VFT2_DRV_INPUTMETHOD 0Bh
%define VFT2_FONT_RASTER 1h
%define VFT2_FONT_VECTOR 2h
%define VFT2_FONT_TRUETYPE 3h
%define VFFF_ISSHAREDFILE 1h
%define VFF_CURNEDEST 1h
%define VFF_FILEINUSE 2h
%define VFF_BUFFTOOSMALL 4h
%define VIFF_FORCEINSTALL 1h
%define VIFF_DONTDELETEOLD 2h
%define VIF_TEMPFILE 1h
%define VIF_MISMATCH 2h
%define VIF_SRCOLD 4h
%define VIF_DIFFLANG 8h
%define VIF_DIFFCODEPG 10h
%define VIF_DIFFTYPE 20h
%define VIF_WRITEPROT 40h
%define VIF_FILEINUSE 80h
%define VIF_OUTOFSPACE 100h
%define VIF_ACCESSVIOLATION 200h
%define VIF_SHARINGVIOLATION 400h
%define VIF_CANNOTCREATE 800h
%define VIF_CANNOTDELETE 1000h
%define VIF_CANNOTRENAME 2000h
%define VIF_CANNOTDELETECUR 4000h
%define VIF_OUTOFMEMORY 8000h
%define VIF_CANNOTREADSRC 10000h
%define VIF_CANNOTREADDST 20000h
%define VIF_BUFFTOOSMALL 40000h
%define PROCESS_HEAP_REGION 1h
%define PROCESS_HEAP_UNCOMMITTED_RANGE 2h
%define PROCESS_HEAP_ENTRY_BUSY 4h
%define PROCESS_HEAP_ENTRY_MOVEABLE 10h
%define PROCESS_HEAP_ENTRY_DDESHARE 20h
%define SCS_32BIT_BINARY 0
%define SCS_DOS_BINARY 1
%define SCS_WOW_BINARY 2
%define SCS_PIF_BINARY 3
%define SCS_POSIX_BINARY 4
%define SCS_OS216_BINARY 5
%define LOGON32_LOGON_INTERACTIVE 2
%define LOGON32_LOGON_BATCH 4
%define LOGON32_LOGON_SERVICE 5
%define LOGON32_PROVIDER_DEFAULT 0
%define LOGON32_PROVIDER_WINNT35 1
%define VER_PLATFORM_WIN32s 0
%define VER_PLATFORM_WIN32_WINDOWS 1
%define VER_PLATFORM_WIN32_NT 2
%define AC_LINE_OFFLINE 0h
%define AC_LINE_ONLINE 1h
%define AC_LINE_BACKUP_POWER 2h
%define AC_LINE_UNKNOWN 0FFh
%define BATTERY_FLAG_HIGH 1h
%define BATTERY_FLAG_LOW 2h
%define BATTERY_FLAG_CRITICAL 4h
%define BATTERY_FLAG_CHARGING 8h
%define BATTERY_FLAG_NO_BATTERY 80h
%define BATTERY_FLAG_UNKNOWN 0FFh
%define BATTERY_PERCENTAGE_UNKNOWN 0FFh
%define BATTERY_LIFE_UNKNOWN 0FFFFh
%define CDM_FIRST WM_USER+100
%define CDM_LAST WM_USER+200
%define CDM_GETSPEC CDM_FIRST+0h
%define CDM_GETFILEPATH CDM_FIRST+1h
%define CDM_GETFOLDERPATH CDM_FIRST+2h
%define CDM_GETFOLDERIDLIST CDM_FIRST+3h
%define CDM_SETCONTROLTEXT CDM_FIRST+4h
%define CDM_HIDECONTROL CDM_FIRST+5h
%define CDM_SETDEFEXT CDM_FIRST+6h
%define SIMULATED_FONTTYPE 8000h
%define PRINTER_FONTTYPE 4000h
%define SCREEN_FONTTYPE 2000h
%define BOLD_FONTTYPE 100h
%define ITALIC_FONTTYPE 200h
%define REGULAR_FONTTYPE 400h
%define WM_PSD_PAGESETUPDLG WM_USER
%define WM_PSD_FULLPAGERECT WM_USER+1
%define WM_PSD_MINMARGINRECT WM_USER+2
%define WM_PSD_MARGINRECT WM_USER+3
%define WM_PSD_GREEKTEXTRECT WM_USER+4
%define WM_PSD_ENVSTAMPRECT WM_USER+5
%define WM_PSD_YAFULLPAGERECT WM_USER+6
%define PSD_DEFAULTMINMARGINS 0h
%define PSD_INWININIINTLMEASURE 0h
%define PSD_MINMARGINS 1h
%define PSD_MARGINS 2h
%define PSD_INTHOUSANDTHSOFINCHES 4h
%define PSD_INHUNDREDTHSOFMILLIMETERS 8h
%define PSD_DISABLEMARGINS 10h
%define PSD_DISABLEPRINTER 20h
%define PSD_NOWARNING 80h
%define PSD_DISABLEORIENTATION 100h
%define PSD_RETURNDEFAULT 400h
%define PSD_DISABLEPAPER 200h
%define PSD_SHOWHELP 800h
%define PSD_ENABLEPAGESETUPHOOK 2000h
%define PSD_ENABLEPAGESETUPTEMPLATE 8000h
%define PSD_ENABLEPAGESETUPTEMPLATEHANDLE 20000h
%define PSD_ENABLEPAGEPAINTHOOK 40000h
%define PSD_DISABLEPAGEPAINTING 80000h
%define NM_FIRST 0-0
%define NM_LAST 0-99
%define DBG_CONTINUE 00010002h
%define DBG_TERMINATE_THREAD 40010003h
%define DBG_TERMINATE_PROCESS 40010004h
%define DBG_CONTROL_C 40010005h
%define DBG_CONTROL_BREAK 40010008h
%define DBG_EXCEPTION_NOT_HANDLED 80010001h
%define SIZE_OF_80387_REGISTERS 80
%define STATUS_WAIT_0 00000000h
%define STATUS_ABANDONED_WAIT_0 00000080h
%define STATUS_USER_APC 000000C0h
%define STATUS_TIMEOUT 00000102h
%define STATUS_PENDING 00000103h
%define STATUS_DATATYPE_MISALIGNMENT 80000002h
%define STATUS_BREAKPOINT 80000003h
%define STATUS_SINGLE_STEP 80000004h
%define STATUS_ACCESS_VIOLATION 0C0000005h
%define STATUS_IN_PAGE_ERROR 0C0000006h
%define STATUS_NO_MEMORY 0C0000017h
%define STATUS_ILLEGAL_INSTRUCTION 0C000001Dh
%define STATUS_NONCONTINUABLE_EXCEPTION 0C0000025h
%define STATUS_INVALID_DISPOSITION 0C0000026h
%define STATUS_ARRAY_BOUNDS_EXCEEDED 0C000008Ch
%define STATUS_FLOAT_DENORMAL_OPERAND 0C000008Dh
%define STATUS_FLOAT_DIVIDE_BY_ZERO 0C000008Eh
%define STATUS_FLOAT_INEXACT_RESULT 0C000008Fh
%define STATUS_FLOAT_INVALID_OPERATION 0C0000090h
%define STATUS_FLOAT_OVERFLOW 0C0000091h
%define STATUS_FLOAT_STACK_CHECK 0C0000092h
%define STATUS_FLOAT_UNDERFLOW 0C0000093h
%define STATUS_INTEGER_DIVIDE_BY_ZERO 0C0000094h
%define STATUS_INTEGER_OVERFLOW 0C0000095h
%define STATUS_PRIVILEGED_INSTRUCTION 0C0000096h
%define STATUS_STACK_OVERFLOW 0C00000FDh
%define STATUS_CONTROL_C_EXIT 0C000013Ah
%define EXCEPTION_CONTINUABLE 0
%define EXCEPTION_NONCONTINUABLE 1h
%define EXCEPTION_ACCESS_VIOLATION STATUS_ACCESS_VIOLATION
%define EXCEPTION_DATATYPE_MISALIGNMENT STATUS_DATATYPE_MISALIGNMENT
%define EXCEPTION_BREAKPOINT STATUS_BREAKPOINT
%define EXCEPTION_SINGLE_STEP STATUS_SINGLE_STEP
%define EXCEPTION_ARRAY_BOUNDS_EXCEEDED STATUS_ARRAY_BOUNDS_EXCEEDED
%define EXCEPTION_FLT_DENORMAL_OPERAND STATUS_FLOAT_DENORMAL_OPERAND
%define EXCEPTION_FLT_DIVIDE_BY_ZERO STATUS_FLOAT_DIVIDE_BY_ZERO
%define EXCEPTION_FLT_INEXACT_RESULT STATUS_FLOAT_INEXACT_RESULT
%define EXCEPTION_FLT_INVALID_OPERATION STATUS_FLOAT_INVALID_OPERATION
%define EXCEPTION_FLT_OVERFLOW STATUS_FLOAT_OVERFLOW
%define EXCEPTION_FLT_STACK_CHECK STATUS_FLOAT_STACK_CHECK
%define EXCEPTION_FLT_UNDERFLOW STATUS_FLOAT_UNDERFLOW
%define EXCEPTION_INT_DIVIDE_BY_ZERO STATUS_INTEGER_DIVIDE_BY_ZERO
%define EXCEPTION_INT_OVERFLOW STATUS_INTEGER_OVERFLOW
%define EXCEPTION_PRIV_INSTRUCTION STATUS_PRIVILEGED_INSTRUCTION
%define EXCEPTION_IN_PAGE_ERROR STATUS_IN_PAGE_ERROR
%define CONTEXT_i386 00010000h
%define CONTEXT_i486 00010000h
%define CONTEXT_CONTROL CONTEXT_i386|00000001h
%define CONTEXT_INTEGER CONTEXT_i386|00000002h
%define CONTEXT_SEGMENTS CONTEXT_i386|00000004h
%define CONTEXT_FLOATING_POINT CONTEXT_i386|00000008h
%define CONTEXT_DEBUG_REGISTERS CONTEXT_i386|00000010h
%define CONTEXT_FULL CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_SEGMENTS
%define IMAGE_DIRECTORY_ENTRY_EXPORT 0
%define IMAGE_DIRECTORY_ENTRY_IMPORT 1
%define IMAGE_DIRECTORY_ENTRY_RESOURCE 2
%define IMAGE_DIRECTORY_ENTRY_EXCEPTION 3
%define IMAGE_DIRECTORY_ENTRY_SECURITY 4
%define IMAGE_DIRECTORY_ENTRY_BASERELOC 5
%define IMAGE_DIRECTORY_ENTRY_DEBUG 6
%define IMAGE_DIRECTORY_ENTRY_COPYRIGHT 7
%define IMAGE_DIRECTORY_ENTRY_GLOBALPTR 8
%define IMAGE_DIRECTORY_ENTRY_TLS 9
%define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG 10
%define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT 11
%define IMAGE_DIRECTORY_ENTRY_IAT 12
%define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
%define IMAGE_BITMAP 0
%define IMAGE_ICON 1
%define IMAGE_CURSOR 2
%define IMAGE_ENHMETAFILE 3
%define PROCESSOR_INTEL_386 386
%define PROCESSOR_INTEL_486 486
%define PROCESSOR_INTEL_PENTIUM 586
%define PROCESSOR_MIPS_R4000 4000
%define PROCESSOR_ALPHA_21064 21064





;---------------------------comctl equates-------------------------------
%define ODT_HEADER 100
%define ODT_TAB 101
%define ODT_LISTVIEW 102
%define LVM_FIRST 1000h
%define TV_FIRST 1100h
%define HDM_FIRST 1200h
%define NM_OUTOFMEMORY NM_FIRST-1
%define NM_CLICK NM_FIRST-2
%define NM_DBLCLK NM_FIRST-3
%define NM_RETURN NM_FIRST-4
%define NM_RCLICK NM_FIRST-5
%define NM_RDBLCLK NM_FIRST-6
%define NM_SETFOCUS NM_FIRST-7
%define NM_KILLFOCUS NM_FIRST-8
%define CCS_TOP 00000001h
%define CCS_NOMOVEY 00000002h
%define CCS_BOTTOM 00000003h
%define CCS_NORESIZE 00000004h
%define CCS_NOPARENTALIGN 00000008h
%define CCS_ADJUSTABLE 00000020h
%define CCS_NODIVIDER 00000040h
%define CCM_FIRST 2000h
%define CCM_SETBKCOLOR CCM_FIRST+1
%define CCM_SETCOLORSCHEME CCM_FIRST+2
%define CCM_GETCOLORSCHEME CCM_FIRST+3
%define CCM_GETDROPTARGET CCM_FIRST+4
%define CCM_SETUNICODEFORMAT CCM_FIRST+5
%define CCM_GETUNICODEFORMAT CCM_FIRST+6
%define LVN_FIRST 0-100
%define LVN_LAST 0-199
%define HDN_FIRST 0-300
%define HDN_LAST 0-399
%define TVN_FIRST 0-400
%define TVN_LAST 0-499
%define TTN_FIRST 0-520
%define TTN_LAST 0-549
%define TCN_FIRST 0-550
%define TCN_LAST 0-580
%define CDN_FIRST 0-601
%define CDN_LAST 0-699
%define TBN_FIRST 0-700
%define TBN_LAST 0-720
%define UDN_FIRST 0-721
%define UDN_LAST 0-740
%define MCN_FIRST 0-750
%define MCN_LAST 0-759
%define DTN_FIRST 0-760
%define DTN_LAST 0-799
%define CBEN_FIRST 0-800
%define CBEN_LAST 0-830
%define RBN_FIRST 0-831
%define RBN_LAST 0-859
%define IPN_FIRST 0-860
%define IPN_LAST 0-879
%define SBN_FIRST 0-880
%define SBN_LAST 0-899
%define PGN_FIRST 0-900
%define PGN_LAST 0-950
%define MSGF_COMMCTRL_BEGINDRAG 4200h
%define MSGF_COMMCTRL_SIZEHEADER 4201h
%define MSGF_COMMCTRL_DRAGSELECT 4202h
%define MSGF_COMMCTRL_TOOLBARCUST 4203h
%define ICC_LISTVIEW_CLASSES 00000001h
%define ICC_TREEVIEW_CLASSES 00000002h
%define ICC_BAR_CLASSES 00000004h
%define ICC_TAB_CLASSES 00000008h
%define ICC_UPDOWN_CLASS 00000010h
%define ICC_PROGRESS_CLASS 00000020h
%define ICC_HOTKEY_CLASS 00000040h
%define ICC_ANIMATE_CLASS 00000080h
%define ICC_WIN95_CLASSES 000000FFh
%define ICC_DATE_CLASSES 00000100h
%define ICC_USEREX_CLASSES 00000200h
%define ICC_COOL_CLASSES 00000400h
%define ICC_INTERNET_CLASSES 00000800h
%define ICC_PAGESCROLLER_CLASS 00001000h
%define ICC_NATIVEFNTCTL_CLASS 00002000h
%define RBIM_IMAGELIST 00000001h
%define RBS_TOOLTIPS 0100h
%define RBS_VARHEIGHT 0200h
%define RBS_BANDBORDERS 0400h
%define RBS_FIXEDORDER 0800h
%define RBS_REGISTERDROP 1000h
%define RBS_AUTOSIZE 2000h
%define RBS_VERTICALGRIPPER 4000h
%define RBS_DBLCLKTOGGLE 8000h
%define RBBS_BREAK 00000001h
%define RBBS_FIXEDSIZE 00000002h
%define RBBS_CHILDEDGE 00000004h
%define RBBS_HIDDEN 00000008h
%define RBBS_NOVERT 00000010h
%define RBBS_FIXEDBMP 00000020h
%define RBBS_VARIABLEHEIGHT 00000040h
%define RBBS_GRIPPERALWAYS 00000080h
%define RBBS_NOGRIPPER 00000100h
%define RBBIM_STYLE 00000001h
%define RBBIM_COLORS 00000002h
%define RBBIM_TEXT 00000004h
%define RBBIM_IMAGE 00000008h
%define RBBIM_CHILD 00000010h
%define RBBIM_CHILDSIZE 00000020h
%define RBBIM_SIZE 00000040h
%define RBBIM_BACKGROUND 00000080h
%define RBBIM_ID 00000100h
%define RBBIM_IDEALSIZE 00000200h
%define RBBIM_LPARAM 00000400h
%define RBBIM_HEADERSIZE 00000800h
%define RB_INSERTBAND WM_USER+1
%define RB_DELETEBAND WM_USER+2
%define RB_GETBARINFO WM_USER+3
%define RB_SETBARINFO WM_USER+4
%define RB_GETBANDINFO WM_USER+5
%define RB_SETBANDINFO WM_USER+6
%define RB_SETPARENT WM_USER+7
%define RB_HITTEST WM_USER+8
%define RB_GETRECT WM_USER+9
%define RB_GETBANDCOUNT WM_USER+12
%define RB_GETROWCOUNT WM_USER+13
%define RB_GETROWHEIGHT WM_USER+14
%define RB_IDTOINDEX WM_USER+16
%define RB_GETTOOLTIPS WM_USER+17
%define RB_SETTOOLTIPS WM_USER+18
%define RB_SETBKCOLOR WM_USER+19
%define RB_GETBKCOLOR WM_USER+20
%define RB_SETTEXTCOLOR WM_USER+21
%define RB_GETTEXTCOLOR WM_USER+22
%define RB_SIZETORECT WM_USER+23
%define RB_SETCOLORSCHEME CCM_SETCOLORSCHEME
%define RB_GETCOLORSCHEME CCM_GETCOLORSCHEME
%define RB_BEGINDRAG WM_USER+24
%define RB_ENDDRAG WM_USER+25
%define RB_DRAGMOVE WM_USER+26
%define RB_GETBARHEIGHT WM_USER+27
%define RB_MINIMIZEBAND WM_USER+30
%define RB_MAXIMIZEBAND WM_USER+31
%define RB_GETDROPTARGET CCM_GETDROPTARGET
%define RB_GETBANDBORDERS WM_USER+34
%define RB_SHOWBAND WM_USER+35
%define RB_SETPALETTE WM_USER+37
%define RB_GETPALETTE WM_USER+38
%define RB_MOVEBAND WM_USER+39
%define RB_SETUNICODEFORMAT CCM_SETUNICODEFORMAT
%define RB_GETUNICODEFORMAT CCM_GETUNICODEFORMAT
%define RBN_HEIGHTCHANGE RBN_FIRST-0
%define RBN_GETOBJECT RBN_FIRST-1
%define RBN_LAYOUTCHANGED RBN_FIRST-2
%define RBN_AUTOSIZE RBN_FIRST-3
%define RBN_BEGINDRAG RBN_FIRST-4
%define RBN_ENDDRAG RBN_FIRST-5
%define RBN_DELETINGBAND RBN_FIRST-6
%define RBN_DELETEDBAND RBN_FIRST-7
%define RBN_CHILDSIZE RBN_FIRST-8
%define RBNM_ID 00000001h
%define RBNM_STYLE 00000002h
%define RBNM_LPARAM 00000004h
%define RBHT_NOWHERE 0001h
%define RBHT_CAPTION 0002h
%define RBHT_CLIENT 0003h
%define RBHT_GRABBER 0004h
%define CLR_NONE 0FFFFFFFFh
%define CLR_DEFAULT 0FF000000h
%define ILC_MASK 0001h
%define ILC_COLOR 0000h
%define ILC_COLORDDB 00FEh
%define ILC_COLOR4 0004h
%define ILC_COLOR8 0008h
%define ILC_COLOR16 0010h
%define ILC_COLOR24 0018h
%define ILC_COLOR32 0020h
%define ILC_PALETTE 0800h
%define ILD_NORMAL 0000h
%define ILD_TRANSPARENT 0001h
%define ILD_MASK 0010h
%define ILD_IMAGE 0020h
%define ILD_BLEND25 0002h
%define ILD_BLEND50 0004h
%define ILD_OVERLAYMASK 0F00h
%define ILD_SELECTED ILD_BLEND50
%define ILD_FOCUS ILD_BLEND25
%define ILD_BLEND ILD_BLEND50
%define CLR_HILIGHT CLR_DEFAULT
%define HDS_HORZ 00000000h
%define HDS_BUTTONS 00000002h
%define HDS_HIDDEN 00000008h
%define HDI_WIDTH 0001h
%define HDI_HEIGHT HDI_WIDTH
%define HDI_TEXT 0002h
%define HDI_FORMAT 0004h
%define HDI_LPARAM 0008h
%define HDI_BITMAP 0010h
%define HDF_LEFT 0
%define HDF_RIGHT 1
%define HDF_CENTER 2
%define HDF_JUSTIFYMASK 0003h
%define HDF_RTLREADING 4
%define HDF_OWNERDRAW 8000h
%define HDF_STRING 4000h
%define HDF_BITMAP 2000h
%define HDM_GETITEMCOUNT HDM_FIRST+0
%define HDM_INSERTITEM HDM_FIRST+1
%define HDM_INSERTITEMW HDM_FIRST+10
%define HDM_DELETEITEM HDM_FIRST+2
%define HDM_GETITEM HDM_FIRST+3
%define HDM_GETITEMW HDM_FIRST+11
%define HDM_SETITEM HDM_FIRST+4
%define HDM_SETITEMW HDM_FIRST+12
%define HDM_LAYOUT HDM_FIRST+5
%define HHT_NOWHERE 0001h
%define HHT_ONHEADER 0002h
%define HHT_ONDIVIDER 0004h
%define HHT_ONDIVOPEN 0008h
%define HHT_ABOVE 0100h
%define HHT_BELOW 0200h
%define HHT_TORIGHT 0400h
%define HHT_TOLEFT 0800h
%define HDM_HITTEST HDM_FIRST+6
%define HDN_ITEMCHANGING HDN_FIRST-0
%define HDN_ITEMCHANGINGW HDN_FIRST-20
%define HDN_ITEMCHANGED HDN_FIRST-1
%define HDN_ITEMCHANGEDW HDN_FIRST-21
%define HDN_ITEMCLICK HDN_FIRST-2
%define HDN_ITEMCLICKW HDN_FIRST-22
%define HDN_ITEMDBLCLICK HDN_FIRST-3
%define HDN_ITEMDBLCLICKW HDN_FIRST-23
%define HDN_DIVIDERDBLCLICK HDN_FIRST-5
%define HDN_DIVIDERDBLCLICKW HDN_FIRST-25
%define HDN_BEGINTRACK HDN_FIRST-6
%define HDN_BEGINTRACKW HDN_FIRST-26
%define HDN_ENDTRACK HDN_FIRST-7
%define HDN_ENDTRACKW HDN_FIRST-27
%define HDN_TRACK HDN_FIRST-8
%define HDN_TRACKW HDN_FIRST-28
%define CMB_MASKED 02h
%define TBSTATE_CHECKED 01h
%define TBSTATE_PRESSED 02h
%define TBSTATE_ENABLED 04h
%define TBSTATE_HIDDEN 08h
%define TBSTATE_INDETERMINATE 10h
%define TBSTATE_WRAP 20h
%define TBSTYLE_BUTTON 00h
%define TBSTYLE_SEP 01h
%define TBSTYLE_CHECK 02h
%define TBSTYLE_GROUP 04h
%define TBSTYLE_CHECKGROUP TBSTYLE_GROUP|TBSTYLE_CHECK
%define TBSTYLE_TOOLTIPS 0100h
%define TBSTYLE_WRAPABLE 0200h
%define TBSTYLE_ALTDRAG 0400h
%define TBSTYLE_FLAT 0800h
%define TBSTYLE_LIST 1000h
%define TBSTYLE_CUSTOMERASE 2000h
%define TBSTYLE_REGISTERDROP 4000h
%define TBSTYLE_TRANSPARENT 8000h
%define TB_ENABLEBUTTON WM_USER+1
%define TB_CHECKBUTTON WM_USER+2
%define TB_PRESSBUTTON WM_USER+3
%define TB_HIDEBUTTON WM_USER+4
%define TB_INDETERMINATE WM_USER+5
%define TB_ISBUTTONENABLED WM_USER+9
%define TB_ISBUTTONCHECKED WM_USER+10
%define TB_ISBUTTONPRESSED WM_USER+11
%define TB_ISBUTTONHIDDEN WM_USER+12
%define TB_ISBUTTONINDETERMINATE WM_USER+13
%define TB_SETSTATE WM_USER+17
%define TB_GETSTATE WM_USER+18
%define TB_ADDBITMAP WM_USER+19
%define TB_SETSTYLE WM_USER+56
%define TB_GETSTYLE WM_USER+57
%define HINST_COMMCTRL -1
%define IDB_STD_SMALL_COLOR 0
%define IDB_STD_LARGE_COLOR 1
%define IDB_VIEW_SMALL_COLOR 4
%define IDB_VIEW_LARGE_COLOR 5
%define STD_CUT 0
%define STD_COPY 1
%define STD_PASTE 2
%define STD_UNDO 3
%define STD_REDOW 4
%define STD_DELETE 5
%define STD_FILENEW 6
%define STD_FILEOPEN 7
%define STD_FILESAVE 8
%define STD_PRINTPRE 9
%define STD_PROPERTIES 10
%define STD_HELP 11
%define STD_FIND 12
%define STD_REPLACE 13
%define STD_PRINT 14
%define VIEW_LARGEICONS 0
%define VIEW_SMALLICONS 1
%define VIEW_LIST 2
%define VIEW_DETAILS 3
%define VIEW_SORTNAME 4
%define VIEW_SORTSIZE 5
%define VIEW_SORTDATE 6
%define VIEW_SORTTYPE 7
%define VIEW_PARENTFOLDER 8
%define VIEW_NETCONNECT 9
%define VIEW_NETDISCONNECT 10
%define VIEW_NEWFOLDER 11
%define TB_ADDBUTTONS WM_USER+20
%define TB_INSERTBUTTON WM_USER+21
%define TB_DELETEBUTTON WM_USER+22
%define TB_GETBUTTON WM_USER+23
%define TB_BUTTONCOUNT WM_USER+24
%define TB_COMMANDTOINDEX WM_USER+25
%define TB_SAVERESTORE WM_USER+26
%define TB_SAVERESTOREW WM_USER+76
%define TB_CUSTOMIZE WM_USER+27
%define TB_ADDSTRING WM_USER+28
%define TB_ADDSTRINGW WM_USER+77
%define TB_GETITEMRECT WM_USER+29
%define TB_BUTTONSTRUCTSIZE WM_USER+30
%define TB_SETBUTTONSIZE WM_USER+31
%define TB_SETBITMAPSIZE WM_USER+32
%define TB_AUTOSIZE WM_USER+33
%define TB_GETTOOLTIPS WM_USER+35
%define TB_SETTOOLTIPS WM_USER+36
%define TB_SETPARENT WM_USER+37
%define TB_SETROWS WM_USER+39
%define TB_GETROWS WM_USER+40
%define TB_SETCMDID WM_USER+42
%define TB_CHANGEBITMAP WM_USER+43
%define TB_GETBITMAP WM_USER+44
%define TB_GETBUTTONTEXT WM_USER+45
%define TB_GETBUTTONTEXTW WM_USER+75
%define TB_REPLACEBITMAP WM_USER+46
%define TBBF_LARGE 0001h
%define TB_GETBITMAPFLAGS WM_USER+41
%define TBN_GETBUTTONINFO TBN_FIRST-0
%define TBN_GETBUTTONINFOW TBN_FIRST-20
%define TBN_BEGINDRAG TBN_FIRST-1
%define TBN_ENDDRAG TBN_FIRST-2
%define TBN_BEGINADJUST TBN_FIRST-3
%define TBN_ENDADJUST TBN_FIRST-4
%define TBN_RESET TBN_FIRST-5
%define TBN_QUERYINSERT TBN_FIRST-6
%define TBN_QUERYDELETE TBN_FIRST-7
%define TBN_TOOLBARCHANGE TBN_FIRST-8
%define TBN_CUSTHELP TBN_FIRST-9
%define TTS_ALWAYSTIP 01h
%define TTS_NOPREFIX 02h
%define TTF_IDISHWND 01h
%define TTF_CENTERTIP 02h
%define TTF_RTLREADING 04h
%define TTF_SUBCLASS 10h
%define TTDT_AUTOMATIC 0
%define TTDT_RESHOW 1
%define TTDT_AUTOPOP 2
%define TTDT_INITIAL 3
%define TTM_ACTIVATE WM_USER+1
%define TTM_SETDELAYTIME WM_USER+3
%define TTM_ADDTOOL WM_USER+4
%define TTM_ADDTOOLW WM_USER+50
%define TTM_DELTOOL WM_USER+5
%define TTM_DELTOOLW WM_USER+51
%define TTM_NEWTOOLRECT WM_USER+6
%define TTM_NEWTOOLRECTW WM_USER+52
%define TTM_RELAYEVENT WM_USER+7
%define TTM_GETTOOLINFO WM_USER+8
%define TTM_GETTOOLINFOW WM_USER+53
%define TTM_SETTOOLINFO WM_USER+9
%define TTM_SETTOOLINFOW WM_USER+54
%define TTM_HITTEST WM_USER+10
%define TTM_HITTESTW WM_USER+55
%define TTM_GETTEXT WM_USER+11
%define TTM_GETTEXTW WM_USER+56
%define TTM_UPDATETIPTEXT WM_USER+12
%define TTM_UPDATETIPTEXTW WM_USER+57
%define TTM_GETTOOLCOUNT WM_USER+13
%define TTM_ENUMTOOLS WM_USER+14
%define TTM_ENUMTOOLSW WM_USER+58
%define TTM_GETCURRENTTOOL WM_USER+15
%define TTM_GETCURRENTTOOLW WM_USER+59
%define TTM_WINDOWFROMPOINT WM_USER+16
%define TTN_NEEDTEXT TTN_FIRST-0
%define TTN_NEEDTEXTW TTN_FIRST-10
%define TTN_SHOW TTN_FIRST-1
%define TTN_POP TTN_FIRST-2
%define SBARS_SIZEGRIP 0100h
%define SB_SETTEXT WM_USER+1
%define SB_SETTEXTW WM_USER+11
%define SB_GETTEXT WM_USER+2
%define SB_GETTEXTW WM_USER+13
%define SB_GETTEXTLENGTH WM_USER+3
%define SB_GETTEXTLENGTHW WM_USER+12
%define SB_SETPARTS WM_USER+4
%define SB_GETPARTS WM_USER+6
%define SB_GETBORDERS WM_USER+7
%define SB_SETMINHEIGHT WM_USER+8
%define SB_SIMPLE WM_USER+9
%define SB_GETRECT WM_USER+10
%define SBT_OWNERDRAW 1000h
%define SBT_NOBORDERS 0100h
%define SBT_POPOUT 0200h
%define SBT_RTLREADING 0400h
%define MINSYSCOMMAND SC_SIZE
%define TBS_AUTOTICKS 0001h
%define TBS_VERT 0002h
%define TBS_HORZ 0000h
%define TBS_TOP 0004h
%define TBS_BOTTOM 0000h
%define TBS_LEFT 0004h
%define TBS_RIGHT 0000h
%define TBS_BOTH 0008h
%define TBS_NOTICKS 0010h
%define TBS_ENABLESELRANGE 0020h
%define TBS_FIXEDLENGTH 0040h
%define TBS_NOTHUMB 0080h
%define TBM_GETPOS WM_USER
%define TBM_GETRANGEMIN WM_USER+1
%define TBM_GETRANGEMAX WM_USER+2
%define TBM_GETTIC WM_USER+3
%define TBM_SETTIC WM_USER+4
%define TBM_SETPOS WM_USER+5
%define TBM_SETRANGE WM_USER+6
%define TBM_SETRANGEMIN WM_USER+7
%define TBM_SETRANGEMAX WM_USER+8
%define TBM_CLEARTICS WM_USER+9
%define TBM_SETSEL WM_USER+10
%define TBM_SETSELSTART WM_USER+11
%define TBM_SETSELEND WM_USER+12
%define TBM_GETPTICS WM_USER+14
%define TBM_GETTICPOS WM_USER+15
%define TBM_GETNUMTICS WM_USER+16
%define TBM_GETSELSTART WM_USER+17
%define TBM_GETSELEND WM_USER+18
%define TBM_CLEARSEL WM_USER+19
%define TBM_SETTICFREQ WM_USER+20
%define TBM_SETPAGESIZE WM_USER+21
%define TBM_GETPAGESIZE WM_USER+22
%define TBM_SETLINESIZE WM_USER+23
%define TBM_GETLINESIZE WM_USER+24
%define TBM_GETTHUMBRECT WM_USER+25
%define TBM_GETCHANNELRECT WM_USER+26
%define TBM_SETTHUMBLENGTH WM_USER+27
%define TBM_GETTHUMBLENGTH WM_USER+28
%define TB_LINEUP 0
%define TB_LINEDOWN 1
%define TB_PAGEUP 2
%define TB_PAGEDOWN 3
%define TB_THUMBPOSITION 4
%define TB_THUMBTRACK 5
%define TB_TOP 6
%define TB_BOTTOM 7
%define TB_ENDTRACK 8
%define DL_BEGINDRAG WM_USER+133
%define DL_DRAGGING WM_USER+134
%define DL_DROPPED WM_USER+135
%define DL_CANCELDRAG WM_USER+136
%define DL_CURSORSET 0
%define DL_STOPCURSOR 1
%define DL_COPYCURSOR 2
%define DL_MOVECURSOR 3
%define UD_MAXVAL 7FFFh
%define UD_MINVAL -UD_MAXVAL
%define UDS_WRAP 0001h
%define UDS_SETBUDDYINT 0002h
%define UDS_ALIGNRIGHT 0004h
%define UDS_ALIGNLEFT 0008h
%define UDS_AUTOBUDDY 0010h
%define UDS_ARROWKEYS 0020h
%define UDS_HORZ 0040h
%define UDS_NOTHOUSANDS 0080h
%define UDM_SETRANGE WM_USER+101
%define UDM_GETRANGE WM_USER+102
%define UDM_SETPOS WM_USER+103
%define UDM_GETPOS WM_USER+104
%define UDM_SETBUDDY WM_USER+105
%define UDM_GETBUDDY WM_USER+106
%define UDM_SETACCEL WM_USER+107
%define UDM_GETACCEL WM_USER+108
%define UDM_SETBASE WM_USER+109
%define UDM_GETBASE WM_USER+110
%define UDN_DELTAPOS UDN_FIRST-1
%define PBM_SETRANGE WM_USER+1
%define PBM_SETPOS WM_USER+2
%define PBM_DELTAPOS WM_USER+3
%define PBM_SETSTEP WM_USER+4
%define PBM_STEPIT WM_USER+5
%define HOTKEYF_SHIFT 01h
%define HOTKEYF_CONTROL 02h
%define HOTKEYF_ALT 04h
%define HOTKEYF_EXT 08h
%define HKCOMB_NONE 0001h
%define HKCOMB_S 0002h
%define HKCOMB_C 0004h
%define HKCOMB_A 0008h
%define HKCOMB_SC 0010h
%define HKCOMB_SA 0020h
%define HKCOMB_CA 0040h
%define HKCOMB_SCA 0080h
%define HKM_SETHOTKEY WM_USER+1
%define HKM_GETHOTKEY WM_USER+2
%define HKM_SETRULES WM_USER+3
%define LVS_ICON 0000h
%define LVS_REPORT 0001h
%define LVS_SMALLICON 0002h
%define LVS_LIST 0003h
%define LVS_TYPEMASK 0003h
%define LVS_SINGLESEL 0004h
%define LVS_SHOWSELALWAYS 0008h
%define LVS_SORTASCENDING 0010h
%define LVS_SORTDESCENDING 0020h
%define LVS_SHAREIMAGELISTS 0040h
%define LVS_NOLABELWRAP 0080h
%define LVS_AUTOARRANGE 0100h
%define LVS_EDITLABELS 0200h
%define LVS_NOSCROLL 2000h
%define LVS_TYPESTYLEMASK 0fc00h
%define LVS_ALIGNTOP 0000h
%define LVS_ALIGNLEFT 0800h
%define LVS_ALIGNMASK 0c00h
%define LVS_OWNERDRAWFIXED 0400h
%define LVS_NOCOLUMNHEADER 4000h
%define LVS_NOSORTHEADER 8000h
%define LVM_GETBKCOLOR LVM_FIRST+0
%define LVM_SETBKCOLOR LVM_FIRST+1
%define LVM_GETIMAGELIST LVM_FIRST+2
%define LVSIL_NORMAL 0
%define LVSIL_SMALL 1
%define LVSIL_STATE 2
%define LVM_SETIMAGELIST LVM_FIRST+3
%define LVM_GETITEMCOUNT LVM_FIRST+4
%define LVIF_TEXT 0001h
%define LVIF_IMAGE 0002h
%define LVIF_PARAM 0004h
%define LVIF_STATE 0008h
%define LVIS_FOCUSED 0001h
%define LVIS_SELECTED 0002h
%define LVIS_CUT 0004h
%define LVIS_DROPHILITED 0008h
%define LVIS_OVERLAYMASK 0F00h
%define LVIS_STATEIMAGEMASK 0F000h
%define LPSTR_TEXTCALLBACKW -1
%define LPSTR_TEXTCALLBACK -1
%define I_IMAGECALLBACK -1
%define LVM_GETITEM LVM_FIRST+5
%define LVM_GETITEMW LVM_FIRST+75
%define LVM_SETITEM LVM_FIRST+6
%define LVM_SETITEMW LVM_FIRST+76
%define LVM_INSERTITEM LVM_FIRST+7
%define LVM_INSERTITEMW LVM_FIRST+77
%define LVM_DELETEITEM LVM_FIRST+8
%define LVM_DELETEALLITEMS LVM_FIRST+9
%define LVM_GETCALLBACKMASK LVM_FIRST+10
%define LVM_SETCALLBACKMASK LVM_FIRST+11
%define LVNI_ALL 0000h
%define LVNI_FOCUSED 0001h
%define LVNI_SELECTED 0002h
%define LVNI_CUT 0004h
%define LVNI_DROPHILITED 0008h
%define LVNI_ABOVE 0100h
%define LVNI_BELOW 0200h
%define LVNI_TOLEFT 0400h
%define LVNI_TORIGHT 0800h
%define LVM_GETNEXTITEM LVM_FIRST+12
%define LVFI_PARAM 0001h
%define LVFI_STRING 0002h
%define LVFI_PARTIAL 0008h
%define LVFI_WRAP 0020h
%define LVFI_NEARESTXY 0040h
%define LVM_FINDITEM LVM_FIRST+13
%define LVM_FINDITEMW LVM_FIRST+83
%define LVIR_BOUNDS 0
%define LVIR_ICON 1
%define LVIR_LABEL 2
%define LVIR_SELECTBOUNDS 3
%define LVM_GETITEMRECT LVM_FIRST+14
%define LVM_SETITEMPOSITION LVM_FIRST+15
%define LVM_GETITEMPOSITION LVM_FIRST+16
%define LVM_GETSTRINGWIDTH LVM_FIRST+17
%define LVM_GETSTRINGWIDTHW LVM_FIRST+87
%define LVHT_NOWHERE 0001h
%define LVHT_ONITEMICON 0002h
%define LVHT_ONITEMLABEL 0004h
%define LVHT_ONITEMSTATEICON 0008h
%define LVHT_ONITEM LVHT_ONITEMICON|LVHT_ONITEMLABEL|LVHT_ONITEMSTATEICON
%define LVHT_ABOVE 0008h
%define LVHT_BELOW 0010h
%define LVHT_TORIGHT 0020h
%define LVHT_TOLEFT 0040h
%define LVM_HITTEST LVM_FIRST+18
%define LVM_ENSUREVISIBLE LVM_FIRST+19
%define LVM_SCROLL LVM_FIRST+20
%define LVM_REDRAWITEMS LVM_FIRST+21
%define LVA_DEFAULT 0000h
%define LVA_ALIGNLEFT 0001h
%define LVA_ALIGNTOP 0002h
%define LVA_SNAPTOGRID 0005h
%define LVM_ARRANGE LVM_FIRST+22
%define LVM_EDITLABEL LVM_FIRST+23
%define LVM_EDITLABELW LVM_FIRST+118
%define LVM_GETEDITCONTROL LVM_FIRST+24
%define LVCF_FMT 0001h
%define LVCF_WIDTH 0002h
%define LVCF_TEXT 0004h
%define LVCF_SUBITEM 0008h
%define LVCFMT_LEFT 0000h
%define LVCFMT_RIGHT 0001h
%define LVCFMT_CENTER 0002h
%define LVCFMT_JUSTIFYMASK 0003h
%define LVM_GETCOLUMN LVM_FIRST+25
%define LVM_GETCOLUMNW LVM_FIRST+95
%define LVM_SETCOLUMN LVM_FIRST+26
%define LVM_SETCOLUMNW LVM_FIRST+96
%define LVM_INSERTCOLUMN LVM_FIRST+27
%define LVM_INSERTCOLUMNW LVM_FIRST+97
%define LVM_DELETECOLUMN LVM_FIRST+28
%define LVM_GETCOLUMNWIDTH LVM_FIRST+29
%define LVSCW_AUTOSIZE -1
%define LVSCW_AUTOSIZE_USEHEADER -2
%define LVM_SETCOLUMNWIDTH LVM_FIRST+30
%define LVM_CREATEDRAGIMAGE LVM_FIRST+33
%define LVM_GETVIEWRECT LVM_FIRST+34
%define LVM_GETTEXTCOLOR LVM_FIRST+35
%define LVM_SETTEXTCOLOR LVM_FIRST+36
%define LVM_GETTEXTBKCOLOR LVM_FIRST+37
%define LVM_SETTEXTBKCOLOR LVM_FIRST+38
%define LVM_GETTOPINDEX LVM_FIRST+39
%define LVM_GETCOUNTPERPAGE LVM_FIRST+40
%define LVM_GETORIGIN LVM_FIRST+41
%define LVM_UPDATE LVM_FIRST+42
%define LVM_SETITEMSTATE LVM_FIRST+43
%define LVM_GETITEMSTATE LVM_FIRST+44
%define LVM_GETITEMTEXT LVM_FIRST+45
%define LVM_GETITEMTEXTW LVM_FIRST+115
%define LVM_SETITEMTEXT LVM_FIRST+46
%define LVM_SETITEMTEXTW LVM_FIRST+116
%define LVM_SETITEMCOUNT LVM_FIRST+47
%define LVM_SORTITEMS LVM_FIRST+48
%define LVM_SETITEMPOSITION32 LVM_FIRST+49
%define LVM_GETSELECTEDCOUNT LVM_FIRST+50
%define LVM_GETITEMSPACING LVM_FIRST+51
%define LVM_GETISEARCHSTRING LVM_FIRST+52
%define LVM_GETISEARCHSTRINGW LVM_FIRST+117
%define LVN_ITEMCHANGING LVN_FIRST-0
%define LVN_ITEMCHANGED LVN_FIRST-1
%define LVN_INSERTITEM LVN_FIRST-2
%define LVN_DELETEITEM LVN_FIRST-3
%define LVN_DELETEALLITEMS LVN_FIRST-4
%define LVN_BEGINLABELEDIT LVN_FIRST-5
%define LVN_BEGINLABELEDITW LVN_FIRST-75
%define LVN_ENDLABELEDIT LVN_FIRST-6
%define LVN_ENDLABELEDITW LVN_FIRST-76
%define LVN_COLUMNCLICK LVN_FIRST-8
%define LVN_BEGINDRAG LVN_FIRST-9
%define LVN_BEGINRDRAG LVN_FIRST-11
%define LVN_GETDISPINFO LVN_FIRST-50
%define LVN_GETDISPINFOW LVN_FIRST-77
%define LVN_SETDISPINFO LVN_FIRST-51
%define LVN_SETDISPINFOW LVN_FIRST-78
%define LVIF_DI_SETITEM 1000h
%define LVN_KEYDOWN LVN_FIRST-55
%define TVS_HASBUTTONS 0001h
%define TVS_HASLINES 0002h
%define TVS_LINESATROOT 0004h
%define TVS_EDITLABELS 0008h
%define TVS_DISABLEDRAGDROP 0010h
%define TVS_SHOWSELALWAYS 0020h
%define TVIF_TEXT 0001h
%define TVIF_IMAGE 0002h
%define TVIF_PARAM 0004h
%define TVIF_STATE 0008h
%define TVIF_HANDLE 0010h
%define TVIF_SELECTEDIMAGE 0020h
%define TVIF_CHILDREN 0040h
%define TVIS_FOCUSED 0001h
%define TVIS_SELECTED 0002h
%define TVIS_CUT 0004h
%define TVIS_DROPHILITED 0008h
%define TVIS_BOLD 0010h
%define TVIS_EXPANDED 0020h
%define TVIS_EXPANDEDONCE 0040h
%define TVIS_OVERLAYMASK 0F00h
%define TVIS_STATEIMAGEMASK 0F000h
%define TVIS_USERMASK 0F000h
%define I_CHILDRENCALLBACK -1
%define TVI_ROOT 0FFFF0000h
%define TVI_FIRST 0FFFF0001h
%define TVI_LAST 0FFFF0002h
%define TVI_SORT 0FFFF0003h
%define TVM_INSERTITEM TV_FIRST+0
%define TVM_INSERTITEMW TV_FIRST+50
%define TVM_DELETEITEM TV_FIRST+1
%define TVM_EXPAND TV_FIRST+2
%define TVE_COLLAPSE 0001h
%define TVE_EXPAND 0002h
%define TVE_TOGGLE 0003h
%define TVE_COLLAPSERESET 8000h
%define TVM_GETITEMRECT TV_FIRST+4
%define TVM_GETCOUNT TV_FIRST+5
%define TVM_GETINDENT TV_FIRST+6
%define TVM_SETINDENT TV_FIRST+7
%define TVM_GETIMAGELIST TV_FIRST+8
%define TVSIL_NORMAL 0
%define TVSIL_STATE 2
%define TVM_SETIMAGELIST TV_FIRST+9
%define TVM_GETNEXTITEM TV_FIRST+10
%define TVGN_ROOT 0000h
%define TVGN_NEXT 0001h
%define TVGN_PREVIOUS 0002h
%define TVGN_PARENT 0003h
%define TVGN_CHILD 0004h
%define TVGN_FIRSTVISIBLE 0005h
%define TVGN_NEXTVISIBLE 0006h
%define TVGN_PREVIOUSVISIBLE 0007h
%define TVGN_DROPHILITE 0008h
%define TVGN_CARET 0009h
%define TVM_SELECTITEM TV_FIRST+11
%define TVM_GETITEM TV_FIRST+12
%define TVM_GETITEMW TV_FIRST+62
%define TVM_SETITEM TV_FIRST+13
%define TVM_SETITEMW TV_FIRST+63
%define TVM_EDITLABEL TV_FIRST+14
%define TVM_EDITLABELW TV_FIRST+65
%define TVM_GETEDITCONTROL TV_FIRST+15
%define TVM_GETVISIBLECOUNT TV_FIRST+16
%define TVM_HITTEST TV_FIRST+17
%define TVHT_NOWHERE 0001h
%define TVHT_ONITEMICON 0002h
%define TVHT_ONITEMLABEL 0004h
%define TVHT_ONITEMSTATEICON 0040h
%define TVHT_ONITEM TVHT_ONITEMICON|TVHT_ONITEMLABEL|TVHT_ONITEMSTATEICON
%define TVHT_ONITEMINDENT 0008h
%define TVHT_ONITEMBUTTON 0010h
%define TVHT_ONITEMRIGHT 0020h
%define TVHT_ABOVE 0100h
%define TVHT_BELOW 0200h
%define TVHT_TORIGHT 0400h
%define TVHT_TOLEFT 0800h
%define TVM_CREATEDRAGIMAGE TV_FIRST+18
%define TVM_SORTCHILDREN TV_FIRST+19
%define TVM_ENSUREVISIBLE TV_FIRST+20
%define TVM_SORTCHILDRENCB TV_FIRST+21
%define TVM_ENDEDITLABELNOW TV_FIRST+22
%define TVM_GETISEARCHSTRING TV_FIRST+23
%define TVM_GETISEARCHSTRINGW TV_FIRST+64
%define TVN_SELCHANGINGA TVN_FIRST-1
%define TVN_SELCHANGINGW TVN_FIRST-50
%define TVN_SELCHANGEDA TVN_FIRST-2
%define TVN_SELCHANGEDW TVN_FIRST-51
%define TVC_UNKNOWN 0000h
%define TVC_BYMOUSE 0001h
%define TVC_BYKEYBOARD 0002h
%define TVN_GETDISPINFOA TVN_FIRST-3
%define TVN_GETDISPINFOW TVN_FIRST-52
%define TVN_SETDISPINFOA TVN_FIRST-4
%define TVN_SETDISPINFOW TVN_FIRST-53
%define TVIF_DI_SETITEM 1000h
%define TVN_ITEMEXPANDING TVN_FIRST-5
%define TVN_ITEMEXPANDINGW TVN_FIRST-54
%define TVN_ITEMEXPANDED TVN_FIRST-6
%define TVN_ITEMEXPANDEDW TVN_FIRST-55
%define TVN_BEGINDRAG TVN_FIRST-7
%define TVN_BEGINDRAGW TVN_FIRST-56
%define TVN_BEGINRDRAG TVN_FIRST-8
%define TVN_BEGINRDRAGW TVN_FIRST-57
%define TVN_DELETEITEM TVN_FIRST-9
%define TVN_DELETEITEMW TVN_FIRST-58
%define TVN_BEGINLABELEDIT TVN_FIRST-10
%define TVN_BEGINLABELEDITW TVN_FIRST-59
%define TVN_ENDLABELEDIT TVN_FIRST-11
%define TVN_ENDLABELEDITW TVN_FIRST-60
%define TVN_KEYDOWN TVN_FIRST-12
%define TCS_FORCEICONLEFT 0010h
%define TCS_FORCELABELLEFT 0020h
%define TCS_TABS 0000h
%define TCS_BUTTONS 0100h
%define TCS_SINGLELINE 0000h
%define TCS_MULTILINE 0200h
%define TCS_RIGHTJUSTIFY 0000h
%define TCS_FIXEDWIDTH 0400h
%define TCS_RAGGEDRIGHT 0800h
%define TCS_FOCUSONBUTTONDOWN 1000h
%define TCS_OWNERDRAWFIXED 2000h
%define TCS_TOOLTIPS 4000h
%define TCS_FOCUSNEVER 8000h
%define TCM_FIRST 1300h
%define TCM_GETIMAGELIST TCM_FIRST+2
%define TCM_SETIMAGELIST TCM_FIRST+3
%define TCM_GETITEMCOUNT TCM_FIRST+4
%define TCIF_TEXT 0001h
%define TCIF_IMAGE 0002h
%define TCIF_RTLREADING 0004h
%define TCIF_PARAM 0008h
%define TCM_GETITEM TCM_FIRST+5
%define TCM_SETITEM TCM_FIRST+6
%define TCM_SETITEMW TCM_FIRST+61
%define TCM_INSERTITEM TCM_FIRST+7
%define TCM_INSERTITEMW TCM_FIRST+62
%define TCM_DELETEITEM TCM_FIRST+8
%define TCM_DELETEALLITEMS TCM_FIRST+9
%define TCM_GETITEMRECT TCM_FIRST+10
%define TCM_GETCURSEL TCM_FIRST+11
%define TCM_SETCURSEL TCM_FIRST+12
%define TCHT_NOWHERE 0001h
%define TCHT_ONITEMICON 0002h
%define TCHT_ONITEMLABEL 0004h
%define TCHT_ONITEM TCHT_ONITEMICON|TCHT_ONITEMLABEL
%define TCM_HITTEST TCM_FIRST+13
%define TCM_SETITEMEXTRA TCM_FIRST+14
%define TCM_ADJUSTRECT TCM_FIRST+40
%define TCM_SETITEMSIZE TCM_FIRST+41
%define TCM_REMOVEIMAGE TCM_FIRST+42
%define TCM_SETPADDING TCM_FIRST+43
%define TCM_GETROWCOUNT TCM_FIRST+44
%define TCM_GETTOOLTIPS TCM_FIRST+45
%define TCM_SETTOOLTIPS TCM_FIRST+46
%define TCM_GETCURFOCUS TCM_FIRST+47
%define TCM_SETCURFOCUS TCM_FIRST+48
%define TCN_KEYDOWN TCN_FIRST-0
%define TCN_SELCHANGE TCN_FIRST-1
%define TCN_SELCHANGING TCN_FIRST-2
%define ACS_CENTER 0001h
%define ACS_TRANSPARENT 0002h
%define ACS_AUTOPLAY 0004h
%define ACM_OPEN WM_USER+100
%define ACM_OPENW WM_USER+103
%define ACM_PLAY WM_USER+101
%define ACM_STOP WM_USER+102
%define ACN_START 1
%define ACN_STOP 2
ACMFORMATCHOOSE_STYLEF_INITTOWFXSTRUCT equ 0x00000040
ACM_FORMATENUMF_INPUT         equ 0x00800000
ACM_FORMATENUMF_CONVERT   equ 0x00100000
ACMFORMATCHOOSE_STYLEF_ENABLEHOOK      equ     0x00000008
ACMFORMATCHOOSE_STYLEF_CONTEXTHELP    equ      0x00000080
ACMFORMATCHOOSE_STYLEF_SHOWHELP  equ           0x00000004
ACM_FORMATENUMF_HARDWARE equ       0x00400000
ACM_FORMATENUMF_OUTPUT    equ     0x01000000



;--------------------------comdlg equates-------------------------------
%define CDERR_GENERALCODES 0000h
%define CDERR_STRUCTSIZE 0001h
%define CDERR_INITIALIZATION 0002h
%define CDERR_NOTEMPLATE 0003h
%define CDERR_NOHINSTANCE 0004h
%define CDERR_LOADSTRFAILURE 0005h
%define CDERR_FINDRESFAILURE 0006h
%define CDERR_LOADRESFAILURE 0007h
%define CDERR_LOCKRESFAILURE 0008h
%define CDERR_MEMALLOCFAILURE 0009h
%define CDERR_MEMLOCKFAILURE 000Ah
%define CDERR_NOHOOK 000Bh
%define CDERR_REGISTERMSGFAIL 000Ch
%define CC_RGBINIT 00000001h
%define CC_FULLOPEN 00000002h
%define CC_PREVENTFULLOPEN 00000004h
%define CC_SHOWHELP 00000008h
%define CC_ENABLEHOOK 00000010h
%define CC_ENABLETEMPLATE 00000020h
%define CC_ENABLETEMPLATEHANDLE 00000040h
%define CCERR_CHOOSECOLORCODES 5000h
%define FR_DOWN 00000001h
%define FR_WHOLEWORD 00000002h
%define FR_MATCHCASE 00000004h
%define FR_FINDNEXT 00000008h
%define FR_REPLACE 00000010h
%define FR_REPLACEALL 00000020h
%define FR_DIALOGTERM 00000040h
%define FR_SHOWHELP 00000080h
%define FR_ENABLEHOOK 00000100h
%define FR_ENABLETEMPLATE 00000200h
%define FR_NOUPDOWN 00000400h
%define FR_NOMATCHCASE 00000800h
%define FR_NOWHOLEWORD 00001000h
%define FR_ENABLETEMPLATEHANDLE 00002000h
%define FR_HIDEUPDOWN 00004000h
%define FR_HIDEMATCHCASE 00008000h
%define FR_HIDEWHOLEWORD 00010000h
%define FRERR_FINDREPLACECODES 4000h
%define FRERR_BUFFERLENGTHZERO 4001h
%define CF_SCREENFONTS 00000001h
%define CF_PRINTERFONTS 00000002h
%define CF_BOTH CF_SCREENFONTS+CF_PRINTERFONTS
%define CF_SHOWHELP 00000004h
%define CF_ENABLEHOOK 00000008h
%define CF_ENABLETEMPLATE 00000010h
%define CF_ENABLETEMPLATEHANDLE 00000020h
%define CF_INITTOLOGFONTSTRUCT 00000040h
%define CF_USESTYLE 00000080h
%define CF_EFFECTS 00000100h
%define CF_APPLY 00000200h
%define CF_ANSIONLY 00000400h
%define CF_NOVECTORFONTS 00000800h
%define CF_NOOEMFONTS CF_NOVECTORFONTS
%define CF_NOSIMULATIONS 00001000h
%define CF_LIMITSIZE 00002000h
%define CF_FIXEDPITCHONLY 00004000h
%define CF_WYSIWYG 00008000h
%define CF_FORCEFONTEXIST 00010000h
%define CF_SCALABLEONLY 00020000h
%define CF_TTONLY 00040000h
%define CF_NOFACESEL 00080000h
%define CF_NOSTYLESEL 00100000h
%define CF_NOSIZESEL 00200000h
%define CFERR_CHOOSEFONTCODES 2000h
%define CFERR_NOFONTS 2001h
%define CFERR_MAXLESSTHANMIN 2002h
%define WM_CHOOSEFONT_GETLOGFONT WM_USER+1
%define CD_LBSELNOITEMS -1
%define CD_LBSELCHANGE 0
%define CD_LBSELSUB 1
%define CD_LBSELADD 2
%define PD_ALLPAGES 00000000h
%define PD_SELECTION 00000001h
%define PD_PAGENUMS 00000002h
%define PD_NOSELECTION 00000004h
%define PD_NOPAGENUMS 00000008h
%define PD_COLLATE 00000010h
%define PD_PRINTTOFILE 00000020h
%define PD_PRINTSETUP 00000040h
%define PD_NOWARNING 00000080h
%define PD_RETURNDC 00000100h
%define PD_RETURNIC 00000200h
%define PD_RETURNDEFAULT 00000400h
%define PD_SHOWHELP 00000800h
%define PD_ENABLEPRINTHOOK 00001000h
%define PD_ENABLESETUPHOOK 00002000h
%define PD_ENABLEPRINTTEMPLATE 00004000h
%define PD_ENABLESETUPTEMPLATE 00008000h
%define PD_ENABLEPRINTTEMPLATEHANDLE 00010000h
%define PD_ENABLESETUPTEMPLATEHANDLE 00020000h
%define PD_USEDEVMODECOPIES 00040000h
%define PD_DISABLEPRINTTOFILE 00080000h
%define PD_HIDEPRINTTOFILE 00100000h
%define PDERR_PRINTERCODES 1000h
%define PDERR_SETUPFAILURE 1001h
%define PDERR_PARSEFAILURE 1002h
%define PDERR_RETDEFFAILURE 1003h
%define PDERR_LOADDRVFAILURE 1004h
%define PDERR_GETDEVMODEFAIL 1005h
%define PDERR_INITFAILURE 1006h
%define PDERR_NODEVICES 1007h
%define PDERR_NODEFAULTPRN 1008h
%define PDERR_DNDMMISMATCH 1009h
%define PDERR_CREATEICFAILURE 100Ah
%define PDERR_PRINTERNOTFOUND 100Bh
%define PDERR_DEFAULTDIFFERENT 100Ch
%define DN_DEFAULTPRN 0001h
%define OFN_ALLOWMULTISELECT 00000200h
%define OFN_CREATEPROMPT 00002000h
%define OFN_ENABLEHOOK 00000020h
%define OFN_ENABLETEMPLATE 00000040h
%define OFN_ENABLETEMPLATEHANDLE 00000080h
%define OFN_EXPLORER 00080000h
%define OFN_EXTENSIONDIFFERENT 00000400h
%define OFN_FILEMUSTEXIST 00001000h
%define OFN_HIDEREADONLY 00000004h
%define OFN_LONGNAMES 00200000h
%define OFN_NOCHANGEDIR 00000008h
%define OFN_NODEREFERENCELINKS 00100000h
%define OFN_NOLONGNAMES 00040000h
%define OFN_NONETWORKBUTTON 00020000h
%define OFN_NOREADONLYRETURN 00008000h
%define OFN_NOTESTFILECREATE 00010000h
%define OFN_NOVALIDATE 00000100h
%define OFN_OVERWRITEPROMPT 00000002h
%define OFN_PATHMUSTEXIST 00000800h
%define OFN_READONLY 00000001h
%define OFN_SHAREAWARE 00004000h
%define OFN_SHOWHELP 00000010h
%define OFN_SHAREFALLTHROUGH 2
%define OFN_SHARENOWARN 1
%define OFN_SHAREWARN 0
%define CDERR_DIALOGFAILURE 0FFFFh
%define FNERR_FILENAMECODES 3000h
%define FNERR_SUBCLASSFAILURE 3001h
%define FNERR_INVALIDFILENAME 3002h
%define FNERR_BUFFERTOOSMALL 3003h

;--------------------------riched equates-------------------------------
%define cchTextLimitDefault 32767
%define EM_CANPASTE WM_USER+50
%define EM_DISPLAYBAND WM_USER+51
%define EM_EXGETSEL WM_USER+52
%define EM_EXLIMITTEXT WM_USER+53
%define EM_EXLINEFROMCHAR WM_USER+54
%define EM_EXSETSEL WM_USER+55
%define EM_FINDTEXT WM_USER+56
%define EM_FORMATRANGE WM_USER+57
%define EM_GETCHARFORMAT WM_USER+58
%define EM_GETEVENTMASK WM_USER+59
%define EM_GETOLEINTERFACE WM_USER+60
%define EM_GETPARAFORMAT WM_USER+61
%define EM_GETSELTEXT WM_USER+62
%define EM_HIDESELECTION WM_USER+63
%define EM_PASTESPECIAL WM_USER+64
%define EM_REQUESTRESIZE WM_USER+65
%define EM_SELECTIONTYPE WM_USER+66
%define EM_SETBKGNDCOLOR WM_USER+67
%define EM_SETCHARFORMAT WM_USER+68
%define EM_SETEVENTMASK WM_USER+69
%define EM_SETOLECALLBACK WM_USER+70
%define EM_SETPARAFORMAT WM_USER+71
%define EM_SETTARGETDEVICE WM_USER+72
%define EM_STREAMIN WM_USER+73
%define EM_STREAMOUT WM_USER+74
%define EM_GETTEXTRANGE WM_USER+75
%define EM_FINDWORDBREAK WM_USER+76
%define EM_SETOPTIONS WM_USER+77
%define EM_GETOPTIONS WM_USER+78
%define EM_FINDTEXTEX WM_USER+79
%define EM_GETWORDBREAKPROCEX WM_USER+80
%define EM_SETWORDBREAKPROCEX WM_USER+81
%define EM_SETPUNCTUATION WM_USER+100
%define EM_GETPUNCTUATION WM_USER+101
%define EM_SETWORDWRAPMODE WM_USER+102
%define EM_GETWORDWRAPMODE WM_USER+103
%define EM_SETIMECOLOR WM_USER+104
%define EM_GETIMECOLOR WM_USER+105
%define EM_SETIMEOPTIONS WM_USER+106
%define EM_GETIMEOPTIONS WM_USER+107
%define EN_MSGFILTER 0700h
%define EN_REQUESTRESIZE 0701h
%define EN_SELCHANGE 0702h
%define EN_DROPFILES 0703h
%define EN_PROTECTED 0704h
%define EN_CORRECTTEXT 0705h
%define EN_STOPNOUNDO 0706h
%define EN_IMECHANGE 0707h
%define EN_SAVECLIPBOARD 0708h
%define EN_OLEOPFAILED 0709h
%define ENM_NONE 00000000h
%define ENM_CHANGE 00000001h
%define ENM_UPDATE 00000002h
%define ENM_SCROLL 00000004h
%define ENM_KEYEVENTS 00010000h
%define ENM_MOUSEEVENTS 00020000h
%define ENM_REQUESTRESIZE 00040000h
%define ENM_SELCHANGE 00080000h
%define ENM_DROPFILES 00100000h
%define ENM_PROTECTED 00200000h
%define ENM_CORRECTTEXT 00400000h
%define ENM_IMECHANGE 00800000h
%define ES_SAVESEL 00008000h
%define ES_SUNKEN 00004000h
%define ES_DISABLENOSCROLL 00002000h
%define ES_SELECTIONBAR 01000000h
%define ES_EX_NOCALLOLEINIT 01000000h
%define ES_VERTICAL 00400000h
%define ES_NOIME 00080000h
%define ES_SELFIME 00040000h
%define ECO_AUTOWORDSELECTION 00000001h
%define ECO_AUTOVSCROLL 00000040h
%define ECO_AUTOHSCROLL 00000080h
%define ECO_NOHIDESEL 00000100h
%define ECO_READONLY 00000800h
%define ECO_WANTRETURN 00001000h
%define ECO_SAVESEL 00008000h
%define ECO_SELECTIONBAR 01000000h
%define ECO_VERTICAL 00400000h
%define ECOOP_SET 0001h
%define ECOOP_OR 0002h
%define ECOOP_AND 0003h
%define ECOOP_XOR 0004h
%define WB_CLASSIFY 3
%define WB_MOVEWORDLEFT 4
%define WB_MOVEWORDRIGHT 5
%define WB_LEFTBREAK 6
%define WB_RIGHTBREAK 7
%define WB_MOVEWORDPREV 4
%define WB_MOVEWORDNEXT 5
%define WB_PREVBREAK 6
%define WB_NEXTBREAK 7
%define PC_FOLLOWING 1
%define PC_LEADING 2
%define PC_OVERFLOW 3
%define PC_DELIMITER 4
%define WBF_WORDWRAP 010h
%define WBF_WORDBREAK 020h
%define WBF_OVERFLOW 040h
%define WBF_LEVEL1 080h
%define WBF_LEVEL2 100h
%define WBF_CUSTOM 200h
%define IMF_FORCENONE 0001h
%define IMF_FORCEENABLE 0002h
%define IMF_FORCEDISABLE 0004h
%define IMF_CLOSESTATUSWINDOW 0008h
%define IMF_VERTICAL 0020h
%define IMF_FORCEACTIVE 0040h
%define IMF_FORCEINACTIVE 0080h
%define IMF_FORCEREMEMBER 0100h
%define WBF_CLASS 0Fh
%define WBF_ISWHITE 10h
%define WBF_BREAKLINE 20h
%define WBF_BREAKAFTER 40h
%define CFM_BOLD 00000001h
%define CFM_ITALIC 00000002h
%define CFM_UNDERLINE 00000004h
%define CFM_STRIKEOUT 00000008h
%define CFM_PROTECTED 00000010h
%define CFM_SIZE 80000000h
%define CFM_COLOR 40000000h
%define CFM_FACE 20000000h
%define CFM_OFFSET 10000000h
%define CFM_CHARSET 08000000h
%define CFE_BOLD 0001h
%define CFE_ITALIC 0002h
%define CFE_UNDERLINE 0004h
%define CFE_STRIKEOUT 0008h
%define CFE_PROTECTED 0010h
%define CFE_AUTOCOLOR 40000000h
%define yHeightCharPtsMost 1638
%define SCF_SELECTION 0001h
%define SCF_WORD 0002h
%define SF_TEXT 0001h
%define SF_RTF 0002h
%define SF_RTFNOOBJS 0003h
%define SF_TEXTIZED 0004h
%define SFF_SELECTION 8000h
%define SFF_PLAINRTF 4000h
%define MAX_TAB_STOPS 32
%define lDefaultTab 720
%define PFM_STARTINDENT 00000001h
%define PFM_RIGHTINDENT 00000002h
%define PFM_OFFSET 00000004h
%define PFM_ALIGNMENT 00000008h
%define PFM_TABSTOPS 00000010h
%define PFM_NUMBERING 00000020h
%define PFM_OFFSETINDENT 80000000h
%define PFN_BULLET 0001h
%define PFA_LEFT 0001h
%define PFA_RIGHT 0002h
%define PFA_CENTER 0003h
%define SEL_EMPTY 0000h
%define SEL_TEXT 0001h
%define SEL_OBJECT 0002h
%define SEL_MULTICHAR 0004h
%define SEL_MULTIOBJECT 0008h
%define OLEOP_DOVERB 1

;--------------------------wsock32 equates-------------------------------
%define WSADESCRIPTION_LEN 256
%define WSASYS_STATUS_LEN 128
%define IPPROTO_IP 0
%define IPPROTO_ICMP 1
%define IPPROTO_GGP 2
%define IPPROTO_TCP 6
%define IPPROTO_PUP 12
%define IPPROTO_UDP 17
%define IPPROTO_IDP 22
%define IPPROTO_ND 77
%define IPPROTO_RAW 255
%define IPPROTO_MAX 256
%define IOCPARM_MASK 7Fh
%define IOC_VOID 20000000h
%define IOC_OUT 40000000h
%define IOC_IN 80000000h
%define IOC_INOUT IOC_IN|IOC_OUT
%define FIONBIO 8004667Eh
%define FIONSYNC 8004667Dh
%define FIONREAD 4004667Fh
%define IPPORT_ECHO 7
%define IPPORT_DISCARD 9
%define IPPORT_SYSTAT 11
%define IPPORT_DAYTIME 13
%define IPPORT_NETSTAT 15
%define IPPORT_FTP 21
%define IPPORT_TELNET 23
%define IPPORT_SMTP 25
%define IPPORT_TIMESERVER 37
%define IPPORT_NAMESERVER 42
%define IPPORT_WHOIS 43
%define IPPORT_MTP 57
%define IPPORT_TFTP 69
%define IPPORT_RJE 77
%define IPPORT_FINGER 79
%define IPPORT_TTYLINK 87
%define IPPORT_SUPDUP 95
%define IPPORT_EXECSERVER 512
%define IPPORT_LOGINSERVER 513
%define IPPORT_CMDSERVER 514
%define IPPORT_EFSSERVER 520
%define IPPORT_BIFFUDP 512
%define IPPORT_WHOSERVER 513
%define IPPORT_ROUTESERVER 520
%define IPPORT_RESERVED 1024
%define IMPLINK_IP 155
%define IMPLINK_LOWEXPER 156
%define IMPLINK_HIGHEXPER 158
%define IN_CLASSA_NET 0FF000000h
%define IN_CLASSA_NSHIFT 24
%define IN_CLASSA_HOST 000FFFFFFh
%define IN_CLASSA_MAX 128
%define IN_CLASSB_NET 0FFFF0000h
%define IN_CLASSB_NSHIFT 16
%define IN_CLASSB_HOST 00000FFFFh
%define IN_CLASSB_MAX 65536
%define IN_CLASSC_NET 0FFFFFF00h
%define IN_CLASSC_NSHIFT 8
%define IN_CLASSC_HOST 0000000FFh
%define INADDR_ANY 000000000h
%define INADDR_LOOPBACK 07F000001h
%define INADDR_BROADCAST 0FFFFFFFFh
%define INADDR_NONE 0FFFFFFFFh
%define SOCK_STREAM 1
%define SOCK_DGRAM 2
%define SOCK_RAW 3
%define SOCK_RDM 4
%define SOCK_SEQPACKET 5
%define SO_DEBUG 00001h
%define SO_ACCEPTCONN 00002h
%define SO_REUSEADDR 00004h
%define SO_KEEPALIVE 00008h
%define SO_DONTROUTE 00010h
%define SO_BROADCAST 00020h
%define SO_USELOOPBACK 00040h
%define SO_LINGER 00080h
%define SO_OOBINLINE 00100h
%define SOL_SOCKET 0FFFFh
%define SO_DONTLINGER (-1-SO_LINGER)
%define SO_SNDBUF 01001h
%define SO_RCVBUF 01002h
%define SO_SNDLOWAT 01003h
%define SO_RCVLOWAT 01004h
%define SO_SNDTIMEO 01005h
%define SO_RCVTIMEO 01006h
%define SO_ERROR 01007h
%define SO_TYPE 01008h
%define TCP_NODELAY 00001h
%define AF_UNSPEC 0
%define AF_UNIX 1
%define AF_INET 2
%define AF_IMPLINK 3
%define AF_PUP 4
%define AF_CHAOS 5
%define AF_NS 6
%define AF_IPX 6
%define AF_ISO 7
%define AF_OSI AF_ISO
%define AF_ECMA 8
%define AF_DATAKIT 9
%define AF_CCITT 10
%define AF_SNA 11
%define AF_DECnet 12
%define AF_DLI 13
%define AF_LAT 14
%define AF_HYLINK 15
%define AF_APPLETALK 16
%define AF_NETBIOS 17
%define AF_MAX 18
%define PF_UNSPEC AF_UNSPEC
%define PF_UNIX AF_UNIX
%define PF_INET AF_INET
%define PF_IMPLINK AF_IMPLINK
%define PF_PUP AF_PUP
%define PF_CHAOS AF_CHAOS
%define PF_NS AF_NS
%define PF_IPX AF_IPX
%define PF_ISO AF_ISO
%define PF_OSI AF_OSI
%define PF_ECMA AF_ECMA
%define PF_DATAKIT AF_DATAKIT
%define PF_CCITT AF_CCITT
%define PF_SNA AF_SNA
%define PF_DECnet AF_DECnet
%define PF_DLI AF_DLI
%define PF_LAT AF_LAT
%define PF_HYLINK AF_HYLINK
%define PF_APPLETALK AF_APPLETALK
%define PF_MAX AF_MAX
%define SOMAXCONN 5
%define MSG_OOB 01h
%define MSG_PEEK 02h
%define MSG_DONTROUTE 04h
%define MSG_MAXIOVLEN 16
%define MAXGETHOSTSTRUCT 1024
%define FD_READ 001h
%define FD_WRITE 002h
%define FD_OOB 004h
%define FD_ACCEPT 008h
%define FD_CONNECT 010h
%define FD_CLOSE 020h
%define WSABASEERR 10000
%define WSAEINTR WSABASEERR+4
%define WSAEBADF WSABASEERR+9
%define WSAEACCES WSABASEERR+13
%define WSAEFAULT WSABASEERR+14
%define WSAEINVAL WSABASEERR+22
%define WSAEMFILE WSABASEERR+24
%define WSAEWOULDBLOCK WSABASEERR+35
%define WSAEINPROGRESS WSABASEERR+36
%define WSAEALREADY WSABASEERR+37
%define WSAENOTSOCK WSABASEERR+38
%define WSAEDESTADDRREQ WSABASEERR+39
%define WSAEMSGSIZE WSABASEERR+40
%define WSAEPROTOTYPE WSABASEERR+41
%define WSAENOPROTOOPT WSABASEERR+42
%define WSAEPROTONOSUPPORT WSABASEERR+43
%define WSAESOCKTNOSUPPORT WSABASEERR+44
%define WSAEOPNOTSUPP WSABASEERR+45
%define WSAEPFNOSUPPORT WSABASEERR+46
%define WSAEAFNOSUPPORT WSABASEERR+47
%define WSAEADDRINUSE WSABASEERR+48
%define WSAEADDRNOTAVAIL WSABASEERR+49
%define WSAENETDOWN WSABASEERR+50
%define WSAENETUNREACH WSABASEERR+51
%define WSAENETRESET WSABASEERR+52
%define WSAECONNABORTED WSABASEERR+53
%define WSAECONNRESET WSABASEERR+54
%define WSAENOBUFS WSABASEERR+55
%define WSAEISCONN WSABASEERR+56
%define WSAENOTCONN WSABASEERR+57
%define WSAESHUTDOWN WSABASEERR+58
%define WSAETOOMANYREFS WSABASEERR+59
%define WSAETIMEDOUT WSABASEERR+60
%define WSAECONNREFUSED WSABASEERR+61
%define WSAELOOP WSABASEERR+62
%define WSAENAMETOOLONG WSABASEERR+63
%define WSAEHOSTDOWN WSABASEERR+64
%define WSAEHOSTUNREACH WSABASEERR+65
%define WSAENOTEMPTY WSABASEERR+66
%define WSAEPROCLIM WSABASEERR+67
%define WSAEUSERS WSABASEERR+68
%define WSAEDQUOT WSABASEERR+69
%define WSAESTALE WSABASEERR+70
%define WSAEREMOTE WSABASEERR+71
%define WSASYSNOTREADY WSABASEERR+91
%define WSAVERNOTSUPPORTED WSABASEERR+92
%define WSANOTINITIALISED WSABASEERR+93
%define WSAHOST_NOT_FOUND WSABASEERR+1001
%define HOST_NOT_FOUND WSAHOST_NOT_FOUND
%define WSATRY_AGAIN WSABASEERR+1002
%define TRY_AGAIN WSATRY_AGAIN
%define WSANO_RECOVERY WSABASEERR+1003
%define NO_RECOVERY WSANO_RECOVERY
%define WSANO_DATA WSABASEERR+1004
%define NO_DATA WSANO_DATA
%define WSANO_ADDRESS WSANO_DATA
%define NO_ADDRESS WSANO_ADDRESS
%define EWOULDBLOCK WSAEWOULDBLOCK
%define EINPROGRESS WSAEINPROGRESS
%define EALREADY WSAEALREADY
%define ENOTSOCK WSAENOTSOCK
%define EDESTADDRREQ WSAEDESTADDRREQ
%define EMSGSIZE WSAEMSGSIZE
%define EPROTOTYPE WSAEPROTOTYPE
%define ENOPROTOOPT WSAENOPROTOOPT
%define EPROTONOSUPPORT WSAEPROTONOSUPPORT
%define ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
%define EOPNOTSUPP WSAEOPNOTSUPP
%define EPFNOSUPPORT WSAEPFNOSUPPORT
%define EAFNOSUPPORT WSAEAFNOSUPPORT
%define EADDRINUSE WSAEADDRINUSE
%define EADDRNOTAVAIL WSAEADDRNOTAVAIL
%define ENETDOWN WSAENETDOWN
%define ENETUNREACH WSAENETUNREACH
%define ENETRESET WSAENETRESET
%define ECONNABORTED WSAECONNABORTED
%define ECONNRESET WSAECONNRESET
%define ENOBUFS WSAENOBUFS
%define EISCONN WSAEISCONN
%define ENOTCONN WSAENOTCONN
%define ESHUTDOWN WSAESHUTDOWN
%define ETOOMANYREFS WSAETOOMANYREFS
%define ETIMEDOUT WSAETIMEDOUT
%define ECONNREFUSED WSAECONNREFUSED
%define ELOOP WSAELOOP
%define ENAMETOOLONG WSAENAMETOOLONG
%define EHOSTDOWN WSAEHOSTDOWN
%define EHOSTUNREACH WSAEHOSTUNREACH
%define ENOTEMPTY WSAENOTEMPTY
%define EPROCLIM WSAEPROCLIM
%define EUSERS WSAEUSERS
%define EDQUOT WSAEDQUOT
%define ESTALE WSAESTALE
%define EREMOTE WSAEREMOTE
%define FD_SETSIZE 64
%define INVALID_SOCKET (-1-0)
%define SOCKET_ERROR -1
%define SOCKET_BUFFER_SIZE 512
%define ICMP_ECHOREPLY 0
%define ICMP_ECHOREQ 8


                               LR_DEFAULTCOLOR     EQU    0000h

                               LR_MONOCHROME       EQU    0001h

                               LR_COLOR            EQU    0002h

                               LR_COPYRETURNORG    EQU    0004h

                               LR_COPYDELETEORG    EQU    0008h

                               LR_LOADFROMFILE     EQU    0010h

                               LR_LOADTRANSPARENT  EQU    0020h

                               LR_DEFAULTSIZE      EQU    0040h

                               LR_VGACOLOR         EQU    0080h

                               LR_LOADMAP3DCOLORS  EQU    1000h

                               LR_CREATEDIBSECTION EQU    2000h

                               LR_COPYFROMRESOURCE EQU    4000h

                               LR_SHARED           EQU    8000h
;==============





%define EM_SETUNDOLIMIT                 WM_USER + 82
%define EM_REDO                                 WM_USER + 84
%define EM_CANREDO                              WM_USER + 85
%define EM_GETUNDONAME                  WM_USER + 86
%define EM_GETREDONAME                  WM_USER + 87
%define EM_STOPGROUPTYPING              WM_USER + 88

%define EM_SETTEXTMODE                  WM_USER + 89
%define EM_GETTEXTMODE                  WM_USER + 90



;==========================
%define SCF_SELECTION   1
%define SCF_WORD                2
%define SCF_DEFAULT     0
%define SCF_ALL         4
%define SCF_USEUIRULES  8
;============



;========       common dialog messages
%define COLOROKSTRING                   "commdlg_ColorOK"
%define FILEOKSTRING                    "commdlg_FileNameOK"
%define FINDMSGSTRING                   "commdlg_FindReplace"
%define HELPMSGSTRING                   "commdlg_help"
%define LBSELCHSTRING                   "commdlg_LBSelChangedNotify"
%define SETRGBSTRING                    "commdlg_SetRGBColor"
%define SHAREVISTRING                   "commdlg_ShareViolation"

;==============================


;===========    CreateProcess
%assign CREATE_DEFAULT_ERROR_MODE       67108864
%assign CREATE_NEW_CONSOLE              16
%assign CREATE_NEW_PROCESS_GROUP        512
%assign CREATE_SEPARATE_WOW_VDM         2048
%assign CREATE_SUSPENDED                4
%assign CREATE_UNICODE_ENVIRONMENT      1024
%assign DEBUG_PROCESS                   1
%assign DEBUG_ONLY_THIS_PROCESS         2
%assign DETACHED_PROCESS                8
%assign HIGH_PRIORITY_CLASS             128
%assign IDLE_PRIORITY_CLASS             64
%assign NORMAL_PRIORITY_CLASS           32
%assign REALTIME_PRIORITY_CLASS         256
;======================================







;==============================================
;                           DIRECTSOUND DIRECT SOUND
;==============================================

_FACDS                          equ      0878h

; Return Codes
;

DS_OK                           EQU 0
DSERR_ALLOCATED                 EQU  80000000h +_FACDS * 16 + 10      ;80000000h +( _FACDS shl 16) + 10
DSERR_CONTROLUNAVAIL            EQU 80000000h + _FACDS * 16 + 30
DSERR_INVALIDPARAM              EQU E_INVALIDARG
DSERR_INVALIDCALL               EQU 80000000h +_FACDS * 16 + 50
DSERR_GENERIC                   EQU E_FAIL
DSERR_PRIOLEVELNEEDED           EQU 80000000h +_FACDS * 16 + 70
DSERR_OUTOFMEMORY               EQU E_OUTOFMEMORY
DSERR_BADFORMAT                 EQU 80000000h + _FACDS * 16 + 100
DSERR_UNSUPPORTED               EQU E_NOTIMPL
DSERR_NODRIVER                  EQU 80000000h + _FACDS * 16 + 120
DSERR_ALREADYINITIALIZED        EQU 80000000h + _FACDS * 16 + 130
DSERR_NOAGGREGATION             EQU CLASS_E_NOAGGREGATION
DSERR_BUFFERLOST                EQU 80000000h + _FACDS * 16 + 150
DSERR_OTHERAPPHASPRIO           EQU 80000000h + _FACDS * 16 + 160
DSERR_UNINITIALIZED             EQU 80000000h + _FACDS * 16 + 170
DSERR_NOINTERFACE               EQU E_NOINTERFACE

;
; Flags
;

DSCAPS_PRIMARYMONO              EQU 000000001h
DSCAPS_PRIMARYSTEREO            EQU 000000002h
DSCAPS_PRIMARY8BIT              EQU 000000004h
DSCAPS_PRIMARY16BIT             EQU 000000008h
DSCAPS_CONTINUOUSRATE   EQU 000000010h
DSCAPS_EMULDRIVER               EQU 000000020h
DSCAPS_CERTIFIED                EQU 000000040h
DSCAPS_SECONDARYMONO            EQU 000000100h
DSCAPS_SECONDARYSTEREO          EQU 000000200h
DSCAPS_SECONDARY8BIT            EQU 000000400h
DSCAPS_SECONDARY16BIT           EQU 000000800h

DSBPLAY_LOOPING                 EQU 000000001h

DSBSTATUS_PLAYING               EQU 000000001h
DSBSTATUS_BUFFERLOST            EQU 000000002h
DSBSTATUS_LOOPING               EQU 000000004h

DSBLOCK_FROMWRITECURSOR         EQU 000000001h
DSBLOCK_ENTIREBUFFER            EQU 000000002h

DSSCL_NORMAL                    EQU 000000001h
DSSCL_PRIORITY                  EQU 000000002h
DSSCL_EXCLUSIVE                 EQU 000000003h
DSSCL_WRITEPRIMARY              EQU 000000004h

;ifndef NODSOUND3D

DS3DMODE_NORMAL                 EQU 000000000h
DS3DMODE_HEADRELATIVE           EQU 000000001h
DS3DMODE_DISABLE                EQU 000000002h

DS3D_IMMEDIATE                  EQU 000000000h
DS3D_DEFERRED                   EQU 000000001h

%define pdroMCN_LAST 0-759
%define DS3D_MINDISTANCEFACTOR   0.0
%define DS3D_MAXDISTANCEFACTOR   10.0
%define DS3D_DEFAULTDISTANCEFACTOR       1.0
%define DS3D_MINROLLOFFFACTOR   0.0             ; ou será assim ? DS3D_MINROLLOFFFACTOR db "0.0",0
%define DS3D_MAXROLLOFFFACTOR   10.0
%define DS3D_DEFAULTROLLOFFFACTOR       1.0
%define DS3D_MINDOPPLERFACTOR   0.0
%define DS3D_MAXDOPPLERFACTOR            10.0
%define DS3D_DEFAULTDOPPLERFACTOR       1.0
%define DS3D_DEFAULTMINDISTANCE 1.0
%define DS3D_DEFAULTMAXDISTANCE  1000000000.0

DS3D_MINCONEANGLE               EQU 0
DS3D_MAXCONEANGLE               EQU 360
DS3D_DEFAULTCONEANGLE           EQU 360

DS3D_DEFAULTCONEOUTSIDEVOLUME   EQU 0

;endif ; NODSOUND3D

DSBCAPS_PRIMARYBUFFER           EQU 000000001h
DSBCAPS_STATIC                  EQU 000000002h
DSBCAPS_LOCHARDWARE             EQU 000000004h
DSBCAPS_LOCSOFTWARE             EQU 000000008h
DSBCAPS_CTRL3D                  EQU 000000010h
DSBCAPS_CTRLFREQUENCY           EQU 000000020h
DSBCAPS_CTRLPAN                 EQU 000000040h
DSBCAPS_CTRLVOLUME              EQU 000000080h
DSBCAPS_CTRLPOSITIONNOTIFY      EQU 000000100h
DSBCAPS_CTRLDEFAULT             EQU 0000000E0h
DSBCAPS_CTRLALL                 EQU 0000001F0h
DSBCAPS_STICKYFOCUS             EQU 000004000h
DSBCAPS_GLOBALFOCUS             EQU 000008000h
DSBCAPS_GETCURRENTPOSITION2     EQU 000010000h
DSBCAPS_MUTE3DATMAXDISTANCE     EQU 000020000h

DSCBCAPS_WAVEMAPPED             EQU 080000000h

DSSPEAKER_HEADPHONE             EQU 000000001h
DSSPEAKER_MONO                  EQU 000000002h
DSSPEAKER_QUAD                  EQU 000000003h
DSSPEAKER_STEREO                EQU 000000004h
DSSPEAKER_SURROUND              EQU 000000005h

DSSPEAKER_GEOMETRY_MIN          EQU 000000005h  ;   5 degrees
DSSPEAKER_GEOMETRY_NARROW       EQU 00000000Ah  ;  10 degrees
DSSPEAKER_GEOMETRY_WIDE         EQU 000000014h  ;  20 degrees
DSSPEAKER_GEOMETRY_MAX          EQU 0000000B4h  ; 180 degrees

DSCCAPS_EMULDRIVER              EQU 000000020h

DSCBLOCK_ENTIREBUFFER           EQU 000000001h

DSCBSTATUS_CAPTURING            EQU 000000001h
DSCBSTATUS_LOOPING              EQU 000000002h

DSCBSTART_LOOPING               EQU 000000001h

DSBFREQUENCY_MIN                EQU 100
DSBFREQUENCY_MAX                EQU 100000
DSBFREQUENCY_ORIGINAL           EQU 0

DSBPAN_LEFT                     EQU -10000
DSBPAN_CENTER                   EQU 0
DSBPAN_RIGHT                    EQU 10000

DSBVOLUME_MIN                   EQU -10000
DSBVOLUME_MAX                   EQU 0

DSBSIZE_MIN                     EQU 4
DSBSIZE_MAX                     EQU 00FFFFFFFh

DSBPN_OFFSETSTOP                EQU 0FFFFFFFFh

;***************************************



;******** MCI window styles
MCIWNDF_NOAUTOSIZEWINDOW    EQU 0001h
MCIWNDF_NOPLAYBAR           EQU 0002h
MCIWNDF_NOAUTOSIZEMOVIE     EQU 0004h
MCIWNDF_NOMENU              EQU 0008h
MCIWNDF_SHOWNAME            EQU 0010h
MCIWNDF_SHOWPOS             EQU 0020h
MCIWNDF_SHOWMODE            EQU 0040h
MCIWNDF_SHOWALL             EQU 0070h
MCIWNDF_NOTIFYMODE          EQU 0100h
MCIWNDF_NOTIFYPOS           EQU 0200h
MCIWNDF_NOTIFYSIZE          EQU 0400h
MCIWNDF_NOTIFYERROR         EQU 1000h
MCIWNDF_NOTIFYALL           EQU 1F00h
MCIWNDF_NOTIFYANSI          EQU 0080h


;**************************************
; --------------------- Imagehlp.h EQUATES ---------------------

CHECKSUM_SUCCESS                            equ   0
CHECKSUM_OPEN_FAILURE                       equ   1
CHECKSUM_MAP_FAILURE                        equ   2
CHECKSUM_MAPVIEW_FAILURE                    equ   3
CHECKSUM_UNICODE_FAILURE                    equ   4
SPLITSYM_REMOVE_PRIVATE                     equ  000000001h
SPLITSYM_EXTRACT_ALL                        equ  000000002h
SPLITSYM_SYMBOLPATH_IS_SRC                  equ  000000004h
BIND_NO_BOUND_IMPORTS                       equ  000000001h
BIND_NO_UPDATE                              equ  000000002h
BIND_ALL_IMAGES                             equ  000000004h
BIND_CACHE_IMPORT_DLLS                      equ  000000008h

IMAGE_SEPARATION                            equ  65536

UNDNAME_COMPLETE                            equ  00000h
UNDNAME_NO_LEADING_UNDERSCORES              equ  00001h
UNDNAME_NO_MS_KEYWORDS                      equ  00002h
UNDNAME_NO_FUNCTION_RETURNS                 equ  00004h
UNDNAME_NO_ALLOCATION_MODEL                 equ  00008h
UNDNAME_NO_ALLOCATION_LANGUAGE              equ  00010h
UNDNAME_NO_MS_THISTYPE                      equ  00020h
UNDNAME_NO_CV_THISTYPE                      equ  00040h
UNDNAME_NO_THISTYPE                         equ  00060h
UNDNAME_NO_ACCESS_SPECIFIERS                equ  00080h
UNDNAME_NO_THROW_SIGNATURES                 equ  00100h
UNDNAME_NO_MEMBER_TYPE                      equ  00200h
UNDNAME_NO_RETURN_UDT_MODEL                 equ  00400h
UNDNAME_32_BIT_DECODE                       equ  00800h
UNDNAME_NAME_ONLY                           equ  01000h
UNDNAME_NO_ARGUMENTS                        equ  02000h
UNDNAME_NO_SPECIAL_SYMS                     equ  04000h
SYMF_OMAP_GENERATED                         equ  000000001h
SYMF_OMAP_MODIFIED                          equ  000000002h
CBA_DEFERRED_SYMBOL_LOAD_START              equ  000000001h
CBA_DEFERRED_SYMBOL_LOAD_COMPLETE           equ  000000002h
CBA_DEFERRED_SYMBOL_LOAD_FAILURE            equ  000000003h
CBA_SYMBOLS_UNLOADED                        equ  000000004h
CBA_DUPLICATE_SYMBOL                        equ  000000005h
SYMOPT_CASE_INSENSITIVE                     equ  000000001h
SYMOPT_UNDNAME                              equ  000000002h
SYMOPT_DEFERRED_LOADS                       equ  000000004h
SYMOPT_NO_CPP                               equ  000000008h
SYMOPT_LOAD_LINES                           equ  000000010h
SYMOPT_OMAP_FIND_NEAREST                    equ  000000020h
CERT_PE_IMAGE_DIGEST_DEBUG_INFO             equ  001h
CERT_PE_IMAGE_DIGEST_RESOURCES              equ  002h
CERT_PE_IMAGE_DIGEST_ALL_IMPORT_INFO        equ  004h
CERT_PE_IMAGE_DIGEST_NON_PE_INFO            equ  008h
CERT_SECTION_TYPE_ANY                       equ  0FFh

AddrMode1616        equ  0
AddrMode1632        equ  1
AddrModeReal        equ  2
AddrModeFlat        equ  3
SymNone             equ 0
SymCoff             equ 1
SymCv               equ 2
SymPdb              equ 3
SymExport           equ 4
SymDeferred         equ 5
SymSym              equ 6
;**************** Imagehlp.h  equates end






;///////////////////////////


;***********************************************



COINIT_APARTMENTTHREADED        equ 2
COINIT_MULTITHREADED            equ 0
COINIT_DISABLE_OLE1DDE          equ 4
COINIT_SPEED_OVER_MEMORY        equ 8

REST_NONE                       equ 00000000h
REST_NORUN                      equ 00000001h
REST_NOCLOSE                    equ 00000002h
REST_NOSAVESET                  equ 00000004h
REST_NOFILEMENU                 equ 00000008h
REST_NOSETFOLDERS               equ 00000010h
REST_NOSETTASKBAR               equ 00000020h
REST_NODESKTOP                  equ 00000040h
REST_NOFIND                     equ 00000080h
REST_NODRIVES                   equ 00000100h
REST_NODRIVEAUTORUN             equ 00000200h
REST_NODRIVETYPEAUTORUN         equ 00000400h
REST_NONETHOOD                  equ 00000800h
REST_STARTBANNER                equ 00001000h
REST_RESTRICTRUN                equ 00002000h
REST_NOPRINTERTABS              equ 00004000h
REST_NOPRINTERDELETE            equ 00008000h
REST_NOPRINTERADD               equ 00010000h
REST_NOSTARTMENUSUBFOLDERS      equ 00020000h
REST_MYDOCSONNET                equ 00040000h
REST_NOEXITTODOS                equ 00080000h
REST_ENFORCESHELLEXTSECURITY    equ 00100000h
REST_LINKRESOLVEIGNORELINKINFO  equ 00200000h
REST_NOCOMMONGROUPS             equ 00400000h
REST_SEPARATEDESKTOPPROCESS     equ 00800000h
REST_NOWEB                      equ 01000000h
REST_NOTRAYCONTEXTMENU          equ 02000000h
REST_NOVIEWCONTEXTMENU          equ 04000000h
REST_NONETCONNECTDISCONNECT     equ 08000000h
REST_STARTMENULOGOFF            equ 10000000h
REST_NOSETTINGSASSIST           equ 20000000h
REST_NOINTERNETICON             equ 40000001h
REST_NORECENTDOCSHISTORY        equ 40000002h
REST_NORECENTDOCSMENU           equ 40000003h
REST_NOACTIVEDESKTOP            equ 40000004h
REST_NOACTIVEDESKTOPCHANGES     equ 40000005h
REST_NOFAVORITESMENU            equ 40000006h
REST_CLEARRECENTDOCSONEXIT      equ 40000007h
REST_CLASSICSHELL               equ 40000008h
REST_NOCUSTOMIZEWEBVIEW         equ 40000009h
REST_NOHTMLWALLPAPER            equ 40000010h
REST_NOCHANGINGWALLPAPER        equ 40000011h
REST_NODESKCOMP                 equ 40000012h
REST_NOADDDESKCOMP              equ 40000013h
REST_NODELDESKCOMP              equ 40000014h
REST_NOCLOSEDESKCOMP            equ 40000015h
REST_NOCLOSE_DRAGDROPBAND       equ 40000016h
REST_NOMOVINGBAND               equ 40000017h
REST_NOEDITDESKCOMP             equ 40000018h
REST_NORESOLVESEARCH            equ 40000019h
REST_NORESOLVETRACK             equ 4000001Ah
REST_FORCECOPYACLWITHFILE       equ 4000001Bh
REST_NOLOGO3CHANNELNOTIFY       equ 4000001Ch
REST_NOFORGETSOFTWAREUPDATE     equ 4000001Dh
REST_NOSETACTIVEDESKTOP         equ 4000001Eh
REST_NOUPDATEWINDOWS            equ 4000001Fh
REST_NOCHANGESTARMENU           equ 40000020h
REST_NOFOLDEROPTIONS            equ 40000021h
REST_HASFINDCOMPUTERS           equ 40000022h
REST_INTELLIMENUS               equ 40000023h
REST_RUNDLGMEMCHECKBOX          equ 40000024h
REST_ARP_ShowPostSetup          equ 40000025h
REST_NOCSC                      equ 40000026h
REST_NOCONTROLPANEL             equ 40000027h
REST_ENUMWORKGROUP              equ 40000028h
REST_ARP_NOARP                  equ 40000029h
REST_ARP_NOREMOVEPAGE           equ 4000002Ah
REST_ARP_NOADDPAGE              equ 4000002Bh
REST_ARP_NOWINSETUPPAGE         equ 4000002Ch
REST_GREYMSIADS                 equ 4000002Dh
REST_NOCHANGEMAPPEDDRIVELABEL   equ 4000002Eh
REST_NOCHANGEMAPPEDDRIVECOMMENT equ 4000002Fh
REST_MaxRecentDocs              equ 40000030h
REST_NONETWORKCONNECTIONS       equ 40000031h
REST_FORCESTARTMENULOGOFF       equ 40000032h
REST_NOWEBVIEW                  equ 40000033h
REST_NOCUSTOMIZETHISFOLDER      equ 40000034h
REST_NOENCRYPTION               equ 40000035h
REST_DONTSHOWSUPERHIDDEN        equ 40000037h
REST_NOSHELLSEARCHBUTTON        equ 40000038h
REST_NOHARDWARETAB              equ 40000039h
REST_NORUNASINSTALLPROMPT       equ 4000003Ah
REST_PROMPTRUNASINSTALLNETPATH  equ 4000003Bh
REST_NOMANAGEMYCOMPUTERVERB     equ 4000003Ch
REST_NORECENTDOCSNETHOOD        equ 4000003Dh
REST_DISALLOWRUN                equ 4000003Eh
REST_NOWELCOMESCREEN            equ 4000003Fh
REST_RESTRICTCPL                equ 40000040h
REST_DISALLOWCPL                equ 40000041h
REST_NOSMBALLOONTIP             equ 40000042h
REST_NOSMHELP                   equ 40000043h
REST_NOWINKEYS                  equ 40000044h
REST_NOENCRYPTONMOVE            equ 40000045h
REST_NOLOCALMACHINERUN          equ 40000046h
REST_NOCURRENTUSERRUN           equ 40000047h
REST_NOLOCALMACHINERUNONCE      equ 40000048h
REST_NOCURRENTUSERRUNONCE       equ 40000049h
REST_FORCEACTIVEDESKTOPON       equ 4000004Ah
REST_NOCOMPUTERSNEARME          equ 4000004Bh
REST_NOVIEWONDRIVE              equ 4000004Ch
REST_NONETCRAWL                 equ 4000004Dh
REST_NOSHAREDDOCUMENTS          equ 4000004Eh
REST_NOSMMYDOCS                 equ 4000004Fh
REST_NOSMMYPICS                 equ 40000050h
REST_ALLOWBITBUCKDRIVES         equ 40000051h
REST_NONLEGACYSHELLMODE         equ 40000052h
REST_NOCONTROLPANELBARRICADE    equ 40000053h
REST_NOSTARTPAGE                equ 40000054h
REST_NOAUTOTRAYNOTIFY           equ 40000055h
REST_NOTASKGROUPING             equ 40000056h
REST_NOCDBURNING                equ 40000057h
REST_MYCOMPNOPROP               equ 40000058h
REST_MYDOCSNOPROP               equ 40000059h
REST_NOSTARTPANEL               equ 4000005Ah
REST_NODISPLAYAPPEARANCEPAGE    equ 4000005Bh
REST_NOTHEMESTAB                equ 4000005Ch
REST_NOVISUALSTYLECHOICE        equ 4000005Dh
REST_NOSIZECHOICE               equ 4000005Eh
REST_NOCOLORCHOICE              equ 4000005Fh
REST_SETVISUALSTYLE             equ 40000060h
REST_STARTRUNNOHOMEPATH         equ 40000061h
REST_NOUSERNAMEINSTARTPANEL     equ 40000062h
REST_NOMYCOMPUTERICON           equ 40000063h
REST_NOSMNETWORKPLACES          equ 40000064h
REST_NOSMPINNEDLIST             equ 40000065h
REST_NOSMMYMUSIC                equ 40000066h
REST_NOSMEJECTPC                equ 40000067h
REST_NOSMMOREPROGRAMS           equ 40000068h
REST_NOSMMFUPROGRAMS            equ 40000069h
REST_NOTRAYITEMSDISPLAY         equ 4000006Ah
REST_NOTOOLBARSONTASKBAR        equ 4000006Bh
REST_NOSMCONFIGUREPROGRAMS      equ 4000006Fh
REST_HIDECLOCK                  equ 40000070h
REST_NOLOWDISKSPACECHECKS       equ 40000071h
REST_NOENTIRENETWORK            equ 40000072h
REST_NODESKTOPCLEANUP           equ 40000073h
REST_BITBUCKNUKEONDELETE        equ 40000074h
REST_BITBUCKCONFIRMDELETE       equ 40000075h
REST_BITBUCKNOPROP              equ 40000076h
REST_NODISPBACKGROUND           equ 40000077h
REST_NODISPSCREENSAVEPG         equ 40000078h
REST_NODISPSETTINGSPG           equ 40000079h
REST_NODISPSCREENSAVEPREVIEW    equ 4000007Ah
REST_NODISPLAYCPL               equ 4000007Bh
REST_HIDERUNASVERB              equ 4000007Ch
REST_NOTHUMBNAILCACHE           equ 4000007Dh
REST_NOSTRCMPLOGICAL            equ 4000007Eh
REST_NOPUBLISHWIZARD            equ 4000007Fh
REST_NOONLINEPRINTSWIZARD       equ 40000080h
REST_NOWEBSERVICES              equ 40000081h
REST_ALLOWUNHASHEDWEBVIEW       equ 40000082h
REST_ALLOWLEGACYWEBVIEW         equ 40000083h
REST_REVERTWEBVIEWSECURITY      equ 40000084h
REST_INHERITCONSOLEHANDLES      equ 40000086h
REST_NODISCONNECT               equ 41000001h
REST_NOSECURITY                 equ 41000002h
REST_NOFILEASSOCIATE            equ 41000003h



;======= Video for Windows



BI_1632                     equ 32333631h
ICERR_OK                    equ 0
ICERR_DONTDRAW              equ 1
ICERR_NEWPALETTE            equ 2
ICERR_GOTOKEYFRAME          equ 3
ICERR_STOPDRAWING           equ 4
ICERR_UNSUPPORTED           equ -1
ICERR_BADFORMAT             equ -2
ICERR_MEMORY                equ -3
ICERR_INTERNAL              equ -4
ICERR_BADFLAGS              equ -5
ICERR_BADPARAM              equ -6
ICERR_BADSIZE               equ -7
ICERR_BADHANDLE             equ -8
ICERR_CANTUPDATE            equ -9
ICERR_ABORT                 equ -10
ICERR_ERROR                 equ -100
ICERR_BADBITDEPTH           equ -200
ICERR_BADIMAGESIZE          equ -201
ICERR_CUSTOM                equ -400
ICMODE_COMPRESS             equ 1
ICMODE_DECOMPRESS           equ 2
ICMODE_FASTDECOMPRESS       equ 3
ICMODE_QUERY                equ 4
ICMODE_FASTCOMPRESS         equ 5
ICMODE_DRAW                 equ 8
ICMODE_INTERNALF_FUNCTION32 equ 8000h
ICMODE_INTERNALF_MASK       equ 8000h
AVIIF_LIST                  equ 00000001h
AVIIF_TWOCC                 equ 00000002h
AVIIF_KEYFRAME              equ 00000010h
ICQUALITY_LOW               equ 0
ICQUALITY_HIGH              equ 10000
ICQUALITY_DEFAULT           equ -1
ICM_USER                    equ DRV_USER+0000h
ICM_RESERVED_LOW            equ DRV_USER+1000h
ICM_RESERVED                equ ICM_RESERVED_LOW
ICM_RESERVED_HIGH           equ DRV_USER+2000h
ICM_GETSTATE                equ ICM_RESERVED+0
ICM_SETSTATE                equ ICM_RESERVED+1
ICM_GETINFO                 equ ICM_RESERVED+2
ICM_CONFIGURE               equ ICM_RESERVED+10
ICM_ABOUT                   equ ICM_RESERVED+11
ICM_GETERRORTEXT            equ ICM_RESERVED+12
ICM_GETFORMATNAME           equ ICM_RESERVED+20
ICM_ENUMFORMATS             equ ICM_RESERVED+21
ICM_GETDEFAULTQUALITY       equ ICM_RESERVED+30
ICM_GETQUALITY              equ ICM_RESERVED+31
ICM_SETQUALITY              equ ICM_RESERVED+32
ICM_SET                     equ ICM_RESERVED+40
ICM_GET                     equ ICM_RESERVED+41
ICM_COMPRESS_GET_FORMAT     equ ICM_USER+4
ICM_COMPRESS_GET_SIZE       equ ICM_USER+5
ICM_COMPRESS_QUERY          equ ICM_USER+6
ICM_COMPRESS_BEGIN          equ ICM_USER+7
ICM_COMPRESS                equ ICM_USER+8
ICM_COMPRESS_END            equ ICM_USER+9
ICM_DECOMPRESS_GET_FORMAT   equ ICM_USER+10
ICM_DECOMPRESS_QUERY        equ ICM_USER+11
ICM_DECOMPRESS_BEGIN        equ ICM_USER+12
ICM_DECOMPRESS              equ ICM_USER+13
ICM_DECOMPRESS_END          equ ICM_USER+14
ICM_DECOMPRESS_SET_PALETTE  equ ICM_USER+29
ICM_DECOMPRESS_GET_PALETTE  equ ICM_USER+30
ICM_DRAW_QUERY              equ ICM_USER+31
ICM_DRAW_BEGIN              equ ICM_USER+15
ICM_DRAW_GET_PALETTE     equ ICM_USER+16
ICM_DRAW_UPDATE             equ ICM_USER+17
ICM_DRAW_START              equ ICM_USER+18
ICM_DRAW_STOP               equ ICM_USER+19
ICM_DRAW_BITS               equ ICM_USER+20
ICM_DRAW_END                equ ICM_USER+21
ICM_DRAW_GETTIME            equ ICM_USER+32
ICM_DRAW                    equ ICM_USER+33
ICM_DRAW_WINDOW             equ ICM_USER+34
ICM_DRAW_SETTIME            equ ICM_USER+35
ICM_DRAW_REALIZE            equ ICM_USER+36
ICM_DRAW_FLUSH              equ ICM_USER+37
ICM_DRAW_RENDERBUFFER       equ ICM_USER+38
ICM_DRAW_START_PLAY         equ ICM_USER+39
ICM_DRAW_STOP_PLAY          equ ICM_USER+40
ICM_DRAW_SUGGESTFORMAT      equ ICM_USER+50
ICM_DRAW_CHANGEPALETTE      equ ICM_USER+51
ICM_DRAW_IDLE               equ ICM_USER+52
ICM_GETBUFFERSWANTED        equ ICM_USER+41
ICM_GETDEFAULTKEYFRAMERATE  equ ICM_USER+42

ICM_DECOMPRESSEX_BEGIN      equ ICM_USER+60
ICM_DECOMPRESSEX_QUERY      equ ICM_USER+61
ICM_DECOMPRESSEX            equ ICM_USER+62
ICM_DECOMPRESSEX_END        equ ICM_USER+63
ICM_COMPRESS_FRAMES_INFO    equ ICM_USER+70
ICM_COMPRESS_FRAMES         equ ICM_USER+71
ICM_SET_STATUS_PROC         equ ICM_USER+72
VIDCF_QUALITY               equ 0001h
VIDCF_CRUNCH                equ 0002h
VIDCF_TEMPORAL              equ 0004h
VIDCF_COMPRESSFRAMES        equ 0008h
VIDCF_DRAW                  equ 0010h
VIDCF_FASTTEMPORALC         equ 0020h
VIDCF_FASTTEMPORALD         equ 0080h
ICCOMPRESS_KEYFRAME         equ 00000001h
ICCOMPRESSFRAMES_PADDING    equ 00000001h
ICSTATUS_START              equ 0
ICSTATUS_STATUS             equ 1
ICSTATUS_END                equ 2
ICSTATUS_ERROR              equ 3
ICSTATUS_YIELD              equ 4
ICDECOMPRESS_HURRYUP        equ 80000000h
ICDECOMPRESS_UPDATE         equ 40000000h
ICDECOMPRESS_PREROLL        equ 20000000h
ICDECOMPRESS_NULLFRAME      equ 10000000h
ICDECOMPRESS_NOTKEYFRAME    equ 08000000h
ICDRAW_QUERY                equ 00000001h
ICDRAW_FULLSCREEN           equ 00000002h
ICDRAW_HDC                  equ 00000004h
ICDRAW_ANIMATE              equ 00000008h
ICDRAW_CONTINUE             equ 00000010h
ICDRAW_MEMORYDC             equ 00000020h
ICDRAW_UPDATING             equ 00000040h
ICDRAW_RENDER               equ 00000080h
ICDRAW_BUFFER               equ 00000100h
ICDRAW_HURRYUP              equ 80000000h
ICDRAW_UPDATE               equ 40000000h
ICDRAW_PREROLL              equ 20000000h
ICDRAW_NULLFRAME            equ 10000000h
ICDRAW_NOTKEYFRAME          equ 08000000h
ICINSTALL_UNICODE           equ 8000h
ICINSTALL_FUNCTION          equ 0001h
ICINSTALL_DRIVER            equ 0002h
ICINSTALL_HDRV              equ 0004h
ICINSTALL_DRIVERW           equ 8002h
ICMF_CONFIGURE_QUERY        equ 00000001h
ICMF_ABOUT_QUERY            equ 00000001h
ICMF_COMPVARS_VALID         equ 00000001h
ICMF_CHOOSE_KEYFRAME        equ 0001h
ICMF_CHOOSE_DATARATE        equ 0002h
ICMF_CHOOSE_PREVIEW         equ 0004h
ICMF_CHOOSE_ALLCOMPRESSORS  equ 0008h
DDF_0001                    equ 0001h
DDF_UPDATE                  equ 0002h
DDF_SAME_HDC                equ 0004h
DDF_SAME_DRAW               equ 0008h
DDF_DONTDRAW                equ 0010h
DDF_ANIMATE                 equ 0020h
DDF_BUFFER                  equ 0040h
DDF_JUSTDRAWIT              equ 0080h
DDF_FULLSCREEN              equ 0100h
DDF_BACKGROUNDPAL           equ 0200h
DDF_NOTKEYFRAME             equ 0400h
DDF_HURRYUP                 equ 0800h
DDF_HALFTONE                equ 1000h
DDF_2000                    equ 2000h
DDF_PREROLL                 equ DDF_DONTDRAW
DDF_SAME_DIB                equ DDF_SAME_DRAW
DDF_SAME_SIZE               equ DDF_SAME_DRAW
AVIF_HASINDEX               equ 00000010h
AVIF_MUSTUSEINDEX           equ 00000020h
AVIF_ISINTERLEAVED          equ 00000100h
AVIF_WASCAPTUREFILE         equ 00010000h
AVIF_COPYRIGHTED            equ 00020000h
AVI_HEADERSIZE              equ 2048
AVISF_DISABLED              equ 00000001h
AVISF_VIDEO_PALCHANGES      equ 00010000h
AVIIF_FIRSTPART equ 00000020h
AVIIF_LASTPART              equ 00000040h
AVIIF_MIDPART               equ AVIIF_LASTPART | AVIIF_FIRSTPART ;???
AVIIF_NOTIME                equ 00000100h
AVIIF_COMPUSE               equ 0FFF0000h
;AVIIF_KEYFRAME              equ 00000010h
AVIGETFRAMEF_BESTDISPLAYFMT equ 1
AVISTREAMINFO_DISABLED      equ 00000001h
AVISTREAMINFO_FORMATCHANGES equ 00010000h
AVIFILEINFO_HASINDEX        equ 00000010h
AVIFILEINFO_MUSTUSEINDEX    equ 00000020h
AVIFILEINFO_ISINTERLEAVED   equ 00000100h
AVIFILEINFO_WASCAPTUREFILE  equ 00010000h
AVIFILEINFO_COPYRIGHTED     equ 00020000h
AVIFILECAPS_CANREAD         equ 00000001h
AVIFILECAPS_CANWRITE        equ 00000002h
AVIFILECAPS_ALLKEYFRAMES    equ 00000010h
AVIFILECAPS_NOCOMPRESSION   equ 00000020h
AVICOMPRESSF_INTERLEAVE     equ 00000001h
AVICOMPRESSF_DATARATE       equ 00000002h
AVICOMPRESSF_KEYFRAMES      equ 00000004h
AVICOMPRESSF_VALID          equ 00000008h
AVIFILEHANDLER_CANREAD      equ 0001h
AVIFILEHANDLER_CANWRITE     equ 0002h
AVIFILEHANDLER_CANACCEPTNONRGB  equ 0004h
FIND_DIR                    equ 0000000Fh
FIND_NEXT                   equ 00000001h
FIND_PREV                   equ 00000004h
FIND_FROM_START             equ 00000008h
FIND_TYPE                   equ 000000F0h
FIND_KEY                    equ 00000010h
FIND_ANY                    equ 00000020h
FIND_FORMAT                 equ 00000040h
FIND_RET                    equ 0000F000h
FIND_POS                    equ 00000000h
FIND_LENGTH                 equ 00001000h
FIND_OFFSET                 equ 00002000h
FIND_SIZE                   equ 00003000h
FIND_INDEX                  equ 00004000h


SEARCH_NEAREST              equ FIND_PREV
SEARCH_BACKWARD             equ FIND_PREV
SEARCH_FORWARD              equ FIND_NEXT
SEARCH_KEY                  equ FIND_KEY
SEARCH_ANY                  equ FIND_ANY
MCIWNDOPENF_NEW             equ 0001h
MCIWNDF_NOTIFYMEDIAA        equ 0880h
MCIWNDF_NOTIFYMEDIAW        equ 0800h
MCIWNDF_RECORD              equ 2000h
MCIWNDF_NOERRORDLG          equ 4000h
MCIWNDF_NOOPEN              equ 8000h
MCIWNDM_GETDEVICEID         equ WM_USER+100
MCIWNDM_GETSTART            equ WM_USER+103
MCIWNDM_GETLENGTH           equ WM_USER+104
MCIWNDM_GETEND              equ WM_USER+105
MCIWNDM_EJECT               equ WM_USER+107
MCIWNDM_SETZOOM             equ WM_USER+108
MCIWNDM_GETZOOM             equ WM_USER+109
MCIWNDM_SETVOLUME           equ WM_USER+110
MCIWNDM_GETVOLUME           equ WM_USER+111
MCIWNDM_SETSPEED            equ WM_USER+112
MCIWNDM_GETSPEED            equ WM_USER+113
MCIWNDM_SETREPEAT           equ WM_USER+114
MCIWNDM_GETREPEAT           equ WM_USER+115
MCIWNDM_REALIZE             equ WM_USER+118
MCIWNDM_VALIDATEMEDIA       equ WM_USER+121
MCIWNDM_PLAYFROM            equ WM_USER+122
MCIWNDM_PLAYTO              equ WM_USER+123
MCIWNDM_GETPALETTE          equ WM_USER+126
MCIWNDM_SETPALETTE          equ WM_USER+127
MCIWNDM_SETTIMERS           equ WM_USER+129
MCIWNDM_SETACTIVETIMER      equ WM_USER+130
MCIWNDM_SETINACTIVETIMER    equ WM_USER+131
MCIWNDM_GETACTIVETIMER      equ WM_USER+132
MCIWNDM_GETINACTIVETIMER    equ WM_USER+133
MCIWNDM_CHANGESTYLES        equ WM_USER+135
MCIWNDM_GETSTYLES           equ WM_USER+136
MCIWNDM_GETALIAS            equ WM_USER+137
MCIWNDM_PLAYREVERSE         equ WM_USER+139
MCIWNDM_GET_SOURCE          equ WM_USER+140
MCIWNDM_PUT_SOURCE          equ WM_USER+141
MCIWNDM_GET_DEST            equ WM_USER+142
MCIWNDM_PUT_DEST            equ WM_USER+143
MCIWNDM_CAN_PLAY            equ WM_USER+144
MCIWNDM_CAN_WINDOW          equ WM_USER+145
MCIWNDM_CAN_RECORD          equ WM_USER+146
MCIWNDM_CAN_SAVE            equ WM_USER+147
MCIWNDM_CAN_EJECT           equ WM_USER+148
MCIWNDM_CAN_CONFIG          equ WM_USER+149
MCIWNDM_PALETTEKICK         equ WM_USER+150
MCIWNDM_OPENINTERFACE       equ WM_USER+151
MCIWNDM_SETOWNER            equ WM_USER+152
MCIWNDM_SENDSTRINGA         equ WM_USER+101
MCIWNDM_GETPOSITIONA        equ WM_USER+102
MCIWNDM_GETMODEA            equ WM_USER+106
MCIWNDM_SETTIMEFORMATA      equ WM_USER+119
MCIWNDM_GETTIMEFORMATA      equ WM_USER+120
MCIWNDM_GETFILENAMEA        equ WM_USER+124
MCIWNDM_GETDEVICEA          equ WM_USER+125
MCIWNDM_GETERRORA           equ WM_USER+128
MCIWNDM_NEWA                equ WM_USER+134
MCIWNDM_RETURNSTRINGA       equ WM_USER+138
MCIWNDM_OPENA               equ WM_USER+153
MCIWNDM_SENDSTRINGW         equ WM_USER+201
MCIWNDM_GETPOSITIONW        equ WM_USER+202
MCIWNDM_GETMODEW            equ WM_USER+206
MCIWNDM_SETTIMEFORMATW      equ WM_USER+219
MCIWNDM_GETTIMEFORMATW      equ WM_USER+220
MCIWNDM_GETFILENAMEW        equ WM_USER+224
MCIWNDM_GETDEVICEW          equ WM_USER+225
MCIWNDM_GETERRORW           equ WM_USER+228
MCIWNDM_NEWW                equ WM_USER+234
MCIWNDM_RETURNSTRINGW       equ WM_USER+238
MCIWNDM_OPENW               equ WM_USER+252
MCIWNDM_NOTIFYMODE          equ WM_USER+200
MCIWNDM_NOTIFYPOS           equ WM_USER+201
MCIWNDM_NOTIFYSIZE          equ WM_USER+202
MCIWNDM_NOTIFYMEDIA         equ WM_USER+203
MCIWNDM_NOTIFYERROR         equ WM_USER+205
MCIWND_START                equ -1
MCIWND_END                  equ -2
;?????????
DV_ERR_OK                   equ 0
DV_ERR_BASE                 equ 1
DV_ERR_NONSPECIFIC          equ DV_ERR_BASE
DV_ERR_BADFORMAT            equ DV_ERR_BASE+1
DV_ERR_STILLPLAYING         equ DV_ERR_BASE+2
DV_ERR_UNPREPARED           equ DV_ERR_BASE+3
DV_ERR_SYNC                 equ DV_ERR_BASE+4
DV_ERR_TOOMANYCHANNELS      equ DV_ERR_BASE+5
DV_ERR_NOTDETECTED          equ DV_ERR_BASE+6
DV_ERR_BADINSTALL           equ DV_ERR_BASE+7
DV_ERR_CREATEPALETTE        equ DV_ERR_BASE+8
DV_ERR_SIZEFIELD            equ DV_ERR_BASE+9
DV_ERR_PARAM1               equ DV_ERR_BASE+10
DV_ERR_PARAM2               equ DV_ERR_BASE+11
DV_ERR_CONFIG1              equ DV_ERR_BASE+12
DV_ERR_CONFIG2              equ DV_ERR_BASE+13
DV_ERR_FLAGS                equ DV_ERR_BASE+14
DV_ERR_13                   equ DV_ERR_BASE+15
DV_ERR_NOTSUPPORTED         equ DV_ERR_BASE+16
DV_ERR_NOMEM                equ DV_ERR_BASE+17
DV_ERR_ALLOCATED            equ DV_ERR_BASE+18
DV_ERR_BADDEVICEID          equ DV_ERR_BASE+19
DV_ERR_INVALHANDLE          equ DV_ERR_BASE+20
DV_ERR_BADERRNUM            equ DV_ERR_BASE+21
DV_ERR_NO_BUFFERS           equ DV_ERR_BASE+22
DV_ERR_MEM_CONFLICT         equ DV_ERR_BASE+23
DV_ERR_IO_CONFLICT          equ DV_ERR_BASE+24
DV_ERR_DMA_CONFLICT         equ DV_ERR_BASE+25
DV_ERR_INT_CONFLICT         equ DV_ERR_BASE+26
DV_ERR_PROTECT_ONLY         equ DV_ERR_BASE+27
DV_ERR_LASTERROR            equ DV_ERR_BASE+27
DV_ERR_USER_MSG             equ DV_ERR_BASE+1000
MM_DRVM_OPEN                equ 3D0h
MM_DRVM_CLOSE               equ 3D1h
MM_DRVM_DATA                equ 3D2h
MM_DRVM_ERROR               equ 3D3h
DV_VM_OPEN                  equ MM_DRVM_OPEN
DV_VM_CLOSE                 equ MM_DRVM_CLOSE
DV_VM_DATA                  equ MM_DRVM_DATA
DV_VM_ERROR                 equ MM_DRVM_ERROR
VHDR_DONE                   equ 00000001h
VHDR_PREPARED               equ 00000002h
VHDR_INQUEUE                equ 00000004h
VHDR_KEYFRAME               equ 00000008h
VHDR_VALID                  equ 0000000Fh
VCAPS_OVERLAY               equ 00000001h
VCAPS_SRC_CAN_CLIP          equ 00000002h
VCAPS_DST_CAN_CLIP          equ 00000004h
VCAPS_CAN_SCALE             equ 00000008h
VIDEO_EXTERNALIN            equ 0001h
VIDEO_EXTERNALOUT           equ 0002h
VIDEO_IN                    equ 0004h
VIDEO_OUT                   equ 0008h
VIDEO_DLG_QUERY             equ 0010h
VIDEO_CONFIGURE_QUERY       equ 8000h
VIDEO_CONFIGURE_SET         equ 1000h
VIDEO_CONFIGURE_GET         equ 2000h
VIDEO_CONFIGURE_QUERYSIZE   equ 0001h
VIDEO_CONFIGURE_CURRENT     equ 0010h
VIDEO_CONFIGURE_NOMINAL     equ 0020h
VIDEO_CONFIGURE_MIN         equ 0040h
VIDEO_CONFIGURE_MAX         equ 0080h
DVM_USER                    equ 4000h
DVM_CONFIGURE_START         equ 1000h
DVM_CONFIGURE_END           equ 1FFFh
DVM_PALETTE                 equ DVM_CONFIGURE_START+1
DVM_FORMAT                  equ DVM_CONFIGURE_START+2
DVM_PALETTERGB555           equ DVM_CONFIGURE_START+3
DVM_SRC_RECT                equ DVM_CONFIGURE_START+4
DVM_DST_RECT                equ DVM_CONFIGURE_START+5
WM_CAP_START                equ WM_USER
WM_CAP_UNICODE_START        equ WM_USER+100
WM_CAP_GET_CAPSTREAMPTR     equ WM_CAP_START+1
WM_CAP_SET_CALLBACK_ERRORW  equ WM_CAP_UNICODE_START+2
WM_CAP_SET_CALLBACK_STATUSW equ WM_CAP_UNICODE_START+3
WM_CAP_SET_CALLBACK_ERRORA  equ WM_CAP_START+2
WM_CAP_SET_CALLBACK_STATUSA equ WM_CAP_START+3
WM_CAP_SET_CALLBACK_YIELD   equ WM_CAP_START+4
WM_CAP_SET_CALLBACK_FRAME   equ WM_CAP_START+5
WM_CAP_SET_CALLBACK_VIDEOSTREAM equ WM_CAP_START+6
WM_CAP_SET_CALLBACK_WAVESTREAM equ WM_CAP_START+7
WM_CAP_GET_USER_DATA        equ WM_CAP_START+8
WM_CAP_SET_USER_DATA        equ WM_CAP_START+9
WM_CAP_DRIVER_CONNECT       equ WM_CAP_START+10
WM_CAP_DRIVER_DISCONNECT    equ WM_CAP_START+11
WM_CAP_DRIVER_GET_NAMEA     equ WM_CAP_START+12
WM_CAP_DRIVER_GET_VERSIONA  equ WM_CAP_START+13
WM_CAP_DRIVER_GET_NAMEW     equ WM_CAP_UNICODE_START+12
WM_CAP_DRIVER_GET_VERSIONW  equ WM_CAP_UNICODE_START+13
WM_CAP_DRIVER_GET_CAPS      equ WM_CAP_START+14
WM_CAP_FILE_SET_CAPTURE_FILE equ WM_CAP_START + 20
WM_CAP_FILE_SET_CAPTURE_FILEA equ WM_CAP_START+20
WM_CAP_FILE_GET_CAPTURE_FILE equ WM_CAP_START+21
WM_CAP_FILE_GET_CAPTURE_FILEA equ WM_CAP_START+21
WM_CAP_FILE_SAVEASA         equ WM_CAP_START+23
WM_CAP_FILE_SAVEDIBA        equ WM_CAP_START+25
WM_CAP_FILE_SAVEDIB  equ WM_CAP_START+25
 WM_CAP_SAVEDIB equ WM_CAP_START+25
WM_CAP_FILE_SET_CAPTURE_FILEW equ WM_CAP_UNICODE_START+20
WM_CAP_FILE_GET_CAPTURE_FILEW equ WM_CAP_UNICODE_START+21
WM_CAP_FILE_SAVEASW         equ WM_CAP_UNICODE_START+23
WM_CAP_FILE_SAVEAS equ WM_CAP_START + 23
WM_CAP_FILE_SAVEDIBW        equ WM_CAP_UNICODE_START+25
WM_CAP_FILE_ALLOCATE        equ WM_CAP_START+22
WM_CAP_FILE_SET_INFOCHUNK   equ WM_CAP_START+24
WM_CAP_EDIT_COPY            equ WM_CAP_START+30
WM_CAP_SET_AUDIOFORMAT      equ WM_CAP_START+35
WM_CAP_GET_AUDIOFORMAT      equ WM_CAP_START+36
WM_CAP_DLG_VIDEOFORMAT      equ WM_CAP_START+41
WM_CAP_DLG_VIDEOSOURCE      equ WM_CAP_START+42
WM_CAP_DLG_VIDEODISPLAY     equ WM_CAP_START+43
WM_CAP_GET_VIDEOFORMAT      equ WM_CAP_START+44
WM_CAP_SET_VIDEOFORMAT      equ WM_CAP_START+45
WM_CAP_DLG_VIDEOCOMPRESSION equ WM_CAP_START+46
WM_CAP_SET_PREVIEW          equ WM_CAP_START+50
WM_CAP_SET_OVERLAY          equ WM_CAP_START+51
WM_CAP_SET_PREVIEWRATE      equ WM_CAP_START+52
WM_CAP_SET_SCALE            equ WM_CAP_START+53
WM_CAP_GET_STATUS           equ WM_CAP_START+54
WM_CAP_SET_SCROLL           equ WM_CAP_START+55
WM_CAP_GRAB_FRAME           equ WM_CAP_START+60
WM_CAP_GRAB_FRAME_NOSTOP    equ WM_CAP_START+61
WM_CAP_SEQUENCE             equ WM_CAP_START+62
WM_CAP_SEQUENCE_NOFILE      equ WM_CAP_START+63
WM_CAP_SET_SEQUENCE_SETUP   equ WM_CAP_START+64
WM_CAP_GET_SEQUENCE_SETUP   equ WM_CAP_START+65
WM_CAP_SET_MCI_DEVICEA      equ WM_CAP_START+66
WM_CAP_GET_MCI_DEVICEA      equ WM_CAP_START+67
WM_CAP_SET_MCI_DEVICEW      equ WM_CAP_UNICODE_START+66
WM_CAP_GET_MCI_DEVICEW      equ WM_CAP_UNICODE_START+67
WM_CAP_STOP                 equ WM_CAP_START+68
WM_CAP_ABORT                equ WM_CAP_START+69
WM_CAP_SINGLE_FRAME_OPEN    equ WM_CAP_START+70
WM_CAP_SINGLE_FRAME_CLOSE   equ WM_CAP_START+71
WM_CAP_SINGLE_FRAME         equ WM_CAP_START+72
WM_CAP_PAL_OPENA            equ WM_CAP_START+80
WM_CAP_PAL_SAVEA            equ WM_CAP_START+81
WM_CAP_PAL_OPENW            equ WM_CAP_UNICODE_START+80
WM_CAP_PAL_SAVEW            equ WM_CAP_UNICODE_START+81
WM_CAP_PAL_PASTE            equ WM_CAP_START+82
WM_CAP_PAL_AUTOCREATE       equ WM_CAP_START+83
WM_CAP_PAL_MANUALCREATE     equ WM_CAP_START+84
WM_CAP_SET_CALLBACK_CAPCONTROL equ WM_CAP_START+85
WM_CAP_UNICODE_END          equ WM_CAP_PAL_SAVEW
WM_CAP_END                  equ WM_CAP_UNICODE_END
AVSTREAMMASTER_AUDIO        equ 0
AVSTREAMMASTER_NONE         equ 1
CONTROLCALLBACK_PREROLL     equ 1
CONTROLCALLBACK_CAPTURING   equ 2
IDS_CAP_BEGIN               equ 300
IDS_CAP_END                 equ 301
IDS_CAP_INFO                equ 401
IDS_CAP_OUTOFMEM            equ 402
IDS_CAP_FILEEXISTS          equ 403
IDS_CAP_ERRORPALOPEN        equ 404
IDS_CAP_ERRORPALSAVE        equ 405
IDS_CAP_ERRORDIBSAVE        equ 406
IDS_CAP_DEFAVIEXT           equ 407
IDS_CAP_DEFPALEXT           equ 408
IDS_CAP_CANTOPEN            equ 409
IDS_CAP_SEQ_MSGSTART        equ 410
IDS_CAP_SEQ_MSGSTOP         equ 411
IDS_CAP_VIDEDITERR          equ 412
IDS_CAP_READONLYFILE        equ 413
IDS_CAP_WRITEERROR          equ 414
IDS_CAP_NODISKSPACE         equ 415
IDS_CAP_SETFILESIZE         equ 416
IDS_CAP_SAVEASPERCENT       equ 417
IDS_CAP_DRIVER_ERROR        equ 418
IDS_CAP_WAVE_OPEN_ERROR     equ 419
IDS_CAP_WAVE_ALLOC_ERROR    equ 420
IDS_CAP_WAVE_PREPARE_ERROR  equ 421
IDS_CAP_WAVE_ADD_ERROR      equ 422
IDS_CAP_WAVE_SIZE_ERROR     equ 423
IDS_CAP_VIDEO_OPEN_ERROR    equ 424
IDS_CAP_VIDEO_ALLOC_ERROR   equ 425
IDS_CAP_VIDEO_PREPARE_ERROR equ 426
IDS_CAP_VIDEO_ADD_ERROR     equ 427
IDS_CAP_VIDEO_SIZE_ERROR    equ 428
IDS_CAP_FILE_OPEN_ERROR     equ 429
IDS_CAP_FILE_WRITE_ERROR    equ 430
IDS_CAP_RECORDING_ERROR     equ 431
IDS_CAP_RECORDING_ERROR2    equ 432
IDS_CAP_AVI_INIT_ERROR      equ 433
IDS_CAP_NO_FRAME_CAP_ERROR  equ 434
IDS_CAP_NO_PALETTE_WARN     equ 435
IDS_CAP_MCI_CONTROL_ERROR   equ 436
IDS_CAP_MCI_CANT_STEP_ERROR equ 437
IDS_CAP_NO_AUDIO_CAP_ERROR  equ 438
IDS_CAP_AVI_DRAWDIB_ERROR   equ 439
IDS_CAP_COMPRESSOR_ERROR    equ 440
IDS_CAP_AUDIO_DROP_ERROR    equ 441
IDS_CAP_AUDIO_DROP_COMPERROR equ 442
IDS_CAP_STAT_LIVE_MODE      equ 500
IDS_CAP_STAT_OVERLAY_MODE   equ 501
IDS_CAP_STAT_CAP_INIT       equ 502
IDS_CAP_STAT_CAP_FINI       equ 503
IDS_CAP_STAT_PALETTE_BUILD  equ 504
IDS_CAP_STAT_OPTPAL_BUILD   equ 505
IDS_CAP_STAT_I_FRAMES       equ 506
IDS_CAP_STAT_L_FRAMES       equ 507
IDS_CAP_STAT_CAP_L_FRAMES   equ 508
IDS_CAP_STAT_CAP_AUDIO      equ 509
IDS_CAP_STAT_VIDEOCURRENT   equ 510
IDS_CAP_STAT_VIDEOAUDIO     equ 511
IDS_CAP_STAT_VIDEOONLY      equ 512
IDS_CAP_STAT_FRAMESDROPPED  equ 513

;************************************************



;===== [ GUID ] =============================

AMTVAUDIO_MODE_MONO    equ 0x0001
AMTVAUDIO_MODE_STEREO       equ 0x0002
AMTVAUDIO_MODE_LANG_A      equ 0x0010
AMTVAUDIO_MODE_LANG_B       equ 0x0020
AMTVAUDIO_MODE_LANG_C      equ 0x0040
AMTUNER_MODE_DEFAULT equ 0x0000
AMTUNER_MODE_TV equ 0x0001
AMTUNER_MODE_FM_RADIO equ 0x0002
AMTUNER_MODE_AM_RADIO equ 0x0004
AMTUNER_MODE_DSS equ 0x0008
CLSCTX_INPROC_SERVER            equ 1
CLSCTX_INPROC_HANDLER  equ  2
CLSCTX_LOCAL_SERVER   equ  4
CLSCTX_REMOTE_SERVER equ 16
CLSCTX_SERVER    equ CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER
CLSCTX_ALL       equ (CLSCTX_INPROC_HANDLER | CLSCTX_SERVER)




        AnalogVideo_None       equ 0x00000000
        AnalogVideo_NTSC_M     equ 0x00000001  ;        75 IRE Setup
        AnalogVideo_NTSC_M_J   equ 0x00000002  ; Japan,  0 IRE Setup
        AnalogVideo_NTSC_433   equ 0x00000004
        AnalogVideo_PAL_B      equ 0x00000010
        AnalogVideo_PAL_D      equ 0x00000020
        AnalogVideo_PAL_H      equ 0x00000080
        AnalogVideo_PAL_I      equ 0x00000100
        AnalogVideo_PAL_M      equ 0x00000200
        AnalogVideo_PAL_N      equ 0x00000400
        AnalogVideo_PAL_60     equ 0x00000800
        AnalogVideo_SECAM_B    equ 0x00001000
        AnalogVideo_SECAM_D    equ 0x00002000
        AnalogVideo_SECAM_G    equ 0x00004000
        AnalogVideo_SECAM_H    equ 0x00008000
        AnalogVideo_SECAM_K    equ 0x00010000
        AnalogVideo_SECAM_K1   equ 0x00020000
        AnalogVideo_SECAM_L    equ 0x00040000
        AnalogVideo_SECAM_L1   equ 0x00080000
;========= [ end  some GUID ] =================


;//////////////////////////

;??????????????????????????????????????????????????

;==========[ enum.inc ]====================

;       ACL_INFORMATION_CLASS
%assign AclRevisionInformation          1
%assign AclSizeInformation              2

;       MEDIA_TYPE
%assign Unknown                         0
%assign F5_1Pt2_512                     1
%assign F3_1Pt44_512                    2
%assign F3_2Pt88_512                    3
%assign F3_20Pt8_512                    4
%assign F3_720_512                      5
%assign F5_360_512                      6
%assign F5_320_512                      7
%assign F5_320_1024                     8
%assign F5_180_512                      9
%assign F5_160_512                      10
%assign RemovableMedia                  11
%assign FixedMedia                      12

;       RASCONNSTATE
%assign RASCS_DONE                      0x2000
%assign RASCS_PAUSED                    0x1000
%assign RASCS_OpenPort                  0
%assign RASCS_PortOpened                1
%assign RASCS_ConnectDevice             2
%assign RASCS_DeviceConnected           3
%assign RASCS_AllDevicesConnected       4
%assign RASCS_Authenticate              5
%assign RASCS_AuthNotify                6
%assign RASCS_AuthRetry                 7
%assign RASCS_AuthCallback              8
%assign RASCS_AuthChangePassword        9
%assign RASCS_AuthProject               10
%assign RASCS_AuthLinkSpeed             11
%assign RASCS_AuthAck                   12
%assign RASCS_ReAuthenticate            13
%assign RASCS_Authenticated             14
%assign RASCS_PrepareForCallback        15
%assign RASCS_WaitForModemReset         16
%assign RASCS_WaitForCallback           17
%assign RASCS_Projected                 18
%assign RASCS_StartAuthentication       19
%assign RASCS_CallbackComplete          20
%assign RASCS_LogonNetwork              21
%assign RASCS_Interactive               RASCS_PAUSED
%assign RASCS_RetryAuthentication       RASCS_PAUSED + 1
%assign RASCS_CallbackSetByCaller       RASCS_PAUSED + 2
%assign RASCS_PasswordExpired           RASCS_PAUSED + 3
%assign RASCS_Connected                 RASCS_DONE
%assign RASCS_Disconnected              RASCS_DONE + 1

;       RASPROJECTION
%assign RASP_Amb                        0x10000
%assign RASP_PppNbf                     0x803F
%assign RASP_PppIpx                     0x802B
%assign RASP_PppIp                      0x8021

;       SECURITY_IMPERSONATION_LEVEL
%assign SecurityAnonymous               0
%assign SecurityIdentification          1
%assign SecurityImpersonation           2
%assign SecurityDelegation              3

;       SHCONTF
%assign SHCONTF_FOLDERS                 32
%assign SHCONTF_NONFOLDERS              64
%assign SHCONTF_INCLUDEHIDDEN           128

;       SHGNO
%assign SHGDN_NORMAL                    0
%assign SHGDN_INFOLDER                  1
%assign SHGDN_FORPARSING                0x8000

;       SID_NAME_USE
%assign SidTypeUser                     1
%assign SidTypeGroup                    2
%assign SidTypeDomain                   3
%assign SidTypeAlias                    4
%assign SidTypeWellKnownGroup           5
%assign SidTypeDeletedAccount           6
%assign SidTypeInvalid                  7
%assign SidTypeUnknown                  8

;       TOKEN_INFORMATION_CLASS
%assign TokenUser                       1
%assign TokenGroups                     2
%assign TokenPrivileges                 3
%assign TokenOwner                      4
%assign TokenPrimaryGroup               5
%assign TokenDefaultDacl                6
%assign TokenSource                     7
%assign TokenType                       8
%assign TokenImpersonationLevel         9
%assign TokenStatistics                 10

;       TOKEN_TYPE
%assign TokenPrimary                    1
%assign TokenImpersonation              2

;===========[ enum.inc  end ]===============================
%assign TLS_OUT_OF_INDEXES              0xFFFFFFFF
%assign MAX_PATH                        260
%assign UNICODE_NULL                    0
%assign LF_FACESIZE                     32
%assign LF_FULLFACESIZE                 64
%assign ELF_VENDOR_SIZE                 4
%assign SECURITY_STATIC_TRACKING        0
%assign SECURITY_DYNAMIC_TRACKING       1
%assign MAX_DEFAULTCHAR                 2
%assign MAX_LEADBYTES                   12
%assign EXCEPTION_MAXIMUM_PARAMETERS    15
%assign CCHDEVICENAME                   32
%assign CCHFORMNAME                     32
%assign MENU_TEXT_LEN                   40
%assign MAX_LANA                        254
%assign NCBNAMSZ                        16
%assign NETBIOS_NAME_LEN                16
%assign OFS_MAXPATHNAME                 128
%assign MAX_TAB_STOPS                   32
%assign ANYSIZE_ARRAY                   1
%assign RAS_MaxCallbackNumber           128
%assign RAS_MaxDeviceName               128
%assign RAS_MaxDeviceType               16
%assign RAS_MaxEntryName                256
%assign RAS_MaxIpAddress                15
%assign RAS_MaxIpxAddress               21
%assign RAS_MaxPhoneNumber              128
%assign UNLEN                           256
%assign PWLEN                           256
%assign CNLEN                           15
%assign DNLEN                           15
;======================================
;       BroadcastSystemMessage
%assign BSF_FLUSHDISK                   4
%assign BSF_FORCEIFHUNG                 32
%assign BSF_IGNORECURRENTTASK           2
%assign BSF_NOHANG                      8
%assign BSF_POSTMESSAGE                 16
%assign BSF_QUERY                       1
%assign BSM_ALLCOMPONENTS               0
%assign BSM_APPLICATIONS                8
%assign BSM_INSTALLABLEDRIVERS          4
%assign BSM_NETDRIVER                   2
%assign BSM_VXDS                        1
%assign BROADCAST_QUERY_DENY            1112363332
;=========================================
;       ChangeDisplaySettings
%assign DM_BITSPERPEL                   0x40000
%assign DM_PELSWIDTH                    0x80000
%assign DM_PELSHEIGHT                   0x100000
%assign DM_DISPLAYFLAGS                 0x200000
%assign DM_DISPLAYFREQUENCY             0x400000
%assign CDS_UPDATEREGISTRY              1
%assign CDS_TEST                        2
%assign DISP_CHANGE_SUCCESSFUL          0
%assign DISP_CHANGE_RESTART             1
%assign DISP_CHANGE_BADFLAGS            -4
%assign DISP_CHANGE_FAILED              -1
%assign DISP_CHANGE_BADMODE             -2
%assign DISP_CHANGE_NOTUPDATED          -3
;==========================================
;       ChangeServiceConfig
%assign SERVICE_NO_CHANGE               -1
%assign SERVICE_WIN32_OWN_PROCESS       16
%assign SERVICE_WIN32_SHARE_PROCESS     32
%assign SERVICE_KERNEL_DRIVER           1
%assign SERVICE_FILE_SYSTEM_DRIVER      2
%assign SERVICE_INTERACTIVE_PROCESS     256
%assign SERVICE_BOOT_START              0
%assign SERVICE_SYSTEM_START            1
%assign SERVICE_AUTO_START              2
%assign SERVICE_DEMAND_START            3
%assign SERVICE_DISABLED                4
%assign SERVICE_ERROR_CRITICAL          3
;=====================================
;       ChildWindowFromPointEx
%assign CWP_ALL                         0
%assign CWP_SKIPINVISIBLE               1
%assign CWP_SKIPDISABLED                2
%assign CWP_SKIPTRANSPARENT             4
;==================================
;       CompareString, LCMapString
%assign LOCALE_SYSTEM_DEFAULT           0x800
%assign LOCALE_USER_DEFAULT             0x400
%assign NORM_IGNORECASE                 1
%assign NORM_IGNOREKANATYPE             65536
%assign NORM_IGNORENONSPACE             2
%assign NORM_IGNORESYMBOLS              4
%assign NORM_IGNOREWIDTH                131072
%assign SORT_STRINGSORT                 4096
%assign LCMAP_BYTEREV                   2048
%assign LCMAP_FULLWIDTH                 8388608
%assign LCMAP_HALFWIDTH                 4194304
%assign LCMAP_HIRAGANA                  1048576
%assign LCMAP_KATAKANA                  2097152
%assign LCMAP_LOWERCASE                 256
%assign LCMAP_SORTKEY                   1024
%assign LCMAP_UPPERCASE                 512
;=============================================
;       CreateDesktop
%assign DF_ALLOWOTHERACCOUNTHOOK        0x1
%assign DESKTOP_CREATEMENU              0x4
%assign DESKTOP_CREATEWINDOW            0x2
%assign DESKTOP_ENUMERATE               0x40
%assign DESKTOP_HOOKCONTROL             0x8
%assign DESKTOP_JOURNALPLAYBACK         0x20
%assign DESKTOP_JOURNALRECORD           0x10
%assign DESKTOP_READOBJECTS             0x1
%assign DESKTOP_SWITCHDESKTOP           0x100
%assign DESKTOP_WRITEOBJECTS            0x80
%assign WSF_VISIBLE                     0x1
;=========================================
;       CreateFileMapping, VirtualAlloc, VirtualFree, VirtualProtect
%assign SEC_COMMIT                      134217728
%assign SEC_IMAGE                       16777216
%assign SEC_NOCACHE                     268435456
%assign SEC_RESERVE                     67108864
%assign PAGE_READONLY                   2
%assign PAGE_READWRITE                  4
%assign PAGE_WRITECOPY                  8
%assign PAGE_EXECUTE                    16
%assign PAGE_EXECUTE_READ               32
%assign PAGE_EXECUTE_READWRITE          64
%assign PAGE_EXECUTE_WRITECOPY          128
%assign PAGE_GUARD                      256
%assign PAGE_NOACCESS                   1
%assign PAGE_NOCACHE                    512
%assign MEM_COMMIT                      4096
%assign MEM_FREE                        65536
%assign MEM_RESERVE                     8192
%assign MEM_IMAGE                       16777216
%assign MEM_MAPPED                      262144
%assign MEM_PRIVATE                     131072
%assign MEM_DECOMMIT                    16384
%assign MEM_RELEASE                     32768
%assign MEM_TOP_DOWN                    1048576
%assign EXCEPTION_GUARD_PAGE            0x80000001
%assign SECTION_EXTEND_SIZE             0x10
%assign SECTION_MAP_READ                0x4
%assign SECTION_MAP_WRITE               0x2
%assign SECTION_QUERY                   0x1
%assign SECTION_ALL_ACCESS              0xF001
;==========================================

;       CreateTapePartition, WriteTapemark
%assign TAPE_FIXED_PARTITIONS           0
%assign TAPE_INITIATOR_PARTITIONS       0x2
%assign TAPE_SELECT_PARTITIONS          0x1
%assign TAPE_FILEMARKS                  0x1
%assign TAPE_LONG_FILEMARKS             0x3
%assign TAPE_SETMARKS                   0
%assign TAPE_SHORT_FILEMARKS            0x2
;==========================================
;       DrawAnimatedRects
%assign IDANI_OPEN                      1
%assign IDANI_CLOSE                     2
;       DrawCaption
%assign DC_ACTIVE                       1
%assign DC_SMALLCAP                     2
;       DrawIconEx
%assign DI_COMPAT                       4
%assign DI_DEFAULTSIZE                  8
%assign DI_IMAGE                        2
%assign DI_MASK                         1
%assign DI_NORMAL                       3
;       DrawState
%assign DST_BITMAP                      4
%assign DST_COMPLEX                     0
%assign DST_ICON                        3
%assign DST_PREFIXTEXT                  2
%assign DST_TEXT                        1
%assign DSS_NORMAL                      0
%assign DSS_UNION                       16
%assign DSS_DISABLED                    32
%assign DSS_MONO                        128
;       DrawText, DrawTextEx
%assign DT_BOTTOM                       8
%assign DT_CALCRECT                     1024
%assign DT_CENTER                       1
%assign DT_EDITCONTROL                  8192
%assign DT_END_ELLIPSIS                 32768
%assign DT_PATH_ELLIPSIS                16384
%assign DT_EXPANDTABS                   64
%assign DT_EXTERNALLEADING              512
%assign DT_LEFT                         0
%assign DT_MODIFYSTRING                 65536
%assign DT_NOCLIP                       256
%assign DT_NOPREFIX                     2048
%assign DT_RIGHT                        2
%assign DT_RTLREADING                   131072
%assign DT_SINGLELINE                   32
%assign DT_TABSTOP                      128
%assign DT_TOP                          0
%assign DT_VCENTER                      4
%assign DT_WORDBREAK                    16
%assign DT_INTERNAL                     4096
;       DuplicateHandle, MapViewOfFile
%assign DUPLICATE_CLOSE_SOURCE          1
%assign DUPLICATE_SAME_ACCESS           2
%assign FILE_MAP_ALL_ACCESS             0xF001F
%assign FILE_MAP_READ                   4
%assign FILE_MAP_WRITE                  2
%assign FILE_MAP_COPY                   1
%assign MUTEX_ALL_ACCESS                0x1F0001
%assign MUTEX_MODIFY_STATE              1
%assign SYNCHRONIZE                     0x100000
%assign SEMAPHORE_ALL_ACCESS            0x1F0003
%assign SEMAPHORE_MODIFY_STATE          2
%assign EVENT_ALL_ACCESS                0x1F0003
%assign EVENT_MODIFY_STATE              2
%assign KEY_ALL_ACCESS                  0xF003F
%assign KEY_CREATE_LINK                 32
%assign KEY_CREATE_SUB_KEY              4
%assign KEY_ENUMERATE_SUB_KEYS          8
%assign KEY_EXECUTE                     0x20019
%assign KEY_NOTIFY                      16
%assign KEY_QUERY_VALUE                 1
%assign KEY_READ                        0x20019
%assign KEY_SET_VALUE                   2
%assign KEY_WRITE                       0x20006
%assign PROCESS_ALL_ACCESS              0x1F0FFF
%assign PROCESS_CREATE_PROCESS          128
%assign PROCESS_CREATE_THREAD           2
%assign PROCESS_DUP_HANDLE              64
%assign PROCESS_QUERY_INFORMATION       1024
%assign PROCESS_SET_INFORMATION         512
%assign PROCESS_TERMINATE               1
%assign PROCESS_VM_OPERATION            8
%assign PROCESS_VM_READ                 16
%assign PROCESS_VM_WRITE                32
%assign THREAD_ALL_ACCESS               0x1F03FF
%assign THREAD_DIRECT_IMPERSONATION     512
%assign THREAD_GET_CONTEXT              8
%assign THREAD_IMPERSONATE              256
%assign THREAD_QUERY_INFORMATION        64
%assign THREAD_SET_CONTEXT              16
%assign THREAD_SET_INFORMATION          32
%assign THREAD_SET_THREAD_TOKEN         128
%assign THREAD_SUSPEND_RESUME           2
%assign THREAD_TERMINATE                1

;       EnumSystemCodePages
%assign CP_INSTALLED                    1
%assign CP_SUPPORTED                    2
;=========================================
;       EnumSystemLocales
%assign LCID_INSTALLED                  1
%assign LCID_SUPPORTED                  2
;       EraseTape
%assign TAPE_ERASE_LONG                 0x1
%assign TAPE_ERASE_SHORT                0
;       ExtTextOut
%assign ETO_CLIPPED                     4
%assign ETO_GLYPH_INDEX                 16
%assign ETO_OPAQUE                      2
%assign ETO_RTLREADING                  128
;       GdiComment
%assign GDICOMMENT_WINDOWS_METAFILE     -2147483647
%assign GDICOMMENT_BEGINGROUP           2
%assign GDICOMMENT_ENDGROUP             3
%assign GDICOMMENT_MULTIFORMATS         1073741828
%assign GDICOMMENT_IDENTIFIER           1128875079
;       GetComputerName
%assign MAX_COMPUTERNAME_LENGTH         15
;       GetDateFormat
%assign DATE_SHORTDATE                  1
%assign DATE_LONGDATE                   2
%assign DATE_USE_ALT_CALENDAR           4
;       GetDriveType
%assign DRIVE_REMOVABLE                 2
%assign DRIVE_FIXED                     3
%assign DRIVE_REMOTE                    4
%assign DRIVE_CDROM                     5
%assign DRIVE_RAMDISK                   6
%assign DRIVE_UNKNOWN                   0
%assign DRIVE_NO_ROOT_DIR               1
;       GetExceptionCode
%assign EXCEPTION_ACCESS_VIOLATION      0xC0000005
%assign EXCEPTION_BREAKPOINT            0x80000003
%assign EXCEPTION_DATATYPE_MISALIGNMENT 0x80000002
%assign EXCEPTION_SINGLE_STEP           0x80000004
%assign EXCEPTION_ARRAY_BOUNDS_EXCEEDED 0xC000008C
%assign EXCEPTION_FLT_DENORMAL_OPERAND  0xC000008D
%assign EXCEPTION_FLT_DIVIDE_BY_ZERO    0xC000008E
%assign EXCEPTION_FLT_INEXACT_RESULT    0xC000008F
%assign EXCEPTION_FLT_INVALID_OPERATION 0xC0000090
%assign EXCEPTION_FLT_OVERFLOW          0xC0000091
%assign EXCEPTION_FLT_STACK_CHECK       0xC0000092
%assign EXCEPTION_FLT_UNDERFLOW         0xC0000093
%assign EXCEPTION_INT_DIVIDE_BY_ZERO    0xC0000094
%assign EXCEPTION_INT_OVERFLOW          0xC0000095
%assign EXCEPTION_PRIV_INSTRUCTION      0xC0000096
%assign EXCEPTION_NONCONTINUABLE_EXCEPTION      0xC0000025
%assign EXCEPTION_NONCONTINUABLE        0x1
%assign EXCEPTION_STACK_OVERFLOW        0xC00000FD
%assign EXCEPTION_INVALID_DISPOSITION   0xC0000026
;       GetHandleInformation
%assign HANDLE_FLAG_INHERIT             1
%assign HANDLE_FLAG_PROTECT_FROM_CLOSE  2
;       GetMenuDefaultItem
%assign GMDI_GOINTOPOPUPS               0x2
%assign GMDI_USEDISABLED                0x1
;       GetTapePosition
%assign TAPE_ABSOLUTE_POSITION          0
%assign TAPE_LOGICAL_POSITION           0x1
;       GetThreadPriority
%assign THREAD_PRIORITY_ABOVE_NORMAL    1
%assign THREAD_PRIORITY_BELOW_NORMAL    -1
%assign THREAD_PRIORITY_HIGHEST         2
%assign THREAD_PRIORITY_IDLE            -15
%assign THREAD_PRIORITY_LOWEST          -2
%assign THREAD_PRIORITY_NORMAL          0
%assign THREAD_PRIORITY_TIME_CRITICAL   15
%assign THREAD_PRIORITY_ERROR_RETURN    2147483647
%assign TLS_MINIMUM_AVAILABLE           64
;	SetProcessWorkingSetSize
%assign	PROCESS_SET_QUOTA		256
;	WNetGetResourceInformation, WNetGetResourceParent
%assign	WN_BAD_NETNAME			0x43
%assign	WN_EXTENDED_ERROR		0x4B8
%assign	WN_MORE_DATA			0xEA
%assign	WN_NO_NETWORK			0x4C6
%assign	WN_SUCCESS			0
%assign	WN_ACCESS_DENIED		0x5
%assign	WN_BAD_PROVIDER			0x4B4
%assign	WN_NOT_AUTHENTICATED		0x4DC
;	WNetGetUniversalName
%assign	UNIVERSAL_NAME_INFO_LEVEL	1
%assign	REMOTE_NAME_INFO_LEVEL		2




;	GetExitCodeThread
%assign	STILL_ACTIVE			0x103

;	COMMPROP structure
%assign	SP_SERIALCOMM			0x1
%assign	BAUD_075			0x1
%assign	BAUD_110			0x2
%assign	BAUD_134_5			0x4
%assign	BAUD_150			0x8
%assign	BAUD_300			0x10
%assign	BAUD_600			0x20
%assign	BAUD_1200			0x40
%assign	BAUD_1800			0x80
%assign	BAUD_2400			0x100
%assign	BAUD_4800			0x200
%assign	BAUD_7200			0x400
%assign	BAUD_9600			0x800
%assign	BAUD_14400			0x1000
%assign	BAUD_19200			0x2000
%assign	BAUD_38400			0x4000
%assign	BAUD_56K			0x8000
%assign	BAUD_57600			0x40000
%assign	BAUD_115200			0x20000
%assign	BAUD_128K			0x10000
%assign	BAUD_USER			0x10000000
%assign	PST_FAX				0x21
%assign	PST_LAT				0x101
%assign	PST_MODEM			0x6
%assign	PST_NETWORK_BRIDGE		0x100
%assign	PST_PARALLELPORT		0x2
%assign	PST_RS232			0x1
%assign	PST_RS422			0x3
%assign	PST_RS423			0x4
%assign	PST_RS449			0x5
%assign	PST_SCANNER			0x22
%assign	PST_TCPIP_TELNET		0x102
%assign	PST_UNSPECIFIED			0
%assign	PST_X25				0x103
%assign	PCF_16BITMODE			0x200
%assign	PCF_DTRDSR			0x1
%assign	PCF_INTTIMEOUTS			0x80
%assign	PCF_PARITY_CHECK		0x8
%assign	PCF_RLSD			0x4
%assign	PCF_RTSCTS			0x2
%assign	PCF_SETXCHAR			0x20
%assign	PCF_SPECIALCHARS		0x100
%assign	PCF_TOTALTIMEOUTS		0x40
%assign	PCF_XONXOFF			0x10
%assign	SP_BAUD				0x2
%assign	SP_DATABITS			0x4
%assign	SP_HANDSHAKING			0x10
%assign	SP_PARITY			0x1
%assign	SP_PARITY_CHECK			0x20
%assign	SP_RLSD				0x40
%assign	SP_STOPBITS			0x8
%assign	DATABITS_5			1
%assign	DATABITS_6			2
%assign	DATABITS_7			4
%assign	DATABITS_8			8
%assign	DATABITS_16			16
%assign	DATABITS_16X			32
%assign	STOPBITS_10			1
%assign	STOPBITS_15			2
%assign	STOPBITS_20			4
%assign	PARITY_NONE			256
%assign	PARITY_ODD			512
%assign	PARITY_EVEN			1024
%assign	PARITY_MARK			2048
%assign	PARITY_SPACE			4096
%assign	COMMPROP_INITIALIZED		0xE73CF52E
;	Win32s
%assign	HINSTANCE_ERROR			32

;	PROPSHEETPAGE structure
%assign	MAXPROPPAGES			100
%assign	PSP_DEFAULT			0
%assign	PSP_DLGINDIRECT			1
%assign	PSP_HASHELP			32
%assign	PSP_USECALLBACK			128
%assign	PSP_USEHICON			2
%assign	PSP_USEICONID			4
%assign	PSP_USEREFPARENT		64
%assign	PSP_USETITLE			8
%assign	PSP_RTLREADING			16

;	PROPSHEETHEADER structure
%assign	PSH_DEFAULT			0
%assign	PSH_HASHELP			512
%assign	PSH_MODELESS			1024
%assign	PSH_NOAPPLYNOW			128
%assign	PSH_PROPSHEETPAGE		8
%assign	PSH_PROPTITLE			1
%assign	PSH_USECALLBACK			256
%assign	PSH_USEHICON			2
%assign	PSH_USEICONID			4
%assign	PSH_USEPSTARTPAGE		64
%assign	PSH_WIZARD			32
%assign	PSH_RTLREADING			2048
%assign	PSCB_INITIALIZED		1
%assign	PSCB_PRECREATE			2


;	PSN_APPLY message
%assign	PSNRET_NOERROR			0
%assign	PSNRET_INVALID_NOCHANGEPAGE	2

;	property sheet
%assign	PSBTN_APPLYNOW			4
%assign	PSBTN_BACK			0
%assign	PSBTN_CANCEL			5
%assign	PSBTN_FINISH			2
%assign	PSBTN_HELP			6
%assign	PSBTN_NEXT			1
%assign	PSBTN_OK			3
%assign	PSWIZB_BACK			1
%assign	PSWIZB_NEXT			2
%assign	PSWIZB_FINISH			4
%assign	PSWIZB_DISABLEDFINISH		8
%assign	ID_PSREBOOTSYSTEM		3
%assign	ID_PSRESTARTWINDOWS		2
%assign	WIZ_BODYCX			184
%assign	WIZ_BODYX			92
%assign	WIZ_CXBMP			80
%assign	WIZ_CXDLG			276
%assign	WIZ_CYDLG			140

;	GCP_RESULTS structure
%assign	GCPCLASS_ARABIC			2
%assign	GCPCLASS_HEBREW			2
%assign	GCPCLASS_LATIN			1
%assign	GCPCLASS_LATINNUMBER		5
%assign	GCPCLASS_LOCALNUMBER		4
%assign	GCPCLASS_LATINNUMERICSEPARATOR	7
%assign	GCPCLASS_LATINNUMERICTERMINATOR	6
%assign	GCPCLASS_NEUTRAL		3
%assign	GCPCLASS_NUMERICSEPARATOR	8
%assign	GCPCLASS_PREBOUNDLTR		128
%assign	GCPCLASS_PREBOUNDRTL		64
%assign	GCPCLASS_POSTBOUNDLTR		32
%assign	GCPCLASS_POSTBOUNDRTL		16
%assign	GCPGLYPH_LINKBEFORE		32768
%assign	GCPGLYPH_LINKAFTER		16384

;	DRAWITEMSTRUCT structure
%assign	ODT_BUTTON			4
%assign	ODT_COMBOBOX			3
%assign	ODT_LISTBOX			2
%assign	ODT_LISTVIEW			102
%assign	ODT_MENU			1
%assign	ODT_STATIC			5
%assign	ODT_TAB				101
%assign	ODT_HEADER			100
%assign	ODA_DRAWENTIRE			1
%assign	ODA_FOCUS			4
%assign	ODA_SELECT			2
%assign	ODS_CHECKED			8
%assign	ODS_COMBOBOXEDIT		4096
%assign	ODS_DEFAULT			32
%assign	ODS_DISABLED			4
%assign	ODS_FOCUS			16
%assign	ODS_GRAYED			2
%assign	ODS_SELECTED			1

;	common control window classes
%define	ANIMATE_CLASS			"SysAnimate32"
%define	HOTKEY_CLASS			"msctls_hotkey32"
%define	PROGRESS_CLASS			"msctls_progress32"
%define	STATUSCLASSNAME			"msctls_statusbar32"
%define	TOOLBARCLASSNAME		"ToolbarWindow32"
%define	TOOLTIPS_CLASS			"tooltips_class32"
%define	TRACKBAR_CLASS			"msctls_trackbar32"
%define	UPDOWN_CLASS			"msctls_updown32"
%define	WC_HEADER			"SysHeader32"
%define	WC_LISTVIEW			"SysListView32"
%define	WC_TABCONTROL			"SysTabControl32"
%define	WC_TREEVIEW			"SysTreeView32"
;	FSCTL_SET_COMPRESSION
%assign	COMPRESSION_FORMAT_NONE		0
%assign	COMPRESSION_FORMAT_DEFAULT	1
%assign	COMPRESSION_FORMAT_LZNT1	2
;	TAPE_GET_DRIVE_PARAMETERS structure
%assign	TAPE_DRIVE_COMPRESSION		131072
%assign	TAPE_DRIVE_ECC			65536
%assign	TAPE_DRIVE_ERASE_BOP_ONLY	64
%assign	TAPE_DRIVE_ERASE_LONG		32
%assign	TAPE_DRIVE_ERASE_IMMEDIATE	128
%assign	TAPE_DRIVE_ERASE_SHORT		16
%assign	TAPE_DRIVE_FIXED		1
%assign	TAPE_DRIVE_FIXED_BLOCK		1024
%assign	TAPE_DRIVE_INITIATOR		4
%assign	TAPE_DRIVE_PADDING		262144
%assign	TAPE_DRIVE_GET_ABSOLUTE_BLK	1048576
%assign	TAPE_DRIVE_GET_LOGICAL_BLK	2097152
%assign	TAPE_DRIVE_REPORT_SMKS		524288
%assign	TAPE_DRIVE_SELECT		2
%assign	TAPE_DRIVE_SET_EOT_WZ_SIZE	4194304
%assign	TAPE_DRIVE_TAPE_CAPACITY	256
%assign	TAPE_DRIVE_TAPE_REMAINING	512
%assign	TAPE_DRIVE_VARIABLE_BLOCK	2048
%assign	TAPE_DRIVE_WRITE_PROTECT	4096
%assign	TAPE_DRIVE_ABS_BLK_IMMED	-2147475456
%assign	TAPE_DRIVE_ABSOLUTE_BLK		-2147479552
%assign	TAPE_DRIVE_END_OF_DATA		-2147418112
%assign	TAPE_DRIVE_FILEMARKS		-2147221504
%assign	TAPE_DRIVE_LOAD_UNLOAD		-2147483647
%assign	TAPE_DRIVE_LOAD_UNLD_IMMED	-2147483616
%assign	TAPE_DRIVE_LOCK_UNLOCK		-2147483644
%assign	TAPE_DRIVE_LOCK_UNLK_IMMED	-2147483520
%assign	TAPE_DRIVE_LOG_BLK_IMMED	-2147450880
%assign	TAPE_DRIVE_LOGICAL_BLK		-2147467264
%assign	TAPE_DRIVE_RELATIVE_BLKS	-2147352576
%assign	TAPE_DRIVE_REVERSE_POSITION	-2143289344
%assign	TAPE_DRIVE_REWIND_IMMEDIATE	-2147483640
%assign	TAPE_DRIVE_SEQUENTIAL_FMKS	-2146959360
%assign	TAPE_DRIVE_SEQUENTIAL_SMKS	-2145386496
%assign	TAPE_DRIVE_SET_BLOCK_SIZE	-2147483632
%assign	TAPE_DRIVE_SET_COMPRESSION	-2147483136
%assign	TAPE_DRIVE_SET_ECC		-2147483392
%assign	TAPE_DRIVE_SET_PADDING		-2147482624
%assign	TAPE_DRIVE_SET_REPORT_SMKS	-2147481600
%assign	TAPE_DRIVE_SETMARKS		-2146435072
%assign	TAPE_DRIVE_SPACE_IMMEDIATE	-2139095040
%assign	TAPE_DRIVE_TENSION		-2147483646
%assign	TAPE_DRIVE_TENSION_IMMED	-2147483584
%assign	TAPE_DRIVE_WRITE_FILEMARKS	-2113929216
%assign	TAPE_DRIVE_WRITE_LONG_FMKS	-2013265920
%assign	TAPE_DRIVE_WRITE_MARK_IMMED	-1879048192
%assign	TAPE_DRIVE_WRITE_SETMARKS	-2130706432
%assign	TAPE_DRIVE_WRITE_SHORT_FMKS	-2080374784

;	virtual key codes
%assign	VK_LBUTTON			1
%assign	VK_RBUTTON			2
%assign	VK_CANCEL			3
%assign	VK_MBUTTON			4
%assign	VK_BACK				8
%assign	VK_TAB				9
%assign	VK_CLEAR			12
%assign	VK_RETURN			13
%assign	VK_SHIFT			16
%assign	VK_CONTROL			17
%assign	VK_MENU				18
%assign	VK_PAUSE			19
%assign	VK_CAPITAL			20
%assign	VK_ESCAPE			27
%assign	VK_SPACE			32
%assign	VK_PRIOR			33
%assign	VK_NEXT				34
%assign	VK_END				35
%assign	VK_HOME				36
%assign	VK_LEFT				37
%assign	VK_UP				38
%assign	VK_RIGHT			39
%assign	VK_DOWN				40
%assign	VK_SELECT			41
%assign	VK_PRINT			42
%assign	VK_EXECUTE			43
%assign	VK_SNAPSHOT			44
%assign	VK_INSERT			45
%assign	VK_DELETE			46
%assign	VK_HELP				47
%assign	VK_0				48
%assign	VK_1				49
%assign	VK_2				50
%assign	VK_3				51
%assign	VK_4				52
%assign	VK_5				53
%assign	VK_6				54
%assign	VK_7				55
%assign	VK_8				56
%assign	VK_9				57
%assign	VK_A				65
%assign	VK_B				66
%assign	VK_C				67
%assign	VK_D				68
%assign	VK_E				69
%assign	VK_F				70
%assign	VK_G				71
%assign	VK_H				72
%assign	VK_I				73
%assign	VK_J				74
%assign	VK_K				75
%assign	VK_L				76
%assign	VK_M				77
%assign	VK_N				78
%assign	VK_O				79
%assign	VK_P				80
%assign	VK_Q				81
%assign	VK_R				82
%assign	VK_S				83
%assign	VK_T				84
%assign	VK_U				85
%assign	VK_V				86
%assign	VK_W				87
%assign	VK_X				88
%assign	VK_Y				89
%assign	VK_Z				90
%assign	VK_NUMPAD0			96
%assign	VK_NUMPAD1			97
%assign	VK_NUMPAD2			98
%assign	VK_NUMPAD3			99
%assign	VK_NUMPAD4			100
%assign	VK_NUMPAD5			101
%assign	VK_NUMPAD6			102
%assign	VK_NUMPAD7			103
%assign	VK_NUMPAD8			104
%assign	VK_NUMPAD9			105
%assign	VK_MULTIPLY			106
%assign	VK_ADD				107
%assign	VK_SEPARATOR			108
%assign	VK_SUBTRACT			109
%assign	VK_DECIMAL			110
%assign	VK_DIVIDE			111
%assign	VK_F1				112
%assign	VK_F2				113
%assign	VK_F3				114
%assign	VK_F4				115
%assign	VK_F5				116
%assign	VK_F6				117
%assign	VK_F7				118
%assign	VK_F8				119
%assign	VK_F9				120
%assign	VK_F10				121
%assign	VK_F11				122
%assign	VK_F12				123
%assign	VK_F13				124
%assign	VK_F14				125
%assign	VK_F15				126
%assign	VK_F16				127
%assign	VK_F17				128
%assign	VK_F18				129
%assign	VK_F19				130
%assign	VK_F20				131
%assign	VK_F21				132
%assign	VK_F22				133
%assign	VK_F23				134
%assign	VK_F24				135

;	GetAsyncKeyState
%assign	VK_NUMLOCK			144
%assign	VK_SCROLL			145
%assign	VK_LSHIFT			160
%assign	VK_LCONTROL			162
%assign	VK_LMENU			164
%assign	VK_RSHIFT			161
%assign	VK_RCONTROL			163
%assign	VK_RMENU			165

;	ImmGetVirtualKey
%assign	VK_PROCESSKEY			229

;	keystroke message flags
%assign	KF_ALTDOWN			8192
%assign	KF_DLGMODE			2048
%assign	KF_EXTENDED			256
%assign	KF_MENUMODE			4096
%assign	KF_REPEAT			16384
%assign	KF_UP				32768


;	MENUITEMINFO structure
%assign	MIIM_CHECKMARKS			8
%assign	MIIM_DATA			32
%assign	MIIM_ID				2
%assign	MIIM_STATE			1
%assign	MIIM_SUBMENU			4
%assign	MIIM_TYPE			16
%assign	MFT_BITMAP			0x4
%assign	MFT_MENUBARBREAK		0x20
%assign	MFT_MENUBREAK			0x40
%assign	MFT_OWNERDRAW			0x100
%assign	MFT_RADIOCHECK			0x200
%assign	MFT_RIGHTJUSTIFY		0x4000
%assign	MFT_SEPARATOR			0x800
%assign	MFT_STRING			0
%assign	MFS_CHECKED			0x8
%assign	MFS_DEFAULT			0x1000
%assign	MFS_DISABLED			0x3
%assign	MFS_ENABLED			0
%assign	MFS_GRAYED			0x3
%assign	MFS_HILITE			0x80
%assign	MFS_UNCHECKED			0
%assign	MFS_UNHILITE			0


;	SERIALKEYS structure
%assign	SERKF_AVAILABLE			2
%assign	SERKF_INDICATOR			4
%assign	SERKF_SERIALKEYSON		1

;	FILTERKEYS structure
%assign	FKF_AVAILABLE			2
%assign	FKF_CLICKON			64
%assign	FKF_FILTERKEYSON		1
%assign	FKF_HOTKEYACTIVE		4
%assign	FKF_HOTKEYSOUND			16
%assign	FKF_CONFIRMHOTKEY		8
%assign	FKF_INDICATOR			32


;	STICKYKEYS structure
%assign	SKF_AUDIBLEFEEDBACK		64
%assign	SKF_AVAILABLE			2
%assign	SKF_CONFIRMHOTKEY		8
%assign	SKF_HOTKEYACTIVE		4
%assign	SKF_HOTKEYSOUND			16
%assign	SKF_INDICATOR			32
%assign	SKF_STICKYKEYSON		1
%assign	SKF_TRISTATE			128
%assign	SKF_TWOKEYSOFF			256

;	MOUSEKEYS structure
%assign	MKF_AVAILABLE			2
%assign	MKF_CONFIRMHOTKEY		8
%assign	MKF_HOTKEYACTIVE		4
%assign	MKF_HOTKEYSOUND			16
%assign	MKF_INDICATOR			32
%assign	MKF_MOUSEKEYSON			1
%assign	MKF_MODIFIERS			64
%assign	MKF_REPLACENUMBERS		128

;	SOUNDSENTRY structure
%assign	SSF_AVAILABLE			2
%assign	SSF_SOUNDSENTRYON		1
%assign	SSTF_BORDER			2
%assign	SSTF_CHARS			1
%assign	SSTF_DISPLAY			3
%assign	SSTF_NONE			0
%assign	SSGF_DISPLAY			3
%assign	SSGF_NONE			0
%assign	SSWF_CUSTOM			4
%assign	SSWF_DISPLAY			3
%assign	SSWF_NONE			0
%assign	SSWF_TITLE			1
%assign	SSWF_WINDOW			2

;	ACCESSTIMEOUT structure
%assign	ATF_ONOFFFEEDBACK		2
%assign	ATF_TIMEOUTON			1

;	HIGHCONTRAST structure
%assign	HCF_AVAILABLE			2
%assign	HCF_CONFIRMHOTKEY		8
%assign	HCF_HIGHCONTRASTON		1
%assign	HCF_HOTKEYACTIVE		4
%assign	HCF_HOTKEYAVAILABLE		64
%assign	HCF_HOTKEYSOUND			16
%assign	HCF_INDICATOR			32


;	TOGGLEKEYS structure
%assign	TKF_AVAILABLE			2
%assign	TKF_CONFIRMHOTKEY		8
%assign	TKF_HOTKEYACTIVE		4
%assign	TKF_HOTKEYSOUND			16
%assign	TKF_TOGGLEKEYSON		1


;	installable policy
%assign	PP_DISPLAYERRORS		1



;	ChoosePixelFormat
%assign	PFD_DRAW_TO_WINDOW		0x4
%assign	PFD_DRAW_TO_BITMAP		0x8
%assign	PFD_SUPPORT_GDI			0x10
%assign	PFD_SUPPORT_OPENGL		0x20
%assign	PFD_DOUBLEBUFFER		0x1
%assign	PFD_STEREO			0x2
%assign	PFD_DOUBLEBUFFER_DONTCARE	0x40000000
%assign	PFD_STEREO_DONTCARE		0x80000000
%assign	PFD_TYPE_RGBA			0
%assign	PFD_TYPE_COLORINDEX		1
%assign	PFD_MAIN_PLANE			0
%assign	PFD_OVERLAY_PLANE		1
%assign	PFD_UNDERLAY_PLANE		-1


;	wglUseFontOutlines
%assign	WGL_FONT_LINES			0
%assign	WGL_FONT_POLYGONS		1

;	PIXELFORMATDESCRIPTOR structure
%assign	PFD_GENERIC_FORMAT		0x40
%assign	PFD_NEED_PALETTE		0x80
%assign	PFD_NEED_SYSTEM_PALETTE		0x100
%assign	PFD_SWAP_COPY			0x400
%assign	PFD_SWAP_EXCHANGE		0x200

;	BEGINNING of shellapi.h stuff from old headers
%assign	SE_ERR_SHARE			26
%assign	SE_ERR_ASSOCINCOMPLETE		27
%assign	SE_ERR_DDETIMEOUT		28
%assign	SE_ERR_DDEFAIL			29
%assign	SE_ERR_DDEBUSY			30
%assign	SE_ERR_NOASSOC			31

;	from ddeml.h in old Cygnus headers
%assign	XCLASS_BOOL			0x1000
%assign	XCLASS_DATA			0x2000
%assign	XCLASS_FLAGS			0x4000
%assign	XCLASS_MASK			0xFC00
%assign	XCLASS_NOTIFICATION		0x8000
%assign	XTYPF_NOBLOCK			0x0002
%assign	XTYP_ADVDATA			0x4010
%assign	XTYP_ADVREQ			0x2022
%assign	XTYP_ADVSTART			0x1030
%assign	XTYP_ADVSTOP			0x8040
%assign	XTYP_CONNECT			0x1062
%assign	XTYP_CONNECT_CONFIRM		0x8072
%assign	XTYP_DISCONNECT			0x80C2
%assign	XTYP_EXECUTE			0x4050
%assign	XTYP_POKE			0x4090
%assign	XTYP_REQUEST			0x20B0
%assign	XTYP_WILDCONNECT		0x20E2
%assign	XTYP_REGISTER			0x80A2
%assign	XTYP_ERROR			0x8002
%assign	XTYP_XACT_COMPLETE		0x8080
%assign	XTYP_UNREGISTER			0x80D2

%assign	DMLERR_DLL_USAGE		0x4004
%assign	DMLERR_INVALIDPARAMETER		0x4006
%assign	DMLERR_NOTPROCESSED		0x4009
%assign	DMLERR_POSTMSG_FAILED		0x400C
%assign	DMLERR_SERVER_DIED		0x400E
%assign	DMLERR_SYS_ERROR		0x400F
%assign	DMLERR_BUSY			0x4001
%assign	DMLERR_DATAACKTIMEOUT		0x4002
%assign	DMLERR_ADVACKTIMEOUT		0x4000
%assign	DMLERR_DLL_NOT_INITIALIZED	0x4003
%assign	DMLERR_LOW_MEMORY		0x4007
%assign	DMLERR_MEMORY_ERROR		0x4008
%assign	DMLERR_POKEACKTIMEOUT		0x400B
%assign	DMLERR_NO_CONV_ESTABLISHED	0x400A
%assign	DMLERR_REENTRANCY		0x400D
%assign	DMLERR_UNFOUND_QUEUE_ID		0x4011
%assign	DMLERR_UNADVACKTIMEOUT		0x4010
%assign	DMLERR_EXECACKTIMEOUT		0x4005
%assign	DDE_FACK			0x8000
%assign	DDE_FNOTPROCESSED		0x0000

%assign	DNS_REGISTER			0x0001
%assign	DNS_UNREGISTER			0x0002
%assign	CP_WINANSI			1004
%assign	CP_WINUNICODE			1200
%assign	APPCLASS_STANDARD		0x00000000
;	end of stuff from ddeml.h in old Cygnus headers
%assign	BKMODE_LAST			2
%assign	CTLCOLOR_MSGBOX		0
%assign	CTLCOLOR_EDIT			1
%assign	CTLCOLOR_LISTBOX		2
%assign	CTLCOLOR_BTN			3
%assign	CTLCOLOR_DLG			4
%assign	CTLCOLOR_SCROLLBAR		5
%assign	CTLCOLOR_STATIC		6
%assign	CTLCOLOR_MAX			7

%assign	META_SETMAPMODE		0x0103
%assign	META_SETWINDOWORG		0x020B
%assign	META_SETWINDOWEXT		0x020C

%assign	POLYFILL_LAST			2

%assign	STATUS_WAIT_0			0x00000000
%assign	STATUS_ABANDONED_WAIT_0		0x00000080
%assign	STATUS_USER_APC			0x000000C0
%assign	STATUS_TIMEOUT			0x00000102
%assign	STATUS_PENDING			0x00000103
%assign	STATUS_GUARD_PAGE_VIOLATION	0x80000001
%assign	STATUS_DATATYPE_MISALIGNMENT	0x80000002
%assign	STATUS_BREAKPOINT		0x80000003
%assign	STATUS_SINGLE_STEP		0x80000004
%assign	STATUS_IN_PAGE_ERROR		0xC0000006
%assign	STATUS_ILLEGAL_INSTRUCTION	0xC000001D
%assign	STATUS_NONCONTINUABLE_EXCEPTION	0xC0000025
%assign	STATUS_INVALID_DISPOSITION	0xC0000026
%assign	STATUS_ARRAY_BOUNDS_EXCEEDED	0xC000008C
%assign	STATUS_FLOAT_DENORMAL_OPERAND	0xC000008D
%assign	STATUS_FLOAT_DIVIDE_BY_ZERO	0xC000008E
%assign	STATUS_FLOAT_INEXACT_RESULT	0xC000008F
%assign	STATUS_FLOAT_INVALID_OPERATION	0xC0000090
%assign	STATUS_FLOAT_OVERFLOW		0xC0000091
%assign	STATUS_FLOAT_STACK_CHECK	0xC0000092
%assign	STATUS_FLOAT_UNDERFLOW		0xC0000093
%assign	STATUS_INTEGER_DIVIDE_BY_ZERO	0xC0000094
%assign	STATUS_INTEGER_OVERFLOW		0xC0000095
%assign	STATUS_PRIVILEGED_INSTRUCTION	0xC0000096
%assign	STATUS_STACK_OVERFLOW		0xC00000FD
%assign	STATUS_CONTROL_C_EXIT		0xC000013A

%assign	PROCESSOR_ARCHITECTURE_INTEL	0
%assign	PROCESSOR_ARCHITECTURE_MIPS	1
%assign	PROCESSOR_ARCHITECTURE_ALPHA	2
%assign	PROCESSOR_ARCHITECTURE_PPC	3

;	used by wxwindows.
%assign	SIZEFULLSCREEN			SIZE_MAXIMIZED
%assign	SIZENORMAL			SIZE_RESTORED
%assign	SIZEICONIC			SIZE_MINIMIZED

;	winnt.h
%assign	SIZE_OF_80387_REGISTERS		80

;	values for contextflags
%assign	CONTEXT_i386			0x10000
%assign	CONTEXT_CONTROL			CONTEXT_i386|1
%assign	CONTEXT_INTEGER			CONTEXT_i386|2
%assign	CONTEXT_SEGMENTS		CONTEXT_i386|4
%assign	CONTEXT_FLOATING_POINT		CONTEXT_i386|8
%assign	CONTEXT_DEBUG_REGISTERS		CONTEXT_i386|0x10
%assign	CONTEXT_FULL	CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_SEGMENTS

%assign	FILTER_TEMP_DUPLICATE_ACCOUNT		0x0001
%assign	FILTER_NORMAL_ACCOUNT			0x0002
%assign	FILTER_INTERDOMAIN_TRUST_ACCOUNT	0x0008
%assign	FILTER_WORKSTATION_TRUST_ACCOUNT	0x0010
%assign	FILTER_SERVER_TRUST_ACCOUNT		0x0020

%assign	LOGON32_LOGON_INTERACTIVE	0x02
%assign	LOGON32_LOGON_BATCH		0x04
%assign	LOGON32_LOGON_SERVICE		0x05
%assign	LOGON32_PROVIDER_DEFAULT	0x00
%assign	LOGON32_PROVIDER_WINNT35	0x01

%assign	QID_SYNC			0xFFFFFFFF

%assign	IMAGE_DOS_SIGNATURE		0x5A4D
%assign	IMAGE_OS2_SIGNATURE		0x454E
%assign	IMAGE_OS2_SIGNATURE_LE		0x454C
%assign	IMAGE_VXD_SIGNATURE		0x454C
%assign	IMAGE_NT_SIGNATURE		0x4550




CBEM_INSERTITEM		equ WM_USER+1
CBEM_SETIMAGELIST		equ WM_USER+2
CBEM_GETIMAGELIST		equ WM_USER+3
CBEM_GETITEM			equ WM_USER+4
CBEM_SETITEM			equ WM_USER+5
CBEM_DELETEITEM		equ CB_DELETESTRING
CBEM_GETCOMBOCONTROL	equ WM_USER+6
CBEM_GETEDITCONTROL	equ WM_USER+7
CBEM_SETEXSTYLE		equ WM_USER+8
CBEM_SETEXTENDEDSTYLE	equ WM_USER+14
CBEM_GETEXSTYLE	 	equ WM_USER+9
CBEM_GETEXTENDEDSTYLE	equ WM_USER+9
CBEM_SETUNICODEFORMAT	equ CCM_SETUNICODEFORMAT
CBEM_GETUNICODEFORMAT	equ CCM_GETUNICODEFORMAT
CBEM_HASEDITCHANGED	equ WM_USER+10
;CBEM_INSERTITEMW		equ WM_USER+11
;CBEM_SETITEMW		equ WM_USER+12
;CBEM_GETITEMW		equ WM_USER+13

CBEIF_TEXT			equ 1
CBEIF_IMAGE			equ 2
CBEIF_SELECTEDIMAGE		equ 4
CBEIF_OVERLAY			equ 8
CBEIF_INDENT			equ 16
CBEIF_LPARAM			equ 32
CBEIF_DI_SETITEM		equ 10000000h




TAB	Equ	9
CR	Equ	13
EOL	Equ	0
LF	Equ	10
%define NEXT_LINE  10,13

MB_USERICON					  equ 80h
;========== [ const.inc end ]=====================


;-----------------------win32api structures-----------------------------
STRUC RECT
.left RESD 1
.top RESD 1
.right RESD 1
.bottom RESD 1
ENDSTRUC

STRUC POINT
.x RESD 1
.y RESD 1
ENDSTRUC

STRUC SIZEL
.x RESD 1
.y RESD 1
ENDSTRUC

STRUC MSG
.hwnd RESD 1
.message RESD 1
.wParam RESD 1
.lParam RESD 1
.time RESD 1
.pt RESB POINT_size
ENDSTRUC

STRUC SID_AND_ATTRIBUTES
.Sid RESD 1
.Attributes RESD 1
ENDSTRUC

STRUC SID_IDENTIFIER_AUTHORITY
.Value RESB 1
ENDSTRUC

STRUC OVERLAPPED
.Internal RESD 1
.InternalHigh RESD 1
.loffset RESD 1
.OffsetHigh RESD 1
.hEvent RESD 1
ENDSTRUC

STRUC SECURITY_ATTRIBUTES
.nLength RESD 1
.lpSecurityDescriptor RESD 1
.bInheritHandle RESD 1
ENDSTRUC

STRUC PROCESS_INFORMATION
.hProcess RESD 1
.hThread RESD 1
.dwProcessId RESD 1
.dwThreadId RESD 1
ENDSTRUC

STRUC FILETIME
.dwLowDateTime RESD 1
.dwHighDateTime RESD 1
ENDSTRUC

STRUC SYSTEMTIME
.wYear RESW 1
.wMonth RESW 1
.wDayOfWeek RESW 1
.wDay RESW 1
.wHour RESW 1
.wMinute RESW 1
.wSecond RESW 1
.wMilliseconds RESW 1
ENDSTRUC

STRUC COMMPROP
.wPacketiLength RESW 1
.wPacketVersion RESW 1
.dwServiceMask RESD 1
.dwReserved1 RESD 1
.dwMaxTxQueue RESD 1
.dwMaxRxQueue RESD 1
.dwMaxBaud RESD 1
.dwProvSubType RESD 1
.dwProvCapabilities RESD 1
.dwSettableParams RESD 1
.dwSettableBaud RESD 1
.wSettableData RESW 1
.wSettableStopParity RESW 1
.dwCurrentTxQueue RESD 1
.dwCurrentRxQueue RESD 1
.dwProvSpec1 RESD 1
.dwProvSpec2 RESD 1
.wcProvChar RESW 1
ENDSTRUC

STRUC COMSTAT
.fCtsHold RESD 1
.fDsrHold RESD 1
.fRlsdHold RESD 1
.fXoffHold RESD 1
.fXoffSent RESD 1
.fEof RESD 1
.fTxim RESD 1
.fReserved RESD 1
.cbInQue RESD 1
.cbOutQue RESD 1
ENDSTRUC

STRUC DCB
.DCBlength RESD 1
.BaudRate RESD 1
.fbits RESD 1
.wReserved RESW 1
.XonLim RESW 1
.XoffLim RESW 1
.ByteSize RESB 1
.Parity RESB 1
.StopBits RESB 1
.XonChar RESB 1
.XoffChar RESB 1
.ErrorChar RESB 1
.EofChar RESB 1
.EvtChar RESB 1
ENDSTRUC

STRUC COMMTIMEOUTS
.ReadIntervalTimeout RESD 1
.ReadTotalTimeoutMultiplier RESD 1
.ReadTotalTimeoutConstant RESD 1
.WriteTotalTimeoutMultiplier RESD 1
.WriteTotalTimeoutConstant RESD 1
ENDSTRUC

STRUC SYSTEM_INFO
.dwOemID RESD 1
.dwPageSize RESD 1
.lpMinimumApplicationAddress RESD 1
.lpMaximumApplicationAddress RESD 1
.dwActiveProcessorMask RESD 1
.dwNumberOrfProcessors RESD 1
.dwProcessorType RESD 1
.dwAllocationGranularity RESD 1
.wProcessorLevel RESW 1
.wProcessorRevision RESW 1
ENDSTRUC

STRUC MEMORYSTATUS
.dwiLength RESD 1
.dwMemoryLoad RESD 1
.dwTotalPhys RESD 1
.dwAvailPhys RESD 1
.dwTotalPageFile RESD 1
.dwAvailPageFile RESD 1
.dwTotalVirtual RESD 1
.dwAvailVirtual RESD 1
ENDSTRUC

STRUC TPMPARAMS
.cbSize RESD 1
.rcExclude RESB RECT_size
ENDSTRUC

STRUC GENERIC_MAPPING
.GenericRead RESD 1
.GenericWrite RESD 1
.GenericExecute RESD 1
.GenericAll RESD 1
ENDSTRUC

STRUC LUID
.LowPart RESD 1
.HighPart RESD 1
ENDSTRUC

STRUC LUID_AND_ATTRIBUTES
.pLuid RESD 1
.Attributes RESD 1
ENDSTRUC

STRUC ACL
.AclRevision RESB 1
.Sbz1 RESB 1
.AclSize RESW 1
.AceCount RESW 1
.Sbz2 RESW 1
ENDSTRUC

STRUC ACE_HEADER
.AceType RESB 1
.AceFlags RESB 1
.AceSize RESD 1
ENDSTRUC

STRUC ACCESS_ALLOWED_ACE
.Header RESD 1
.imask RESD 1
.SidStart RESD 1
ENDSTRUC

STRUC ACCESS_DENIED_ACE
.Header RESD 1
.imask RESD 1
.SidStart RESD 1
ENDSTRUC

STRUC SYSTEM_AUDIT_ACE
.Header RESD 1
.imask RESD 1
.SidStart RESD 1
ENDSTRUC

STRUC SYSTEM_ALARM_ACE
.Header RESD 1
.imask RESD 1
.SidStart RESD 1
ENDSTRUC

STRUC ACL_REVISION_INFORMATION
.AclRevision RESD 1
ENDSTRUC

STRUC ACL_SIZE_INFORMATION
.AceCount RESD 1
.AclBytesInUse RESD 1
.AclBytesFree RESD 1
ENDSTRUC

STRUC SECURITY_DESCRIPTOR
.Revision RESB 1
.Sbz1 RESB 1
.Control RESD 1
.Owner RESD 1
.lGroup RESD 1
.Sacl RESD 1
.Dacl RESD 1
ENDSTRUC

STRUC PRIVILEGE_SET
.PrivilegeCount RESD 1
.Control RESD 1
.Privilege RESD 1
ENDSTRUC


STRUC EXCEPTION_RECORD
.ExceptionCode RESD 1
.ExceptionFlags RESD 1
.pExceptionRecord RESD 1
.ExceptionAddress RESD 1
.NumberParameters RESD 1
.ExceptionInformation RESD EXCEPTION_MAXIMUM_PARAMETERS ;corrected
ENDSTRUC




STRUC EXCEPTION_DEBUG_INFO
.ExceptionRecord RESB EXCEPTION_RECORD_size ;corrected
.dwFirstChance RESD 1
ENDSTRUC




STRUC CREATE_THREAD_DEBUG_INFO
.hThread RESD 1
.lpThreadLocalBase RESD 1
.lpStartAddress RESD 1
ENDSTRUC



STRUC CREATE_PROCESS_DEBUG_INFO
.hFile RESD 1
.hProcess RESD 1
.hThread RESD 1
.lpBaseOfImage RESD 1
.dwDebugInfoFileOffset RESD 1
.nDebugInfoSize RESD 1
.lpThreadLocalBase RESD 1
.lpStartAddress RESD 1
.lpImageName RESD 1
.fUnicode RESW 1 ;corrected
ENDSTRUC




STRUC EXIT_THREAD_DEBUG_INFO
.dwExitCode RESD 1
ENDSTRUC

STRUC EXIT_PROCESS_DEBUG_INFO
.dwExitCode RESD 1
ENDSTRUC

STRUC LOAD_DLL_DEBUG_INFO
.hFile RESD 1
.lpBaseOfDll RESD 1
.dwDebugInfoFileOffset RESD 1
.nDebugInfoSize RESD 1
.lpImageName RESD 1
.fUnicode RESW 1
ENDSTRUC

STRUC UNLOAD_DLL_DEBUG_INFO
.lpBaseOfDll RESD 1
ENDSTRUC

STRUC OUTPUT_DEBUG_STRING_INFO
.lpDebugStringData RESD 1
.fUnicode RESW 1
.nDebugStringiLength RESW 1
ENDSTRUC

STRUC RIP_INFO
.dwError RESD 1
.dwType RESD 1
ENDSTRUC

STRUC OFSTRUCT
.cBytes RESB 1
.fFixedDisk RESB 1
.nErrCode RESW 1
.Reserved1 RESW 1
.Reserved2 RESW 1
.szPathName RESB 1
ENDSTRUC

STRUC WNDCLASSEX
.cbSize RESD 1
.style RESD 1
.lpfnWndProc RESD 1
.cbClsExtra RESD 1
.cbWndExtra RESD 1
.hInstance RESD 1
.hIcon RESD 1
.hCursor RESD 1
.hbrBackground RESD 1
.lpszMenuName RESD 1
.lpszClassName RESD 1
.hIconSm RESD 1
ENDSTRUC

STRUC WNDCLASS
.style RESD 1
.lpfnWndProc RESD 1
.cbClsExtra RESD 1
.cbWndExtra RESD 1
.hInstance RESD 1
.hIcon RESD 1
.hCursor RESD 1
.hbrBackground RESD 1
.lpszMenuName RESD 1
.lpszClassName RESD 1
ENDSTRUC

STRUC CRITICAL_SECTION
.Par1 RESD 1
.Par2 RESD 1
.Par3 RESD 1
.Par4 RESD 1
.Par5 RESD 1
.Par6 RESD 1
ENDSTRUC

STRUC BY_HANDLE_FILE_INFORMATION
.dwFileAttributes RESD 1
.ftCreationTime RESB FILETIME_size
.ftLastAccessTime RESB FILETIME_size
.ftLastWriteTime RESB FILETIME_size
.dwVolumeSerialNumber RESD 1
.nFileSizeHigh RESD 1
.nFileSizeLow RESD 1
.nNumberOfLinks RESD 1
.nFileIndexHigh RESD 1
.nFileIndexLow RESD 1
ENDSTRUC

STRUC MEMORY_BASIC_INFORMATION
.BaseAddress RESD 1
.AllocationBase RESD 1
.AllocationProtect RESD 1
.RegionSize RESD 1
.State RESD 1
.Protect RESD 1
.lType RESD 1
ENDSTRUC

STRUC EVENTLOGRECORD
.iLength RESD 1
.Reserved RESD 1
.RecordNumber RESD 1
.TimeGenerated RESD 1
.TimeWritten RESD 1
.EventID RESD 1
.EventType RESW 1
.NumStrings RESW 1
.EventCategory RESW 1
.ReservedFlags RESW 1
.ClosingRecordNumber RESD 1
.StringOffset RESD 1
.UserSidiLength RESD 1
.UserSidOffset RESD 1
.DataiLength RESD 1
.DataOffset RESD 1
ENDSTRUC


STRUC TOKEN_GROUPS
.GroupCount RESD 1
.Groups RESD 1
ENDSTRUC

STRUC TOKEN_PRIVILEGES
.PrivilegeCount RESD 1
.Privileges RESD 1
ENDSTRUC



STRUC FLOATING_SAVE_AREA
.ControlWord RESD 1
.StatusWord RESD 1
.TagWord RESD 1
.ErrorOffset RESD 1
.ErrorSelector RESD 1
.DataOffset RESD 1
.DataSelector RESD 1
.RegisterArea RESB 80 ;corrected
.Cr0NpxState RESD 1
ENDSTRUC

STRUC CONTEXT
.ContextFlags RESD 1
.iDr0 RESD 1
.iDr1 RESD 1
.iDr2 RESD 1
.iDr3 RESD 1
.iDr6 RESD 1
.iDr7 RESD 1
.FloatSave RESB FLOATING_SAVE_AREA_size ;corrected
.regGs RESD 1
.regFs RESD 1
.regEs RESD 1
.regDs RESD 1
.regEdi RESD 1
.regEsi RESD 1
.regEbx RESD 1
.regEdx RESD 1
.regEcx RESD 1
.regEax RESD 1
.regEbp RESD 1
.regEip RESD 1
.regCs RESD 1
.regFlag RESD 1
.regEsp RESD 1
.regSs RESD 1
ENDSTRUC



STRUC EXCEPTION_POINTERS
.pExceptionRecord RESD 1
.ContextRecord RESD 1
ENDSTRUC

STRUC LDT_BYTES
.BaseMid RESB 1
.Flags1 RESB 1
.Flags2 RESB 1
.BaseHi RESB 1
ENDSTRUC

STRUC LDT_ENTRY
.LimitLow RESW 1
.BaseLow RESW 1
.HiWord RESD 1
ENDSTRUC

STRUC TIME_ZONE_INFORMATION
.Bias RESD 1
.StandardName RESW 1
.StandardDate RESD 1
.StandardBias RESD 1
.DaylightName RESW 1
.DaylightDate RESD 1
.DaylightBias RESD 1
ENDSTRUC

STRUC WIN32_STREAM_ID
.dwStreamID RESD 1
.dwStreamAttributes RESD 1
.dwStreamSizeLow RESD 1
.dwStreamSizeHigh RESD 1
.dwStreamNameSize RESD 1
.cStreamName RESB 1
ENDSTRUC

STRUC STARTUPINFO
.cb RESD 1
.lpReserved RESD 1
.lpDesktop RESD 1
.lpTitle RESD 1
.dwX RESD 1
.dwY RESD 1
.dwXSize RESD 1
.dwYSize RESD 1
.dwXCountChars RESD 1
.dwYCountChars RESD 1
.dwFillAttribute RESD 1
.dwFlags RESD 1
.wShowWindow RESW 1
.cbReserved2 RESW 1
.lpReserved2 RESD 1
.hStdInput RESD 1
.hStdOutput RESD 1
.hStdError RESD 1
ENDSTRUC

STRUC WIN32_FIND_DATA
.dwFileAttributes RESD 1
.ftCreationTime RESB FILETIME_size
.ftLastAccessTime RESB FILETIME_size
.ftLastWriteTime RESB FILETIME_size
.nFileSizeHigh RESD 1
.nFileSizeLow RESD 1
.dwReserved0 RESD 1
.dwReserved1 RESD 1
.cFileName RESB MAX_PATH
.cAlternate RESB 14
ENDSTRUC

STRUC CPINFO
.MaxCharSize RESD 1
.DefaultChar RESB 1
.LeadByte RESB 1
ENDSTRUC

STRUC NUMBERFMT
.NumDigits RESD 1
.LeadingZero RESD 1
.Grouping RESD 1
.lpDecimalSep RESD 1
.lpThousandSep RESD 1
.NegativeOrder RESD 1
ENDSTRUC

STRUC CURRENCYFMT
.NumDigits RESD 1
.LeadingZero RESD 1
.Grouping RESD 1
.lpDecimalSep RESD 1
.lpThousandSep RESD 1
.NegativeOrder RESD 1
.PositiveOrder RESD 1
.lpCurrencySymbol RESD 1
ENDSTRUC

STRUC COORD
.x RESW 1
.y RESW 1
ENDSTRUC

STRUC SMALL_RECT
.left RESW 1
.top RESW 1
.right RESW 1
.bottom RESW 1
ENDSTRUC

STRUC KEY_EVENT_RECORD
.bKeyDown RESD 1
.wRepeatCount RESW 1
.wVirtualKeyCode RESW 1
.wVirtualScanCode RESW 1
.uChar RESW 1
.dwControlKeyState RESD 1
ENDSTRUC

STRUC MOUSE_EVENT_RECORD
.dwMousePosition RESD 1
.dwButtonState RESD 1
.dwControlKeyState RESD 1
.dwEventFlags RESD 1
ENDSTRUC

STRUC WINDOW_BUFFER_SIZE_RECORD
.dwSize RESD 1
ENDSTRUC

STRUC MENU_EVENT_RECORD
.dwCommandId RESD 1
ENDSTRUC

STRUC FOCUS_EVENT_RECORD
.bSetFocus RESD 1
ENDSTRUC

STRUC CHAR_INFO
.Char RESW 1
.Attributes RESW 1
ENDSTRUC

STRUC CONSOLE_SCREEN_BUFFER_INFO
.dwSize RESD 1
.dwCursorPosition RESD 1
.wAttributes RESW 1
.srWindow RESB SMALL_RECT_size
.dwMaximumWindowSize RESD 1
ENDSTRUC

STRUC CONSOLE_CURSOR_INFO
.dwSize RESD 1
.bVisible RESD 1
ENDSTRUC

STRUC XFORM
.eM11 RESQ 1
.eM12 RESQ 1
.eM21 RESQ 1
.eM22 RESQ 1
.ex RESQ 1
.ey RESQ 1
ENDSTRUC

STRUC BITMAP
.bmType RESD 1
.bmWidth RESD 1
.bmHeight RESD 1
.bmWidthBytes RESD 1
.bmPlanes RESW 1
.bmBitsPixel RESW 1
.bmBits RESD 1
ENDSTRUC

STRUC RGBTRIPLE
.rgbtBlue RESB 1
.rgbtGreen RESB 1
.rgbtRed RESB 1
ENDSTRUC

STRUC RGBQUAD
.rgbBlue RESB 1
.rgbGreen RESB 1
.rgbRed RESB 1
.rgbReserved RESB 1
ENDSTRUC

STRUC BITMAPCOREHEADER
.bcSize RESD 1
.bcWidth RESW 1
.bcHeight RESW 1
.bcPlanes RESW 1
.bcBitCount RESW 1
ENDSTRUC

STRUC BITMAPINFOHEADER
.biSize RESD 1
.biWidth RESD 1
.biHeight RESD 1
.biPlanes RESW 1
.biBitCount RESW 1
.biCompression RESD 1
.biSizeImage RESD 1
.biXPelsPerMeter RESD 1
.biYPelsPerMeter RESD 1
.biClrUsed RESD 1
.biClrImportant RESD 1
ENDSTRUC

STRUC BITMAPINFO
.bmiHeader RESD 1
.bmiColors RESD 1
ENDSTRUC

STRUC BITMAPCOREINFO
.bmciHeader RESD 1
.bmciColors RESD 1
ENDSTRUC

STRUC BITMAPFILEHEADER
.bfType RESW 1
.bfSize RESD 1
.bfReserved1 RESW 1
.bfReserved2 RESW 1
.bfOffBits RESD 1
ENDSTRUC

STRUC HANDLETABLE
.objectHandle RESD 1
ENDSTRUC

STRUC METARECORD
.rdSize RESD 1
.rdFunction RESW 1
.rdParm1 RESW 1
ENDSTRUC

STRUC METAFILEPICT
.imm RESD 1
.xExt RESD 1
.yExt RESD 1
.hMF RESD 1
ENDSTRUC

STRUC METAHEADER
.mtType RESW 1
.mtHeaderSize RESW 1
.mtVersion RESW 1
.mtSize RESD 1
.mtNoObjects RESW 1
.mtMaxRecord RESD 1
.mtNoParameters RESW 1
ENDSTRUC

STRUC ENHMETARECORD
.iType RESD 1
.nSize RESD 1
.dParm1 RESD 1
ENDSTRUC

STRUC ENHMETAHEADER
.iType RESD 1
.nSize RESD 1
.rclBounds RESB RECT_size
.rclFrame RESB RECT_size
.dSignature RESD 1
.nVersion RESD 1
.nBytes RESD 1
.nRecords RESD 1
.nHandles RESW 1
.sReserved RESW 1
.nDescription RESD 1
.offDescription RESD 1
.nPalEntries RESD 1
.szlDevice RESD 1
.szlMillimeters RESD 1
ENDSTRUC

STRUC TEXTMETRIC
.tmHeight RESD 1
.tmAscent RESD 1
.tmDescent RESD 1
.tmInternalLeading RESD 1
.tmExternalLeading RESD 1
.tmAveCharWidth RESD 1
.tmMaxCharWidth RESD 1
.tmWeight RESD 1
.tmOverhang RESD 1
.tmDigitizedAspectX RESD 1
.tmDigitizedAspectY RESD 1
.tmFirstChar RESB 1
.tmLastChar RESB 1
.tmDefaultChar RESB 1
.tmBreakChar RESB 1
.tmItalic RESB 1
.tmUnderlined RESB 1
.tmStruckOut RESB 1
.tmPitchAndFamily RESB 1
.tmCharSet RESB 1
ENDSTRUC

STRUC NEWTEXTMETRIC
.tmHeight RESD 1
.tmAscent RESD 1
.tmDescent RESD 1
.tmInternalLeading RESD 1
.tmExternalLeading RESD 1
.tmAveCharWidth RESD 1
.tmMaxCharWidth RESD 1
.tmWeight RESD 1
.tmOverhang RESD 1
.tmDigitizedAspectX RESD 1
.tmDigitizedAspectY RESD 1
.tmFirstChar RESB 1
.tmLastChar RESB 1
.tmDefaultChar RESB 1
.tmBreakChar RESB 1
.tmItalic RESB 1
.tmUnderlined RESB 1
.tmStruckOut RESB 1
.tmPitchAndFamily RESB 1
.tmCharSet RESB 1
.ntmFlags RESD 1
.ntmSizeEM RESD 1
.ntmCellHeight RESD 1
.ntmAveWidth RESD 1
ENDSTRUC

STRUC PELARRAY
.paXCount RESD 1
.paYCount RESD 1
.paXExt RESD 1
.paYExt RESD 1
.paRGBs RESW 1
ENDSTRUC

STRUC LOGBRUSH
.lbStyle RESD 1
.lbColor RESD 1
.lbHatch RESD 1
ENDSTRUC

STRUC LOGPEN
.lopnStyle RESD 1
.lopnWidth RESD 1
.lopnColor RESD 1
ENDSTRUC

STRUC EXTLOGPEN
.elpPenStyle RESD 1
.elpWidth RESD 1
.elpBrushStyle RESD 1
.elpColor RESD 1
.elpHatch RESD 1
.elpNumEntries RESD 1
.elpStyleEntry RESD 1
ENDSTRUC

STRUC PALETTEENTRY
.peRed RESB 1
.peGreen RESB 1
.peBlue RESB 1
.peFlags RESB 1
ENDSTRUC

STRUC LOGPALETTE
.palVersion RESW 1
.palNumEntries RESW 1
.palPalEntry RESD 1
ENDSTRUC

STRUC LOGFONT
.lfHeight RESD 1
.lfWidth RESD 1
.lfEscapement RESD 1
.lfOrientation RESD 1
.lfWeight RESD 1
.lfItalic RESB 1
.lfUnderline RESB 1
.lfStrikeOut RESB 1
.lfCharSet RESB 1
.lfOutPrecision RESB 1
.lfClipPrecision RESB 1
.lfQuality RESB 1
.lfPitchAndFamily RESB 1
.lfFaceName RESB LF_FACESIZE
ENDSTRUC

STRUC NONCLIENTMETRICS
.cbSize RESD 1
.iBorderWidth RESD 1
.iScrollWidth RESD 1
.iScrollHeight RESD 1
.iCaptionWidth RESD 1
.iCaptionHeight RESD 1
.lfCaptionFont RESD 1
.iSMCaptionWidth RESD 1
.iSMCaptionHeight RESD 1
.lfSMCaptionFont RESD 1
.iMenuWidth RESD 1
.iMenuHeight RESD 1
.lfMenuFont RESD 1
.lfStatusFont RESD 1
.lfMessageFont RESD 1
ENDSTRUC

STRUC ENUMLOGFONT
.elfLogFont RESD 1
.elfFullName RESB 1
.elfStyle RESB 1
ENDSTRUC

STRUC PANOSE
.ulculture RESD 1
.bFamilyType RESB 1
.bSerifStyle RESB 1
.bWeight RESB 1
.bProportion RESB 1
.bContrast RESB 1
.bStrokeVariation RESB 1
.bArmStyle RESB 1
.bLetterform RESB 1
.bMidline RESB 1
.bXHeight RESB 1
ENDSTRUC

STRUC EXTLOGFONT
.elfLogFont RESD 1
.elfFullName RESB 1
.elfStyle RESB 1
.elfVersion RESD 1
.elfStyleSize RESD 1
.elfMatch RESD 1
.elfReserved RESD 1
.elfVendorId RESB 1
.elfCulture RESD 1
.elfPanose RESD 1
ENDSTRUC

STRUC DEVMODE
.dmDeviceName RESB 1
.dmSpecVersion RESW 1
.dmDriverVersion RESW 1
.dmSize RESW 1
.dmDriverExtra RESW 1
.dmFields RESD 1
.dmOrientation RESW 1
.dmPaperSize RESW 1
.dmPaperiLength RESW 1
.dmPaperWidth RESW 1
.dmScale RESW 1
.dmCopies RESW 1
.dmDefaultSource RESW 1
.dmPrintQuality RESW 1
.dmColor RESW 1
.dmDuplex RESW 1
.dmYResolution RESW 1
.dmTTOption RESW 1
.dmCollate RESW 1
.dmFormName RESB CCHFORMNAME
.dmUnusedPadding RESW 1
.dmBitsPerPel RESW 1
.dmPelsWidth RESD 1
.dmPelsHeight RESD 1
.dmDisplayFlags RESD 1
.dmDisplayFrequency RESD 1
ENDSTRUC

STRUC RGNDATAHEADER
.dwSize RESD 1
.iType RESD 1
.nCount RESD 1
.nRgnSize RESD 1
.rcBound RESB RECT_size
ENDSTRUC

STRUC RGNDATA
.rdh RESD 1
.Buffer RESB 1
ENDSTRUC

STRUC ABC
.abcA RESD 1
.abcB RESD 1
.abcC RESD 1
ENDSTRUC

STRUC ABCFLOAT
.abcfA RESQ 1
.abcfB RESQ 1
.abcfC RESQ 1
ENDSTRUC

STRUC OUTLINETEXTMETRIC
.otmSize RESD 1
.otmTextMetrics RESD 1
.otmFiller RESB 1
.otmPanoseNumber RESD 1
.otmfsSelection RESD 1
.otmfsType RESD 1
.otmsCharSlopeRise RESD 1
.otmsCharSlopeRun RESD 1
.otmItalicAngle RESD 1
.otmEMSquare RESD 1
.otmAscent RESD 1
.otmDescent RESD 1
.otmLineGap RESD 1
.otmsCapEmHeight RESD 1
.otmsXHeight RESD 1
.otmrcFontBox RESB RECT_size
.otmMacAscent RESD 1
.otmMacDescent RESD 1
.otmMacLineGap RESD 1
.otmusMinimumPPEM RESD 1
.otmptSubscriptSize RESD 1
.otmptSubscriptOffset RESD 1
.otmptSuperscriptSize RESD 1
.otmptSuperscriptOffset RESD 1
.otmsStrikeoutSize RESD 1
.otmsStrikeoutPosition RESD 1
.otmsUnderscorePosition RESD 1
.otmsUnderscoreSize RESD 1
.otmpFamilyName RESD 1
.otmpFaceName RESD 1
.otmpStyleName RESD 1
.otmpFullName RESD 1
ENDSTRUC

STRUC POLYTEXT
.x RESD 1
.y RESD 1
.n RESD 1
.lpStr RESD 1
.uiFlags RESD 1
.rcl RESB RECT_size
.pdx RESD 1
ENDSTRUC

STRUC FIXED
.fract RESW 1
.Value RESW 1
ENDSTRUC

STRUC MAT2
.eM11 RESD 1
.eM12 RESD 1
.eM21 RESD 1
.eM22 RESD 1
ENDSTRUC

STRUC GLYPHMETRICS
.gmBlackBoxX RESD 1
.gmBlackBoxY RESD 1
.gmptGlyphOrigin RESD 1
.gmCellIncX RESW 1
.gmCellIncY RESW 1
ENDSTRUC

STRUC POINTFX
.x RESD 1
.y RESD 1
ENDSTRUC

STRUC TTPOLYCURVE
.wType RESW 1
.cpfx RESW 1
.apfx RESD 1
ENDSTRUC

STRUC TTPOLYGONHEADER
.cb RESD 1
.dwType RESD 1
.pfxStart RESD 1
ENDSTRUC

STRUC RASTERIZER_STATUS
.nSize RESW 1
.wFlags RESW 1
.nLanguageID RESW 1
ENDSTRUC

STRUC COLORADJUSTMENT
.caSize RESW 1
.caFlags RESW 1
.caIlluminantIndex RESW 1
.caRedGamma RESW 1
.caGreenGamma RESW 1
.caBlueGamma RESW 1
.caReferenceBlack RESW 1
.caReferenceWhite RESW 1
.caContrast RESW 1
.caBrightness RESW 1
.caColorfulness RESW 1
.caRedGreenTint RESW 1
ENDSTRUC

STRUC DOCINFO
.cbSize RESD 1
.lpszDocName RESD 1
.lpszOutput RESD 1
.lpszDataType RESD 1
.fwType RESD 1
ENDSTRUC

STRUC KERNINGPAIR
.wFirst RESW 1
.wSecond RESW 1
.iKernAmount RESD 1
ENDSTRUC

STRUC emr
.iType RESD 1
.nSize RESD 1
ENDSTRUC

STRUC emrtext
.ptlReference RESB POINT_size
.nchars RESD 1
.offString RESD 1
.fOptions RESD 1
.ircl RESD 1
.offDx RESD 1
ENDSTRUC

STRUC EMR
.iType RESD 1
.nSize RESD 1
ENDSTRUC

STRUC EMRABORTPATH
.emr RESB EMR_size
ENDSTRUC

STRUC EMRBEGINPATH
.emr RESB EMR_size
ENDSTRUC

STRUC EMRENDPATH
.emr RESB EMR_size
ENDSTRUC

STRUC EMRCLOSEFIGURE
.emr RESB EMR_size
ENDSTRUC

STRUC EMRFLATTENPATH
.emr RESB EMR_size
ENDSTRUC

STRUC EMRWIDENPATH
.emr RESB EMR_size
ENDSTRUC

STRUC EMRSETMETARGN
.emr RESB EMR_size
ENDSTRUC

STRUC EMREMRSAVEDC
.emr RESB EMR_size
ENDSTRUC

STRUC EMRREALIZEPALETTE
.emr RESB EMR_size
ENDSTRUC

STRUC EMRSELECTCLIPPATH
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETBKMODE
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC


STRUC EMRSETMAPMODE
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETPOLYFILLMODE
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETROP2
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETSTRETCHBLTMODE
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETTEXTALIGN
.emr RESB EMR_size
.iMode RESD 1
ENDSTRUC

STRUC EMRSETMITERLIMIT
.emr RESB EMR_size
.eMiterLimit RESQ 1
ENDSTRUC

STRUC EMRRESTOREDC
.emr RESB EMR_size
.iRelative RESD 1
ENDSTRUC

STRUC EMRSETARCDIRECTION
.emr RESB EMR_size
.iArcDirection RESD 1
ENDSTRUC

STRUC EMRSETMAPPERFLAGS
.emr RESB EMR_size
.dwFlags RESD 1
ENDSTRUC

STRUC EMRSETTEXTCOLOR
.emr RESB EMR_size
.crColor RESD 1
ENDSTRUC

STRUC EMRSETBKCOLOR
.emr RESB EMR_size
.crColor RESD 1
ENDSTRUC

STRUC EMRSELECTOBJECT
.emr RESB EMR_size
.ihObject RESD 1
ENDSTRUC

STRUC EMRDELETEOBJECT
.emr RESB EMR_size
.ihObject RESD 1
ENDSTRUC

STRUC EMRSELECTPALETTE
.emr RESB EMR_size
.ihPal RESD 1
ENDSTRUC

STRUC EMRRESIZEPALETTE
.emr RESB EMR_size
.ihPal RESD 1
.cEntries RESD 1
ENDSTRUC

STRUC EMRSETPALETTEENTRIES
.emr RESB EMR_size
.ihPal RESD 1
.iStart RESD 1
.cEntries RESD 1
.aPalEntries RESD 1
ENDSTRUC

STRUC EMRSETCOLORADJUSTMENT
.emr RESB EMR_size
.ColorAdjustment RESD 1
ENDSTRUC

STRUC EMRGDICOMMENT
.emr RESB EMR_size
.cbData RESD 1
.xData1 RESW 1
ENDSTRUC

STRUC EMREOF
.emr RESB EMR_size
.nPalEntries RESD 1
.offPalEntries RESD 1
.nSizeLast RESD 1
ENDSTRUC

STRUC EMRLINETO
.emr RESB EMR_size
.ptl RESB POINT_size
ENDSTRUC

STRUC EMRMOVETOEX
.emr RESB EMR_size
.ptl RESB POINT_size
ENDSTRUC

STRUC EMROFFSETCLIPRGN
.emr RESB EMR_size
.ptlOffset RESB POINT_size
ENDSTRUC

STRUC EMRFILLPATH
.emr RESB EMR_size
.rclBounds RESB RECT_size
ENDSTRUC

STRUC EMRSTROKEANDFILLPATH
.emr RESB EMR_size
.rclBounds RESB RECT_size
ENDSTRUC

STRUC EMRSTROKEPATH
.emr RESB EMR_size
.rclBounds RESB RECT_size
ENDSTRUC

STRUC EMREXCLUDECLIPRECT
.emr RESB EMR_size
.rclClip RESB RECT_size
ENDSTRUC

STRUC EMRINTERSECTCLIPRECT
.emr RESB EMR_size
.rclClip RESB RECT_size
ENDSTRUC

STRUC EMRSETVIEWPORTORGEX
.emr RESB EMR_size
.ptlOrigin RESB POINT_size
ENDSTRUC

STRUC EMRSETWINDOWORGEX
.emr RESB EMR_size
.ptlOrigin RESB POINT_size
ENDSTRUC

STRUC EMRSETBRUSHORGEX
.emr RESB EMR_size
.ptlOrigin RESB POINT_size
ENDSTRUC

STRUC EMRSETVIEWPORTEXTEX
.emr RESB EMR_size
.szlExtent RESD 1
ENDSTRUC

STRUC EMRSETWINDOWEXTEX
.emr RESB EMR_size
.szlExtent RESD 1
ENDSTRUC

STRUC EMRSCALEVIEWPORTEXTEX
.emr RESB EMR_size
.xNum RESD 1
.xDenom RESD 1
.yNum RESD 1
.yDemon RESD 1
ENDSTRUC

STRUC EMRSCALEWINDOWEXTEX
.emr RESB EMR_size
.xNum RESD 1
.xDenom RESD 1
.yNum RESD 1
.yDemon RESD 1
ENDSTRUC

STRUC EMRSETWORLDTRANSFORM
.emr RESB EMR_size
.xform RESD 1
ENDSTRUC

STRUC EMRMODIFYWORLDTRANSFORM
.emr RESB EMR_size
.xform RESD 1
.iMode RESD 1
ENDSTRUC

STRUC EMRSETPIXELV
.emr RESB EMR_size
.ptlPixel RESB POINT_size
.crColor RESD 1
ENDSTRUC

STRUC EMREXTFLOODFILL
.emr RESB EMR_size
.ptlStart RESB POINT_size
.crColor RESD 1
.iMode RESD 1
ENDSTRUC

STRUC EMRELLIPSE
.emr RESB EMR_size
.rclBox RESB RECT_size
ENDSTRUC

STRUC EMRRECTANGLE
.emr RESB EMR_size
.rclBox RESB RECT_size
ENDSTRUC

STRUC EMRROUNDRECT
.emr RESB EMR_size
.rclBox RESB RECT_size
.szlCorner RESD 1
ENDSTRUC

STRUC EMRARC
.emr RESB EMR_size
.rclBox RESB RECT_size
.ptlStart RESB POINT_size
.ptlEnd RESB POINT_size
ENDSTRUC

STRUC EMRARCTO
.emr RESB EMR_size
.rclBox RESB RECT_size
.ptlStart RESB POINT_size
.ptlEnd RESB POINT_size
ENDSTRUC

STRUC EMRCHORD
.emr RESB EMR_size
.rclBox RESB RECT_size
.ptlStart RESB POINT_size
.ptlEnd RESB POINT_size
ENDSTRUC

STRUC EMRPIE
.emr RESB EMR_size
.rclBox RESB RECT_size
.ptlStart RESB POINT_size
.ptlEnd RESB POINT_size
ENDSTRUC

STRUC EMRANGLEARC
.emr RESB EMR_size
.ptlCenter RESB POINT_size
.nRadius RESD 1
.eStartAngle RESQ 1
.eSweepAngle RESQ 1
ENDSTRUC

STRUC EMRPOLYLINE
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cptl RESD 1
.aptl1 RESD 1
ENDSTRUC

STRUC EMRPOLYBEZIER
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cptl RESD 1
.aptl1 RESD 1
ENDSTRUC

STRUC EMRPOLYGON
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cptl RESD 1
.aptl1 RESD 1
ENDSTRUC

STRUC EMRPOLYBEZIERTO
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cptl RESD 1
.aptl1 RESD 1
ENDSTRUC

STRUC EMRPOLYLINE16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPOLYBEZIER16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPOLYGON16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPLOYBEZIERTO16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPOLYLINETO16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPOLYDRAW
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cptl RESD 1
.aptl1 RESD 1
.abTypes1 RESW 1
ENDSTRUC

STRUC EMRPOLYDRAW16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cpts RESD 1
.apts RESD 1
.abTypes RESW 1
ENDSTRUC

STRUC EMRPOLYPOLYLINE
.emr RESB EMR_size
.rclBounds RESB RECT_size
.nPolys RESD 1
.cptl RESD 1
.aPolyCounts RESD 1
.aptl RESD 1
ENDSTRUC

STRUC EMRPOLYPOLYGON
.emr RESB EMR_size
.rclBounds RESB RECT_size
.nPolys RESD 1
.cptl RESD 1
.aPolyCounts RESD 1
.aptl1 RESD 1
ENDSTRUC

STRUC EMRPOLYPOLYLINE16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.nPolys RESD 1
.cpts RESD 1
.aPolyCounts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRPOLYPOLYGON16
.emr RESB EMR_size
.rclBounds RESB RECT_size
.nPolys RESD 1
.cpts RESD 1
.aPolyCounts RESD 1
.apts1 RESD 1
ENDSTRUC

STRUC EMRINVERTRGN
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cbRgnData RESD 1
.RgnData1 RESW 1
ENDSTRUC

STRUC EMRPAINTRGN
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cbRgnData RESD 1
.RgnData1 RESW 1
ENDSTRUC

STRUC EMRFILLRGN
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cbRgnData RESD 1
.ihBrush RESD 1
.RgnData RESW 1
ENDSTRUC

STRUC EMRFRAMERGN
.emr RESB EMR_size
.rclBounds RESB RECT_size
.cbRgnData RESD 1
.ihBrush RESD 1
.szlStroke RESD 1
.RgnData1 RESW 1
ENDSTRUC

STRUC EMREXTSELECTCLIPRGN
.emr RESB EMR_size
.cbRgnData RESD 1
.iMode RESD 1
.RgnData RESW 1
ENDSTRUC

STRUC EMREXTTEXTOUT
.emr RESB EMR_size
.rclBounds RESB RECT_size
.iGraphicsMode RESD 1
.exScale RESQ 1
.eyScale RESQ 1
.emrtext RESD 1
ENDSTRUC

STRUC EMRBITBLT
.emr RESB EMR_size
.rclBounds RESB RECT_size
.xDest RESD 1
.yDest RESD 1
.cxDest RESD 1
.cyDest RESD 1
.dwRop RESD 1
.xSrc RESD 1
.ySrc RESD 1
.xformSrc RESD 1
.crBkColorSrc RESD 1
.iUsageSrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
ENDSTRUC

STRUC EMRSTRETCHBLT
.emr RESB EMR_size
.rclBounds RESB RECT_size
.xDest RESD 1
.yDest RESD 1
.cxDest RESD 1
.cyDest RESD 1
.dwRop RESD 1
.xSrc RESD 1
.ySrc RESD 1
.xformSrc RESD 1
.crBkColorSrc RESD 1
.iUsageSrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
.cxSrc RESD 1
.cySrc RESD 1
ENDSTRUC

STRUC EMRMASKBLT
.emr RESB EMR_size
.rclBounds RESB RECT_size
.xDest RESD 1
.yDest RESD 1
.cxDest RESD 1
.cyDest RESD 1
.dwRop RESD 1
.xSrc2 RESD 1
.cyDest2 RESD 1
.dwRop2 RESD 1
.xSrc RESD 1
.ySrc RESD 1
.xformSrc RESD 1
.crBkColorSrc RESD 1
.iUsageSrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
.xMask RESD 1
.yMask RESD 1
.iUsageMask RESD 1
.offBmiMask RESD 1
.cbBmiMask RESD 1
.offBitsMask RESD 1
.cbBitsMask RESD 1
ENDSTRUC

STRUC EMRPLGBLT
.emr RESB EMR_size
.rclBounds RESB RECT_size
.aptlDest3 RESD 1
.xSrc RESD 1
.ySrc RESD 1
.cxSrc RESD 1
.cySrc RESD 1
.xformSrc RESD 1
.crBkColorSrc RESD 1
.iUsageSrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
.xMask RESD 1
.yMask RESD 1
.iUsageMask RESD 1
.offBmiMask RESD 1
.cbBmiMask RESD 1
.offBitsMask RESD 1
.cbBitsMask RESD 1
ENDSTRUC

STRUC EMRSETDIBITSTODEVICE
.emr RESB EMR_size
.rclBounds RESB RECT_size
.xDest RESD 1
.yDest RESD 1
.xSrc RESD 1
.ySrc RESD 1
.cxSrc RESD 1
.cySrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
.iUsageSrc RESD 1
.iStartScan RESD 1
.cScans RESD 1
ENDSTRUC

STRUC EMRSTRETCHDIBITS
.emr RESB EMR_size
.rclBounds RESB RECT_size
.xDest RESD 1
.yDest RESD 1
.xSrc RESD 1
.ySrc RESD 1
.cxSrc RESD 1
.cySrc RESD 1
.offBmiSrc RESD 1
.cbBmiSrc RESD 1
.offBitsSrc RESD 1
.cbBitsSrc RESD 1
.iUsageSrc RESD 1
.dwRop RESD 1
.cxDest RESD 1
.cyDest RESD 1
ENDSTRUC

STRUC EMREXTCREATEFONTINDIRECT
.emr RESB EMR_size
.ihFont RESD 1
.elfw RESD 1
ENDSTRUC

STRUC EMRCREATEPALETTE
.emr RESB EMR_size
.ihPal RESD 1
.lgpl RESD 1
ENDSTRUC

STRUC EMRCREATEPEN
.emr RESB EMR_size
.ihPen RESD 1
.lopn RESD 1
ENDSTRUC

STRUC EMREXTCREATEPEN
.emr RESB EMR_size
.ihPen RESD 1
.offBmi RESD 1
.cbBmi RESD 1
.offBits RESD 1
.cbBits RESD 1
.elp RESD 1
ENDSTRUC

STRUC EMRCREATEBRUSHINDIRECT
.emr RESB EMR_size
.ihBrush RESD 1
.lb RESD 1
ENDSTRUC

STRUC EMRCREATEMONOBRUSH
.emr RESB EMR_size
.ihBrush RESD 1
.iUsage RESD 1
.offBmi RESD 1
.cbBmi RESD 1
.offBits RESD 1
.cbBits RESD 1
ENDSTRUC

STRUC EMRCREATEDIBPATTERNBRUSHPT
.emr RESB EMR_size
.ihBursh RESD 1
.iUsage RESD 1
.offBmi RESD 1
.cbBmi RESD 1
.offBits RESD 1
.cbBits RESD 1
ENDSTRUC

STRUC BITMAPV4HEADER
.bV4Size RESD 1
.bV4Width RESD 1
.bV4Height RESD 1
.bV4Planes RESW 1
.bV4BitCount RESW 1
.bV4V4Compression RESD 1
.bV4SizeImage RESD 1
.bV4XPelsPerMeter RESD 1
.bV4YPelsPerMeter RESD 1
.bV4ClrUsed RESD 1
.bV4ClrImportant RESD 1
.bV4RedMask RESD 1
.bV4GreenMask RESD 1
.bV4BlueMask RESD 1
.bV4AlphaMask RESD 1
.bV4CSType RESD 1
.bV4Endpoints RESD 1
.bV4GammaRed RESD 1
.bV4GammaGreen RESD 1
.bV4GammaBlue RESD 1
ENDSTRUC

STRUC FONTSIGNATURE
.fsUsb4 RESD 1
.fsCsb2 RESD 1
ENDSTRUC

STRUC CHARSETINFO
.ciCharset RESD 1
.ciACP RESD 1
.xlfs RESD 1
ENDSTRUC

STRUC LOCALESIGNATURE
.lsUsb4 RESD 1
.lsCsbDefault RESD 1
.lsCsbSupported RESD 1
ENDSTRUC

STRUC NEWTEXTMETRICEX
.ntmTm RESD 1
.ntmFontSig RESD 1
ENDSTRUC


;-----------------
;STRUC ENUMLOGFONTEX
;.elfLogFont RESD 1
;.elfFullName RESB 1
;.elfStyle RESB 1
;.elfScript RESB 1
;ENDSTRUC
;----------------




STRUC GCP_RESULTS
.lStructSize RESD 1
.lpOutString RESD 1
.lpOrder RESD 1
.lpDX RESD 1
.lpCaretPos RESD 1
.lpClass RESD 1
.lpGlyphs RESD 1
.nGlyphs RESD 1
.nMaxFit RESD 1
ENDSTRUC

STRUC CIEXYZ
.ciexyzX RESD 1
.ciexyzY RESD 1
.ciexyzZ RESD 1
ENDSTRUC

STRUC CIEXYZTRIPLE
.ciexyzRed RESD 1
.ciexyzGreen RESD 1
.ciexyBlue RESD 1
ENDSTRUC

STRUC LOGCOLORSPACE
.lcsSignature RESD 1
.lcsVersion RESD 1
.lcsSize RESD 1
.lcsCSType RESD 1
.lcsIntent RESD 1
.lcsEndPoints RESD 1
.lcsGammaRed RESD 1
.lcsGammaGreen RESD 1
.lcsGammaBlue RESD 1
.lcsFileName RESB MAX_PATH
ENDSTRUC

STRUC EMRSELECTCOLORSPACE
.emr RESB EMR_size
.ihCS RESD 1
ENDSTRUC

STRUC EMRCREATECOLORSPACE
.emr RESB EMR_size
.ihCS RESD 1
.lcs RESD 1
ENDSTRUC

STRUC CBTACTIVATESTRUCT
.fMouse RESD 1
.hWndActive RESD 1
ENDSTRUC

STRUC EVENTMSG
.message RESD 1
.paramL RESD 1
.paramH RESD 1
.time RESD 1
.hwnd RESD 1
ENDSTRUC

STRUC CWPSTRUCT
.lParam RESD 1
.wParam RESD 1
.message RESD 1
.hwnd RESD 1
ENDSTRUC

STRUC DEBUGHOOKINFO
.hModuleHook RESD 1
.Reserved RESD 1
.lParam RESD 1
.wParam RESD 1
.code RESD 1
ENDSTRUC

STRUC MOUSEHOOKSTRUCT
.pt RESB POINT_size
.hwnd RESD 1
.wHitTestCode RESD 1
.dwExtraInfo RESD 1
ENDSTRUC

STRUC MINMAXINFO
.ptReserved RESB POINT_size
.ptMaxSize RESB POINT_size
.ptMaxPosition RESB POINT_size
.ptMinTrackSize RESB POINT_size
.ptMaxTrackSize RESB POINT_size
ENDSTRUC

STRUC COPYDATASTRUCT
.dwData RESD 1
.cbData RESD 1
.lpData RESD 1
ENDSTRUC

STRUC WINDOWPOS
.hwnd RESD 1
.hWndInsertAfter RESD 1
.x RESD 1
.y RESD 1
.lx RESD 1
.ly RESD 1
.flags RESD 1
ENDSTRUC

STRUC ACCEL
.fVirt RESB 1
.key RESW 1
.cmd RESW 1
ENDSTRUC

STRUC PAINTSTRUCT
.hdc RESD 1
.fErase RESD 1
.rcPaint RESB RECT_size
.fRestore RESD 1
.fIncUpdate RESD 1
.rgbReserved RESB 32
ENDSTRUC

STRUC CREATESTRUCT
.lpCreateParams RESD 1
.hInstance RESD 1
.hMenu RESD 1
.hWndParent RESD 1
.ly RESD 1
.lx RESD 1
.y RESD 1
.x RESD 1
.style RESD 1
.lpszName RESD 1
.lpszClass RESD 1
.ExStyle RESD 1
ENDSTRUC

STRUC CBT_CREATEWND
.lpcs RESD 1
.hWndInsertAfter RESD 1
ENDSTRUC

STRUC WINDOWPLACEMENT
.iLength RESD 1
.flags RESD 1
.showCmd RESD 1
.ptMinPosition RESB POINT_size
.ptMaxPosition RESB POINT_size
.rcNormalPosition RESB RECT_size
ENDSTRUC

STRUC MEASUREITEMSTRUCT
.CtlType RESD 1
.CtlID RESD 1
.itemID RESD 1
.itemWidth RESD 1
.itemHeight RESD 1
.itemData RESD 1
ENDSTRUC

STRUC DRAWITEMSTRUCT
.CtlType RESD 1
.CtlID RESD 1
.itemID RESD 1
.itemAction RESD 1
.itemState RESD 1
.hwndItem RESD 1
.hDC RESD 1
.rcItem RESB RECT_size
.itemData RESD 1
ENDSTRUC

STRUC DELETEITEMSTRUCT
.CtlType RESD 1
.CtlID RESD 1
.itemID RESD 1
.hwndItem RESD 1
.itemData RESD 1
ENDSTRUC

STRUC COMPAREITEMSTRUCT
.CtlType RESD 1
.CtlID RESD 1
.hwndItem RESD 1
.itemID1 RESD 1
.itemData1 RESD 1
.itemID2 RESD 1
.itemData2 RESD 1
ENDSTRUC

STRUC DLGTEMPLATE
.style RESD 1
.dwExtendedStyle RESD 1
.cdit RESW 1
.x RESW 1
.y RESW 1
.lx RESW 1
.ly RESW 1
ENDSTRUC

STRUC DLGITEMTEMPLATE
.style RESD 1
.dwExtendedStyle RESD 1
.x RESW 1
.y RESW 1
.lx RESW 1
.ly RESW 1
.id RESW 1
ENDSTRUC

STRUC MENUITEMTEMPLATEHEADER
.versionNumber RESW 1
.loffset RESW 1
ENDSTRUC

STRUC MENUITEMTEMPLATE
.mtOption RESW 1
.mtID RESW 1
.mtString RESB 1
ENDSTRUC

STRUC ICONINFO
.fIcon RESD 1
.xHotspot RESD 1
.yHotspot RESD 1
.hbmMask RESD 1
.hbmColor RESD 1
ENDSTRUC

STRUC MDICREATESTRUCT
.szClass RESD 1
.szTitle RESD 1
.hOwner RESD 1
.x RESD 1
.y RESD 1
.lx RESD 1
.ly RESD 1
.style RESD 1
.lParam RESD 1
ENDSTRUC

STRUC CLIENTCREATESTRUCT
.hWindowMenu RESD 1
.idFirstChild RESD 1
ENDSTRUC

STRUC MULTIKEYHELP
.mkSize RESD 1
.mkKeylist RESB 1
.szKeyphrase RESB 253
ENDSTRUC

STRUC HELPWININFO
.wStructSize RESD 1
.x RESD 1
.y RESD 1
.lx RESD 1
.ly RESD 1
.wMax RESD 1
.rgchMember RESB 2
ENDSTRUC

STRUC DDEACK
.bAppReturnCode RESW 1
.Reserved RESW 1
.fbusy RESW 1
.fack RESW 1
ENDSTRUC

STRUC DDEADVISE
.Reserved RESW 1
.fDeferUpd RESW 1
.fAckReq RESW 1
.cfFormat RESW 1
ENDSTRUC

STRUC DDEDATA
.unused RESW 1
.fresponse RESW 1
.fRelease RESW 1
.Reserved RESW 1
.fAckReq RESW 1
.cfFormat RESW 1
.Value1 RESB 1
ENDSTRUC

STRUC DDEPOKE
.unused RESW 1
.fRelease RESW 1
.fReserved RESW 1
.cfFormat RESW 1
.Value1 RESB 1
ENDSTRUC

STRUC DDELN
.unused RESW 1
.fRelease RESW 1
.fDeferUpd RESW 1
.fAckReq RESW 1
.cfFormat RESW 1
ENDSTRUC

STRUC DDEUP
.unused RESW 1
.fAck RESW 1
.fRelease RESW 1
.fReserved RESW 1
.fAckReq RESW 1
.cfFormat RESW 1
.xRGB1 RESB 1
ENDSTRUC

STRUC HSZPAIR
.hszSvc RESD 1
.hszTopic RESD 1
ENDSTRUC

STRUC SECURITY_QUALITY_OF_SERVICE
.iLength RESD 1
.Impersonationlevel RESW 1
.ContextTrackingMode RESW 1
.EffectiveOnly RESD 1
ENDSTRUC

STRUC CONVCONTEXT
.cb RESD 1
.wFlags RESD 1
.wCountryID RESD 1
.iCodePage RESD 1
.dwLangID RESD 1
.dwSecurity RESD 1
.qos RESD 1
ENDSTRUC

STRUC CONVINFO
.cb RESD 1
.hUser RESD 1
.hConvPartner RESD 1
.hszSvcPartner RESD 1
.hszServiceReq RESD 1
.hszTopic RESD 1
.hszItem RESD 1
.wFmt RESD 1
.wType RESD 1
.wStatus RESD 1
.wConvst RESD 1
.wLastError RESD 1
.hConvList RESD 1
.ConvCtxt RESD 1
.hwnd RESD 1
.hwndPartner RESD 1
ENDSTRUC

STRUC DDEML_MSG_HOOK_DATA
.uiLo RESD 1
.uiHi RESD 1
.cbData RESD 1
.xData RESD 1
ENDSTRUC

STRUC MONMSGSTRUCT
.cb RESD 1
.hwndTo RESD 1
.dwTime RESD 1
.htask RESD 1
.wMsg RESD 1
.wParam RESD 1
.lParam RESD 1
.dmhd RESD 1
ENDSTRUC

STRUC MONCBSTRUCT
.cb RESD 1
.dwTime RESD 1
.htask RESD 1
.dwRet RESD 1
.wType RESD 1
.wFmt RESD 1
.hConv RESD 1
.hsz1 RESD 1
.hsz2 RESD 1
.hData RESD 1
.dwData1 RESD 1
.dwData2 RESD 1
.cc RESD 1
.cbData RESD 1
.xData8 RESD 1
ENDSTRUC

STRUC MONHSZSTRUCT
.cb RESD 1
.fsAction RESD 1
.dwTime RESD 1
.hsz RESD 1
.htask RESD 1
.xstr RESB 1
ENDSTRUC

STRUC MONERRSTRUCT
.cb RESD 1
.wLastError RESD 1
.dwTime RESD 1
.htask RESD 1
ENDSTRUC

STRUC MONLINKSTRUCT
.cb RESD 1
.dwTime RESD 1
.htask RESD 1
.fEstablished RESD 1
.fNoData RESD 1
.hszSvc RESD 1
.hszTopic RESD 1
.hszItem RESD 1
.wFmt RESD 1
.fServer RESD 1
.hConvServer RESD 1
.hConvClient RESD 1
ENDSTRUC

STRUC MONCONVSTRUCT
.cb RESD 1
.fConnect RESD 1
.dwTime RESD 1
.htask RESD 1
.hszSvc RESD 1
.hszTopic RESD 1
.hConvClient RESD 1
.hConvServer RESD 1
ENDSTRUC

STRUC smpte
.hour RESB 1
.minute RESB 1
.sec RESB 1
.frame RESB 1
.fps RESB 1
.dummy RESB 1
.pad RESB 1
ENDSTRUC

STRUC midi
.songptrpos RESD 1
ENDSTRUC

STRUC MMTIME
.wType RESD 1
.u RESD 1
ENDSTRUC

STRUC MIDIEVENT
.dwDeltaTime RESD 1
.dwStreamID RESD 1
.dwEvent RESD 1
.dwParms RESD 1
ENDSTRUC

STRUC MIDISTRMBUFFVER
.dwVersion RESD 1
.dwMid RESD 1
.dwOEMVersion RESD 1
ENDSTRUC

STRUC MIDIPROPTIMEDIV
.cbStruct RESD 1
.dwTimeDiv RESD 1
ENDSTRUC

STRUC MIDIPROPTEMPO
.cbStruct RESD 1
.dwTempo RESD 1
ENDSTRUC

STRUC MIXERCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
.fdwSupport RESD 1
.cDestinations RESD 1
ENDSTRUC

STRUC Target
.dwType RESD 1
.dwDeviceID RESD 1
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
ENDSTRUC

STRUC MIXERLINECONTROLS
.cbStruct RESD 1
.dwLineID RESD 1
.dwControl RESD 1
.cControls RESD 1
.cbmxctrl RESD 1
.pamxctrl RESD 1
ENDSTRUC

STRUC MIXERCONTROLDETAILS
.cbStruct RESD 1
.dwControlID RESD 1
.cChannels RESD 1
.item RESD 1
.cbDetails RESD 1
.paDetails RESD 1
ENDSTRUC

STRUC MIXERCONTROLDETAILS_BOOLEAN
.fValue RESD 1
ENDSTRUC

STRUC MIXERCONTROLDETAILS_SIGNED
.lValue RESD 1
ENDSTRUC

STRUC MIXERCONTROLDETAILS_UNSIGNED
.dwValue RESD 1
ENDSTRUC

STRUC JOYINFOEX
.dwSize RESD 1
.dwFlags RESD 1
.dwXpos RESD 1
.dwYpos RESD 1
.dwZpos RESD 1
.dwRpos RESD 1
.dwUpos RESD 1
.dwVpos RESD 1
.dwButtons RESD 1
.dwButtonNumber RESD 1
.dwPOV RESD 1
.dwReserved1 RESD 1
.dwReserved2 RESD 1
ENDSTRUC

STRUC DRVCONFIGINFO
.dwDCISize RESD 1
.lpszDCISectionName RESD 1
.lpszDCIAliasName RESD 1
.dnDevNode RESD 1
ENDSTRUC

STRUC WAVEHDR
.lpData RESD 1
.dwBufferiLength RESD 1
.dwBytesRecorded RESD 1
.dwUser RESD 1
.dwFlags RESD 1
.dwLoops RESD 1
.lpNext RESD 1
.Reserved RESD 1
ENDSTRUC

STRUC WAVEOUTCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
.dwFormats RESD 1
.wChannels RESW 1
.dwSupport RESD 1
ENDSTRUC

STRUC WAVEINCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
.dwFormats RESD 1
.wChannels RESW 1
ENDSTRUC

STRUC WAVEFORMAT
.wFormatTag RESW 1
.nChannels RESW 1
.nSamplesPerSec RESD 1
.nAvgBytesPerSec RESD 1
.nBlockAlign RESW 1
ENDSTRUC

STRUC PCMWAVEFORMAT
.wf RESD 1
.wBitsPerSample RESW 1
ENDSTRUC

STRUC MIDIOUTCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
.wTechnology RESW 1
.wVoices RESW 1
.wNotes RESW 1
.wChannelMask RESW 1
.dwSupport RESD 1
ENDSTRUC

STRUC MIDIINCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
ENDSTRUC

STRUC MIDIHDR
.lpData RESD 1
.dwBufferiLength RESD 1
.dwBytesRecorded RESD 1
.dwUser RESD 1
.dwFlags RESD 1
.lpNext RESD 1
.Reserved RESD 1
ENDSTRUC

STRUC AUXCAPS
.wMid RESW 1
.wPid RESW 1
.vDriverVersion RESD 1
.szPname RESB MAXPNAMELEN
.wTechnology RESW 1
.dwSupport RESD 1
ENDSTRUC

STRUC TIMECAPS
.wPeriodMin RESD 1
.wPeriodMax RESD 1
ENDSTRUC

STRUC JOYCAPS
.wMid RESW 1
.wPid RESW 1
.szPname RESB MAXPNAMELEN
.wXmin RESW 1
.wXmax RESW 1
.wYmin RESW 1
.wYmax RESW 1
.wZmin RESW 1
.wZmax RESW 1
.wNumButtons RESW 1
.wPeriodMin RESW 1
.wPeriodMax RESW 1
ENDSTRUC

STRUC JOYINFO
.wXpos RESW 1
.wYpos RESW 1
.wZpos RESW 1
.wButtons RESW 1
ENDSTRUC

STRUC MMIOINFO
.dwFlags RESD 1
.fccIOProc RESD 1
.pIOProc RESD 1
.wErrorRet RESD 1
.htask RESD 1
.cchBuffer RESD 1
.pchBuffer RESD 1
.pchNext RESD 1
.pchEndRead RESD 1
.pchEndWrite RESD 1
.lBufOffset RESD 1
.lDiskOffset RESD 1
.adwInfo4 RESD 1
.dwReserved1 RESD 1
.dwReserved2 RESD 1
.hmmio RESD 1
ENDSTRUC



STRUC MMCKINFO
.ckid resd 1
 .cksize resd 1
  .fccType resd 1
   .dwDataOffset resd 1
    .dwFlags resd 1
ENDSTRUC


STRUC MCI_GENERIC_PARMS
.dwCallback RESD 1
ENDSTRUC

STRUC MCI_OPEN_PARMS
.dwCallback RESD 1
.wDeviceID RESD 1
.lpstrDeviceType RESD 1
.lpstrElementName RESD 1
.lpstrAlias RESD 1
ENDSTRUC

STRUC MCI_PLAY_PARMS
.dwCallback RESD 1
.dwFrom RESD 1
.dwTo RESD 1
ENDSTRUC

STRUC MCI_SEEK_PARMS
.dwCallback RESD 1
.dwTo RESD 1
ENDSTRUC

STRUC MCI_STATUS_PARMS
.dwCallback RESD 1
.dwReturn RESD 1
.dwItem RESD 1
.dwTrack RESW 1
ENDSTRUC

STRUC MCI_INFO_PARMS
.dwCallback RESD 1
.lpstrReturn RESD 1
.dwRetSize RESD 1
ENDSTRUC

STRUC MCI_GETDEVCAPS_PARMS
.dwCallback RESD 1
.dwReturn RESD 1
.dwIten RESD 1
ENDSTRUC

STRUC MCI_SYSINFO_PARMS
.dwCallback RESD 1
.lpstrReturn RESD 1
.dwRetSize RESD 1
.dwNumber RESD 1
.wDeviceType RESD 1
ENDSTRUC

STRUC MCI_SET_PARMS
.dwCallback RESD 1
.dwTimeFormat RESD 1
.dwAudio RESD 1
ENDSTRUC

STRUC MCI_BREAK_PARMS
.dwCallback RESD 1
.nVirtKey RESD 1
.hwndBreak RESD 1
ENDSTRUC

STRUC MCI_SOUND_PARMS
.dwCallback RESD 1
.lpstrSoundName RESD 1
ENDSTRUC

STRUC MCI_SAVE_PARMS
.dwCallback RESD 1
.lpFileName RESD 1
ENDSTRUC

STRUC MCI_LOAD_PARMS
.dwCallback RESD 1
.lpFileName RESD 1
ENDSTRUC

STRUC MCI_RECORD_PARMS
.dwCallback RESD 1
.dwFrom RESD 1
.dwTo RESD 1
ENDSTRUC

STRUC MCI_VD_PLAY_PARMS
.dwCallback RESD 1
.dwFrom RESD 1
.dwTo RESD 1
.dwSpeed RESD 1
ENDSTRUC

STRUC MCI_VD_STEP_PARMS
.dwCallback RESD 1
.dwFrames RESD 1
ENDSTRUC

STRUC MCI_VD_ESCAPE_PARMS
.dwCallback RESD 1
.lpstrCommand RESD 1
ENDSTRUC

STRUC MCI_WAVE_OPEN_PARMS
.dwCallback RESD 1
.wDeviceID RESD 1
.lpstrDeviceType RESD 1
.lpstrElementName RESD 1
.lpstrAlias RESD 1
.dwBufferSeconds RESD 1
ENDSTRUC

STRUC MCI_WAVE_DELETE_PARMS
.dwCallback RESD 1
.dwFrom RESD 1
.dwTo RESD 1
ENDSTRUC

STRUC MCI_WAVE_SET_PARMS
.dwCallback RESD 1
.dwTimeFormat RESD 1
.dwAudio RESD 1
.wInput RESD 1
.wOutput RESD 1
.wFormatTag RESW 1
.wReserved2 RESW 1
.nChannels RESW 1
.wReserved3 RESW 1
.nSamplesPerSec RESD 1
.nAvgBytesPerSec RESD 1
.nBlockAlign RESW 1
.wReserved4 RESW 1
.wBitsPerSample RESW 1
.wReserved5 RESW 1
ENDSTRUC

STRUC MCI_SEQ_SET_PARMS
.dwCallback RESD 1
.dwTimeFormat RESD 1
.dwAudio RESD 1
.dwTempo RESD 1
.dwPort RESD 1
.dwSlave RESD 1
.dwMaster RESD 1
.dwOffset RESD 1
ENDSTRUC

STRUC MCI_ANIM_OPEN_PARMS
.dwCallback RESD 1
.wDeviceID RESD 1
.lpstrDeviceType RESD 1
.lpstrElementName RESD 1
.lpstrAlias RESD 1
.dwStyle RESD 1
.hWndParent RESD 1
ENDSTRUC

STRUC MCI_ANIM_PLAY_PARMS
.dwCallback RESD 1
.dwFrom RESD 1
.dwTo RESD 1
.dwSpeed RESD 1
ENDSTRUC

STRUC MCI_ANIM_STEP_PARMS
.dwCallback RESD 1
.dwFrames RESD 1
ENDSTRUC

STRUC MCI_ANIM_WINDOW_PARMS
.dwCallback RESD 1
.hwnd RESD 1
.nCmdShow RESD 1
.lpstrText RESD 1
ENDSTRUC

STRUC MCI_ANIM_RECT_PARMS
.dwCallback RESD 1
.rc RESB RECT_size
ENDSTRUC

STRUC MCI_ANIM_UPDATE_PARMS
.dwCallback RESD 1
.rc RESB RECT_size
.hDC RESD 1
ENDSTRUC

STRUC MCI_OVLY_OPEN_PARMS
.dwCallback RESD 1
.wDeviceID RESD 1
.lpstrDeviceType RESD 1
.lpstrElementName RESD 1
.lpstrAlias RESD 1
.dwStyle RESD 1
.hWndParent RESD 1
ENDSTRUC

STRUC MCI_OVLY_WINDOW_PARMS
.dwCallback RESD 1
.hwnd RESD 1
.nCmdShow RESD 1
.lpstrText RESD 1
ENDSTRUC

STRUC MCI_OVLY_RECT_PARMS
.dwCallback RESD 1
.rc RESB RECT_size
ENDSTRUC

STRUC MCI_OVLY_SAVE_PARMS
.dwCallback RESD 1
.lpFileName RESD 1
.rc RESB RECT_size
ENDSTRUC

STRUC MCI_OVLY_LOAD_PARMS
.dwCallback RESD 1
.lpFileName RESD 1
.rc RESB RECT_size
ENDSTRUC

STRUC PRINTER_INFO_1
.flags RESD 1
.pDescription RESD 1
.pName RESD 1
.pComment RESD 1
ENDSTRUC

STRUC PRINTER_INFO_2
.pServerName RESD 1
.pPrinterName RESD 1
.pShareName RESD 1
.pPortName RESD 1
.pDriverName RESD 1
.pComment RESD 1
.pLocation RESD 1
.pDevMode RESD 1
.pSepFile RESD 1
.pPrintProcessor RESD 1
.pDatatype RESD 1
.pParameters RESD 1
.pSecurityDescriptor RESD 1
.Attributes RESD 1
.Priority RESD 1
.DefaultPriority RESD 1
.StartTime RESD 1
.UntilTime RESD 1
.Status RESD 1
.cJobs RESD 1
.AveragePPM RESD 1
ENDSTRUC

STRUC PRINTER_INFO_3
.pSecurityDescriptor RESD 1
ENDSTRUC

STRUC JOB_INFO_1
.JobId RESD 1
.pPrinterName RESD 1
.pMachineName RESD 1
.pUserName RESD 1
.pDocument RESD 1
.pDatatype RESD 1
.pStatus RESD 1
.Status RESD 1
.Priority RESD 1
.Position RESD 1
.TotalPages RESD 1
.PagesPrinted RESD 1
.Submitted RESD 1
ENDSTRUC

STRUC JOB_INFO_2
.JobId RESD 1
.pPrinterName RESD 1
.pMachineName RESD 1
.pUserName RESD 1
.pDocument RESD 1
.pNotifyName RESD 1
.pDatatype RESD 1
.pPrintProcessor RESD 1
.pParameters RESD 1
.pDriverName RESD 1
.pDevMode RESD 1
.pStatus RESD 1
.pSecurityDescriptor RESD 1
.Status RESD 1
.Priority RESD 1
.Position RESD 1
.StartTime RESD 1
.UntilTime RESD 1
.TotalPages RESD 1
.isize RESD 1
.Submitted RESD 1
.time RESD 1
.PagesPrinted RESD 1
ENDSTRUC

STRUC ADDJOB_INFO_1
.Path RESD 1
.JobId RESD 1
ENDSTRUC

STRUC DRIVER_INFO_1
.pName RESD 1
ENDSTRUC

STRUC DRIVER_INFO_2
.cVersion RESD 1
.pName RESD 1
.pEnvironment RESD 1
.pDriverPath RESD 1
.pDataFile RESD 1
.pConfigFile RESD 1
ENDSTRUC

STRUC DOC_INFO_1
.pDocName RESD 1
.pOutputFile RESD 1
.pDatatype RESD 1
ENDSTRUC

STRUC FORM_INFO_1
.pName RESD 1
.isize RESD 1
.ImageableArea RESD 1
ENDSTRUC

STRUC PRINTPROCESSOR_INFO_1
.pName RESD 1
ENDSTRUC

STRUC PORT_INFO_1
.pName RESD 1
ENDSTRUC

STRUC MONITOR_INFO_1
.pName RESD 1
ENDSTRUC

STRUC MONITOR_INFO_2
.pName RESD 1
.pEnvironment RESD 1
.pDLLName RESD 1
ENDSTRUC

STRUC DATATYPES_INFO_1
.pName RESD 1
ENDSTRUC

STRUC PRINTER_DEFAULTS
.pDatatype RESD 1
.pDevMode RESD 1
.DesiredAccess RESD 1
ENDSTRUC

STRUC PRINTER_INFO_4
.pPrinterName RESD 1
.pServerName RESD 1
.Attributes RESD 1
ENDSTRUC

STRUC PRINTER_INFO_5
.pPrinterName RESD 1
.pPortName RESD 1
.Attributes RESD 1
.DeviceNotSelectedTimeout RESD 1
.TransmissionRetryTimeout RESD 1
ENDSTRUC

STRUC DRIVER_INFO_3
.cVersion RESD 1
.pName RESD 1
.pEnvironment RESD 1
.pDriverPath RESD 1
.pDataFile RESD 1
.pConfigFile RESD 1
.pHelpFile RESD 1
.pDependentFiles RESD 1
.pMonitorName RESD 1
.pDefaultDataType RESD 1
ENDSTRUC

STRUC DOC_INFO_2
.pDocName RESD 1
.pOutputFile RESD 1
.pDatatype RESD 1
.dwMode RESD 1
.JobId RESD 1
ENDSTRUC

STRUC PORT_INFO_2
.pPortName RESD 1
.pMonitorName RESD 1
.pDescription RESD 1
.fPortType RESD 1
.Reserved RESD 1
ENDSTRUC

STRUC PROVIDOR_INFO_1
.pName RESD 1
.pEnvironment RESD 1
.pDLLName RESD 1
ENDSTRUC

STRUC NETRESOURCE
.dwScope RESD 1
.dwType RESD 1
.dwDisplayType RESD 1
.dwUsage RESD 1
.lpLocalName RESD 1
.lpRemoteName RESD 1
.lpComment RESD 1
.lpProvider RESD 1
ENDSTRUC

STRUC NCB
.ncb_command RESW 1
.ncb_retcode RESW 1
.ncb_lsn RESW 1
.ncb_num RESW 1
.ncb_buffer RESD 1
.ncb_length RESW 1
.ncb_callname RESB NCBNAMSZ
.ncb_name RESB NCBNAMSZ
.ncb_rto RESW 1
.ncb_sto RESW 1
.ncb_post RESD 1
.ncb_lana_num RESW 1
.ncb_cmd_cplt RESW 1
.ncb_reserve10 RESB 1
.ncb_event RESD 1
ENDSTRUC

STRUC ADAPTER_STATUS
.adapter_address RESB 6
.rev_major RESW 1
.reserved0 RESW 1
.adapter_type RESW 1
.rev_minor RESW 1
.duration RESW 1
.frmr_recv RESW 1
.frmr_xmit RESW 1
.iframe_recv_err RESW 1
.xmit_aborts RESW 1
.xmit_success RESD 1
.recv_success RESD 1
.iframe_xmit_err RESW 1
.recv_buff_unavail RESW 1
.t1_timeouts RESW 1
.ti_timeouts RESW 1
.Reserved1 RESD 1
.free_ncbs RESW 1
.max_cfg_ncbs RESW 1
.max_ncbs RESW 1
.xmit_buf_unavail RESW 1
.max_dgram_isize RESW 1
.pending_sess RESW 1
.max_cfg_sess RESW 1
.max_sess RESW 1
.max_sess_pkt_isize RESW 1
.name_count RESW 1
ENDSTRUC

STRUC NAME_BUFFER
.xname RESB NCBNAMSZ
.name_num RESW 1
.name_flags RESW 1
ENDSTRUC

STRUC SESSION_HEADER
.sess_name RESW 1
.num_sess RESW 1
.rcv_dg_outstanding RESW 1
.rcv_any_outstanding RESW 1
ENDSTRUC

STRUC SESSION_BUFFER
.lsn RESW 1
.State RESW 1
.local_name RESB NCBNAMSZ
.remote_name RESB NCBNAMSZ
.rcvs_outstanding RESW 1
.sends_outstanding RESW 1
ENDSTRUC

STRUC LANA_ENUM
.iLength RESW 1
.lana RESW 1
ENDSTRUC

STRUC FIND_NAME_HEADER
.node_count RESW 1
.Reserved RESW 1
.unique_group RESW 1
ENDSTRUC

STRUC FIND_NAME_BUFFER
.iLength RESW 1
.access_control RESW 1
.frame_control RESW 1
.destination_addr RESW 1
.source_addr RESW 1
.routing_info RESW 1
ENDSTRUC

STRUC ACTION_HEADER
.transport_id RESD 1
.action_code RESW 1
.Reserved RESW 1
ENDSTRUC

STRUC CRGB
.bRed RESB 1
.bGreen RESB 1
.bBlue RESB 1
.bExtra RESB 1
ENDSTRUC

STRUC SERVICE_STATUS
.dwServiceType RESD 1
.dwCurrentState RESD 1
.dwControlsAccepted RESD 1
.dwWin32ExitCode RESD 1
.dwServiceSpecificExitCode RESD 1
.dwCheckPoint RESD 1
.dwWaitHint RESD 1
ENDSTRUC

STRUC ENUM_SERVICE_STATUS
.lpServiceName RESD 1
.lpDisplayName RESD 1
.ServiceStatus RESD 1
ENDSTRUC

STRUC QUERY_SERVICE_LOCK_STATUS
.fIsLocked RESD 1
.lpLockOwner RESD 1
.dwLockDuration RESD 1
ENDSTRUC

STRUC QUERY_SERVICE_CONFIG
.dwServiceType RESD 1
.dwStartType RESD 1
.dwErrorControl RESD 1
.lpBinaryPathName RESD 1
.lpLoadOrderGroup RESD 1
.dwTagId RESD 1
.lpDependencies RESD 1
.lpServiceStartName RESD 1
.lpDisplayName RESD 1
ENDSTRUC

STRUC SERVICE_TABLE_ENTRY
.lpServiceName RESD 1
.lpServiceProc RESD 1
ENDSTRUC

STRUC LARGE_INTEGER
.lowpart RESD 1
.highpart RESD 1
ENDSTRUC

STRUC PERF_DATA_BLOCK
.Signature RESB 4
.LittleEndian RESD 1
.Version RESD 1
.Revision RESD 1
.TotalByteiLength RESD 1
.HeaderiLength RESD 1
.NumObjectTypes RESD 1
.DefaultObject RESD 1
.SystemTime RESD 1
.PerfTime RESD 1
.PerfFreq RESD 1
.PerTime100nSec RESD 1
.SystemNameiLength RESD 1
.SystemNameOffset RESD 1
ENDSTRUC

STRUC PERF_OBJECT_TYPE
.TotalByteiLength RESD 1
.DefinitioniLength RESD 1
.HeaderiLength RESD 1
.ObjectNameTitleIndex RESD 1
.ObjectNameTitle RESD 1
.ObjectHelpTitleIndex RESD 1
.ObjectHelpTitle RESD 1
.DetailLevel RESD 1
.NumCounters RESD 1
.DefaultCounter RESD 1
.NumInstances RESD 1
.CodePage RESD 1
.PerfTime RESD 1
.PerfFreq RESD 1
ENDSTRUC

STRUC PERF_COUNTER_DEFINITION
.ByteiLength RESD 1
.CounterNameTitleIndex RESD 1
.CounterNameTitle RESD 1
.CounterHelpTitleIndex RESD 1
.CounterHelpTitle RESD 1
.DefaultScale RESD 1
.DetailLevel RESD 1
.CounterType RESD 1
.CounterSize RESD 1
.CounterOffset RESD 1
ENDSTRUC

STRUC PERF_INSTANCE_DEFINITION
.ByteiLength RESD 1
.ParentObjectTitleIndex RESD 1
.ParentObjectInstance RESD 1
.UniqueID RESD 1
.NameOffset RESD 1
.NameiLength RESD 1
ENDSTRUC

STRUC PERF_COUNTER_BLOCK
.ByteiLength RESD 1
ENDSTRUC

STRUC COMPOSITIONFORM
.dwStyle RESD 1
.ptCurrentPos RESB POINT_size
.rcArea RESB RECT_size
ENDSTRUC

STRUC CANDIDATEFORM
.dwIndex RESD 1
.dwStyle RESD 1
.ptCurrentPos RESB POINT_size
.rcArea RESB RECT_size
ENDSTRUC

STRUC CANDIDATELIST
.dwSize RESD 1
.dwStyle RESD 1
.dwCount RESD 1
.dwSelection RESD 1
.dwPageStart RESD 1
.dwPageSize RESD 1
.dwOffset1 RESD 1
ENDSTRUC

STRUC STYLEBUF
.dwStyle RESD 1
.szDescription RESB STYLE_DESCRIPTION_SIZE
ENDSTRUC

STRUC MODEMDEVCAPS
.dwActualSize RESD 1
.dwRequiredSize RESD 1
.dwDevSpecificOffset RESD 1
.dwDevSpecificSize RESD 1
.dwModemProviderVersion RESD 1
.dwModemManufacturerOffset RESD 1
.dwModemManufacturerSize RESD 1
.dwModemModelOffset RESD 1
.dwModemModelSize RESD 1
.dwModemVersionOffset RESD 1
.dwModemVersionSize RESD 1
.dwDialOptions RESD 1
.dwCallSetupFailTimer RESD 1
.dwInactivityTimeout RESD 1
.dwSpeakerVolume RESD 1
.dwSpeakerMode RESD 1
.dwModemOptions RESD 1
.dwMaxDTERate RESD 1
.dwMaxDCERate RESD 1
.abVariablePortion RESB 1
ENDSTRUC

STRUC MODEMSETTINGS
.dwActualSize RESD 1
.dwRequiredSize RESD 1
.dwDevSpecificOffset RESD 1
.dwDevSpecificSize RESD 1
.dwCallSetupFailTimer RESD 1
.dwInactivityTimeout RESD 1
.dwSpeakerVolume RESD 1
.dwSpeakerMode RESD 1
.dwPreferredModemOptions RESD 1
.dwNegotiatedModemOptions RESD 1
.dwNegotiatedDCERate RESD 1
.abVariablePortion RESB 1
ENDSTRUC

STRUC DRAGINFO
.uSize RESD 1
.pt RESB POINT_size
.fNC RESD 1
.lpFileList RESD 1
.grfKeyState RESD 1
ENDSTRUC

STRUC APPBARDATA
.cbSize RESD 1
.hwnd RESD 1
.uCallbackMessage RESD 1
.uEdge RESD 1
.rc RESB RECT_size
.lParam RESD 1
ENDSTRUC

STRUC SHFILEOPSTRUCT
.hwnd RESD 1
.wFunc RESD 1
.pFrom RESD 1
.pTo RESD 1
.fFlags RESW 1
.fAnyOperationsAborted RESD 1
.hNameMappings RESD 1
.lpszProgressTitle RESD 1
ENDSTRUC

STRUC SHNAMEMAPPING
.pszOldPath RESD 1
.pszNewPath RESD 1
.cchOldPath RESD 1
.cchNewPath RESD 1
ENDSTRUC

STRUC SHELLEXECUTEINFO
.cbSize RESD 1
.fMask RESD 1
.hwnd RESD 1
.lpVerb RESD 1
.lpFile RESD 1
.lpParameters RESD 1
.lpDirectory RESD 1
.nShow RESD 1
.hInstApp RESD 1
.lpIDList RESD 1
.lpClass RESD 1
.hkeyClass RESD 1
.dwHotKey RESD 1
.hIcon RESD 1
.hProcess RESD 1
ENDSTRUC

STRUC NOTIFYICONDATA
.cbSize RESD 1
.hwnd RESD 1
.uID RESD 1
.uFlags RESD 1
.uCallbackMessage RESD 1
.hIcon RESD 1
.szTip RESB 64
ENDSTRUC

STRUC SHFILEINFO
.hIcon RESD 1
.iIcon RESD 1
.dwAttributes RESD 1
.szDisplayName RESB 1
.szTypeName RESB 80
ENDSTRUC

STRUC VS_FIXEDFILEINFO
.dwSignature RESD 1
.dwStrucVersion RESD 1
.dwFileVersionMS RESD 1
.dwFileVersionLS RESD 1
.dwProductVersionMS RESD 1
.dwProductVersionLS RESD 1
.dwFileFlagsMask RESD 1
.dwFileFlags RESD 1
.dwFileOS RESD 1
.dwFileType RESD 1
.dwFileSubtype RESD 1
.dwFileDateMS RESD 1
.dwFileDateLS RESD 1
ENDSTRUC

STRUC ICONMETRICS
.cbSize RESD 1
.iHorzSpacing RESD 1
.iVertSpacing RESD 1
.iTitleWrap RESD 1
.lfFont RESD 1
ENDSTRUC

STRUC HELPINFO
.cbSize RESD 1
.iContextType RESD 1
.iCtrlId RESD 1
.hItemHandle RESD 1
.dwContextId RESD 1
.MousePos RESD 1
ENDSTRUC

STRUC ANIMATIONINFO
.cbSize RESD 1
.iMinAnimate RESD 1
ENDSTRUC

STRUC MINIMIZEDMETRICS
.cbSize RESD 1
.iWidth RESD 1
.iHorzGap RESD 1
.iVertGap RESD 1
.iArrange RESD 1
.lfFont RESD 1
ENDSTRUC

STRUC OSVERSIONINFO
.dwOSVersionInfoSize RESD 1
.dwMajorVersion RESD 1
.dwMinorVersion RESD 1
.dwBuildNumber RESD 1
.dwPlatformId RESD 1
.szCSDVersion RESB 128
ENDSTRUC

STRUC SYSTEM_POWER_STATUS
.ACLineStatus RESB 1
.BatteryFlag RESB 1
.BatteryLifePercent RESB 1
.Reserved1 RESB 1
.BatteryLifeTime RESD 1
.BatteryFullLifeTime RESD 1
ENDSTRUC

;=============iiiiiiiiiiiiiiiiiiiiiiiii


STRUC NMHDR
.hwndFrom RESD 1
.idfrom RESD 1
.code RESD 1
ENDSTRUC

STRUC DEVNAMES
.wDriverOffset RESW 1
.wDeviceOffset RESW 1
.wOutputOffset RESW 1
.wDefault RESW 1
ENDSTRUC

STRUC PAGESETUPDLG
.lStructSize RESD 1
.hwndOwner RESD 1
.hDevMode RESD 1
.hDevNames RESD 1
.flags RESD 1
.ptPaperSize RESB POINT_size
.rtMinMargin RESD 1
.rtMargin RESD 1
.hInstance RESD 1
.lCustData RESD 1
.lpfnPageSetupHook RESD 1
.lpfnPagePaintHook RESD 1
.lpPageSetupTemplateName RESD 1
.hPageSetupTemplate RESD 1
ENDSTRUC

STRUC COMMCONFIG
.dwSize RESD 1
.wVersion RESW 1
.wReserved RESW 1
.dcbx RESD 1
.dwProviderSubType RESD 1
.dwProviderOffset RESD 1
.dwProviderSize RESD 1
.wcProviderData RESB 1
ENDSTRUC

STRUC PIXELFORMATDESCRIPTOR
.nSize RESW 1
.nVersion RESW 1
.dwFlags RESD 1
.iPixelType RESB 1
.cColorBits RESB 1
.cRedBits RESB 1
.cRedShift RESB 1
.cGreenBits RESB 1
.cGreenShift RESB 1
.cBlueBits RESB 1
.cBlueShift RESB 1
.cAlphaBits RESB 1
.cAlphaShift RESB 1
.cAccumBits RESB 1
.cAccumRedBits RESB 1
.cAccumGreenBits RESB 1
.cAccumBlueBits RESB 1
.cAccumAlphaBits RESB 1
.cDepthBits RESB 1
.cStencilBits RESB 1
.cAuxBuffers RESB 1
.iLayerType RESB 1
.bReserved RESB 1
.dwLayerMask RESD 1
.dwVisibleMask RESD 1
.dwDamageMask RESD 1
ENDSTRUC

STRUC DRAWTEXTPARAMS
.cbSize RESD 1
.iTabiLength RESD 1
.iLeftMargin RESD 1
.iRightMargin RESD 1
.uiiLengthDrawn RESD 1
ENDSTRUC

STRUC MENUITEMINFO
.cbSize RESD 1
.fMask RESD 1
.fType RESD 1
.fState RESD 1
.wID RESD 1
.hSubMenu RESD 1
.hbmpChecked RESD 1
.hbmpUnchecked RESD 1
.dwItemData RESD 1
.dwTypeData RESD 1
.cch RESD 1
ENDSTRUC

STRUC SCROLLINFO
.cbSize RESD 1
.fMask RESD 1
.nMin RESD 1
.nMax RESD 1
.nPage RESD 1
.nPos RESD 1
.nTrackPos RESD 1
ENDSTRUC

STRUC MSGBOXPARAMS
.cbSize RESD 1
.hwndOwner RESD 1
.hInstance RESD 1
.lpszText RESD 1
.lpszCaption RESD 1
.dwStyle RESD 1
.lpszIcon RESD 1
.dwContextHelpId RESD 1
.lpfnMsgBoxCallback RESD 1
.dwLanguageId RESD 1
ENDSTRUC



STRUC DEBUG_EVENT
.dwDebugEventCode RESD 1
.dwProcessId RESD 1
.dwThreadId RESD 1
.u RESB EXCEPTION_DEBUG_INFO_size 
ENDSTRUC






STRUC COLORMAP
.cmFrom RESD 1
.cmTo RESD 1
ENDSTRUC

STRUC AuxVol
.vLow RESW 1
.vHigh RESW 1
ENDSTRUC

STRUC DBGTHREAD
.hThread RESD 1
.lpStartAddress RESD 1
.bfState RESD 1
.nNext RESQ 1
ENDSTRUC

STRUC DbgProcess
.hDbgHeap RESD 1
.dwProcessID RESD 1
.dwThreadID RESD 1
.hProcess RESD 1
.hFile RESD 1
.lpImage RESD 1
ENDSTRUC


;===========================
;STRUC IMAGE_DATA_DIRECTORY
;.VirtualAddress RESD 1
;.isize RESD 1
;ENDSTRUC
;===========================

;========================================
;STRUC IMAGE_OPTIONAL_HEADER
;.Magic RESW 1
;.MajorLinkerVersion RESB 1
;.MinorLinkerVersion RESB 1
;.SizeOfCode RESD 1
;.SizeOfInitializedData RESD 1
;.SizeOfUninitializedData RESD 1
;.AddressOfEntryPoint RESD 1
;.BaseOfCode RESD 1
;.BaseOfData RESD 1
;.ImageBase RESD 1
;.SectionAlignment RESD 1
;.FileAlignment RESD 1
;.MajorOperatingSystemVersion RESW 1
;.MinorOperatingSystemVersion RESW 1
;.MajorImageVersion RESW 1
;.MinorImageVersion RESW 1
;.MajorSubsystemVersion RESW 1
;.MinorSubsystemVersion RESW 1
;.Reserved1 RESD 1
;.SizeOfImage RESD 1
;.SizeOfHeaders RESD 1
;.CheckSum RESD 1
;.Subsystem RESW 1
;.DllCharacteristics RESW 1
;.SizeOfStackReserve RESD 1
;.SizeOfStackCommit RESD 1
;.SizeOfHeapReserve RESD 1
;.SizeOfHeapCommit RESD 1
;.LoaderFlags RESD 1
;.NumberOfRvaAndSizes RESD 1
;.DataDirectory RESB 8*16
;.DataDirectory RESB IMAGE_DATA_DIRECTORY_size * IMAGE_NUMBEROF_DIRECTORY_ENTRIES
;ENDSTRUC
;=====================================






;===========================
;STRUC IMAGE_FILE_HEADER
;.Machine RESW 1
;.NumberOfSections RESW 1
;.TimeDateStamp RESD 1
;.PointerToSymbolTable RESD 1
;.NumberOfSymbols RESD 1
;.SizeOfOptionalHeader RESW 1
;.Characteristics RESW 1
;ENDSTRUC
;============================


;==============================
;STRUC IMAGE_NT_HEADERS
;.Signature RESD 1
;.FileHeader RESD 1
;.OptionalHeader RESD 1
;ENDSTRUC
;==================================




STRUC IMAGE_EXPORT_DIRECTORY
  .Characteristics           resd 1
  .TimeDateStamp             resd 1
  .MajorVersion              resw 1
  .MinorVersion              resw 1
  .nName                     resd 1
  .nBase                     resd 1
  .NumberOfFunctions         resd 1
  .NumberOfNames             resd 1
  .AddressOfFunctions        resd 1
  .AddressOfNames            resd 1
  .AddressOfNameOrdinals     resd 1
ENDSTRUC




STRUC IMAGE_DOS_HEADER
.e_magic RESW 1
.e_cblp RESW 1
.e_cp RESW 1
.e_crlc RESW 1
.e_cparhdr RESW 1
.e_minalloc RESW 1
.e_maxalloc RESW 1
.e_ss RESW 1
.e_sp RESW 1
.e_csum RESW 1
.e_ip RESW 1
.e_cs RESW 1
.e_lfarlc RESW 1
.e_ovno RESW 1
.e_res4 RESW 1
.e_oemid RESW 1
.e_oeminfo RESW 1
.e_res2 RESW 1
.e_lfanew RESD 1
ENDSTRUC

STRUC USER_INFO_3
.uName RESD 1
.Password RESD 1
.PasswordAge RESD 1
.Privilege RESD 1
.HomeDir RESD 1
.Comment RESD 1
.Flags RESD 1
.ScriptPath RESD 1
.AuthFlags RESD 1
.FullName RESD 1
.UserComment RESD 1
.Parms RESD 1
.Workstations RESD 1
.LastLogon RESD 1
.LastLogoff RESD 1
.AcctExpires RESD 1
.MaxStorage RESD 1
.UnitsPerWeek RESD 1
.LogonHours RESD 1
.BadPwCount RESD 1
.NumLogons RESD 1
.LogonServer RESD 1
.CountryCode RESD 1
.CodePage RESD 1
.UserID RESD 1
.PrimaryGroupID RESD 1
.Profile RESD 1
.HomeDirDrive RESD 1
.PasswordExpired RESD 1
ENDSTRUC

STRUC GROUP_INFO_2
.uName RESD 1
.Comment RESD 1
.GroupID RESD 1
.Attributes RESD 1
ENDSTRUC




STRUC BROWSEINFO 
 .hwndOwner      resd 1
 .pidlRoot       resd 1
 .pszDisplayName resd 1
 .lpszTitle      resd 1
 .ulFlags        resd 1
 .lpfn           resd 1
 .lParam         resd 1
 .iImage         resd 1
ENDSTRUC



; ------- PE Macros -------------------------------------

STRUC IMAGE_DATA_DIRECTORY
.VirtualAddress RESD 1
.isize RESD 1
ENDSTRUC


STRUC IMAGE_OPTIONAL_HEADER
.Magic RESW 1
.MajorLinkerVersion RESB 1
.MinorLinkerVersion RESB 1
.SizeOfCode RESD 1
.SizeOfInitializedData RESD 1
.SizeOfUninitializedData RESD 1
.AddressOfEntryPoint RESD 1
.BaseOfCode RESD 1
.BaseOfData RESD 1
.ImageBase RESD 1
.SectionAlignment RESD 1
.FileAlignment RESD 1
.MajorOperatingSystemVersion RESW 1
.MinorOperatingSystemVersion RESW 1
.MajorImageVersion RESW 1
.MinorImageVersion RESW 1
.MajorSubsystemVersion RESW 1
.MinorSubsystemVersion RESW 1
.Reserved1 RESD 1
.SizeOfImage RESD 1
.SizeOfHeaders RESD 1
.CheckSum RESD 1
.Subsystem RESW 1
.DllCharacteristics RESW 1
.SizeOfStackReserve RESD 1
.SizeOfStackCommit RESD 1
.SizeOfHeapReserve RESD 1
.SizeOfHeapCommit RESD 1
.LoaderFlags RESD 1
.NumberOfRvaAndSizes RESD 1
.DataDirectory RESB IMAGE_DATA_DIRECTORY_size*16
ENDSTRUC

STRUC IMAGE_FILE_HEADER
.Machine RESW 1
.NumberOfSections RESW 1
.TimeDateStamp RESD 1
.PointerToSymbolTable RESD 1
.NumberOfSymbols RESD 1
.SizeOfOptionalHeader RESW 1
.Characteristics RESW 1
ENDSTRUC

STRUC IMAGE_NT_HEADERS
.Signature RESD 1
.FileHeader RESD 1
.OptionalHeader RESD 1
ENDSTRUC



 

;====================== end PE


;============== ultimos ====


STRUC WAVEFORMATEX
    .wFormatTag resw 1
    .nChannels resw 1
    .nSamplesPerSec resd 1
    .nAvgBytesPerSec resd 1
    .nBlockAlign resw 1
    .wBitsPerSample resw 1
    .cbSize resw 1
ENDSTRUC





STRUC CAPTUREPARMS
    .dwRequestMicroSecPerFrame resd 1
    .fMakeUserHitOKToCapture resd 1
    .wPercentDropForError resd 1
    .fYield resd 1
    .dwIndexSize resd 1
    .wChunkGranularity resd 1
    .fUsingDOSMemory resd 1
    .wNumVideoRequested resd 1
    .fCaptureAudio resd 1
    .wNumAudioRequested resd 1
    .vKeyAbort resd 1
    .fAbortLeftMouse resd 1
    .fAbortRightMouse resd 1
    .fLimitEnabled resd 1
    .wTimeLimit resd 1
    .fMCIControl resd 1
    .fStepMCIDevice resd 1
    .dwMCIStartTime resd 1
    .dwMCIStopTime resd 1
    .fStepCaptureAt2x resd 1
    .wStepCaptureAverageFrames resd 1
    .dwAudioBufferSize resd 1
    .fDisableWriteCache resd 1
    .AVStreamMaster resd 1
ENDSTRUC


STRUC ACMFORMATCHOOSE
.cbStruct resd 1
.fdwStyle resd 1
.hwndOwner resd 1
.pwfx resd 1
.cbwfx resd 1
.pszTitle resd 1
.szFormatTag resd 48
.szFormat resd 128
.pszName resd 1
.cchName resd 1
.fdwEnum resd 1
.pwfxEnum resd 1  ; can be a WAVEFORMATEX
.hInstance resd 1
.pszTemplateName resd 1
.lCustData resd 1
.pfnHook resd 1
ENDSTRUC

STRUC ACMFILTERCHOOSE
.cbStruct resd 1
.fdwStyle resd 1
.hwndOwner resd 1
.pwfltr resd 1
.cbwfltr resd 1
.pszTitle resd 1
.szFilterTag resd 48
.szFilter resd 128
.pszName resd 1
.cchName resd 1
.fdwEnum resd 1
.pwfltrEnum resd 1
.hInstance resd 1
.pszTemplateName resd 1
.lCustData resd 1
.pfnHook resd 1
ENDSTRUC

STRUC MPEG1WAVEFORMAT
    .wfx resb WAVEFORMATEX_size
    .fwHeadLayer resb 1
    .dwHeadBitrate resd 1
    .fwHeadMode resb 1
    .fwHeadModeExt resb 1
    .wHeadEmphasis resb 1
    .fwHeadFlags resb 1
    .dwPTSLow resd 1
    .dwPTSHigh resd 1
ENDSTRUC


STRUC MPEGLAYER3WAVEFORMAT
.wfx resb  WAVEFORMATEX_size
.wID  resb 1
.fdwFlags resd 1
.nBlockSize resb 1
.nFramesPerBlock resb 1
.nCodecDelay resb 1
ENDSTRUC

;===================================================
; WAVE_FORMAT_PCM   WAVEFORMATEX  = sizeof  = 0x12
; MPEGLAYER3WAVEFORMAT mp3 ;    sizeof(mp3) = 0x1E
; WAVE_FORMAT_MPEGLAYER3  cbFmt = sizeof(MPEGLAYER3WAVEFORMAT) = 0x1E
; WAVE_FORMAT_ADPCM  cbFmt = 0x32

;-----------------------------
;  mp3format->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
;  mp3format->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
;  mp3format->wfx.nChannels = 2;
;  mp3format->wfx.nAvgBytesPerSec = 128 * (1024 / 8);  // not really used but must be one of 64, 96, ;  112, 128, 160kbps
;  mp3format->wfx.wBitsPerSample = 0;                  // MUST BE ZERO
;  mp3format->wfx.nBlockAlign = 1;                     // MUST BE ONE
;  mp3format->wfx.nSamplesPerSec = 44100;              // 44.1kHz
;  mp3format->fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
;  mp3format->nBlockSize = MP3_BLOCK_SIZE;             // voodoo value #1
;  mp3format->nFramesPerBlock = 1;                     // MUST BE ONE
;  mp3format->nCodecDelay = 1393;                      // voodoo value #2
;  mp3format->wID = MPEGLAYER3_ID_MPEG;
;-------------------------------------------------------------------





STRUC GUID
.Data1 resd 1
.Data2 resw 1
.Data3 resw 1
.Data4 resb 8
ENDSTRUC




STRUC  IAMTVAudio
    ;==This are pointers to IUnknown interface
   .QueryInterface    resd 1
   .AddRef                resd 1
   .Release                resd 1
    ; == IAMTVAudio functions
.GetHardwareSupportedTVAudioModes resd 1
.GetAvailableTVAudioModes resd 1
.get_TVAudioMode resd 1
.put_TVAudioMode    resd 1
.RegisterNotification resd 1
.lEvents resd 1
.UnRegisterNotificationCallBack  resd 1
ENDSTRUC






STRUC  IAMTuner
    ;==This are pointers to IUnknown interface
   .QueryInterface    resd 1
   .AddRef                resd 1
   .Release                resd 1
    ;== This are pointers to Methods (functions) IAMTunerNotification
   .OnEvent              resd 1
   ;==This are pointers to Methods (functions) of IAMTuner
    .put_Channel resd 1
    .get_Channel resd 1
    .ChannelMinMax resd 1
    .put_CountryCode resd 1
    .get_CountryCode resd 1
    .put_TuningSpace resd 1
    .get_TuningSpace resd 1
    .Logon resd 1
    .Logout resd 1
    .SignalPresent resd 1
    .put_Mode resd 1
    .get_Mode resd 1
    .GetAvailableModes resd 1
    .RegisterNotificationCallBack resd 1
    .UnRegisterNotificationCallBack resd 1
ENDSTRUC













STRUC ICaptureGraphBuilder2
    ;==This are pointers to IUnknown interface
   .QueryInterface    resd 1
   .AddRef                resd 1
   .Release                resd 1
   ;==This are pointers to Methods (functions) ICaptureGraphBuilder2
.AllocCapFile resd 1
.ControlStream resd 1
.CopyCaptureFile resd 1
.FindInterface resd 1
.FindPin resd 1
.GetFiltergraph resd 1
.RenderStream resd 1
.SetFiltergraph resd 1
.SetOutputFileName resd 1
ENDSTRUC


;=== IAMTVTuner interface

STRUC IAMTVTuner
    ;==This are pointers to IUnknown interface
   .QueryInterface    resd 1
   .AddRef                resd 1
   .Release                resd 1
   ;== This are pointers to Methods (functions) IAMTunerNotification
   .OnEvent              resd 1
   ;==This are pointers to Methods (functions) of IAMTuner
    .put_Channel resd 1
    .get_Channel resd 1
    .ChannelMinMax resd 1
    .put_CountryCode resd 1
    .get_CountryCode resd 1
    .put_TuningSpace resd 1
    .get_TuningSpace resd 1
    .Logon resd 1
    .Logout resd 1
    .SignalPresent resd 1
    .put_Mode resd 1
    .get_Mode resd 1
    .GetAvailableModes resd 1
    .RegisterNotificationCallBack resd 1
    .UnRegisterNotificationCallBack resd 1
   ;==This are pointers to Methods (functions) IAMTVtuner
 .get_AvailableTVFormats resd 1
 .get_TVFormat resd 1
 .AutoTune resd 1
 .StoreAutoTune resd 1
 .get_NumInputConnections resd 1
 .put_InputType resd 1
 .get_InputType resd 1
 .put_ConnectInput resd 1
 .get_ConnectInput resd 1
 .get_VideoFrequency resd 1
 .get_AudioFrequency resd 1
ENDSTRUC




;==== IBaseFilter interface

STRUC IBaseFilter
;==IUnknown methods
.QueryInterface  resd 1
.AddRef  resd 1
.Release  resd 1
;==IMediaFilter methods
.Stop  resd 1
.Pause resd 1
.Run resd 1
.GetState resd 1
.SetSyncSource resd 1
.GetSyncSource resd 1
;==IBaseFilter methods
.EnumPins resd 1
.FindPin resd 1
.QueryFilterInfo resd 1
.JoinFilterGraph resd 1
.QueryVendorInfo resd 1
ENDSTRUC





;==== IFilterGraph interface
STRUC  IFilterGraph
;==IUnknown methods
.QueryInterface  resd 1
.AddRef  resd 1
.Release  resd 1
;== IFilterGraph methods
.AddFilter resd 1
.RemoveFilter resd 1
.EnumFilters resd 1
.FindFilterByName resd 1
.ConnectDirect resd 1
.Reconnect resd 1
.Disconnect resd 1
.SetDefaultSyncSource resd 1
ENDSTRUC


;-------------------------comctl structures------------------------------
STRUC INIT_COMMON_CONTROLSEX
.dwSize RESD 1
.dwICC RESD 1
ENDSTRUC

STRUC REBARINFO
.cbSize RESD 1
.fMask RESD 1
.himl RESD 1
ENDSTRUC

STRUC REBARBANDINFO
.cbSize RESD 1
.fMask RESD 1
.fStyle RESD 1
.clrFore RESD 1
.clrBack RESD 1
.lpText RESD 1
.cch RESD 1
.iImage RESD 1
.hwndChild RESD 1
.cxMinChild RESD 1
.cyMinChild RESD 1
.lx RESD 1
.hbmBack RESD 1
.wID RESD 1
.cyChild RESD 1
.cyMaxChild RESD 1
.cyIntegral RESD 1
.cxIdeal RESD 1
.lParam RESD 1
.cxHeader RESD 1
ENDSTRUC

STRUC NMREBARCHILDSIZE
.hdr RESB NMHDR_size
.uBand RESD 1
.wID RESD 1
.rcChild RESB RECT_size
.rcBand RESB RECT_size
ENDSTRUC

STRUC NMREBAR
.hdr RESB NMHDR_size
.dwMask RESD 1
.uBand RESD 1
.fStyle RESD 1
.wID RESD 1
.lParam RESD 1
ENDSTRUC

STRUC NMRBAUTOSIZE
.hdr RESB NMHDR_size
.fChanged RESD 1
.rcTarget RESB RECT_size
.rcActual RESB RECT_size
ENDSTRUC

STRUC RB_HITTESTINFO
.pt RESB POINT_size
.flags RESD 1
.iBand RESW 1
ENDSTRUC

STRUC IMAGEINFO
.hbmImage RESD 1
.hbmMask RESD 1
.Unused1 RESD 1
.Unused2 RESD 1
.rcImage RESB RECT_size
ENDSTRUC

STRUC HD_ITEM
.imask RESD 1
.lxy RESD 1
.pszText RESD 1
.hbm RESD 1
.cchTextMax RESD 1
.fmt RESD 1
.lParam RESD 1
ENDSTRUC

STRUC HD_LAYOUT
.prc RESD 1
.pwpos RESD 1
ENDSTRUC

STRUC HD_HITTESTINFO
.pt RESB POINT_size
.flags RESD 1
.iItem RESD 1
ENDSTRUC

STRUC HD_NOTIFY
.hdr RESB NMHDR_size
.iItem RESD 1
.iButton RESD 1
.pitem RESD 1
ENDSTRUC

STRUC TBBUTTON
.iBitmap RESD 1
.idCommand RESD 1
.fsState RESB 1
.fsStyle RESB 1
._wPad1  RESW  1       
.dwData RESD 1
.iString RESD 1
ENDSTRUC

STRUC ColorMap
.cmFrom RESD 1
.cmTo RESD 1
ENDSTRUC

STRUC TBADDBITMAP
.hInst RESD 1
.nID RESD 1
ENDSTRUC

STRUC TBSAVEPARAMS
.hkr RESD 1
.pszSubKey RESD 1
.pszValueName RESD 1
ENDSTRUC

STRUC TBREPLACEBITMAP
.hInstOld RESD 1
.nIdOld RESD 1
.hInstNew RESD 1
.nIdNew RESD 1
.nButtons RESD 1
ENDSTRUC

STRUC TBNOTIFY
.hdr RESB NMHDR_size
.iItem RESD 1
.tbButton RESB TBBUTTON_size
.cchText RESD 1
.pszText RESD 1
ENDSTRUC

STRUC TOOLINFO
.cbSize RESD 1
.uFlags RESD 1
.hWnd RESD 1
.uId RESD 1
.rect RESB RECT_size
.hInst RESD 1
.lpszText RESD 1
.lParam RESD 1
ENDSTRUC

STRUC TT_HITTESTINFO
.hWnd RESD 1
.pt RESB POINT_size
.ti RESB TOOLINFO_size
ENDSTRUC

STRUC TOOLTIPTEXT
.hdr RESB NMHDR_size
.lpszText RESD 1
.szText RESB 80
.hInst RESD 1
.uFlags RESD 1
ENDSTRUC

STRUC DRAGLISTINFO
.uNotification RESD 1
.hWnd RESD 1
.ptCursor RESB POINT_size
ENDSTRUC

STRUC UDACCEL
.nSec RESD 1
.nInc RESD 1
ENDSTRUC

STRUC NM_UPDOWN
.hdr RESB NMHDR_size
.iPos RESD 1
.iDelta RESD 1
ENDSTRUC

STRUC LV_ITEM
.imask RESD 1
.iItem RESD 1
.iSubItem RESD 1
.state RESD 1
.stateMask RESD 1
.pszText RESD 1
.cchTextMax RESD 1
.iImage RESD 1
.lParam RESD 1
ENDSTRUC

STRUC LV_FINDINFO
.flags RESD 1
.psz RESD 1
.lParam RESD 1
.pt RESB POINT_size
.vkDirection RESD 1
ENDSTRUC

STRUC LV_HITTESTINFO
.pt RESB POINT_size
.flags RESD 1
.iItem RESD 1
ENDSTRUC

STRUC LV_COLUMN
.imask RESD 1
.fmt RESD 1
.lx RESD 1
.pszText RESD 1
.cchTextMax RESD 1
.iSubItem RESD 1
ENDSTRUC

STRUC NM_LISTVIEW
.hdr RESB NMHDR_size
.iItem RESD 1
.iSubItem RESD 1
.uNewState RESD 1
.uOldState RESD 1
.uChanged RESD 1
.ptAction RESB POINT_size
.lParam RESD 1
ENDSTRUC

STRUC LV_DISPINFO
.hdr RESB NMHDR_size
.item RESD 1
ENDSTRUC

STRUC LV_KEYDOWN
.hdr RESB NMHDR_size
.wVKey RESW 1
.flags RESD 1
ENDSTRUC

STRUC TREEITEM
.dummy RESD 1
ENDSTRUC

STRUC TV_ITEM
.imask RESD 1
.hItem RESD 1
.state RESD 1
.stateMask RESD 1
.pszText RESD 1
.cchTextMax RESD 1
.iImage RESD 1
.iSelectedImage RESD 1
.cChildren RESD 1
.lParam RESD 1
ENDSTRUC



STRUC TV_INSERTSTRUCT
   .hParent RESD 1
   .hInsertAfter RESD 1
   .item  RESB TV_ITEM_size
 ENDSTRUC



STRUC TV_HITTESTINFO
.pt RESB POINT_size
.flags RESD 1
.hItem RESD 1
ENDSTRUC

STRUC TV_SORTCB
.hParent RESD 1
.lpfnCompare RESD 1
.lParam RESD 1
ENDSTRUC

STRUC NM_TREEVIEW
.hdr RESB NMHDR_size
.action RESD 1
.itemOld RESD 1
.itemNew RESD 1
.ptDrag RESB POINT_size
ENDSTRUC

STRUC TV_DISPINFO
.hdr RESB NMHDR_size
.item RESD 1
ENDSTRUC

STRUC TV_KEYDOWN
.hdr RESB NMHDR_size
.wVKey RESW 1
.flags RESD 1
ENDSTRUC

STRUC TC_ITEMHEADER
.imask RESD 1
.lpReserved1 RESD 1
.lpReserved2 RESD 1
.pszText RESD 1
.cchTextMax RESD 1
.iImage RESD 1
ENDSTRUC

STRUC TC_ITEM
.imask RESD 1
.lpReserved1 RESD 1
.lpReserved2 RESD 1
.pszText RESD 1
.cchTextMax RESD 1
.iImage RESD 1
.lParam RESD 1
ENDSTRUC

STRUC TC_HITTESTINFO
.pt RESB POINT_size
.flags RESD 1
ENDSTRUC

STRUC TC_KEYDOWN
.hdr RESB NMHDR_size
.wVKey RESW 1
.flags RESD 1
ENDSTRUC


;--------------------------comdlg structures----------------------------
STRUC CHOOSECOLOR
.lStructSize RESD 1
.hwndOwner RESD 1
.hInstance RESD 1
.rgbResult RESD 1
.lpCustColors RESD 1
.Flags RESD 1
.lCustData RESD 1
.lpfnHook RESD 1
.lpTemplateName RESD 1
ENDSTRUC

STRUC FINDREPLACE
.lStructSize RESD 1
.hwndOwner RESD 1
.hInstance RESD 1
.Flags RESD 1
.lpstrFindWhat RESD 1
.lpstrReplaceWith RESD 1
.wFindWhatLen RESW 1
.wReplaceWithLen RESW 1
.lCustData RESD 1
.lpfnHook RESD 1
.lpTemplateName RESD 1
ENDSTRUC

STRUC CHOOSEFONT
.lStructSize RESD 1
.hwndOwner RESD 1
.hDC RESD 1
.lpLogFont RESD 1
.iPointSize RESD 1
.Flags RESD 1
.rgbColors RESD 1
.lCustData RESD 1
.lpfnHook RESD 1
.lpTemplateName RESD 1
.hInstance RESD 1
.lpszStyle RESD 1
.nFontType RESW 1
.Alignment RESW 1
.nSizeMin RESD 1
.nSizeMax RESD 1
ENDSTRUC

STRUC PRINTDLG
.lStructSize RESD 1
.hwndOwner RESD 1
.hDevMode RESD 1
.hDevNames RESD 1
.hDC RESD 1
.Flags RESD 1
.nFromPage RESW 1
.nToPage RESW 1
.nMinPage RESW 1
.nMaxPage RESW 1
.nCopies RESW 1
.hInstance RESD 1
.lCustData RESD 1
.lpfnPrintHook RESD 1
.lpfnSetupHook RESD 1
.lpPrintTemplateName RESD 1
.lpPrintSetupTemplateName RESD 1
.hPrintTemplate RESD 1
.hSetupTemplate RESD 1
ENDSTRUC

STRUC OPENFILENAME
.lStructSize RESD 1
.hwndOwner RESD 1
.hInstance RESD 1
.lpstrFilter RESD 1
.lpstrCustomFilter RESD 1
.nMaxCustFilter RESD 1
.nFilterIndex RESD 1
.lpstrFile RESD 1
.nMaxFile RESD 1
.lpstrFileTitle RESD 1
.nMaxFileTitle RESD 1
.lpstrInitialDir RESD 1
.lpstrTitle RESD 1
.Flags RESD 1
.nFileOffset RESW 1
.nFileExtension RESW 1
.lpstrDefExt RESD 1
.lCustData RESD 1
.lpfnHook RESD 1
.lpTemplateName RESD 1
ENDSTRUC


;--------------------------riched structures-----------------------------
STRUC CHARFORMAT
.cbSize RESD 1
.dwMask RESD 1
.dwEffects RESD 1
.yHeight RESD 1
.yOffset RESD 1
.crTextColor RESD 1
.bCharSet RESB 1
.bPitchAndFamily RESB 1
.szFaceName RESB 1
 ._wPad2       RESW      1
ENDSTRUC

STRUC CHARRANGE
.cpMin RESD 1
.cpMax RESD 1
ENDSTRUC

STRUC TEXTRANGE
.chrg RESB CHARRANGE_size
.lpstrText RESD 1
ENDSTRUC

STRUC EDITSTREAM
.dwCookie RESD 1
.dwError RESD 1
.pfnCallback RESD 1
ENDSTRUC

STRUC FINDTEXT
.chrg RESB CHARRANGE_size
.lpstrText RESD 1
ENDSTRUC

STRUC FINDTEXTEX
.chrg RESB CHARRANGE_size
.lpstrText RESD 1
.chrgText RESB CHARRANGE_size
ENDSTRUC

STRUC FORMATRANGE
.hdc RESD 1
.hdcTarget RESD 1
.rc RESB RECT_size
.rcPage RESB RECT_size
.chrg RESB CHARRANGE_size
ENDSTRUC

STRUC PARAFORMAT
.cbSize RESD 1
.dwMask RESD 1
.wNumbering RESW 1
.wReserved RESW 1
.dxStartIndent RESD 1
.dxRightIndent RESD 1
.dxOffset RESD 1
.wAlignment RESW 1
.cTabCount RESW 1
.rgxTabs RESD 1
ENDSTRUC

STRUC MSGFILTER
.nmhdr RESB NMHDR_size
.msg RESD 1
.wParam RESD 1
.lParam RESD 1
ENDSTRUC

STRUC REQRESIZE
.nmhdr RESB NMHDR_size
.rc RESB RECT_size
ENDSTRUC

STRUC SELCHANGE
.nmhdr RESB NMHDR_size
.chrg RESB CHARRANGE_size
.seltyp RESW 1
ENDSTRUC

STRUC ENDROPFILES
.nmhdr RESB NMHDR_size
.hDrop RESD 1
.cp RESD 1
.fProtected RESD 1
ENDSTRUC

STRUC ENPROTECTED
.nmhdr RESB NMHDR_size
.msg RESD 1
.wParam RESD 1
.lParam RESD 1
.chrg RESB CHARRANGE_size
ENDSTRUC

STRUC ENSAVECLIPBOARD
.nmhdr RESB NMHDR_size
.cObjectCount RESD 1
.cch RESD 1
ENDSTRUC

STRUC ENOLEOPFAILED
.nmhdr RESB NMHDR_size
.iob RESD 1
.lOper RESD 1
.hr RESD 1
ENDSTRUC

STRUC ENCORRECTTEXT
.nmhdr RESB NMHDR_size
.chrg RESB CHARRANGE_size
.seltyp RESW 1
ENDSTRUC

STRUC PUNCTUATION
.iSize RESD 1
.szPunctuation RESD 1
ENDSTRUC

STRUC COMPCOLOR
.crText RESD 1
.crBackground RESD 1
.dwEffects RESD 1
ENDSTRUC

STRUC REPASTESPECIAL
.dwAspect RESD 1
.dwParam RESD 1
ENDSTRUC


;------------------------wsock32 structures-----------------------------
STRUC fd_setstruc
.fd_count RESD 1
.fd_array RESD 1
ENDSTRUC

STRUC timeval
.tv_sec RESD 1
.tv_usec RESD 1
ENDSTRUC

STRUC sockaddr_in
.sin_family RESW 1
.sin_port RESW 1
.sin_addr RESD 1
.sin_zero RESB 8
ENDSTRUC

STRUC sockaddr
.sa_family RESW 1
.sa_data RESW 1
ENDSTRUC

STRUC WSAdata
.wVersion RESW 1
.wHighVersion RESW 1
.szDescription RESB WSADESCRIPTION_LEN + 1
.szSystemStatus RESB WSASYS_STATUS_LEN+1
.iMaxSockets RESW 1
.iMaxUdpDg RESW 1
.lpVendorInfo RESD 1
ENDSTRUC

STRUC sockproto
.sp_family RESW 1
.sp_protocol RESW 1
ENDSTRUC

STRUC linger
.l_onoff RESW 1
.l_linger RESW 1
ENDSTRUC

STRUC hostentStru
.h_name RESD 1
.h_alias RESD 1
.h_addr RESW 1
.h_len RESW 1
.h_list RESD 1
ENDSTRUC

STRUC netent
.n_name RESD 1
.n_aliases RESD 1
.n_addrtype RESW 1
.n_net RESD 1
ENDSTRUC

STRUC servent
.s_name RESD 1
.s_aliases RESD 1
.s_port RESW 1
.s_proto RESD 1
ENDSTRUC

STRUC icmp_hdr
.icmp_type RESB 1
.icmp_code RESB 1
.icmp_cksum RESW 1
.icmp_id RESW 1
.icmp_seq RESW 1
.icmp_data RESB 1
ENDSTRUC

STRUC ip_hdr
.ip_hlv RESB 1
.ip_tos RESB 1
.ip_len RESW 1
.ip_id RESW 1
.ip_off RESW 1
.ip_ttl RESB 1
.ip_p RESB 1
.ip_cksum RESW 1
.ip_src RESD 1
.ip_dest RESD 1
ENDSTRUC

STRUC ICMP_OPTIONS
.Ttl RESB 1
.Tos RESB 1
.Flags RESB 1
.OptionsSize RESB 1
.OptionsData RESD 1
ENDSTRUC

STRUC ICMP_ECHO_REPLY
.Address RESD 1
.Status RESD 1
.RoundTripTime RESD 1
.DataSize RESW 1
.Reserved RESW 1
.DataPointer RESD 1
.Options RESD 1
.zData RESB 250
ENDSTRUC

STRUC BLENDFUNCTION
.BlendOp RESD 1
.BlendFlags RESD 1
.SourceConstantAlpha RESD 1
.AlphaFormat RESD 1
ENDSTRUC


struc   DIBSECTION
        .dsBm:          resb    BITMAP_size
        .dsBmih:        resb    BITMAPINFOHEADER_size
        .dsBitfields:   resd    3
        .dshSection:    resd    1
        .dsOffset:      resd    1
endstruc








;=============

STRUC PROCESSENTRY32
.dwSize                    resd         1       ; DWORD
.cntUsage                  resd         1       ; DWORD
.th32ProcessID             resd         1       ; DWORD
.th32DefaultHeapID   resd       1       ; DWORD
.th32ModuleID              resd         1       ; DWORD
.cntThreads                resd         1       ; DWORD
.th32ParentProcessID resd       1       ; DWORD
.pcPriClassBase            resd         1       ; LONG
.dwFlags                   resd         1       ; DWORD
.szExeFile                 resb         260     ; char
ENDSTRUC
;================







struc   DSCAPS
        .dwSize                 resd  1
        .dwFlags                resd  1
        .dwMinSecondarySampleRate resd  1
        .dwMaxSecondarySampleRate resd  1
        .dwPrimaryBuffers       resd  1
        .dwMaxHwMixingAllBuffers resd  1
        .dwMaxHwMixingStaticBuffers resd  1
        .dwMaxHwMixingStreamingBuffers resd  1
        .dwFreeHwMixingAllBuffers resd  1
        .dwFreeHwMixingStaticBuffers resd  1
        .dwFreeHwMixingStreamingBuffers resd  1
        .dwMaxHw3DAllBuffers    resd  1
        .dwMaxHw3DStaticBuffers resd  1
        .dwMaxHw3DStreamingBuffers resd  1
        .dwFreeHw3DAllBuffers   resd  1
        .dwFreeHw3DStaticBuffers resd  1
        .dwFreeHw3DStreamingBuffers resd  1
        .dwTotalHwMemBytes      resd  1
        .dwFreeHwMemBytes       resd  1
        .dwMaxContigFreeHwMemBytes resd  1
        .dwUnlockTransferRateHwBuffers resd  1
        .dwPlayCpuOverheadSwBuffers resd  1
        .dwReserved1            resd  1
        .dwReserved2            resd  1
endstruc

struc   DSBCAPS
        .dwSize                 resd  1
        .dwFlags                resd  1
        .dwBufferBytes          resd  1
        .dwUnlockTransferRate   resd  1
        .dwPlayCpuOverhead      resd  1
endstruc

struc   DSBUFFERDESC
        .dwSize                 resd  1
        .dwFlags                resd  1
        .dwBufferBytes          resd  1
        .dwReserved             resd  1
        .lpwfxFormat            resd  1
endstruc



struc   DS3DBUFFER
        .dwSize                 resd  1
        .vPosition              resd 1
        .vVelocity              resd 1
        .dwInsideConeAngle      resd  1
        .dwOutsideConeAngle     resd  1
        .vConeOrientation             resd 1
        .lConeOutsideVolume     resd  1
        .flMinDistance          resd 1
        .flMaxDistance          resd 1
        .dwMode                 resd  1
endstruc

struc   DS3DLISTENER
        .dwSize                 resd  1
        .vPosition              resd 1
        .vVelocity              resd 1
        .vOrientFront           resd 1
        .vOrientTop             resd 1
        .flDistanceFactor       resd 1
        .flRolloffFactor        resd 1
        .flDopplerFactor        resd 1
endstruc



struc   DSCCAPS
        .dwSize                 resd  1
        .dwFlags                              resd  1
        .dwFormats              resd  1
        .dwChannels             resd  1
endstruc


struc   DSCBUFFERDESC
        .dwSize                 resd  1
        .dwFlags                resd  1
        .dwBufferBytes          resd  1
        .dwReserved             resd  1
        .lpwfxFormat            resd  1
endstruc

struc   DSCBCAPS
        .dwSize                 resd  1
        .dwFlags                resd  1
        .dwBufferBytes          resd  1
        .dwReserved             resd  1
endstruc

struc   DSBPOSITIONNOTIFY
        .dwOffset               resd  1
        .hEventNotify           resd  1
endstruc



; --------------------- Imagehlp.h STRUCTURES ---------------------

STRUC LIST_ENTRY
   .FLink   resd 1
   .BLink   resd 1
ENDSTRUC


STRUC LOADED_IMAGE
    .ModuleName        resd 1
    .hFile             resd 1
    .MappedAddress     resd 1
    .FileHeader        resd 1
    .LastRvaSection    resd 1
    .NumberOfSections  resd 1
    .Sections          resd 1
    .Characteristics   resd 1
    .fSystemImage      resw 1
    .fDOSImage         resw 1
    .Links             resb LIST_ENTRY_size
    .SizeOfImage       resd 1
ENDSTRUC


STRUC IMAGE_DEBUG_INFORMATION
    .List                            RESD LIST_ENTRY_size     ; IN MASM IS  LIST_ENTRY <>
    .Size1                           RESD    1
    .MappedBase                      RESD    1
    .Machine                         RESW    1
    .Characteristics                 RESW    1
    .CheckSum                        RESD    1
    .ImageBase                       RESD    1
    .SizeOfImage                     RESD    1
    .NumberOfSections                RESD    1
    .Sections                        RESD          1
    .ExportedNamesSize               RESD    1
    .ExportedNames                   RESD    1
    .NumberOfFunctionTableEntries    RESD    1
    .FunctionTableEntries            RESD    1
    .LowestFunctionStartingAddress   RESD    1
    .HighestFunctionEndingAddress    RESD    1
    .NumberOfFpoTableEntries         RESD    1
    .FpoTableEntries                 RESD          1
    .SizeOfCoffSymbols               RESD    1
    .CoffSymbols                     RESD          1
    .SizeOfCodeViewSymbols           RESD    1
    .CodeViewSymbols                 RESD    1
    .ImageFilePath                   RESD    1
    .ImageFileName                   RESD    1
    .DebugFilePath                   RESD    1
    .TimeDateStamp                   RESD    1
    .RomImage                        RESB    1     ; IN MASM IS   BYTE     ?
    .DebugDirectory                  RESD          1
    .NumberOfDebugDirectories        RESD    1
    .Reserved                        RESB    3   ;times 3 db 0  ; IN MASM IS   3 dup (?)
ENDSTRUC

STRUC ADDRESS
    .Offset1  RESD      1
    .Segment1 RESW      1
    .Mode     RESD      1
ENDSTRUC

STRUC KDHELP
    .Thread                      RESD    1
    .ThCallbackStack             RESD    1
    .NextCallback                RESD    1
    .FramePointer                RESD    1
    .KiCallUserMode              RESD    1
    .KeUserCallbackDispatcher    RESD    1
    .SystemRangeStart            RESD    1
ENDSTRUC

;---


STRUC STACKFRAME
    .AddrPC          RESB ADDRESS_size  ; MASM IS  ADDRESS      <>
    .AddrReturn      RESB ADDRESS_size  ; MASM IS  ADDRESS      <>
    .AddrFrame       RESB ADDRESS_size  ; MASM IS  ADDRESS      <>
    .AddrStack       RESB ADDRESS_size  ; MASM IS  ADDRESS      <>
    .FuncTableEntry  RESD 1
    .Params          RESB 4            ; times 4 db 0      ; 4 dup (?)
    .Far1            RESD 1            ; BOOL   ?  (BOOL IS DWORD)
    .Virtual         RESD 1            ; BOOL   ?
    .Reserved        RESB 3            ;times 3 db 0      ; 3 dup (?)
    .KdHelp          RESB KDHELP_size  ; KDHELP <>
ENDSTRUC

;---


STRUC API_VERSION
    .MajorVersion    RESW      1
    .MinorVersion    RESW      1
    .Revision        RESW      1
    .Reserved        RESW      1
ENDSTRUC


STRUC IMAGEHLP_SYMBOL
    .SizeOfStruct    RESD   1
    .Address         RESD   1
    .Size1           RESD   1
    .Flags           RESD   1
    .MaxNameLength   RESD   1
    .Name1           RESB   1     ;  db     1
ENDSTRUC



STRUC IMAGEHLP_MODULE
    .SizeOfStruct    RESD   1
    .BaseOfImage     RESD   1
    .ImageSize       RESD   1
    .TimeDateStamp   RESD   1
    .CheckSum        RESD   1
    .NumSyms         RESD   1
    .SymType1        RESD   1
    .ModuleName      RESB   32    ;BYTE  32 dup (?)
    .ImageName       RESB   256   ;BYTE 256 dup (?)
    .LoadedImageName RESB   256   ;BYTE 256 dup (?)
ENDSTRUC

STRUC IMAGEHLP_LINE
    .SizeOfStruct    RESD    1
    .Key             RESD    1
    .LineNumber      RESD    1
    .FileName        RESD    1
    .Address         RESD    1
ENDSTRUC

STRUC IMAGEHLP_DEFERRED_SYMBOL_LOAD
    .SizeOfStruct    RESD    1
    .BaseOfImage     RESD    1
    .CheckSum        RESD    1
    .TimeDateStamp   RESD    1
    .FileName        RESB    MAX_PATH    ;BYTE MAX_PATH dup (?)
    .Reparse         RESB    1           ;BYTE     ?
ENDSTRUC

STRUC IMAGEHLP_DUPLICATE_SYMBOL
    .SizeOfStruct    RESD   1
    .NumberOfDups    RESD   1
    .Symbol          RESD   1
    .SelectedSymbol  RESD   1
ENDSTRUC



;***********Imagehlp.h STRUCTURES  end ************
struc   IMAGE_VXD_HEADER
        .e32_magic:             resw    1
        .e32_border:            resb    1
        .e32_worder:            resb    1
        .e32_level:             resd    1
        .e32_cpu:               resw    1
        .e32_os:                resw    1
        .e32_ver:               resd    1
        .e32_mflags:            resd    1
        .e32_mpages:            resd    1
        .e32_startobj:          resd    1
        .e32_eip:               resd    1
        .e32_stackobj:          resd    1
        .e32_esp:               resd    1
        .e32_pagesize:          resd    1
        .e32_lastpagesize:      resd    1
        .e32_fixupsize:         resd    1
        .e32_fisupsum:          resd    1
        .e32_ldrsize:           resd    1
        .e32_ldrsum:            resd    1
        .e32_objtab:            resd    1
        .e32_objcnt:            resd    1
        .e32_objmap:            resd    1
        .e32_itermap:           resd    1
        .e32_rsrctab:           resd    1
        .e32_rsrccnt:           resd    1
        .e32_restab:            resd    1
        .e32_enttab:            resd    1
        .e32_dirtab:            resd    1
        .e32_dircnt:            resd    1
        .e32_fpagetab:          resd    1
        .e32_frectab:           resd    1
        .e32_impmod:            resd    1
        .e32_impmodcnt:         resd    1
        .e32_impproc:           resd    1
        .e32_pagesum:           resd    1
        .e32_datapage:          resd    1
        .e32_preload:           resd    1
        .e32_nrestab:           resd    1
        .e32_cbnrestab:         resd    1
        .e32_nressum:           resd    1
        .e32_autodata:          resd    1
        .e32_debuginfo:         resd    1
        .e32_debuglen:          resd    1
        .e32_instpreload:       resd    1
        .e32_instdemand:        resd    1
        .e32_heapsize:          resd    1
        .e32_res3:              resb    12
        .e32_winresoff:         resd    1
        .e32_winreslen:         resd    1
        .e32_devid:             resw    1
        .e32_ddkver:            resw    1
endstruc

struc   IMAGE_OS2_HEADER
        .ne_magic:              resw    1
        .ne_ver:                resb    1
        .ne_rev:                resb    1
        .ne_enttab:             resw    1
        .ne_cbenttab:           resw    1
        .ne_crc:                resd    1
        .ne_flags:              resw    1
        .ne_autodata:           resw    1
        .ne_heap:               resw    1
        .ne_stack:              resw    1
        .ne_csip:               resd    1
        .ne_sssp:               resd    1
        .ne_cseg:               resw    1
        .ne_cmod:               resw    1
        .ne_cbnrestab:          resw    1
        .ne_segtab:             resw    1
        .ne_rsrctab:            resw    1
        .ne_restab:             resw    1
        .ne_modtab:             resw    1
        .ne_imptab:             resw    1
        .ne_nrestab:            resd    1
        .ne_cmovent:            resw    1
        .ne_align:              resw    1
        .ne_cres:               resw    1
        .ne_exetyp:             resb    1
        .ne_flagsothers:        resb    1
        .ne_pretthunks:         resw    1
        .ne_psegrefbytes:       resw    1
        .ne_swaparea:           resw    1
        .ne_expver:             resw    1
endstruc




struc   CONNECTDLGSTRUCT
        .cbStructure:   resd    1
        .hwndOwner:     resd    1
        .lpConnRes:     resd    1
        .dwFlags:       resd    1
        .dwDevNum:      resd    1
endstruc

struc   DISCDLGSTRUCT
        .cbStructure:   resd    1
        .hwndOwner:     resd    1
        .lpLocalName:   resd    1
        .lpRemoteName:  resd    1
        .dwFlags:       resd    1
endstruc

struc   NETINFOSTRUCT
        .cbStructure:           resd    1
        .dwProviderVersion:     resd    1
        .dwStatus:              resd    1
        .dwCharacteristics:     resd    1
        .dwHandle:              resd    1
        .wNetType:              resw    1
        .dwPrinters:            resd    1
        .dwDrives:              resd    1
endstruc

struc   NETCONNECTINFOSTRUCT
        .cbStructure:   resd    1
        .dwFlags:       resd    1
        .dwSpeed:       resd    1
        .dwDelay:       resd    1
        .dwOptDataSize: resd    1
endstruc

struc   POINTFLOAT
        .x:     resd    1
        .y:     resd    1
endstruc

struc   GLYPHMETRICSFLOAT
        .gmfBlackBoxX:          resd    1
        .gmfBlackBoxY:          resd    1
        .gmfptGlyphOrigin:      resb    POINTFLOAT_size
        .gmfCellIncX:           resd    1
        .gmfCellIncY:           resd    1
endstruc

struc   LAYERPLANEDESCRIPTOR
        .nSize:                 resw    1
        .nVersion:              resw    1
        .dwFlags:               resd    1
        .iPixelType:            resb    1
        .cColorBits:            resb    1
        .cRedBits:              resb    1
        .cRedShift:             resb    1
        .cGreenBits:            resb    1
        .cGreenShift:           resb    1
        .cBlueBits:             resb    1
        .cBlueShift:            resb    1
        .cAlphaBits:            resb    1
        .cAlphaShift:           resb    1
        .cAccumBits:            resb    1
        .cAccumRedBits:         resb    1
        .cAccumGreenBits:       resb    1
        .cAccumBlueBits:        resb    1
        .cAccumAlphaBits:       resb    1
        .cDepthBits:            resb    1
        .cStencilBits:          resb    1
        .cAuxBuffers:           resb    1
        .iLayerPlane:           resb    1
        .bReserved:             resb    1
        .crTransparent:         resd    1
endstruc



struc   USER_INFO_0
        .usri0_name:    resd    1
endstruc



struc   LOCALGROUP_INFO_0
        .lgrpi0_name:   resd    1
endstruc






STRUC WSADATA 
  .wVersion      RESW 1
  .wHighVersion  RESW 1
  .szDescription   RESB  WSADESCRIPTION_LEN + 1 
  .szSystemStatus  RESB WSASYS_STATUS_LEN + 1 
  .iMaxSockets   RESW 1
  .iMaxUdpDg     RESW 1
  .lpVendorInfo  RESD 1
ENDSTRUC





STRUC ITaskBarList
    ;==This are pointers to IUnknown interface 
   .QueryInterface    resd 1
   .AddRef                resd 1
   .Release                resd 1
   ;==This are pointers to Methods (functions) of ITaskbarlist    
  
   .HrInit                 resd 1
   .AddTab                 resd 1
   .DeleteTab              resd 1
   .ActivateTab            resd 1
   .SetActiveAlt           resd 1
 ENDSTRUC

  
STRUC INITCOMMONCONTROLSEX
  .dwSize  RESD 1
  .dwICC   RESD 1
ENDSTRUC


STRUC st_mysql_field 
	.name        resd 1        ; column name <PTR>
	.table		resd 1		; table of column if col was field <PTR>
	.def		resd 1	; defaults val (from mysql_list_fields) <PTR>
	.type		resd 1	; type of field
	.length	resd 1	; width
	.max_length	resd 1		; max width of set
	.flags		resd 1		; div flags
	.decimals	resd 1		; no. of decimals in field
ENDSTRUC






STRUC COMBOBOXEXITEM 
    .fmask			resd 1
    .iItem			resd 1
    .pszText			resd 1
    .cchTextMax		resd 1
    .iImage			resd 1
    .iSelectedImage	resd 1
    .iOverlay		        resd 1
    .iIndent			resd 1
    .lParam			resd 1
ENDSTRUC




; MACROS FROM NAGOA  for NASM 
; version  date  :  14 Março 2004  
; ---------------------------------------------------------------------
; ---------------------------------------------------------------------
; 
; pedrogc macros proc endproc stack  20-março-2003 (undef resolved)  
; nuMIT_or  PEMAC  only NASM  , adapted to NAGOA by PedroGC 10-fev-2004  
; ADDR_   or  addr  or ADDR   ( lea eax , [xxx] )  LuckyStar  15- jan-2004
; NPEMAC   ( nuMIT_or ) edit by PedroGC 12-fev-2004  (  %define ONLY_NASM )
; pedrogc 14 march 2004  proc end proc stack new updated version    
; pedrogc 14 march 2004 GUID_ macro
;=====================================================================


;========================================================
;  for calling external function in dll´s or ocx
;------------------------------------------------------------------------
;   INVOKE or   invoke or STDCALL or stdcall  or CALL_  or call_
;========================================================

;%define call call_   ; remove this ! if you do not like call macro 

;========================================================
;
;  example  
;  invoke MessageBoxA,NULL,"Info to say","the title",MB_OK
;  or
;  stdcall MessageBoxA,NULL,"Info to say","the title",MB_OK
;  or 
;  call_   MessageBoxA,NULL,"Info to say","the title",MB_OK
;
;======================================================================
;  local function    is  LOCCALL or loccall
; =====================================================================
;  
;  loccall myMessage,"ola meu","my tiltle"
;
;======================================================================
;  NEW MACRO CAN USE  * ADDR_ *   it will out put   LEA EAX,[var]   ,   push eax     
;======================================================================
; proc teste,ola
;    
;     stack opf, OPENFILENAME_size
;
;     call MessageBoxA , NULL,[ola],"title1",MB_OK
;     call RtlZeroMemory,  ADDR_ opf   , OPENFILENAME_size      <-  LOOK HERE 
;     mov dword [opf],OPENFILENAME_size 
;     call  GetOpenFileName ,   ADDR_ opf                       <-  LOOK HERE 
;     call ExitProcess,0
;
; endproc
;
;========================================================================


;----------------------------------------------------------
;db        = byte          1 Byte ( 8 Bit)
;dw        = word          2 Byte (16 Bit)
;dd        = doubleword    4 Byte (32 Bit)
;----------------------------------------------------------

;;=====pedrogc macros ==========================
;------------------------------------------------
; proc Funcion,arg1,..,argN
;
;   sectiondata
;   var_1  dd 0
;   var_2  dd 0
;   sectioncode
;    
;    stack var1,4,msg,MSG_size,hWnd,4  ; locals using stack !
;
;  mov [hWnd],eax
;
;  
;
;  return           ; jump to endrotin
;
;  lea eax,[msg]
;
;  return 1        ;  jmp to endrotin and eax=1
;
;  mov eax,[arg1]
;
; endproc
;-------------------------------------------------

; dword = 4  ;)

;-------------------------------------------------



%MACRO pushd  1
  push DWORD %1
 %ENDMACRO

 %MACRO popd 1
  pop DWORD %1
 %ENDMACRO

 %MACRO pushb 1
  push BYTE %1
 %ENDMACRO

 %MACRO movd 2
  mov DWORD %1, %2
 %ENDMACRO



; ------- Segments Macros -------------------------------

 %MACRO data32 0
  section data class=data use32 align=16
 %ENDMACRO

 %MACRO _data 0
  [segment .data use32]
 %ENDMACRO

 %MACRO code32 0
  section code class=code use32 align=16
 %ENDMACRO

 %MACRO _code32 0
  [segment .code use32]
 %ENDMACRO

 %MACRO bss32 0
  section bss class=bss use32 align=16
 %ENDMACRO








;;================================
;; MACRO  str2int     int2str by Written by mastercpp.
;;================================
%macro str2int 1
        push    ebx ;
        push    esi ;
        push    edi ;
        mov ebx, 0
        mov ecx, 0
        xor eax,eax
        mov ebx,0000000Ah
        mov esi,offset %1
        %%ConvertLoop
        movzx ecx,byte [esi] ;Zeichen laden.
        test ecx,ecx
        jz  short %%ExitConvertLoop ;0 => Exit
        inc esi
        sub cl,30h ;0-9...
        mul ebx ;Ergebnis * 10
        add eax,ecx ;+ nächste Ziffer
        jmp short %%ConvertLoop
        %%ExitConvertLoop
        pop     edi
        pop     esi
        pop     ebx
%endmacro

%macro int2str 2
        push    ebx ;
        push    esi ;
        push    edi ;
        %%start
        mov  eax, %1
        xor  ecx, ecx
        mov  ebx, 000ah
        %%DecConvert
        xor  edx,  edx
        div  ebx
        add  edx,  0030h
        push edx
        inc  ecx
        or   eax,  eax
        jnz  short %%DecConvert
        mov  edi,  %2
        %%SortDec
        pop   eax
        stosb
        loop  %%SortDec
        mov eax, 0h
        stosb
        pop     edi
        pop     esi
        pop     ebx
%endmacro
;;================================
;; mastercpp  end macros
;;================================


;; -----------------------------------
;; INPUT red, green & blue BYTE values
;; OUTPUT DWORD COLORREF value in eax
;; -----------------------------------
;; using example:
;;-----------------------------------
;; RGB    200,200,50
;;------------------------------------
%macro RGB 3
        xor eax, eax
        mov ah, %3
        shl eax, 8
        mov ah, %2
        mov al, %1
%endmacro



;=== HGB THANKS !
%macro def 1
        definir %1
%endmacro

%macro definir 1
        %define %1_definida
%endmacro
;=============


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



;=== [ end macro call_ ] =====================



;=================================================================
; thanks Scali at #win32asm efnet for the help on this  by PedroGC
;=================================================================
;    mmioFOURCC,'W', 'A', 'V', 'E'               the result is in eax  =   EVAW
;---------------------------------------------------------------------------------------------------------------------

%macro mmioFOURCC 4
%define ch0 %1
%define ch1 %2
%define ch2 %3
%define ch3 %4
    mov al, ch3
    shl eax, 8
    mov al, ch2
    shl eax, 8
    mov al, ch1
    shl eax, 8
    mov al, ch0
%endmacro


;-----to place constantes in side the code-------------------------
;  EXAMPLE OF THIS
;------------------------------------------------------------------
;
;   SomeRotine:
;
;      sectiondata
;      hWnd2 dd 0
;      hInst dd 0
;      rect resb RECT_size
;
;      filtro:  db      "asm", 0,       "*.asm", 0
;                   db  "inc", 0,       "*.inc", 0
;                   db  "rc", 0,        "*.rc", 0
;                   db  0,              0
;      sectioncode
;
;
;
;
;
;   call GetWindowText, [hMainWin], ebx, MAX_PATH
;       test    eax, eax
;       jnz     .save
;
; filePrompt:
;
;  invoke       fselSelect, filtro, ebx, 0, 0, filtro
;
;------------------------------------------------------------------

%define sectiondata [section data]
%define sectioncode [section code]

;------------------------------------------------------------------


;---- means nothing in NASM --------
%idefine offset
;------------------------------------

%imacro m2m 2
push dword %2
pop dword %1
%endmacro

;================================================================
;  CALLCOM or callcom for COM calling   by PedroGC
;================================================================
;   USAGE :
;================================================================
; callcom ptObject,ptMethod,var1,...,var_n
;================================================================
%macro CALLCOM  2-*
%define ptInterface %1
%define method %2
%rep %0-2
%rotate -1
push dword %1
%endrep
 mov  eax,   [ptInterface]                  ; pointer to the interface
 mov  eax,   [eax]                            ; pointer to VTable [in DLL]
 push dword [ptInterface]
 CALL [eax+method]
%endmacro


%macro callcom  2-*
%define ptInterface %1
%define method %2
%rep %0-2
%rotate -1
push dword %1
%endrep
 mov  eax,   [ptInterface]                  ; pointer to the interface
 mov  eax,   [eax]                            ; pointer to VTable [in DLL]
 push dword [ptInterface]
 CALL [eax+method]
%endmacro



;================================================================
;----------------------------------
; CALL FOR COM OBJECTS !!!
;-----------------------------------
; macro COMCALL
;----------------------------------
; push dword varn
;     .......
; push dword var1
; comcall object , method
;            %1        %2
;-----------------------------------
;-----------------------------------
%macro COMCALL 1-*
mov eax,dword [%1]     ;mov eax, dword  [%1]
push eax
mov eax, dword  [eax]
CALL [eax+%2]
%endmacro

%macro comcall 1-*
mov eax,dword [%1]    ;mov eax, dword  [%1]
push eax
mov eax, dword  [eax]
CALL [eax+%2]
%endmacro








;****************************************************
;                                       HLL macros
;****************************************************
;  - for/next
;
%imacro for 5
        %push for
        %define %$v %1
        %define %$c %3
        %define %$e %4
        %define %$s %5
        mov     %1, %2
%$f:
%endmacro
%imacro next 0
        %ifctx for
                add     %$v, %$s
                cmp     %$v, %$e
                j%$c    near %$f
                %pop
        %else
                ; %error "Expected 'for' before 'next'"
        %endif
%endmacro
;

;
; HLL macros - if/elseif/else/ifand/endif
;
%imacro if 3-*
        %push if
	%$if:
	%assign %$i 1
        %if %0 = 3
                cmp     %1, %3
                j%-2    near %$else_%$i
        %else
                %assign %$c 0
                %rep (%0 / 3)
                        cmp     %1,%3
                        j%2     near %$c_%$i
                        %rotate 3
                %endrep
                jmp     near %$else_%$i
		%$c_%$i:
        %endif
%endmacro

%imacro ifand 3-*
        %push if
	%$if:
	%assign %$i 1
        %if %0 = 3
                cmp     %1, %3
                j%-2    near %$else_%$i
        %else
                %rep (%0 / 3)
                        cmp     %1, %3
                        j%-2    near %$else_%$i
                        %rotate 3
                %endrep
        %endif
%endmacro

%imacro elseif 3-*.nolist
	%ifctx if
		jmp	near %$endif
		%$else_%$i:
		%assign %$i %$i+1
		%if %0=3
			cmp %1,%3
			j%-2 near %$else_%$i
		%else		
			%rep (%0 / 3)
				cmp %1,%3
				j%2 near %$c_%$i
				%rotate 3
			%endrep
			jmp     near %$else_%$i
			%$c_%$i:
		%endif
	%else
		%error "'ELSEIF' can only be used following 'IF'"
	%endif
%endmacro


%imacro else 0
        %ifctx if
                jmp     near %$endif
                %$else_%$i:
		%assign %$i %$i+1
        %else
                %error "Expected 'if' or 'ifand' before 'else'"
        %endif
%endmacro
%imacro endif 0.nolist
        %ifctx if
		%$else_%$i:
		%$endif:
		%pop
        %else
                %error "Expected 'if' before 'endif'"
        %endif
%endmacro
;***************************





;***************************
; HLL macros - repeat/until
;
%imacro repeat 0
        %push repeat
%$r:
%endmacro
%imacro until 3-*
        %ifctx repeat
                %if %0 = 3
                        cmp     %1, %3
                        j%-2    near %$r
                %else
                        %rep (%0 / 3)
                                cmp     %1, %3
                                j%-2    near %$r
                                %rotate 3
                        %endrep
                %endif
                %pop
        %else
                ; %error "Expected 'repeat' before 'until'"
        %endif
%endmacro
%imacro untiland 3-*
        %ifctx repeat
                %if %0 = 3
                        cmp     %1, %3
                        j%-2    near %$r
                %else
                        %rep (%0 / 3)
                                cmp     %1, %3
                                j%2     near %$x
                                %rotate 3
                        %endrep
                        jmp     %$r
                        %$x:
                %endif
                %pop
        %else
                ; %error "Expected 'repeat' before 'untiland'"
        %endif
%endmacro

;****************************************
; HLL macros - switch
;
%imacro switch 1
        %push switch
        %define %$v %1
        %assign %$c 0
%endmacro
%imacro case 1-*
        %ifctx switch
                %$s%$c:
                %assign %$c %$c + 1
                %if %0 = 1
                        cmp     %$v, %1
                        jne     near %$s%$c
                %else
                        %rep %0
                                cmp     %$v, %1
                                je      near %$sc%$c
                                %rotate 1
                        %endrep
                        jmp     %$s%$c
                        %$sc%$c:
                %endif
                %repl case
        %else
                ; %error "Expected 'switch' before 'case' or missing 'break' in previous 'case'"
        %endif
%endmacro
%imacro caseand 1-*
        %ifctx switch
                %$s%$c:
                %assign %$c %$c + 1
                %if %0 = 1
                        cmp     %$v, %1
                        jne     near %$s%$c
                %else
                        %rep %0
                                cmp     %$v, %1
                                jne     near %$s%$c
                                %rotate 1
                        %endrep
                %endif
                %repl case
        %else
                ; %error "Expected 'switch' before 'caseand' or missing 'break' in previous 'case'"
        %endif
%endmacro
%imacro range 2-*
        %ifctx switch
                %if %0 = 2
                        %$s%$c:
                        %assign %$c %$c + 1
                        cmp     %$v, %1
                        jl      near %$s%$c
                        cmp     %$v, %2
                        jg      near %$s%$c
                %else
                        %$s%$c:
                        %assign %$c %$c + 1
                        %assign %$d 100
                        %rep (%0 / 2)
                                        cmp     %$v, %1
                                        jl      %$sr%$d
                                        cmp     %$v, %2
                                        jle     near %$sre%$c
                                %$sr%$d:
                                %assign %$d %$d + 1
                                %rotate 2
                        %endrep
                        jmp     %$s%$c
                        %$sre%$c:
                %endif
                %repl case
        %else
                ; %error "Expected 'switch' before 'range' or missing 'break' in previous 'case'"
        %endif
%endmacro
%imacro rangeand 2-*
        %ifctx switch
                %if %0 = 2
                        %$s%$c:
                        %assign %$c %$c + 1
                        cmp     %$v, %1
                        jl      near %$s%$c
                        cmp     %$v, %2
                        jg      near %$s%$c
                %else
                        %$s%$c:
                        %assign %$c %$c + 1
                        %rep (%0 / 2)
                                        cmp     %$v, %1
                                        jl      near %$s%$c
                                        cmp     %$v, %2
                                        jg      near %$s%$c
                                %rotate 2
                        %endrep
                %endif
                %repl case
        %else
                ; %error "Expected 'switch' before 'rangeand' or missing 'break' in previous 'case'"
        %endif
%endmacro
%imacro break 0
        %ifctx case
                jmp     %$e
                %repl switch
        %else
                ; %error "Expected 'case' before 'break'"
        %endif
%endmacro
%imacro default 0
        %ifctx switch
                %$s%$c:
                %assign %$c %$c + 1
        %else
                ; %error "Expected 'switch' before 'default'"
        %endif
%endmacro
%imacro switchend 0
        %ifctx switch
                %$e:
                %$s%$c:
                %pop
        %else
                ; %error "Expected 'switch' before 'switchend'"
        %endif
%endmacro


;*************************************************
; HLL macros - while/wend
;
%imacro while 3-*
        %push while
        %$w:
        %if %0 = 3
                cmp     %1, %3
                j%-2    near %$x
        %else
                %assign %$c 0
                %rep (%0 / 3)
                        cmp     %1, %3
                        j%2     near %$w%$c
                        %rotate 3
                %endrep
                jmp     %$x
                %$w%$c:
        %endif
%endmacro
%imacro whileand 3-*
        %push while
        %$w:
        %if %0 = 3
                cmp     %1, %3
                j%-2    near %$x
        %else
                %assign %$c 0
                %rep (%0 / 3)
                        cmp     %1, %3
                        j%-2    near %$x
                        %rotate 3
                %endrep
        %endif
%endmacro
%imacro wend 0
        %ifctx while
                jmp     %$w
        %$x:
                %pop
        %else
                ; %error "Expected 'while' before 'wend'"
        %endif
%endmacro

;****************************************************
;            end                HLL macros
;****************************************************






; ----  Anton Eremeev  edit PedroGC

;=====================================================
 %imacro CONST 2+	; macro to insert constant into code
	jmp near %%skip	; jump to macro-local label
	%1 %2		; mark label and define data
	%%skip:		; macro-local label
 %endm
;======================================================




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





; -------> FOR WIN32API use : call or stdcall or invoke
%imacro ccall 1-*.nolist
	%push ccall
	%assign %$i %0-1
	%assign %$params %0-1
	%rep %0-1
		%rotate -1  
			%assign %$i	%$i-1
			%rotate -1
			%ifidni %1,"ADDR_"
				%rotate 1
				lea eax,[%1]
				push eax
				%rotate -1
				%assign %$i %$i-1
				%assign %$params %$params-1
			%else		
				%rotate 1
				STDPUSH {%1}
			%endif
			%if %$i=0
				%exitrep
			%endif
	%endrep
	%rotate -1
	%ifid %1
		%ifdef %1_definida
			CALL %1
			%assign %$params %$params*4
			add esp,%$params
		%else
			%ifdef ONLY_NASM
				CALL [%1]
			%else
				extern %1
				CALL [%1]
			%endif
			%assign %$params %$params*4
			add esp,%$params
		%endif
	%else
		CALL	%1
		%assign %$params %$params*4
		add esp,%$params
	%endif
	%pop
%endmacro



;------------- for internal local functions


;=============== [ LOCAL FUNCTIONS   *LOCCALL* ] =====
; examples :
; loccall LocalFunction,ADDR_ msg
; loccall Message,  " Some Text " , " title "
;
;======================================= 

%imacro loccall 1-*  
%assign i	%0
	%if %0 > 1
		%rep %0 - 1
			%rotate -1  
			%assign i	i-1
			; ===== ADDR_
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
	CALL %1
%endmacro
;===================== [ LOCAL FUNCTIONS END *LOCCALL* ]



;========= nuMIT_or MACROS 

; ------------------ 
; DO_UNICODE macro
; ------------------
%macro DO_UNICODE 1
 %assign  %%a 0
 %assign  %%b 0
 %strlen  %%b %1
 %assign  %%c 1
 %assign  %%d 0
 %rep     %%b
  %substr  %%a %1 %%c
  %if %%d=0
    %ifidn %%a, '/'
        %assign %%d 1
    %else
      DW %%a
    %endif
  %else
    %assign %%d 0
    %ifidn %%a, '/'
      DW '/'
    %elifidn %%a, 'n'
      DW 0Dh, 0Ah
    %elifidn %%a, 'r'
      DW 0Dh
    %elifidn %%a, 'l'
      DW 0Ah
    %elifidn %%a, 's'
      DW 20h
    %elifidn %%a, 'c'
      DW 3Bh
    %elifidn %%a, 't'
      DW 09h
    %elifidn %%a, '0'
      DW 0
    %elifidn %%a, '1'
      DW 1
    %else
      DW '/', %%a
    %endif
  %endif
  %assign %%c %%c+1
 %endrep
%endmacro
; ----------------------------
; macro: DO_UNICODEZ
; ----------------------------
%macro DO_UNICODEZ 1
 DO_UNICODE %1
 set_wzero 
%endmacro
; ----------------------------
; set_wchar macro
; ----------------------------
%macro set_wchar 1
  DW %1
%endmacro
; ----------------------------
; set_wzero macro
; ----------------------------
%define set_wzero DW 0
; ----------------------------
; TEXTW macro
; ----------------------------
%macro TEXTW 2
 %strlen %%a %2
 align 2, DB 0
 %1:
    s%1 EQU %1
    DO_UNICODEZ %2
    L%1 EQU %%a
%endmacro
; ----------------------------
; TEXTA macro
; ----------------------------
%macro TEXTA 2
 %strlen %%a %2
 align 1, DB 0
 %1:
    s%1 EQU %1
    ATEXT %2
    L%1 EQU %%a
%endmacro


; ----------------------------
; PushStr macro
; ----------------------------
%macro	PushStr 1+
 CALL	%%a
 DB	%1, 0
 ALIGN  4, DB 0
%%a
%endmacro
; ----------------------------
; PushUStr macro
; ----------------------------
%macro  PushUStr 1+
 CALL	%%a
    DO_UNICODEZ %1
%%a
%endmacro
; ----------------------------
; pushn macro
; ----------------------------
%macro pushn 1
 lea  eax, %1
 push eax
%endmacro
; ----------------------------
; SZSTRING macro
; ----------------------------
%macro	SZSTRING 2
	CALL	%%a
	%1 db %2, 0
	%%a
%endmacro

;========= nuMIT_or MACROS END










; :::: PE MACROS   nguga ::::::::::::::
;===========================================

%macro IMPORT_END 0-*  
_end_import:
%define	import_size _end_import - import
end
%endmacro

%define IMPORT_BEGIN section .text

%define LAST_BEGIN    section .text
%define LAST_END       IMPORT_END

; :::::::: end PE macros nguga ::::::::::::::::





;======= GUID_   macro by PedroGC =====
%macro GUID_ 0-*
%1:
ISTRUC GUID
AT GUID.Data1 , dd %2     ;56FDF344h
AT GUID.Data2 , dw %3    ;0FD6Dh
AT GUID.Data3 ,	dw %4   ;11D0h
AT GUID.Data4 , db %5     ;{95h,8Ah,0,60h,97h,0C9h,0A0h,90h}
IEND
%endmacro
;================================
; example declaring a GUID with this macro
;
; GUID_ IID_ITaskbarList , 56fdf342h ,0FD6Dh,11D0h,{95h,8Ah,0,60h,97h,0C9h,0A0h,90h}
;===================================================================================

;===============================================
;  PROC ENDPROC  and STACK locals  by PedroGC
;____________________________________________________
; proc	funcao3,arg_1,arg_2,arg_3
;
;	         stack	hwnd,4,msg,MSG_size,var_3,10
;
;mov eax,[@hwnd]
;mov eax,[@msg]
;  return                ; go to end of proc
;
;mov eax,[@var_3]
;mov eax,[@arg_1]
;
;return TRUE         ; go to end of proc and  eax = 1
;
;mov eax,[@arg_2]
;mov eax,[@arg_3]
;endproc	funcao3
;--------------------------------------------------------------
;                                   dword = 4 
;=================================================
%imacro proc 1-*.nolist
	%assign SizeLocalAdd 0  ; inicializar o size das locais a zero
	%assign stack_var  1     ; inicializar a stack pra primeira  variavel declarada
	%assign c_l 1  ; INICILIZAR OS ARGUMENTOS PARA O PRIMEIRO !
	%assign n_l %0-1
	%assign numarg_ %0-1   ; para fazer ret final
	%define nome_rotina %1

	%push proc
	global %1
	%1:
	push ebp
	mov ebp,esp
	;== loop para defenir parametros
	%rep %0-1
		%rotate 1
		%xdefine @%1  ebp+4 +c_l*4 
		%assign c_l c_l+1
	%endrep      
	;=== loop end  para defenir parametros
%endmacro
;; ==================

;---
;Usage: proc Function
;            uses edi,esi,"C"
;       endproc
;Saves the register at start of proc and retains at the end of function
;automatically. Also it makes a proc, a C proc too. 
;---
%imacro uses 1-*.nolist
	%ifctx proc
		%push uses
		%assign %$j 1
		%assign %$count %0
		%rep %0
			%ifstr %1
				%ifidni %1,"C"
					%define __@CFunction
					%assign %$count %$count-1
					%rotate 1
				%endif
			%else
				%xdefine %$reg_%$j %1
				push dword %1
				%assign %$j %$j+1
				%rotate 1
			%endif
		%endrep
	%else
		%error "'USES' must be used inside 'PROC/ENDPROC'"
	%endif
%endmacro


;======================
; START STACK  LOCALS
;======================
; tipo par de argumentos sem fim
; stack var1,4,var2,4,msg,28
;======================

%imacro stack 1-*.nolist
	%ifctx proc
		%assign %%pare_Argts ((%0) / 2)
		%assign _SomaDasLocais_ 0
		%rep %%pare_Argts
			%assign SizeLocalAdd SizeLocalAdd +%2
			%xdefine @%1 ebp-SizeLocalAdd 
			%assign stack_var stack_var+1          
			%assign _SomaDasLocais_ _SomaDasLocais_ +%2
			%rotate 2
		%endrep        ; end loop para defenir parametros
		sub esp , _SomaDasLocais_
	%endif
%endmacro



;=====================
;  END STACK LOCALS
;=====================
%imacro endproc 0-1.nolist
	terminar_ nome_rotina
	%ifctx uses
		%assign %$j %$j-1
		%rep %$count
			pop dword %$reg_%$j
			%assign %$j %$j-1
		%endrep
		%pop
	%endif
	%ifctx proc
		leave
		%ifdef __@CFunction
			ret
		%else
			%if numarg_
				ret 4*numarg_
			%else
				ret
			%endif
		%endif
		%pop
	%endif
%endmacro

;; ==========_ret  ========================
;; return macro  , can only be used in proc endproc
;; ======================================
%imacro  return  0-1.nolist
	%if %0
		%if %1=1
			xor     eax, eax
			inc     eax
		%elif %1=0
			xor     eax, eax
		%else
			mov eax, %1
		%endif
	%endif
	l_ret_l
%endmacro

%macro terminar_ 1
        %1terminar_:
%endmacro
%macro _brincar 2
             jmp near %1%2
%endmacro
%macro l_ret_l 0
            _brincar nome_rotina,terminar_
%endmacro
;===== undef resolved  =============================
;; PROC END PROC and STACK locals macros by Nguga
;;==============================================