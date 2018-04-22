#include <stdio.h>
#include <stdlib.h>

int query(int x, int y, int z) {
	printf("? %d %d %d\n", x, y, z);
	fflush(stdout);
	int ans = -1;
	(void)scanf("%d", &ans);
	if (ans == -1) exit(0);
	return ans;
}

[[noreturn]]
void guess(int x, int y, int z) {
	printf("! %d %d %d\n", x, y, z);
	exit(0);
}


int main() {
	int N, M, K, Q;
	(void)scanf("%d %d %d %d", &N, &M, &K, &Q);

	// TODO do something smart

	query(1, 1, 1);
	query(N, M, K);
	guess(1, 1, 1);
}
