#include <stdio.h>
#include "ansin.h"
#include "alex.h"

int main()
{
	FILE *fis;
	fis = fopen("1.q", "rb");
	if (fis == NULL)
	{
		printf("nu s-a putut deschide fisierul");
		return -1;
	}
	int n = fread(bufin, 1, 30000, fis); // returneaza nr de elemente citite integral
	bufin[n] = '\0';
	fclose(fis);
	pch = bufin; // initializare pch pe prima pozitie din bufin
		// extragere atomi

	while (getNextTk() != FINISH)
	{
	}
	afisare_atomi();
	// afisare atomi

	putchar('\n');
	if (program())
		printf("Sintaxa corecta\n");
	else
		printf("Sintaxa incorecta\n");
	putchar('\n');
}