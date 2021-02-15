
REM create JLINK.CMD
echo savebin RSL10.bin,0x00100000,0x5F000 >jlink.cmd
echo r >>jlink.cmd
echo q >>jlink.cmd

REM execute it
"%ProgramFiles(x86)%\SEGGER\JLink\Jlink.exe" -device RSL10 -if SWD -speed 4000 -autoconnect 1 -CommanderScript jlink.cmd
