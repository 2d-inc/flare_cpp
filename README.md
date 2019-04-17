# Flare-Cpp
Flare C++ runtime.

# Libraries
There are a few libraries in this repository:
## Flare Math
Includes linear algebra for 2d vectors, matrices, bounding boxes, etc.

## Flare
Manages a Flare file's contents, loads them, etc.

## Flare Skia
Rendering implementation with Skia. If you are trying to compile the example app, make sure you read about how to [compile dependencies](#compile-dependencies).

# Build System
Using [GENie](https://github.com/bkaradzic/GENie). 

## OSX
Instructions should be similar on other platforms.

Set it up as follows (use your own ```~/Projects``` base dir).
```
cd ~/Projects
git clone git@github.com:bkaradzic/GENie.git
cd genie
make
```

Add to PATH, for example in ```~/.bash_profile```
```
export PATH=$PATH:~/Projects/genie/bin/darwin
```

# Building
If you are using VSCode and have opened the flare.code-workspace, you can simply hit F5 to build and debug. Alternatively, several tasks are available in each project to build and generate compile commands.

If you wish to use the build system, read on.
```
cd build
genie --file=example.lua ninja
make
make config=release
```

# CQuery
Install cquery in order to get code completion/jump to definition/hints in VSCode.

```
brew install --HEAD cquery
```

## compile_commands.json
### Generate All
You can invoke ```generate_compile_commands.sh``` in this folder to generate all the necessary compile commands for the various projects.
```
./generate_compile_commands.sh
```

### Per Project
If you want to re-generate them per project, you can use the command pallette in VSCode.

```
command+p
Tasks: Run Task
generate compile commands
```

### Manually
To do it manually:
```
genie ninja
ninja -C debug -t compdb cxx > ../compile_commands.json
```

# Formatting
## Clang
If you don't have clang installed:
```
brew install llvm
```
## Clang-Format in VSCode
Install Clang-Format in VSCode and make sure your settings are configured correctly. For example:

**settings.json**
```json
    "[cpp]": {
      "editor.defaultFormatter": "xaver.clang-format"
    },
    "clang-format.executable": "/usr/local/opt/llvm/bin/clang-format"
```

## Debugging in VSCode
Install the vscode-cpptools extension (simply called C/C++ in the extension browser). Press F5 and everything should already be configured for you (at least on OSX, PRs welcomed for other platforms).

We suggest disabling the cpptools intellisense engine in favor of the cquery one mentioned above. You can do this in your settings.json:
```
  "C_Cpp.autocomplete": "Disabled",
  "C_Cpp.formatting": "Disabled",
  "C_Cpp.errorSquiggles": "Disabled",
  "C_Cpp.intelliSenseEngine": "Disabled",
```

## Compile Dependencies
Take a look at the [dependencies/make_dependencies.sh](dependencies/make_dependencies.sh) file.