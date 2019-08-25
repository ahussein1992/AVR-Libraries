/*
 * DFPlayer.h
 * Atmega8
 * Se7S_92
 * Last modification 8-24-2019
 */ 

#include "stdutils.h"

//				DFP_PlayTrackInRoot(1);			/* Play track 1 */	
// 				DFP_PlayNext();					/* Play Next */
// 				DFP_PlayPrevious();				/* Play previous */
// 				DFP_IncreaseVolume();			/* Increase volume */
// 				DFP_DecreaseVolume();			/* Decrease volume */
// 				DFP_RepeatCurrentTrack(true);	/* Repeat current track on */
// 				DFP_RepeatCurrentTrack(false);	/* Repeat current track off */
// 				DFP_Play();						/* Play */
// 				DFP_Pause();					/* Pause */
// 				DFP_Stop();						/* Stop */

#ifndef __DFPLAYER_H_
#define __DFPLAYER_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include "8P_USART.h"

#define DFPlayer_En_Pin PD2
#define DFPlayer_En_Port PORTD
#define DFPlayer_En_Port_Dir DDRD

#define DFP_MSG_SIZE		10
#define DFP_MSG_DELAY		200

#define DFP_STORAGE_USB			1
#define DFP_STORAGE_SDCARD		2

#define DFP_EQ_NORMAL			0
#define DFP_EQ_POP				1
#define DFP_EQ_ROCK				2
#define DFP_EQ_JAZZ				3
#define DFP_EQ_CLASSIC			4
#define DFP_EQ_BASS				5

/* Protocol Index */
#define DFP_MSG_IDX_HEADER		0
#define DFP_MSG_IDX_VERSION		1
#define DFP_MSG_IDX_LENGTH		2
#define DFP_MSG_IDX_COMMAND		3
#define DFP_MSG_IDX_FEEDBACK	4
#define DFP_MSG_IDX_PARAM_MSB	5
#define DFP_MSG_IDX_PARAM_LSB	6
#define DFP_MSG_IDX_CHECK_MSB	7
#define DFP_MSG_IDX_CHECK_LSB	8
#define DFP_MSG_IDX_TAIL		9

/* Default values */
#define DFP_MSG_VAL_HEADER		0x7E
#define DFP_MSG_VAL_VERSION		0xFF
#define DFP_MSG_VAL_LENGTH		0x06
#define DFP_MSG_VAL_NOFEEDBACK	0x00
#define DFP_MSG_VAL_TAIL		0xEF

/* Operation commands */
#define DFP_CMD_PLAY_NEXT			0x01
#define DFP_CMD_PLAY_PREVIOUS		0x02
#define DFP_CMD_PLAY_TRACK			0x03
#define DFP_CMD_INC_VOLUME			0x04
#define DFP_CMD_DEC_VOLUME			0x05
#define DFP_CMD_SET_VOLUME			0x06
#define DFP_CMD_SET_EQ				0x07
#define DFP_CMD_REPEAT_TRACK		0x08
#define DFP_CMD_SET_STORAGE			0x09
#define DFP_CMD_SET_SLEEP			0x0A
#define DFP_CMD_SET_NORMAL			0x0B
#define DFP_CMD_RESET				0x0C
#define DFP_CMD_PLAY				0x0D
#define DFP_CMD_PAUSE				0x0E
#define DFP_CMD_PLAY_TRACK_FOLDER	0x0F
#define DFP_CMD_SET_AMP				0x10
#define DFP_CMD_REPEAT_ALL			0x11
#define DFP_CMD_PLAY_MP3_FOLDER		0x12
#define DFP_CMD_INSERT_ADVERT		0x13
#define DFP_CMD_PLAY_3K_FOLDER		0x14
#define DFP_CMD_STOP_ADVERT			0x15
#define DFP_CMD_STOP				0x16
#define DFP_CMD_REPEAT_FOLDER		0x17
#define DFP_CMD_PLAY_RANDOM			0x18
#define DFP_CMD_REPEAT_CUR_TRACK	0x19
#define DFP_CMD_SET_DAC				0x1A
#define DFP_CMD_STORAGE_IN			0x3A
#define DFP_CMD_STORAGE_OUT			0x3B
#define DFP_CMD_USB_TRACK_FINISHED	0x3C
#define DFP_CMD_SD_TRACK_FINISHED	0x3D

