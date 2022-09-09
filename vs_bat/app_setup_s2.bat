echo off
setlocal
      path=%CD%;%path% 
      app_version -n s2 -i 1 -d "Surface 02 (application surface_02)"
      app_setup vs surface_02 s2 -a 2ds
endlocal 