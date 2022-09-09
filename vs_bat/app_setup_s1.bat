echo off
setlocal
      path=%CD%;%path% 
      app_version -n s1 -i 1 -d "Star 01 (application surface_01)"
      app_setup vs surface_01 s1 -a star
endlocal 