#include <jni.h>
#include <iostream>
#include <vector>
#include "android_log.h"
#include "../bean/simple_bean.hpp"
#include "../bean/complex_bean.hpp"

/**
 * 声明模板函数
 */
template<class _T>
_T java_to_native(JNIEnv *env, jobject &element);

template<class _T>
jobject native_to_java(JNIEnv *env, _T &element);

template<class _T>
std::vector<_T> array_list_to_vector(JNIEnv *env, jobject &arrayList);

template<class _T>
jobject vector_to_array_list(JNIEnv *env, std::vector<_T> &vector);

/**
 * 重载不同类型的转换函数
 */
template<>
int java_to_native(JNIEnv *env, jobject &element) {
    //查找要调用的类
    jclass cls = env->FindClass("java/lang/Integer");
    //获取java方法id
    jmethodID getVal = env->GetMethodID(cls, "intValue", "()I");
    //删除局部引用
    env->DeleteLocalRef(cls);
    //调用intValue方法
    return env->CallIntMethod(element, getVal);
}

template<>
long java_to_native(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Long");
    jmethodID getVal = env->GetMethodID(cls, "longValue", "()J");
    env->DeleteLocalRef(cls);
    return static_cast<long>(env->CallLongMethod(element, getVal));
}

template<>
float java_to_native(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Float");
    jmethodID getVal = env->GetMethodID(cls, "floatValue", "()F");
    env->DeleteLocalRef(cls);
    return env->CallFloatMethod(element, getVal);
}

template<>
double java_to_native(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("java/lang/Double");
    jmethodID getVal = env->GetMethodID(cls, "doubleValue", "()D");
    env->DeleteLocalRef(cls);
    return env->CallDoubleMethod(element, getVal);
}

template<>
std::string java_to_native(JNIEnv *env, jobject &element) {
    auto j_str = reinterpret_cast<jstring>(element);
    const char *c = env->GetStringUTFChars(j_str, nullptr);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

template<>
simple_bean java_to_native<simple_bean>(JNIEnv *env, jobject &element) {
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/SimpleBean");
    jmethodID getVal = env->GetMethodID(cls, "getInteger", "()I");
    env->DeleteLocalRef(cls);
    int value = env->CallIntMethod(element, getVal);
    simple_bean _simple_bean(value);
    return _simple_bean;
}

template<>
complex_bean java_to_native(JNIEnv *env, jobject &element) {
    //查找要调用的类
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/ComplexBean");
    //获取java方法id
    jmethodID getInteger = env->GetMethodID(cls, "getInteger", "()I");
    jmethodID getList = env->GetMethodID(cls, "getList", "()Ljava/util/ArrayList;");
    //删除ComplexBean jclass局部引用
    env->DeleteLocalRef(cls);
    //创建c++对象
    complex_bean _complex_bean;
    //调用getInteger方法后赋值
    _complex_bean.integer = env->CallIntMethod(element, getInteger);
    //调用getList方法获取arrayList
    jobject list = env->CallObjectMethod(element, getList);
    //将arrayList转换成vector后赋值
    _complex_bean.list = array_list_to_vector<float>(env, list);
    return _complex_bean;
}

template<class _T>
std::vector<_T> array_list_to_vector(JNIEnv *env, jobject &arrayList) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = reinterpret_cast<jclass> (env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    //获取java方法id
    //参数2是调用的方法名，<init>表示构造函数
    //参数3表示方法签名，(I)V表示参数为int型，返回值void型
    jmethodID java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size", "()I");
    //同上，参数2表示add方法，参数3表示参数为Object对象，返回值为boolean型
    jmethodID java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get",
                                                         "(I)Ljava/lang/Object;");
    //调用size方法，获取arrayList的大小
    jint len = env->CallIntMethod(arrayList, java_util_ArrayList_size);
    //创建vector对象
    std::vector<_T> result;
    result.reserve(len);
    for (jint i = 0; i < len; i++) {
        //调用get方法，获取jobject
        jobject element = env->CallObjectMethod(arrayList, java_util_ArrayList_get, i);
        //将jobject转换成c/c++类型
        auto f = java_to_native<_T>(env, element);
        //放入victor中
        result.emplace_back(f);
        //删除局部引用
        env->DeleteLocalRef(element);
    }
    //删除全局引用
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, int &element) {
    //查找要调用的类
    jclass cls = env->FindClass("java/lang/Integer");
    //获取java方法id
    jmethodID init = env->GetMethodID(cls, "<init>", "(I)V");
    //调用构造函数创建对象
    jobject result = env->NewObject(cls, init, element);
    //删除局部引用
    env->DeleteLocalRef(cls);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, long &element) {
    jclass cls = env->FindClass("java/lang/Long");
    jmethodID init = env->GetMethodID(cls, "<init>", "(J)V");
    jobject result = env->NewObject(cls, init, element);
    env->DeleteLocalRef(cls);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, float &element) {
    jclass cls = env->FindClass("java/lang/Float");
    jmethodID init = env->GetMethodID(cls, "<init>", "(F)V");
    jobject result = env->NewObject(cls, init, element);
    env->DeleteLocalRef(cls);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, double &element) {
    jclass cls = env->FindClass("java/lang/Double");
    jmethodID init = env->GetMethodID(cls, "<init>", "(D)V");
    jobject result = env->NewObject(cls, init, element);
    env->DeleteLocalRef(cls);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, std::string &element) {
    return env->NewStringUTF(element.c_str());
}

