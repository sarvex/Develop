;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include InstallOptions.nsh
  !include "nsProcess.nsh"

;--------------------------------

; The name of the installer
Name "Aircel"
BrandingText "Aircel Dialer"
; Olive Aircel 1.0.0
; The file to write
OutFile "setup.exe"

; The default installation directory
InstallDir "$PROGRAMFILES32\Aircel\Olive\AircelDialer\"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Show install details
ShowInstDetails show

;--------------------------------
  
;Custom Icons
!define MUI_ICON "logo.ico"
!define MUI_UNICON "logo.ico"

; MUI Settings / Wizard
!define MUI_WELCOMEFINISHPAGE_BITMAP "aircel_splash.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "aircel_splash.bmp"
 
;Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE license.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES  
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Launch Aircel"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
!insertmacro MUI_LANGUAGE "English"
;--------------------------------

Function .onInit

	BringToFront
	; Check if already running
	; If so don't open another but bring to front
	
	System::Call "kernel32::CreateMutexA(i 0, i 0, t '$(^Name)') i .r0 ?e"
	  Pop $0
	  StrCmp $0 0 launch
	   StrLen $0 "$(^Name)"
	   IntOp $0 $0 + 1
	  loop:
	    FindWindow $1 '#32770' '' 0 $1
	    IntCmp $1 0 +5
	    System::Call "user32::GetWindowText(i r1, t .r2, i r0) i."
	    StrCmp $2 "$(^Name)" 0 loop
	    System::Call "user32::ShowWindow(i r1,i 9) i."         ; If minimized then restore
	    System::Call "user32::SetForegroundWindow(i r1) i."    ; Bring to front
		Abort
	  launch:
	
	# the plugins dir is automatically deleted when the installer exits
	InitPluginsDir
	File /oname=$PLUGINSDIR\splash.bmp "splash.bmp"
	#optional
	#File /oname=$PLUGINSDIR\splash.wav "C:\myprog\sound.wav"

	advsplash::show 1000 600 400 0x04025C $PLUGINSDIR\splash
	
	ReadRegStr $R0 HKLM \
  "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel" \
  "UninstallString"
  StrCmp $R0 "" done
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "Aircel is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
 
;Run the uninstaller
uninst:
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
  IfErrors no_remove_uninstaller done
    ;You can either use Delete /REBOOTOK in the uninstaller or add some code
    ;here to remove the uninstaller. Use a registry key to check
    ;whether the user has chosen to uninstall. If you are using an uninstaller
    ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:
 
done:

	Pop $0 ; $0 has '1' if the user closed the splash screen early,
			; '0' if everything closed normally, and '-1' if some error occurred.

	Pop $0 ; $0 has '1' if the user closed the splash screen early,
			; '0' if everything closed normally, and '-1' if some error occurred.
	
FunctionEnd

Function LaunchLink
  ExecShell "" "$SMPROGRAMS\Aircel\Olive\AircelDialer\Aircel.lnk"
FunctionEnd


Section "Desktop Shortcut" SectionX
	SetOutPath $INSTDIR
    SetShellVarContext current    
    CreateShortCut "$DESKTOP\Aircel.lnk" "$INSTDIR\Aircel.exe" "" "$INSTDIR\Aircel.exe" 0
SectionEnd

Section "Quick Launch Shortcut" SectionY
    SetOutPath $INSTDIR
	SetShellVarContext current   
    CreateShortCut "$QUICKLAUNCH\Aircel.lnk" "$INSTDIR\Aircel.exe" "" "$INSTDIR\Aircel.exe" 0
SectionEnd

Section "Aircel"
  !include "FileFunc.nsh"
  
  ;Add registry keys
  WriteRegStr HKLM "SOFTWARE\Aircel" "InstallationDirectory" $INSTDIR
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel" "DisplayName" "Aircel"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel" "NoModify" 1
  WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel" "NoRepair" 1
  
  SetOutPath $INSTDIR
  
  CreateDirectory "$SMPROGRAMS\Aircel\Olive\AircelDialer"
  CreateShortCut "$SMPROGRAMS\Aircel\Olive\AircelDialer\Aircel.lnk" "$INSTDIR\Aircel.exe" "" "$INSTDIR\Aircel.exe" 0
  CreateShortCut "$SMPROGRAMS\Aircel\Olive\AircelDialer\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\Aircel.exe" 0
  
  File Aircel.exe
  File style
  File dll\libgcc_s_dw2-1.dll
  File dll\mingwm10.dll
  File dll\QtCore4.dll         
  File dll\QtGui4.dll
  File dll\QtMultimedia4.dll
  File dll\QtNetwork4.dll     
  File dll\QtSql4.dll
   
  WriteUninstaller "uninstall.exe"
  
  SetOutPath $INSTDIR\drivers
  File drivers\eject.exe
  
  SetOutPath $INSTDIR\drivers\x86
  File drivers\x86\Oliveusbmdm.inf
  File drivers\x86\Oliveusbser.inf
  File drivers\x86\Oliveusbser.cat
  File drivers\x86\dpinst.xml
  File drivers\x86\dpinst.exe
  
  SetOutPath $INSTDIR\drivers\x86\x86
  File drivers\x86\x86\Oliveusbser.sys
  
  SetOutPath $INSTDIR\drivers\x64  
  File drivers\x64\Oliveusbmdm.inf
  File drivers\x64\Oliveusbser.inf
  File drivers\x64\Oliveusbser.cat
  File drivers\x64\dpinst.xml
  File drivers\x64\dpinst.exe  
  
  SetOutPath $INSTDIR\drivers\x64\amd64 
  File drivers\x64\amd64\Oliveusbser.sys

  SetOutPath $INSTDIR\manual
  File manual\balance.html
  File manual\call.html
  File manual\copyright.html
  File manual\entertainment.html
  File manual\faq.html
  File manual\history.html
  File manual\information.html
  File manual\messaging.html
  File manual\phonebook.html
  File manual\security.html
  File manual\service.html
  File manual\started.html
  File manual\upgrade.html
  File manual\usage.html 
  
  SetOutPath $INSTDIR\sqldrivers
  File sqldrivers\qsqlite4.dll
