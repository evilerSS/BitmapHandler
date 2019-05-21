#pragma once

typedef struct foretree{
	BYTE*			 data;
	struct foretree* first;
	struct foretree* second;
	struct foretree* third;
	struct foretree* fourth;
} ForeTree;