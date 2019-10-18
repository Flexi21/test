#!/bin/bash
#export MTK_SECURITY_BOOT_SUPPORT=yes
##customer requirement begin 20150408

##added by xen for flashlist, 20150623
YK_FLASH_LIST=(
   "3204"
   "3204DDR3"
   "6404"
   "6404DDR3"
   "6408"
   "6408DDR3"
   "6416DDR3"
   "12808"
   "12808_DIS"
   "25616_DIS"
   "12816"
   "12816_12808_CAIFU9"
   "12824"
   "25624"
   "51224"
)

#project lists:
YK_PROJECT_NAME_LIST=(
   "YK828_EMMC_YKQ_FWVGA"
)

YK_PROJECT_NAME_LIST_yk736=(
   "YK736_EMMC_YKQ_FWVGA"
   "YK736_EMMC_YKQ_HD640_CT6M"
   "YK736_EMMC_YKQ_FWVGA_PLUS_CT6M"
   "YK736_EMMC_CAIFU9_KS937_HDPLUS"
   "YK736_EMMC_CAIFU9_KS927_HD640"
   "YK736_EMMC_CAIFU9_KS927_HDPLUS"
   "YK736_EMMC_CAIFU9_KS907E_HDPLUS"
   "YK736_EMMC_CAIFU9_KS907_HD640"
   "YK736_EMMC_CAIFU9_KS907_HDPLUS"
   "YK736_EMMC_FULING_E727_HD720"
   "YK736_EMMC_FULING_E729_HD640"
   "YK736_EMMC_KUANGRE_A17_FWVGA_PLUS"
   "YK736_EMMC_KUANGRE_A17_HDPLUS"
   "YK736_EMMC_XINDIHENG_M130_HD720"
   "YK736_EMMC_YIZHANTONG_L5501_HD640"
   "YK736_EMMC_XINJIDE_G1_HDPLUS"
)

YK_PROJECT_NAME_LIST_yk739=(
   "YK739_EMMC_YKQ_FWVGA"
   "YK739_EMMC_YKQ_HD720"
   "YK739_EMMC_CHANGSHENGTAI_H209_HDPLUS"
)

YK_PROJECT_NAME_LIST_yk568=(
   "YK568_EMMC_YKQ_FWVGA"
   "YK568_EMMC_YKQ_HD720"
   "YK568_EMMC_SHUANGHOU_WVGA"
)

YK_PROJECT_NAME_LIST_yk737=(
   "YK737_EMMC_YKQ_FWVGA"
   "YK737_EMMC_CAIFU9_KS972A_FWPLUS"
   "YK737_EMMC_CAIFU9_KS972B_FWPLUS"
)

###################################
###Note: below there are three places of "exit 1", if parameter is 1, compile process will stop, if it is 0,compile process will continue using ProjectConfig.mk 
###      of compiling project
###################################

#cust_path="mediatek/cust_req"
# 需要生成的目的配置文件
#cust_file="mediatek/cust_req/cust_req.mk"
# 生成配置文件需要的源客制化文件
#main_cust="mediatek/cust_req/cust_project.mk"


#function declaring
declare -a _inlist
function select_choice()
{
	_target_arg=$1
	_arg_list=(${_inlist[@]})
	_outc=""
	select _c in ${_arg_list[@]}
	do
		if [ -n "$_c" ]; then
			_outc=$_c
			break
		else
			for _i in ${_arg_list[@]}
			do
				_t=`echo $_i | grep -E "^$REPLY"`
				if [ -n "$_t" ]; then
					_outc=$_i
					break
				fi
			done
			if [ -n "$_outc" ]; then
				break
			fi
		fi
	done
	if [ -n "$_outc" ]; then
	eval "$_target_arg=$_outc"
	fi
}

EnviromentSet()
{
	source mbldenv.sh
	source build/envsetup.sh
}

ProjectSelect()
{
if [ -f "./myProjectNum" ];then
	myProjectNum=$(head -1 ./myProjectNum)
	rm ./myProjectNum
	lunch $myProjectNum
else
	echo "./myProjectNum not found"
	lunch
fi
}

