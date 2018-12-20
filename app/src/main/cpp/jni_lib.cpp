#include "jni_lib.h"

string jstring_to_string(JNIEnv *env, jstring j_str) {
    const char *c = env->GetStringUTFChars(j_str, 0);
    string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}