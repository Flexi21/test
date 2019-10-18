#include <jni.h>
#include <stdio.h>

  jclass clazz;

  JNIEnv* env = NULL;


static void test(JNIEnv* env,jclass clazz)
{

    jmethodID testcontruct = env->GetMethodID(clazz, "<init>", "()V"); 

    jobject testobject = env->NewObject(clazz, testcontruct); 

    jmethodID test = env->GetMethodID(clazz, "test", "()V"); 

    env->CallVoidMethod(testobject, test, 1);

}

static void test(JNIEnv* env,jclass clazz);

int hello()
{
    printf("Hello world!\n");

    test(env,clazz);

    return 0;
}

static JNINativeMethod methods[] = {
    {"hello", "()I", (void*)hello },
};

/*
static void c_to_java_test(JNIEnv *env, jclass class_test)
{
    java_test = (*env)-> GetMethodID(env, class_test, "test", "()I" );
    
    obj_test = env->NewObject(class_test, java_test);  
}

JNIEXPORT jint JNICALL Java_Hello_hello(JNIEnv *env, jclass cls)
{
    jclass testclass = env->FindClass("Hello");

    jmethodID GetMethodID(jclass clazz, const char *name,
                          const char *sig)
    jmethodID testcontruct = env->GetMethodID(testclass, "<init>", "()V"); 

    jobject testobject = env->NewObject(testclass, testcontruct); 

    jmethodID test = env->GetMethodID(testclass, "test", "()V"); 

    env->CallVoidMethod(testobject, test, 1);

    printf("hello\n");
}

*/

/*
*  The java class name 
*/
static const char *className = "Hello";  

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint result = -1;
   // env->CallVoidMethod(testobject, test, 1);

//zwl c_to_java end

    if (vm->GetEnv(reinterpret_cast<void**> (&env), JNI_VERSION_1_4) != JNI_OK) { //get a environment 
        goto bail;
    }

    clazz = env->FindClass(className);

    if (clazz == NULL) {
        return JNI_ERR;
    }


    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_FALSE;
    }

    //c_to_java_test(env, clazz);
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
