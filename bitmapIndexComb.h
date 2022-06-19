#ifndef _BitmapIndexComb_H
#define	_BitmapIndexComb_H
#include "inc.h"

//Using combinational method to package 2-points (position where 1 occurs in bitmap) plus time difference into one 32-bits code.
//
extern "C" {
class DLLEXPORT CBitmapIndexCombination {
public:
	CBitmapIndexCombination(); //for convert frame-based index to block-frame-based index to improve robustness by considering temporal info
	~CBitmapIndexCombination();
private:
	enum bit64AllocationIndex {
		anchorFreqStart=63,  //first 10bits for anchor frequency
		anchorBitSize=10,
		contextFreqStart=anchorFreqStart-anchorBitSize, //Following 10bits for context frequency
		contextBitSize=10,
		deltaTimeStart=contextFreqStart-contextBitSize,  //Following 7bits for delta time 
		deltaTimeBitSize=7,
		songIdStart=deltaTimeStart-deltaTimeBitSize,  //following 20bits for song id, so maximal 1M songs
		songIdBitSize=20,
		timeStampStart=songIdStart-songIdBitSize,  //lowest  17bits for absolute time stamp, 
		timeStampBitSize=17,
	};
	
	//bitmap feature file extension, same as in bitmap feature extraction 
	//variables for indexing 
	vector<string> m_songName; //song ID->song names
	UINT32	m_totalIndexedSong; //total indexed songs
	vector<UINT64> m_bitmapComb; //index for pair-wise points in bitmap, 
	UINT32	m_hashBucketSize;
	UINT64	m_totalIndexUint;
	//variables for query and match
	UINT16	*m_bitmap;
	UINT32	m_queryFrameNum;
	//feaure extraction parameters setting in feature extraction stage from audio
	UINT32	m_chromaDim;
	float	m_codingThreshold;
	float	m_codingThresholdHigh;
	UINT64	m_totalMemory;
	INT16 m_bitSizePerUnit;
	INT16 m_byteSizePerFrame;
	vector<pair<INT16, INT16>> m_bitPosPerFrame;
	//define bits number for each component in combinational hash, where it is pair [anchor_freq, neighbor_freq, deltaTime], total bits is 32
	INT16	m_freqBitSize;
	INT16	m_deltaTimeBitSize;
	INT16	m_indexUnitBitSize;
	//varibales for context range
	INT16	m_freqWinWidth; //half-frequency band width, centered at the band of current point
	INT16	m_timeWinWidth; //time-band width, from current point
	INT16	m_timeWinShift; //time-band shift frames from current point, i.e. time window is [anchor_point_time+m_timeWinShift, anchor_point_time+m_timeWinWidth]
	INT16 m_layerSize;
private:
	void	Initial();
	void	ExactMatch();
	void	ReleaseMemory();
	bool	DoingIndex(char *featureFile, char*songListFile, char*saveIndexFile, bool);
	int		CombIndex(UINT16* m_bitmap, UINT32 songFrameNum, vector<UINT64>&);
	inline int		CombKeyInContext(vector<vector<UINT32>>  onepos, int, vector<UINT64>& );
	bool SaveIndex(char*);
	pair<UINT32,int> Match(UINT16 *queryBitmap, int songFrameNum);
	inline UINT64 CombKeyGenerate(int centerfreq, int neighborfreq, int deltatime, int songid, int timestamp);
	inline void CombKeyInContextMatch(int centerFrameId, int centerBitId, UINT16 *frameIndex, UINT32 songFrameNum, hash_map<UINT64, int> &matchHist);
	inline void CombKeyInContextMatch(int centerFrameId, int centerBitId, hash_map<int,vector<int>> *frameIndex, UINT32 songFrameNum, vector<pair<int, UINT32>> &matchHist);
	void KeySearch(UINT64 targetKey, hash_map<UINT64, int>&) ;
public:
	bool Index(char *songLisFile, char*bitmapPath, int freqContextWidth, int timeContextWidth, int timeContextShift, char*saveIndexFile);
	bool LoadIndex(char* indexFile);
	//void QueryMatch(UINT16 *queryBitmap, int framenum, string &getSongName, int &getSongScore);
	void QueryMatch(UINT16 *queryBitmap, int songFrameNum, string &getSongName, int &getSongScore);

};
}

#endif