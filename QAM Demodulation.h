/* (C) 2018 EsonJohn(zhangyithu@163.com) */

#include<vector>
#include<math.h>
using namespace std;

// demodulate voltage into constellation points represented by grey code
vector<vector<int>> QAMdemod(vector<vector<double>> I, vector<vector<double>> Q, int modulationOrder);

// normalize each row in I & Q
void normalize(vector<vector<double>> &I, vector<vector<double>> &Q);

// get grey codec according to index
int greyEncode(int idx, int order);

// the max function
inline double max(double x, double y);