#pragma once

typedef enum
{
	eAddFlight, eAddAirport, ePrintCompany, ePrintAirports, ePrintFlightOrigDest,
	eSortFlights, eSearchFlight,
	eNofOptions
} eMenuOptions;

const char* str[eNofOptions];

#define EXIT			-1
#define MANAGER
#define COMPANY

//#define FILE_NAME "airport_authority.txt"
//#define FILE_NAME "company.bin"
int menu();
int initManagerAndCompany(AirportManager* pManager, Company* pCompany, const char* fileNameComp, const char* fileNameManag);