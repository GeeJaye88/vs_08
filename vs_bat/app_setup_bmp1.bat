echo off
setlocal 
      path=%CD%;%path%
      app_version -n bmp1 -i 1 -d "AppVersion (application bmp_utility_01)"
      app_setup vsx bmp_utility_01 bmp1
endlocal
REM set /p response= Ran setup for Camera_03. Press enter to continue. 