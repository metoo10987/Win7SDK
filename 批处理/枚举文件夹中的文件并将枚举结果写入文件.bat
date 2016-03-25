@echo off  
@echo ...................begin  
set APP_BIN_DIR="D:\third_party"  
for /R %APP_BIN_DIR% %%i in (*.*) do echo %%i >>d:files.txt
@echo ...................end