CustReqSelect()
{
        echo "board_project_name=$YKQ_BOARD_PROJECT_NAME"
        ##fork (bash build/cust_req.sh $YKQ_BOARD_PROJECT_NAME)
# myProjectName at least appears 2 times in cust_project.mk
min_times=2

# get ProjectName from cmdline
parameter=$YKQ_BOARD_PROJECT_NAME

prj_folder_name=${parameter##*full_}
prj_folder_name=${prj_folder_name%%-eng}
prj_folder_name=${prj_folder_name%%-user*}

echo "prj_folder_name=$prj_folder_name"

# 需要生成的目的配置文件
#cust_file="mediatek/config/cust_req/cust_req.mk"
def_file="device/mediateksample/$prj_folder_name/ProjectConfig_def.mk"
cust_file="device/mediateksample/$prj_folder_name/ProjectConfig.mk"
# 生成配置文件需要的源客制化文件
main_cust="mediatek/cust_req/cust_project.mk"

if [ -f "$cust_file" ]; then
 rm -rf "$cust_file"
fi

if [ ! -f "$cust_file" ]; then
 touch "$cust_file"
fi

chmod 755 "$cust_file"

##copy _def mk file to ProjectConfig.mk 
cp $def_file $cust_file

## delete lcm and touchpanel,imgsensor driver first
#echo "xen===$1,$2,$3"

if [[ $1 = "" ]];then  ##if remake all,then uboot and kernel drivers are both deleted
export IS_NEED_PACK_SW="yes"
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/lcm
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/logo
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/input/touchscreen
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/power
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lcm
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/imgsensor
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/camera
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lens
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/keypad
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/alsps
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/mach/mt6735/$prj_folder_name/dct/dct/inc
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/power

rm -rf out/target/product/$prj_folder_name/obj/PRELOADER_OBJ
rm -rf out/target/product/$prj_folder_name/obj/PTGEN

###modem related
rm -rf out/target/product/$prj_folder_name/obj/ETC/mdm_layout_desc_1_*
rm -rf out/target/product/$prj_folder_name/system/vendor/etc/mddb/mdm_layout_desc_1_*

rm -rf out/target/product/$prj_folder_name/obj/ETC/em_filter_1_*
rm -rf out/target/product/$prj_folder_name/system/vendor/firmware/em_filter_1_*

rm -rf out/target/product/$prj_folder_name/obj/ETC/DbgInfo_*
rm -rf out/target/product/$prj_folder_name/system/vendor/etc/mddb/DbgInfo_*

rm -rf out/target/product/$prj_folder_name/obj/ETC/catcher_filter_1_*
rm -rf out/target/product/$prj_folder_name/system/vendor/firmware/catcher_filter_1_*

rm -rf out/target/product/$prj_folder_name/obj/ETC/md1dsp*
rm -rf out/target/product/$prj_folder_name/md1dsp*

rm -rf out/target/product/$prj_folder_name/obj/ETC/md1rom*
rm -rf out/target/product/$prj_folder_name/md1rom*

rm -rf out/target/product/$prj_folder_name/obj/ETC/MDDB*
rm -rf out/target/product/$prj_folder_name/system/vendor/etc/mddb/MDDB*
fi

if [[ $1 = "pl" ]];then
rm -rf out/target/product/$prj_folder_name/obj/PRELOADER_OBJ
fi

if [[ $1 = "lk" ]];then
#echo "xen==delete uboot lcm and logo"
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/lcm
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/logo
elif [[ $2 = "lk" ]];then
#echo "xen==delete uboot lcm and logo"
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/lcm
rm -rf out/target/product/$prj_folder_name/obj/BOOTLOADER_OBJ/build-$prj_folder_name/dev/logo
fi

if [[ $1 = "kernel" ]];then
#echo "xen==delete kernel TP-LCM-CAM-ALSPS-DCT obj"
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/input/touchscreen
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/power
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lcm
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/imgsensor
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/camera
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lens
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/keypad
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/alsps
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/mach/mt6735/$prj_folder_name/dct/dct/inc
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/power
elif [[ $2 = "kernel" ]];then
#echo "xen==delete kernel TP-LCM-CAM-ALSPS-DCT obj"
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/input/touchscreen
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/power
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lcm
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/imgsensor
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/camera
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lens
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/keypad
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/alsps
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/mach/mt6735/$prj_folder_name/dct/dct/inc
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/power
elif [[ $3 = "kernel" ]];then
#echo "xen==delete kernel TP-LCM-CAM-ALSPS-DCT obj"
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/input/touchscreen
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/power
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lcm
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/imgsensor
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/camera
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/lens
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/keypad
rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/alsps
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/mach/mt6735/$prj_folder_name/dct/dct/inc
#rm -rf out/target/product/$prj_folder_name/obj/KERNEL_OBJ/drivers/misc/mediatek/power
fi

#20151106 zwb add, remove build.prop
rm -rf out/target/product/$prj_folder_name/system/build.prop
rm -rf out/target/product/$prj_folder_name/obj/ETC/system_build_prop_intermediates/build.prop

echo "parameter=$parameter"
prj_emmc=${parameter%%_emmc*}
prj_emmc=${prj_emmc%%_67*}
prj_nand=${parameter%%_nand*}
prj_nand=${prj_nand%%_67*}
echo "prj_emmc=$prj_emmc"
echo "prj_nand=$prj_nand"

prj_name=${parameter##*full_}
echo "prj_name=$prj_name"
prj_name=${prj_name%%-eng}
prj_name=${prj_name%%-user*}
echo "prj_name=$prj_name"

echo "Please select target project name:"

case $prj_name in
	yk736_lwctg_bsp1g)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk736[@]})
	;;
	yk736_lwg_bsp1g)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk736[@]})
	;;
	yk736_lwg_bsp)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk736[@]})
	;;
	yk739_lwctg_bsp)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk739[@]})
	;;
	yk739_lwg_bsp)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk739[@]})
	;;
	yk568_lwg_bsp)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk568[@]})
	;;
	yk737_lwg_bsp)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk737[@]})
	;;
	yk737_lwg_bsp1g)
	    _inlist=(${YK_PROJECT_NAME_LIST_yk737[@]})
	;;
esac
#zwenbing add start
#echo "myProjectName = $myProjectName"

if [ -f ./myProjectName ];then
myProjectName=$(head -1 "./myProjectName")
echo "$myProjectName"
fi

if [ -z "$myProjectName" ];then
select_choice myProjectName
fi

##by yxy for mkq config
if [ "$CHOICE_BUILD_PROJECT" == "Yes" ]; then
echo buildProject=$YKQ_BOARD_PROJECT_NAME > ./build.ini
echo PROJECT=$prj_name >> ./build.ini
echo CUSTOM=$myProjectName >> ./build.ini
echo CONFIG= >> ./build.ini
exit 0
fi

#echo "myProjectName = $myProjectName"
#zwenbing add end
##ended @20140529
echo "...selected myProjectName=$myProjectName..."
# search string of myProjectName in the file cust_project.mk
temp=`grep -c "$myProjectName" "$main_cust"`
# echo "...$myProjectName appears $temp times.."

if [ $temp -lt $min_times ]; then
 echo "...$myProjectName appears no more than 1 times, please check cust_project.mk!"
 # exit 1
 exit 0
fi

# search first line number of this string's appearance
temp=`grep -n "$myProjectName##" "$main_cust" | head -1 | cut -d ":" -f 1`
line1=`expr $temp + 1`
# echo "...first_line = $line1"
# search second line number of this string's appearance
temp=`grep -n "$myProjectName##" "$main_cust" | head -2 | tail -1 | cut -d ":" -f 1`
line2=`expr $temp - 1`
# echo "...second_line = $line2"

if [ $line1 -ge $line2 ]; then
 echo "...$myProjectName requirement is Not OK, please check cust_project.mk!"
 #exit 1
 exit 0
fi

#20150207 zwb add
if [ -f ./myProjectCustom ];then
myProjectCustom=$(head -1 ./myProjectCustom)
echo "myProjectCustom = $myProjectCustom"
temp=`grep -n "${myProjectName}_PRIVATE##" "$main_cust" | head -1 | cut -d ":" -f 1`
line2=`expr $temp - 1`
#echo "myProjectCustom = $myProjectCustom"
temp=`grep -n "${myProjectName}_${myProjectCustom}##" "$main_cust" | head -1 | cut -d ":" -f 1`
line3=`expr $temp`
temp=`grep -n "${myProjectName}_${myProjectCustom}##" "$main_cust" | head -2 | tail -1 | cut -d ":" -f 1`
line4=`expr $temp`
else
	#20160622 zwb add
	temp=`grep -n "${myProjectName}_PRIVATE##" "$main_cust" | head -1 | cut -d ":" -f 1`
	echo $temp $line1 $line2
	if [ ! -z $temp ] && [ $temp -ge $line1 ]; then
	line2=`expr $temp - 1`
	fi
	#end
fi
#end


## dummy line first
echo "">>"$cust_file"
echo "">>"$cust_file"
echo "# ##############################################">>"$cust_file"
echo "# Below is auto-generated by mk.sh..">>"$cust_file"
echo "# ##############################################">>"$cust_file"

# echo "...Copy makefile from cust_project.mk.."
# echo "..search common part tail line number.."
temp=`grep -n "customized requirement begin" "$main_cust" | head -1 | cut -d ":" -f 1`
line=`expr $temp - 1`
# copy common part to device/mediateksample/ProjectNameX/ProjectConfig.mk
sed -n "1,${line}"p "$main_cust" >>"$cust_file"

