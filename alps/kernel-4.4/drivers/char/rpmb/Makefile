ifeq ($(CONFIG_MEDIATEK_SOLUTION),y)
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
ccflags-y += -I$(srctree)/drivers/mmc/card
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
ccflags-y += -I$(srctree)/drivers/scsi/ufs

ifeq ($(CONFIG_MTK_PLATFORM), "mt6739")
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA/mt6739
else ifeq ($(CONFIG_MTK_PLATFORM), "mt6799")
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA/mt6799
else ifeq ($(CONFIG_MTK_PLATFORM), "mt6763")
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA/mt6763
else ifeq ($(CONFIG_MTK_PLATFORM), "mt6771")
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA/mt6771
else ifeq ($(CONFIG_MTK_PLATFORM), "mt6775")
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/ComboA/mt6775
else
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/$(MTK_PLATFORM)/$(MTK_PLATFORM)
endif

ifeq ($(CONFIG_TRUSTONIC_TEE_SUPPORT),y)
include $(srctree)/drivers/misc/mediatek/gud/Makefile.include
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/rpmb/drrpmb/public
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/rpmb/drrpmb_gp/public
ccflags-y += -I$(srctree)/drivers/mmc/host/mediatek/rpmb/tlrpmb/public
endif

##zwl add for pingbo tee
ifeq ($(CONFIG_TRUSTKERNEL_TEE_SUPPORT),y)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/tkcore/include/linux
endif

#
# Attention:
# Enabling/using CFG_RPMB_KEY_PROGRAMED_IN_KERNEL means accepting the risk of revealing key in
# Linux Kernel. Mediatek won¡¦t take the responsibility for loss incurred by the key revealing.
#
#ccflags-y += -DCFG_RPMB_KEY_PROGRAMED_IN_KERNEL

ifeq ($(CONFIG_MTK_HW_FDE_AES),y)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/fde_aes/$(MTK_PLATFORM)
endif

obj-$(CONFIG_RPMB) += rpmb-mtk.o
endif

obj-$(CONFIG_RPMB) += rpmb.o
rpmb-objs += core.o
rpmb-$(CONFIG_RPMB_INTF_DEV) += cdev.o
obj-$(CONFIG_RPMB_SIM) += rpmb_sim.o

ccflags-y += -D__CHECK_ENDIAN__
