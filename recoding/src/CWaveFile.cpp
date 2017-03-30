#include "stdafx.h"
#include "CWaveFile.h"
#include "Resource.h"
#include "record.h"



#include "BladeMP3EncDLL.h"

#pragma comment(lib,"winmm.lib")

#define BUF_NUMS 5
//#define BUF_SIZE 2*1024
#define BUF_SIZE 512

#define BUF2_SIZE 512*1024

double inReal[512],inImg[512];
double outReal[512],outImg[512];
double outAmp[512];



extern CDialog *g_pDlg;


FILE *g_fpWaveFile=NULL;

/**********************************************************************
* Global variables and functions for recording                        *
**********************************************************************/
HWAVEIN hwi;
HWAVEOUT hwo=NULL;
WAVEHDR wh[BUF_NUMS];//WAVEHDR音频数据块缓存结构
WAVEHDR wh2;
WAVEFORMATEX wfm;
WriteParams params;
int recording=0;

int nChanels,nBits,nSpeed;


	WORD				m_wInQueu;
	DWORD				m_dwQueuBufferSize;

	// mp3编码用到的变量
	PBYTE				m_ForMp3_pMP3Buffer[ENUM_FILE_CHANNEL_NUM];
	PBYTE				m_ForMp3_pWaveBuffer[ENUM_FILE_CHANNEL_NUM];
	FILE*				m_pFileMp3[ENUM_FILE_CHANNEL_NUM];
	CString				m_csMp3FileName[ENUM_FILE_CHANNEL_NUM];
	DWORD				m_ForMp3_dwWaveDataBytes[ENUM_FILE_CHANNEL_NUM];
	HBE_STREAM			m_ForMp3_hStream[ENUM_FILE_CHANNEL_NUM];

	HINSTANCE			m_ForMp3_hDLL_LameEnc;
	DWORD				m_ForMp3_dwWaveBufferSize;
	BEINITSTREAM		m_ForMp3_Proc_hInitStream;
	BEENCODECHUNK		m_ForMp3_Proc_hEncodeChunk;
	BEDEINITSTREAM		m_ForMp3_Proc_hDeinitStream;
	BECLOSESTREAM		m_ForMp3_Proc_hCloseStream;
	BEVERSION			m_ForMp3_Proc_hVersion;
	BEWRITEVBRHEADER	m_ForMp3_Proc_hWriteVBRHeader;
	BEWRITEINFOTAG		m_ForMp3_Proc_hWriteInfoTag;
	DWORD				m_dwSamplesEncodeMp3Block;

/*************************************************************
* Implication of recording                                   *
*************************************************************/
BOOL CreateWaveFile(LPSTR filename,WriteParams *pParams)
{

	MMRESULT result;
	HMMIO hmmio;
	pParams->mmciRiff=new MMCKINFO;
	pParams->mmciFmt=new MMCKINFO;
	pParams->mmciData=new MMCKINFO;

	//Create waveform file
	hmmio = mmioOpen(filename, NULL,
		MMIO_CREATE | MMIO_WRITE | MMIO_ALLOCBUF);
	pParams->hmmio=hmmio;

	//Create Global chunk
	memset(pParams->mmciRiff,0,sizeof(MMCKINFO));
	pParams->mmciRiff->fccType=mmioFOURCC('W', 'A', 'V', 'E');
	result=mmioCreateChunk(hmmio,pParams->mmciRiff,MMIO_CREATERIFF);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	//Create format chunk
	memset(pParams->mmciFmt,0,sizeof(MMCKINFO));
	pParams->mmciFmt->ckid=mmioFOURCC('f', 'm', 't', ' ');
	result=mmioCreateChunk(hmmio,pParams->mmciFmt,0);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	//Write waveform format
	mmioWrite(hmmio, (char*)pParams->pwf,
        sizeof(PCMWAVEFORMAT));

	//Adjust format chunk size
	mmioAscend(hmmio,pParams->mmciFmt,0);

	//Create data chunk
	memset(pParams->mmciData,0,sizeof(MMCKINFO));
	pParams->mmciData->ckid=mmioFOURCC('d', 'a', 't', 'a');
	result=mmioCreateChunk(hmmio,pParams->mmciData,0);
	if(result != MMSYSERR_NOERROR)
		return FALSE;
	
	char strFile[MAX_PATH];
	sprintf(strFile,"%s.dat",filename);
	g_fpWaveFile=fopen(strFile,"w");

	return TRUE;
}

