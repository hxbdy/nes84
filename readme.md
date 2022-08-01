# SDL setup
[Simple DirectMedia Layer - SDL version 2.0.22 (stable)](http://libsdl.org/download-2.0.php)  
1. Development Libraries から DL して nes84 フォルダ直下に置く。  
2. ビルド生成物(.exe)と同じ階層にSDL2.dll を置いておく。
# How 2 build
## Compiler
MSVC (Visual Studio Comm 2022)
## VSCode
Ctrl + Shift + P から tasks.json を生成して args に以下を設定する

```
"args": [
                "${workspaceFolder}/main.c",
                "/Fe:",
                "${workspaceFolder}/build/main.exe",
                "/Fo:",
                "${workspaceFolder}/build/main.obj",
				"/I",
				"${workspaceFolder}/SDL2-2.0.22/include/",
                "/link",
                "/LIBPATH:${workspaceFolder}/SDL2-2.0.22/lib/x64",
                "SDL2.lib",
                "SDL2main.lib",
                "SDL2test.lib",
                "shell32.lib",
                "/SUBSYSTEM:CONSOLE"
            ],
```
設定後 Ctrl + Shift + B でビルド実行する
## CMD
cmd 上から実行する場合は以下。
(事前にSDLライブラリ、インクルードにパスを通しておく必要がある。)
```
> cl main.c /Fe:"./build/" /Fo:"./build/" SDL2.lib SDL2main.lib SDL2test.lib shell32.lib /link /Subsystem:Console
> ./build/main.exe
```