/* Query commands */
#define DFP_CMD_GET_CUR_STORAGE				0x3F
#define DFP_CMD_ERROR						0x40
#define DFP_CMD_FEEDBACK					0x41
#define DFP_CMD_GET_CUR_STATUS				0x42
#define DFP_CMD_GET_CUR_VOLUME				0x43
#define DFP_CMD_GET_CUR_EQ					0x44
#define DFP_CMD_GET_NUM_USB_ROOT_TRACKS		0x47
#define DFP_CMD_GET_NUM_SD_ROOT_TRACKS		0x48
#define DFP_CMD_GET_CUR_USB_TRACK			0x4B
#define DFP_CMD_GET_CUR_SD_TRACK			0x4C
#define DFP_CMD_GET_NUM_FOLDER_TRACKS		0x4E
#define DFP_CMD_GET_NUM_STORAGE_FOLDERS		0x4F

typedef struct {
	uint8 StorageStatus;
	uint16 FinishedTrack;
	uint8 CurrentStorageDevice;
	uint8 Error;
	uint8 CurrentStatus;
	uint8 CurrentVolume;
	uint8 CurrentEQ;
	uint16 NumOfTracks;
	uint16 CurrentTrack;
	uint16 NumOfTracksInFolder;
	uint8 NumOfFolders;	
} _DFPlayerInfo;

_DFPlayerInfo DFPlayerInfo;
static uint8 RxBuffer[DFP_MSG_SIZE];
static uint8 TxBuffer[DFP_MSG_SIZE];
//extern uint8 Volume;
static void DFP_SendControlMessage(uint8 cmd, uint8 param_msb, uint8 param_lsb);
static bool DFP_SendQueryMessage(uint8 cmd, uint8 param_msb, uint8 param_lsb);
static bool DFP_ParseMessage();
static uint16 DFP_CalChecksum();

// void DFP_PlayNext(void);
// void DFP_PlayPrevious(void);
// void DFP_PlayTrackInRoot(uint16 track);
// void DFP_IncreaseVolume(void);
// void DFP_DecreaseVolume(void);
// void DFP_SetVolume(uint8 volume);
// void DFP_SetEQ(uint8 eq);
// void DFP_RepeatTrack(uint16 track);
// void DFP_SetStorage(uint8 storage);
// void DFP_SetSleep(void);
// void DFP_SetNormal(void);
// void DFP_Reset(void);
// void DFP_Play(void);
// void DFP_Pause(void);
// void DFP_PlayTrackInFolder(uint8 folder, uint8 track);
// void DFP_SetAudioGain(bool onoff, uint8 gain);
// void DFP_RepeatAll(bool repeat);
// void DFP_PlayTrackInMp3(uint16 track);
// void DFP_InsertAdvertisement(uint16 track);
// void DFP_PlayTrackIn3KFolder(uint8 folder, uint16 track);
// void DFP_StopAdvertisement(void);
// void DFP_Stop(void);
// void DFP_RepeatTrackInFolder(uint8 folder);
// void DFP_PlayRandomTrack(void);
// void DFP_RepeatCurrentTrack(bool onoff);
// void DFP_SetDAC(bool onoff);
// 
// bool DFP_QueryCurrentStorage(void);
// bool DFP_QueryCurrentStatus(void);
// bool DFP_QueryCurrentVolume(void);
// bool DFP_QueryCurrentEQ(void);
// bool DFP_QueryTracksInRoot(void);
// bool DFP_QueryCurrentTrack(void);
// bool DFP_QueryTrackInFolder(uint8 folder);
// bool DFP_QueryFolderInStorage(void);
// 
// void DFP_Initialize(void);
// void DFP_HandleMessage(void);

