echo off
setlocal 
      path=%CD%;%path%
      app_version -n cpp2 -i 1 -d "Cpp (application cpp_02)"
      app_setup vsx cpp_02 cpp2
endlocal 
REM set /p response= Ran setup for Camera_03. Press enter to continue. 