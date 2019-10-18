#ifdef FT_DRM_KEY_MNG_TRUSTONIC_FEATURE
#include "Keyinstall.h"
#include <vendor/mediatek/hardware/keyinstall/1.0/IKeyinstall.h>
using namespace vendor::mediatek::hardware::keyinstall::V1_0;
#endif

#include <string.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <cutils/properties.h>

#include <fcntl.h>
#include <unistd.h>

#include <cutils/sockets.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <dlfcn.h>
#include <time.h>
#include "sysenv_utils.h"

#include "FtModule.h"
#include "LogDefine.h"
#include "Context.h"
#include "PortInterface.h"
#include "PortHandle.h"
#include "hardware/ccci_intf.h"
#include "wifi_api.h"
#include "MSocket.h"

#ifdef MICROTRUST_FACTORY_SUPPORT
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/sem.h>
#define MICROTRUST_USE_COMMAND_ID       251
#define BUFFER_MAX_SIZE                 2000
#define DATA_OUT_BUFFER_SIZE            1998
#define WRITE_BUFFER_TO_PHONE           0
#define READ_FILE_TO_BUFFER             1
#define CHANGE_PHONE_FILE_PERMISSION    2
#define CALL_PHONE_PROGRAM              4
#define TEST_FILE_EXIST                 5
#define GET_FILE_LENGTH                 6
#define REMOVE_FILE                     7
#define CHECK_INIT_THH_STATUS           8
#define GET_FILENAME_BY_SUFFIX          10
#define CHECK_SOTER_STATUS              11
#define CHECK_ACTIVE_STATUS             13
static int  handle_spmeta_request(unsigned char *data_in, int data_in_len, char *peer_buf, int *peer_buff_size);
#endif

#ifdef TRUSTKERNEL_TEE_META
#include "TrustkernelExternalCommand.h"
#endif

#define SIM_SWITCH_MODE_FILE  "/sys/mtk_ssw/mode"
#define SIM_SWITCH_MODE_STR_LEN 8

#define MTK_MDDB_PATH 	"system/etc/mddb"
#define MTK_APDB_PATH   "system/etc/apdb"

#ifndef FT_CNF_OK
#define FT_CNF_OK 0
#endif
#ifndef FT_CNF_FAIL
#define FT_CNF_FAIL 1
#endif

#if defined(__LP64__)
#define CCAP_LIB_PATH "/system/lib64/libccap.so"
#else
#define CCAP_LIB_PATH "/system/lib/libccap.so"
#endif

#ifdef FT_WIFI_FEATURE

FtModWifi::FtModWifi(void)
	:CmdTarget(FT_WIFI_REQ_ID)
{
}

FtModWifi::~FtModWifi(void)
{
}