/**********************************************************************************/


void DFP_PlayNext()
{
	DFP_SendControlMessage(DFP_CMD_PLAY_NEXT, 0x00, 0x00);
}

void DFP_PlayPrevious()
{
	DFP_SendControlMessage(DFP_CMD_PLAY_PREVIOUS, 0x00, 0x00);
}

void DFP_PlayTrackInRoot(uint16 track)
{
	DFP_SendControlMessage(DFP_CMD_PLAY_TRACK, (track & 0xFF00) >> 8, (track & 0xFF));
}

void DFP_IncreaseVolume()
{
	DFP_SendControlMessage(DFP_CMD_INC_VOLUME, 0x00, 0x00);
}

void DFP_DecreaseVolume()
{
	DFP_SendControlMessage(DFP_CMD_DEC_VOLUME, 0x00, 0x00);
}

void DFP_SetVolume(uint8 volume)
{
	if(volume > 30) volume = 30;
	
	DFP_SendControlMessage(DFP_CMD_SET_VOLUME, 0x00, volume);
}

void DFP_SetEQ(uint8 eq)
{
	if(eq > 5) eq = 0;
	
	DFP_SendControlMessage(DFP_CMD_SET_EQ, 0x00, eq);
}

void DFP_RepeatTrack(uint16 track)
{
	DFP_SendControlMessage(DFP_CMD_REPEAT_TRACK, (track & 0xFF00) >> 8, track & 0xFF);
}

void DFP_SetStorage(uint8 storage)
{
	if(storage > 2) storage = 2;
	
	DFP_SendControlMessage(DFP_CMD_SET_STORAGE, 0x00, storage);
}

void DFP_SetSleep()
{
	DFP_SendControlMessage(DFP_CMD_SET_SLEEP, 0x00, 0x00);
}

void DFP_SetNormal()
{
	DFP_SendControlMessage(DFP_CMD_SET_NORMAL, 0x00, 0x00);
}

void DFP_Reset()
{
	DFP_SendControlMessage(DFP_CMD_RESET, 0x00, 0x00);
}

void DFP_Play()
{
	DFP_SendControlMessage(DFP_CMD_PLAY, 0x00, 0x00);
}

void DFP_Pause()
{
	DFP_SendControlMessage(DFP_CMD_PAUSE, 0x00, 0x00);
}

void DFP_PlayTrackInFolder(uint8 folder, uint8 track)
{
	if(folder > 99) folder = 99;
	
	DFP_SendControlMessage(DFP_CMD_PLAY_TRACK_FOLDER, folder, track);
}

void DFP_SetAudioGain(bool onoff, uint8 gain)
{
	if(gain > 31) gain = 31;
	
	DFP_SendControlMessage(DFP_CMD_SET_AMP, onoff, gain);
}

void DFP_RepeatAll(bool repeat)
{
	DFP_SendControlMessage(DFP_CMD_REPEAT_ALL, 0x00, repeat);
}

void DFP_PlayTrackInMp3(uint16 track)
{
	DFP_SendControlMessage(DFP_CMD_PLAY_MP3_FOLDER, (track & 0xFF00) >> 8, track & 0xFF);
}

void DFP_InsertAdvertisement(uint16 track)
{
	DFP_SendControlMessage(DFP_CMD_INSERT_ADVERT, (track & 0xFF00) >> 8, track & 0xFF);
}

void DFP_PlayTrackIn3KFolder(uint8 folder, uint16 track)
{
	if(folder > 15) folder = 15;
	if(track > 3000) track = 3000;
	
	DFP_SendControlMessage(DFP_CMD_PLAY_3K_FOLDER, (folder << 4) | (track & 0xF00) >> 8 , track & 0xFF);
}

void DFP_StopAdvertisement()
{
	DFP_SendControlMessage(DFP_CMD_STOP_ADVERT, 0x00, 0x00);
}

