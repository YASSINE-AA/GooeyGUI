#!/bin/bash
echo "------------------"
echo "RUNNING THE CJON PATSH SCRIPT"
echo "------------------"

CURR_PATH="${PWD}"

ROOT_DIR="$(dirname ${CURR_PATH})"
CJSON_SUBMODULE_PATH="${ROOT_DIR}/third_party/cjson"
PATSH_FILE="${CURR_PATH}/cjson_submodule.patch" 
# echo "${ROOT_DIR}"
# echo "${PATSH_PATH}"
# echo "${CJSON_SUBMODULE_FILE}"

pushd "${CJSON_SUBMODULE_PATH}"
git apply "${PATSH_FILE}"
popd
