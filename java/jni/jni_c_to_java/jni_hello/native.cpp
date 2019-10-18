#include <jni.h>
#include <stdio.h>

int hello()
{
    printf("Hello world!\n");
    return 0;
}

static JNINativeMethod methods[] = {
    {"hello", "()I", (void*)hello },
};

/*
*  The java class name 
*/
static const char *className = "Hello";  

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint result = -1;
    JNIEnv* env = NULL;
    jclass clazz;

/*
*  jint GetEnv(void **penv, jint version)
*/

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
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