void FtModWifi::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_WIFI_OP ");
    META_WIFI_OP((FT_WM_WIFI_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_GPS_FEATURE

FtModGPS::FtModGPS(void)
	:CmdTarget(FT_GPS_REQ_ID)
{
}

FtModGPS::~FtModGPS(void)
{
}

void FtModGPS::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_GPS_OP ");

    int ATM_Test = ((NORMAL_BOOT == getBootMode()) && (1 == getNormalModeTestFlag()))? 1:0;
    META_GPS_OP((GPS_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen(), ATM_Test);
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////

#ifdef FT_NFC_FEATURE
FtModNFC::FtModNFC(void)
	:CmdTarget(FT_NFC_REQ_ID)
{
}

FtModNFC::~FtModNFC(void)
{
}

void FtModNFC::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NFC_OP ");

	if(getInitState())
    META_NFC_OP((NFC_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}

int FtModNFC::init(Frame*)
{
	if (META_NFC_init() != 0)
	{	
		 META_NFC_deinit();
		 return false;
	}
	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_FM_FEATURE

FtModFM::FtModFM(void)
	:CmdTarget(FT_FM_REQ_ID)
{
}

FtModFM::~FtModFM(void)
{

}

void FtModFM::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_FM_OP ");
    META_FM_OP((FM_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}

int FtModFM::init(Frame*)
{
	META_FM_init();
	return true;

}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_BT_FEATURE

FtModBT::FtModBT(void)
	:CmdTarget(FT_BT_REQ_ID)
{
}

FtModBT::~FtModBT(void)
{
}

void FtModBT::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_BT_OP ");

    META_BT_OP((BT_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());

}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_AUDIO_FEATURE

FtModAudio::FtModAudio(void)
	:CmdTarget(FT_L4AUD_REQ_ID)
{
}

FtModAudio::~FtModAudio(void)
{
}

void FtModAudio::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_Audio_OP ");

    META_Audio_OP((FT_L4AUD_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}

int FtModAudio::init(Frame*)
{
	META_Audio_init();
	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_CCAP_FEATURE

FtModCCAP::FtModCCAP(void)
	:CmdTarget(FT_CCT_REQ_ID)
{
}

FtModCCAP::~FtModCCAP(void)
{
}

void FtModCCAP::exec(Frame *pFrm)
{
    static void *CcapHwhndl = NULL;
    
    static int (*func_ccap_init)() = NULL;
    static void (*func_ccap_op)(const void*, void*, void**, unsigned short*, void**, unsigned short*) = NULL;
    static void (*func_ccap_const)(int*, int*, int*, int*, int*) = NULL;
    static void (*func_ccap_set_error)(const void*, void*) = NULL;
    static int (*func_ccap_get_req_op)(const void*) = NULL;
    
    static int cnf_size = 0;
    static int FT_CCT_OP_SUBPREVIEW_LCD_START = 0;
    static int FT_CCT_OP_SUBPREVIEW_LCD_STOP = 0;
    
    if(CcapHwhndl == NULL)
    {
        CcapHwhndl = dlopen(CCAP_LIB_PATH, RTLD_NOW);
        
        if(CcapHwhndl == NULL)
        {
            META_LOG("[Meta][FT] link libccap.so fail ");	
            return;
        }
        else
        {
            func_ccap_init = (int(*)()) dlsym(CcapHwhndl, "intf_ccap_init");
            func_ccap_op = (void(*)(const void*, void*, void**, unsigned short*, void**, unsigned short*)) dlsym(CcapHwhndl, "intf_ccap_op");
            func_ccap_const = (void(*)(int*, int*, int*, int*, int*)) dlsym(CcapHwhndl, "intf_ccap_const");
            func_ccap_set_error = (void(*)(const void*, void*)) dlsym(CcapHwhndl, "intf_ccap_set_error");
            func_ccap_get_req_op = (int(*)(const void*)) dlsym(CcapHwhndl, "intf_ccap_get_req_op");
            if (func_ccap_init == NULL || func_ccap_op == NULL || 
                func_ccap_const == NULL || func_ccap_set_error == NULL ||
                func_ccap_get_req_op == NULL )
            {
                META_LOG("[Meta][FT] link libccap.so function pointers fail ");	
                dlclose(CcapHwhndl);
                CcapHwhndl = NULL;
                return;
            }

            func_ccap_const(NULL, &cnf_size, NULL, &FT_CCT_OP_SUBPREVIEW_LCD_START, &FT_CCT_OP_SUBPREVIEW_LCD_STOP);
        }
    }
    
	CmdTarget::exec(pFrm);
   	
	void *req = (void*)pFrm->localBuf();
	int req_op = func_ccap_get_req_op( req );
	if ((req_op != FT_CCT_OP_SUBPREVIEW_LCD_START) && (req_op !=FT_CCT_OP_SUBPREVIEW_LCD_STOP))
	{
		META_LOG("[Meta][FT] META_CCAP_init ");	
		if (func_ccap_init() == 0)
		{
            void *cnf = malloc(cnf_size);
            if(cnf == NULL)
            {
			    META_LOG("[Meta][FT] FT_CCAP_OP META_CCT_init Fail, malloc Fail. ");
                return;
            }
	        memset(cnf, 0, cnf_size);

            func_ccap_set_error(req, cnf);
			WriteDataToPC(cnf, cnf_size, NULL, 0);

            free(cnf);

			META_LOG("[Meta][FT] FT_CCAP_OP META_CCT_init Fail ");
			return;
		}
	}
	else
    {
   		META_LOG("[Meta][FT] Now is sub Camera, init will be do later");	
   	}

	META_LOG("[Meta][FT] FT_CCAP_OP ");

#if 0
    func_ccap_op( (const void*)req, (void*)pFrm->peerBuf() );
#else
    //sned data & free buffer in meta 
    void *localBuf = NULL, *peerBuf = NULL;
    unsigned short localBufLen = 0, peerBufLen = 0;
    
    func_ccap_op( (const void*)req, (void*)pFrm->peerBuf(), &localBuf, &localBufLen, &peerBuf, &peerBufLen );    
    WriteDataToPC(localBuf, localBufLen, peerBuf, peerBufLen);    
    free(localBuf);
    free(peerBuf);    
#endif
    
    //dlclose(CcapHwhndl);    
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////
	
#ifdef FT_HDCP_FEATURE

FtModHDCP::FtModHDCP(void)
	:CmdTarget(FT_HDCP_REQ_ID)
{
}

FtModHDCP::~FtModHDCP(void)
{
}

void FtModHDCP::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);	
  
	if(getInitState())
		META_HDCP_OP((HDCP_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());  
}

int FtModHDCP::init(Frame *pFrm)
{ 
	META_LOG("[Meta][FT] Ft_HDCP");
	HDCP_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(ft_cnf));
	static int bInitFlagHDCP = false;
	HDCP_REQ *req = (HDCP_REQ *)pFrm->localBuf();

	if (false == bInitFlagHDCP)
	{
		// initial the HDCP module when it is called first time
		if (!META_HDCP_init())
		{
			ft_cnf.header.id = req->header.id +1;
			ft_cnf.header.token = req->header.token;
			ft_cnf.op= req->op;
			ft_cnf.status = META_FAILED;
			WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);
			return false;
		}
		bInitFlagHDCP = true;
	}

	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_DRM_KEY_MNG_FEATURE
FtModDRM::FtModDRM(void)
	:CmdTarget(FT_DRMKEY_REQ_ID)
{
}

FtModDRM::~FtModDRM(void)
{

}

void FtModDRM::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

#ifdef FT_DRM_KEY_MNG_TRUSTONIC_FEATURE
	//phone drmkey solution
	META_LOG("[Meta][FT] Trustonic Ft_DRM");
	FT_DRMKEY_INSTALL_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(FT_DRMKEY_INSTALL_CNF));
	//init the header
	ft_cnf.header.id = pFrm->getCmdTarget()->getId() + 1;
	ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
	ft_cnf.status = DRMKEY_INSTALL_FAIL;

	android::hardware::hidl_vec<uint8_t> data;
	HIDL_FT_DRMKEY_INSTALL_REQ toServer_req;
	HIDL_FT_DRMKEY_INSTALL_CNF toServer_cnf;

	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->header.token: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->header.token);
	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->header.id: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->header.id);
	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->op: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->op);
	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->cmd.set_req.file_size: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->cmd.set_req.file_size);
	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->cmd.set_req.stage: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->cmd.set_req.stage);
	META_LOG("[DRMKEY_HIDL] pFrm->localBuf())->cmd.query_req.req: %d\n", ((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf())->cmd.query_req.req);
	META_LOG("[DRMKEY_HIDL] =========================================================\n");
	META_LOG("[DRMKEY_HIDL] ft_cnf.header.token: %d\n", ft_cnf.header.token);
	META_LOG("[DRMKEY_HIDL] ft_cnf.header.id: %d\n", ft_cnf.header.id);
	META_LOG("[DRMKEY_HIDL] ft_cnf.op: %d\n", ft_cnf.op);
	META_LOG("[DRMKEY_HIDL] ft_cnf.status: %d\n", ft_cnf.status);
	META_LOG("[DRMKEY_HIDL] ft_cnf.result.set_cnf.result: %d\n", ft_cnf.result.set_cnf.result);
	META_LOG("[DRMKEY_HIDL] ft_cnf.result.keyresult.keycount %d\n", ft_cnf.result.keyresult.keycount);

	convertREQ2HIDL((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf(), &toServer_req);

	memset(&toServer_cnf, 0, sizeof(HIDL_FT_DRMKEY_INSTALL_CNF));
	convertCNF2HIDL(&ft_cnf, &toServer_cnf);

	META_LOG("[DRMKEY_HIDL] start HIDL");
	android::sp<IKeyinstall> client = IKeyinstall::getService();
	META_LOG("[DRMKEY_HIDL] getService done");

	data.setToExternal(pFrm->peerBuf(), pFrm->peerLen());
	META_LOG("[DRMKEY_HIDL] data.setToExternal done");

	auto callback = [&] (const HIDL_FT_DRMKEY_INSTALL_CNF& fromServer_cnf) {
		convertCNF2nonHIDL(&fromServer_cnf, &ft_cnf);
		META_LOG("[DRMKEY_HIDL] from server cnf\n");
		META_LOG("[DRMKEY_HIDL] ft_cnf.header.token: %d\n", ft_cnf.header.token);
		META_LOG("[DRMKEY_HIDL] ft_cnf.header.id: %d\n", ft_cnf.header.id);
		META_LOG("[DRMKEY_HIDL] ft_cnf.op: %d\n", ft_cnf.op);
		META_LOG("[DRMKEY_HIDL] ft_cnf.status: %d\n", ft_cnf.status);
		META_LOG("[DRMKEY_HIDL] ft_cnf.result.set_cnf.result: %d\n", ft_cnf.result.set_cnf.result);
		META_LOG("[DRMKEY_HIDL] ft_cnf.result.keyresult.keycount %d\n", ft_cnf.result.keyresult.keycount);
	};

	client->meta_drmkey_install_op(toServer_req, toServer_cnf, data, pFrm->peerLen(), callback);
	META_LOG("[DRMKEY_HIDL] hidl_meta_drmkey_install_op done");
	WriteDataToPC(&ft_cnf, sizeof(FT_DRMKEY_INSTALL_CNF), NULL, 0);
	META_LOG("[DRMKEY_HIDL] WriteDataToPC done");
#else
	//tablet drmkey solution
	META_LOG("[Meta][FT] Inhouse Ft_DRM");
	META_DRMKEY_INSTALL_OP((FT_DRMKEY_INSTALL_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
#endif

}
#endif

#ifdef FT_GAMMA_FEATURE
FtModGAMMA::FtModGAMMA(void)
    :CmdTarget(FT_GAMMA_REQ_ID)
{
}
FtModGAMMA::~FtModGAMMA(void)
{
}
void FtModGAMMA::exec(Frame *pFrm)
{
    CmdTarget::exec(pFrm);
    META_LOG("[Meta][FT] FT_GAMMA_OP ");
    META_GAMMA_OP((GAMMA_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////+

#ifdef FT_ATTESTATION_KEY_FEATURE
FtModAttestationKey::FtModAttestationKey(void)
	:CmdTarget(FT_ATTESTATIONKEY_REQ_ID)
{

}

FtModAttestationKey::~FtModAttestationKey(void)
{

}

void FtModAttestationKey::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta] FtModAttestationKey");

	int finish = 0;
	FT_ATTESTATIONKEY_INSTALL_CNF  ft_cnf;

	memset(&ft_cnf, 0, sizeof(FT_ATTESTATIONKEY_INSTALL_CNF));
	ft_cnf.status = META_FAILED;


	FT_ATTESTATIONKEY_INSTALL_REQ *req = (FT_ATTESTATIONKEY_INSTALL_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
	ft_cnf.header.token = req->header.token;
	ft_cnf.type= req->type;

	META_LOG("[Meta] FtModAttestationKey req->type = %d", req->type);
	switch(req->type)
	{
		case FT_ATTESTATIONKEY_INSTALL_SET:
			{
				META_LOG("[Meta] FtModAttestationKey state = %d", req->cmd.set_req.stage);
				if((req->cmd.set_req.stage & 0x04) == 0x04) //KEY_BLK_EOF
					finish = 1;
				META_LOG("[Meta] call ree_import_attest_keybox to send (%d) byte.", pFrm->peerLen());
				META_LOG("[Meta] ft_cnf.status(B): %d\n", ft_cnf.status);
				META_LOG("[Meta] ft_cnf.result.set_cnf.result(B): %d\n", ft_cnf.result.set_cnf.result);
				ft_cnf.result.set_cnf.result = ree_import_attest_keybox((const uint8_t *)pFrm->peerBuf(), (const uint32_t)pFrm->peerLen(), finish);
				META_LOG("[Meta] ft_cnf.status(A): %d\n", ft_cnf.status);
				META_LOG("[Meta] ft_cnf.result.set_cnf.result(A): %d\n", ft_cnf.result.set_cnf.result);
				if(ft_cnf.result.set_cnf.result == 0)
					ft_cnf.status = META_SUCCESS;
				else
					ft_cnf.status = META_FAILED;
			}
			break;
		default:
			break;
	}

	WriteDataToPC(&ft_cnf, sizeof(FT_ATTESTATIONKEY_INSTALL_CNF), NULL, 0);
}

#endif
///////////////////////////////////////////////////////////////////////////////////////////////

#define NVRAM_PEER_MAX_LEN 2000
#define BLK_CREATE	0x01
#define BLK_WRITE	0x02
#define BLK_EOF		0x04

#ifdef FT_NVRAM_FEATURE

FtModNvramBackup::FtModNvramBackup(void)
	:CmdTarget(FT_NVRAM_BACKUP_REQ_ID)
{
}

FtModNvramBackup::~FtModNvramBackup(void)
{
}

bool FtModNvramBackup::SendNVRAMFile(unsigned char file_ID, FT_NVRAM_BACKUP_CNF* pft_cnf)
{
	int backupFd;
	int peer_buff_size = 0;
    char* peer_buf = 0;
	bool return_value = FALSE;

	backupFd = open(makepath(file_ID), O_RDWR);

	if(backupFd >= 0)
	{
		META_LOG("[Meta][FT] File%d opens succeed ! ",file_ID);
		unsigned int fileLen = getFileSize(backupFd);

		peer_buf = (char*)malloc(NVRAM_PEER_MAX_LEN);
		memset(peer_buf, 0, NVRAM_PEER_MAX_LEN);

		pft_cnf->block.stage = BLK_CREATE;
		pft_cnf->block.file_ID = file_ID;

		while(!(pft_cnf->block.stage & BLK_EOF))
		{
			peer_buff_size = read(backupFd, peer_buf, NVRAM_PEER_MAX_LEN);

			if(peer_buff_size != -1)
			{
				pft_cnf->status = META_SUCCESS;
				if(peer_buff_size == 0)
				{
					pft_cnf->block.stage |= BLK_EOF;
					META_LOG("[Meta][FT] File%d backups succeed! ",file_ID);
					pft_cnf->block.file_size = fileLen;

					close(backupFd);

					free(peer_buf);

					if(remove(makepath(file_ID)) == 0)
					{
						META_LOG("[Meta][FT] File%d DeleteFile succeed! ",file_ID);
						return_value = TRUE;
						WriteDataToPC(pft_cnf, sizeof(FT_NVRAM_BACKUP_CNF),NULL, 0);
					}
					return return_value;
				}
				else
				{
					pft_cnf->block.stage |= BLK_WRITE;
					if(peer_buff_size != NVRAM_PEER_MAX_LEN)
					{
						META_LOG("[Meta][FT] File%d backups %d data ! ",file_ID,peer_buff_size);
					}
					WriteDataToPC(pft_cnf, sizeof(FT_NVRAM_BACKUP_CNF),peer_buf, peer_buff_size);
					memset(peer_buf,0,NVRAM_PEER_MAX_LEN);
					pft_cnf->block.stage &= ~BLK_CREATE;
				}

			}
			else
			{
				pft_cnf->block.stage |= BLK_EOF;
				META_LOG("[Meta][FT] File%d backups read failed ! ", file_ID);
			}

		}

		free(peer_buf);
		close(backupFd);

	}
	else
	{
		META_LOG("[Meta][FT] File%d backups open failed ! ", file_ID);
	}

	return return_value;

}


void FtModNvramBackup::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NvramBackup_OP ");

	FT_NVRAM_BACKUP_CNF ft_cnf;
	int bFileOpResult = 0;
	memset(&ft_cnf, 0, sizeof(FT_NVRAM_BACKUP_CNF));
	//init the header
	ft_cnf.header.id = pFrm->getCmdTarget()->getId() + 1;
	ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
	ft_cnf.status = META_FAILED;

	FT_NVRAM_BACKUP_REQ *req = (FT_NVRAM_BACKUP_REQ *)pFrm->localBuf();

	if (req->count > 0)
	{
		META_LOG("[Meta][FT] Count is %d, backup parts of NvRam!", req->count);
		bFileOpResult = FileOp_BackupData_Special(req->buffer, req->count, req->mode);
	}
	else
	{
		META_LOG("[Meta][FT] Count is %d, backup all NvRam!", req->count);
		bFileOpResult = FileOp_BackupAll_NvRam();
	}
	
	if(bFileOpResult)
	{
	    META_LOG("[Meta][FT] NVM_PcBackup_Get_Data Start ! ");
		if(SendNVRAMFile(0,&ft_cnf))
		{
			META_LOG("[Meta][FT] Send file 0 succeed! ! ");
			//init the header
			ft_cnf.header.id = req->header.id +1;
			ft_cnf.header.token = req->header.token;
			ft_cnf.status = META_FAILED;
			ft_cnf.block.file_size = 0;

			if(SendNVRAMFile(1,&ft_cnf))
			{
				META_LOG("[Meta][FT] Send file 1 succeed! ! ");
				return;
			}
		}
	}
	else
	{
		META_LOG("[Meta][FT] Failed to backup NvRam!");
	}

	WriteDataToPC(&ft_cnf, sizeof(FT_NVRAM_BACKUP_CNF),NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModNvramRestore::FtModNvramRestore(void)
	:CmdTarget(FT_NVRAM_RESTORE_REQ_ID)
{
}

FtModNvramRestore::~FtModNvramRestore(void)
{
}


void FtModNvramRestore::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NvramRestore_OP ");
	
	FT_NVRAM_RESTORE_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(FT_NVRAM_RESTORE_CNF));

	FT_NVRAM_RESTORE_REQ *req = (FT_NVRAM_RESTORE_REQ *)pFrm->localBuf();
	
	//init the header
	ft_cnf.header.id = req->header.id +1;
	ft_cnf.header.token = req->header.token;
	ft_cnf.status = META_FAILED;
	
	int backupFd;
	unsigned int fileLen;
	META_LOG("[Meta][FT] FT_NVRAM_Restore_OP receive block stage %x  file id %d file size %d!",req->block.stage,req->block.file_ID,req->block.file_size);
	if(req->block.stage & BLK_CREATE)
	{
		backupFd = open(makepath(req->block.file_ID), O_RDWR | O_TRUNC | O_CREAT, 0777);
	}
	else
	{
		backupFd = open(makepath(req->block.file_ID), O_RDWR | O_APPEND);
	}
	
	if(backupFd >= 0)
	{
		META_LOG("[Meta][FT] FT_NVRAM_Restore_OP create or open file OK!");
		int sWriten = 0;
		sWriten = write(backupFd,pFrm->peerBuf(),pFrm->peerLen());

		if(sWriten>0)
		{
			ft_cnf.status = META_SUCCESS;
			META_LOG("[Meta][FT] FT_NVRAM_Restore_OP File%d write %d data total data %d!",req->block.file_ID,sWriten,pFrm->peerLen());
			if(req->block.stage & BLK_EOF)
			{
				fileLen = getFileSize(backupFd);
				if(req->block.file_size == fileLen)
				{
					META_LOG("[Meta][FT] FT_NVRAM_Restore_OP write file transfer success! ");
					close(backupFd);
					backupFd = -1;
	
					if(req->block.file_ID == 1)
					{
						if(!FileOp_RestoreAll_NvRam())
						{
							ft_cnf.status = META_FAILED;
							META_LOG("[Meta][FT] META_Editor_PcRestore_Set_Data failed! ");
	
						}
					}
				}
				else
				{
					ft_cnf.status = META_FAILED;
					META_LOG("[Meta][FT] FT_NVRAM_Restore_OP file %d size error! / %d ",req->block.file_ID,req->block.file_size);
				}
			}
		}
		else
		{
			META_LOG("[Meta][FT] FT_NVRAM_Restore_OP write file failed! sWriten =%d errno = %d",sWriten,errno);
		}
	
		if(backupFd != -1)
			close(backupFd);
	
	
	}
	else
	{
		META_LOG("[Meta][FT] FT_NVRAM_Restore_OP create or open file failed!");
	}
	
	WriteDataToPC(&ft_cnf, sizeof(FT_NVRAM_RESTORE_CNF),NULL, 0);

}



///////////////////////////////////////////////////////////////////////////////////////////////

FtModNvramReset::FtModNvramReset(void)
	:CmdTarget(FT_NVRAM_RESET_REQ_ID)
{
}

FtModNvramReset::~FtModNvramReset(void)
{
}

void FtModNvramReset::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NvramReset_OP ");
	FT_AP_Editor_reset_cnf ft_cnf;
 
    memset(&ft_cnf, 0, sizeof(FT_AP_Editor_reset_cnf));

	FT_AP_Editor_reset_req *req = (FT_AP_Editor_reset_req *)pFrm->localBuf();

    //if the reset_category and file_idx is 0xfc and 0xfccf, we reset all nvram files.
    if ((req->reset_category == 0xfc )&& (req->file_idx ==0xfccf))
        ft_cnf = META_Editor_ResetAllFile_OP(req);	//reset all files
    else
        ft_cnf = META_Editor_ResetFile_OP(req);		//reset one nvram file

    //fill the ft module header
    ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.status = META_SUCCESS;

    WriteDataToPC(&ft_cnf, sizeof(FT_AP_Editor_reset_cnf),NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////
FtModNvramRead::FtModNvramRead(void)
	:CmdTarget(FT_NVRAM_READ_REQ_ID)
{
}

FtModNvramRead::~FtModNvramRead(void)
{
}

void FtModNvramRead::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NvramRead_OP ");
 	// just call the inferface of ap_editor lib which will reture the data after reading sucessfully
 	if (!META_Editor_ReadFile_OP((FT_AP_Editor_read_req *)pFrm->localBuf()))
	 	META_LOG("[Meta][FT] FT_APEditorR_OP META Test Fail");

}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModNvramWrite::FtModNvramWrite(void)
	:CmdTarget(FT_NVRAM_WRITE_REQ_ID)
{
}

FtModNvramWrite::~FtModNvramWrite(void)
{
}

void FtModNvramWrite::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

	META_LOG("[Meta][FT] FT_NvramWrite_OP ");

    FT_AP_Editor_write_cnf ft_cnf;

    memset(&ft_cnf, 0, sizeof(FT_AP_Editor_write_cnf));
	FT_AP_Editor_write_req *req = (FT_AP_Editor_write_req *)pFrm->localBuf();

    //// just call the inferface of ap_editor lib
    ft_cnf = META_Editor_WriteFile_OP(req, (char *)pFrm->peerBuf(), pFrm->peerLen());



    //fill the ft module header
    ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.status = META_SUCCESS;

    WriteDataToPC(&ft_cnf, sizeof(FT_AP_Editor_write_cnf),NULL, 0);

}


FtModAPDB::FtModAPDB(void)
    :CmdTarget(FT_APDB_REQ_ID)
{
}

FtModAPDB::~FtModAPDB(void)
{
}

void FtModAPDB::exec(Frame *pFrm)
{
    CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] FT_APDB_OP ");

    FT_APDB_CNF ft_cnf;
    memset(&ft_cnf, 0, sizeof(FT_APDB_CNF));

    FT_APDB_REQ *req = (FT_APDB_REQ *)pFrm->localBuf();


    //fill the ft module header
    ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.type= req->type;
    ft_cnf.status = META_FAILED;

    char szAPDBPath[64] = {MTK_APDB_PATH};

    switch(req->type)
    {
        case FT_APDB_OP_QUERYPATH:
            memcpy(ft_cnf.result.query_apdbpath_cnf.apdb_path, szAPDBPath, strlen(szAPDBPath));
            META_LOG("[Meta][FT] FT_APDB_OP FT_APDB_OP_QUERYPATH apdb_path: %s",
            ft_cnf.result.query_apdbpath_cnf.apdb_path);
            ft_cnf.status = META_SUCCESS;
            break;
        default:
            break;
    }

    WriteDataToPC(&ft_cnf, sizeof(FT_APDB_CNF),NULL, 0);

}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////

FtModSysEnv::FtModSysEnv(void)
    :CmdTarget(FT_SYSENV_REQ_ID)
{
}

FtModSysEnv::~FtModSysEnv(void)
{
}

extern "C" int sysenv_set_in_user(const char *name, const char *value, int area);
void FtModSysEnv::exec(Frame *pFrm)
{
	
	CmdTarget::exec(pFrm);
	
	META_LOG("[Meta][FT] FT_SYS_ENV_OP");
	
	FT_SYS_ENV_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(FT_SYS_ENV_CNF));	
	FT_SYS_ENV_REQ *req = (FT_SYS_ENV_REQ *)pFrm->localBuf();	
	
	//fill the ft module header
	ft_cnf.header.id = req->header.id +1;
	ft_cnf.header.token = req->header.token;
	ft_cnf.type= req->type;
	ft_cnf.status = META_FAILED;

	switch(req->type)
    {
        case FT_SYSENV_SET:
			META_LOG("[Meta][FT] FT_SYS_ENV_OP FT_SYSENV_SET name = %s, value = %s",
				req->cmd.sysenv_set_req.name, req->cmd.sysenv_set_req.value);
			if(sysenv_set_in_user((char*)req->cmd.sysenv_set_req.name, (char*)req->cmd.sysenv_set_req.value, SYSENV_RW_AREA) < 0)
			{
				META_LOG("[Meta][FT] FT_SYS_ENV_OP FT_SYSENV_SET failed!");
			}
			else
			{
				META_LOG("[Meta][FT] FT_SYS_ENV_OP FT_SYSENV_SET succeed!");
				ft_cnf.status = META_SUCCESS;
			}	
			break;
		case FT_SYSENV_GET:
			META_LOG("[Meta][FT] FT_SYS_ENV_OP FT_SYSENV_GET name = %s", req->cmd.sysenv_get_req.name);
			const char *val = sysenv_get((char*)req->cmd.sysenv_get_req.name);
			if(val != NULL)
			{
				META_LOG("[Meta][FT] FT_SYS_ENV_OP FT_SYSENV_GET value = %s", val);
				memcpy(ft_cnf.result.sysenv_get_cnf.value, val, strlen(val));
				ft_cnf.status = META_SUCCESS;
			}
			break;
	}

	
    WriteDataToPC(&ft_cnf, sizeof(FT_SYS_ENV_CNF),NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModTargetClock::FtModTargetClock(void)
    :CmdTarget(FT_TARGETCLOCK_REQ_ID)
{
}

FtModTargetClock::~FtModTargetClock(void)
{
}

void FtModTargetClock::exec(Frame *pFrm)
{
	
	CmdTarget::exec(pFrm);
	
	META_LOG("[Meta][FT] FT_TARGET_CLOCK_OP");
	
	FT_TARGETCLOCK_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(FT_TARGETCLOCK_CNF));	
	FT_TARGETCLOCK_REQ *req = (FT_TARGETCLOCK_REQ *)pFrm->localBuf();	
	
	//fill the ft module header
	ft_cnf.header.id = req->header.id +1;
	ft_cnf.header.token = req->header.token;
	ft_cnf.type= req->type;
	ft_cnf.status = META_FAILED;

	switch(req->type)
    {
        case FT_CLOCK_SET:
			if(SetSysClock(&req->cmd.set_clock_req))
			{
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_CLOCK_GET:
			break;
	}
	
    WriteDataToPC(&ft_cnf, sizeof(FT_TARGETCLOCK_CNF), NULL, 0);
}

unsigned int FtModTargetClock::SetSysClock(SET_TARGET_CLOCK_REQ *req)
{
	int ret = 0;	
	struct tm tmpsec;     
	struct timespec rtime;     
	struct timespec cur_time;

	memset(&tmpsec, 0, sizeof(tm));
	memset(&rtime, 0, sizeof(timespec));
	memset(&cur_time, 0, sizeof(timespec));

	if(!IsValidDate(req))
		return ret;
	
	tmpsec.tm_year  = req->year- 1900;
	tmpsec.tm_mon   = req->mon - 1;
	tmpsec.tm_mday  = req->day;
	tmpsec.tm_hour  = req->hour;
	tmpsec.tm_min   = req->min;
	tmpsec.tm_sec   = req->sec;
	tmpsec.tm_isdst = -1;

	rtime.tv_sec = mktime(&tmpsec);
	clock_gettime(CLOCK_REALTIME, &cur_time);
	rtime.tv_nsec = cur_time.tv_nsec;
	if(clock_settime(CLOCK_REALTIME, &rtime) != 0)
	{
		META_LOG("[Meta][FT] set target clock failed! ret = %d, err = %d", ret, errno); 
		return 0;
	}
	
	META_LOG("[Meta][FT] set target clock successful!");
	return 1;

}

unsigned int FtModTargetClock::IsValidDate(SET_TARGET_CLOCK_REQ *req)
{
	if((req->year > 2067) || (req->year < 1900))
	{
		META_LOG("[Meta][FT] invalid year [1900~2067]! year = %d",  req->year);
		return 0;
	}

	if((req->mon > 12) || (req->mon < 1))
	{
		META_LOG("[Meta][FT] invalid mon [1~12]! mon = %d",  req->mon);
		return 0;
	}

	if((req->day > 31) || (req->day < 1))
	{
		META_LOG("[Meta][FT] invalid day [1~31]! day = %d",  req->day);
		return 0;
	}

	if(req->hour > 23)
	{
		META_LOG("[Meta][FT] invalid year [0~23]! hour = %d", req->hour);
		return 0;
	}

	if(req->min > 59)
	{
		META_LOG("[Meta][FT] invalid mon [0~59]! mon = %d", req->min);
		return 0;
	}

	if(req->sec > 59)
	{
		META_LOG("[Meta][FT] invalid day [0~59]! day = %d", req->sec);
		return 0;
	}

	return 1;

}


FtModTestAlive::FtModTestAlive(void)
	:CmdTarget(FT_IS_ALIVE_REQ_ID)
{
}

FtModTestAlive::~FtModTestAlive(void)
{
}

void FtModTestAlive::exec(Frame *pFrm)
{
	META_LOG("[Meta][FT] FT_TestAlive");
	FT_IS_ALIVE_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(FT_IS_ALIVE_CNF));

	CmdTarget::exec(pFrm);

	
	//just give the respone.
	ft_cnf.header.id = pFrm->getCmdTarget()->getId()+1;
	ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
	
	//write bootprof
	writeBootprof("[META] AP Send FT_TestAlive To PC.");
	
	WriteDataToPC(&ft_cnf, sizeof(FT_IS_ALIVE_CNF),NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//for normal version
#define TOTAL_PATH_NUM				2
#define VERSION_FILE_PATH_VENDOR    "/vendor/build.prop"
#define VERSION_FILE_PATH_SYSTEM    "/system/build.prop"


#define RELEASE_SW_TOKEN            "ro.mediatek.version.release"
#define RELEASE_PLATFORM_TOKEN      "ro.board.platform"
#define RELEASE_CHIP_TOKEN          "ro.mediatek.chip_ver"
#define RELEASE_PRODUCT_TOKEN       "ro.product.name"
#define RELEASE_BUILD_TIME_TOKEN    "ro.build.date"
#define RELEASE_BUILD_DISP_ID_TOKEN "ro.build.display.id"

FtModVersionInfo::FtModVersionInfo(void)
	:CmdTarget(FT_VER_INFO_REQ_ID)
{
}

FtModVersionInfo::~FtModVersionInfo(void)
{
}

void FtModVersionInfo::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);
	META_LOG("[Meta][FT] FT_VersionInfo");
	FT_VER_INFO_CNF ft_cnf;
	unsigned int dwRc = 0;
	unsigned int dwValSize = 0;
	unsigned int dwValType = 0;
	char szBuffer[MAX_PATH] = {0};
	FILE *fd = NULL;
	char str[256] = {0};
	char *loc = NULL;
	int bFind = 0;
	
	memset(&ft_cnf, 0, sizeof(ft_cnf));
	memset(szBuffer,0, sizeof(szBuffer));
	char* tmp = NULL;
	char platform[256] = {0};
	char chipVersion[256] = {0};
	
    char info_path[TOTAL_PATH_NUM][32] = {VERSION_FILE_PATH_VENDOR,
                                          VERSION_FILE_PATH_SYSTEM};
	
	//initail the value of ft header
	ft_cnf.header.id = pFrm->getCmdTarget()->getId()+1;
	ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
	ft_cnf.status = META_FAILED;
	
	for(int idx = 0; idx < TOTAL_PATH_NUM; idx++)
	{
		META_LOG("[Meta][FT] FT_GetVersionInfo to open file : %s\n", info_path[idx]);
		if((fd = fopen(info_path[idx],"r")) == NULL)
	{
		    META_LOG("[Meta][FT] FT_GetVersionInfo Can't open file : %s\n", info_path[idx]);
	}
	else
	{
	    while(!feof(fd))
	    {
		    if(fgets(str, 255, fd)!=NULL)
		    {
			    tmp = str;
			    loc = strsep(&tmp, "=");
			    if(!strcmp(loc, RELEASE_SW_TOKEN))
			    {
				    META_LOG("[Meta][FT] SW Version = %s\n", tmp);
				    strncpy((char*)ft_cnf.sw_ver, tmp, 63);
						bFind = 1;
			    }
			    if(!strcmp(loc, RELEASE_PLATFORM_TOKEN))
			    {
				    META_LOG("[Meta][FT] Platform = %s\n", tmp);
				    strncpy(platform, tmp, 255);
			    }
			    if(!strcmp(loc, RELEASE_PRODUCT_TOKEN))
			    {
				    META_LOG("[Meta][FT] Product Name = %s\n", tmp);
			    }
			    if(!strcmp(loc, RELEASE_CHIP_TOKEN))
			    {
				    META_LOG("[Meta][FT] Chip Version = %s\n", tmp);
				    strncpy(chipVersion, tmp, 255);
                    strncpy((char*)ft_cnf.hw_ver, tmp, 63);
			    }
			    if(!strcmp(loc, RELEASE_BUILD_TIME_TOKEN))
			    {
				    META_LOG("[Meta][FT] Build Time = %s\n", tmp);
				    strncpy((char*)ft_cnf.sw_time, tmp, 63);
			    }
		    }
	    }
		    fclose(fd);
		}
		if(bFind)
			break;
	}
	
	int i = 0;
	int k = 0;
	while (i < 256)
	{
		if (platform[i] != '\r' && platform[i] != '\n')
			szBuffer[k++] = platform[i++];				
		else
			break;
	}
	szBuffer[k++] = ',';
	i = 0;
	while (i < 256)
	{
		if (chipVersion[i] != '\r' && chipVersion[i] != '\n')
			szBuffer[k++] = chipVersion[i++];
		else
			break;
	}
	szBuffer[k++] = '\0';
	if (strlen(szBuffer) <= 64)
	{
		strncpy((char*)ft_cnf.bb_chip, szBuffer,strlen(szBuffer));
	}
	else
	{
		META_LOG("[Meta][FT] String is too long, length=%zd ", strlen(szBuffer));
	}

	/* Get Software version : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.sw_ver = %s ", ft_cnf.sw_ver);
	
	/* Get the build time : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.sw_time = %s ", ft_cnf.sw_time);
	
	/* Get the chip version : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.bb_chip = %s ", ft_cnf.bb_chip);
	META_LOG("[Meta][FT] ft_cnf.hw_ver = %s ", ft_cnf.hw_ver);
	
	ft_cnf.status = META_SUCCESS;
	
Ver_error:
	WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModVersionInfo2::FtModVersionInfo2(void)
	:CmdTarget(FT_VER_INFO_V2_REQ_ID)
{
}

FtModVersionInfo2::~FtModVersionInfo2(void)
{
}

void FtModVersionInfo2::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm); 
	META_LOG("[Meta][FT] FT_VersionInfo2");
	FT_VER_INFO_V2_CNF ft_cnf;
    unsigned int dwRc = 0;
    unsigned int dwValSize = 0;
    unsigned int dwValType = 0;
    char szBuffer[MAX_PATH] = {0};
    FILE *fd = 0;
    char str[256] = {0};
    char *loc = NULL;
	int bFind = 0;

    memset(&ft_cnf, 0, sizeof(ft_cnf));
    memset(szBuffer,0, sizeof(szBuffer));
    char* tmp = NULL;
    char platform[256] = {0};
    char chipVersion[256] = {0};

    //initail the value of ft header
    ft_cnf.header.id = pFrm->getCmdTarget()->getId() +1;
    ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
    ft_cnf.status = META_FAILED;

    char info_path[TOTAL_PATH_NUM][32] = {VERSION_FILE_PATH_VENDOR, 
                                          VERSION_FILE_PATH_SYSTEM};


    META_LOG("[Meta][FT] FT_GetVersionInfoV2 ");

    for(int idx = 0; idx < TOTAL_PATH_NUM; idx++)
    {
        META_LOG("[Meta][FT] FT_GetVersionInfo to open file : %s\n", info_path[idx]);
		if((fd = fopen(info_path[idx],"r")) == NULL)
		{
			META_LOG("[Meta][FT] FT_GetVersionInfo Can't open file : %s\n", info_path[idx]);
    }
    else
    {
        while(!feof(fd))
        {
            if(fgets(str, 256, fd)!=NULL)
            {
                tmp = str;
                loc = strsep(&tmp, "=");
                if(!strcmp(loc, RELEASE_SW_TOKEN))
                {
                    META_LOG("[Meta][FT] SW Version = %s\n", tmp);
				    strncpy((char*)ft_cnf.sw_ver, tmp, 63);
						bFind = 1;
                }
                if(!strcmp(loc, RELEASE_PLATFORM_TOKEN))
                {
                    META_LOG("[Meta][FT] Platform = %s\n", tmp);
				    strncpy(platform, tmp, 255);
                }
                if(!strcmp(loc, RELEASE_PRODUCT_TOKEN))
                {
                    META_LOG("[Meta][FT] Product Name = %s\n", tmp);
                }
                if(!strcmp(loc, RELEASE_CHIP_TOKEN))
                {
                    META_LOG("[Meta][FT] Chip Version = %s\n", tmp);
				    strncpy(chipVersion, tmp, 255);
                    strncpy((char*)ft_cnf.hw_ver, tmp, 63);
                }
                if(!strcmp(loc, RELEASE_BUILD_TIME_TOKEN))
                {
                    META_LOG("[Meta][FT] Build Time = %s\n", tmp);
                    strncpy((char*)ft_cnf.sw_time, tmp, 63);
                }
                if(!strcmp(loc, RELEASE_BUILD_DISP_ID_TOKEN))
                {
                    META_LOG("[Meta][FT] Build Display ID = %s\n", tmp);
                    strncpy((char*)ft_cnf.build_disp_id, tmp, 63);
                }
             }
         }
			fclose(fd);
		}

		if(bFind)
			break;
    }

    int i = 0;
    int k = 0;
	while (i < 256)
	{
		if (platform[i] != '\r' && platform[i] != '\n')
			szBuffer[k++] = platform[i++];
		else
			break;
	}
	szBuffer[k++] = ',';
	i = 0;
	while (i < 256)
	{
		if (chipVersion[i] != '\r' && chipVersion[i] != '\n')
			szBuffer[k++] = chipVersion[i++];
		else
			break;
	}
	szBuffer[k++] = '\0';
	if (strlen(szBuffer) <= 64)
	{
		strncpy((char*)ft_cnf.bb_chip, szBuffer,strlen(szBuffer));
	}
	else
	{
		META_LOG("[Meta][FT] String is too long, length=%zd ", strlen(szBuffer));
	}

	/* Get Software version : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.sw_ver = %s ", ft_cnf.sw_ver);

	/* Get the build time : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.sw_time = %s ", ft_cnf.sw_time);

	/* Get the chip version : ft_cnf.sw_ver */
	META_LOG("[Meta][FT] ft_cnf.bb_chip = %s ", ft_cnf.bb_chip);
	META_LOG("[Meta][FT] ft_cnf.hw_ver = %s ", ft_cnf.hw_ver);

    ft_cnf.status = META_SUCCESS;

Ver_error:
    WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);


}

///////////////////////////////////////////////////////////////////////////////////////////////
//for chip version
#define CHIPINFO_CODE_FUNC          "/proc/chip/code_func"
#define CHIPINFO_CODE_PROJ          "/proc/chip/code_proj"
#define CHIPINFO_CODE_DATE          "/proc/chip/code_date"
#define CHIPINFO_CODE_FAB           "/proc/chip/code_fab"

FtModChipInfo::FtModChipInfo(void)
	:CmdTarget(FT_CHIP_INFO_REQ_ID)
{
}

FtModChipInfo::~FtModChipInfo(void)
{
}

void FtModChipInfo::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);
	META_LOG("[Meta][FT] FT_ChipInfo");
	FT_CHIP_INFO_CNF ft_cnf;
	FILE *fd = 0;
	char szInfoPath[][32] = { 
			CHIPINFO_CODE_FUNC,
			CHIPINFO_CODE_PROJ,
		    CHIPINFO_CODE_DATE,
			CHIPINFO_CODE_FAB};

	char chip_info[4][64];
	memset(&chip_info,0,sizeof(char)*4*64);
	
	memset(&ft_cnf, 0, sizeof(ft_cnf));
	
	//initail the value of ft header
	ft_cnf.header.id = pFrm->getCmdTarget()->getId()+1;
	ft_cnf.header.token = pFrm->getCmdTarget()->getToken();
	ft_cnf.status = META_FAILED;
	
	/* Get the chip info */
	int i = 0;
	for(i; i<4; i++)
	{
		if((fd = fopen(szInfoPath[i], "r")) != NULL)
		{
			if(fgets((char*)chip_info[i], 64, fd) != NULL)
			{
				META_LOG("[Meta][FT] %s = %s\n", szInfoPath[i], chip_info[i]);
			}
			else
			{
				META_LOG("[Meta][FT] failed to read <%s>\n", szInfoPath[i]);
			}

			fclose(fd);
		}
		else
		{
			META_LOG("[Meta][FT] Can't open file : %s\n", szInfoPath[i]);
			break;
		}
	}

	if(i == 4)
	{
		strncpy((char*)ft_cnf.code_func, chip_info[0], 63);
		strncpy((char*)ft_cnf.code_proj, chip_info[1], 63);
		strncpy((char*)ft_cnf.code_date, chip_info[2], 63);
		strncpy((char*)ft_cnf.code_fab, chip_info[3], 63);
		ft_cnf.status = META_SUCCESS;
	}
	
	WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

}
///////////////////////////////////////////////////////////////////////////////////////////////
FtModPowerOff::FtModPowerOff(void)
	:CmdTarget(FT_POWER_OFF_REQ_ID)
{
}