SectionEnd

Section "Drivers"
  !include "x64.nsh"
  ${If} ${RunningX64}
    ExecWait $INSTDIR\drivers\x64\dpinst.exe
  ${Else}
    ExecWait $INSTDIR\drivers\x86\dpinst.exe
  ${EndIf} 
SectionEnd

;UninstPage uninstConfirm
;UninstPage instfiles
Section "Uninstall"
  
  ; Close application before uninstallation 
  loop:
	${nsProcess::FindProcess} "Aircel.exe" $R0
	StrCmp $R0 0 0 +2
	MessageBox MB_OK|MB_ICONEXCLAMATION 'Close "Aircel" before continue' IDOK loop
  end:
  
  ; Remove registry keys
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Aircel"
  DeleteRegKey HKLM "SOFTWARE\Aircel" 
  
  ; Remove files and uninstaller
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\Aircel.exe
  Delete $INSTDIR\style
  Delete $INSTDIR\olive
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\mingwm10.dll 
  Delete $INSTDIR\QtCore4.dll        
  Delete $INSTDIR\QtGui4.dll  
  Delete $INSTDIR\QtNetwork4.dll   
  Delete $INSTDIR\QtSql4.dll  
  Delete $INSTDIR\QtMultimedia4.dll
  Delete $INSTDIR\sqldrivers\qsqlite4.dll
  
  ;delete html files
  Delete $INSTDIR\manual\balance.html
  Delete $INSTDIR\manual\call.html
  Delete $INSTDIR\manual\copyright.html
  Delete $INSTDIR\manual\entertainment.html
  Delete $INSTDIR\manual\faq.html
  Delete $INSTDIR\manual\history.html
  Delete $INSTDIR\manual\information.html
  Delete $INSTDIR\manual\messaging.html
  Delete $INSTDIR\manual\phonebook.html
  Delete $INSTDIR\manual\security.html
  Delete $INSTDIR\manual\service.html
  Delete $INSTDIR\manual\started.html
  Delete $INSTDIR\manual\upgrade.html
  Delete $INSTDIR\manual\usage.html
  
  !include "x64.nsh"
  ${If} ${RunningX64}
	ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\oliveusbmdm.inf" /S /D'
	ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\oliveusbser.inf" /S /D'
  ${Else}
    ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\oliveusbmdm.inf" /S /D'
	ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\oliveusbser.inf" /S /D'
  ${EndIf}
  
  ;Driver Files
  Delete $INSTDIR\drivers\x86\Oliveusbmdm.inf
  Delete $INSTDIR\drivers\x86\Oliveusbser.inf 
  Delete $INSTDIR\drivers\x86\Oliveusbser.cat 
  Delete $INSTDIR\drivers\x86\dpinst.exe 
  Delete $INSTDIR\drivers\x86\dpinst.xml
  Delete $INSTDIR\drivers\x86\x86\Oliveusbser.sys  
  
  Delete $INSTDIR\drivers\x64\Oliveusbmdm.inf
  Delete $INSTDIR\drivers\x64\Oliveusbser.inf 
  Delete $INSTDIR\drivers\x64\Oliveusbser.cat 
  Delete $INSTDIR\drivers\x64\dpinst.xml
  Delete $INSTDIR\drivers\x64\dpinst.exe
  Delete $INSTDIR\drivers\x64\amd64\Oliveusbser.sys
  
  Delete $INSTDIR\drivers\eject.exe
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Aircel\Olive\AircelDialer\Aircel.lnk"
  Delete "$SMPROGRAMS\Aircel\Olive\AircelDialer\Uninstall.lnk"
  Delete "$DESKTOP\Aircel.lnk"
  Delete "$QUICKLAUNCH\Aircel.lnk"
  
  ; Remove directories used
  RMDir "$SMPROGRAMS\Aircel\Olive\AircelDialer"
  RMDir "$SMPROGRAMS\Aircel\Olive"
  RMDir "$SMPROGRAMS\Aircel"
  
  RMDir "$INSTDIR\manual"
  
  RMDir "$INSTDIR\drivers\x86\x86"
  RMDir "$INSTDIR\drivers\x64\amd64"
  RMDir "$INSTDIR\drivers\x86"
  RMDir "$INSTDIR\drivers\x64"
  RMDir "$INSTDIR\drivers"
  RMDir "$INSTDIR\sqldrivers"
    
  RMDir "$INSTDIR"
  
  RMDir "$PROGRAMFILES32\Aircel\Olive\AircelDialer"
  RMDir "$PROGRAMFILES32\Aircel\Olive\"
  RMDir "$PROGRAMFILES32\Aircel\"
  
SectionEnd
