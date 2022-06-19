#ifndef	_GETAUDIOBITMAP_H
#define	_GETAUDIOBITMAP_H
#include "inc.h"
#include "fft.h"

#define	EnergyThresholdSpectral	200
#define	EnergyZeroBound	300

typedef struct {
	int center;
	int low;
	int high;
}SMidiFilter;

extern "C" {
class DLLEXPORT CAudioBitmap: CFft {
public:
	CAudioBitmap(int bitMapType, float lowfreq, float highfreq, int frameLen, int frameShiftLen, int sampleRate, int blockLen, float threshold, float thresholdUppperLimit, int layerBitmapNum, int *layerBitmapWindow, bool m_bitOneFrameIndex);
	~CAudioBitmap();
private:
	INT16	m_bitMapType;
	INT16	m_byteSizePerFrame;
	INT16	m_bitSizePerUnit; //we use N bits as feature store unit, when dimension is >N, then multiple unit is used. e.g.  m_bitSizePerUnit=16, then 16 bit element in one int16, if feature >16, multiple units are used
	bool m_isSuccess;
	float m_codingThreshold;
	float m_codingThresholdHigh;
	INT16 *m_sample;
	float *m_realFft;
	float *m_spectrum; //store FFT spectrum
	float *m_spectrumInterpolate; //store FFT spectrum
	vector<SMidiFilter>	m_noteFreqList; //store corresponding note for each frequency
	vector<float>	m_midiFreq;
	INT16	*m_freqNote; //store corresponding note for each frequency
	float *m_chroma;
	UINT16 *m_bitmap; //store extracted bit stream for audio 
	UINT32 m_frameNum;
	UINT32 m_frameLen;
	UINT32 m_frameShiftLen;
	UINT32 m_chromaDim;
	UINT32 m_sampleRate;
	float m_frameSecond;
	float m_frameShiftSecond;
	UINT32 m_blockLen;
	UINT32 m_sampleNum;
	UINT32	m_totalAudioNum;
	UINT32	m_totalFrameNum;
	UINT32 m_sPos, m_ePos;
	UINT32	m_detectFrameNum;
	UINT32 m_queryFrameNum; //frame number of query with shift
	hash_map<int,vector<int>> m_bitOneFrame; //store frames & bit-one position, key=frame
	bool m_bitOneFrameIndex; //if true, store one bit-one for each frame in m_bitOneFrame
	float m_lowFreq;
	float m_highFreq;
	INT16	m_midiNoteLow;
	INT16	m_midiNoteHigh;
	//define how many layers used for extracting hierarchical bitmap when considering band rcontext
	INT16		m_layerSize;
	vector<int>		m_layerContextWinSize;
	int	m_byteSizePerFrameLayer;
	//UINT32	m_superBitDim;
	vector<pair<int,float>> *m_normSpectrum;

private:
	int	Freq2Midinote(float freq);
	float Midinote2Freq(int midinote);
	void FreqMidinoteMap(void);
	void MidinoteFreqMap(void);
	bool ReadWaveAudio(char* wavefile);
	bool ReadPcmAudio(char* wavefile);
	bool ReadFftAudio(char* wavefile);
	int	GetWAVHeaderInfo(FILE *f);
	void SaveBitmapHeader(ofstream&);
	bool SaveBitmap(char *outfile);
	void	ComputeFft(short *data, int datalen);
	void ComputeBitmap() ;
	void	ComputeBitmapFft();
	void	ComputeBitmapFftWeight();
	void ComputeBitmapFftReduce();
	void CalculateBandWeight(vector<float>&, int num);
	void	ComputeBitmapMidi();
	void CheckSilence();
	void ResetVar();
	void ComputeBitmapShift(int shift);
	void FftInterpolate();
public:
	bool IsSuccess();
	bool ExtractBitmap(char* wavefileList, char* wavpath, char* filetype, char* savepath, bool useSparseBitmap); //for batch extraction of a list of audio files in wavefile
	bool ExtractBitmap(char* wavefile, char*audiotype, bool useSparseBitmap); //extract single file
	bool ExtractBitmap(short int *pdat, int sampleSize, bool useSparseBitmap);// extract from stream
	UINT16* GetBitmap(int &frameNum); //get bitmap just extracted
	void GetBitmap(hash_map<int, vector<int>> &frameBitOne, int &frameNum) ;
	UINT16* GetBitmapShift(int &frameNum, int shift); //get bitmap just extracted
	hash_map<int,vector<int>>*  GetBitmapCompress(int &frameNum);
	bool IsBitOneCompress() {return m_bitOneFrameIndex;}
};
}
#endif
