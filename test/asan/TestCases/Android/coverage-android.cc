// Test for direct coverage writing with dlopen.

// Test normal exit, coverage level 1.
// RUN: %clangxx_asan -fsanitize-coverage=1 -DSHARED %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=1 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android
// RUN: rm -rf %T/coverage-android

// RUN: adb shell mkdir -p %device/coverage-android/direct
// RUN: mkdir -p %T/coverage-android/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android/direct:verbosity=1 %run %t
// RUN: adb pull %device/coverage-android/direct %T/coverage-android/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK1 %s


// Test sudden death, coverage level 1.
// RUN: %clangxx_asan -fsanitize-coverage=1 -DSHARED -DKILL %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=1 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android-kill
// RUN: rm -rf %T/coverage-android-kill

// RUN: adb shell mkdir -p %device/coverage-android-kill/direct
// RUN: mkdir -p %T/coverage-android-kill/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android-kill/direct:verbosity=1 not %run %t
// RUN: adb pull %device/coverage-android-kill/direct %T/coverage-android-kill/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android-kill/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK1 %s


// Test normal exit, coverage level 2.
// RUN: %clangxx_asan -fsanitize-coverage=2 -DSHARED %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=2 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android
// RUN: rm -rf %T/coverage-android

// RUN: adb shell mkdir -p %device/coverage-android/direct
// RUN: mkdir -p %T/coverage-android/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android/direct:verbosity=1 %run %t
// RUN: adb pull %device/coverage-android/direct %T/coverage-android/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK2 %s


// Test sudden death, coverage level 2.
// RUN: %clangxx_asan -fsanitize-coverage=2 -DSHARED -DKILL %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=2 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android-kill
// RUN: rm -rf %T/coverage-android-kill

// RUN: adb shell mkdir -p %device/coverage-android-kill/direct
// RUN: mkdir -p %T/coverage-android-kill/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android-kill/direct:verbosity=1 not %run %t
// RUN: adb pull %device/coverage-android-kill/direct %T/coverage-android-kill/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android-kill/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK2 %s


// Test normal exit, coverage level 3.
// RUN: %clangxx_asan -fsanitize-coverage=3 -DSHARED %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=3 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android
// RUN: rm -rf %T/coverage-android

// RUN: adb shell mkdir -p %device/coverage-android/direct
// RUN: mkdir -p %T/coverage-android/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android/direct:verbosity=1 %run %t
// RUN: adb pull %device/coverage-android/direct %T/coverage-android/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK3 %s


// Test sudden death, coverage level 3.
// RUN: %clangxx_asan -fsanitize-coverage=3 -DSHARED -DKILL %s -shared -o %T/libcoverage_android_test_1.so -fPIC
// RUN: %clangxx_asan -fsanitize-coverage=3 -DSO_DIR=\"%device\" %s -o %t

// RUN: adb shell rm -rf %device/coverage-android-kill
// RUN: rm -rf %T/coverage-android-kill

// RUN: adb shell mkdir -p %device/coverage-android-kill/direct
// RUN: mkdir -p %T/coverage-android-kill/direct
// RUN: ASAN_OPTIONS=coverage=1:coverage_direct=1:coverage_dir=%device/coverage-android-kill/direct:verbosity=1 not %run %t
// RUN: adb pull %device/coverage-android-kill/direct %T/coverage-android-kill/direct
// RUN: ls; pwd
// RUN: cd %T/coverage-android-kill/direct
// RUN: %sancov rawunpack *.sancov.raw
// RUN: %sancov print *.sancov |& FileCheck --check-prefix=CHECK3 %s

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#ifdef SHARED
extern "C" {
void bar() {
  printf("bar\n");
#ifdef KILL
  kill(getpid(), SIGKILL);
#endif
}
}
#else

volatile int sink;

int main(int argc, char **argv) {
  fprintf(stderr, "PID: %d\n", getpid());
  void *handle1 =
      dlopen(SO_DIR "/libcoverage_android_test_1.so", RTLD_LAZY);
  assert(handle1);

  if (argc == 0)
    sink = 0;

  void (*bar1)() = (void (*)())dlsym(handle1, "bar");
  assert(bar1);
  bar1();

  return 0;
}
#endif

// CHECK1: 2 PCs total
// CHECK2: 7 PCs total
// CHECK3: 8 PCs total
