@echo off

IF NOT EXIST w:\build mkdir w:\build
pushd w:\build

set compiler_flags=/nologo /c /Od /Oi /Z7 /Gm- /EHa- /Wall /WX /FC /GS- /Gs9999999 /wd4820 /wd4201 /wd4204
rem /Oi generate intrinsic functions
set linker_flags=/nologo /INCREMENTAL:NO /NODEFAULTLIB /DEBUG /SUBSYSTEM:windows /STACK:0x100000,0x100000

cl %compiler_flags% w:\bundler\src\win32_main.c /Fowin32_bundler.obj
link %linker_flags% win32_bundler.obj kernel32.lib

popd

rem /wd4201 - Nonstandard extension used: nameless struct/union
rem /wd4204 - Nonstandard extension used: non-constant aggregate initializer
rem /wd4100 - Unreferenced formal parameter
rem /wd4189 - Local variable is initialized but not referenced
rem /wd4101 - Unreferenced local variable

rem /wd4711 - Function ___ selected for automatic inline expansion
