@echo off
set DO_TEST=Y
cl /W3 /D_CRT_SECURE_NO_WARNINGS csv2cell.c cJSON.c
if %DO_TEST% NEQ Y goto end
echo.
csv2cell test.csv test.wdt
diff -q test.wdt test.base
if ERRORLEVEL 2 (echo diff failed! & goto end)
if ERRORLEVEL 1 (echo Test failed! & goto end) 
if ERRORLEVEL 0 (echo Test passed. & goto end) 

:end
