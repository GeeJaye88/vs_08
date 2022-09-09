@echo off
echo.
echo System Scan
echo ===========
echo.
echo Using: sfc /scannow, outputs
echo.
echo    Beginning system scan.  This process will take some time.
echo.
echo    Beginning verification phase of system scan.
echo    Verification 100% complete.
echo.
echo    Windows Resource Protection found corrupt files but was unable to fix some
echo    of them. Details are included in the CBS.Log windir\Logs\CBS\CBS.log. For
echo    example C:\Windows\Logs\CBS\CBS.log. Note that logging is currently not
echo    supported in offline servicing scenarios.
echo.
echo To copy CBS log file to desktop, use:
echo.
echo "findstr /c:"[SR]" %windir%\Logs\CBS\CBS.log >"%userprofile%\Desktop\sfcdetails.txt"
echo.
echo.
echo Deployment Image Servicing and Management (DISM)
echo ================================================
echo.
echo Note: Recommend do a full backup or create a system restore point
echo.
echo Use indows key + X keyboard shortcut to open the Power User menu and select Command Prompt (Admin).
echo.
echo Check health, use: DISM /Online /Cleanup-Image /CheckHealth
echo Scan health, use:DISM /Online /Cleanup-Image /ScanHealth
echo Restore health, use:DISM /Online /Cleanup-Image /RestoreHealth
echo.
echo.
echo Rename folder pinned to Quick Access in Windows 10
echo ==================================================
echo.
echo 1. Navigate to the folder under Quick Access that you want to rename.
echo 2. Hold down the Shift key, right-click on the folder that you want to rename, and then click Copy as path option.
echo 3. Right-click on Start button, and then click Command Prompt (Admin). Click Yes button when you see the User Account Control dialog to open Command Prompt as administrator.
echo 4: In the Command Prompt, type the following command:
echo.       mklink /J <Link> <Target>
echo.
echo.       e.g. mklink /J “D:\My Downloads” “D:\Users\Test\Downloads”
echo.
echo 5. Pin newly added folder shortcut in Quick Access.
echo 6. Unpin shortcut from Quick Access.
echo.
echo.