FtModPowerOff::~FtModPowerOff(void)
{
}

void FtModPowerOff::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] FT_PowerOff");

	
	FT_POWER_OFF_REQ *req = (FT_POWER_OFF_REQ *)pFrm->localBuf();

	//sync();
    usleep(200*1000);

    int comPortType;
    comPortType = getComType();
    if(comPortType == META_USB_COM && req->dummy != 3)
    {

		FILE *PUsbFile = NULL;
        PUsbFile = fopen("sys/devices/platform/mt_usb/cmode","w");
		if(PUsbFile == NULL)
		{
           	META_LOG("[Meta][FT] Could not open sys/devices/platform/mt_usb/cmode ");
			PUsbFile = fopen("/sys/devices/platform/musb-mtu3d/musb-hdrc/cmode","w");
			if(PUsbFile == NULL)
			{
			    META_LOG("[Meta][FT] Could not open /sys/devices/platform/musb-mtu3d/musb-hdrc/cmode ");
			    PUsbFile = fopen("/sys/class/udc/musb-hdrc/device/cmode","w");
			    if(PUsbFile == NULL)
			    {
			        META_LOG("[Meta][FT] Could not open /sys/class/udc/musb-hdrc/device/cmode ");
			    }
			    else
			    {
			        fputc('0',PUsbFile);
			        fclose(PUsbFile);
			    }
			}
			else
		    {
			    fputc('0',PUsbFile);
			    fclose(PUsbFile);
		    }
        }
        else
       	{
       		fputc('0',PUsbFile);
			fclose(PUsbFile);	
       	}
		
    }
	else if(comPortType == META_SOCKET)
	{
		if(NORMAL_BOOT == getBootMode())
		{
			META_LOG("[Meta][FT] Send broadcast to disconnect WIFI");
			const char *strBroadcast = "am broadcast -a Exit_Meta_Info_Activity";
			system(strBroadcast);	
		}
		else
		{
		    meta_close_wifi(); 
			META_LOG("[Meta][FT] Close WIFI Connect ");
		}

    }
    else
    {
        META_LOG("[Meta][FT] com port type is uart! ");
    }
	
    if(req->dummy == 0)
    {
        usleep(1000 * 1000);
		//reboot(RB_POWER_OFF);
		property_set("sys.powerctl","shutdown");
	}
    else if(req->dummy == 2)
    {
		usleep(1000 * 1000);
		//reboot(RB_AUTOBOOT);
		property_set("sys.powerctl","reboot");
    }
	else if(req->dummy == 3)
	{
		META_LOG("[Meta][FT] Disconnect ATM Meta ");
		destroySerPortThread();	
		MSocket *pSocket = getSocket(SOCKET_ATCI);
		if(pSocket != NULL)
		{
			pSocket->send_msg("calibration_stop");
		}
		else
		{
			META_LOG("[Meta][FT] pSocket is NULL");	
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
FtModReboot::FtModReboot(void)
	:CmdTarget(FT_REBOOT_REQ_ID)
{
}

FtModReboot::~FtModReboot(void)
{
}

void FtModReboot::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] FT_Reboot");

	
	FT_META_REBOOT_REQ *req = (FT_META_REBOOT_REQ *)pFrm->localBuf();

	sleep(req->delay);

    //Reboot target side after finishing the meta
	//sync();
	//reboot(RB_AUTOBOOT);
	property_set("sys.powerctl","reboot");
}

///////////////////////////////////////////////////////////////////////////////////////////////
FtModBuildProp::FtModBuildProp(void)
	:CmdTarget(FT_BUILD_PROP_REQ_ID)
{
}

FtModBuildProp::~FtModBuildProp(void)
{
}

void FtModBuildProp::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] FT_BuildProp");

	FT_BUILD_PROP_CNF ft_cnf;
	FT_BUILD_PROP_REQ *req = (FT_BUILD_PROP_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
	
    property_get((const char*)req->tag, (char *)ft_cnf.content, "unknown");	

	META_LOG("[Meta][FT] %s = %s ",req->tag,ft_cnf.content);

	ft_cnf.status = META_SUCCESS;

	WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);	
}

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_GSENSOR_FEATURE

FtModGSensor::FtModGSensor(void)
	:CmdTarget(FT_GSENSOR_REQ_ID)
{
}

FtModGSensor::~FtModGSensor(void)
{
}

void FtModGSensor::exec(Frame *pFrm)
{

	CmdTarget::exec(pFrm);

     //do the G-Sensor test by called the interface in meta G-Sensor lib
    if(getInitState())
	    Meta_GSensor_OP((GS_REQ *)pFrm->localBuf());

}

int FtModGSensor::init(Frame*pFrm)
{
	META_LOG("[Meta][FT] FT_GSensor");	
	
  	GS_CNF ft_cnf;
    static int bInitFlag_GS = false;
	GS_REQ *req = (GS_REQ *)pFrm->localBuf();


    memset(&ft_cnf, 0, sizeof(GS_CNF));

    if (false == bInitFlag_GS)
    {
        // initial the G-Sensor module when it is called first time
        if (!Meta_GSensor_Open())
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;
		    ft_cnf.op = req->op;
			 
            META_LOG("[Meta][FT] FT_GSENSOR_OP Meta_GSensor_Open Fail ");
            WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);
			return false;
        }
        bInitFlag_GS = true;
    }
	return true;
}

#endif


///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_MSENSOR_FEATURE

FtModMSensor::FtModMSensor(void)
	:CmdTarget(FT_MSENSOR_REQ_ID)
{
   memset(&m_ft_cnf, 0, sizeof(FT_MSENSOR_CNF));
}

FtModMSensor::~FtModMSensor(void)
{
}

void FtModMSensor::exec(Frame *pFrm)
{

  META_LOG("[Meta][FT] FT_MSensor");	
    
	int res = -1;
  	memset(&m_ft_cnf, 0, sizeof(FT_MSENSOR_CNF));
	
	FT_MSENSOR_REQ *req = (FT_MSENSOR_REQ *)pFrm->localBuf();
	
	m_ft_cnf.header.id = req->header.id + 1;
	m_ft_cnf.header.token = req->header.token;
	m_ft_cnf.status = META_SUCCESS;

  	CmdTarget::exec(pFrm);

	if(getInitState())
	{
		res = Meta_MSensor_OP();
		if (0 == res)
		{
		    META_LOG("[Meta][FT] FT_MSENSOR_OP Meta_MSensor_OP success!");
		    m_ft_cnf.status = META_SUCCESS;
		}
		else
		{
		    META_LOG("[Meta][FT] FT_MSENSOR_OP Meta_MSensor_OP failed!");
		    m_ft_cnf.status = META_FAILED;
		}

	  WriteDataToPC(&m_ft_cnf, sizeof(m_ft_cnf),NULL, 0);
	}

}

