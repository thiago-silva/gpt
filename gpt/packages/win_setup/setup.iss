; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=G-Portugol
AppVerName=G-Portugol versão 1.1
AppPublisherURL=http://gpt.berlios.de
AppCopyright=Copyright (C) 2003-2008 Thiago Silva
LicenseFile=..\..\COPYING
DefaultDirName={sd}\gpt
DefaultGroupName=G-Portugol
Compression=bzip
ShowLanguageDialog=yes
OutputDir="_setup"
ChangesEnvironment=yes

[Tasks]
Name: modifypath; Description: &Adicionar diretório da aplicação na variável de sistema PATH;
Name: notepadfiles; Description: &Configurações para o editor Notepad++ (ATENÇÃO: dados serão sobrepostos);


[Files]
Source: "..\..\build\src\gpt.exe"; DestDir: "{app}\bin"
;Source: "bin\pcrecpp.dll"; DestDir: "{app}\bin"
;Source: "bin\pcre.dll"; DestDir: "{app}\bin"
Source: "bin\nasm.exe"; DestDir: "{app}\bin"
Source: "bin\gptshell.bat"; DestDir: "{app}\bin"
Source: "..\..\lib\base.gpt"; DestDir: "{app}\lib"
Source: "codigos\olamundo.gpt"; DestDir: "{app}\codigos"
Source: "..\..\README.pt_BR"; DestName: "LEIAME.txt"; DestDir: "{app}\doc"; Flags: isreadme
Source: "copy\BSD.COPYING.txt"; DestDir: "{app}\doc"
Source: "copy\GNU.COPYING.txt"; DestDir: "{app}\doc"
Source: "..\..\AUTHORS"; DestName: "AUTORES.txt"; DestDir: "{app}\doc"
Source: "..\..\THANKS"; DestName: "AGRADECIMENTOS.txt"; DestDir: "{app}\doc"
Source: "doc\MUDANÇAS.txt"; DestDir: "{app}\doc"
Source: "..\..\doc\manual\manual.pdf"; DestName: "G-Portugol - Manual.pdf"; DestDir: "{app}\doc"


;syntax highlight
Source: "notepad++\userDefineLang.xml"; DestDir: "{app}\Notepad++";
;keyboard command shortcuts
Source: "notepad++\shortcuts.xml";      DestDir: "{app}\Notepad++";
;New doc defaults to UTF-8
Source: "notepad++\config.xml";         DestDir: "{app}\Notepad++";

Source: "notepad++\userDefineLang.xml"; DestDir: "{userappdata}\Notepad++"; Flags: uninsneveruninstall;  Tasks: notepadfiles
Source: "notepad++\shortcuts.xml";      DestDir: "{userappdata}\Notepad++"; Flags: uninsneveruninstall;  Tasks: notepadfiles
Source: "notepad++\config.xml";         DestDir: "{userappdata}\Notepad++"; Flags: uninsneveruninstall;  Tasks: notepadfiles

[Icons]
Name: "{group}\G-Portugol"; Filename: "{app}\bin\gptshell.bat"; WorkingDir: "{app}"
Name: "{group}\Manual"; Filename: "{app}\doc\G-Portugol - Manual.pdf"; WorkingDir: "{app}"
Name: "{group}\LEIAME"; Filename: "{app}\doc\LEIAME.txt"; WorkingDir: "{app}"

[Languages]
Name: pt_BR; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "GPT_INCLUDE"; ValueData: "{app}\lib\base.gpt"; Flags: uninsdeletevalue

[Code]
function ModPathDir(): TArrayOfString;
var
	Dir:	TArrayOfString;
begin
	setArrayLength(Dir, 1)
	Dir[0] := ExpandConstant('{app}') + '\bin';
	Result := Dir;
end;
#include "modpath.iss"