template<>
jobject native_to_java(JNIEnv *env, simple_bean &element) {
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/SimpleBean");
    jmethodID init = env->GetMethodID(cls, "<init>", "(I)V");
    jobject result = env->NewObject(cls, init, element);
    env->DeleteLocalRef(cls);
    return result;
}

template<>
jobject native_to_java(JNIEnv *env, complex_bean &element) {
    //查找要调用的类
    jclass cls = env->FindClass("com/gavinandre/jnidemo/bean/ComplexBean");
    //获取java方法id
    jmethodID init = env->GetMethodID(cls, "<init>", "()V");
    jmethodID setInteger = env->GetMethodID(cls, "setInteger", "(I)V");
    jmethodID setList = env->GetMethodID(cls, "setList", "(Ljava/util/ArrayList;)V");
    //调用构造函数创建ComplexBean对象
    jobject result = env->NewObject(cls, init);
    //调用setInteger方法
    env->CallVoidMethod(result, setInteger, element.integer);
    //将vector转换成arrayList
    jobject list = vector_to_array_list<float>(env, element.list);
    //调用setList方法
    env->CallVoidMethod(result, setList, list);
    //删除setList方法局部引用
    env->DeleteLocalRef(list);
    //删除ComplexBean jclass局部引用
    env->DeleteLocalRef(cls);
    return result;
}

template<class _T>
jobject vector_to_array_list(JNIEnv *env, std::vector<_T> &vector) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    //获取java方法id
    //参数2是调用的方法名，<init>表示构造函数
    //参数3表示方法签名，(I)V表示参数为int型，返回值void型
    jmethodID java_util_ArrayList_ = env->GetMethodID(java_util_ArrayList, "<init>", "(I)V");
    //同上，参数2表示add方法，参数3表示参数为Object对象，返回值为boolean型
    jmethodID java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
    //调用构造函数创建ArrayList对象
    jobject result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, vector.size());
    //遍历vector
    for (auto &v: vector) {
        //c/c++类型转jobject
        auto element = native_to_java<_T>(env, v);
        //调用add方法，将jobject添加到arrayList中
        env->CallBooleanMethod(result, java_util_ArrayList_add, element);
        //删除局部引用
        env->DeleteLocalRef(element);
    }
    //删除全局引用
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}