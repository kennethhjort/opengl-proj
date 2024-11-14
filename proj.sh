
#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <option>"
    exit 1
fi

case "$1" in
    "run")
        echo "Running application..."
        pushd build/bin/ > /dev/null
        ./ecs
        ;;
    "build")
        SOURCE_DIR="./assets/"
        DEST_DIR="./build/bin/"

        echo "Building application..."
        pushd build/ > /dev/null
        cmake --build .
        popd > /dev/null

        mkdir -p "$DEST_DIR"
        rsync -av --update "$SOURCE_DIR" "$DEST_DIR"
        ;;
    "clean")
        echo "Cleaning Build"
        pushd build/ > /dev/null
        cmake --build . --target clean
        ;;
    "cmake")
        echo "Running CMAKE"
        pushd build/ > /dev/null
        cmake ..
        popd > /dev/null
        ;;
    *)
        echo "Invalid option: $1"
        echo "Usage: $0 <option>"
        exit 1
        ;;
esac
