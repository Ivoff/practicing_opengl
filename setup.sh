!# /bin/bash

GLAD_GL_VERSION=${GLAD_GL_VERSION:-4.6}
GLAD_GL_PROFILE=${GLAD_GL_PROFILE:-core}

GLFW_VERSION=${GLFW_VERSION:-3.3.2}

PYTHON_PATH=${PYTHON_PATH:-python3}

GLM_VERSION=${GLM_VERSION:-0.9.9.8}

ASSIMP_VERSION=${ASSIMP_VERSION:-5.2.4}

NUMBER_THREADS=${NUMBER_THREADS:-12}

if [ "$1" = "reset"  ]; then
    
    rm -rf ./_extern
    rm -rf ./include
    rm -rf ./src
    rm -rf ./lib
    rm -rf ./build
    exit 0
fi

if [ "$1" = "rmzip" ]; then
    rm -rf ./_extern/*.zip
    exit 0
fi

mkdir ./_extern #  por algum motivo isso aqui não funciona na linha seguinte -P ./_extern 
wget "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip" -O ./_extern/glad.zip
unzip ./_extern/glad.zip -d ./_extern
cd ./_extern/glad-master
"$PYTHON_PATH" -m glad --profile "$GLAD_GL_PROFILE" --api "gl=$GLAD_GL_VERSION" --generator "c" --out-path "./../../"
cd ./../../
mkdir src/glad
mv src/glad.c src/glad/glad.c

wget "https://github.com/g-truc/glm/archive/refs/tags/$GLM_VERSION.zip" -O ./_extern/glm-$GLM_VERSION.zip
cd ./_extern
unzip ./glm-$GLM_VERSION.zip
cp -r ./glm-$GLM_VERSION/glm ../include/
cd ../

mkdir ./include/stb_image
wget "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" -O ./include/stb_image/stb_image.h
wget "https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h" -O ./include/stb_image/stb_image_write.h

wget "https://github.com/glfw/glfw/releases/download/$GLFW_VERSION/glfw-$GLFW_VERSION.zip" -O ./_extern/glfw-$GLFW_VERSION.zip
cd ./_extern
unzip glfw-$GLFW_VERSION.zip
cd glfw-$GLFW_VERSION
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../../../
make
make install
cd ../../../ && touch src/env.hpp
ENV_CODE="#ifndef ENV_H\n#define ENV_H\n\n#define PROJECT_ROOT \"$PWD/\"\n\n#endif"
echo "$ENV_CODE" > src/env.hpp
mkdir build
touch src/main.cpp

wget "https://github.com/assimp/assimp/archive/refs/tags/v$ASSIMP_VERSION.zip" -O ./_extern/assimp.zip
unzip ./_extern/assimp.zip -d ./_extern
cd ./_extern/assimp-$ASSIMP_VERSION
cmake CMakeLists.txt -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF
make -j$NUMBER_THREADS
cp ./bin/libassimp* ../../lib/
cp -r ./include/* ../../include/
cd ../../