void DFP_Stop()
{
	DFP_SendControlMessage(DFP_CMD_STOP, 0x00, 0x00);
}

void DFP_RepeatTrackInFolder(uint8 folder)
{
	if(folder > 99) folder = 99;
	
	DFP_SendControlMessage(DFP_CMD_REPEAT_FOLDER, 0x00, folder);
}

void DFP_PlayRandomTrack()
{
	DFP_SendControlMessage(DFP_CMD_PLAY_RANDOM, 0x00, 0x00);
}

void DFP_RepeatCurrentTrack(bool onoff)
{
	DFP_SendControlMessage(DFP_CMD_REPEAT_CUR_TRACK, 0x00, !onoff);
}

void DFP_SetDAC(bool onoff)
{
	DFP_SendControlMessage(DFP_CMD_SET_DAC, 0x00, !onoff);
}

bool DFP_QueryCurrentStorage()
{
	return DFP_SendQueryMessage(DFP_CMD_GET_CUR_STORAGE, 0x00, 0x00);
}

bool DFP_QueryCurrentStatus()
{
	return DFP_SendQueryMessage(DFP_CMD_GET_CUR_STATUS, 0x00, 0x00);
}

bool DFP_QueryCurrentVolume()
{
	return DFP_SendQueryMessage(DFP_CMD_GET_CUR_VOLUME, 0x00, 0x00);
}

bool DFP_QueryCurrentEQ()
{
	return DFP_SendQueryMessage(DFP_CMD_GET_CUR_EQ, 0x00, 0x00);
}

bool DFP_QueryTracksInRoot()
{
	if(DFPlayerInfo.CurrentStorageDevice == DFP_STORAGE_USB)
	{
		return DFP_SendQueryMessage(DFP_CMD_GET_NUM_USB_ROOT_TRACKS, 0x00, 0x00);
	}
	else
	{
		return DFP_SendQueryMessage(DFP_CMD_GET_NUM_SD_ROOT_TRACKS, 0x00, 0x00);
	}
}

bool DFP_QueryCurrentTrack()
{
	if(DFPlayerInfo.CurrentStorageDevice == DFP_STORAGE_USB)
	{
		return DFP_SendQueryMessage(DFP_CMD_GET_CUR_USB_TRACK, 0x00, 0x00);
	}
	else
	{
		return DFP_SendQueryMessage(DFP_CMD_GET_CUR_SD_TRACK, 0x00, 0x00);
	}
}

bool DFP_QueryTrackInFolder(uint8 folder)
{
	if(folder > 99) folder = 99;
	
	return DFP_SendQueryMessage(DFP_CMD_GET_NUM_FOLDER_TRACKS, 0x00, 0x00);
}

bool DFP_QueryFolderInStorage()
{
	return DFP_SendQueryMessage(DFP_CMD_GET_NUM_STORAGE_FOLDERS, 0x00, 0x00);
}

void DFP_Initialize()
{
	memset(RxBuffer, 0, sizeof(RxBuffer));
	memset(TxBuffer, 0, sizeof(RxBuffer));
	
	DFPlayer_En_Port |= (1<<DFPlayer_En_Pin);
	
	//_delay_ms(1000);
	/* Initialize USART */
	USART_Init();
	
	/* Set Volume */
	DFP_SetVolume(30);
	
	/* Set EQ */
	DFP_SetEQ(DFP_EQ_ROCK);
	
	/* Query current status */
	DFP_QueryCurrentStorage();
	
	
	DFP_QueryCurrentStatus();
	
	
	DFP_QueryCurrentEQ();
	
	
	DFP_QueryCurrentTrack();
	
	
	DFP_QueryCurrentVolume();
	
	
	DFP_QueryTracksInRoot();
	
	
	DFP_QueryCurrentTrack();
	
}

