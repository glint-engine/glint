configure:
    xmake config --mode=debug
    # xmake project -k compile_commands

check:
    xmake check clang.tidy --quiet

build:
    xmake build muen

run game:
    xmake run muen {{ justfile_dir() / "examples" / game }}

format:
    xmake format

