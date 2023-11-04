#pragma once
#define STATE_SORTING 2
#define STATE_SHUFFLING 1
#define STATE_WAITING 0
#define STATE_STOPPED -1

namespace globals {
	int programState = 0;
	int shuffleDepth = 1;
	long long sortingTime;
	int sleepTime = 5;
}