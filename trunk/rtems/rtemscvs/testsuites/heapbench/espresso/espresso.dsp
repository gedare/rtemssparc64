# Microsoft Developer Studio Project File - Name="espresso" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=espresso - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "espresso.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "espresso.mak" CFG="espresso - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "espresso - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "espresso - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "espresso - Win32 Release"

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
# ADD CPP /nologo /G6 /MTd /W3 /Zi /Ox /Ot /Og /Ob2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /force:multiple
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "espresso - Win32 Debug"

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
# ADD CPP /nologo /G6 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /force:multiple
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "espresso - Win32 Release"
# Name "espresso - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\espresso\cofactor.c
# End Source File
# Begin Source File

SOURCE=..\espresso\cols.c
# End Source File
# Begin Source File

SOURCE=..\espresso\compl.c
# End Source File
# Begin Source File

SOURCE=..\espresso\contain.c
# End Source File
# Begin Source File

SOURCE=..\espresso\cubestr.c
# End Source File
# Begin Source File

SOURCE=..\..\customheap.cpp
# End Source File
# Begin Source File

SOURCE=..\espresso\cvrin.c
# End Source File
# Begin Source File

SOURCE=..\espresso\cvrm.c
# End Source File
# Begin Source File

SOURCE=..\espresso\cvrmisc.c
# End Source File
# Begin Source File

SOURCE=..\espresso\cvrout.c
# End Source File
# Begin Source File

SOURCE=..\..\dlmalloc.c
# End Source File
# Begin Source File

SOURCE=..\espresso\dominate.c
# End Source File
# Begin Source File

SOURCE=..\espresso\equiv.c
# End Source File
# Begin Source File

SOURCE=..\espresso\espresso.c
# End Source File
# Begin Source File

SOURCE=..\espresso\essen.c
# End Source File
# Begin Source File

SOURCE=..\espresso\exact.c
# End Source File
# Begin Source File

SOURCE=..\espresso\expand.c
# End Source File
# Begin Source File

SOURCE=..\espresso\gasp.c
# End Source File
# Begin Source File

SOURCE=..\espresso\getopt.c
# End Source File
# Begin Source File

SOURCE=..\espresso\gimpel.c
# End Source File
# Begin Source File

SOURCE=..\espresso\globals.c
# End Source File
# Begin Source File

SOURCE=..\espresso\hack.c
# End Source File
# Begin Source File

SOURCE=..\espresso\indep.c
# End Source File
# Begin Source File

SOURCE=..\espresso\irred.c
# End Source File
# Begin Source File

SOURCE=..\..\kmalloc.c
# End Source File
# Begin Source File

SOURCE=..\espresso\main.c
# End Source File
# Begin Source File

SOURCE=..\espresso\map.c
# End Source File
# Begin Source File

SOURCE=..\espresso\matrix.c
# End Source File
# Begin Source File

SOURCE=..\espresso\mincov.c
# End Source File
# Begin Source File

SOURCE=..\espresso\opo.c
# End Source File
# Begin Source File

SOURCE=..\espresso\pair.c
# End Source File
# Begin Source File

SOURCE=..\espresso\part.c
# End Source File
# Begin Source File

SOURCE=..\espresso\primes.c
# End Source File
# Begin Source File

SOURCE=..\espresso\reduce.c
# End Source File
# Begin Source File

SOURCE=..\espresso\rows.c
# End Source File
# Begin Source File

SOURCE=..\espresso\set.c
# End Source File
# Begin Source File

SOURCE=..\espresso\setc.c
# End Source File
# Begin Source File

SOURCE=..\espresso\sharp.c
# End Source File
# Begin Source File

SOURCE=..\espresso\sminterf.c
# End Source File
# Begin Source File

SOURCE=..\espresso\solution.c
# End Source File
# Begin Source File

SOURCE=..\espresso\sparse.c
# End Source File
# Begin Source File

SOURCE=..\..\stats.cpp
# End Source File
# Begin Source File

SOURCE=..\espresso\unate.c
# End Source File
# Begin Source File

SOURCE=..\espresso\utility.c
# End Source File
# Begin Source File

SOURCE=..\espresso\verify.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\espresso\ansi.h
# End Source File
# Begin Source File

SOURCE=..\espresso\copyright.h
# End Source File
# Begin Source File

SOURCE=..\espresso\espresso.h
# End Source File
# Begin Source File

SOURCE=..\espresso\main.h
# End Source File
# Begin Source File

SOURCE=..\espresso\mincov.h
# End Source File
# Begin Source File

SOURCE=..\espresso\mincov_int.h
# End Source File
# Begin Source File

SOURCE=..\espresso\port.h
# End Source File
# Begin Source File

SOURCE=..\espresso\regex.h
# End Source File
# Begin Source File

SOURCE=..\espresso\sparse.h
# End Source File
# Begin Source File

SOURCE=..\espresso\sparse_int.h
# End Source File
# Begin Source File

SOURCE=..\espresso\stdlib.h
# End Source File
# Begin Source File

SOURCE=..\espresso\utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
