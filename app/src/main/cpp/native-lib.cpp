#include "native-lib.h"
#include <string>
#include <iostream>
#include "android_log.h"
#include "android_buf.h"

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
Java_com_gavinandre_jnidemo_NativeLib_stringFromJNI(JNIEnv *env, jclass /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    const char *c_str = env->GetStringUTFChars(str, 0);
    std::string string = std::string(c_str);
    cout << string << endl;
    LOGI("%s", string.c_str());
    env->ReleaseStringUTFChars(str, c_str);
}
