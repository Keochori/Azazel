cd "vendor/premake"
copy "premake5.lua" "../../"
cd "../../"
call "vendor/premake/premake5.exe" vs2022
del "premake5.lua"

PAUSE