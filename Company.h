#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"

typedef enum { eNone, eHour, eDate, eSorceCode, eDestCode, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];

typedef struct
{
	char* name;
	int			flightCount;
	Flight** flightArr;
	eSortOption	sortOpt;
	LIST		flighDateList;
}Company;

int		initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName);
void	initCompany(Company* pComp, AirportManager* pManaer);
void	initDateList(Company* pComp);
int		isUniqueDate(const Company* pComp, int index);
int		addFlight(Company* pComp, const AirportManager* pManager);
void	printCompany(const Company* pComp);
void	printFlightsCount(const Company* pComp);
char* printCompanyName(char* word, ...);
int		saveCompanyToFile(const Company* pComp, const char* fileName);
int		loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName);
int		createCompanyFromFile(const char* fileName, Company* pComp);
void	getCompanyInfo(FILE* fp, Company* pComp);
int		saveCompanyAfterCompress(const char* fileName, const Company* pComp);
void	sortFlight(Company* pComp);
void	findFlight(const Company* pComp);
eSortOption showSortMenu();
void	freeCompany(Company* pComp);
#endif

