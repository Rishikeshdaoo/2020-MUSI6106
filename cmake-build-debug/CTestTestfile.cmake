# CMake generated Testfile for 
# Source directory: /Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106
# Build directory: /Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AudioIo "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/bin/TestExec" "AudioIo" "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/src/Tests/TestData/")
set_tests_properties(AudioIo PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;64;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;0;")
add_test(CombFilter "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/bin/TestExec" "CombFilter")
set_tests_properties(CombFilter PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;65;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;0;")
add_test(RingBuff "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/bin/TestExec" "RingBuff")
set_tests_properties(RingBuff PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;66;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;0;")
add_test(Vibrato "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/bin/TestExec" "Vibrato")
set_tests_properties(Vibrato PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;67;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/ASE_Main/2020-MUSI6106/CMakeLists.txt;0;")
subdirs("MUSI6106Exec")
subdirs("AudioFileIO")
subdirs("CombFilter")
subdirs("Vibrato")
subdirs("TestExec")
