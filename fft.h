#ifndef _FFT_H
#define _FFT_H

#include "inc.h"

extern "C" {
DLLEXPORT class CFft {
private:
	void FFT(float *s, int, int);
public:
	void Realft (float *s, int);
};
}
#endif