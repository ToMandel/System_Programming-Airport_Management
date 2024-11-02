#pragma warning(disable : 6031)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Company.h"
#include "AirportManager.h"
#include "General.h"
#include "main.h"

const char* str[eNofOptions] = { "Add Flight", "Add Airport",
"PrintCompany", "Print all Airports",
	"Print flights between origin-destination",
"Sort Flights", "Search Flight" };

int main(int argc, char* argv[])
{
	AirportManager	manager;
	Company			company;
	if (argc != 3)
		ERROR_MSG_PAUSE_RETURN_0("Error in main parameters\n");

	char* managFileName = argv[1];
	char* compFileName = argv[2];
	
	if(!initManagerAndCompany(&manager, &company, compFileName, managFileName))
		ERROR_MSG_PAUSE_RETURN_0("Error initializing company and airport manager\n");
	int option;
	int stop = 0;
	do
	{
		option = menu();
		switch (option)
		{
		case eAddFlight:
			if (!addFlight(&company, &manager))
				printf("Error adding flight\n");
			break;
		case eAddAirport:
			if (!addAirport(&manager))
				printf("Error adding airport\n");
			break;
		case ePrintCompany:
			printCompany(&company);
			break;
		case ePrintAirports:
			printAirports(&manager);
			break;
		case ePrintFlightOrigDest:
			printFlightsCount(&company);
			break;
		case eSortFlights:
			sortFlight(&company);
			break;
		case eSearchFlight:
			findFlight(&company);
			break;
		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);


	saveManagerToFile(&manager, managFileName);
	saveCompanyAfterCompress(compFileName, &company);
	freeManager(&manager);
	freeCompany(&company);
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initManagerAndCompany(AirportManager* pManager, Company* pCompany, const char* fileNameComp, const char* fileNameManag)
{
	int res = initManager(pManager, fileNameManag);
	if (!res)
	{
		printf("error init manager\n");
		return 0;
	}
	if (res == FROM_FILE)
		/*if (!createCompanyFromFile(fileNameComp, pCompany))*/
		return initCompanyFromFile(pCompany, pManager, fileNameComp);
	else
		initCompany(pCompany, pManager);
	return 1;
}