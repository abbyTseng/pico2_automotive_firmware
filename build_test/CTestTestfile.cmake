# CMake generated Testfile for 
# Source directory: /workspace/test
# Build directory: /workspace/build_test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_blink "/workspace/build_test/test_blink")
set_tests_properties(test_blink PROPERTIES  _BACKTRACE_TRIPLES "/workspace/test/CMakeLists.txt;36;add_test;/workspace/test/CMakeLists.txt;0;")
subdirs("_deps/unity-build")
