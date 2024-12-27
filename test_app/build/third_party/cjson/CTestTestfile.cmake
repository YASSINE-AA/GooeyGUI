# CMake generated Testfile for 
# Source directory: /home/yassine-ahmed-ali/Documents/GooeyGUI/third_party/cjson
# Build directory: /home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "/home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "/home/yassine-ahmed-ali/Documents/GooeyGUI/third_party/cjson/CMakeLists.txt;248;add_test;/home/yassine-ahmed-ali/Documents/GooeyGUI/third_party/cjson/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
