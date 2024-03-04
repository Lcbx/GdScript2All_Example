./compile_extension_windows.sh || { echo ''; echo 'FAIL' ; exit 1; }
cd ./demo # otherwise godot shits the bed
../../Godot4Sharp/godot.windows.editor.x86_64.mono.exe -d main.tscn