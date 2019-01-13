#include "native-lib.h"
#include "utils/jni_lib.hpp"
#include "utils/android_log.h"
#include "utils/android_buf.hpp"
#include "utils/uuid_lib.hpp"

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

void stringToJNI(JNIEnv *env, jstring str) {
    std::string s = jstring_to_string(env, str);
    std::cout << s << std::endl;
    LOGI("%s", s.c_str());
}

void listToJNI(JNIEnv *env, jobject array_list) {
    std::vector<std::string> result = array_list_to_vector(env, array_list);
    for (std::string s: result) {
        std::cout << s << std::endl;
    }
}

jobject listFromJNI(JNIEnv *env) {
    std::vector<std::string> v_s;
    v_s.reserve(3);
    v_s.emplace_back("3");
    v_s.emplace_back("2");
    v_s.emplace_back("1");
    return vector_to_array_list(env, v_s);
}

jstring uuidFromJNI(JNIEnv *env) {
    std::string uuid = generate_hex(16);
    return string_to_jstring(env, uuid);
}

jstring getSDPath(JNIEnv *env) {
    std::string sd_path = callJavaStaticMethod(env, "com/gavinandre/jnidemo/utils/FileUtil",
                                               "getSDPath", "()Ljava/lang/String;");
    return string_to_jstring(env, sd_path);
}