BOOL WriteWaveFile(WriteParams *pParams,LPSTR pData,LONG length)
{
	fwrite(pData,length,1,g_fpWaveFile);


	MMRESULT result;
	if(pParams->hmmio == NULL)
		return FALSE;

	//Write data to file
	result=mmioWrite(pParams->hmmio,pData,length);

	if(result != MMSYSERR_NOERROR)
		return FALSE;


	return TRUE;
}

BOOL CloseWaveFile(WriteParams *pParams)
{

	MMRESULT result;
	if(pParams->hmmio == NULL)
		return FALSE;

	//Adjust data chunk
	mmioAscend(pParams->hmmio,pParams->mmciData,0);

	//Adjust RIFF chunk
	mmioAscend(pParams->hmmio,pParams->mmciRiff,0);

	//Close waveform file
	result=mmioClose(pParams->hmmio,0);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	pParams->hmmio=NULL;
	delete pParams->mmciRiff;
	delete pParams->mmciFmt;
	delete pParams->mmciData;

	if(g_fpWaveFile != NULL)
	{
		fclose(g_fpWaveFile);
		g_fpWaveFile=NULL;
	}

	return TRUE;
}

//
// 为mp3编码装载动态库
//
BOOL LoadMp3DllFunc ()
{
	if ( m_ForMp3_hDLL_LameEnc ) return TRUE;
	BE_VERSION	Version			={0,};
	
	// Load lame_enc.dll library (Make sure though that you set the 
	// project/settings/debug Working Directory correctly, otherwhise the DLL can't be loaded
	
	m_ForMp3_hDLL_LameEnc = LoadLibrary ( "lame_enc.dll" );
	if( NULL == m_ForMp3_hDLL_LameEnc )
	{
		AfxMessageBox ( "Error loading lame_enc.DLL" );
		return FALSE;
	}
	
	// Get Interface functions from the DLL
	m_ForMp3_Proc_hInitStream		= (BEINITSTREAM) GetProcAddress(m_ForMp3_hDLL_LameEnc, TEXT_BEINITSTREAM);
	m_ForMp3_Proc_hEncodeChunk		= (BEENCODECHUNK) GetProcAddress(m_ForMp3_hDLL_LameEnc, TEXT_BEENCODECHUNK);
	m_ForMp3_Proc_hDeinitStream		= (BEDEINITSTREAM) GetProcAddress(m_ForMp3_hDLL_LameEnc, TEXT_BEDEINITSTREAM);
	m_ForMp3_Proc_hCloseStream		= (BECLOSESTREAM) GetProcAddress(m_ForMp3_hDLL_LameEnc, TEXT_BECLOSESTREAM);
	m_ForMp3_Proc_hVersion			= (BEVERSION) GetProcAddress(m_ForMp3_hDLL_LameEnc, TEXT_BEVERSION);
	m_ForMp3_Proc_hWriteVBRHeader	= (BEWRITEVBRHEADER) GetProcAddress(m_ForMp3_hDLL_LameEnc,TEXT_BEWRITEVBRHEADER);
	m_ForMp3_Proc_hWriteInfoTag  = (BEWRITEINFOTAG) GetProcAddress(m_ForMp3_hDLL_LameEnc,TEXT_BEWRITEINFOTAG);
	
	// Check if all interfaces are present
	if ( !m_ForMp3_Proc_hInitStream || !m_ForMp3_Proc_hEncodeChunk || !m_ForMp3_Proc_hDeinitStream || !m_ForMp3_Proc_hCloseStream || !m_ForMp3_Proc_hVersion || !m_ForMp3_Proc_hWriteVBRHeader )
	{
		AfxMessageBox ( "Unable to get LAME interfaces" );
		return FALSE;
	}
	
	// Get the version number
	m_ForMp3_Proc_hVersion ( &Version );
	
	TRACE (
		"lame_enc.dll version %u.%02u (%u/%u/%u)\n"
		"lame_enc Engine %u.%02u\n"
		"lame_enc homepage at %s\n\n",	
		Version.byDLLMajorVersion, Version.byDLLMinorVersion,
		Version.byDay, Version.byMonth, Version.wYear,
		Version.byMajorVersion, Version.byMinorVersion,
		Version.zHomepage );
	
	return TRUE;
}

