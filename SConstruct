#!/usr/bin/env python

import os
import sys

libName = "gd2AllTest"
interfaceExt = ".gdextension"
gdProjectPath = "demo/"
binPath = f"{gdProjectPath}bin/"

env = SConscript("./godot-cpp/SConstruct")

# to update bindings :
# godot.exe --dump-extension-api
# scons platform=<platform> custom_api_file=extension_api.json
# see https://github.com/godotengine/godot-docs/blob/master/tutorials/scripting/gdextension/gdextension_cpp_example.rst

# to generate clang compile_commands
# this is mostly for code completion in my IDE
# see https://scons.org/doc/production/HTML/scons-user/ch27.html
env.Tool('compilation_db')
cdb = env.CompilationDatabase()
Alias('cdb', cdb)

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = [*Glob("src/*.cpp"), *Glob("src/*/*.cpp")]

if env["platform"] == "macos":
	library = env.SharedLibrary(
		f"{binPath}{libName}.{env['platform']}.{env['target']}.framework/{libName}.{env['platform']}.{env['target']}",
		source=sources,
	)
else:
	library = env.SharedLibrary(
		f"{binPath}{libName}{env['suffix']}{env['SHLIBSUFFIX']}",
		source=sources,
	)


Default(library)

# auto generation of .gdextension file
with open(f"src/TEMPLATE{interfaceExt}") as f:
	gdextensionTemplate = f.read().replace("PROJECT_NAME",libName)

# delete old interface file, in case was named diferently
# NOTE : this will delete other interface files in the dir
for file in os.listdir(gdProjectPath):
	if file.endswith(interfaceExt):
		os.remove(os.path.join(gdProjectPath, file))

with open(f"{binPath}{libName}{interfaceExt}", "w") as f:
	f.write(gdextensionTemplate)

# NOTE : you still need to name your register_types functions the right way,
#		notably the library's init function (which MUST be named : {libName}_init )

