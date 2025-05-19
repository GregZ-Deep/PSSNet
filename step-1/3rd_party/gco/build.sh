set -e

rm -rf build
mkdir -p build
cd build

cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5 2>&1 | tee cmake_log.txt

make 2>&1 | tee build_log.txt