//
// 为mp3编码做准备
//
BOOL PrepareEncodeMp3 ( LPCTSTR lpszMp3FileName, ENUM_FILE_CHANNEL eFileChannel )
{
//	ASSERT ( m_ForMp3_hDLL_LameEnc );
//	ASSERT ( eFileChannel >= ENUM_FILE_CHANNEL_COMMON && eFileChannel < ENUM_FILE_CHANNEL_NUM );
	if ( !m_ForMp3_hDLL_LameEnc ) return FALSE;
	if ( m_pFileMp3[eFileChannel] )		// 已经开始纪录mp3文件了
		return TRUE;

	ASSERT ( lpszMp3FileName && strlen(lpszMp3FileName) > 0 );
	m_csMp3FileName[eFileChannel] = lpszMp3FileName;
	BE_ERR err = 0;

	BE_CONFIG beConfig = {0};
	memset ( &beConfig, 0, sizeof(beConfig) );					// clear all fields
	
	// use the LAME config structure
	beConfig.dwConfig = BE_CONFIG_LAME;
	
//	WAVEFORMATEX wfx = m_Format;
//	if ( m_eRecChannel == ENUM_REC_CHANNEL_ALONE )
//		wfx.nChannels = 1;
	// this are the default settings for testcase.wav
//	int nMode = (wfx.nChannels==1)?BE_MP3_MODE_MONO:BE_MP3_MODE_JSTEREO;
	beConfig.format.LHV1.dwStructVersion	= 1;
	beConfig.format.LHV1.dwStructSize		= sizeof(beConfig);		
	beConfig.format.LHV1.dwSampleRate		= wfm.nSamplesPerSec;		// INPUT FREQUENCY
	beConfig.format.LHV1.dwReSampleRate		= 0;						// DON"T RESAMPLE
//	beConfig.format.LHV1.nMode				= BE_MP3_MODE_DUALCHANNEL;					// OUTPUT IN STREO
beConfig.format.LHV1.nMode				= BE_MP3_MODE_MONO;	
	beConfig.format.LHV1.dwBitrate			= 128;						// MINIMUM BIT RATE
	beConfig.format.LHV1.nPreset			= LQP_R3MIX;				// QUALITY PRESET SETTING
	beConfig.format.LHV1.dwMpegVersion		= MPEG1;					// MPEG VERSION (I or II)
	beConfig.format.LHV1.dwPsyModel			= 0;						// USE DEFAULT PSYCHOACOUSTIC MODEL 
	beConfig.format.LHV1.dwEmphasis			= 0;						// NO EMPHASIS TURNED ON
	beConfig.format.LHV1.bOriginal			= TRUE;						// SET ORIGINAL FLAG
	beConfig.format.LHV1.bWriteVBRHeader	= TRUE;						// Write INFO tag
	
	//	beConfig.format.LHV1.dwMaxBitrate		= 320;					// MAXIMUM BIT RATE
	//	beConfig.format.LHV1.bCRC				= TRUE;					// INSERT CRC
	//	beConfig.format.LHV1.bCopyright			= TRUE;					// SET COPYRIGHT FLAG	
	//	beConfig.format.LHV1.bPrivate			= TRUE;					// SET PRIVATE FLAG
	//	beConfig.format.LHV1.bWriteVBRHeader	= TRUE;					// YES, WRITE THE XING VBR HEADER
	//	beConfig.format.LHV1.bEnableVBR			= TRUE;					// USE VBR
	//	beConfig.format.LHV1.nVBRQuality		= 5;					// SET VBR QUALITY
	beConfig.format.LHV1.bNoRes				= TRUE;					// No Bit resorvoir

	
	// Preset Test
	//	beConfig.format.LHV1.nPreset			= LQP_PHONE;
	
	// Init the MP3 Stream
	m_wInQueu = (WORD) ( wfm.nChannels + wfm.wBitsPerSample/8 + wfm.nSamplesPerSec/11025 );
	m_dwQueuBufferSize=1024;


	DWORD dwMP3Buffer = 0;
	err = m_ForMp3_Proc_hInitStream ( &beConfig, &m_dwSamplesEncodeMp3Block, &dwMP3Buffer, &m_ForMp3_hStream[eFileChannel] );
	m_ForMp3_dwWaveBufferSize = m_wInQueu*m_dwQueuBufferSize;
	
	// Check result
	if ( err != BE_ERR_SUCCESSFUL )
	{
		AfxMessageBox ( "Error opening encoding stream (%lu)", err);
		return FALSE;
	}

	
	// Open MP3 file
	::DeleteFile ( lpszMp3FileName );
	m_pFileMp3[eFileChannel] = fopen ( lpszMp3FileName, "wb+" );
	if ( m_pFileMp3[eFileChannel] == NULL )
	{
		CString csMsg;
		csMsg.Format ( "Error creating file %s", lpszMp3FileName );
		AfxMessageBox ( csMsg );
		return FALSE;
	}	
	
	// Allocate buffer
	if ( m_ForMp3_pMP3Buffer[eFileChannel] ) delete[] m_ForMp3_pMP3Buffer[eFileChannel];
	m_ForMp3_pMP3Buffer[eFileChannel] = new BYTE[dwMP3Buffer];
	if ( m_ForMp3_pWaveBuffer[eFileChannel] ) delete[] m_ForMp3_pWaveBuffer[eFileChannel];
	m_ForMp3_pWaveBuffer[eFileChannel] = new BYTE[m_ForMp3_dwWaveBufferSize];
	
	// Check if Buffer are allocated properly
	if( !m_ForMp3_pMP3Buffer[eFileChannel] || !m_ForMp3_pWaveBuffer[eFileChannel] )
	{
		AfxMessageBox ( "Out of memory" );
		return FALSE;
	}

	return TRUE;
}

