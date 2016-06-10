#include <stdio.h>
#include <stdlib.h>
#include "paging.h"

#define	BUFF_SIZE	1024

int main (int argc, char **argv){
	char	buffer[BUFF_SIZE];
	int		input, phys_addr;
	FILE	*file, *output;

	if (argc != 2){
		printf ("usage: %s <input file>\n", argv[0]);
		exit (-1);
	}

	if ((file = fopen (argv[1], "r")) == NULL){
		printf ("cannot open the %s\n", argv[1]);
		exit (-1);
	}

	output = fopen ("Physical.txt", "w");

	pag_init ();

	while (fscanf (file, "%d", &input) != EOF){
		printf ("input: %d\n", input);
		phys_addr = addr_tr (input);
		fprintf (output, "%d ", phys_addr);
	}

	tlb_hr();
	lru_hr();

	fclose (file);
	
	return 0;
}
