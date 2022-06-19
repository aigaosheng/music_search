#ifndef _BitmapIndex_H
#define	_BitmapIndex_H
#include "inc.h"

//#define MIN_WISE_SIMILARITY
//#define STRUCTURE_INDEX

#define	CONFIDENCE_ACCEPT	2
#define	CONFIDENCE_NOTHING	1
#define	CONFIDENCE_REJECT		0

extern "C" {
 class DLLEXPORT CBitmapIndex {
public:
	CBitmapIndex(); //for convert frame-based index to block-frame-based index to improve robustness by considering temporal info
	~CBitmapIndex();
private:
	//bitmap feature file extension, same as in bitmap feature extraction 
	//variables for indexing 
	vector<string> m_songName; //song ID->song names
	UINT32	m_totalIndexedSong; //total indexed songs
	vector<UINT32> *m_bitmapInvert; //inverted index for bitmap feature, pair<first, second>: 1. song ID, 2. index-unit ID
	vector<UINT32> m_indexUint2Song;
	//vector<UINT32> m_indexUintBoundaryLow;
	//vector<UINT32> m_indexUintBoundaryHigh;
	UINT32	m_totalIndexUint;
	UINT64	m_totalIndexTermPoint;
	INT16		m_frameSampleRate;
	//parameters for index
	UINT16	m_frameNumOfIndexUnit; //how many frames concatenate together to form one index unit
	UINT32	m_indexUnitDim; //how many bits for one index unit
	//variables for query and match
	UINT16	*m_bitmap;
	UINT32	m_queryFrameNum;
	//feaure extraction parameters setting in feature extraction stage from audio
	UINT32	m_chromaDim;
	float	m_codingThreshold;
	float m_codingThresholdHigh;
	UINT64	m_totalMemory;
	INT16 m_bitSizePerUnit;
	INT16 m_byteSizePerFrame;
	vector<bool> m_selectedTerm;
	hash_map<INT32, vector<UINT16>> m_queryKeyInv;
	INT16 m_layerSize;
	//defined for cluster index units for structure indexing scheme, March 28, 2012
	float *m_wMix, *m_thetaMix, *m_thetaLogSum;
	UINT16 m_clusterK;
	int	m_structureMatchTopN;

#ifdef	MIN_WISE_SIMILARITY
	vector<UINT32> m_indexUnitLen; //for min-wise similarity score, Feb.8, 2013
#endif
private:
	void	Initial();
	void	ExactMatch();
	void	ReleaseMemory();
	bool	DoingIndex(char *featureFile, char*songListFile, char*saveIndexFile, bool);
	int		Frame2BlockIndex(UINT16* m_bitmap, UINT32 songFrameNum);
	bool SaveIndex(char*);
#ifdef MIN_WISE_SIMILARITY
	pair<UINT32,float> Match(int indexBlockNum, int shift, int songFrameNum, UINT16 *queryBitmap);
#else
	pair<UINT32,int> Match(int indexBlockNum, int shift, int songFrameNum, UINT16 *queryBitmap);
#endif
	void MatchBlock(int indexBlockId, int songFrameNum, UINT16 *queryBitmap, vector<hash_map<UINT32,int>> &);

	void QueryRandomSelection(int songFrameNum, UINT16 *queryBitmap);
	pair<UINT32,int> Match3(int indexBlockNum, int shift, int songFrameNum, UINT16 *queryBitmap);
	pair<UINT32,int> Match2(int indexBlockNum, int totalBlockNum);
	
	void FeatureSelection(int);
	void FeatureSelection2(int);
	void GetQueryKey(UINT16 *queryBitmap, int framenum, int indexBlockNum, int pseudoFrameFrameShift);
	void ShiftSelection(UINT16 *queryBitmap, int framenum, int indexBlockNum, int pseudoFrameFrameShift, hash_set<int> &m_selectedShift);
	//
	int		GetConfidenceValue(vector<string> getSongName, vector<int> getSongScore);
	//
	void ReducePoints(UINT16 *frameIndex, UINT32 songFrameNum);
	//group index units
	void ClusterIndexUnits(int clusterKbit, char* filename, vector<vector<UINT16>> &bitmapIndexInst, int featureDim, int	instNum) ;
	bool StructureIndex(char*,char*);
	int		FindMatchCluster(vector<UINT16> &key, vector<int>&);
	inline float	GetSingleModelScoreThetaRatio(vector<UINT16> &key, int clusterId);
	inline float	GetSingleModelScore(vector<UINT16> &key, int clusterId);
	void	SamplingClusterPoints(vector<vector<UINT16>> &inst, int);

public:
	void	LoadIndexStructure(char*filename, int topnStructure);
	bool Index(char *songLisFile, char*bitmapPath, char*oldIndexFile, char*saveIndexFile) ;
	bool Index(char *songLisFile, char*bitmapPath, int frameOfIndexBlock, int frameSampleRate, char*saveIndexFile);
	bool LoadIndex(char* indexFile);
	void QueryMatch(UINT16 *queryBitmap, int framenum, string &getSongName, int &getSongScore);
	int	 QueryMatchParallel(UINT16 *queryBitmap, int framenum, vector<string> &getSongName, vector<int> &getSongScore, int topN);
	void QueryMatchParallel2(UINT16 *queryBitmap, int framenum, string &getSongName, int &getSongScore);
	void QueryMatch(hash_map<int,vector<int>> &queryBitmap, int framenum, string &getSongName, int &getSongScore);
	void QueryMatch(CAudioBitmap *queryBitmap, string &getSongName, int &getSongScore) ;
	bool	IndexCluster(char* indexFile, char*, int, int bitSizeCluster);

};
}

#endif