# TF2Items

SourceMod extension for modifying items as Team Fortress 2 gives them to players. This repository preserves [the original TF2Items history](https://github.com/asherkin/TF2Items).

## Builds

CI builds for 32-bit TF2 on Linux and Windows against SourceMod 1.12 and 1.13. Releases include the extension, gamedata, autoload marker, example config, SourcePawn include, and the compiled TF2Items Manager and its source. Extract the archive for your OS and SourceMod version into the server's `tf` directory.

The manager ships in `addons/sourcemod/plugins/optional`. To enable it, create `addons/sourcemod/configs/tf2items.weapons.txt` using the included example, then move `tf2items_manager.smx` into `addons/sourcemod/plugins`.

To build locally, install [AMBuild 2.2 or newer](https://github.com/alliedmodders/ambuild) and use a compiler with x86 support. On Windows, use an x86 MSVC developer prompt. Clone SourceMod and Metamod:Source with their submodules, and check out the `tf2` branch of AlliedModders' HL2SDK. With all three dependency checkouts next to this repo, run the following from the TF2Items checkout.

```sh
mkdir build
cd build
spcomp -i ../pawn -i ../../sourcemod/plugins/include ../pawn/tf2items_manager.sp \
  -o tf2items_manager.smx
python ../configure.py \
  --sm-path ../../sourcemod \
  --mms-path ../../mmsource \
  --hl2sdk-path ../../hl2sdk-tf2 \
  --manager-plugin tf2items_manager.smx \
  --enable-optimize
ambuild
```

Use the `spcomp` executable for the SourceMod version you are targeting. The install tree is written to `build/package/addons`.
