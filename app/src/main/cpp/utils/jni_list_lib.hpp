#include <jni.h>
#include <iostream>
#include <vector>
#include "android_log.h"
#include "../bean/simple_bean.hpp"
#include "../bean/complex_bean.hpp"

template<class _T>
_T java_to_native(JNIEnv *env, jobject &element);

template<class _T>
std::vector<_T> array_list_to_vector(JNIEnv *env, jobject &arrayList);

template<>
int java_to_native<int>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Integer");
    jmethodID getVal = env->GetMethodID(cls, "intValue", "()I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(element, getVal);
}

template<>
long java_to_native<long>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Long");
    jmethodID getVal = env->GetMethodID(cls, "longValue", "()J");
    env->DeleteLocalRef(cls);
    return static_cast<long>(env->CallLongMethod(element, getVal));
}

template<>
float java_to_native<float>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Float");
    jmethodID getVal = env->GetMethodID(cls, "floatValue", "()F");
    env->DeleteLocalRef(cls);
    return env->CallFloatMethod(element, getVal);
}

template<>
double java_to_native<double>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Double");
    jmethodID getVal = env->GetMethodID(cls, "doubleValue", "()D");
    env->DeleteLocalRef(cls);
    return env->CallDoubleMethod(element, getVal);
}

template<>
std::string java_to_native<std::string>(JNIEnv *env, jobject &element) {
    auto j_str = reinterpret_cast<jstring>(element);
    const char *c = env->GetStringUTFChars(j_str, nullptr);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

template<>
simple_bean java_to_native<simple_bean>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/SimpleBean");
    jmethodID getVal = env->GetMethodID(cls, "getList", "()I");
    env->DeleteLocalRef(cls);

    simple_bean _simple_bean;
    _simple_bean.integer = env->CallIntMethod(element, getVal);
    return _simple_bean;
}

template<>
complex_bean java_to_native<complex_bean>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/ComplexBean");
    jmethodID getVal = env->GetMethodID(cls, "getInteger", "()I");
    jmethodID getPath = env->GetMethodID(cls, "getList", "()Ljava/util/ArrayList;");
    env->DeleteLocalRef(cls);

    complex_bean _complex_bean;
    _complex_bean.integer = env->CallIntMethod(element, getVal);

    jobject list = env->CallObjectMethod(element, getPath);

    _complex_bean.list = array_list_to_vector<float>(env, list);
    return _complex_bean;
}

template<class _T>
std::vector<_T> array_list_to_vector(JNIEnv *env, jobject &arrayList) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = reinterpret_cast<jclass> (env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    // 获取java方法id
    // 参数2是调用的方法名，<init>表示构造函数
    // 参数3表示方法签名，(I)V表示参数为int型，返回值void型
    jmethodID java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size", "()I");
    // 同上，参数2表示add方法，参数3表示参数为Object对象，返回值为boolean型
    jmethodID java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get",
                                                         "(I)Ljava/lang/Object;");
    // 调用size方法，获取arrayList的大小
    jint len = env->CallIntMethod(arrayList, java_util_ArrayList_size);
    // 创建vector对象
    std::vector<_T> result;
    result.reserve(len);
    for (jint i = 0; i < len; i++) {
        // 调用get方法，获取jobject
        jobject element = env->CallObjectMethod(arrayList, java_util_ArrayList_get, i);
        _T f = java_to_native<_T>(env, element);
        env->DeleteLocalRef(element);
        result.emplace_back(f);
    }
    // 删除全局引用
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}

jobject vector_to_array_list(JNIEnv *env, std::vector<std::string> &vector) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    // 获取java方法id
    // 参数2是调用的方法名，<init>表示构造函数
    // 参数3表示方法签名，(I)V表示参数为int型，返回值void型
    jmethodID java_util_ArrayList_ = env->GetMethodID(java_util_ArrayList, "<init>", "(I)V");
    // 同上，参数2表示add方法，参数3表示参数为Object对象，返回值为boolean型
    jmethodID java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
    // 调用构造函数创建ArrayList对象
    jobject result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, vector.size());
    // 遍历vector
    for (std::string s: vector) {
        // string转jstring
        jstring element = env->NewStringUTF(s.c_str());
        // 调用add方法，将jstring添加到arrayList中
        env->CallBooleanMethod(result, java_util_ArrayList_add, element);
        // 删除jstring引用
        env->DeleteLocalRef(element);
    }
    // 删除全局引用
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}