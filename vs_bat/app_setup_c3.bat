echo off
setlocal 
      path=%CD%;%path% 
      app_version -n c3 -i 1 -d "PanoCam (application camera_03)"
      app_setup vs camera_03 c3
endlocal 
REM set /p response= Ran setup for Camera_03. Press enter to continue. 