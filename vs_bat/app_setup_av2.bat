echo off
setlocal 
      path=%CD%;%path%
      app_version -n as2 -i 1 -d "AppVersion (application app_version_02)"
      app_setup vsx app_version_02 av2
endlocal
REM set /p response= Ran setup for Camera_03. Press enter to continue. 