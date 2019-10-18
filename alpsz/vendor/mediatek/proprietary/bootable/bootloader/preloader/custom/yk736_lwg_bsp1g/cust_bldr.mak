###################################################################
# Include Project Feautre  (cust_bldr.h)
###################################################################

# To set CFG_BOOT_DEV, select one of the following options:
# BOOTDEV_UFS, BOOTDEV_SDMMC, BOOTDEV_NAND
CFG_BOOT_DEV :=BOOTDEV_SDMMC

CFG_UART_LOG :=UART1
CFG_UART_META :=UART1

##### TEE >PLEASE ADD CONFIGS ABOVE THIS LINE< TEE #####
ifeq ($(strip $(MTK_TEE_SUPPORT)),yes)
  CFG_TEE_SUPPORT = 1
  ifeq ($(strip $(TRUSTONIC_TEE_SUPPORT)),yes)
    CFG_TRUSTONIC_TEE_SUPPORT = 1
  else
    CFG_TRUSTONIC_TEE_SUPPORT = 0
  endif
  ifeq ($(strip $(MICROTRUST_TEE_SUPPORT)),yes)
    CFG_MICROTRUST_TEE_SUPPORT = 1
  else
    CFG_MICROTRUST_TEE_SUPPORT = 0
  endif
  ifeq ($(strip $(MTK_GOOGLE_TRUSTY_SUPPORT)),yes)
    CFG_GOOGLE_TRUSTY_SUPPORT = 1
  else
    CFG_GOOGLE_TRUSTY_SUPPORT = 0
  endif
###zwl add for pingbo tee 20180505
  ifeq ($(strip $(TRUSTKERNEL_TEE_SUPPORT)),yes)
    CFG_TRUSTKERNEL_TEE_SUPPORT = 1
    CFG_TRUSTKERNEL_TEE_SDRPMB_SUPPORT = 1
  else
    CFG_TRUSTKERNEL_TEE_SUPPORT = 0
    CFG_TRUSTKERNEL_TEE_SDRPMB_SUPPORT = 0
  endif
else
  CFG_TEE_SUPPORT = 0
  CFG_TRUSTONIC_TEE_SUPPORT = 0
  CFG_MICROTRUST_TEE_SUPPORT = 0
  CFG_GOOGLE_TRUSTY_SUPPORT = 0
  ifeq ($(strip $(TRUSTKERNEL_TEE_SUPPORT)),yes)
  CFG_TRUSTKERNEL_TEE_SUPPORT = 0
  CFG_TRUSTKERNEL_TEE_SDRPMB_SUPPORT = 0
  endif
endif
$(warning CFG_TEE_SUPPORT=$(CFG_TEE_SUPPORT))
$(warning CFG_TRUSTONIC_TEE_SUPPORT=$(CFG_TRUSTONIC_TEE_SUPPORT))
$(warning CFG_MICROTRUST_TEE_SUPPORT=$(CFG_MICROTRUST_TEE_SUPPORT))
$(warning CFG_GOOGLE_TRUSTY_SUPPORT=$(CFG_GOOGLE_TRUSTY_SUPPORT))
$(warning CFG_TRUSTKERNEL_TEE_SUPPORT=$(CFG_TRUSTKERNEL_TEE_SUPPORT))