void DFP_HandleMessage()
{
	/* No received message */
	if(!USART_Available()) return;
	
	/* Receive 10 bytes */
	if(!USART_RxBuffer(RxBuffer, sizeof(RxBuffer)))
	{
		/* When received garbage packets, try to receive again */
		if(!USART_RxBuffer(RxBuffer, sizeof(RxBuffer)))
		{
			return;
		}
	}
	else
	{
		DFP_ParseMessage();
	}
	
}

void DFP_SendControlMessage(uint8 cmd, uint8 param_msb, uint8 param_lsb)
{
	memset(TxBuffer, 0, sizeof(TxBuffer));
	memset(RxBuffer, 0, sizeof(RxBuffer));
	
	TxBuffer[DFP_MSG_IDX_HEADER] = DFP_MSG_VAL_HEADER;
	TxBuffer[DFP_MSG_IDX_VERSION] = DFP_MSG_VAL_VERSION;
	TxBuffer[DFP_MSG_IDX_LENGTH] = DFP_MSG_VAL_LENGTH;
	TxBuffer[DFP_MSG_IDX_COMMAND] = cmd;
	TxBuffer[DFP_MSG_IDX_FEEDBACK] = DFP_MSG_VAL_NOFEEDBACK;
	TxBuffer[DFP_MSG_IDX_PARAM_MSB] = param_msb;
	TxBuffer[DFP_MSG_IDX_PARAM_LSB] = param_lsb;
	uint16 checksum = DFP_CalChecksum();
	TxBuffer[DFP_MSG_IDX_CHECK_MSB] = (checksum & 0xFF00) >> 8;
	TxBuffer[DFP_MSG_IDX_CHECK_LSB] = checksum & 0xFF;
	TxBuffer[DFP_MSG_IDX_TAIL] = DFP_MSG_VAL_TAIL;
	
	USART_TxBuffer(TxBuffer, sizeof(TxBuffer));
	
	/* Delay between messages */
	_delay_ms(DFP_MSG_DELAY);
}

bool DFP_SendQueryMessage(uint8 cmd, uint8 param_msb, uint8 param_lsb)
{
	memset(TxBuffer, 0, sizeof(TxBuffer));
	memset(RxBuffer, 0, sizeof(RxBuffer));
	
	TxBuffer[DFP_MSG_IDX_HEADER] = DFP_MSG_VAL_HEADER;
	TxBuffer[DFP_MSG_IDX_VERSION] = DFP_MSG_VAL_VERSION;
	TxBuffer[DFP_MSG_IDX_LENGTH] = DFP_MSG_VAL_LENGTH;
	TxBuffer[DFP_MSG_IDX_COMMAND] = cmd;
	TxBuffer[DFP_MSG_IDX_FEEDBACK] = DFP_MSG_VAL_NOFEEDBACK;
	TxBuffer[DFP_MSG_IDX_PARAM_MSB] = param_msb;
	TxBuffer[DFP_MSG_IDX_PARAM_LSB] = param_lsb;
	uint16 checksum = DFP_CalChecksum();
	TxBuffer[DFP_MSG_IDX_CHECK_MSB] = (checksum & 0xFF00) >> 8;
	TxBuffer[DFP_MSG_IDX_CHECK_LSB] = checksum & 0xFF;
	TxBuffer[DFP_MSG_IDX_TAIL] = DFP_MSG_VAL_TAIL;
	
	USART_TxBuffer(TxBuffer, sizeof(TxBuffer));
	
	if(!USART_RxBuffer(RxBuffer, sizeof(RxBuffer)))
	{
		/* When received garbage packets, try to receive again */
		
		if(!USART_RxBuffer(RxBuffer, sizeof(RxBuffer)))
		{
			/* Delay between messages */
			_delay_ms(DFP_MSG_DELAY);
			
			return false;
		}
	}
	
	DFP_ParseMessage();
	
	/* Delay between messages */
	_delay_ms(DFP_MSG_DELAY);
	
	return true;
}

