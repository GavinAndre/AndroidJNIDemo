#include "native-lib.h"
#include <string>
#include <iostream>
#include "android_buf.h"
#include "android_log.h"
#include "jni_lib.h"

using namespace std;

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_enableCout(JNIEnv *env, jclass type) {
    cout.rdbuf(new AndroidBuf);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_disableCout(JNIEnv *env, jclass type) {
    delete cout.rdbuf(0);
}

JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringFromJNI(JNIEnv *env, jclass type) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    string s = jstring_to_string(env, str);
    cout << s << endl;
    LOGI("%s", s.c_str());
}
