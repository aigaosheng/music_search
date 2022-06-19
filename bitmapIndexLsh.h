#ifndef _BitmapIndexLsh_H
#define	_BitmapIndexLsh_H
#include "inc.h"

typedef struct {
	vector<pair<UINT32, INT32>> bit; //first is bit pos, second is bit thresh
	hash_map<UINT32, vector<UINT32>> bucket; 
}SBitLsh;


extern "C" {
class DLLEXPORT CBitmapIndexLsh {
public:
	CBitmapIndexLsh(); //for convert frame-based index to block-frame-based index to improve robustness by considering temporal info
	~CBitmapIndexLsh();
private:
	//bitmap feature file extension, same as in bitmap feature extraction 
	//variables for indexing 
	vector<string> m_songName; //song ID->song names
	UINT32	m_totalIndexedSong; //total indexed songs
	SBitLsh *m_bitHash; //save hash function for binary LSH, each function is vector<UINT32>
	int		m_hashTableSize; //set how many tables are used
	int		m_hashFuncSize; //set how many bits selected to build hash bucket
	UINT32	m_totalIndexUint;
	UINT16 *m_bitmapDatabase;
	vector<UINT32> m_indexUint2Song;
	//parameters for index
	UINT16	m_frameNumOfIndexUnit; //how many frames concatenate together to form one index unit
	UINT32	m_indexUnitDim; //how many bits for one index unit
	//variables for query and match
	vector<string> m_matchedDocs;
	UINT16	*m_bitmap;
	UINT32	m_queryFrameNum;
	//feaure extraction parameters setting in feature extraction stage from audio
	UINT32	m_chromaDim;
	float	m_codingThreshold;
	float	m_codingThresholdHigh;
private:
	void	Initial();
	void	ReleaseMemory();
	bool	DoingIndex(char *featureFile, char*songListFile, char*saveIndexFile, bool);
	int	Frame2BlockIndex(UINT16* m_bitmap, UINT32 songFrameNum);
	bool SaveIndexLsh(char*);
	void	GenerateHashFunction();
	void FindBestInst(vector<pair<int,int>> feat, hash_set<UINT32> candInst, UINT32 &bestpos, int &bestinstscore);
public:
	bool Index(char *songLisFile, char*bitmapPath, char*oldIndexFile, char*saveIndexFile) ;
	bool Index(char *songLisFile, char*bitmapPath, int frameOfIndexBlock, int chromaDim, int lshTableSize, int hashBitSize, char*saveIndexFile);
	bool LoadIndexLsh(char* indexFile);
	void QueryMatch(UINT16 *queryBitmap, int framenum, string &getSongName, int &getSongScore);

};
}
#endif