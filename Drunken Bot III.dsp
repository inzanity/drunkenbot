# Microsoft Developer Studio Project File - Name="Drunken Bot III" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Drunken Bot III - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Drunken Bot III.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Drunken Bot III.mak" CFG="Drunken Bot III - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Drunken Bot III - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Drunken Bot III - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Drunken Bot III - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Drunken Bot III - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Drunken Bot III - Win32 Release"
# Name "Drunken Bot III - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\bot.cpp
# End Source File
# Begin Source File

SOURCE=.\src\botai.cpp
# End Source File
# Begin Source File

SOURCE=.\src\botinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ddgraphicsengine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gameobj.cpp
# End Source File
# Begin Source File

SOURCE=.\src\graphicsengine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sdlgraphicsengine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\textengine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tilemap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\weaponinfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\bot.h
# End Source File
# Begin Source File

SOURCE=.\inc\botai.h
# End Source File
# Begin Source File

SOURCE=.\inc\botinfo.h
# End Source File
# Begin Source File

SOURCE=.\inc\bullet.h
# End Source File
# Begin Source File

SOURCE=.\inc\ddgraphicsengine.h
# End Source File
# Begin Source File

SOURCE=.\inc\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\inc\engine.h
# End Source File
# Begin Source File

SOURCE=.\inc\gameobj.h
# End Source File
# Begin Source File

SOURCE=.\inc\graphicsengine.h
# End Source File
# Begin Source File

SOURCE=.\inc\sdlgraphicsengine.h
# End Source File
# Begin Source File

SOURCE=.\inc\textengine.h
# End Source File
# Begin Source File

SOURCE=.\inc\tilemap.h
# End Source File
# Begin Source File

SOURCE=.\inc\weapon.h
# End Source File
# Begin Source File

SOURCE=.\inc\weaponinfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\map.txt
# End Source File
# Begin Source File

SOURCE=.\res\weapons.txt
# End Source File
# End Group
# End Target
# End Project
