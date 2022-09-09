echo off
setlocal 
      path=%CD%;%path% 
      app_version -n c1 -i 1 -d "Cube Test (application cube_01)"
      app_setup vs cube_01 c1
endlocal 
REM set /p response= Ran setup for cube_01. Press enter to continue. 