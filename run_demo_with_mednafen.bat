@ECHO Off
SET EMULATOR_DIR=..\..\Emulators

if exist sl_coff.iso (
cd "D:\Google Drive\GameDev\Saturn\JO Engine\Samples\demo - 3D map"
"%EMULATOR_DIR%\mednafen\mednafen.exe" sl_coff.cue -sound.volume "150"

) else (
echo Please compile first !
pause
)