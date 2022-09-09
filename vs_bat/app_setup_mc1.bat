echo off
setlocal 
      path=%CD%;%path% 
      app_version -n mc1 -i 1 -d "MountCalc (application mount_calc_01)"
      app_setup vsx mount_calc_01 mc1
endlocal
REM set /p response= Ran setup for app_setup_02. Press enter to continue. 