BOOL WaveBufferMp3Encode ( char *szWavData, int nWavSize, ENUM_FILE_CHANNEL eFileChannel)
{
	ASSERT ( eFileChannel >= ENUM_FILE_CHANNEL_COMMON && eFileChannel < ENUM_FILE_CHANNEL_NUM );

	DWORD		dwWrite			=0;
	BE_ERR		err				=0;
	
	// Encode samples
	err = m_ForMp3_Proc_hEncodeChunk ( m_ForMp3_hStream[eFileChannel], nWavSize/sizeof(SHORT),
		(SHORT*)szWavData, m_ForMp3_pMP3Buffer[eFileChannel], &dwWrite );
	
	// Check result
	if ( err != BE_ERR_SUCCESSFUL )
	{
		m_ForMp3_Proc_hCloseStream ( m_ForMp3_hStream[eFileChannel] );
		CString csMsg;
		csMsg.Format ( "m_ForMp3_Proc_hEncodeChunk() failed (%lu)", err );
		AfxMessageBox ( csMsg );
		return FALSE;
	}
	
	// write dwWrite bytes that are returned in tehe m_ForMp3_pMP3Buffer to disk
	if(fwrite(m_ForMp3_pMP3Buffer[eFileChannel],1,dwWrite,m_pFileMp3[eFileChannel]) != dwWrite)
	{
		AfxMessageBox ( "Output file write error" );
		return FALSE;
	}

	return TRUE;
	
}

