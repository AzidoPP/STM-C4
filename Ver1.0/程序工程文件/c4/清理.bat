@echo off
echo 清理 Keil MDK 编译中间文件...

REM 删除常见中间文件类型
for /R %%i in (*.o *.d *.crf *.axf *.elf *.lst *.map *.bak *.lnp) do (
    del /f /q "%%i"
)

REM 如果你也想删除 hex 和 bin 文件，取消下面两行的注释
REM for /R %%i in (*.hex *.bin) do (
REM     del /f /q "%%i"
REM )

REM 删除常见中间目录
for %%d in (Objects Listings Output) do (
    if exist "%%d" (
        echo 删除目录 %%d
        rmdir /s /q "%%d"
    )
)

echo 清理完成。
pause