int FtModMSensor::init(Frame*)
{
	static int bInitFlag_MS = false;
	

	if (false == bInitFlag_MS)
    {
        // initial the M-Sensor module when it is called first time
        if (!Meta_MSensor_Open())
        {
            META_LOG("[Meta][FT] FT_MSENSOR_OP Meta_MSensor_Open failed!");
			m_ft_cnf.status = META_FAILED;
            WriteDataToPC(&m_ft_cnf, sizeof(m_ft_cnf),NULL, 0);
			return false;
        }
        bInitFlag_MS = TRUE;
    }

	return true;
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FT_ALSPS_FEATURE

FtModALSPS::FtModALSPS(void)
	:CmdTarget(FT_ALSPS_REQ_ID)
{
    memset(&m_ft_cnf, 0, sizeof(FT_ALSPS_CNF));
}

FtModALSPS::~FtModALSPS(void)
{
}

void FtModALSPS::exec(Frame *pFrm)
{
	META_LOG("[Meta][FT] FT_ALSPS");	
	
	int res = -1;
  	memset(&m_ft_cnf, 0, sizeof(FT_ALSPS_CNF));
	
	FT_ALSPS_REQ *req = (FT_ALSPS_REQ *)pFrm->localBuf();
		
	m_ft_cnf.header.id = req->header.id + 1;
	m_ft_cnf.header.token = req->header.token;
	m_ft_cnf.status = META_SUCCESS;

	CmdTarget::exec(pFrm);

	if(getInitState())
	{		
	    res = Meta_ALSPS_OP();
	    if (0 == res)
	    {
	        META_LOG("[Meta][FT] FT_ALSPS_OP Meta_ALSPS_OP success!");
	        m_ft_cnf.status = META_SUCCESS;
	    }
		else
		{
		    META_LOG("[Meta][FT] FT_ALSPS_OP Meta_ALSPS_OP failed!");
		    m_ft_cnf.status = META_FAILED;
		}
	    WriteDataToPC(&m_ft_cnf, sizeof(m_ft_cnf),NULL, 0);
	}

}

int FtModALSPS::init(Frame*)
{
	static int bInitFlag_ALSPS = false;

	if (false == bInitFlag_ALSPS)
    {
        // initial the M-Sensor module when it is called first time
        if (!Meta_ALSPS_Open())
        {
            META_LOG("[Meta][FT] FT_ALSPS_OP Meta_ALSPS_Open failed!");
			m_ft_cnf.status = META_FAILED;
            WriteDataToPC(&m_ft_cnf, sizeof(m_ft_cnf),NULL, 0);
			return false;
        }
        bInitFlag_ALSPS = true;
    }
	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_GYROSCOPE_FEATURE

FtModGyroSensor::FtModGyroSensor(void)
	:CmdTarget(FT_GYROSCOPE_REQ_ID)
{
}

FtModGyroSensor::~FtModGyroSensor(void)
{
}

void FtModGyroSensor::exec(Frame *pFrm)
{

	CmdTarget::exec(pFrm);

    //do the Gyroscope-Sensor test by called the interface in meta Gyroscope-Sensor lib  
	if(getInitState())
		Meta_Gyroscope_OP((GYRO_REQ *)pFrm->localBuf());

}

int FtModGyroSensor::init(Frame* pFrm)
{
 	META_LOG("[Meta][FT] FT_GyroSensor");
	
	GYRO_REQ *req = (GYRO_REQ *)pFrm->localBuf();
	
	
    static int bInitFlag_GYRO = false;
	GYRO_CNF ft_cnf;
	memset(&ft_cnf, 0, sizeof(GYRO_CNF));
	
    if (false == bInitFlag_GYRO)
    {
        // initial the Gyroscope-Sensor module when it is called first time
        if (!Meta_Gyroscope_Open())
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;
	    	ft_cnf.op = req->op;

            META_LOG("[Meta][FT] FT_GYROSENSOR_OP Meta_GYROSensor_Open Fail ");
            WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);
			return false;
        }
        bInitFlag_GYRO = true;
    }

	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#define MDDB_FILE_FOLDER "/data/misc/mddb/"
#define MDDB_FILE_FOLDER_EX "/data/misc/mddb"
#define MD1INFO_FILE_PATH "/data/misc/mddb/md1_file_map.log"
#define MD1INFO_FILE_MAP_KEYWORD "md1_file_map"
#define MD1DB_FILE_KEYWORD "md1_mddbmeta"
#define MD1OPENDB_FILE_KEYWORD "md1_mddbmetaodb"

#define MD3INFO_FILE_PATH "/data/misc/mddb/md3_file_map.log"
#define MD3INFO_FILE_MAP_KEYWORD "md3_file_map"
#define MD3DB_FILE_KEYWORD "md3_mddb_c2k_meta"



FtModModemInfo::FtModModemInfo(void)
	:CmdTarget(FT_MODEM_REQ_ID)
{
}

FtModModemInfo::~FtModModemInfo(void)
{
}

int FtModModemInfo::getModemCapability(MODEM_CAPABILITY_LIST_CNF* modem_capa)
{
	int modem_type = 0;	
	modem_type = getMdmType();

	if((modem_type & MD1_INDEX) == MD1_INDEX)
	{
	    if(getModemProtocol(0, (void*)modem_capa) == 0)
	    {
	        META_LOG("[Meta][FT] MD1 getModemProtocol fail");
	        return 0;
	    }
	}

	if((modem_type & MD2_INDEX) == MD2_INDEX)
	{
	    if(getModemProtocol(1, (void*)modem_capa) == 0)
	    {
	        META_LOG("[Meta][FT] MD2 getModemProtocol fail");
	        return 0;
	    }
	}

	if((modem_type & MD3_INDEX) == MD3_INDEX)
	{
	    if(getModemProtocol(2, (void*)modem_capa) == 0)
	    {
	        META_LOG("[Meta][FT] MD3 getModemProtocol fail");
	        return 0;
	    }
	}

	if((modem_type & MD5_INDEX) == MD5_INDEX)
	{	
	    if(getModemProtocol(4, (void*)modem_capa) == 0)
	    {
	        META_LOG("[Meta][FT] MD5 getModemProtocol fail");
	        return 0;
	    }
	}

	return 1;
}

void FtModModemInfo::rebootModem(FT_MODEM_REQ *req, FT_MODEM_CNF & ft_cnf, int fd)
{
	META_LOG("[Meta] Enter rebootModem");

	setModemModeSwitching(1);

	int result = 0;
	int modem_mode = req->cmd.reboot_modem_req.mode;
	int modem_index = req->cmd.reboot_modem_req.modem_index;

	char initMdMode[128] = {0};
	property_get("persist.atm.mdmode", initMdMode, "normal");  //Get current modem mode

	META_LOG("[Meta] modem_mode = %d", modem_mode);
	META_LOG("[Meta] modem_index = %d", modem_index);

	if(getModemHwVersion(0)==MODEM_6293) //for Gen 93 modem
	{
		property_set("persist.atm.mdmode", "switching");
		if(modem_mode == getMDMode())
		{
			META_LOG("[Meta] Modem mode is already %d, no need to switch", modem_mode);
			property_set("persist.atm.mdmode", initMdMode);
			ft_cnf.status = META_SUCCESS;
		}
		else
		{
			if(0 == ChangeModemMode(modem_mode))
			{
				META_LOG("[Meta] Switch modem mode from %d to %d success", getMDMode(), modem_mode);
				setMDMode(modem_mode);
				if(modem_mode == 1)
				{
				    property_set("persist.atm.mdmode", "normal");
				}
				else if(modem_mode == 2)
				{
					property_set("persist.atm.mdmode", "meta");
					createAllModemThread();
				}
				ft_cnf.status = META_SUCCESS;
			}
			else
			{
				META_LOG("[Meta] Switch modem mode from %d to %d fail", getMDMode(), modem_mode);
				property_set("persist.atm.mdmode", initMdMode);
				ft_cnf.status = META_FAILED;
			}
		}
	}
	else  //for modem before 93
	{
		setActiveATModem(modem_index);

		META_LOG("[Meta] Destory modem thread and close modem handle");
		destroyAllModemThread();

		if (fd >= 0 && 0 == (result = ioctl(fd, CCCI_IOC_SET_MD_BOOT_MODE, &modem_mode)))
		{	
			META_LOG("[Meta][FT] ioctl CCCI_IOC_SET_MD_BOOT_MODE success modem_mode = %d", modem_mode);
			if (0 == (result = ioctl(fd, CCCI_IOC_MD_RESET)))
			{
				ft_cnf.status = META_SUCCESS;
				META_LOG("[Meta][FT] ioctl CCCI_IOC_MD_RESET success " );
			}
			else
			{
				ft_cnf.status = META_FAILED;
				META_LOG("[Meta][FT] ioctl CCCI_IOC_MD_RESET fail result = %d, errno = %d, fd = %d", result, errno, fd);	
			}
		}
		else
		{
			ft_cnf.status = META_FAILED;
			META_LOG("[Meta][FT] ioctl CCCI_IOC_SET_MD_BOOT_MODE fail modem_mode = %d, result = %d, errno = %d, fd = %d", modem_mode, result, errno, fd);
		}
	}
	setModemModeSwitching(0);
}
int FtModModemInfo::getModemMode(FT_MODEM_REQ *req, FT_MODEM_CNF & ft_cnf, int fd)
{
	int modem_state = 0;
	int modem_boot_mode = 0;
	int result = 0;

	req; //just for reolve unused warning
	if(fd < 0)
	{
		ft_cnf.status = META_FAILED;
		META_LOG("[Meta][FT] invalid ioctl dev port. fd = %d", fd );
		return -1;
	}
	
	while(modem_state != 2)
	{
		if(0 == (result=ioctl(fd, CCCI_IOC_GET_MD_STATE, &modem_state)))
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_STATE success modem_state = %d",modem_state );
		usleep(200*1000);
		}
		else
		{
			ft_cnf.status = META_FAILED;
			META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_STATE fail result = %d, errno = %d, fd = %d", result, errno, fd );
			return -1;
		}				
	}
		
	if (0 == (result=ioctl(fd, CCCI_IOC_GET_MD_BOOT_MODE,&modem_boot_mode)))
	{
		ft_cnf.status = META_SUCCESS;
		ft_cnf.result.get_modem_mode_cnf.mode = modem_boot_mode;
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_BOOT_MODE success modem_boot_mode = %d",modem_boot_mode );
		setMDMode(modem_boot_mode);//normal= 1 meta=2
		return 0;
		}
	else
	{
		ft_cnf.status = META_FAILED;
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_BOOT_MODE fail result = %d, errno = %d, fd = %d", result, errno, fd);	
		return -1;
	}
}

int FtModModemInfo::getModemState(int *modem_state, int fd)
{
	int result = 0;
	int retry_count = 0;

	//Get modem reboot status
	do
	{
		if(0 == ioctl(fd, CCCI_IOC_GET_MD_STATE, modem_state))
		{
			retry_count++;
		}
		usleep(500*1000);
		META_LOG("[Meta][FT][DEBUG] Query modem reboot result %d times", retry_count);
	}while((*modem_state != 2) && (retry_count<10));
	if(*modem_state == 2)
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_STATE success, modem_state = %d", *modem_state);
	}
	else
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_STATE fail, modem_state = %d", *modem_state);
		result = -1;
	}

	return result;
}

int FtModModemInfo::getModemType(int *modem_type, int fd)
{
	int result = 0;
	//Get modem type
	if (fd >= 0 && 0 == ioctl(fd, CCCI_IOC_GET_MD_TYPE, modem_type))
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_TYPE success, modem_type = %d", *modem_type);
	}
	else
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_TYPE fail, modem_type = %d", *modem_type);
	    result = -1;
	}
	return result;
}

int FtModModemInfo::setModemType(int modem_type, int fd)
{
	int result = 0;

	//Set modem type and reboot modem
	if (fd >= 0 && 0 == ioctl(fd, CCCI_IOC_RELOAD_MD_TYPE, &modem_type))
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_RELOAD_MD_TYPE success, modem_type = %d", modem_type);
		if (0 == ioctl(fd, CCCI_IOC_MD_RESET))
		{
			META_LOG("[Meta][FT] ioctl CCCI_IOC_MD_RESET success " );
		}
		else
		{
			META_LOG("[Meta][FT] ioctl CCCI_IOC_MD_RESET fail " );
		    result = -1;
		}
	}
	else
	{
		META_LOG("[Meta][FT] ioctl CCCI_IOC_RELOAD_MD_TYPE fail, modem_type = %d", modem_type);
		result = -1;
	}

	return result;
}

int FtModModemInfo::getModemIndex(FT_MODEM_REQ *req)
{
	int nModemIndex = 0;	
	
	if(req->type == FT_MODEM_OP_SET_MODEMTYPE)
    {
        nModemIndex = req->cmd.set_modem_type_req.modem_id;
    }
	else if(req->type == FT_MODEM_OP_GET_CURENTMODEMTYPE)
	{
	    nModemIndex = req->cmd.get_currentmodem_type_req.modem_id;
	}
	else if(req->type == FT_MODEM_OP_QUERY_MDIMGTYPE)
	{
	    nModemIndex = req->cmd.query_modem_imgtype_req.modem_id;
	}
	
	if(nModemIndex > 0 && nModemIndex < 6)
	{
	    nModemIndex = nModemIndex - 1;
	}
	else
	{
	    nModemIndex = 0;
	}	

	return nModemIndex;
	
}

#ifdef MTK_SINGLE_BIN_MODEM_SUPPORT
int FtModModemInfo::CopyMDDBFile(unsigned int nModemId)
{
    if (access(MDDB_FILE_FOLDER,F_OK) != 0 )
    {
        META_LOG("[Meta][FT] CopyMDDBFile /data/misc/mddb/ folder doesn't exist error %s\n", strerror(errno));
        return 0;
    }

    char mddb_path[256] = {0};
    char mdopendb_path[256] = {0};
    char info_name[128] = {0};
    char info_path[256] = {0};
    memcpy(mddb_path,MDDB_FILE_FOLDER, strlen(MDDB_FILE_FOLDER));
    memcpy(mdopendb_path,MDDB_FILE_FOLDER, strlen(MDDB_FILE_FOLDER));
    if(nModemId == 2)
    {
        memcpy(info_name,MD3INFO_FILE_MAP_KEYWORD, strlen(MD3INFO_FILE_MAP_KEYWORD));
        memcpy(info_path,MD3INFO_FILE_PATH, strlen(MD3INFO_FILE_PATH));
    }
    else
    {
        memcpy(info_name,MD1INFO_FILE_MAP_KEYWORD, strlen(MD1INFO_FILE_MAP_KEYWORD));
        memcpy(info_path,MD1INFO_FILE_PATH, strlen(MD1INFO_FILE_PATH));
    }

    int dbRet = -1;
    int opendbRet = -1;
    int info_ret = restore_image_from_pt(info_name, info_path);
    META_LOG("[Meta][FT] CopyMDDBFile copy info file retore_image_from_pt info_ret:%d", info_ret);
    if(info_ret <= 0)
    {
        META_LOG("[Meta][FT] CopyMDDBFile copy info file error %s\n", strerror(errno));
        return -1;
    }
    else
    {
        FILE* fileInfoFd = NULL;
        fileInfoFd = fopen(info_path,"r");
        if (fileInfoFd == NULL)
        {
            META_LOG("[Meta][FT] CopyMDDBFile open info file error %s\n", strerror(errno));
            return -1;
        }
        else
        {
            char *loc = NULL;
            char* tmp = NULL;
            char str[256] = {0};
            while(!feof(fileInfoFd))
            {
                if(fgets(str, 256, fileInfoFd)!=NULL)
                {
                    tmp = str;
                    loc = strsep(&tmp, "=");
                    if(nModemId == 2)
                    {
                        if(!strcmp(loc, MD3DB_FILE_KEYWORD))
                        {
                            if (tmp[strlen(tmp)-1] == '\n')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            else if (tmp[strlen(tmp)-1] == '\r')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            strncat(mddb_path, tmp, strlen(tmp));
                            META_LOG("[Meta][FT] CopyMDDBFile MDDB file path =%s, len =%d\n", mddb_path,strlen(mddb_path));
                        }
                    }
                    else   //default is 0
                    {
                        if(!strcmp(loc, MD1DB_FILE_KEYWORD))
                        {
                            if (tmp[strlen(tmp)-1] == '\n')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            else if (tmp[strlen(tmp)-1] == '\r')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            strncat(mddb_path, tmp, strlen(tmp));
                            META_LOG("[Meta][FT] CopyMDDBFile MDDB file path =%s, len =%d\n", mddb_path,strlen(mddb_path));
                        }
                        else if(!strcmp(loc, MD1OPENDB_FILE_KEYWORD))
                        {
                            if (tmp[strlen(tmp)-1] == '\n')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            else if (tmp[strlen(tmp)-1] == '\r')
                            {
                                tmp[strlen(tmp)-1] = '\0';
                            }
                            strncat(mdopendb_path, tmp, strlen(tmp));
                            META_LOG("[Meta][FT] CopyMDDBFile MD open DB file path =%s, len =%d\n", mdopendb_path,strlen(mdopendb_path));
                        }
                    }
                }
            }
            if (fileInfoFd != NULL)
            {
                fclose(fileInfoFd);
            }
        }
    }
    if(nModemId == 2)
    {
        dbRet = restore_image_from_pt(MD3DB_FILE_KEYWORD, mddb_path);
        return dbRet;
    }
    else
    {
        dbRet = restore_image_from_pt(MD1DB_FILE_KEYWORD, mddb_path);
        opendbRet = restore_image_from_pt(MD1OPENDB_FILE_KEYWORD, mdopendb_path);
        if (dbRet > 0)
        {
            return dbRet;
        }
        else
        {
            return opendbRet;
        }
    }
}
#endif

void FtModModemInfo::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] FT_ModemInfo");	

	FT_MODEM_CNF ft_cnf;
	int modem_req=0;
	int fd = -1;
	static int modemCreate = 0;
	int bDataDevice = FALSE;
	

	memset(&ft_cnf, 0, sizeof(FT_MODEM_CNF));
	ft_cnf.status = META_FAILED;

	
	FT_MODEM_REQ *req = (FT_MODEM_REQ *)pFrm->localBuf();
    META_LOG("[Meta][FT] FT_MODEM_INFO_OP, req type:%d ",req->type);

	if(req->type == FT_MODEM_OP_QUERY_INFO)
	{
		ft_cnf.result.query_modem_info_cnf.modem_number = getMdmNumber();
		ft_cnf.result.query_modem_info_cnf.modem_id = getActiveMdmId();
		ft_cnf.status = META_SUCCESS;
	}
	else if(req->type == FT_MODEM_OP_CAPABILITY_LIST)
	{
		int nRet = 0;
		MODEM_CAPABILITY_LIST_CNF modem_capa;
		memset(&modem_capa, 0, sizeof(MODEM_CAPABILITY_LIST_CNF));
		nRet = getModemCapability(&modem_capa);
		memcpy(&ft_cnf.result.query_modem_cap_cnf,&modem_capa,sizeof(MODEM_CAPABILITY_LIST_CNF));
		if (nRet == 1)
		{
		    ft_cnf.status = META_SUCCESS;
		}
		else
		{
		    ft_cnf.status = META_FAILED;
		}
	}
	else if(req->type == FT_MODEM_OP_QUERY_MDDOWNLOADSTATUS)
	{
		char temp[128]={0};
		int percentage = 0;
		int status_code = 0;
		property_get("persist.sys.extmddlprogress",temp,NULL);
		META_LOG("[Meta][FT] persist.sys.extmddlprogress = %s",temp);
		sscanf(temp,"%03d_%04d",&percentage,&status_code);
		META_LOG("[Meta][FT] FT_MODEM_INFO_OP FT_MODEM_OP_QUERY_MDDOWNLOADSTATUS percentage = %d,status_code = %d",percentage,status_code);
		ft_cnf.result.query_modem_download_status_cnf.percentage = percentage;
		ft_cnf.result.query_modem_download_status_cnf.status_code = status_code;
		
		if(percentage == 110 && status_code ==0 && modemCreate == 0)
		{
			unsigned int modemType = getMdmType();
			if((modemType & MD5_INDEX) == MD5_INDEX)
			{
				createModemThread(4,1);
			}
			modemCreate = 1;
		}
		ft_cnf.status = META_SUCCESS;
	}
	else if(req->type == FT_MODEM_OP_QUERY_MDDBPATH)
	{
		char szMDDBPath[64] = {0};
#ifdef MTK_SINGLE_BIN_MODEM_SUPPORT		
        	if (CopyMDDBFile(req->cmd.query_mddbpath_req.modem_id) <= 0)
		{
		    META_LOG("[Meta][FILE OPERATION] FtModModemInfo Failed to copy MD %d DB from modem image, error %s\n",req->cmd.query_mddbpath_req.modem_id,strerror(errno));
		    ft_cnf.status = META_FAILED;
		}
		else
		{
		    ft_cnf.status = META_SUCCESS;
		}

		strncpy(szMDDBPath, MDDB_FILE_FOLDER_EX, strlen(MDDB_FILE_FOLDER_EX));
#else

                strncpy(szMDDBPath, MTK_MDDB_PATH, strlen(MTK_MDDB_PATH));
                ft_cnf.status = META_SUCCESS;
#endif
		memcpy(ft_cnf.result.query_mddbpath_cnf.mddb_path, szMDDBPath, strlen(szMDDBPath));
		META_LOG("[Meta][FT] FtModModemInfo mddb_path: %s",ft_cnf.result.query_mddbpath_cnf.mddb_path);
		ft_cnf.header.id = req->header.id +1;
		ft_cnf.header.token = req->header.token;
		ft_cnf.type = req->type;
		WriteDataToPC(&ft_cnf, sizeof(FT_MODEM_CNF),NULL, 0);
		return;
	}

	else if(req->type == FT_MODEM_OP_SET_MODEMTYPE)
    {

		unsigned int modem_type = req->cmd.set_modem_type_req.modem_type;
		fd = getIOCPort(getModemIndex(req),bDataDevice);
	
		if (fd >= 0 && 0 == ioctl(fd, CCCI_IOC_RELOAD_MD_TYPE, &modem_type))
		{
			if (0 == ioctl(fd, CCCI_IOC_MD_RESET))
			{
				ft_cnf.status = META_SUCCESS;		
			}
			else
			{
				ft_cnf.status = META_FAILED;
				META_LOG("[Meta][FT] ioctl CCCI_IOC_MD_RESET fail " );	
			}
		}
		else
		{
			ft_cnf.status = META_FAILED;
			META_LOG("[Meta][FT] ioctl CCCI_IOC_RELOAD_MD_TYPE fail modem_type = %d", modem_type);	
		}
	}
	else if(req->type == FT_MODEM_OP_GET_CURENTMODEMTYPE)
	{
		unsigned int modem_type=0;

		fd = getIOCPort(getModemIndex(req),bDataDevice);
		
		if (fd >= 0 && 0 == ioctl(fd, CCCI_IOC_GET_MD_TYPE, &modem_type))
		{
			ft_cnf.status = META_SUCCESS;
			ft_cnf.result.get_currentmodem_type_cnf.current_modem_type = modem_type;	
			META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_TYPE success modem_type = %d", modem_type);
		}
		else
		{
			ft_cnf.status = META_FAILED;
			META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_TYPE fail");	
		}	
		
	}
	else if(req->type == FT_MODEM_OP_QUERY_MDIMGTYPE)
	{
		unsigned int mdimg_type[16]={0};
		fd = getIOCPort(getModemIndex(req),bDataDevice);

		if (fd >= 0 && 0 == ioctl(fd, CCCI_IOC_GET_MD_IMG_EXIST, &mdimg_type))
		{
			ft_cnf.status = META_SUCCESS;
			memcpy(ft_cnf.result.query_modem_imgtype_cnf.mdimg_type,mdimg_type,16*sizeof(unsigned int));

			for(int i = 0;i<16;i++)
			{
				META_LOG("[Meta][FT] mdimg_type[%d] %d",i, mdimg_type[i]);	
			}
				
		}
		else
		{
			ft_cnf.status = META_FAILED;
			META_LOG("[Meta][FT] ioctl CCCI_IOC_GET_MD_IMG_EXIST fail");	
		}	
	}
	else if(req->type == FT_MODEM_OP_REBOOT_MODEM)
	{
		fd = getIOCPort(getModemIndex(req),bDataDevice);
		rebootModem(req,ft_cnf,fd);
	}
	else if(req->type == FT_MODEM_OP_GET_MODEMMODE)
	{
		fd = getIOCPort(getModemIndex(req),bDataDevice);
		int result = getModemMode(req,ft_cnf,fd);
		if(bDataDevice == FALSE)
		{
	     	if(fd != -1)
		 	{
		     	close(fd);
				META_LOG("[Meta][FT]Close fd");
				fd = -1;
	     	}
		}

		//create modem thread and open modem handle
		if(result == 0)
			createAllModemThread();		
		
	}
	else
	{
		META_LOG("[Meta][FT] FT_MODEM_REQ have no this type %d",req->type );		
	}
	

	if(bDataDevice == FALSE)
	{
	     if(fd != -1)
		 {
		     close(fd);
			 META_LOG("[Meta][FT]Close fd");
			 fd = -1;
	     }
	}
	
	ft_cnf.header.id = req ->header.id +1;
    ft_cnf.header.token = req ->header.token;
	ft_cnf.type = req ->type;	


	META_LOG("[META] ft_cnf.status = %d",ft_cnf.status);
	
	WriteDataToPC(&ft_cnf, sizeof(FT_MODEM_CNF),NULL, 0);	
}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModSIMNum::FtModSIMNum(void)
	:CmdTarget(FT_SIM_NUM_REQ_ID)
{
}

