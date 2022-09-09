echo off
setlocal 
      path=%CD%;%path%
      app_version -n r2 -i 1 -d "ImgReview (application review_04 configured from review_02)"
      app_setup vs review_02 r2 -c review_04 -i r4
endlocal
REM set /p response= Ran setup for Review_04. Press enter to continue. 