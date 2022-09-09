echo off
setlocal 
      path=%CD%;%path% 
      app_version -n sca1 -i 1 -d "Scavenger (application scavenger_01)"
      app_setup vsx scavenger_01 sca1
endlocal
REM set /p response= Ran setup for scavenger_01. Press enter to continue. 