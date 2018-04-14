#include <iostream>
using namespace std;

int main(){
	int n, m, k, maxq;
	cin >> n >> m >> k >> maxq;
	// m, k should be 1

	int lo = 1, hi = n+1;
	// we want local maximum somewhere in [lo, hi)
	while(lo+1 < hi){
		int mi = (lo + hi) / 2;

		int r1, r2;
		cout << "? " << mi-1 << " 1 1" << endl;
		cin >> r1;
		cout << "? " << mi   << " 1 1" << endl;
		cin >> r2;

		if(r1 >= r2) hi = mi;
		else lo = mi;
	}
	
	cout << "! " << lo << " 1 1" << endl;
}
