#include <jni.h>
#include <iostream>

using namespace std;

jstring string_to_jstring(JNIEnv *env, string str) {
    return env->NewStringUTF(str.c_str());
}

string jstring_to_string(JNIEnv *env, jstring j_str) {
    const char *c = env->GetStringUTFChars(j_str, 0);
    string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}