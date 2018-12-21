#include "native-lib.h"
#include "jni_lib.hpp"
#include "android_log.h"
#include "android_buf.hpp"

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
    return string_to_jstring(env, hello);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    string s = jstring_to_string(env, str);
    cout << s << endl;
    LOGI("%s", s.c_str());
}
