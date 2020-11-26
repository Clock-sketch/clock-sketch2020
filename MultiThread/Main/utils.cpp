#include <stdio.h>
#include "utils.h"

void ReadTraces(TRACE* traces)
{
    for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "../formatted%02d.dat", datafileCnt);

		FILE* fin = fopen(datafileName, "rb");
		if(fin == nullptr){
			printf("failed to open file: %s\n", datafileName);
			exit(1);
		}
		// printf("successfully open %s\n", datafileName);

		FIVE_TUPLE temp_five_tuple;
		traces[datafileCnt].clear();

		while (fread(temp_five_tuple.key, 1, ITEM_LENGTH, fin) == ITEM_LENGTH) {
			traces[datafileCnt].push_back(temp_five_tuple);
		}

		fclose(fin);
		printf("Successfully read in %s, %ld packets\n", datafileName, traces[datafileCnt].size());
	}
	printf("\n");
}


