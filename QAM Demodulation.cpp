/* (C) 2018 EsonJohn(zhangyithu@163.com) */

#include"QAM Demodulation.h"

vector<vector<int>> QAMdemod(vector<vector<double>> I, vector<vector<double>> Q, int modulationOrder)
{
	/* A C++ version of Matlab's qamdemod(y, M, varargin) 
	*  Align each symbol to the nearest constellation points by minimum Euclidean distance.
	*  The function only consider rectanglular constellation diagram and grey code.
	*  The input is assumed to have been downconverted into baseband.
	*  I is the I component of phase and Q is the Q component of phase.
	*  The I and Q should be NxM or 1xM matrix/vector, in which the N indicate the subchannels and M indicates the symbols.
	*  The function treats each row as a subchannel and normalizes them respectively.
	*/

	if (I.size() != Q.size() || I[0].size() != Q[0].size())
		return vector<vector<int>>(0, vector<int>(0, 0));

	// normalize I and Q components
	normalize(I, Q);

	// extract I and Q components' digits and order
	int channelNumber = I.size();
	int symbolNumber = I[0].size();
	int bitsI, bitsQ;
	if ((int)log2(modulationOrder) % 2 == 1)
	{
		bitsI = (int)log2(modulationOrder) / 2 + 1;
		bitsQ = (int)log2(modulationOrder) - bitsI;
	}
	else{
		bitsI = (int)log2(modulationOrder) / 2;
		bitsQ = (int)log2(modulationOrder) - bitsI;
	}
	int orderI = pow(2, bitsI);
	int orderQ = pow(2, bitsQ);

	// align each subchannel and each symbol to the constellation point
	vector<vector<int>> digits = vector<vector<int>>(channelNumber, vector<int>(symbolNumber));
	for (int channel = 0; channel < channelNumber; channel++)
	{
		for (int symbolIdx = 0; symbolIdx < symbolNumber; symbolIdx++)
		{
			double voltage_I = I[channel][symbolIdx], voltage_Q = Q[channel][symbolIdx];
			int greyIdx_I, greyIdx_Q;
			greyIdx_I = floor((voltage_I - (-1.0 + 1.0 / ((double)orderI - 1.0))) / (2.0 / ((double)orderI - 1.0)));
			greyIdx_Q = floor((voltage_Q - (-1.0 + 1.0 / ((double)orderQ - 1.0))) / (2.0 / ((double)orderQ - 1.0)));
			greyIdx_I = (greyIdx_I < 0) ? 0 : ((greyIdx_I >= (orderI - 2)) ? orderI - 1 : greyIdx_I + 1);
			greyIdx_Q = (greyIdx_Q < 0) ? 0 : ((greyIdx_Q >= (orderQ - 2)) ? orderQ - 1 : greyIdx_Q + 1);
			greyIdx_Q = orderQ - greyIdx_Q - 1;
			digits[channel][symbolIdx] = greyEncode(greyIdx_I, orderI) * orderQ + greyEncode(greyIdx_Q, orderQ);
		}
	}

	return digits;
}

// normalize each row in I & Q
void normalize(vector<vector<double>> &I, vector<vector<double>> &Q)
{
	vector<double> rowMax = vector<double>(I.size(), 0.0);
	for (int j = 0; j < I[0].size(); j++)
	{
		for (int i = 0; i < I.size(); i++)
		{
			rowMax[i] = max(rowMax[i], max(I[i][j], Q[i][j]));
		}
	}
	for (int j = 0; j < I[0].size(); j++)
	{
		for (int i = 0; i < I.size(); i++)
		{
			I[i][j] /= rowMax[i];
			Q[i][j] /= rowMax[i];
		}
	}
}

// get grey codec according to index
int greyEncode(int idx, int order)
{
	unsigned int code = 0;
	while (order > 1)
	{
		code <<= 1;
		if (idx >= order / 2)
		{
			code += 1;
			idx -= order / 2;
			idx = order / 2 - 1 - idx;
		}
		order /= 2;
	}

	return code;
}

// the max function
inline double max(double x, double y)
{
	return x > y ? x : y;
}