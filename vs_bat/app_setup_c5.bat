echo off
setlocal 
      path=%CD%;%path%
      app_version -n c3 -i 1 -d "ImgCam (application camera_05 configured from camera_03)"
      app_setup vs camera_03 c3 -c camera_05 -i c5
endlocal 
REM set /p response= Ran setup for Camera_03. Press enter to continue. 