//
// 结束mp3编码
//
void EndEncodeMp3 ( ENUM_FILE_CHANNEL eFileChannel )
{
	if ( !m_pFileMp3[eFileChannel] ) return;

	ASSERT ( eFileChannel >= ENUM_FILE_CHANNEL_COMMON && eFileChannel < ENUM_FILE_CHANNEL_NUM );
	DWORD		dwWrite			=0;
	BE_ERR		err				=0;

	if ( !m_ForMp3_Proc_hDeinitStream ) return;

	// Deinit the stream
	err = m_ForMp3_Proc_hDeinitStream ( m_ForMp3_hStream[eFileChannel], m_ForMp3_pMP3Buffer[eFileChannel], &dwWrite );
	
	// Check result
	if ( err != BE_ERR_SUCCESSFUL )
	{
		
		m_ForMp3_Proc_hCloseStream ( m_ForMp3_hStream[eFileChannel] );
		CString csMsg;
		csMsg.Format ( "beExitStream failed (%lu)", err );
		AfxMessageBox ( csMsg );
		return;
	}
	
	// Are there any bytes returned from the DeInit call?
	// If so, write them to disk
	if ( dwWrite )
	{
		if( fwrite ( m_ForMp3_pMP3Buffer[eFileChannel], 1, dwWrite, m_pFileMp3[eFileChannel] ) != dwWrite )
		{
			AfxMessageBox ( "Output file write error" );
			return;
		}
	}
	
	// close the MP3 Stream
	ASSERT ( m_ForMp3_Proc_hCloseStream );
	m_ForMp3_Proc_hCloseStream( m_ForMp3_hStream[eFileChannel] );
	
	// Delete Buffer
	if ( m_ForMp3_pMP3Buffer[eFileChannel] ) delete [] m_ForMp3_pMP3Buffer[eFileChannel];
	m_ForMp3_pMP3Buffer[eFileChannel] = NULL;
	if ( m_ForMp3_pWaveBuffer[eFileChannel] ) delete [] m_ForMp3_pWaveBuffer[eFileChannel];
	m_ForMp3_pWaveBuffer[eFileChannel] = NULL;
	
	// Close output file
	fclose( m_pFileMp3[eFileChannel] );
	m_pFileMp3[eFileChannel] = NULL;
	
	if ( m_ForMp3_Proc_hWriteInfoTag )
	{
		// Write the INFO Tag
		m_ForMp3_Proc_hWriteInfoTag ( m_ForMp3_hStream[eFileChannel], m_csMp3FileName[eFileChannel] );
	}
	else
	{
		m_ForMp3_Proc_hWriteVBRHeader( m_csMp3FileName[eFileChannel] );
	}
}

//
// 释放mp3编码模块所申请的资源
//
void FreeMp3Encode ()
{
	if ( m_ForMp3_hDLL_LameEnc )
		::FreeLibrary ( m_ForMp3_hDLL_LameEnc );
	m_ForMp3_hDLL_LameEnc = NULL;
	ResetMp3EncodeVar ();
}

void ResetMp3EncodeVar()
{
	for ( int eFileChannel=ENUM_FILE_CHANNEL_COMMON; eFileChannel<ENUM_FILE_CHANNEL_NUM; eFileChannel++ )
	{
		m_ForMp3_pMP3Buffer[eFileChannel] = NULL;
		m_ForMp3_pWaveBuffer[eFileChannel] = NULL;
		m_pFileMp3[eFileChannel] = NULL;
		m_csMp3FileName[eFileChannel] = "";
		m_ForMp3_dwWaveDataBytes[eFileChannel] = 0;
		m_ForMp3_hStream[eFileChannel] = 0;
	}
	m_ForMp3_Proc_hInitStream = NULL;
    m_ForMp3_Proc_hEncodeChunk = NULL;
    m_ForMp3_Proc_hDeinitStream = NULL;
    m_ForMp3_Proc_hCloseStream = NULL;
    m_ForMp3_Proc_hVersion = NULL;
    m_ForMp3_Proc_hWriteVBRHeader = NULL;
    m_ForMp3_Proc_hWriteInfoTag = NULL;
    m_ForMp3_hDLL_LameEnc = NULL;
	m_ForMp3_dwWaveBufferSize = 0;
	m_dwSamplesEncodeMp3Block = 0;
}


