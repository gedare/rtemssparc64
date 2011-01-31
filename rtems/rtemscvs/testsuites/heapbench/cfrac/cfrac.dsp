# Microsoft Developer Studio Project File - Name="cfrac" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=cfrac - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cfrac.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cfrac.mak" CFG="cfrac - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cfrac - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cfrac - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "cfrac - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MTd /W3 /Zi /O2 /Ob2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /force:multiple
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cfrac - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /force:multiple
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "cfrac - Win32 Release"
# Name "cfrac - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cfrac\atop.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\cfrac.c
# End Source File
# Begin Source File

SOURCE=..\..\customheap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\dlmalloc.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\errorp.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\getopt.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\itop.c
# End Source File
# Begin Source File

SOURCE=..\..\kmalloc.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\ltop.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pabs.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\padd.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pcfrac.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pcmp.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pconst.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pdivmod.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pfloat.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pgcd.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\phalf.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\picmp.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pidiv.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pimod.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pio.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pmul.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pneg.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\podd.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\pops.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\ppowmod.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\primes.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\psqrt.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\psub.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\ptoa.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\ptob.c
# End Source File
# Begin Source File

SOURCE=..\cfrac\ptou.c
# End Source File
# Begin Source File

SOURCE=..\..\stats.cpp
# End Source File
# Begin Source File

SOURCE=..\cfrac\utop.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\cfrac\asm16bit.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\getopt.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\pcvt.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\pdefs.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\pfactor.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\precision.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\primes.h
# End Source File
# Begin Source File

SOURCE=..\cfrac\seive.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
