!include "MUI2.nsh"

;General

	;Name and file
	Name "GemMatrix DEMO"
	OutFile "Setup.exe"

	;Default installation folder
	InstallDir "$LOCALAPPDATA\GemMatrix"

	;Get installation folder from registry if available
	InstallDirRegKey HKCU "Software\GemMatrix" ""

	;Request application privileges for Windows Vista
	RequestExecutionLevel user
	
	;	set stronger compression
	SetCompressor /SOLID lzma
	

;--------------------------------
;Interface Settings
	!define MUI_ABORTWARNING
	
;--------------------------------
;Pages
	!insertmacro MUI_PAGE_WELCOME  
	!insertmacro MUI_PAGE_LICENSE "readme.txt"
	!insertmacro MUI_PAGE_INSTFILES

		;	Autostart game after install
		;!define MUI_FINISHPAGE_NOAUTOCLOSE
		!define MUI_FINISHPAGE_RUN
		!define MUI_FINISHPAGE_RUN_NOTCHECKED
		!define MUI_FINISHPAGE_RUN_TEXT "Play game now!"
		!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"	
		;	Read Me
		!define MUI_FINISHPAGE_SHOWREADME_CHECKED
		!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\readme.txt
	!insertmacro MUI_PAGE_FINISH

	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Install"

	;	copy binaries
	SetOutPath "$INSTDIR\bin\"
	File /r "..\bin\GemMatrix.exe"
	File /r "..\bin\*.dll"

	;	copy resources
	SetOutPath "$INSTDIR\Resources\"
	File /r "..\Resources\*.png"
	File /r "..\Resources\*.xml"
	File /r "..\Resources\*.plist"
	;File /r "..\Resources\*.json"
	;File /r "..\Resources\*.vsh"
	;File /r "..\Resources\*.fsh"

	;	copy EXE and DLL's
	SetOutPath "$INSTDIR"
	File "game.ico"
	File "readme.txt"

	;Store installation folder
	WriteRegStr HKCU "Software\GemMatrix" "" $INSTDIR

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	
	;	write start menu
	SetOutPath "$INSTDIR\bin\"	
    CreateDirectory "$SMPROGRAMS\GemMatrix"
	CreateShortCut "$SMPROGRAMS\GemMatrix\Play.lnk" "$INSTDIR\bin\GemMatrix.exe" "" "$INSTDIR\game.ico" 0
	CreateShortCut "$SMPROGRAMS\GemMatrix\ReadMe.lnk" "$INSTDIR\readme.txt"
    CreateShortCut "$SMPROGRAMS\GemMatrix\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
	;	write desktop
	CreateShortCut "$DESKTOP\GemMatrix.lnk" "$INSTDIR\bin\GemMatrix.exe" "" "$INSTDIR\game.ico" 0
	
	SetOutPath "$INSTDIR"
		
SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"
	;	remove icon
	Delete "$INSTDIR\game.ico"

	;	Remove resources
	Delete  "$INSTDIR\Resources\*.*"
	RMDir /r "$INSTDIR\Resources\"
	
	;	Remove binaries
	Delete  "$INSTDIR\bin\*.*"
	RMDir /r "$INSTDIR\bin\"
	
	;	remove uninstaller
	Delete "$INSTDIR\Uninstall.exe"	
	Delete "$INSTDIR\Readme.txt"
	
	RMDir "$INSTDIR"

	DeleteRegKey /ifempty HKCU "Software\GemMatrix"
	
	;	remove icons
	Delete "$SMPROGRAMS\GemMatrix\Uninstall.lnk"
	Delete "$SMPROGRAMS\GemMatrix\Play.lnk"
	Delete "$SMPROGRAMS\GemMatrix\ReadMe.lnk"
	RMDir /r "$SMPROGRAMS\GemMatrix"
	
	Delete "$DESKTOP\GemMatrix.lnk"
	

SectionEnd

;	code to run game after install
Function LaunchLink
  ExecShell "" "$DESKTOP\GemMatrix.lnk"
FunctionEnd