#20160722 zwb add
#自动生成当前MTK版本号,方便集成mtk patch
PATCH_NAME=`grep "^MTK_BUILD_VERNO" "$cust_file" |tail -1 |cut -d "=" -f 2 |cut -d "P" -f1 ||tr -d " "`
PATCH_VER=`grep "^MTK_BUILD_VERNO" "device/mediateksample/k39tv1_bsp_1g/ProjectConfig.mk" |tail -1 |cut -d "=" -f 2|cut -d "P" -f2   |tr -d " "`
WEEK_VER=`grep "^MTK_WEEK_NO" "device/mediateksample/k39tv1_bsp_1g/ProjectConfig.mk"`
echo "###MTK SOFTWARE VERSION BEGIN###">>"$cust_file"
echo "1st PATCH_NAME=$PATCH_NAME"
PATCH_NAME=`echo $PATCH_NAME`
echo "2nd PATCH_NAME=$PATCH_NAME"
echo "2nd PATCH_VER=$PATCH_VER"

if [[ $PATCH_NAME = $PATCH_VER ]];then
echo "MTK_BUILD_VERNO = ${PATCH_NAME}">>"$cust_file"
else
echo "MTK_BUILD_VERNO = ${PATCH_NAME}_P${PATCH_VER}">>"$cust_file"
fi

echo "$WEEK_VER">>"$cust_file"
echo "##MTK SOFTWARE VERSION END###">>"$cust_file"
echo "">>"$cust_file"
#end

# copy customomized part to device/mediateksample/ProjectNameX/ProjectConfig.mk
echo "### $myProjectName customized requirement begin####################">>"$cust_file"
sed -n "${line1},${line2}"p "$main_cust" >>"$cust_file"

#20150207 zwb add
if [ -f ./myProjectCustom ];then
sed -n "${line3},${line4}"p "$main_cust" >>"$cust_file"
rm ./myProjectCustom
fi
#end 

##insert flash type and capacity config here,by xen 20150623
if [[ $1 = "pl" ]];then  ##added by xen for test 20150626
echo "Please select target Flash config:"
_inlist=(${YK_FLASH_LIST[@]})
select_choice myFlashConfig
echo "...selected myFlashConfig=$myFlashConfig..."
case $myFlashConfig in
	3204)
	    echo "YK_EMMC_CAPACITY=3204">>"$cust_file"
	;;
	3204DDR3)
	    echo "YK_EMMC_CAPACITY=3204DDR3">>"$cust_file"
	;;
	6404)
	    echo "YK_EMMC_CAPACITY=6404">>"$cust_file"
	;;
	6404DDR3)
	    echo "YK_EMMC_CAPACITY=6404DDR3">>"$cust_file"
	;;
	6408)
	    echo "YK_EMMC_CAPACITY=6408">>"$cust_file"
	;;
	6408DDR3)
	    echo "YK_EMMC_CAPACITY=6408DDR3">>"$cust_file"
	;;
	6416DDR3)
	    echo "YK_EMMC_CAPACITY=6416DDR3">>"$cust_file"
	;;
	12808)
	    echo "YK_EMMC_CAPACITY=12808">>"$cust_file"
        ;;
	12808_DIS)
	    echo "YK_EMMC_CAPACITY=12808_DIS">>"$cust_file"
        ;;
	25616_DIS)
	    echo "YK_EMMC_CAPACITY=25616_DIS">>"$cust_file"
        ;;
	12816)
	    echo "YK_EMMC_CAPACITY=12816">>"$cust_file"
        ;;
	12816_12808_CAIFU9)
	    echo "YK_EMMC_CAPACITY=12816_12808_CAIFU9">>"$cust_file"
        ;;
	12824)
	    echo "YK_EMMC_CAPACITY=12824">>"$cust_file"
        ;;
	25624)
	    echo "YK_EMMC_CAPACITY=25624">>"$cust_file"
        ;;
	51224)
	    echo "YK_EMMC_CAPACITY=51224">>"$cust_file"
        ;;
esac
elif [[ $1 = "abcdefg" ]];then #20151021 zwb removed this function
echo "Please select target Flash config:"
_inlist=(${YK_FLASH_LIST[@]})
select_choice myFlashConfig
echo "...selected myFlashConfig=$myFlashConfig..."
case $myFlashConfig in
	3204)
	    echo "YK_EMMC_CAPACITY=3204">>"$cust_file"
	;;
	3204DDR3)
	    echo "YK_EMMC_CAPACITY=3204DDR3">>"$cust_file"
	;;
	6404)
	    echo "YK_EMMC_CAPACITY=6404">>"$cust_file"
	;;
	6404DDR3)
	    echo "YK_EMMC_CAPACITY=6404DDR3">>"$cust_file"
	;;
	6408)
	    echo "YK_EMMC_CAPACITY=6408">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
	;;
	6408DDR3)
	    echo "YK_EMMC_CAPACITY=6408DDR3">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
	;;
	6416DDR3)
	    echo "YK_EMMC_CAPACITY=6416DDR3">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
	;;
	12808)
	    echo "YK_EMMC_CAPACITY=12808">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	12808_DIS)
	    echo "YK_EMMC_CAPACITY=12808_DIS">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	25616_DIS)
	    echo "YK_EMMC_CAPACITY=25616_DIS">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	12816)
	    echo "YK_EMMC_CAPACITY=12816">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	12816_12808_CAIFU9)
	    echo "YK_EMMC_CAPACITY=12816_12808_CAIFU9">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	12824)
	    echo "YK_EMMC_CAPACITY=12824">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	25624)
	    echo "YK_EMMC_CAPACITY=25624">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
	51224)
	    echo "YK_EMMC_CAPACITY=51224">>"$cust_file"
	    echo "YKQ_LOW_RAM_OPTIMIZE=no">>"$cust_file"
	    echo "CUSTOM_CONFIG_MAX_DRAM_SIZE=0x100000000">>"$cust_file"
        ;;
esac
fi

echo "### $myProjectName customized requirement end######################">>"$cust_file"
echo "">>"$cust_file"
echo "">>"$cust_file"

echo "...Complete Creation of $cust_file !"

