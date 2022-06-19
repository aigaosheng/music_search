#ifndef _INC_H
#define	_INC_H

#define _USE_MATH_DEFINES //use constant defined in math.h
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<float.h>
#include	<conio.h>
#include	<assert.h>
#include	<iostream>
#include	<fstream>
#include	<string.h>
#include	<vector>
#include	<bitset>
 #include <algorithm>
#include <hash_map>
#include <time.h>
#include <hash_set>


#define		UINT8		unsigned __int8 
#define		UINT16	unsigned __int16
#define		UINT32	unsigned __int32
#define		UINT64	unsigned __int64
#define		INT8	__int8
#define		INT16	__int16
#define		INT32	__int32
#define		INT64	__int64

#define	DEFAULT_CHROMA_DIMENSION 12
#define	MAX_SPEECH_FRAME_LEN	100000
//#define LOW_BAND_FREQ	100//0//50
//#define	HIGH_BAND_FREQ 8000//high frequence seg not useful
#define	MAX_FILE_NAME_LEN 512
#define LOW_MIDI_NOTE	1
#define HIGH_MIDI_NOTE	119


#define DLLEXPORT  __declspec(dllexport)

using namespace std;

typedef enum  {
		getChromaBit, getFftBit, getMidiBit
}bitMapTypeConst;

#endif