/*************************************************************
* Implication of recording                                   *
*************************************************************/
BOOL RecordStart()
{
	int i;

	if(hwi != NULL)
		return FALSE;

	//Buffer size
	DWORD bufsize=BUF_SIZE;
	UINT uID;
	MMRESULT result;

	//Is there any device can use
	uID=waveInGetNumDevs();
	if(uID == 0)
		goto ERROR_END;
	uID=WAVE_MAPPER;

	WAVEINCAPS wic;
	//Get device caps
	result=waveInGetDevCaps(uID,&wic,sizeof(WAVEINCAPS));
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;

	//Set wave formats
/*	wfm.nChannels=2;
	wfm.nSamplesPerSec=(DWORD)44100;
	wfm.wBitsPerSample=16;
	wfm.wFormatTag=WAVE_FORMAT_PCM;
	wfm.nBlockAlign=1;
	wfm.nAvgBytesPerSec=(DWORD)22050;
	wfm.cbSize=sizeof(WAVEFORMATEX);*/
/*	wfm.wFormatTag=WAVE_FORMAT_PCM;  //PCM编码
	wfm.nChannels=2; //单声道
	wfm.nSamplesPerSec= 44100;//采样频率，每秒采集次数
	wfm.wBitsPerSample=16; //采样位，模拟信号转数字信号的精准度
	wfm.nBlockAlign= wfm.nChannels * wfm.wBitsPerSample / 8;
	wfm.nAvgBytesPerSec=wfm.nSamplesPerSec * wfm.nBlockAlign;//每秒数据量
	wfm.cbSize=0;*/

	//Open device
	result=waveInOpen(&hwi,
					  uID,
					  &wfm,
					  (DWORD)waveInProc,
					  (DWORD)AfxGetInstanceHandle(),
					  CALLBACK_FUNCTION);
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;

	for(i=0;i<BUF_NUMS;i++)
	{
		//Alloc buffers
		memset(wh+i,0,sizeof(WAVEHDR));
		wh[i].lpData=(char *)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,bufsize);
		wh[i].dwBufferLength=bufsize;

		//Prepare buffers
		result=waveInPrepareHeader(hwi,wh+i,sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR)
			goto ERROR_END;

		//Add buffers
		result=waveInAddBuffer(hwi,wh+i,sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR)
			goto ERROR_END;
	}

	//Start recording
	result=waveInStart(hwi);
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;
	recording=1;

	
	return TRUE;

	ERROR_END:
	for(i=0;i<BUF_NUMS;i++)
	{
		if(wh[i].lpData != NULL)
		{
			waveInUnprepareHeader(hwi,wh+i,sizeof(WAVEHDR));
			GlobalFree(wh[i].lpData);
		}
	}
	if(hwi != NULL)
	{
		waveInClose(hwi);
		hwi=NULL;
	}
	return FALSE;
}

void RecordPause()//录音暂停
{
	waveInStop(hwi);
}

void RecordPauseStart()//录音暂停后，录音继续
{
	waveInStart(hwi);
}


//End recording
void RecordEnd()
{
	int i;

	if(hwi == NULL)
		return;

	MMRESULT result;
	
	//Stop
	result=waveInStop(hwi);
	recording=0;

	//Reset buffers
	result=waveInReset(hwi);

	for(i=0;i<BUF_NUMS;i++)
	{
		//Unprepare and release buffers
		waveInUnprepareHeader(hwi,wh+i,sizeof(WAVEHDR));
		GlobalFree(wh[i].lpData);
	}

	//Close device
	result=waveInClose(hwi);
	hwi=NULL;
	CloseWaveFile(&params);
//	AfxMessageBox("End record.");

	EndEncodeMp3 ( ENUM_FILE_CHANNEL_COMMON);
//	FreeMp3Encode ();
}

