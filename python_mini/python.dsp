# Microsoft Developer Studio Project File - Name="python" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=python - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "python.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "python.mak" CFG="python - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "python - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "python - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "python - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "Include" /I "." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "python - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "Include" /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "python - Win32 Release"
# Name "python - Win32 Debug"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\modules\_sre.c
# End Source File
# Begin Source File

SOURCE=.\modules\config.c
# End Source File
# Begin Source File

SOURCE=.\modules\errnomodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\gcmodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\getbuildinfo.c
# End Source File
# Begin Source File

SOURCE=.\modules\getpath.c
# End Source File
# Begin Source File

SOURCE=.\modules\main.c
# End Source File
# Begin Source File

SOURCE=.\modules\newmodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\python.c
# End Source File
# Begin Source File

SOURCE=.\modules\signalmodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\sre.h
# End Source File
# Begin Source File

SOURCE=.\modules\sre_constants.h
# End Source File
# Begin Source File

SOURCE=.\modules\symtablemodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\threadmodule.c
# End Source File
# Begin Source File

SOURCE=.\modules\xxsubtype.c
# End Source File
# End Group
# Begin Group "objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\objects\abstract.c
# End Source File
# Begin Source File

SOURCE=.\objects\bufferobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\cellobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\classobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\cobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\complexobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\descrobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\dictobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\fileobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\floatobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\frameobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\funcobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\intobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\iterobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\listobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\longobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\methodobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\moduleobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\object.c
# End Source File
# Begin Source File

SOURCE=.\objects\rangeobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\sliceobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\stringobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\structseq.c
# End Source File
# Begin Source File

SOURCE=.\objects\tupleobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\typeobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\unicodectype.c
# End Source File
# Begin Source File

SOURCE=.\objects\unicodeobject.c
# End Source File
# Begin Source File

SOURCE=.\objects\unicodetype_db.h
# End Source File
# Begin Source File

SOURCE=.\objects\weakrefobject.c
# End Source File
# End Group
# Begin Group "parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\parser\acceler.c
# End Source File
# Begin Source File

SOURCE=.\parser\bitset.c
# End Source File
# Begin Source File

SOURCE=.\parser\grammar1.c
# End Source File
# Begin Source File

SOURCE=.\parser\listnode.c
# End Source File
# Begin Source File

SOURCE=.\parser\metagrammar.c
# End Source File
# Begin Source File

SOURCE=.\parser\myreadline.c
# End Source File
# Begin Source File

SOURCE=.\parser\node.c
# End Source File
# Begin Source File

SOURCE=.\parser\parser.c
# End Source File
# Begin Source File

SOURCE=.\parser\parser.h
# End Source File
# Begin Source File

SOURCE=.\parser\parsetok.c
# End Source File
# Begin Source File

SOURCE=.\parser\pgen.h
# End Source File
# Begin Source File

SOURCE=.\parser\tokenizer.c
# End Source File
# Begin Source File

SOURCE=.\parser\tokenizer.h
# End Source File
# End Group
# Begin Group "python"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\python\bltinmodule.c
# End Source File
# Begin Source File

SOURCE=.\python\ceval.c
# End Source File
# Begin Source File

SOURCE=.\python\codecs.c
# End Source File
# Begin Source File

SOURCE=.\python\compile.c
# End Source File
# Begin Source File

SOURCE=.\python\dynload_win.c
# End Source File
# Begin Source File

SOURCE=.\python\errors.c
# End Source File
# Begin Source File

SOURCE=.\python\exceptions.c
# End Source File
# Begin Source File

SOURCE=.\python\frozen.c
# End Source File
# Begin Source File

SOURCE=.\python\future.c
# End Source File
# Begin Source File

SOURCE=.\python\getargs.c
# End Source File
# Begin Source File

SOURCE=.\python\getcompiler.c
# End Source File
# Begin Source File

SOURCE=.\python\getcopyright.c
# End Source File
# Begin Source File

SOURCE=.\python\getmtime.c
# End Source File
# Begin Source File

SOURCE=.\python\getopt.c
# End Source File
# Begin Source File

SOURCE=.\python\getplatform.c
# End Source File
# Begin Source File

SOURCE=.\python\getversion.c
# End Source File
# Begin Source File

SOURCE=.\python\graminit.c
# End Source File
# Begin Source File

SOURCE=.\python\import.c
# End Source File
# Begin Source File

SOURCE=.\python\importdl.c
# End Source File
# Begin Source File

SOURCE=.\python\importdl.h
# End Source File
# Begin Source File

SOURCE=.\python\marshal.c
# End Source File
# Begin Source File

SOURCE=.\python\modsupport.c
# End Source File
# Begin Source File

SOURCE=.\python\mysnprintf.c
# End Source File
# Begin Source File

SOURCE=.\python\mystrtoul.c
# End Source File
# Begin Source File

SOURCE=.\python\pyfpe.c
# End Source File
# Begin Source File

SOURCE=.\python\pystate.c
# End Source File
# Begin Source File

SOURCE=.\python\pythonrun.c
# End Source File
# Begin Source File

SOURCE=.\python\structmember.c
# End Source File
# Begin Source File

SOURCE=.\python\symtable.c
# End Source File
# Begin Source File

