#!/bin/bash

# 빌드 경로 설정
BUILD_DIR="build"
INSTALL_DIR="../../install_folder"

# CMake 설정 및 빌드 스크립트 시작
echo "CMake 설정 및 빌드 시작..."

# CMake 명령어 실행
cmake -B${BUILD_DIR} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCMAKE_PREFIX_PATH=${INSTALL_DIR} .
if [ $? -ne 0 ]; then
    echo "CMake 구성 실패!"
    exit 1
fi

# CMake 빌드 실행
cmake --build ${BUILD_DIR}
if [ $? -ne 0 ]; then
    echo "빌드 실패!"
    exit 1
fi

echo "CMake 빌드 완료!"

# cmake -Bbuild -DCMAKE_INSTALL_PREFIX=../../install_folder -DCMAKE_PREFIX_PATH=../../install_folder .
# cmake --build=build
# LD_LIBRARY_PATH=../../install_folder/lib/:$PWD/build/ ./build/service-example
# LD_LIBRARY_PATH=../../install_folder/lib/:$PWD/build/ ./build/client-example