FtModSIMNum::~FtModSIMNum(void)
{
}

void FtModSIMNum::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_SIMNum");	

	FT_GET_SIM_CNF ft_cnf;

	FT_GET_SIM_REQ *req = (FT_GET_SIM_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.type = req->type;
    ft_cnf.status = META_SUCCESS;

	char tempstr[128]={0};
	property_get("persist.radio.multisim.config",tempstr,"ss");

    ft_cnf.number = 2;
    META_LOG("[Meta][FT] The sim card number default is two");
	if((strcmp(tempstr,"dsds")==0) || (strcmp(tempstr,"dsda")==0))
	{
		ft_cnf.number = 2;
		META_LOG("[Meta][FT] The sim card number is two");
	}
	else if(strcmp(tempstr,"tsts")==0)
	{
		ft_cnf.number = 3;
		META_LOG("[Meta][FT] The sim card number is three");
	}
	else if(strcmp(tempstr,"qsqs")==0)
	{
		ft_cnf.number = 4;
		META_LOG("[Meta][FT] The sim card number is four");
    }
	else if(strcmp(tempstr,"ss")==0)
    {
		ft_cnf.number = 1;
    }
   	
   	WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_SDCARD_FEATURE

FtModSDcard::FtModSDcard(void)
	:CmdTarget(FT_SDCARD_REQ_ID)
{
}

FtModSDcard::~FtModSDcard(void)
{
}

void FtModSDcard::exec(Frame *pFrm)
{

	CmdTarget::exec(pFrm);
	
	if(getInitState())   
	    Meta_SDcard_OP((SDCARD_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());

}

int FtModSDcard::init(Frame* pFrm)
{
 	META_LOG("[Meta][FT] Ft_SDcard");

    SDCARD_CNF ft_cnf;	
	memset(&ft_cnf, 0, sizeof(SDCARD_CNF));
    static int bInitFlag_SDcard = FALSE;
	
	SDCARD_REQ *req = (SDCARD_REQ *)pFrm->localBuf();

	META_LOG("[Meta][FT] FT_SDcard_OP META Test req: %zd , %zd ",
        sizeof(SDCARD_REQ), sizeof(SDCARD_CNF));
		
    if (FALSE == bInitFlag_SDcard)
    {
        // initial the DVB module when it is called first time
        if (!Meta_SDcard_Init(req))
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;

            META_LOG("[Meta][FT] FT_SDcard_OP Meta_SDcard_Init Fail ");
    		WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);
			return false;
        }
        bInitFlag_SDcard = TRUE;
    }
	return true;
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_EMMC_FEATURE

FtModEMMC::FtModEMMC(void)
	:CmdTarget(FT_EMMC_REQ_ID)
{
}

FtModEMMC::~FtModEMMC(void)
{
}

void FtModEMMC::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_EMMC");

    META_CLR_EMMC_OP((FT_EMMC_REQ *)pFrm->localBuf());

}
#endif

#ifdef FT_NAND_FEATURE

FtModEMMC::FtModEMMC(void)
	:CmdTarget(FT_EMMC_REQ_ID)
{
}

FtModEMMC::~FtModEMMC(void)
{
}

void FtModEMMC::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_EMMC");

    META_CLR_EMMC_OP((FT_EMMC_REQ *)pFrm->localBuf());

}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_CRYPTFS_FEATURE

FtModCRYPTFS::FtModCRYPTFS(void)
	:CmdTarget(FT_CRYPTFS_REQ_ID)
{
}

FtModCRYPTFS::~FtModCRYPTFS(void)
{
}

void FtModCRYPTFS::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_CRYPTFS");
	
    META_CRYPTFS_OP((FT_CRYPTFS_REQ *)pFrm->localBuf());    

}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_ADC_FEATURE

FtModADC::FtModADC(void)
	:CmdTarget(FT_ADC_REQ_ID)
{
}

FtModADC::~FtModADC(void)
{
}

void FtModADC::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_ADC");

	Meta_ADC_OP((ADC_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(),  pFrm->peerLen());  
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////

FtModCustomer::FtModCustomer(void)
	:CmdTarget(FT_CUSTOMER_REQ_ID)
{
}

FtModCustomer::~FtModCustomer(void)
{
}

void FtModCustomer::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_Customer");

    FT_CUSTOMER_CNF ft_cnf;
    memset(&ft_cnf, 0, sizeof(FT_CUSTOMER_CNF));
	FT_CUSTOMER_REQ *req = (FT_CUSTOMER_REQ *)pFrm->localBuf();
	
	int peer_buff_size = 1;
    char* peer_buf = NULL;
    int setResult = -1;
    
	// Implement custom API logic here. The following is a sample code for testing.
    ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
	ft_cnf.type = req->type;
    ft_cnf.status = META_SUCCESS;
	
#ifndef MICROTRUST_FACTORY_SUPPORT
#ifdef TRUSTKERNEL_TEE_META
     peer_buf = (char*)malloc(pFrm->peerLen());
     if (peer_buf == NULL)
     {
         META_LOG("[Meta][FT] Ft_Customer malloc fail");
         return;
     }
     memcpy(peer_buf, pFrm->peerBuf(), pFrm->peerLen());
     peer_buff_size = pFrm->peerLen();

    /* ret == 1 means this command is NOT a TRUSTKERNEL meta buf.
     * Therefore, you are free to let other handlers to
     * handle this customer op.
     *
     * ret < 0 means error happens in this handler
     */

    int ret;
    if ((ret = handle_trustkernel_meta_command(
        req->cmd.m_u1Dummy,
       &peer_buf, &peer_buff_size,
        &ft_cnf.result.m_u1Dummy)) < 0) {
        META_LOG("handle_trustkernel_meta_command internal error\n");
    }

    /* other customer op handlers follow here, if ret == 1 */
#else
	peer_buf = (char*)malloc(peer_buff_size);
	memset(peer_buf, 0, peer_buff_size);

	META_LOG("[Meta][FT] setNormalModeTestFlag");
	setResult = setNormalModeTestFlag(req->cmd.m_u1Dummy);
	if(0 == setResult)
	{
		ft_cnf.status = META_SUCCESS;
	}
	else
	{
	    ft_cnf.status = META_FAILED;
	}
#endif
#else
	peer_buf = (char*)malloc(BUFFER_MAX_SIZE);
	if (NULL == peer_buf)
	{
	    META_LOG("[Meta][FT] Ft_Customer malloc fail");
		peer_buff_size = 0;
		goto END;
	}

	META_LOG("[Meta][FT] FT_CUSTOMER_OP successful, OP type is %d!", req->type);
	switch(req->cmd.m_u1Dummy)
	{
		case MICROTRUST_USE_COMMAND_ID:
			handle_spmeta_request(pFrm->peerBuf(), pFrm->peerLen(), peer_buf, &peer_buff_size);
			break;
		default:
			META_LOG("[Meta} unknow cmd!!!");
			peer_buff_size = strlen(peer_buf);
			break;
	}
END:
#endif	
	META_LOG("[Meta][FT] FT_CUSTOMER_OP successful, OP type is %d!", req->type);
	
    WriteDataToPC(&ft_cnf, sizeof(ft_cnf), peer_buf, peer_buff_size);
	free(peer_buf);
}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModSpecialTest::FtModSpecialTest(void)
	:CmdTarget(FT_SPECIALTEST_REQ_ID)
{
}

FtModSpecialTest::~FtModSpecialTest(void)
{
}

void FtModSpecialTest::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_SpecialTest");
	
	FT_SPECIALTEST_REQ *req = (FT_SPECIALTEST_REQ *)pFrm->localBuf();
	
    FT_SPECIALTEST_CNF SpecialTestCnf;
    memset(&SpecialTestCnf, 0, sizeof(FT_SPECIALTEST_CNF));
    SpecialTestCnf.header.id = req->header.id +1;
    SpecialTestCnf.header.token = req->header.token;
    SpecialTestCnf.type = req->type;
    SpecialTestCnf.status= META_FAILED;

    switch (req->type)
    {
          case FT_SPECIALTEST_OP_HUGEDATA:		//query the supported modules
                  META_LOG("[Meta][FT] pFTReq->type is FT_SPECIALTEST_OP_HUGEDATA ");                 
                  SpecialTestCnf.status= META_SUCCESS;
                  break;
          default :
                  break;
    }

    WriteDataToPC(&SpecialTestCnf, sizeof(FT_SPECIALTEST_CNF),(char *)pFrm->peerBuf(),  pFrm->peerLen());   
}


///////////////////////////////////////////////////////////////////////////////////////////////

#define CHIP_RID_PATH	"/proc/rid"
#define CHIP_RID_LEN 	16 


FtModChipID::FtModChipID(void)
	:CmdTarget(FT_GET_CHIPID_REQ_ID)
{
}

FtModChipID::~FtModChipID(void)
{
}

void FtModChipID::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_ChipID");

	FT_GET_CHIPID_CNF ft_cnf;
	int bytes_read = 0;
	int res = 0;
		
	memset(&ft_cnf, 0, sizeof(FT_GET_CHIPID_CNF));

	
	FT_GET_CHIPID_REQ *req = (FT_GET_CHIPID_REQ *)pFrm->localBuf();
		
	ft_cnf.header.id = req->header.id +1;
	ft_cnf.header.token = req->header.token;
	ft_cnf.status = META_FAILED;
		
	int fd = open(CHIP_RID_PATH, O_RDONLY);
	if (fd != -1)
	{
		while (bytes_read < CHIP_RID_LEN)
		{
			res = read(fd, ft_cnf.chipId + bytes_read, CHIP_RID_LEN);
			if (res > 0)
				bytes_read += res;
			else
				break;
		}
		close(fd);
		ft_cnf.status = META_SUCCESS;
		META_LOG("[Meta][FT] Chip rid=%s", ft_cnf.chipId);
	}
	else
	{
		if (errno == ENOENT)
		{
			ft_cnf.status = META_FAILED;
		}
		META_LOG("[Meta][FT] Failed to open chip rid file %s, errno=%d", CHIP_RID_PATH, errno);
	}
		
	WriteDataToPC(&ft_cnf, sizeof(ft_cnf), NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FT_TOUCH_FEATURE

FtModCTP::FtModCTP(void)
	:CmdTarget(FT_CTP_REQ_ID)
{
	memset(&m_ft_cnf, 0, sizeof(Touch_CNF));
}

FtModCTP::~FtModCTP(void)
{
}

void FtModCTP::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);
	
	memset(&m_ft_cnf, 0, sizeof(Touch_CNF));
	Touch_REQ *req = (Touch_REQ *)pFrm->localBuf();
	
	m_ft_cnf.header.id = req->header.id + 1;
	m_ft_cnf.header.token = req->header.token;
	m_ft_cnf.status = META_SUCCESS;
	m_ft_cnf.tpd_type = req->tpd_type;
	
	if(getInitState())
		Meta_Touch_OP(req,(char *)pFrm->peerBuf(),  pFrm->peerLen());	

}

int FtModCTP::init(Frame*)
{
	META_LOG("[Meta][FT] Ft_CTP");
	
	static int bInitFlag_CTP = false;
	if (false == bInitFlag_CTP)
	{
		// initial the touch panel module when it is called first time
		if (!Meta_Touch_Init())
		{
			META_LOG("[Meta][FT] FT_CTP_OP Meta_Touch_Init failed!");
			m_ft_cnf.status = META_FAILED;
			WriteDataToPC(&m_ft_cnf, sizeof(m_ft_cnf),NULL, 0);
			return false;
		}
		bInitFlag_CTP = TRUE;
	}

	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FT_GPIO_FEATURE

FtModGPIO::FtModGPIO(void)
	:CmdTarget(FT_GPIO_REQ_ID)
{
	
}

FtModGPIO::~FtModGPIO(void)
{
}

void FtModGPIO::exec(Frame *pFrm)
{
	GPIO_CNF ft_cnf;
	CmdTarget::exec(pFrm);

    META_LOG("[Meta][FT] Ft_GPIO");
	memset(&ft_cnf, 0, sizeof(GPIO_CNF));

	GPIO_REQ *req = (GPIO_REQ *)pFrm->localBuf();

	if(getInitState())
	{	//do the bat test by called the interface in meta bat lib
    	ft_cnf = Meta_GPIO_OP(*req,(unsigned char *)pFrm->peerBuf(),  pFrm->peerLen());
    	WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);
	}
}

int FtModGPIO::init(Frame*pFrm)
{
	GPIO_CNF ft_cnf;
    static int bInitFlag_GPIO = false;

    META_LOG("[Meta][FT] FT_GPIO_OP META Test ");
    memset(&ft_cnf, 0, sizeof(GPIO_CNF));
	GPIO_REQ *req = (GPIO_REQ *)pFrm->localBuf();

    if (FALSE == bInitFlag_GPIO)
    {
        // initial the bat module when it is called first time
        if (!Meta_GPIO_Init())
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;

            META_LOG("[Meta][FT] FT_GPIO_OP Meta_GPIO_Init Fail ");
            WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

			return false;
        }
        bInitFlag_GPIO = true;
    }

	return true;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//for SIM detect
FtModSIMDetect::FtModSIMDetect(void)
	:CmdTarget(FT_SIM_DETECT_REQ_ID)
{
	
}

FtModSIMDetect::~FtModSIMDetect(void)
{
}

