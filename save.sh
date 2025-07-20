set -e
set -x
set calling_dir=${PWD}
cd ../
./copy_zips
cd ../../
./resave_project
cd ${calling_dir}
set +x
set +e
