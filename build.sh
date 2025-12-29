#!/bin/bash

cd cmake-build-debug || exit 1

select t in enginend test forespend endlauncher; do
    case $t in
        enginend|test|forespend|endlauncher)
            cmake --build . --target "$t"
            ;;
    esac
    break
done