void PlayPause()//播放暂停
{
	waveOutPause(hwo);
}

void PlayPauseStart()//播放暂停后，继续播放
{
	waveOutRestart(hwo);
}


void PlayEnd()
{
	MMRESULT result;
	result=waveOutReset(hwo);
	CloseWaveFile(&params);
	hwo=NULL;
}
//Callback function
void CALLBACK waveInProc(HWAVEIN hwi,
						 UINT uMsg,
						 DWORD dwInstance,
						 DWORD dwParam1,
						 DWORD dwParam2)
{
	
	if(uMsg == WIM_DATA)
	{
		WAVEHDR *pwh;

		pwh=(WAVEHDR *)dwParam1;
		//Save waveform data
		WriteWaveFile(&params,(LPSTR)(pwh->lpData),pwh->dwBytesRecorded);
			

		char *pRecData[ENUM_FILE_CHANNEL_NUM] = { pwh->lpData, pwh->lpData };
		int nRecBytes[ENUM_FILE_CHANNEL_NUM] = { pwh->dwBytesRecorded, pwh->dwBytesRecorded };
		// 保存到mp3文件中
		if ( m_ForMp3_hDLL_LameEnc )
		{
			DWORD dwSamplesBytes = m_dwSamplesEncodeMp3Block * (wfm.wBitsPerSample/8);
			for ( int eFileChannel=ENUM_FILE_CHANNEL_COMMON; eFileChannel<ENUM_FILE_CHANNEL_NUM; eFileChannel++ )
			{
				if ( m_pFileMp3[eFileChannel] )
				{
					int nRemainSize = m_ForMp3_dwWaveBufferSize - m_ForMp3_dwWaveDataBytes[eFileChannel];
					int nCopyBytes = (nRemainSize < nRecBytes[eFileChannel]) ? nRemainSize : nRecBytes[eFileChannel];
					memcpy ( m_ForMp3_pWaveBuffer[eFileChannel]+m_ForMp3_dwWaveDataBytes[eFileChannel], pRecData[eFileChannel], nCopyBytes );
					m_ForMp3_dwWaveDataBytes[eFileChannel] += nCopyBytes;
					
					// mp3压缩
					for ( ; m_ForMp3_dwWaveDataBytes[eFileChannel] > dwSamplesBytes; m_ForMp3_dwWaveDataBytes[eFileChannel] -= dwSamplesBytes )
					{
						WaveBufferMp3Encode ( (char*)m_ForMp3_pWaveBuffer[eFileChannel], (int)dwSamplesBytes, (ENUM_FILE_CHANNEL)eFileChannel );
						memmove ( m_ForMp3_pWaveBuffer[eFileChannel], m_ForMp3_pWaveBuffer[eFileChannel]+dwSamplesBytes, m_ForMp3_dwWaveDataBytes[eFileChannel]-dwSamplesBytes );
					}
					
					char *pUnCopyData = pRecData[eFileChannel] + nCopyBytes;
					int nUnCopyDataBytes = nRecBytes[eFileChannel] - nCopyBytes;
					nRemainSize = m_ForMp3_dwWaveBufferSize - m_ForMp3_dwWaveDataBytes[eFileChannel];
					nCopyBytes = (nRemainSize < nUnCopyDataBytes) ? nRemainSize : nUnCopyDataBytes;
					memcpy ( m_ForMp3_pWaveBuffer[eFileChannel]+m_ForMp3_dwWaveDataBytes[eFileChannel], pUnCopyData, nCopyBytes );
				}
			}
		}

		//Reuse the buffers
		if(recording != 0)
		{
			waveInPrepareHeader(hwi,pwh,sizeof(WAVEHDR));
			waveInAddBuffer(hwi,pwh,sizeof(WAVEHDR));
		}
	}
	if(uMsg == WIM_OPEN)
	{
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_REC_START)->EnableWindow(FALSE);
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_PLAY_START)->EnableWindow(FALSE);
		AfxGetApp()->GetMainWnd()->UpdateData(FALSE);
	}
	if(uMsg == WIM_CLOSE) 
	{
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_REC_START)->EnableWindow(TRUE);
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
		AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_PLAY_START)->EnableWindow(TRUE);
		AfxGetApp()->GetMainWnd()->UpdateData(FALSE);
	}
}
/*************************************************************
* Implication of playback                                    *
*************************************************************/
BOOL PlayStart(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen)
{
	HMMIO hmmio=NULL;
	MMCKINFO ckiRiff,cki;
	MMRESULT result;
	LONG len;

	if(hwo != NULL)
	{
		result=waveOutReset(hwo);
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		result=waveOutClose(hwo);
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		hwo=NULL;
	}

	hmmio=mmioOpen((LPSTR)file,NULL,MMIO_READ | MMIO_ALLOCBUF);
	if(hmmio == NULL)
		return FALSE;

	ckiRiff.fccType=mmioFOURCC('W','A','V','E');
	result=mmioDescend(hmmio,&ckiRiff,NULL,MMIO_FINDRIFF);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	cki.ckid=mmioFOURCC('f','m','t',' ');
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	len=mmioRead(hmmio,(LPSTR)pwf,sizeof(WAVEFORMATEX));
	if(len == -1)
		goto END_ERROR;

	result=mmioAscend(hmmio,&cki,0);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	cki.ckid=mmioFOURCC('d','a','t','a');
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	*pLen=cki.cksize;
	*pData=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);
	if(*pData == NULL)
		goto END_ERROR;

	len=mmioRead(hmmio,(LPSTR)*pData,*pLen);
	if(len == -1)
		goto END_ERROR;

	result=waveOutOpen(&hwo,
					  WAVE_MAPPER,
					  pwf,
					  NULL,
					  (DWORD)AfxGetInstanceHandle(),
					  CALLBACK_NULL);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	memset(&wh2,0,sizeof(WAVEHDR));
	wh2.lpData=*pData;
	wh2.dwBufferLength=*pLen;
	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));

	return TRUE;

