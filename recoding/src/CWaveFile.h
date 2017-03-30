#ifndef __WAVEFILE_H__
#define __WAVEFILE_H__

#include <mmsystem.h>
#include "record.h"
#include "BladeMP3EncDLL.h"

struct WriteParams
{
	HMMIO hmmio;
	MMCKINFO *mmciRiff;
	MMCKINFO *mmciFmt;
	MMCKINFO *mmciData;
	WAVEFORMATEX *pwf;
};

	// 录音的声道
	typedef enum _enum_rec_channel
	{
		ENUM_REC_CHANNEL_MONO,
		ENUM_REC_CHANNEL_STEREO,
		ENUM_REC_CHANNEL_ALONE,
		ENUM_REC_CHANNEL_NUM,
	} ENUM_REC_CHANNEL;

	// 保存文件的通道
	typedef enum _enum_file_channel
	{
		ENUM_FILE_CHANNEL_COMMON,
		ENUM_FILE_CHANNEL_LEFT=ENUM_FILE_CHANNEL_COMMON,
		ENUM_FILE_CHANNEL_RIGHT,
		ENUM_FILE_CHANNEL_NUM,
	} ENUM_FILE_CHANNEL;

BOOL CreateWaveFile(LPSTR filename,WriteParams *pParams);
BOOL WriteWaveFile(WriteParams *pParams,LPSTR pData,LONG length);
BOOL CloseWaveFile(WriteParams *pParams);



BOOL RecordStart();
void RecordEnd();
void RecordPause();
void RecordPauseStart();
void PlayEnd();
void PlayPause();
void PlayPauseStart();
void CALLBACK waveInProc(HWAVEIN hwi,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
int GetTimeLength(LPCSTR filename);

BOOL PlayStart(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen);


	BOOL LoadMp3DllFunc ();
	void DrwaWaveChar ( CClientDC &dc, DWORD dwDrawBytes, BYTE *pCharData, BOOL bLeftChannel );
	void ResetMp3EncodeVar();
	BOOL WaveBufferMp3Encode ( char *szWavData, int nWavSize, ENUM_FILE_CHANNEL eFileChannel );
	void EndEncodeMp3 ( ENUM_FILE_CHANNEL eFileChannel );
	void FreeMp3Encode ();
	BOOL PrepareEncodeMp3 ( LPCTSTR lpszMp3FileName, ENUM_FILE_CHANNEL eFileChannel );





#endif