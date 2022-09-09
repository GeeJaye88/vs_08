echo off
setlocal 
      path=%CD%;%path% 
      app_version -n r2 -i 1 -d "PanoReview (application review_02)"
      app_setup vs review_02 r2
endlocal 
REM set /p response= Ran setup for Review_02. Press enter to continue. 