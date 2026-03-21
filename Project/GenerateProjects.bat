cd "premake5"
copy "premake5.lua" "../"
cd "../"
call "premake5/premake5.exe" vs2022
del "premake5.lua"

PAUSE