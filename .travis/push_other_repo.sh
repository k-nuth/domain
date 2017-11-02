#!/bin/bash

set -e
set -x

git config --global user.email "ci@bitprim.org"
git config --global user.name "Bitprim CI"      

mkdir temp
cd temp

# git clone https://github.com/bitprim/bitprim-node-exe.git --depth 1
git clone https://github.com/bitprim/bitprim-node-exe.git

cd bitprim-node-exe
echo "Travis branch: ${TRAVIS_BRANCH}"
git checkout ${TRAVIS_BRANCH}
echo "bitprim-core: $BITPRIM_BUILD_NUMBER" >> versions.txt
cat versions.txt
git add . versions.txt
git commit --message "Travis bitprim-core build: $BITPRIM_BUILD_NUMBER, $TRAVIS_BUILD_NUMBER"
git remote add origin-commit https://${GH_TOKEN}@github.com/bitprim/bitprim-node-exe.git > /dev/null 2>&1
git push --quiet --set-upstream origin-commit ${TRAVIS_BRANCH} 