static bool DFP_ParseMessage()
{
	/* Check protocol format */
	if(RxBuffer[DFP_MSG_IDX_HEADER] != DFP_MSG_VAL_HEADER) return false;
	if(RxBuffer[DFP_MSG_IDX_LENGTH] != DFP_MSG_VAL_LENGTH) return false;
	if(RxBuffer[DFP_MSG_IDX_TAIL] != DFP_MSG_VAL_TAIL) return false;
	
	/* Check message command */
	uint8 cmd = RxBuffer[DFP_MSG_IDX_COMMAND];
	
	switch(cmd)
	{
		case DFP_CMD_STORAGE_IN:				/* Storage plugged in */
		case DFP_CMD_STORAGE_OUT:				/* Storage plugged out */
		DFPlayerInfo.StorageStatus = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_USB_TRACK_FINISHED:		/* USB track finished playing */
		case DFP_CMD_SD_TRACK_FINISHED:			/* SD card track finished playing */
		DFPlayerInfo.FinishedTrack = (RxBuffer[DFP_MSG_IDX_PARAM_MSB] << 8 | RxBuffer[DFP_MSG_IDX_PARAM_LSB]);
		break;
		case DFP_CMD_GET_CUR_STORAGE:			/* Current storage device */
		DFPlayerInfo.CurrentStorageDevice = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_ERROR:						/* Error */
		DFPlayerInfo.Error = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_FEEDBACK:					/* Feedback */
		break;
		case DFP_CMD_GET_CUR_STATUS:			/* Current status */
		DFPlayerInfo.CurrentStatus = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_GET_CUR_VOLUME:			/* Current volume */
		DFPlayerInfo.CurrentVolume = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_GET_CUR_EQ:				/* Current EQ */
		DFPlayerInfo.CurrentEQ = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		case DFP_CMD_GET_NUM_USB_ROOT_TRACKS:	/* # of tracks in USB */
		case DFP_CMD_GET_NUM_SD_ROOT_TRACKS:	/* # of tracks in SD card */
		DFPlayerInfo.NumOfTracks = (RxBuffer[DFP_MSG_IDX_PARAM_MSB] << 8 | RxBuffer[DFP_MSG_IDX_PARAM_LSB]);
		break;
		case DFP_CMD_GET_CUR_USB_TRACK:			/* Current track in USB */
		case DFP_CMD_GET_CUR_SD_TRACK:			/* Current track in SD card */
		DFPlayerInfo.CurrentTrack = (RxBuffer[DFP_MSG_IDX_PARAM_MSB] << 8 | RxBuffer[DFP_MSG_IDX_PARAM_LSB]);
		break;
		case DFP_CMD_GET_NUM_FOLDER_TRACKS:		/* # of tracks in a folder */
		DFPlayerInfo.NumOfTracksInFolder = (RxBuffer[DFP_MSG_IDX_PARAM_MSB] << 8 | RxBuffer[DFP_MSG_IDX_PARAM_LSB]);
		break;
		case DFP_CMD_GET_NUM_STORAGE_FOLDERS:	/* # of folders in root directory */
		DFPlayerInfo.NumOfFolders = RxBuffer[DFP_MSG_IDX_PARAM_LSB];
		break;
		default:		/* Unknown command */
		break;
	}
	return true;
}

static uint16 DFP_CalChecksum()
{
	uint16 checksum = TxBuffer[DFP_MSG_IDX_VERSION];
	checksum += TxBuffer[DFP_MSG_IDX_LENGTH];
	checksum += TxBuffer[DFP_MSG_IDX_COMMAND];
	checksum += TxBuffer[DFP_MSG_IDX_FEEDBACK];
	checksum += TxBuffer[DFP_MSG_IDX_PARAM_MSB];
	checksum += TxBuffer[DFP_MSG_IDX_PARAM_LSB];
	
	checksum = 0xFFFF - checksum + 1;
	
	return checksum;
}

/****************************************************************************************/

#endif /* __DFPLAYER_H_ */