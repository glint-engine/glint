configure:
    xmake config --mode=debug
    npm install

check:
    xmake check clang.tidy --quiet
    npm run lint

build:
    xmake build muen

run game:
    xmake run muen {{ justfile_dir() / "examples" / game }}

format:
    xmake format
    npm run format

