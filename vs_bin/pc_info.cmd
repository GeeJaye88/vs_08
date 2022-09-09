@echo off
echo Using: "csproduct get name"
wmic csproduct get name

echo Using: "wmic bios get serialnumber"
wmic bios get serialnumber

echo Using: wmic os get caption
wmic os get caption

echo Using: "systeminfo | findstr /B /C: "OS Name" /C:"OS Version"
systeminfo | findstr /B /C:"OS Name" /C:"OS Version"

echo .
echo For short pop up info use: "slmgr.vbs /dli"
echo For long pop up info use: "slmgr.vbs /dlv"
echo For "About" info use: "winver"
echo For "System" info use: "msinfo32"