END_ERROR:
//	AfxMessageBox("文件不存在!");
	if(*pData != NULL)
	{
		GlobalFree(*pData);
		*pData=NULL;
		*pLen=0;
	}
	if(hmmio != NULL)
		mmioClose(hmmio,0);

	if(hwo != NULL)
		waveOutClose(hwo);
}
int GetTimeLength(LPCSTR filename)
{ 
	HMMIO file;//定义HMMIO文件句柄； 
	file=mmioOpen((LPSTR)filename,NULL,MMIO_READWRITE);//以读写模式打开所给的WAVE文件； 
	if(file==NULL) 
	{ 
	TRACE("WAVE文件打开失败！"); 
	return -1; 
	}
	char style[4];//定义一个四字节的数据，用来存放文件的类型； 
	mmioSeek(file,8,SEEK_SET);//定位到WAVE文件的类型位置 
	mmioRead(file,style,4); 
	if(style[0]!='W'||style[1]!='A'||style[2]!='V'||style[3]!='E')//判断该文件是否为"WAVE"文件格式 
	{ 
	TRACE("该文件不是WAVE格式的文件！"); 
	return -1; 
	}
	PCMWAVEFORMAT format; //定义PCMWAVEFORMAT结构对象，用来判断WAVE文件格式； 
	mmioSeek(file,20,SEEK_SET); 
	//对打开的文件进行定位，此时指向WAVE文件的PCMWAVEFORMAT结构的数据； 
	mmioRead(file,(char*)&format,sizeof(PCMWAVEFORMAT));//获取该结构的数据；
	//获取WAVE文件的声音数据的大小； 
	mmioSeek(file,24+sizeof(PCMWAVEFORMAT),SEEK_SET); 
	long size; 
	mmioRead(file,(char*)&size,4);
	//计算文件时长 
	long a = size;
	int timeLength; 
	timeLength = size/format.wf.nAvgBytesPerSec;
	::mmioClose(file, 0);
	return timeLength; 
}