#pragma warning(disable : 6031)
#pragma warning(disable : 6387)
#pragma warning(disable : 6308)
#pragma warning(disable : 6386)
#pragma warning(disable : 28183)
#pragma warning(disable : 26451)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "fileHelper.h"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Hour", "Date", "Airport takeoff code", "Airport landing code" };


int	initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flighDateList);
	if (createCompanyFromFile(fileName, pComp))
	{
		initDateList(pComp);
		return 1;
	}
	return 0;
}

char* printCompanyName(char* word, ...) {
	va_list allWords;
	char* str;
	va_start(allWords, word);
	str = word;
	char* res = NULL;
	while (str != NULL)
	{
		if (res == NULL)
		{
			res = (char*)malloc((strlen(word) + 1) * sizeof(char));
			strcpy(res, str);
		}
		else
		{
			res = (char*)realloc(res, (strlen(res) + strlen(str) + 2) * sizeof(char));
			strcat(res, "_");
			strcat(res, str);
		}
		str = va_arg(allWords, char*);
	}
	va_end(allWords);
	return res;
}

void	initCompany(Company* pComp, AirportManager* pManaer)
{
	printf("-----------  Init Airline Company\n");
	L_init(&pComp->flighDateList);
	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
}

void	initDateList(Company* pComp)
{
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isUniqueDate(pComp, i))
		{
			char* sDate = createDateString(&pComp->flightArr[i]->date);
			L_insert(&(pComp->flighDateList.head), sDate);
		}
	}
}

int		isUniqueDate(const Company* pComp, int index)
{
	Date* pCheck = &pComp->flightArr[index]->date;
	for (int i = 0; i < index; i++)
	{
		if (equalDate(&pComp->flightArr[i]->date, pCheck))
			return 0;
	}
	return 1;
}

int		addFlight(Company* pComp, const AirportManager* pManager)
{
	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
		return 0;
	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	if (isUniqueDate(pComp, pComp->flightCount))
	{
		char* sDate = createDateString(&pComp->flightArr[pComp->flightCount]->date);
		L_insert(&(pComp->flighDateList.head), sDate);
	}
	pComp->flightCount++;
	return 1;
}



void printCompany(const Company* pComp)
{
	char* name = printCompanyName(pComp->name, "Hachi", "Babit", "Ba", "Olam", NULL);
	printf("%s \n", name);
	printf("Has %d flights\n", pComp->flightCount);
#ifdef DETAIL_PRINT
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flighDateList, printStr);
#endif //DETAIL_PRINT
}

void printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];
	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}
	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);
	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");
	printf("from %s to %s\n", codeOrigin, codeDestination);
}

int		saveCompanyToFile(const Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp, "Error open copmpany file to write\n");
	if (!writeStringToFile(pComp->name, fp, "Error write comapny name\n"))
		return 0;
	if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
		return 0;
	if (!writeIntToFile((int)pComp->sortOpt, fp, "Error write sort option\n"))
		return 0;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!saveFlightToFile(pComp->flightArr[i], fp))
			return 0;
	}
	fclose(fp);
	return 1;
}

int loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}
	pComp->flightArr = NULL;
	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	if (!pComp->name)
		return 0;
	if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flight count name\n"))
		return 0;
	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
		return 0;
	pComp->sortOpt = (eSortOption)opt;
	if (pComp->flightCount > 0)
	{
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		CHECK_NULL_MSG_CLOSE_FILE(pComp->flightArr, fp, "Allocation error\n");
	}
	else
		pComp->flightArr = NULL;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		CHECK_NULL_MSG_CLOSE_FILE(pComp->flightArr[i], fp, "Allocation error\n");
		if (!loadFlightFromFile(pComp->flightArr[i], pManager, fp))
			return 0;
	}
	fclose(fp);
	return 1;
}

int	createCompanyFromFile(const char* fileName, Company* pComp)
{
	FILE* fp;
	int res = 0;
	fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	getCompanyInfo(fp, pComp);
	pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)malloc(pComp->flightCount * sizeof(Flight));
		if (!pComp->flightArr[i])
		{
			fclose(fp);
			return 0;
		}
		res = unCompressedFlight(fp, pComp->flightArr[i]);
	}
	fclose(fp);
	return res;
}

void	getCompanyInfo(FILE* fp, Company* pComp)
{
	BYTE data[2] = { 0 };
	if (fread(data, sizeof(BYTE), 2, fp) != 2) {
		fclose(fp);
		return;
	}
	pComp->flightCount = data[0] >> 7 | data[1] << 1;
	pComp->sortOpt = (data[0] >> 4) & 0X07;
	int lenghtName = data[0] & 0X0F;
	pComp->name = (char*)malloc((lenghtName + 1) * sizeof(char));
	if (!readCharsFromFile(pComp->name, lenghtName + 1, fp, "error read name"))
		return;
}

int saveCompanyAfterCompress(const char* fileName, const Company* pComp)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	BYTE data[2] = { 0 };
	data[0] = (pComp->flightCount & 0X1) << 7 | (pComp->sortOpt& 0x7) << 4 | ((int)strlen(pComp->name)&0xF);
	data[1] = (pComp->flightCount >> 1)&0xFF;
	if (fwrite(data, sizeof(BYTE), 2, fp) != 2)
		return 0;
	if (!writeCharsToFile(pComp->name, (int)strlen(pComp->name) + 1, fp, "error write company name"))
		return 0;
	int res;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		res = saveFlightToBinaryFileCompressed(pComp->flightArr[i], fp);
		if (res == 0)
			return 0;
	}
	fclose(fp);
	return 1;
}

void	sortFlight(Company* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;
	switch (pComp->sortOpt)
	{
	case eHour:
		compare = compareByHour;
		break;
	case eDate:
		compare = compareByDate;
		break;
	case eSorceCode:
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		compare = compareByCodeDest;
		break;
	}
	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
}

void	findFlight(const Company* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;
	switch (pComp->sortOpt)
	{
	case eHour:
		f.hour = getFlightHour();
		compare = compareByHour;
		break;
	case eDate:
		getchar();
		getCorrectDate(&f.date);
		compare = compareByDate;
		break;
	case eSorceCode:
		getchar();
		getAirportCode(f.originCode);
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		getchar();
		getAirportCode(f.destCode);
		compare = compareByCodeDest;
		break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pF == NULL)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}
}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);
	return (eSortOption)opt;
}

void	freeCompany(Company* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->flighDateList, freePtr);
}
