#include <jni.h>
#include <stdio.h>

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;



int hello()
{

    printf("Hello world!\n");
    return 0;
}

static JNINativeMethod methods[] = {
    {"hello", "()I", (void*)hello },
};


static const char *className = "Hello";
/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*classPathName
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, className,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


/*
 * This is called by the VM when the shared library is first loaded.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
    

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) { //get a environment 
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        goto bail;
    }
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
