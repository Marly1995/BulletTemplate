TITLE BulletMagnetMakeFile

ECHO build started!

ECHO off

MD build
CD build

conan install --file ../Conanfile.txt -s compiler="Visual Studio" -s compiler.runtime="MD" -s compiler.version="14"
cmake .. -G "Visual Studio 14 Win64"
cmake --build . --config Debug

pause