SOURCE=.\python\sysmodule.c
# End Source File
# Begin Source File

SOURCE=.\python\thread.c
# End Source File
# Begin Source File

SOURCE=.\python\traceback.c
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\abstract.h
# End Source File
# Begin Source File

SOURCE=.\include\bitset.h
# End Source File
# Begin Source File

SOURCE=.\include\bufferobject.h
# End Source File
# Begin Source File

SOURCE=.\include\cellobject.h
# End Source File
# Begin Source File

SOURCE=.\include\ceval.h
# End Source File
# Begin Source File

SOURCE=.\include\classobject.h
# End Source File
# Begin Source File

SOURCE=.\include\cobject.h
# End Source File
# Begin Source File

SOURCE=.\include\codecs.h
# End Source File
# Begin Source File

SOURCE=.\include\compile.h
# End Source File
# Begin Source File

SOURCE=.\include\complexobject.h
# End Source File
# Begin Source File

SOURCE=.\include\descrobject.h
# End Source File
# Begin Source File

SOURCE=.\include\dictobject.h
# End Source File
# Begin Source File

SOURCE=.\include\errcode.h
# End Source File
# Begin Source File

SOURCE=.\include\eval.h
# End Source File
# Begin Source File

SOURCE=.\include\fileobject.h
# End Source File
# Begin Source File

SOURCE=.\include\floatobject.h
# End Source File
# Begin Source File

SOURCE=.\include\frameobject.h
# End Source File
# Begin Source File

SOURCE=.\include\funcobject.h
# End Source File
# Begin Source File

SOURCE=.\include\graminit.h
# End Source File
# Begin Source File

SOURCE=.\include\grammar.h
# End Source File
# Begin Source File

SOURCE=.\include\import.h
# End Source File
# Begin Source File

SOURCE=.\include\intobject.h
# End Source File
# Begin Source File

SOURCE=.\include\intrcheck.h
# End Source File
# Begin Source File

SOURCE=.\include\iterobject.h
# End Source File
# Begin Source File

SOURCE=.\include\listobject.h
# End Source File
# Begin Source File

SOURCE=.\include\longintrepr.h
# End Source File
# Begin Source File

SOURCE=.\include\longobject.h
# End Source File
# Begin Source File

SOURCE=.\include\marshal.h
# End Source File
# Begin Source File

SOURCE=.\include\metagrammar.h
# End Source File
# Begin Source File

SOURCE=.\include\methodobject.h
# End Source File
# Begin Source File

SOURCE=.\include\modsupport.h
# End Source File
# Begin Source File

SOURCE=.\include\moduleobject.h
# End Source File
# Begin Source File

SOURCE=.\include\node.h
# End Source File
# Begin Source File

SOURCE=.\include\object.h
# End Source File
# Begin Source File

SOURCE=.\include\objimpl.h
# End Source File
# Begin Source File

SOURCE=.\include\opcode.h
# End Source File
# Begin Source File

SOURCE=.\include\osdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\parsetok.h
# End Source File
# Begin Source File

SOURCE=.\include\patchlevel.h
# End Source File
# Begin Source File

SOURCE=.\include\pgenheaders.h
# End Source File
# Begin Source File

SOURCE=.\include\pydebug.h
# End Source File
# Begin Source File

SOURCE=.\include\pyerrors.h
# End Source File
# Begin Source File

SOURCE=.\include\pyfpe.h
# End Source File
# Begin Source File

SOURCE=.\include\pygetopt.h
# End Source File
# Begin Source File

SOURCE=.\include\pymem.h
# End Source File
# Begin Source File

SOURCE=.\include\pyport.h
# End Source File
# Begin Source File

SOURCE=.\include\pystate.h
# End Source File
# Begin Source File

SOURCE=.\include\python.h
# End Source File
# Begin Source File

SOURCE=.\include\pythonrun.h
# End Source File
# Begin Source File

SOURCE=.\include\pythread.h
# End Source File
# Begin Source File

SOURCE=.\include\rangeobject.h
# End Source File
# Begin Source File

SOURCE=.\include\sliceobject.h
# End Source File
# Begin Source File

SOURCE=.\include\stringobject.h
# End Source File
# Begin Source File

SOURCE=.\include\structmember.h
# End Source File
# Begin Source File

SOURCE=.\include\structseq.h
# End Source File
# Begin Source File

SOURCE=.\include\symtable.h
# End Source File
# Begin Source File

SOURCE=.\include\sysmodule.h
# End Source File
# Begin Source File

SOURCE=.\include\token.h
# End Source File
# Begin Source File

SOURCE=.\include\traceback.h
# End Source File
# Begin Source File

SOURCE=.\include\tupleobject.h
# End Source File
# Begin Source File

SOURCE=.\include\ucnhash.h
# End Source File
# Begin Source File

SOURCE=.\include\unicodeobject.h
# End Source File
# Begin Source File

SOURCE=.\include\weakrefobject.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\breakpoint.txt
# End Source File
# Begin Source File

SOURCE=.\DebugPythonStacks.txt
# End Source File
# Begin Source File

SOURCE=.\macro.txt
# End Source File
# Begin Source File

SOURCE=.\pyconfig.h
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
