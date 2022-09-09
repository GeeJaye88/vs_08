echo off
setlocal 
      path=%CD%;%path% 
      app_version -n as2 -i 1 -d "AppSetup (application app_setup_02)"
      app_setup vsx app_setup_02 as2
endlocal
REM set /p response= Ran setup for app_setup_02. Press enter to continue. 