#zwb 20180330 add 
MTK_SECURITY_BOOT_SUPPORT=`grep "^MTK_SECURITY_BOOT_SUPPORT" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
if [ "$MTK_SECURITY_BOOT_SUPPORT" = "yes" ];then
export MTK_SECURITY_BOOT_SUPPORT=yes
fi
#end

##added by xen for fingerprint dts customization 20180314--Begin
fingerprint_inc_dts="kernel-4.4/arch/arm/boot/dts/include/dt-bindings/fingerprint/fingerprint_common.dtsi"

if [ -f "$fingerprint_inc_dts" ]; then
 rm -rf "$fingerprint_inc_dts"
fi

if [ ! -f "$fingerprint_inc_dts" ]; then
 touch "$fingerprint_inc_dts"
fi

chmod 755 "$fingerprint_inc_dts"

CS2511_NAME="cs2511"
CS358_NAME="cs358"
CHIPSAILING_TEE_NAME="chipsailing"
ICN7152L_NAME="icn7152l"
ICN7152_TEE_NAME="icnt7152_tee"
ICN7XXX_NAME="icn7xxx"
SUNWAVE_NAME="sunwavecorp"
FT9338_NAME="ft9338"
MICROARRAY_NAME="microarray"

FINGERPRINT_NAME=`grep "^CUSTOM_KERNEL_FINGERPRINT" "$cust_file" |tail -1 |cut -d "=" -f 2 |cut -d "P" -f1 ||tr -d " "`
echo "FINGERPRINT_NAME=$FINGERPRINT_NAME"
if [[ $FINGERPRINT_NAME == *$CS2511_NAME* ]];then
echo "fingerprint has cs2511"
echo "#include \"chipsailing.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$CS358_NAME* ]];then
echo "fingerprint has cs358"
echo "#include \"chipsailing.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$CHIPSAILING_TEE_NAME* ]];then
echo "fingerprint has chipsailing_tee"
echo "#include \"chipsailing.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$ICN7152L_NAME* ]];then
echo "fingerprint has icn7152l"
echo "#include \"icn7152l.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$ICN7152_TEE_NAME* ]];then
echo "fingerprint has icnt7152l_tee"
echo "#include \"icn7152l.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$ICN7XXX_NAME* ]];then
echo "fingerprint has icn7xxx"
echo "#include \"icn7152l.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$SUNWAVE_NAME* ]];then
echo "fingerprint has sunwavecorp"
echo "#include \"sunwave.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$FT9338_NAME* ]];then
echo "fingerprint has ft9338"
echo "#include \"ft9338.dtsi"\">>"$fingerprint_inc_dts"
fi
if [[ $FINGERPRINT_NAME == *$MICROARRAY_NAME* ]];then
echo "fingerprint has microarray"
echo "#include \"microarray.dtsi"\">>"$fingerprint_inc_dts"
fi
###added by xen for fingerprint dts customization 20180314--End

#zwenbing add start
if [ -f ./myProjectName ];then
rm ./myProjectName
fi
#zwenbing add end

#copy custom files
	echo "$prj_folder_name" >> makeMtk.ini
	#COPY COMMON FILES
	CUSTOM_FILE_COMMON=`grep "^CUSTOM_FILES_COMMON" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
	if [ -n "$CUSTOM_FILE_COMMON" ];then 
	echo "copy common files: $CUSTOM_FILE_COMMON..."
	cp -R ./zzCustom2/$CUSTOM_FILE_COMMON/* .
	fi

    #COPY CUSTOM_FILES_PRJ_COMMON  #wwl add 20160104
    CUSTOM_FILES_PRJ_COMMON=`grep "^CUSTOM_FILES_PRJ_COMMON" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
    if [ -n "$CUSTOM_FILES_PRJ_COMMON" ];then 
    echo "copy prj common files: $CUSTOM_FILES_PRJ_COMMON..."
    cp -R ./zzCustom2/$CUSTOM_FILES_PRJ_COMMON/* .
    fi
    #COPY CUSTOM FILES

	#COPY CUSTOM FILES
	CUSTOM_FILE_DIR=`grep "^CUSTOM_FILES_BRAND" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
	if [ -n "$CUSTOM_FILE_DIR" ];then 
	echo "copy custom files: $CUSTOM_FILE_DIR..."
	cp -R ./zzCustom2/$CUSTOM_FILE_DIR/* .
	fi
	
	#COPY MEM_TEST_FILE zwl add start 20180416
	MEM_TEST_FILE=`grep "^YKQ_SPECIAL_FLASH_MEM_TEST_ZWL" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
        if [ "$MEM_TEST_FILE" = "yes" ];then
	echo "copy custom files: simple_memtest..."
	cp -R ./zzCustom2/zwl_common/simple_memtest/* .
	fi
        #COPY MEM_TEST_FILE end
	
        ##Add for finger tee start
        CUSTOM_KERNEL_FINGERPRINT=`grep "^CUSTOM_KERNEL_FINGERPRINT" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
        CUSTOM_SELECT=`grep "^YKQ_ICNT7152_TEE" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
	if [ "$CUSTOM_KERNEL_FINGERPRINT" = "icnt7152_tee" ];then 
        if [ "$CUSTOM_SELECT" = "yes" ];then 
	echo "icn7152_tee: copy custom files ICNT7152_TEE_DIFF_YK736_BSP1G ..."
	cp -R ../ICNT7152_TEE_DIFF_YK736_BSP1G/* .
        fi
	fi
        ##Add for finger tee end

	ANDROID_ICONS=`grep "^YK_LAUNCHER_DEFAULT_ANDROID" "$cust_file" |tail -1 |cut -d "=" -f 2 |tr -d " "`
	#echo $ANDROID_ICONS
	if [  "$ANDROID_ICONS" = "yes" ];then
	echo "deleting files..."
	rm -rf frameworks/base/core/res/assets/icon-xxhdpi/com.android.*.png
	rm -rf  frameworks/base/core/res/assets/icon-xxhdpi/com.google.*.png
	rm -rf  frameworks/base/core/res/assets/icon-xxhdpi/com.mediatek.*.png
	rm -rf  frameworks/base/core/res/assets/icon-xxhdpi-1920x1080/com.android.*.png
	rm -rf  frameworks/base/core/res/assets/icon-xxhdpi-1920x1080/com.google.*.png
	rm -rf  frameworks/base/core/res/assets/icon-xxhdpi-1920x1080/com.mediatek.*.png
	echo "done!"
	fi

	if [ -z "$CUSTOM_FILE_DIR" ];then 
	echo "CUSTOM_FILE_DIR is empty, skip coping"
	fi
#END 


#tom add for hualing
#COPY COMMON FILES
CUSTOM_FILE_COMMON=`grep "^HX_CUSTOM_FILES_COMMON" "$cust_file" |head -1 |cut -d "=" -f 2 |tr -d " "`
if [ -n "$CUSTOM_FILE_COMMON" ];then 
echo "copy common files: $CUSTOM_FILE_COMMON..."
cp -R ./zzCustomer/$CUSTOM_FILE_COMMON/* .
fi
#
CUSTOM_FILE_DIR=`grep "^HX_CUSTOM_FILES_BRAND" "$cust_file" |head -1 |cut -d "=" -f 2 |tr -d " "`
if [ -n "$CUSTOM_FILE_DIR" ];then 
echo "copy custom files: $CUSTOM_FILE_DIR..."
cp -R ./zzCustomer/610_pro_all/$CUSTOM_FILE_DIR/* .
fi
#
CUSTOM_FILE_DIR=`grep "^HX_CUSTOM_FILES_BRAND" "$cust_file" |head -1 |cut -d "=" -f 2 |tr -d " "`
if [ -n "$CUSTOM_FILE_DIR" ];then 
echo "copy custom files: $CUSTOM_FILE_DIR..."
cp -R ./zzCustomer/606_pro_all/$CUSTOM_FILE_DIR/* .
fi
#
CUSTOM_FILE_DIR=`grep "^HX_CUSTOM_FILES_BRAND" "$cust_file" |head -1 |cut -d "=" -f 2 |tr -d " "`
if [ -n "$CUSTOM_FILE_DIR" ];then 
echo "copy custom files: $CUSTOM_FILE_DIR..."
cp -R ./zzCustomer/628_pro_all/$CUSTOM_FILE_DIR/* .
fi
#
if [ -z "$CUSTOM_FILE_DIR" ];then 
echo "CUSTOM_FILE_DIR is empty, skip coping"
fi
#end tom add for hualing

##Begin xieen add for imgsensor config modified into defconfig files @20171013
DEFCONFIG_FILE_PATH="kernel-4.4/arch/arm/configs"
MY_DEBUG_DEFCONFIG="$DEFCONFIG_FILE_PATH/${prj_name}_debug_defconfig"
MY_DEFCONFIG="$DEFCONFIG_FILE_PATH/${prj_name}_defconfig"
echo "MY_DEBUG_DEFCONFIG =$MY_DEBUG_DEFCONFIG"
echo "MY_DEFCONFIG =$MY_DEFCONFIG"
imgsensor_str="CONFIG_CUSTOM_KERNEL_IMGSENSOR"
cust_imgsensor=`grep "^CUSTOM_KERNEL_IMGSENSOR" "$cust_file" |tail -1|cut -d "=" -f 2`
##delete unused blank
cust_imgsensor=`echo $cust_imgsensor`
new_imgsensor_str="CONFIG_CUSTOM_KERNEL_IMGSENSOR=\"$cust_imgsensor\""

imgsensor_debug=`grep -n "$imgsensor_str" "$MY_DEBUG_DEFCONFIG" | head -1 | cut -d ":" -f 1`
imgsensor_user=`grep -n "$imgsensor_str" "$MY_DEFCONFIG" | head -1 | cut -d ":" -f 1`
echo "imgsensor_debug=$imgsensor_debug, imgsensor_user=$imgsensor_user"
sed -i "$imgsensor_debug d" $MY_DEBUG_DEFCONFIG
sed -i "$imgsensor_debug i$new_imgsensor_str" $MY_DEBUG_DEFCONFIG
sed -i "$imgsensor_user d" $MY_DEFCONFIG
sed -i "$imgsensor_user i$new_imgsensor_str" $MY_DEFCONFIG
###End xieen add for imgsensor config modified into defconfig files @20171013

##xen added for CONFIG_MTK_MD1_SUPPORT modified according to ProjectConfig.mk,@20171113
md1_str="CONFIG_MTK_MD1_SUPPORT"
cust_md1_support=`grep "^MTK_MD1_SUPPORT" "$cust_file" |tail -1|cut -d "=" -f 2`
cust_md1_support=`echo $cust_md1_support`
cfg_md1_support="CONFIG_MTK_MD1_SUPPORT=$cust_md1_support"
md1_debug=`grep -n "$md1_str" "$MY_DEBUG_DEFCONFIG" | head -1 | cut -d ":" -f 1`
md1_user=`grep -n "$md1_str" "$MY_DEFCONFIG" | head -1 | cut -d ":" -f 1`
sed -i "$md1_debug d" $MY_DEBUG_DEFCONFIG
sed -i "$md1_debug i$cfg_md1_support" $MY_DEBUG_DEFCONFIG
sed -i "$md1_user d" $MY_DEFCONFIG
sed -i "$md1_user i$cfg_md1_support" $MY_DEFCONFIG
##End

##xen added for microtrust tee update 20180503
mtk_doujia_sw_str="MICROTRUST_TEE_SUPPORT"
cust_doujia_sw_support=`grep "^MICROTRUST_TEE_SUPPORT" "$cust_file" |tail -1|cut -d "=" -f 2`
cust_doujia_sw_support=`echo $cust_doujia_sw_support`
echo "doujia_sw=$cust_doujia_sw_support"

##zwl added for pingbotk tee update 20180505
mtk_pingbo_sw_str="TRUSTKERNEL_TEE_SUPPORT"
cust_pingbo_sw_support=`grep "^TRUSTKERNEL_TEE_SUPPORT" "$cust_file" |tail -1|cut -d "=" -f 2`
cust_pingbo_sw_support=`echo $cust_pingbo_sw_support`
echo "pingbo_sw=$cust_pingbo_sw_support"
##defconfig:
#doujia tee
cfg_tee_str="CONFIG_MICROTRUST_TEE_SUPPORT"
cfg_tee_on_support="CONFIG_MICROTRUST_TEE_SUPPORT=y"
cfg_tee_off_support="# CONFIG_MICROTRUST_TEE_SUPPORT is not set"
tee_debug=`grep -n "$cfg_tee_str" "$MY_DEBUG_DEFCONFIG" | head -1 | cut -d ":" -f 1`
tee_user=`grep -n "$cfg_tee_str" "$MY_DEFCONFIG" | head -1 | cut -d ":" -f 1`
echo "tee_debug=$tee_debug"
echo "tee_user=$tee_user"
#pingbo tee
cfg_pingbotee_str="CONFIG_TRUSTKERNEL_TEE_SUPPORT"
cfg_pingbotee_on_support="CONFIG_TRUSTKERNEL_TEE_SUPPORT=y"
cfg_pingbotee_off_support="# CONFIG_TRUSTKERNEL_TEE_SUPPORT is not set"

cfg_pingboteerpmb_str="CONFIG_TRUSTKERNEL_TEE_RPMB_SUPPORT"
cfg_pingboteerpmb_on_support="CONFIG_TRUSTKERNEL_TEE_RPMB_SUPPORT=y"
cfg_pingboteerpmb_off_support="# CONFIG_TRUSTKERNEL_TEE_RPMB_SUPPORT is not set"

pingbo_tee_debug=`grep -n "$cfg_pingbotee_str" "$MY_DEBUG_DEFCONFIG" | head -1 | cut -d ":" -f 1`
pingbo_tee_user=`grep -n "$cfg_pingbotee_str" "$MY_DEFCONFIG" | head -1 | cut -d ":" -f 1`
echo "cfg_pingbotee_str=$pingbo_tee_debug"
echo "cfg_pingbotee_str=$pingbo_tee_user"
pingborpmb_tee_debug=`grep -n "$cfg_pingboteerpmb_str" "$MY_DEBUG_DEFCONFIG" | head -1 | cut -d ":" -f 1`
pingborpmb_tee_user=`grep -n "$cfg_pingboteerpmb_str" "$MY_DEFCONFIG" | head -1 | cut -d ":" -f 1`
echo "cfg_pingboteerpmb_str=$pingborpmb_tee_debug"
echo "cfg_pingboteerpmb_str=$pingborpmb_tee_user"
if [[ $cust_doujia_sw_support == "yes" ]];then
echo "DOUJIA SW is YES"
if [[ $tee_user == "" ]];then
echo "$cfg_tee_on_support" >> $MY_DEFCONFIG
else
sed -i "$tee_user d" $MY_DEFCONFIG
#sed -i "$tee_user i$cfg_tee_on_support" $MY_DEFCONFIG
echo "$cfg_tee_on_support" >> $MY_DEFCONFIG
fi
if [[ $tee_debug == "" ]];then
echo "$cfg_tee_on_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$tee_debug d" $MY_DEBUG_DEFCONFIG
#sed -i "$tee_debug i$cfg_tee_on_support" $MY_DEBUG_DEFCONFIG
echo "$cfg_tee_on_support" >> $MY_DEBUG_DEFCONFIG
fi
else
if [[ $cust_pingbo_sw_support == "yes" ]];then
echo "PINGBO SW is YES"

if [[ $pingbo_tee_user == "" ]];then
echo "$cfg_pingbotee_on_support" >> $MY_DEFCONFIG
else
sed -i "$pingbo_tee_user d" $MY_DEFCONFIG
echo "$cfg_pingbotee_on_support" >> $MY_DEFCONFIG
fi
if [[ $pingbo_tee_debug == "" ]];then
echo "$cfg_pingbotee_on_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$pingbo_tee_debug d" $MY_DEBUG_DEFCONFIG
echo "$cfg_pingbotee_on_support" >> $MY_DEBUG_DEFCONFIG
fi
#rpmb
if [[ $pingborpmb_tee_user == "" ]];then
echo "$cfg_pingboteerpmb_on_support" >> $MY_DEFCONFIG
else
sed -i "$pingborpmb_tee_user d" $MY_DEFCONFIG
echo "$cfg_pingboteerpmb_on_support" >> $MY_DEFCONFIG
fi
if [[ $pingborpmb_tee_debug == "" ]];then
echo "$cfg_pingboteerpmb_on_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$pingborpmb_tee_debug d" $MY_DEBUG_DEFCONFIG
echo "$cfg_pingboteerpmb_on_support" >> $MY_DEBUG_DEFCONFIG
fi

else ###############
echo "DOUJIA SW is NO"
if [[ $tee_user == "" ]];then
echo "$cfg_tee_off_support" >> $MY_DEFCONFIG
else
sed -i "$tee_user d" $MY_DEFCONFIG
#sed -i "$tee_user i$cfg_tee_off_support" $MY_DEFCONFIG
echo "$cfg_tee_off_support" >> $MY_DEFCONFIG
fi
if [[ $tee_debug == "" ]];then
echo "$cfg_tee_off_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$tee_debug d" $MY_DEBUG_DEFCONFIG
#sed -i "$tee_debug i$cfg_tee_off_support" $MY_DEBUG_DEFCONFIG
echo "$cfg_tee_off_support" >> $MY_DEBUG_DEFCONFIG
fi

###////////////////////////////////////////////////////////////////////
if [[ $pingbo_tee_user == "" ]];then
echo "$cfg_pingbotee_off_support" >> $MY_DEFCONFIG
else
sed -i "$pingbo_tee_user d" $MY_DEFCONFIG
echo "$cfg_pingbotee_off_support" >> $MY_DEFCONFIG
fi
if [[ $pingbo_tee_debug == "" ]];then
echo "$cfg_pingbotee_off_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$tee_debug d" $MY_DEBUG_DEFCONFIG
echo "$cfg_pingbotee_off_support" >> $MY_DEBUG_DEFCONFIG
fi


if [[ $pingborpmb_tee_user == "" ]];then
echo "$cfg_pingboteerpmb_off_support" >> $MY_DEFCONFIG
else
sed -i "$pingbo_tee_user d" $MY_DEFCONFIG
echo "$cfg_pingboteerpmb_off_support" >> $MY_DEFCONFIG
fi
if [[ $pingborpmb_tee_debug == "" ]];then
echo "$cfg_pingboteerpmb_off_support" >> $MY_DEBUG_DEFCONFIG
else
sed -i "$tee_debug d" $MY_DEBUG_DEFCONFIG
echo "$ccfg_pingboteerpmb_off_support" >> $MY_DEBUG_DEFCONFIG
fi


fi

##preloader:
pl_cust_file="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/${prj_name}/${prj_name}.mk"
mtk_tee_support="MTK_TEE_SUPPORT"
microtrust_tee_support="MICROTRUST_TEE_SUPPORT"
trustkernel_tee_support="TRUSTKERNEL_TEE_SUPPORT"
mtk_tee_support_on="MTK_TEE_SUPPORT = yes"
mtk_tee_support_off="MTK_TEE_SUPPORT = no"
microtrust_tee_support_on="MICROTRUST_TEE_SUPPORT = yes"
microtrust_tee_support_off="MICROTRUST_TEE_SUPPORT = no"
trustkernel_tee_support_on="TRUSTKERNEL_TEE_SUPPORT = yes"
trustkernel_tee_support_off="TRUSTKERNEL_TEE_SUPPORT = no"

mtk_tee_support_line=`grep -n "$mtk_tee_support" "$pl_cust_file" | head -1 | cut -d ":" -f 1`
microtrust_tee_support_line=`grep -n "$microtrust_tee_support" "$pl_cust_file" | head -1 | cut -d ":" -f 1`
trustkernel_tee_support_line=`grep -n "$trustkernel_tee_support" "$pl_cust_file" | head -1 | cut -d ":" -f 1`

echo "mtk_tee_support_line=$mtk_tee_support_line"
echo "microtrust_tee_support_line=$microtrust_tee_support_line"
echo "trustkernel_tee_support_line=$trustkernel_tee_support_line"

if [[ $cust_doujia_sw_support == "yes" ]];then
echo "preloader DOUJIA SW is YES"
sed -i "$mtk_tee_support_line d" $pl_cust_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_on" $pl_cust_file
sed -i "$microtrust_tee_support_line d" $pl_cust_file
sed -i "$microtrust_tee_support_line i$microtrust_tee_support_on" $pl_cust_file
else
if [[ $cust_pingbo_sw_support == "yes" ]];then
sed -i "$mtk_tee_support_line d" $pl_cust_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_on" $pl_cust_file
sed -i "$trustkernel_tee_support_line d" $pl_cust_file
sed -i "$trustkernel_tee_support_line i$trustkernel_tee_support_on" $pl_cust_file
##pingbo tee end  
else
echo "preloader DOUJIA SW is NO"
sed -i "$mtk_tee_support_line d" $pl_cust_file
#sed -i "$mtk_tee_support_line i$mtk_tee_support_off" $pl_cust_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_on" $pl_cust_file
sed -i "$microtrust_tee_support_line d" $pl_cust_file
sed -i "$microtrust_tee_support_line i$microtrust_tee_support_off" $pl_cust_file
sed -i "$trustkernel_tee_support_line d" $pl_cust_file
sed -i "$trustkernel_tee_support_line i$trustkernel_tee_support_off" $pl_cust_file
fi
fi
##trustzone:
trustzone_custom_file="vendor/mediatek/proprietary/trustzone/custom/build/project/${prj_name}.mk"
mtk_tee_support_line=`grep -n "$mtk_tee_support" "$trustzone_custom_file" | head -1 | cut -d ":" -f 1`
microtrust_tee_support_line=`grep -n "$microtrust_tee_support" "$trustzone_custom_file" | head -1 | cut -d ":" -f 1`
trustkernel_tee_support_line=`grep -n "$trustkernel_tee_support" "$trustzone_custom_file" | head -1 | cut -d ":" -f 1`
echo "mtk_tee_support_line=$mtk_tee_support_line"
echo "microtrust_tee_support_line=$microtrust_tee_support_line"
echo "trustkernel_tee_support_line=$trustkernel_tee_support_line"
if [[ $cust_doujia_sw_support == "yes" ]];then
sed -i "$mtk_tee_support_line d" $trustzone_custom_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_on" $trustzone_custom_file
sed -i "$microtrust_tee_support_line d" $trustzone_custom_file
sed -i "$microtrust_tee_support_line i$microtrust_tee_support_on" $trustzone_custom_file
else
if [[ $cust_pingbo_sw_support == "yes" ]];then
sed -i "$mtk_tee_support_line d" $trustzone_custom_file
#sed -i "$mtk_tee_support_line i$mtk_tee_support_off" $trustzone_custom_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_on" $trustzone_custom_file
sed -i "$trustkernel_tee_support_line d" $trustzone_custom_file
sed -i "$trustkernel_tee_support_line i$trustkernel_tee_support_on" $trustzone_custom_file
else
sed -i "$mtk_tee_support_line d" $trustzone_custom_file
sed -i "$mtk_tee_support_line i$mtk_tee_support_off" $trustzone_custom_file
sed -i "$microtrust_tee_support_line d" $trustzone_custom_file
sed -i "$microtrust_tee_support_line i$microtrust_tee_support_off" $trustzone_custom_file
sed -i "$trustkernel_tee_support_line d" $trustzone_custom_file
sed -i "$trustkernel_tee_support_line i$trustkernel_tee_support_off" $trustzone_custom_file
fi
fi
#echo "microtrust tee update finished"

#xieen add for show flash ic start 160420
parameter=$YKQ_BOARD_PROJECT_NAME
prj_folder_name=${parameter##*full_}
prj_folder_name=${prj_folder_name%%-eng}
prj_folder_name=${prj_folder_name%%-user*}
PROJ_NAME=$prj_folder_name
YK_EMMC_CAPACITY=`grep "^YK_EMMC_CAPACITY" "$cust_file" |tail -1|cut -d "=" -f 2 |tr -d " "`
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice.h"
if [ "$YK_EMMC_CAPACITY" = "3204" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice.h"
fi
if [ "$YK_EMMC_CAPACITY" = "3204DDR3" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_3204DDR3.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6408" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6408.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6408DDR3" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6408DDR3.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6408DDR3_12816" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6408DDR3_12816.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6416DDR3" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6416DDR3.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6408DDR3_DIS" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6408DDR3_DIS.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6408DDR3_DIS_2256" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6408DDR3_DIS_2256.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6404" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6404.h"
fi
if [ "$YK_EMMC_CAPACITY" = "6404DDR3" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_6404DDR3.h"
fi
if [ "$YK_EMMC_CAPACITY" = "12808" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_12808.h"
fi
if [ "$YK_EMMC_CAPACITY" = "12808_DIS" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_12808_DIS.h"
fi
if [ "$YK_EMMC_CAPACITY" = "25616_DIS" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_25616_DIS.h"
fi
if [ "$YK_EMMC_CAPACITY" = "12816" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_12816.h"
fi
if [ "$YK_EMMC_CAPACITY" = "12816_12808_CAIFU9" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_12816_12808_CAIFU9.h"
fi
if [ "$YK_EMMC_CAPACITY" = "12824" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_12824.h"
fi
if [ "$YK_EMMC_CAPACITY" = "25624" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_25624.h"
fi
if [ "$YK_EMMC_CAPACITY" = "51224" ];then
cust_file_mem="vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/$PROJ_NAME/inc/custom_MemoryDevice_51224.h"
fi
echo "cust_file_mem = $cust_file_mem"

flash=`grep "^#define CS_PART_NUMBER" "$cust_file_mem" |head -10|cut -d "]" -f 2 |tr -d " "`
echo "flash:$flash"

echo "$flash" >./packages/apps/DriverInfo/flashinfo

##auto lcm list:
AUTO_LCM_LIST=`grep "^CUSTOM_KERNEL_LCM" "$cust_file" |tail -1|cut -d "=" -f 2`
echo "auto_lcm_list = $AUTO_LCM_LIST"
echo -e "AutoLCD:\n$AUTO_LCM_LIST" >./packages/apps/DriverInfo/auto_lcm

##auto imgsensor list:
AUTO_CAMERA_LIST=`grep "^CUSTOM_HAL_IMGSENSOR" "$cust_file" |tail -1|cut -d "=" -f 2`
echo "auto_camera_list = $AUTO_CAMERA_LIST"
echo -e "AutoCam:\n$AUTO_CAMERA_LIST" >./packages/apps/DriverInfo/auto_cam

##customer config:
CUSTOMER_CFG=`grep "^YK_CUSTOMER_CONFIG" "$cust_file" |tail -1|cut -d "=" -f 2`
echo "customer_config = $CUSTOMER_CFG"
echo "$CUSTOMER_CFG" >./packages/apps/DriverInfo/customer_config
}

#JavaversionSet()
#{
#	PATH=/usr/lib/jvm/java-1.7.0-openjdk-amd64/bin:$PATH
#}

RunCommand()
{
  if [ "$SKIP_BUILD_COMMAND" != "Yes" ]; then
#	make MTK_BUILD_ROOT=yes -j16 $* 2>&1 | tee build.log
	make -j32 $* 2>&1 | tee build.log
  fi
}

##added by xen for test 20150414
PackSoftware()
{
   echo "xieen pack software bin"
   current_path=$PWD
   #echo "current_path=$current_path"

   parameter=$YKQ_BOARD_PROJECT_NAME

   prj_folder_name=${parameter##*full_}
   prj_folder_name=${prj_folder_name%%-eng}
   prj_folder_name=${prj_folder_name%%-user*}

   BASE_NAME=$current_path
   PROJ_NAME=$prj_folder_name
   DATE_TIME=$(date +%y%m%d_%H%M)
   DIR_FOLDER=$PROJ_NAME"_"$DATE_TIME
   #echo "DIR_FOLDER=$DIR_FOLDER"
   #echo "PROJ_NAME=$PROJ_NAME"

   mkdir $DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/preloader_$PROJ_NAME.bin $BASE_NAME/$DIR_FOLDER
if [ "$MTK_SECURITY_BOOT_SUPPORT" == "yes" ];then
cp -r $BASE_NAME/out/target/product/$PROJ_NAME/logo-verified.bin $BASE_NAME/$DIR_FOLDER/logo.bin
cp -r $BASE_NAME/out/target/product/$PROJ_NAME/lk-verified.img $BASE_NAME/$DIR_FOLDER/lk.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/loader_ext-verified.img $BASE_NAME/$DIR_FOLDER/loader_ext.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/mcupmfw-verified.img $BASE_NAME/$DIR_FOLDER/mcupmfw.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/spmfw-verified.img $BASE_NAME/$DIR_FOLDER/spmfw.img
else 
   cp $BASE_NAME/out/target/product/$PROJ_NAME/logo.bin $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/lk.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/loader_ext.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/mcupmfw.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/spmfw.img $BASE_NAME/$DIR_FOLDER
fi
#end  
if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB.META.ODB_MT6739*_1_*_n.XML.GZ ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB.META.ODB_MT6739*_1_*_n.XML.GZ $BASE_NAME/$DIR_FOLDER
fi
if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB.META_MT6739*_1_*_n.EDB ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB.META_MT6739*_1_*_n.EDB $BASE_NAME/$DIR_FOLDER
fi
if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB_InfoCustomAppSrcP_MT6739*_1_*_n.EDB ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/system/etc/mddb/MDDB_InfoCustomAppSrcP_MT6739*_1_*_n.EDB $BASE_NAME/$DIR_FOLDER
fi

if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB.META.ODB_MT6739*_1_*_n.XML.GZ ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB.META.ODB_MT6739*_1_*_n.XML.GZ $BASE_NAME/$DIR_FOLDER
fi
if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB.META_MT6739*_1_*_n.EDB ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB.META_MT6739*_1_*_n.EDB $BASE_NAME/$DIR_FOLDER
fi
if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB_InfoCustomAppSrcP_MT6739*_1_*_n.EDB ];then
   cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor/etc/mddb/MDDB_InfoCustomAppSrcP_MT6739*_1_*_n.EDB $BASE_NAME/$DIR_FOLDER
fi

   cp $BASE_NAME/out/target/product/$PROJ_NAME/obj/CGEN/APDB_MT6739_S01_alps-trunk-o1.*_W??.?? $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor/data/misc/ProjectConfig.mk $BASE_NAME/$DIR_FOLDER
#wwl add
cp -r $BASE_NAME/out/target/product/$PROJ_NAME/system/build.prop $BASE_NAME/$DIR_FOLDER

   cp $BASE_NAME/out/target/product/$PROJ_NAME/MT67*_Android_scatter.txt $BASE_NAME/$DIR_FOLDER

if [ "$MTK_SECURITY_BOOT_SUPPORT" == "yes" ];then
cp $BASE_NAME/out/target/product/$PROJ_NAME/boot-verified.img $BASE_NAME/$DIR_FOLDER/boot.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/recovery-verified.img $BASE_NAME/$DIR_FOLDER/recovery.img
#cp $BASE_NAME/out/target/product/$PROJ_NAME/secro.img $BASE_NAME/$DIR_FOLDER
cp $BASE_NAME/out/target/product/$PROJ_NAME/system.img $BASE_NAME/$DIR_FOLDER
cp $BASE_NAME/out/target/product/$PROJ_NAME/userdata.img $BASE_NAME/$DIR_FOLDER
#cp $BASE_NAME/out/target/product/$PROJ_NAME/trustzone-verified.bin $BASE_NAME/$DIR_FOLDER/trustzone.bin
cp $BASE_NAME/out/target/product/$PROJ_NAME/cache.img $BASE_NAME/$DIR_FOLDER
cp $BASE_NAME/out/target/product/$PROJ_NAME/odmdtbo-verified.img $BASE_NAME/$DIR_FOLDER/odmdtbo.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor.img $BASE_NAME/$DIR_FOLDER
cp $BASE_NAME/out/target/product/$PROJ_NAME/tee-verified.img $BASE_NAME/$DIR_FOLDER/tee.img

cp $BASE_NAME/out/target/product/$PROJ_NAME/md1arm7-verified.img $BASE_NAME/$DIR_FOLDER/md1arm7.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/md1dsp-verified.img $BASE_NAME/$DIR_FOLDER/md1dsp.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/md1img-verified.img $BASE_NAME/$DIR_FOLDER/md1img.img
cp $BASE_NAME/out/target/product/$PROJ_NAME/md3img.img $BASE_NAME/$DIR_FOLDER/md3img.img

else
   cp $BASE_NAME/out/target/product/$PROJ_NAME/boot.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/recovery.img $BASE_NAME/$DIR_FOLDER
##   cp $BASE_NAME/out/target/product/$PROJ_NAME/secro.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/system.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/userdata.img $BASE_NAME/$DIR_FOLDER
##   cp $BASE_NAME/out/target/product/$PROJ_NAME/trustzone.bin $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/cache.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/odmdtbo.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/vendor.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/tee.img $BASE_NAME/$DIR_FOLDER

   cp $BASE_NAME/out/target/product/$PROJ_NAME/md1arm7.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/md1dsp.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/md1img.img $BASE_NAME/$DIR_FOLDER
   cp $BASE_NAME/out/target/product/$PROJ_NAME/md3img.img $BASE_NAME/$DIR_FOLDER
fi


##pack vmlinux file to folder by xen 20160524
   cp $BASE_NAME/out/target/product/$PROJ_NAME/obj/KERNEL_OBJ/vmlinux $BASE_NAME/$DIR_FOLDER

##pack flashinfo file to folder by zwl 20180414
  if [ -f $BASE_NAME/out/target/product/$PROJ_NAME/system/flashinfo ];then
     cp $BASE_NAME/out/target/product/$PROJ_NAME/system/flashinfo $BASE_NAME/$DIR_FOLDER
  fi

   echo "copy done!!!!!!!!!"
}

if [[ $1 = "bootimage" ]];then  ## if bootimage,no need to reselect custreq
EnviromentSet
ProjectSelect
#JavaversionSet
RunCommand $*
else
EnviromentSet
ProjectSelect
CustReqSelect $*
#JavaversionSet
RunCommand $*
fi

#######################################################################
temp=`grep -c "#### build completed successfully" build.log`
if [ "$temp" == "1" ] && [ "$MTK_SECURITY_BOOT_SUPPORT" == "yes" ] && [ "$SKIP_BUILD_COMMAND" != "Yes" ]; then
./vendor/mediatek/proprietary/scripts/sign-image/sign_image.sh 2>&1 | tee build_signimage.log
fi
if [[ $IS_NEED_PACK_SW = "yes" ]];then 
#if [[ $IS_BUILD_SUCCESS = 1 ]];then
if [[ $IS_SKIP_PACK_SW = "yes" ]];then 
echo "skip auto pack software"
else
echo "begin pack software..."
PackSoftware
fi
#fi
fi

./prebuilts/sdk/tools/jack-admin kill-server