void FtModSIMDetect::exec(Frame *pFrm)
{
	META_LOG("[Meta][SIM DETECT] FtModSIMDetect");	

	FT_SIM_DETECT_CNF  ft_cnf;

	memset(&ft_cnf, 0, sizeof(FT_SIM_DETECT_CNF));
	ft_cnf.status = META_FAILED;

	
	FT_SIM_DETECT_REQ *req = (FT_SIM_DETECT_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.type= req->type;

	META_LOG("[Meta][SIM DETECT] FtModSIMDetect req->type = %d", req->type);
	switch(req->type)
	{
		case FT_SIM_DETECT_OP_EXTMOD:
			{
				ft_cnf.result.sim_query_mdtype_cnf.md_type = CheckExtMd();
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_SIM_DETECT_OP_PRJTYPE:
			{
				unsigned int type;
				if(GetPrjType(type))
				{
					ft_cnf.result.sim_query_prjtype_cnf.prj_type = type;
					ft_cnf.status = META_SUCCESS;
				}
				else
				{
					META_LOG("[Meta][SIM DETECT] Failed to get project type");
				}
			}
			break;
		case FT_SIM_DETECT_OP_MDIDXSET:
			{
				ft_cnf.result.sim_query_mdidxset_cnf.md_idxset = getMdmType();
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_SIM_DETECT_OP_SWITCH:
			{
				if(SetSIMSwitcher(req->cmd.sim_set_switcher_req.mode_cmd))
				{
					META_LOG("[Meta][SIM DETECT] SIM Switch cmd  = 0x%08x", req->cmd.sim_set_switcher_req.mode_cmd );
					ft_cnf.status = META_SUCCESS;
				}
				else
				{
					META_LOG("[Meta][SIM DETECT] Failed to switch SIM");
				}
			}
			break;
		case FT_SIM_DETECT_OP_GETSSW:
			{
				int ssw_val = GetSSWValue();
				if(ssw_val > 0)
				{
					ft_cnf.result.sim_query_ssw_cnf.ssw_val = ssw_val;
					ft_cnf.status = META_SUCCESS;
				}					
			}
			break;
		case FT_SIM_DETECT_END:
			{
				META_LOG("[Meta][SIM DETECT] FtModSIMDetect req->type = %d FT_SIM_DETECT_END", req->type);	
			}
			break;
	}

	WriteDataToPC(&ft_cnf, sizeof(FT_SIM_DETECT_CNF),NULL, 0);
}

int FtModSIMDetect::GetDevNode()
{
	char dev_node[32] = {0};
	int fd = -1;
	
	snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_META_IOCTL,MD_SYS5));
	fd = open(dev_node, O_RDWR|O_NOCTTY|O_NDELAY );	
	
	if(fd < 0)
	{
	    META_LOG("[Meta][SIM DETECT] Failed to open CCCI port: MD_SYS5, %s", dev_node );
	}
    else
	{
	    META_LOG("[Meta][SIM DETECT] Open CCCI success: path = %s, handle = %d", dev_node, fd);
    }
	
	return fd;
		
}

unsigned int FtModSIMDetect::SetSIMSwitcher(unsigned int &mode)
{
	int fd = -1;

	META_LOG("[Meta][SIM DETECT] SetSIMSwitcher mode = 0x%08x", mode);
	
	if((fd = GetDevNode()) < 0)
		return 0;

	if (0 == ioctl(fd, CCCI_IOC_SIM_SWITCH, &mode))
	{
		close(fd);
		META_LOG("[Meta][SIM DETECT] Close ccci  node: handle = %d", fd);
		return 1;
	}

	META_LOG("[Meta][SIM DETECT] IOCTL CCCI_IOC_SIM_SWITCH failed");

    close(fd);
	return 0;	
}

unsigned int  FtModSIMDetect::GetPrjType(unsigned int &type)
{
	int fd = -1;
	
	if((fd = GetDevNode()) < 0)
		return 0;

	if (0 == ioctl(fd, CCCI_IOC_SIM_SWITCH_TYPE, &type))
	{
		close(fd);
		return 1;
	}

	META_LOG("[Meta][SIM DETECT] IOCTL CCCI_IOC_SIM_SWITCH_TYPE failed");

    close(fd);
	return 0;	
}

unsigned int  FtModSIMDetect::CheckExtMd()
{
	unsigned int ret = 0;
	char tempstr[128]={0};
	property_get("ro.boot.opt_c2k_support",tempstr,"0");
	if(strcmp(tempstr,"0")==0) {
//MTK_DT_SUPPORT=yes  && MTK_EXTERNAL_MODEM_SLOT > 0 && MTK_C2K_SUPPORT=no
#if ( defined(MTK_DT_SUPPORT) && defined(MTK_EXTERNAL_MODEM) )
		ret = 1;
#endif
	}
	META_LOG("[Meta][SIM DETECT] CheckExtMd ret = %d", ret);
	return ret;
}

unsigned int FtModSIMDetect::GetSSWValue()
{
	char buf[SIM_SWITCH_MODE_STR_LEN];
	int res = 0;
	unsigned int val = 0;
	int fd = open(SIM_SWITCH_MODE_FILE, O_RDONLY);

	META_LOG("[Meta][SIM DETECT] To open ssw file, fd = %d", fd);
	if (fd != -1)
	{
		memset(buf, 0, SIM_SWITCH_MODE_STR_LEN);
		res = read(fd, buf, SIM_SWITCH_MODE_STR_LEN);
		META_LOG("[Meta][SIM DETECT] To read ssw value, buf = %s", buf);
		if(res >= 3) //0x1, 0x2
			sscanf(buf, "0x%d", &val);
		close(fd);
	}

	META_LOG("[Meta][SIM DETECT] reture ssw value = %d", val);
	return val;	
}

///////////////////////////////////////////////////////////////////////////////////////////////
//for mdlogger ctrl

FtModTargetloggerCtrl::FtModTargetloggerCtrl(void)
	:CmdTarget(FT_TARGETLOG_CTRL_REQ_ID)
{
}

FtModTargetloggerCtrl::~FtModTargetloggerCtrl(void)
{
}

void FtModTargetloggerCtrl::exec(Frame *pFrm)
{
	META_LOG("[Meta][TARGETLOG CTRL] FtModTargetloggerCtrl");	

	FT_TARGETLOG_CTRL_CNF  ft_cnf;	

	memset(&ft_cnf, 0, sizeof(FT_TARGETLOG_CTRL_CNF));
	ft_cnf.status = META_FAILED;

	
	FT_TARGETLOG_CTRL_REQ *req = (FT_TARGETLOG_CTRL_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.type= req->type;

	META_LOG("[Meta][TARGETLOG CTRL] FtModTargetloggerCtrl req->type = %d", req->type);
	switch(req->type)
	{
		case FT_MDLOGGER_OP_SWITCH_TYPE:
			{
				if(SwitchMdloggerMode(req))
					ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MDLOGGER_OP_QUERY_STATUS:
			{
				ft_cnf.result.mdlogger_status_cnf.status = QueryMdloggerStatus();
				META_LOG("[Meta][TARGETLOG CTRL] mdlogger staus = %d", ft_cnf.result.mdlogger_status_cnf.status);
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MDLOGGER_OP_QUERY_NORMALLOG_PATH:
			{
				char prop[256]={0};
    			property_get("debug.mdl.run.folder", prop, NULL);
				if(prop[0] != 0)
				strncpy((char*)ft_cnf.result.mdlogger_logpath_cnf.path, prop, strlen(prop));			
				else
					ft_cnf.result.mdlogger_logpath_cnf.path[0] = 0;
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MDLOGGER_OP_QUERY_EELOG_PATH:
			{
				char prop[256]={0};
    			property_get("debug.mdl.EE.folder", prop, NULL);
				if(prop[0] != 0)
				strncpy((char*)ft_cnf.result.mdlogger_logpath_cnf.path, prop, strlen(prop));				
				else
					ft_cnf.result.mdlogger_logpath_cnf.path[0] = 0;				
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MOBILELOG_OP_SWITCH_TYPE:
			{
				if(SwitchMobilelogMode(req))
					ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MOBILELOG_OP_QUERY_LOG_PATH:
			{
				char prop[256]={0};
    			property_get("debug.MB.realpath", prop, NULL);
				if(prop[0] != 0)
					strncpy((char*)ft_cnf.result.mobilelog_logpath_cnf.path, prop, strlen(prop));				
				else
					ft_cnf.result.mobilelog_logpath_cnf.path[0] = 0;				
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_TARGETLOG_OP_PULL:
			{
				if(TargetLogPulling(req))
					ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_TARGETLOG_OP_PULLING_STATUS:
			{
				ft_cnf.result.targetlog_pulling_status_cnf.status = GetTargetLogPullingStatus(req);
				ft_cnf.status = META_SUCCESS;
			}
			break;
		case FT_MDLOGGER_OP_SET_FILTER:
			{
				if(SetModemLogFilter(req))
					ft_cnf.status = META_SUCCESS;
			}
			break;
		default:
			break;
	}

	WriteDataToPC(&ft_cnf, sizeof(FT_TARGETLOG_CTRL_CNF),NULL, 0);
}

int FtModTargetloggerCtrl::SwitchMdloggerMode(FT_TARGETLOG_CTRL_REQ *req)
{
	char msg[32] = {0};
	int mode   = req->cmd.mdlogger_ctrl_req.mode;
	int action = req->cmd.mdlogger_ctrl_req.action;
	META_LOG("[Meta][TARGETLOG CTRL] mode = %d, action = %d", mode, action);

	MSocket *pSocket = getSocket(SOCKET_MDLOGGER);
	if(pSocket == NULL)
	{
		pSocket = createSocket(SOCKET_MDLOGGER);
		if(pSocket != NULL)
		{
			int bInit = pSocket->init("com.mediatek.mdlogger.socket1", 0);
			if(bInit == 0)
			{
				delSocket(SOCKET_MDLOGGER);
				return 0;
			}
		}
		else
			return 0;
	}

	if(action)
		sprintf(msg, "meta_start,%d,", mode);
	else
		sprintf(msg, "meta_pause,");
	pSocket->send_msg(msg);
			
	return 1;			
}

unsigned int FtModTargetloggerCtrl::QueryMdloggerStatus()
{
	if(getPropValue("debug.mdlogger.Running") == 1)
		return 1;

	return 0;
}

int FtModTargetloggerCtrl::SwitchMobilelogMode(FT_TARGETLOG_CTRL_REQ *req)
{
	char msg[32] = {0};
	int mode   = req->cmd.mobilelog_ctrl_req.mode;
	int action = req->cmd.mobilelog_ctrl_req.action;
	META_LOG("[Meta][MOBILELOG CTRL] mode = %d, action = %d", mode, action);

	MSocket *pSocket = getSocket(SOCKET_MOBILELOG);
	if(pSocket == NULL)
	{
		pSocket = createSocket(SOCKET_MOBILELOG);
		if(pSocket != NULL)
		{
			int bInit = pSocket->init("mobilelogd", 0);
			if(bInit == 0)
			{
				delSocket(SOCKET_MOBILELOG);
				return 0;
			}
		}
		else
			return 0;
	}

	if(action)
		sprintf(msg, "meta_start");
	else
		sprintf(msg, "meta_pause");
	pSocket->send_msg(msg);
			
	return 1;			
}

int FtModTargetloggerCtrl::TargetLogPulling(FT_TARGETLOG_CTRL_REQ *req)
{
	char service[32] = {0};
	char msg[32]     = {0};
	
	int type   = req->cmd.targetlog_pull_req.type;
	int action = req->cmd.targetlog_pull_req.action;
	META_LOG("[Meta][TargetLogPulling] type = %d, action = %d", type, action);

	if(type == 0) //modem log
	{
		strncpy(service, MDLOG_SOCKET_NAME, strlen(MDLOG_SOCKET_NAME));
		if(action)
			strncpy(msg, MDLOG_PULL_START, strlen(MDLOG_PULL_START));
		else
			strncpy(msg, MDLOG_PULL_STOP, strlen(MDLOG_PULL_STOP));
	}
	else if(type == 1) //mobile log
	{
		strncpy(service, MBLOG_SOCKET_NAME, strlen(MBLOG_SOCKET_NAME));
		if(action)
			strncpy(msg, MBLOG_PULL_START, strlen(MBLOG_PULL_START));
		else
			strncpy(msg, MBLOG_PULL_STOP, strlen(MBLOG_PULL_STOP));
	}

	MSocket *pSocket = getSocket(type);
	if(pSocket == NULL)
	{
		pSocket = createSocket(type);
		if(pSocket != NULL)
		{
			int bInit = pSocket->init(service, 0);
			if(bInit == 0)
			{
				delSocket(type);
				return 0;
			}
		}
		else
			return 0;
	}

	pSocket->send_msg(msg);
			
	return 1;
	
}

int FtModTargetloggerCtrl::GetTargetLogPullingStatus(FT_TARGETLOG_CTRL_REQ *req)
{
	unsigned int status = 0;
	char property[32] = {0};
	int type = req->cmd.targetlog_pulling_status_req.type;
	META_LOG("[Meta][GetTargetLogPullingStatus] type = %d", type);

	switch(type)
	{
		case 0:
			strncpy(property, MDLOG_PULL_STATUS, strlen(MDLOG_PULL_STATUS));
			break;
		case 1:
			strncpy(property, MBLOG_PULL_STATUS, strlen(MBLOG_PULL_STATUS));
			break;
		case 2:
			strncpy(property, MDLOG_EE_PULL_STATUS, strlen(MDLOG_EE_PULL_STATUS));
			break;
		default:
			break;
	}

	status = GetLogPropValue(property);	
	META_LOG("[Meta][GetTargetLogPullingStatus] status = %d", status);

	return status;
	
}

unsigned int FtModTargetloggerCtrl::GetLogPropValue(char *key)
{
	char val[128]={0};
    property_get(key, val, "0");

	return atoi(val);
}

unsigned int FtModTargetloggerCtrl::SetModemLogFilter(FT_TARGETLOG_CTRL_REQ *req)
{
	char msg[32] = {0};
	
	META_LOG("[Meta][TARGETLOG CTRL] Set modem log filter");
	MSocket *pSocket = (MSocket*)GetLoggerObj(SOCKET_MDLOGGER, "com.mediatek.mdlogger.socket1");
	if(pSocket != NULL)
	{
		sprintf(msg, "meta_mdfilter,%d,", req->cmd.mdlogger_setfilter_req.type);
		pSocket->send_msg(msg);
		return 1;
	}

	return 0;	
}

void* FtModTargetloggerCtrl::GetLoggerObj(unsigned int type, const char * service)
{
	MSocket *pSocket = getSocket(type);
	if(pSocket == NULL)
	{
		pSocket = createSocket(type);
		if(pSocket != NULL)
		{
			int bInit = pSocket->init(service, 0);
			if(bInit == 0)
			{
				delSocket(type);
				return NULL;
			}
		}
		else
			return NULL;
	}

	return (void*)pSocket;
}

///////////////////////////////////////////////////////////////////////////////////////////////

FtModFileOperation::FtModFileOperation(void)
	:CmdTarget(FT_FILE_OPERATION_REQ_ID)
{
	m_nFileCount = 0;
}

FtModFileOperation::~FtModFileOperation(void)
{    
	ClearFileInfoList();
}

unsigned int FtModFileOperation::GetFileLen(char *pFilePath)
{
    struct stat st;
	if(stat((const char*)pFilePath, &st) < 0)
	{
	    META_LOG("[Meta][FT] GetFileLen pFilePath(%s) fail, errno=%d",pFilePath,errno);
	    return 0;
	}
	else
	{
	    return (unsigned int)st.st_size;
	}
}

void FtModFileOperation::ClearFileInfoList(void)
{
    mlist<FT_FILE_INFO*>::iterator it1 = m_fileInfoList.begin();	
	while (it1 != m_fileInfoList.end())
	{
		delete (*it1);
		++ it1;
	}
	m_fileInfoList.clear();
}


int FtModFileOperation::ListPath(unsigned char *pPath,unsigned char *pFileNameSubStr) 
{
	if(pPath == NULL)
	{
	    META_LOG("[Meta][FT] ListPath path is NULL");
		return 1;
	}

	if(pFileNameSubStr == NULL || pFileNameSubStr[0]  == '\0')
	{
	    META_LOG("[Meta][FT] ListPath file name substr is NULL or 0");
		return 1;
	}

	struct stat s;
	if(stat((const char*)pPath, &s) < 0)
	{
	    META_LOG("[Meta][FT] ListPath call stat fail, errno=%d",errno);
		return 1;
	}

	if(!S_ISDIR(s.st_mode))
	{
	    META_LOG("[Meta][FT] ListPath path(%s) is not a folder name",pPath);
		return 1;
	}

    char currfile[1024] = {0};
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    dir = opendir((const char*)pPath);
	if(dir == NULL)
    {
        META_LOG("[Meta][FT] ListPath opendir(%s) fail, errno=%d",pPath,errno);
        return 1;
    }
    
    m_nFileCount = 0;
	ClearFileInfoList();
    while((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".") == 0
          || strcmp(ptr->d_name,"..") ==0)
            continue;
		if(strlen(ptr->d_name) > 256)
		{
		    META_LOG("[Meta][FT] ListPath file name(%s) length is too large,just skip this file!!!!",ptr->d_name);
		    continue;
		}

        if(strcmp((const char*)pFileNameSubStr,"*") == 0
			|| strstr(ptr->d_name,(const char*)pFileNameSubStr) != NULL)
        {
		     FT_FILE_INFO *pFileInfo = new FT_FILE_INFO;
	         if(pFileInfo==NULL)
	         {		    
		         META_LOG("[Meta][FT] ListPath new FT_FILE_INFO fail,errno=%d",errno);
			     closedir(dir);
		         return 1;
	         }
		     memset(pFileInfo,0,sizeof(FT_FILE_INFO));		
             memcpy(pFileInfo->file_name, ptr->d_name, strlen(ptr->d_name));
			 if(pPath[strlen((const char*)pPath)-1] != '/')
			 {
			     sprintf(currfile,"%s/%s",(char*)pPath,ptr->d_name);
			 }
			 else
		     {
		         sprintf(currfile,"%s%s",(char*)pPath,ptr->d_name);
			 }
		
             if(ptr->d_type == DT_REG)  //file
             {
                 pFileInfo->file_type = FT_FILE_TYPE_FILE;
			     pFileInfo->file_size = GetFileLen(currfile);
             }
             else if(ptr->d_type == DT_DIR) //directory
             {
                 pFileInfo->file_type = FT_FILE_TYPE_FOLDER;
			     pFileInfo->file_size = 0;			
             }
			 else
			 {
			     pFileInfo->file_type = FT_FILE_TYPE_INVALID;
			     pFileInfo->file_size = 0;
			 }
		     m_nFileCount++;
		     META_LOG("[Meta][FT] ListPath find one file or folder,file_name=%s,file_type:%d,file_size:%d,m_nFileCount:%d",pFileInfo->file_name,pFileInfo->file_type,pFileInfo->file_size,m_nFileCount);
		     m_fileInfoList.push_back(pFileInfo);
        }

    }

	closedir(dir);
	return 0;
}

FT_FILE_INFO* FtModFileOperation::GetFileInfo(unsigned int id)
{
    mlist<FT_FILE_INFO*>::iterator it = m_fileInfoList.begin();
	unsigned int i = 0;

	while (it != m_fileInfoList.end())
	{
		
		if (i == id)
		{
			return (*it);
		}
		i++;
		++ it;
	}
	return NULL;
}



int FtModFileOperation::SaveSendData(FILE_OPERATION_SENDFILE_REQ *req, char *peer_buff, unsigned short peer_len)
{
    int nRet = 1;
    if(req == NULL || peer_buff == NULL)
    {
        return nRet;
    }
    if(req->dest_file_name[0] == '\0')
    {
      	return nRet;
    }
	  
    int SendFileFd = -1;
    unsigned int fileLen;
	META_LOG("[Meta][FT] SaveSendData receive block stage %x, file size %d!",req->stream_block.stage,req->stream_block.file_size);
	if(req->stream_block.stage & BLK_CREATE)
	{
		SendFileFd = open((const char*)req->dest_file_name, O_RDWR | O_TRUNC | O_CREAT, 0777);
		chown((const char*)req->dest_file_name,2000,1000);
	}
	else
	{
		SendFileFd = open((const char*)req->dest_file_name, O_RDWR | O_APPEND);
	}
	
	if(SendFileFd >= 0)
	{
		META_LOG("[Meta][FT] SaveSendData create or open file OK!");
		unsigned short sWriten = 0;
		sWriten = write(SendFileFd, peer_buff, peer_len);

		if(sWriten)
		{
			META_LOG("[Meta][FT] SaveSendData  write %d data total data %d!",sWriten,peer_len);
			if(req->stream_block.stage & BLK_EOF)
			{
				fileLen = getFileSize(SendFileFd);
				if(req->stream_block.file_size == fileLen)
				{
					META_LOG("[Meta][FT] SaveSendData write file BLK_EOF success! ");
					close(SendFileFd);
					SendFileFd = -1;
					nRet = 0;
				}
				else
				{
					META_LOG("[Meta][FT] SaveSendData file size(%d) error! ",req->stream_block.file_size);
				}
			}
			else
			{
			    META_LOG("[Meta][FT] SaveSendData write file BLK_WRITE success! ");
			    nRet = 0;
			}
		}
		else
		{
			META_LOG("[Meta][FT] SaveSendData write file failed!");
		}
	
		if(SendFileFd != -1)
			close(SendFileFd);
	
	}
	else
	{
		META_LOG("[Meta][FT] SaveSendData create or open file failed!");
	}
	return nRet;
	
}

#define RECEIVE_PEER_MAX_LEN 20*1024

int FtModFileOperation::SetReceiveData(FILE_OPERATION_RECEIVEFILE_REQ *req, FT_FILE_OPERATION_CNF* pft_cnf)
{
    int nRet = 1;
	if(req == NULL || pft_cnf == NULL)
	{
		return nRet;
	}
	if(req->source_file_name[0]  == '\0')
	{
		return nRet;
	}

    unsigned int nReceiveDataSize = 0;
    int ReceiveFileFd = -1;
	int nPeerBuffSize = 0;
    char* pPeerBuf = NULL;
	
	ReceiveFileFd = open((const char*)req->source_file_name, O_RDONLY);
        unsigned int nFileLen = getFileSize(ReceiveFileFd);
  
	if(ReceiveFileFd >= 0)
	{
            META_LOG("[Meta][FT] SetReceiveData open file %s succeed, fileSize %d ! ",req->source_file_name,nFileLen);

		pPeerBuf = (char*)malloc(RECEIVE_PEER_MAX_LEN);
		memset(pPeerBuf, 0, RECEIVE_PEER_MAX_LEN);

		pft_cnf->result.receivefile_cnf.stream_block.stage = BLK_CREATE;

		while(!(pft_cnf->result.receivefile_cnf.stream_block.stage & BLK_EOF))
		{
			nPeerBuffSize = read(ReceiveFileFd, pPeerBuf, RECEIVE_PEER_MAX_LEN);

                        META_LOG("[Meta][FT] SetReceiveData nPeerBuffSize:%d,nReceiveDataSize:%d,",nPeerBuffSize,nReceiveDataSize);
			if(nPeerBuffSize != -1)
			{
				pft_cnf->status = META_SUCCESS;
				pft_cnf->result.receivefile_cnf.receive_result = 0;
				if(nPeerBuffSize == 0)
				{
					pft_cnf->result.receivefile_cnf.stream_block.stage |= BLK_EOF;
					pft_cnf->result.receivefile_cnf.stream_block.file_size = nReceiveDataSize;
					WriteDataToPC(pft_cnf, sizeof(FT_FILE_OPERATION_CNF),NULL, 0);
					META_LOG("[Meta][FT] SetReceiveData file end, set BLK_EOF! ");
					nRet = 0;
					break;
				}
				else
				{
					pft_cnf->result.receivefile_cnf.stream_block.stage |= BLK_WRITE;
					pft_cnf->result.receivefile_cnf.stream_block.file_size = nReceiveDataSize;
					META_LOG("[Meta][FT] SetReceiveData File set %d data ! ",nPeerBuffSize);
					WriteDataToPC(pft_cnf, sizeof(FT_FILE_OPERATION_CNF),pPeerBuf, nPeerBuffSize);
					memset(pPeerBuf,0,RECEIVE_PEER_MAX_LEN);
					pft_cnf->result.receivefile_cnf.stream_block.stage &= ~BLK_CREATE;
					META_LOG("[Meta][FT] SetReceiveData set BLK_WRITE! ");
				}
				nReceiveDataSize = nReceiveDataSize + nPeerBuffSize;

			}
			else
			{
				pft_cnf->result.receivefile_cnf.stream_block.stage |= BLK_EOF;
				META_LOG("[Meta][FT] SetReceiveData read file fail, set BLK_EOF! ");
			}

		}

		free(pPeerBuf);
		close(ReceiveFileFd);

	}
	else
	{
	    META_LOG("[Meta][FT] SetReceiveData open File %s failed, errno=%d",req->source_file_name,errno);
	}

	return nRet;
}
void FtModFileOperation::exec(Frame *pFrm)
{
	META_LOG("[Meta][FILE OPERATION] FtModFileOperation");	

	FT_FILE_OPERATION_CNF  ft_cnf;

	memset(&ft_cnf, 0, sizeof(FT_FILE_OPERATION_CNF));
	ft_cnf.status = META_FAILED;

	
	FT_FILE_OPERATION_REQ *req = (FT_FILE_OPERATION_REQ *)pFrm->localBuf();

	ft_cnf.header.id = req->header.id +1;
    ft_cnf.header.token = req->header.token;
    ft_cnf.type= req->type;

	META_LOG("[Meta][FILE OPERATION] FtModFileOperation req->type = %d", req->type);
	switch(req->type)
	{
		case FT_FILE_OP_PARSE:
			{
				if(ListPath(req->cmd.parse_req.path_name,req->cmd.parse_req.filename_substr) == 0)
				{
				    ft_cnf.result.parse_cnf.file_count = m_nFileCount;
				    ft_cnf.status = META_SUCCESS;
				    META_LOG("[Meta][FILE OPERATION] FtModFileOperation parse folder success!");
				}
				else
				{
				    META_LOG("[Meta][FILE OPERATION] FtModFileOperation parse folder fail!");
				}
			}
			break;
		case FT_FILE_OP_GETFILEINFO:
			{
				if(req->cmd.getfileinfo_req.index > m_nFileCount)
				{
				    META_LOG("[Meta][FILE OPERATION] FtModFileOperation invalid file index!");
				}
				else
				{
				    FT_FILE_INFO* pFileInfo = GetFileInfo(req->cmd.getfileinfo_req.index);
				    if(pFileInfo != NULL)
				    {				  
				        memcpy(&(ft_cnf.result.getfileinfo_cnf.file_info),pFileInfo,sizeof(FT_FILE_INFO));
						META_LOG("[Meta][FILE OPERATION] file_name=%s,file_type:%d,file_size:%d",pFileInfo->file_name,pFileInfo->file_type,pFileInfo->file_size);
				        ft_cnf.status = META_SUCCESS;
				    }
				}
			}
			break;
		case FT_FILE_OP_SENDFILE:
			{
				if(SaveSendData(&(req->cmd.sendfile_req),(char *)pFrm->peerBuf(), pFrm->peerLen()) == 0)
				{
				    ft_cnf.result.sendfile_cnf.send_result = 0;
				    ft_cnf.status = META_SUCCESS;					   
				}
			}
			break;
		case FT_FILE_OP_RECEIVEFILE:
			{
				if(SetReceiveData(&(req->cmd.receivefile_req),&ft_cnf) == 0)
		        {
			        META_LOG("[Meta][FILE OPERATION] FtModFileOperation success to set receive data!");
					return;
		        }
	            else
	            {
		            META_LOG("[Meta][FILE OPERATION] FtModFileOperation Failed to set receive data!");
	            }
			}
			break;
		default:
			break;
	}

	WriteDataToPC(&ft_cnf, sizeof(FT_FILE_OPERATION_CNF),NULL, 0);
}

#ifdef FT_RAT_FEATURE

FtModRAT::FtModRAT(void)
	:CmdTarget(FT_RATCONFIG_REQ_ID)
{
}

FtModRAT::~FtModRAT(void)
{
}

int FtModRAT::init(Frame*pFrm)
{
	RAT_CNF ft_cnf;
    static int bInitFlag_RAT = false;

    memset(&ft_cnf, 0, sizeof(RAT_CNF));
	RAT_REQ *req = (RAT_REQ *)pFrm->localBuf();

    if (FALSE == bInitFlag_RAT)
    {
        // initial the bat module when it is called first time
        if (!META_RAT_init())
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;

            META_LOG("[Meta][FT] FT_RAT_OP META_RAT_init Fail ");
            WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

			return false;
        }
        bInitFlag_RAT = true;
    }

	return true;
}


void FtModRAT::exec(Frame *pFrm)
{
	META_LOG("[Meta][FT] FT_RAT_OP FtModRAT ");

	CmdTarget::exec(pFrm);
	if(getInitState())
		META_RAT_OP((RAT_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}

#endif


#ifdef FT_MSIM_FEATURE
FtModMSIM::FtModMSIM(void)
	:CmdTarget(FT_MSIM_REQ_ID)
{
}

FtModMSIM::~FtModMSIM(void)
{
}

int FtModMSIM::init(Frame*pFrm)
{
	MSIM_CNF ft_cnf;
    static int bInitFlag_MSIM = false;

    memset(&ft_cnf, 0, sizeof(MSIM_CNF));
	MSIM_REQ *req = (MSIM_REQ *)pFrm->localBuf();

    if (FALSE == bInitFlag_MSIM)
    {
        // initial the bat module when it is called first time
        if (!META_MSIM_init())
        {
            ft_cnf.header.id = req->header.id +1;
            ft_cnf.header.token = req->header.token;
            ft_cnf.status = META_FAILED;

            META_LOG("[Meta][FT] FT_MSIM_OP META_MSIM_init Fail ");
            WriteDataToPC(&ft_cnf, sizeof(ft_cnf),NULL, 0);

			return false;
        }
        bInitFlag_MSIM = true;
    }

	return true;
}


void FtModMSIM::exec(Frame *pFrm)
{
	META_LOG("[Meta][FT] FT_MSIM_OP FtModMSIM");

	CmdTarget::exec(pFrm);
	if(getInitState())
		META_MSIM_OP((MSIM_REQ *)pFrm->localBuf(), (char *)pFrm->peerBuf(), pFrm->peerLen());
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////

BOOL Meta_Mobile_Log()
{        
    int fd = 0;
	int len = 0;
	BOOL ret = FALSE;
	char tempstr[5]={0};
	META_LOG("[Meta][FT] Meta_Mobile_Log ");

    //support end load and user load,send stop command to mobilelog 

	fd = socket_local_client("mobilelogd", ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
	if (fd < 0) 
	{
		META_LOG("[Meta][FT] socket fd <0 ");
		return FALSE;
	}
	META_LOG("[Meta][FT] socket ok\n");
	if((len = write(fd, "stop", sizeof("stop"))) < 0)
	{
		META_LOG("[Meta][FT] socket write error!");
		ret = FALSE;
	}
	else
	{
		META_LOG("[Meta][FT] write %d Bytes.", len);
		ret = TRUE;
	}
	close(fd);
	sleep(4);	      
	return ret;
	
}

void FT_UtilCheckIfFuncExist(FT_UTILITY_COMMAND_REQ  *req, FT_UTILITY_COMMAND_CNF  *cnf)
{

    unsigned int	query_ft_msg_id = req->cmd.CheckIfFuncExist.query_ft_msg_id;
    unsigned int	query_op_code = req->cmd.CheckIfFuncExist.query_op_code;
    META_LOG("[Meta][FT] FT_UtilCheckIfFuncExist META Test ");
    cnf->status = FT_CNF_FAIL;

	META_LOG("[Meta][FT] request id = %d op = %d",query_ft_msg_id,query_op_code);

	
    switch (query_ft_msg_id)
    {

#ifdef FT_FM_FEATURE 
    	case FT_FM_REQ_ID:
			if(query_op_code == 0)//FT_FM_OP_READ_CHIP_ID
			{
				cnf->status = FT_CNF_OK; 
			}        	
        	break;
#endif

#ifdef FT_CRYPTFS_FEATURE
		case FT_CRYPTFS_REQ_ID:
			if(query_op_code == 0)//FT_CRYPTFS_OP_QUERYSUPPORT
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == 1)//FT_CRYPTFS_OP_VERITIFY
			{
				cnf->status = FT_CNF_OK;
			}
        	break;
#endif
			
		case FT_MODEM_REQ_ID:
			if(query_op_code == FT_MODEM_OP_QUERY_INFO )
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_MODEM_OP_CAPABILITY_LIST)
			{
				cnf->status = FT_CNF_OK;
			}
#ifdef MTK_EXTMD_NATIVE_DOWNLOAD_SUPPORT
			else if(query_op_code == FT_MODEM_OP_QUERY_MDDOWNLOADSTATUS)
			{
				cnf->status = FT_CNF_OK;
			}
#endif
			else if(query_op_code == FT_MODEM_OP_SET_MODEMTYPE)
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_MODEM_OP_GET_CURENTMODEMTYPE)
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_MODEM_OP_QUERY_MDIMGTYPE )
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_MODEM_OP_QUERY_MDDBPATH)
			{
			    cnf->status = FT_CNF_OK;
			}
			break;

		case FT_L4AUD_REQ_ID:
			if(query_op_code == 59)//FT_L4AUD_OP_SPEAKER_CALIBRATION_SUPPORT
			{
#ifdef MTK_SPEAKER_MONITOR_SUPPORT
				cnf->status = FT_CNF_OK;
				META_LOG("[Meta][FT] MTK_SPEAKER_MONITOR_SUPPORT = yes");
#else
				cnf->status = FT_CNF_FAIL;
				META_LOG("[Meta][FT] MTK_SPEAKER_MONITOR_SUPPORT = no");
#endif
			}

			break;
		case FT_SIM_DETECT_REQ_ID:
			if(query_op_code == FT_SIM_DETECT_OP_EXTMOD)
			{
				cnf->status = FT_CNF_OK;
				META_LOG("[Meta][FT] FT_SIM_DETECT_OP_EXTMOD = yes");
			}
			break;
		case FT_TARGETLOG_CTRL_REQ_ID:
			if(query_op_code <= FT_MDLOGGER_OP_SET_FILTER)
			{
				if(getDataChannelType() == 1) //93 modem ccb channel
				{
				cnf->status = FT_CNF_OK;
					META_LOG("[Meta][FT] FT_MDLOGGER_OP = yes");
			}
				else
				{
					META_LOG("[Meta][FT] FT_MDLOGGER_OP = no");
				}
			}
			break;			
		case FT_TARGETCLOCK_REQ_ID:
			if(query_op_code <= FT_CLOCK_GET)
			{
				cnf->status = FT_CNF_OK;
				META_LOG("[Meta][FT] FT_TARGETCLOCK_OP = yes");
			}
			break;					
		case FT_UTILITY_COMMAND_REQ_ID:
			if(query_op_code == FT_UTILCMD_QUERY_WCNDRIVER_READY)
			{
			    cnf->status = FT_CNF_OK;
				META_LOG("[Meta][FT] FT_UTILCMD_QUERY_WCNDRIVER_READY = yes");
			}
			else if(query_op_code == FT_UTILCMD_SWITCH_WIFI_USB)
			{
			    cnf->status = FT_CNF_OK;
				META_LOG("[Meta][FT] FT_UTILCMD_SWITCH_WIFI_USB = yes");
			}
			break;
		case FT_FILE_OPERATION_REQ_ID:
			if(query_op_code == FT_FILE_OP_PARSE )
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_FILE_OP_GETFILEINFO)
			{
			    cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_FILE_OP_SENDFILE)
			{
			    cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == FT_FILE_OP_RECEIVEFILE)
			{
				cnf->status = FT_CNF_OK;
			}
            break;			
		case FT_RATCONFIG_REQ_ID:
#ifdef FT_RAT_FEATURE
			if(query_op_code == RAT_OP_READ_OPTR )
			{
				cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == RAT_OP_READ_OPTRSEG)
			{
			    cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == RAT_OP_GET_CURRENT_RAT)
			{
			    cnf->status = FT_CNF_OK;
			}
			else if(query_op_code == RAT_OP_SET_NEW_RAT)
			{
			    cnf->status = FT_CNF_OK;
			}
#else
            cnf->status = FT_CNF_FAIL;
#endif
            break;
		case FT_MSIM_REQ_ID:
#ifdef FT_MSIM_FEATURE
            if(query_op_code == MSIM_OP_GET_MSIM )
            {
				cnf->status = FT_CNF_OK;
            }
            else if(query_op_code == MSIM_OP_SET_MSIM)
            {
				cnf->status = FT_CNF_OK;
            }
#else
            cnf->status = FT_CNF_FAIL;
#endif
            break;
        case FT_APDB_REQ_ID:
#ifdef FT_NVRAM_FEATURE
            if(query_op_code == FT_APDB_OP_QUERYPATH )
            {
				cnf->status = FT_CNF_OK;
            }
#else
            cnf->status = FT_CNF_FAIL;
#endif
			break;
    	default:
			META_LOG("[Meta][FT] NOT FOUND THE PRIMITIVE_ID");
        	cnf->status = FT_CNF_FAIL;		
        	break;
    }
    

    // assign return structure
    cnf->result.CheckIfFuncExist.query_ft_msg_id = query_ft_msg_id;
    cnf->result.CheckIfFuncExist.query_op_code = query_op_code;
}

FtModUtility::FtModUtility(void)
	:CmdTarget(FT_UTILITY_COMMAND_REQ_ID)
{
}

FtModUtility::~FtModUtility(void)
{
}

void FtModUtility::exec(Frame *pFrm)
{
	CmdTarget::exec(pFrm);  
	
	FT_UTILITY_COMMAND_CNF UtilityCnf;
    //PROCESS_INFORMATION cleanBootProcInfo;
    static META_BOOL bLCDBKInitFlag_Peri = FALSE;
    static META_BOOL bLCDFtInitFlag_Peri = FALSE;
    static META_BOOL bVibratorInitFlag_Peri = FALSE;
	int nNVRAMFlag = 0;
	unsigned int level = 0;
	int ret = 0;
	char tempstr[128]={0};

    //cleanBootProcInfo.hProcess = NULL;
    //cleanBootProcInfo.hThread = NULL;

    META_LOG("[Meta][FT] FT_Peripheral_OP META Test ");

    memset(&UtilityCnf, 0, sizeof(FT_UTILITY_COMMAND_CNF));
	FT_UTILITY_COMMAND_REQ *req = (FT_UTILITY_COMMAND_REQ *)pFrm->localBuf();

    UtilityCnf.header.id = req->header.id +1;
    UtilityCnf.header.token = req->header.token;
    UtilityCnf.type = req->type;
    UtilityCnf.status= META_FAILED;

    //do the related test.
    switch (req->type)
    {
    case FT_UTILCMD_CHECK_IF_FUNC_EXIST:		//query the supported modules
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_CHECK_IF_FUNC_EXIST ");
        FT_UtilCheckIfFuncExist(req, &UtilityCnf);
        break;
    //delete since no use.
    //case FT_UTILCMD_QUERY_LOCAL_TIME:			//query RTC from meta cpu lib
    //    META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_QUERY_LOCAL_TIME ");
    //    UtilityCnf.result.m_WatchDogCnf= META_RTCRead_OP(req->cmd.m_WatchDogReq);
    //    UtilityCnf.status= META_SUCCESS;
    //    break;

#ifdef FT_LCDBK_FEATURE
    case FT_UTILCMD_MAIN_SUB_LCD_LIGHT_LEVEL:	//test lcd backlight from meta lcd backlight lig
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_MAIN_SUB_LCD_LIGHT_LEVEL ");
	    if(bLCDBKInitFlag_Peri==FALSE)
     	{   		
			if (!Meta_LCDBK_Init())
			{
				META_LOG("[Meta][FT] FT_Peripheral_OP Meta_LCDBK_Init Fail ");
				goto Per_Exit;
			}   		
		    bLCDBKInitFlag_Peri = TRUE;
    	}        
        UtilityCnf.result.m_LCDCnf = Meta_LCDBK_OP(req->cmd.m_LCDReq);
        UtilityCnf.status= META_SUCCESS;
        break;
#endif

#ifdef FT_LCD_FEATURE
	case FT_UTILCMD_LCD_COLOR_TEST:
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_LCD_COLOR_TEST ");
	    if(bLCDFtInitFlag_Peri==FALSE)
     	{   					
			if (!Meta_LCDFt_Init())
			{
			    META_LOG("[Meta][FT]] FT_Peripheral_OP Meta_LCDFt_Init Fail ");
			    goto Per_Exit;
			}			
		    bLCDFtInitFlag_Peri = TRUE;
    	}        
        UtilityCnf.result.m_LCDColorTestCNF = Meta_LCDFt_OP(req->cmd.m_LCDColorTestReq);
        UtilityCnf.status= META_SUCCESS;
        break;
#endif

#ifdef FT_VIBRATOR_FEATURE
    case FT_UTILCMD_SIGNAL_INDICATOR_ONOFF:
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SIGNAL_INDICATOR_ONOFF ");
        UtilityCnf.result.m_NLEDCnf = Meta_Vibrator_OP(req->cmd.m_NLEDReq);
        UtilityCnf.status= META_SUCCESS;
        break;
#endif

		
#ifdef FT_VIBRATOR_FEATURE
    case FT_UTILCMD_VIBRATOR_ONOFF:				//test vibrate and indicator from meta nled lib
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_VIBRATOR_ONOFF ");
	    if(bVibratorInitFlag_Peri==FALSE)
     	{
    		if (!Meta_Vibrator_Init())
        	{
            	META_LOG("[Meta][FT] FT_Peripheral_OP Meta_Vibrator_Init Fail ");
            	goto Per_Exit;
        	}	
		    bVibratorInitFlag_Peri = TRUE;
    	}         
        UtilityCnf.result.m_NLEDCnf = Meta_Vibrator_OP(req->cmd.m_NLEDReq);
        UtilityCnf.status= META_SUCCESS;
        break;
#endif

#ifdef FT_VIBRATOR_FEATURE
    case FT_UTILCMD_KEYPAD_LED_ONOFF:	
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_KEYPAD_LED_ONOFF ");
        UtilityCnf.result.m_NLEDCnf = Meta_Vibrator_OP(req->cmd.m_NLEDReq);
        UtilityCnf.status= META_SUCCESS;
        break;
#endif

      case FT_UTILCMD_SET_CLEAN_BOOT_FLAG:
        nNVRAMFlag = req->cmd.m_SetCleanBootFlagReq.Notused;
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SET_CLEAN_BOOT_FLAG, nNVRAMFlag =%d",nNVRAMFlag);
        if ((req->cmd.m_SetCleanBootFlagReq.BackupTime)[0] != '\0')
        {
            META_LOG("[Meta][FT] FT_UTILCMD_SET_CLEAN_BOOT_FLAG, BackupTime =%s",req->cmd.m_SetCleanBootFlagReq.BackupTime);
            UtilityCnf.result.m_SetCleanBootFlagCnf.drv_statsu = FileOp_BackupToBinRegion_All_Exx(req->cmd.m_SetCleanBootFlagReq.BackupTime);
        }
        else
        {
            //For NVRAM to record write barcode(1) and IMEI(2) and both barcode and IMEI(3) history
            if ( nNVRAMFlag == 1 || nNVRAMFlag == 2 || nNVRAMFlag == 3 )
            {
                UtilityCnf.result.m_SetCleanBootFlagCnf.drv_statsu = FileOp_BackupToBinRegion_All_Ex(nNVRAMFlag);
            }
            else
            {
                UtilityCnf.result.m_SetCleanBootFlagCnf.drv_statsu = FileOp_BackupToBinRegion_All();
            }
        }
        UtilityCnf.status=META_SUCCESS;
        break;

    case FT_UTILCMD_CHECK_IF_LOW_COST_SINGLE_BANK_FLASH:	//query the single flash feature, we now just return.
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_CHECK_IF_LOW_COST_SINGLE_BANK_FLASH ");
        UtilityCnf.status=META_SUCCESS;
        break;

    case FT_UTILCMD_SAVE_MOBILE_LOG:                           //save mobile log
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SAVE_MOBILE_LOG ");
        META_LOG("[Meta][FT] FT_UTILCMD_SAVE_MOBILE_LOG META Test %s,%d,%s",__FILE__,__LINE__,__FUNCTION__);
        UtilityCnf.result.m_SaveMobileLogCnf.drv_status = Meta_Mobile_Log();
        UtilityCnf.status = META_SUCCESS;
		break;
	case FT_UTILCMD_SET_LOG_LEVEL:	  
		level = req->cmd.m_SetLogLevelReq.level;
		META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SET_LOG_LEVEL ");
		META_LOG("[FTT_Drv:] FT_UTILCMD_SET_LOG_LEVEL META Test %s,%d,%s,level = %d",__FILE__,__LINE__,__FUNCTION__,level);
		setLogLevel(level);
		UtilityCnf.status = META_SUCCESS;
		break;
	case FT_UTILCMD_SDIO_AUTO_CALIBRATION:	
		META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SDIO_AUTO_CALIBRATION ");
		META_LOG("[FTT_Drv:] FT_UTILCMD_SDIO_AUTO_CALIBRATION META Test: no more supported");
		break;
	case FT_UTILCMD_QUERY_WCNDRIVER_READY:
		META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_QUERY_WCNDRIVER_READY ");
		property_get("service.wcn.driver.ready",tempstr,"no");
		if(strcmp(tempstr,"yes")==0)
		{
		    UtilityCnf.result.m_QueryWCNDriverReadyCnf.result = 1; //has ready
		    META_LOG("[FTT_Drv:] FT_UTILCMD_QUERY_WCNDRIVER_READY() wcn driver ready");
		}
		else
		{
		    UtilityCnf.result.m_QueryWCNDriverReadyCnf.result = 0;  //not ready
		    META_LOG("[FTT_Drv:] FT_UTILCMD_QUERY_WCNDRIVER_READY() wcn driver not ready");
		}
		UtilityCnf.status = META_SUCCESS;
		break;
	case FT_UTILCMD_SET_ATM_FLAG:
		META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SET_ATM_FLAG ");

		META_LOG("[Meta][FT] setNormalModeTestFlag");
	
		if(0 == setNormalModeTestFlag(req->cmd.m_SetATMFlagReq.flag))
		{
			UtilityCnf.status = META_SUCCESS;
		}
		else
		{
	    	UtilityCnf.status = META_FAILED;
		}
		break;
	case FT_UTILCMD_SET_PRODUCT_INFO:		
		{
			int offset = 0;
			META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SET_PRODUCT_INFO ");
		
			switch(req->cmd.m_SetProductInfo.type)
			{	
				case 0: //ATM flag
					offset = OFFSET_ATM;
					break;
				case 1: //meta log flag
					offset = OFFSET_METALOG;
					break;
				default:
					break;
			}
			
			META_LOG("[Meta][FT] setProductInfo, type = %d, offset = %d", req->cmd.m_SetProductInfo.type, offset);
			if(0 == setProductInfo(req->cmd.m_SetProductInfo.type, req->cmd.m_SetProductInfo.flag, offset))
			{
				UtilityCnf.status = META_SUCCESS;
			}
			else
			{
	    		UtilityCnf.status = META_FAILED;
			}
		}
		break;
	case FT_UTILCMD_SWITCH_WIFI_USB:
		META_LOG("[FTT_Drv:] FT_Peripheral_OP pFTReq->type is FT_UTILCMD_SWITCH_WIFI_USB ");
		//send response to PC
		UtilityCnf.result.m_SwitchWiFiUSBCnf.result = 1;
		UtilityCnf.status = META_SUCCESS;
		WriteDataToPC(&UtilityCnf, sizeof(FT_UTILITY_COMMAND_CNF),NULL, 0);
		//destroy related resource
		destroyVirtualRxThread();
		//set property and then create resource
		if(req->cmd.m_SwitchWiFiUSBReq.flag == 0) //wifi to usb
		{
			property_set("persist.meta.connecttype","usb");
			META_LOG("[FTT_Drv:] persist.meta.connecttype = usb");
			setComType(META_USB_COM);
		}
		else if(req->cmd.m_SwitchWiFiUSBReq.flag == 1) //usb to wifi
		{
		    property_set("persist.meta.connecttype","wifi");
			META_LOG("[FTT_Drv:] persist.meta.connecttype = wifi");
			setComType(META_SOCKET);
		}
		else
		{
		    META_LOG("[FTT_Drv:] FT_UTILCMD_SWITCH_WIFI_USB flag = %d is valid",req->cmd.m_SwitchWiFiUSBReq.flag);
		}
		createVirtualRxThread();
		return;
    default:
        META_LOG("[Meta][FT] FT_Peripheral_OP pFTReq->type error ");
        UtilityCnf.status= META_FAILED;
        break;

    }

Per_Exit:
    WriteDataToPC(&UtilityCnf, sizeof(FT_UTILITY_COMMAND_CNF),NULL, 0);

}

/////////////////////////////////////////////////////////////////////////////////////////



#ifdef MICROTRUST_FACTORY_SUPPORT
/*
 * name: check_active_status
 * function: check Microtrust soter status
 */
static void check_active_status(char *data_out, int *data_out_len)
{
    int result = 0;
    char property_result[128] = { 0 };
    *data_out_len = 1;
    data_out[0] = 3;

    META_LOG("[Meta][Microtrust] check_active_status in");

	property_get("soter.teei.thh.init", property_result, NULL);
    if (0 == strncmp(property_result, "ACTIVE", strlen("ACTIVE")))
    {
        data_out[0] = 0;
        META_LOG("[Meta][Microtrust] check_active_status out, check status ok, status is %s", property_result);
    }
    else
    {
        data_out[0] = 1;
        META_LOG("[Meta][Microtrust] check_active_status out, check status failed, status is %s", property_result);
    }

    return;
}

/*
 * name: check_soter_status
 * function: check Microtrust soter status
 */
static void check_soter_status(char *data_out, int *data_out_len)
{
    int result = 0;
    char property_result[128] = { 0 };
    *data_out_len = 1;
    data_out[0] = 3;

    META_LOG("[Meta][Microtrust] check_soter_status in");

	property_get("soter.teei.init", property_result, NULL);
    if (0 == strncmp(property_result, "INIT_OK", strlen("INIT_OK")))
    {
        data_out[0] = 0;
        META_LOG("[Meta][Microtrust] check_soer_status out, check status ok, status is %s", property_result);
    }
    else
    {
        data_out[0] = 1;
        META_LOG("[Meta][Microtrust] check_soter_status out, check status failed, status is %s", property_result);
    }

    return;
}

/*
 * name: check_init_thh_status
 * function: check Microtrust factory status
 */
static void check_init_thh_status(char *data_out, int *data_out_len)
{
    int result = 0;
    *data_out_len = 1;
    data_out[0] = 3;

    META_LOG("[Meta][Microtrust] check_init_thh_status in");

    result = access("/data/thh/tee_00/thh_startload.lock", F_OK);
    if (result == 0)
    {
        data_out[0] = 2;
        META_LOG("[Meta][Microtrust] check_init_thh_status out, lock file is exist");
        return;
    }

    system("init_thh initcheck");

    result = access("/data/thh/tee_00/init_thh_SUCCESS", F_OK);
    if (result == 0)
    {
        data_out[0] = 0;
        remove("/data/thh/tee_00/init_thh_SUCCESS");
        META_LOG("[Meta][Microtrust] check_init_thh_status out, check status ok");

        return;
    }

    result = access("/data/thh/tee_00/init_thh_FAILED", F_OK);
    if (result == 0)
    {
        data_out[0] = 1;
        remove("/data/thh/tee_00/init_thh_FAILED");
        META_LOG("[Meta][Microtrust] check_init_thh_status out, check status failed");

        return;
    }

    return;
}

/*
 * name: read_file_to_buffer
 * function:read file content to buffer
 */
static void read_file_to_buffer(const char *file_path, char *data_out, int file_seek_counter, int *data_out_len)
{
    int fseek_result = 0;
    size_t read_bytes_count = 0;

    FILE *read_stream = fopen(file_path, "rb");
    if (read_stream == NULL)
    {
        META_LOG("[Meta][Microtrust]read_file_to_buffer fopen file return NULL, file path is %s", file_path);
        data_out[0] = 1;
        *data_out_len = 1;

        goto END;
    }

    fseek_result = fseek(read_stream, DATA_OUT_BUFFER_SIZE*file_seek_counter, SEEK_SET);
    if (fseek_result != 0)
    {
        META_LOG("[Meta][Microtrust]read_file_to_buffer, fseek file failed");
        data_out[0] = 2;
        *data_out_len = 1;

        goto END;
    }

    if (!feof(read_stream))
    {
        read_bytes_count = fread(&data_out[2], 1, DATA_OUT_BUFFER_SIZE, read_stream);
        if (read_bytes_count == DATA_OUT_BUFFER_SIZE)
        {
            data_out[1] = file_seek_counter + 1;
        }
        else
        {
            data_out[1] = 0;
        }
        *data_out_len = read_bytes_count + 2;
    }
    data_out[0] = 0;

END:
    if (read_stream)
    {
        fclose(read_stream);
        read_stream = NULL;
    }

    return;
}

/*
 * name: write_buffer_to_file
 * function: write buffer content to file
 */
static void write_buffer_to_file(const void *buffer, const char *filepath, int write_file_len, char *data_out, int *data_out_len)
{
    FILE *write_stream = NULL;
    size_t ret = 0;
    *data_out_len = 1;

    write_stream = fopen(filepath, "aw+");
    if (write_stream == NULL)
    {
        META_LOG("[Meta][Microtrust]write_buffer_to_file fopen file return  NULL, file path is: %s", filepath);
        data_out[0] = 1;

        return;
    }
    ret = fwrite(buffer, 1, write_file_len, write_stream);
    if (ret < write_file_len)
    {
        META_LOG("[Meta][Microtrust]write_buffer_to_file fwrite file failed, buffer length is: %d, return value is: %d", write_file_len, ret);
        data_out[0] = 2;
        fclose(write_stream);

        return;
    }

    if (write_stream)
    {
        fclose(write_stream);
        write_stream = NULL;
    }
    data_out[0] = 0;

    return;
}

/*
 * name: remove_file
 * function: remove files
 */
static void remove_file(const char *filepath, unsigned int filepath_len, char *data_out, int *data_out_len)
{
    int result = 0;
    *data_out_len = 1;
    char remove_filepath[512] = { 0 };

    memcpy(remove_filepath, "rm ", 3);
    strncat(remove_filepath, filepath, filepath_len);

    result = system(remove_filepath);

    if (result == 0)
    {
        data_out[0] = 0;
    }
    else
    {
        data_out[0] = 1;
    }

    return;
}

/*
 * name: call_phone_program
 * function: call Microtrust product line command for export uuids and pubkeys
 */
static void call_phone_program(char *command, char *data_out, int *data_out_len)
{
    int result = 0;
    *data_out_len = 1;

    META_LOG("[Meta][Microtrust] call_phone_program in, command is: %s", command);

    result = system(command);
    if (result == 0)
    {
        data_out[0] = 0;
    }
    else
    {
        data_out[0] = 1;
    }

    META_LOG("[Meta][Microtrust] call_phone_program out");

    return;
}

/*
 * name: test_file_exist
 * function: test if exist file
 */
static void test_file_exist(const char *filepath, char *data_out, int *data_out_len)
{
    int result[256] = { 0 };
    char folder[256] = { 0 };
    int i = 0;
    int j = 0;
    *data_out_len = 1;

    META_LOG("[Meta][Microtrust] test_file_exist in");
    while (filepath[i] != '\0')
    {
        if (filepath[i] == '/')
        {
            memcpy(folder, filepath, i + 1);
            result[j] = access(folder, F_OK);
            j++;
        }
        i++;
    }
    if (result[j - 1] == 0)
    {
        data_out[0] = 0;  // the file exist
    }
    else
    {
        data_out[0] = 1;  // the folder no exist

        return;
    }

    if (access(filepath, F_OK) == -1)
    {
        data_out[0] = 2;  // the folder exist but file not exist
        META_LOG("[Meta][Microtrust] the folder exist but file not exist");
    }

    META_LOG("[Meta][Microtrust] test_file_exist out");

    return;
}

/*
 * name: get_file_length
 * function: get file length
 */
static void get_file_length(const char *filepath, char *data_out, int *data_out_len)
{
    FILE *fp = NULL;

    META_LOG("[Meta][Microtrust] get_file_length in");

    if ((fp = fopen(filepath, "r")) == NULL)
    {
        *data_out_len = 1;
        data_out[0] = 1;
        META_LOG("[Meta][Microtrust] get_file_length fopen file failed, file path is: %s", filepath);
    }
    else
    {
        fseek(fp, 0, 2);
        snprintf(&data_out[1], DATA_OUT_BUFFER_SIZE-1, "%d", ftell(fp));
        data_out[0] = 0;
        *data_out_len = DATA_OUT_BUFFER_SIZE;
    }

    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    META_LOG("[Meta][Microtrust] get_file_length out");

    return;
}

/*
 * name: get_filename_by_suffix
 * function: find file by suffix, use for search Microtrust factory config file
 */
static void get_filename_by_suffix(const char *filepath, char *file_suffix, char *data_out, int *data_out_len)
{
    struct dirent *dir_entry = NULL;
    DIR *dir_info = NULL;
    int filename_length = 0;
    int suffix_length = 0;

    data_out[0] = 1;
    suffix_length = strlen(file_suffix);

    dir_info = opendir(filepath);
    if (dir_info)
    {
        while ((dir_entry = readdir(dir_info)) != NULL)
        {
            filename_length = strlen(dir_entry->d_name);
            if (strncmp(&dir_entry->d_name[filename_length - 4], file_suffix, suffix_length) == 0)
            {
                memcpy(&data_out[1], dir_entry->d_name, filename_length);
                data_out[0] = 0;
                break;
            }
        }
    }

    if (dir_info)
    {
        closedir(dir_info);
    }

    *data_out_len = filename_length + 1;

    return;
}

/*
 * name: handle_spmeta_request
 * function: handle request from PC tool
 */
static int  handle_spmeta_request(unsigned char *data_in, int data_in_len, char *peer_buf, int *peer_buff_size)
{
    unsigned int filepath_len = 0;
    int tmp = data_in[1];
    int file_seek_counter = 0;
    char filepath[256] = { 0 };
    char file_suffix[8] = { 0 };
    char command[512] = { 0 };

    META_LOG("[Meta][Microtrust] handle_spmeta_request in, request is %d", data_in[0]);

    switch (data_in[0]) {
    case WRITE_BUFFER_TO_PHONE:
        filepath_len = data_in[1];
        memcpy(filepath, &data_in[2], data_in[1]);
        write_buffer_to_file(&data_in[2 + filepath_len], filepath, data_in_len - 2 - data_in[1], peer_buf, peer_buff_size);
        break;
    case READ_FILE_TO_BUFFER:
        file_seek_counter = data_in[2];
        memcpy(filepath, &data_in[3], data_in[1]);
        read_file_to_buffer(filepath, peer_buf, file_seek_counter, peer_buff_size);
        break;
    case CALL_PHONE_PROGRAM:
        memcpy(command, &data_in[2], data_in[1]);
        call_phone_program(command, peer_buf, peer_buff_size);
        break;
    case TEST_FILE_EXIST:
        memcpy(filepath, &data_in[2], data_in[1]);
        test_file_exist(filepath, peer_buf, peer_buff_size);
        break;
    case GET_FILE_LENGTH:
        memcpy(filepath, &data_in[2], data_in[1]);
        get_file_length(filepath, peer_buf, peer_buff_size);
        break;
    case REMOVE_FILE:
        filepath_len = data_in[1];
        memcpy(filepath, &data_in[2], data_in[1]);
        remove_file(filepath, filepath_len, peer_buf, peer_buff_size);
        break;
    case CHECK_INIT_THH_STATUS:
        check_init_thh_status(peer_buf, peer_buff_size);
        break;
    case GET_FILENAME_BY_SUFFIX:
        memcpy(filepath, &data_in[2], tmp);
        memcpy(file_suffix, &data_in[2 + tmp + 1], data_in[2 + tmp]);
        get_filename_by_suffix(filepath, file_suffix, peer_buf, peer_buff_size);
        break;
    case CHECK_SOTER_STATUS:
        check_soter_status(peer_buf, peer_buff_size);
        break;
    case CHECK_ACTIVE_STATUS:
        check_active_status(peer_buf, peer_buff_size);
        break;
    default:
		peer_buf[0] = 123;
        break;
    }

    META_LOG("[Meta][Microtrust] handle_spmeta_request out");

    return 0;
}
#endif
// End Microtrust

/////////////////////////////////////////////////////////////////////////////////////////



