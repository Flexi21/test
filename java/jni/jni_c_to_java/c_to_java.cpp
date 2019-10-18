#include <jni.h>
#include <stdio.h>
#include "Hello.h"

/*
 * We use inlined functions for C++ so that programmers can write:
 *
 *    env->FindClass("java/lang/String")
 *
 * in C++ rather than:
 *
 *    (*env)->FindClass(env, "java/lang/String")
 *
 * in C.
 */

JNIEXPORT jint JNICALL Java_Hello_hello(JNIEnv *env, jclass cls)
{
    jclass testclass = env->FindClass("Hello");

/*
    jmethodID GetMethodID(jclass clazz, const char *name,
                          const char *sig)
*/
    jmethodID testcontruct = env->GetMethodID(testclass, "<init>", "()V"); 

    jobject testobject = env->NewObject(testclass, testcontruct); 

    jmethodID test = env->GetMethodID(testclass, "test", "()V"); 

    env->CallVoidMethod(testobject, test, 1);

    printf("hello\n");
}
