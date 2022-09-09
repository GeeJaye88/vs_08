echo off
setlocal
      path=%CD%;%path% 
      app_version -n s3 -i 1 -d "Surface 03 (application surface_03)"
      app_setup vs surface_03 s3 -a 3ds
endlocal 