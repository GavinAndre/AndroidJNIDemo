#include "utils/jni_lib.hpp"
#include "utils/jni_list_lib.hpp"
#include "utils/android_log.h"
#include "utils/android_buf.hpp"
#include "utils/uuid_lib.hpp"
#include "utils/base64.hpp"

void enableCout() {
    std::cout.rdbuf(new AndroidBuf);
}

void disableCout() {
    delete std::cout.rdbuf(0);
}

jstring stringFromJNI(JNIEnv *env) {
    std::string hello = "Hello from C++";
    return string_to_jstring(env, hello);
}

void stringToJNI(JNIEnv *env, jstring &str) {
    std::string s = jstring_to_string(env, str);
    std::cout << s << std::endl;
    LOGI("stringToJNI: %s", s.c_str());
}

void intListToJNI(JNIEnv *env, jobject &array_list) {
    auto result = array_list_to_vector<int>(env, array_list);
    std::cout << "intListToJNI: [";
    for (const auto &s: result) {
        std::cout << s << " ";
    }
    std::cout << "]" << std::endl;
}

void doubleListToJNI(JNIEnv *env, jobject &array_list) {
    auto result = array_list_to_vector<double>(env, array_list);
    std::cout << "doubleListToJNI: [";
    for (const auto &s: result) {
        std::cout << s << " ";
    }
    std::cout << "]" << std::endl;
}

void stringListToJNI(JNIEnv *env, jobject &array_list) {
    auto result = array_list_to_vector<std::string>(env, array_list);
    std::cout << "stringListToJNI: [";
    for (const auto &s: result) {
        std::cout << s << " ";
    }
    std::cout << "]" << std::endl;
}

void simpleObjectListToJNI(JNIEnv *env, jobject &array_list) {
    auto result = array_list_to_vector<simple_bean>(env, array_list);
    std::cout << "simpleObjectListToJNI: [";
    for (const auto &s: result) {
        std::cout << s.integer << " ";
    }
    std::cout << "]" << std::endl;
}

void complexObjectListToJNI(JNIEnv *env, jobject &array_list) {
    auto result = array_list_to_vector<complex_bean>(env, array_list);
    std::cout << "complexObjectListToJNI: [";
    for (const auto &s: result) {
        std::cout << s.integer << " ";
        for (const auto &ss : s.list) {
            std::cout << ss << " ";
        }
    }
    std::cout << "]" << std::endl;
}

jobject intListFromJNI(JNIEnv *env) {
    std::vector<int> v_s;
    v_s.reserve(3);
    v_s.emplace_back(3);
    v_s.emplace_back(2);
    v_s.emplace_back(1);
    return vector_to_array_list<int>(env, v_s);
}

jobject doubleListFromJNI(JNIEnv *env) {
    std::vector<double> v_s;
    v_s.reserve(3);
    v_s.emplace_back(3);
    v_s.emplace_back(2);
    v_s.emplace_back(1);
    return vector_to_array_list<double>(env, v_s);
}

jobject stringListFromJNI(JNIEnv *env) {
    std::vector<std::string> v_s;
    v_s.reserve(3);
    v_s.emplace_back("3");
    v_s.emplace_back("2");
    v_s.emplace_back("1");
    return vector_to_array_list<std::string>(env, v_s);
}

jobject simpleObjectListFromJNI(JNIEnv *env) {
    std::vector<simple_bean> v_s;
    v_s.reserve(3);
    v_s.emplace_back(simple_bean(3));
    v_s.emplace_back(simple_bean(2));
    v_s.emplace_back(simple_bean(1));
    return vector_to_array_list<simple_bean>(env, v_s);
}

jobject complexObjectListFromJNI(JNIEnv *env) {
    std::vector<complex_bean> v_s;
    v_s.reserve(3);
    v_s.emplace_back(complex_bean(3, {3.3, 3.2, 3.1}));
    v_s.emplace_back(complex_bean(2, {2.3, 2.2, 2.1}));
    v_s.emplace_back(complex_bean(1, {1.3, 1.2, 1.1}));
    return vector_to_array_list<complex_bean>(env, v_s);
}

jstring uuidFromJNI(JNIEnv *env) {
    std::string uuid = generate_hex(16);
    return string_to_jstring(env, uuid);
}

jstring getSDPath(JNIEnv *env) {
    std::string sd_path = callJavaStaticMethod(
            env, "com/gavinandre/jnidemo/utils/FileUtil", "getSDPath", "()Ljava/lang/String;");
    return string_to_jstring(env, sd_path);
}

jstring base64Encode(JNIEnv *env) {
    std::string str = "Hello World!";
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char *>(str.c_str()), str.length());
    return string_to_jstring(env, encoded);
}

void base64decode(JNIEnv *env, jstring &encode_data) {
    std::string decoded_data = base64_decode(jstring_to_string(env, encode_data));
    LOGI("base64decode %s", decoded_data.c_str());
}