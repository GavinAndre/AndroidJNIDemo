#include <jni.h>
#include <iostream>
#include <vector>
#include "android_log.h"

jstring string_to_jstring(JNIEnv *env, std::string str) {
    return env->NewStringUTF(str.c_str());
}

std::string jstring_to_string(JNIEnv *env, jstring j_str) {
    const char *c = env->GetStringUTFChars(j_str, 0);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

std::vector<std::string> array_list_to_vector(JNIEnv *env, jobject arrayList) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = static_cast<jclass> (env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    // 获取java方法id
    // 参数2是调用的方法名，<init>表示构造函数
    // 参数3表示方法签名，(I)V表示参数为int型，返回值void型
    jmethodID java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size", "()I");
    // 同上，参数2表示add方法，参数3表示参数为Object对象，返回值为boolean型
    jmethodID java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
    // 调用size方法，获取arrayList的大小
    jint len = env->CallIntMethod(arrayList, java_util_ArrayList_size);
    // 创建vector对象
    std::vector<std::string> result;
    result.reserve(len);
    for (jint i = 0; i < len; i++) {
        // 调用get方法，获取jstring
        jstring element = static_cast<jstring>(env->CallObjectMethod(arrayList, java_util_ArrayList_get, i));
        // jstring转char*
        const char *p_chars = env->GetStringUTFChars(element, nullptr);
        // 将char*添加到vector中
        result.emplace_back(p_chars);
        // 删除引用
        env->ReleaseStringUTFChars(element, p_chars);
        env->DeleteLocalRef(element);
    }
    // 删除全局引用
    env->DeleteGlobalRef(java_util_ArrayList);
    return result;
}

jobject vector_to_array_list(JNIEnv *env, std::vector<std::string> vector) {
    //先找到要调用的类，ArrayList，这里使用全局引用，也可以使用局部引用
    jclass java_util_ArrayList = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
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

std::string callJavaStaticMethod(JNIEnv *env, std::string class_name,
                                 std::string method_name, std::string method_sign) {
    // 先找到要调用的类
    jclass clazz = env->FindClass(class_name.c_str());
    if (clazz == nullptr) {
        LOGE("find class %s error !", class_name.c_str());
        return nullptr;
    }
    // 获取java方法id
    // 参数二是调用的方法名,参数三是方法的签名
    jmethodID id = env->GetStaticMethodID(clazz, method_name.c_str(), method_sign.c_str());
    if (id == nullptr) {
        LOGE("find method %s error !", method_name.c_str());
        return nullptr;
    }
    // 开始调用java中的静态方法
    jstring result = (jstring) env->CallStaticObjectMethod(clazz, id); //调用java方法
    // 释放资源
    env->DeleteLocalRef(clazz);
    // 将jstring转换为string
    return jstring_to_string(env, result);
}