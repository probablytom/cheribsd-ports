--- third_party/zlib/cpu_features.c.orig	2020-11-16 14:31:04 UTC
+++ third_party/zlib/cpu_features.c
@@ -35,11 +35,20 @@ int ZLIB_INTERNAL x86_cpu_enable_simd = 0;
 
 #ifndef CPU_NO_SIMD
 
-#if defined(ARMV8_OS_ANDROID) || defined(ARMV8_OS_LINUX) || defined(ARMV8_OS_FUCHSIA) || defined(ARMV8_OS_IOS)
+#if defined(ARMV8_OS_ANDROID) || defined(ARMV8_OS_LINUX) || defined(ARMV8_OS_FUCHSIA) || defined(ARMV8_OS_IOS) || defined(ARMV8_OS_FREEBSD)
 #include <pthread.h>
 #endif
 
-#if defined(ARMV8_OS_ANDROID)
+#if defined(ARMV8_OS_FREEBSD)
+#include <machine/armreg.h>
+#include <sys/types.h>
+#ifndef ID_AA64ISAR0_AES_VAL
+#define ID_AA64ISAR0_AES_VAL ID_AA64ISAR0_AES
+#endif
+#ifndef ID_AA64ISAR0_CRC32_VAL
+#define ID_AA64ISAR0_CRC32_VAL ID_AA64ISAR0_CRC32
+#endif
+#elif defined(ARMV8_OS_ANDROID)
 #include <cpu-features.h>
 #elif defined(ARMV8_OS_LINUX)
 #include <asm/hwcap.h>
@@ -62,7 +72,7 @@ int ZLIB_INTERNAL x86_cpu_enable_simd = 0;
 static void _cpu_check_features(void);
 #endif
 
-#if defined(ARMV8_OS_ANDROID) || defined(ARMV8_OS_LINUX) || defined(ARMV8_OS_MACOS) || defined(ARMV8_OS_FUCHSIA) || defined(X86_NOT_WINDOWS) || defined(ARMV8_OS_IOS)
+#if defined(ARMV8_OS_ANDROID) || defined(ARMV8_OS_LINUX) || defined(ARMV8_OS_MACOS) || defined(ARMV8_OS_FUCHSIA) || defined(X86_NOT_WINDOWS) || defined(ARMV8_OS_IOS) || defined(ARMV8_OS_FREEBSD)
 #if !defined(ARMV8_OS_MACOS)
 // _cpu_check_features() doesn't need to do anything on mac/arm since all
 // features are known at build time, so don't call it.
@@ -146,6 +146,13 @@ static void _cpu_check_features(void)
    len = sizeof(val);
    arm_cpu_enable_pmull = sysctlbyname("hw.optional.arm.FEAT_PMULL", &val, &len, 0, 0) == 0
               && val != 0;
+#elif defined(ARMV8_OS_FREEBSD)
+    uint64_t id_aa64isar1;
+    id_aa64isar0 = READ_SPECIALREG(id_aa64isar0_el1);
+    if (ID_AA64ISAR0_AES_VAL(id_aa64isar0) == ID_AA64ISAR0_AES_PMULL)
+        arm_cpu_enable_pmull = 1;
+    if (ID_AA64ISAR0_CRC32_VAL(id_aa64isar0) == ID_AA64ISAR0_CRC32_BASE)
+        arm_cpu_enable_crc32 = 1;
 #endif
 }
 #endif
