#include <jni.h>
#include <iostream>
#include <vector>

jstring string_to_jstring(JNIEnv *env, std::string str) {
    return env->NewStringUTF(str.c_str());
}

std::string jstring_to_string(JNIEnv *env, jstring j_str) {
    const char *c = env->GetStringUTFChars(j_str, 0);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

std::vector<std::string> java2cpp(JNIEnv *env, jobject arrayList) {
    jclass java_util_ArrayList = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    jmethodID java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size", "()I");
    jmethodID java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

    jint len = env->CallIntMethod(arrayList, java_util_ArrayList_size);
    std::vector<std::string> result;
    result.reserve(len);
    for (jint i = 0; i < len; i++) {
        jstring element = static_cast<jstring>(env->CallObjectMethod(arrayList, java_util_ArrayList_get, i));
        const char *pchars = env->GetStringUTFChars(element, nullptr);
        result.emplace_back(pchars);
        env->ReleaseStringUTFChars(element, pchars);
        env->DeleteLocalRef(element);
    }
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}

jobject cpp2java(JNIEnv *env, std::vector<std::string> vector) {
    jclass java_util_ArrayList = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    jmethodID java_util_ArrayList_ = env->GetMethodID(java_util_ArrayList, "<init>", "(I)V");
    jmethodID java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");

    jobject result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, vector.size());
    for (std::string s: vector) {
        jstring element = env->NewStringUTF(s.c_str());
        env->CallBooleanMethod(result, java_util_ArrayList_add, element);
        env->DeleteLocalRef(element);
    }
    env->DeleteGlobalRef(java_util_ArrayList);

    return result;
}