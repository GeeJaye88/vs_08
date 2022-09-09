@echo off
echo.
echo Rebuild Icon cache in Windows 10
echo ================================
echo.
echo 1. "First, close all open programs."
echo 2. "Next, open Task Manager, look for the process Windows Explorer, right-click on it and select End process."
echo 3. "Next, from the File menu > select Run new task. Type cmd.exe, check the Create this task with administrative privileges box and press Enter."
echo.
echo Then:
echo.

cd /d %userprofile%\AppData\Local\Microsoft\Windows\Explorer
attrib –h iconcache_*.db
del "iconcache_*.db"
start explorer