#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>

#define PORT 0x378


//---------------------------------------- GLOBAL DECLARES ------------------------------------


// Alarm yapýsý zaman ve günleri tutar..
struct alarm {
	int hour;
	int minute;
	struct alarm_day {
		int size;
		int week_day[7];
	} days;
};


//------------------------------------- DLL READING PROTOTYPES --------------------------------


// prototype (function typedef) for DLL function Inp32: 
typedef void (_stdcall *oupfuncPtr)(int portaddr, int datum);

// After successful initialization, this variable will contain function pointers
oupfuncPtr oup32fp;

// Wrapper functions for the function pointers. Call these functions to perform I/O.
void  Out32 (int portaddr, int datum){ (oup32fp)(portaddr,datum); }


//--------------------------------------- FUNCTION PROTOTYPES ---------------------------------


int		consoleWidth(void);
void	timeDisplay(struct tm *_pTime, int _width);
void	timeDisplayforWIN(struct tm *_pTime, int _width);
void	timeDisplayforOTHER(struct tm *_pTime);
void	daySelect(struct alarm *source);
void	alarmDisplay(struct tm *pTime, int _width);
void	alarmDisplayWIN(struct tm *pTime, int _width);
void	alarmDisplayOTHER(struct tm *pTime);
void	alarmLoop(struct alarm *source, int size);


//----------------------------------------- MAIN FUNCTION -------------------------------------


int main(int argc, char** argv)
{
	system("CLS");
	system("COLOR 4E");
	
	HINSTANCE		hLib;
	struct alarm	*alarms;
	int				alarmNumber;

	/* Load the library */
	hLib = LoadLibrary("inpout32.dll");
	
	if (hLib == NULL) {
		fprintf(stderr,"LoadLibrary Failed.\n");
		system("PAUSE");
		return -1;
	}
	
	oup32fp = (oupfuncPtr) GetProcAddress(hLib, "Out32");
	if (oup32fp == NULL) {
		fprintf(stderr,"GetProcAddress for Oup32 Failed.\n");
		system("PAUSE");
		return -1;
	}
	
	/* Dll fonksiyonlarý baþarýyla oluþturuldu */
	Out32(PORT,0);



	printf("Kac tane alarm girmek istiyorsunuz: ");
	scanf("%d", &alarmNumber);
	alarms = (struct alarm *)calloc(alarmNumber, sizeof(struct alarm));
	
	int i;
	for(i=0; i<alarmNumber; i++)
	{
		system("CLS");
		
		printf("%d. Alarm icin;\n\n", i+1);
		printf("Saat giriniz: ");
		scanf("%d", &alarms[i].hour);

		printf("Dakika giriniz: ");
		scanf("%d", &alarms[i].minute);
		
		printf("\n\n");
		daySelect( &alarms[i] );
	}

	alarmLoop(alarms, alarmNumber);



	/* Portu söndür*/
    Out32(PORT, 0);
	
	/* Ýþlemler bitti, dll'i serbest býrak */
	FreeLibrary(hLib);
     
	printf("\n");
    system("PAUSE");
    return EXIT_SUCCESS;
}


//------------------------------------------- FUNCTIONS ---------------------------------------


int consoleWidth(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int ret;
	ret = GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ), &csbi);
	if(ret)
		return csbi.dwSize.X;
	else
		return 0;
}


void timeDisplay(struct tm *_pTime, int _width)
{
	#ifdef _WIN32
			timeDisplayforWIN(_pTime, _width);
	#elif _WIN64
			timeDisplayforWIN(_pTime, _width);
	#else
			timeDisplayforOTHER(_pTime);
	#endif
}


void timeDisplayforWIN(struct tm *_pTime, int width)
{
	if(width >= 50)
	{
		system("CLS");

		printf("\n");
		int i;
		for(i=0; i<width-17; i++) printf(" ");
		printf("--------------\n");
		for(i=0; i<width-18; i++) printf(" ");
		printf("|   %02d:%02d:%02d   |\n", _pTime->tm_hour, _pTime->tm_min, _pTime->tm_sec);
		for(i=0; i<width-18; i++) printf(" ");
		printf("|  %02d.%02d.%d  |\n", _pTime->tm_mday, _pTime->tm_mon+1, _pTime->tm_year+1900);
		for(i=0; i<width-17; i++) printf(" ");
		printf("--------------\n");
	}
	else
	{
		timeDisplayforOTHER(_pTime);
	}
}


void timeDisplayforOTHER(struct tm *_pTime)
{
	system("CLS");

	printf("\n");
	printf("  --------------\n");
	printf(" |   %02d:%02d:%02d   |\n", _pTime->tm_hour, _pTime->tm_min, _pTime->tm_sec);
	printf(" |  %02d.%02d.%d  |\n", _pTime->tm_mday, _pTime->tm_mon+1, _pTime->tm_year+1900);
	printf("  --------------\n");
}


void daySelect(struct alarm *source)
{
	int select = 0;
	printf("Alarm Gunleri: \n\n");
	printf(" ------------------- \n");
	printf("|                   |\n");
	printf("|  1. Tek Sefer     |\n");
	printf("|  2. Her Gun       |\n");
	printf("|  3. Gun Seciniz   |\n");
	printf("|                   |\n");
	printf(" ------------------- \n\n");
	printf("Secim: ");
	scanf("%d", &select);
	
	switch(select)
	{
		case 1: break;
		case 2:
		{
			int i;
			for(i=0; i<7; i++)
			{
				source->days.week_day[i] = i;
				source->days.size++;
			}
		}
		break;
		case 3:
		{
			do
			{
				system("cls");
				printf("\n Gunleri secip cikis yapiniz!\n\n");
				printf(" ------------------ \n");
				printf("|                  |\n");
				printf("|  0. Pazar        |\n");
				printf("|  1. Pazartesi    |\n");
				printf("|  2. Sali         |\n");
				printf("|  3. Carsamba     |\n");
				printf("|  4. Persembe     |\n");
				printf("|  5. Cuma         |\n");
				printf("|  6. Cumartesi    |\n");
				printf("|                  |\n");
				printf("|  7. Cikis        |\n");
				printf("|                  |\n");
				printf(" ------------------ \n\n");
				printf("Secim: ");
				scanf("%d", &select);
				
				int equal = 0;
				int i;
				for(i=0; i<source->days.size; i++)
				{
					if( source->days.week_day[i] == select )
						equal = 1;
				}
				
				if( (select != 7) && !equal )
				{
					source->days.week_day[source->days.size] = select;
					source->days.size++;
				}
			} while ( select != 7);
			break;
		}

		default: break;
	}
}


void alarmDisplay(struct tm *pTime, int _width)
{
	#ifdef _WIN32
		alarmDisplayWIN(pTime, _width);
	#elif _WIN64
		alarmDisplayWIN(pTime, _width);
	#else
		alarmDisplayOTHER(pTime);
	#endif
}


void alarmDisplayWIN(struct tm *pTime, int _width)
{
	if( (pTime->tm_sec % 2) == 0)
	{
		int i;

		printf("\n");
		for(i=0; i<(_width-20)/2; i++) printf(" ");
		printf("----------------\n");
		for(i=0; i<(_width-19)/2-1; i++) printf(" ");
		printf("|      %02d:%02d     |\n", pTime->tm_hour, pTime->tm_min);
		for(i=0; i<(_width-19)/2-1; i++) printf(" ");
		printf("|                |\n");
		for(i=0; i<(_width-19)/2-1; i++) printf(" ");
		printf("|    WAKE UP!    |\n");
		for(i=0; i<(_width-20)/2; i++) printf(" ");
		printf("----------------\n\n");
		printf("\n");
	}
	else
	{
		printf("\n\n\n\n\n\n\n\n");
	}
}


void alarmDisplayOTHER(struct tm *pTime)
{
	if( (pTime->tm_sec % 2) == 0)
	{
		printf("\n");
		printf("  ----------------\n");
		printf(" |      %02d:%02d     |\n", pTime->tm_hour, pTime->tm_min);
		printf(" |                |\n");
		printf(" |    WAKE UP!    |\n");
		printf("  ----------------\n\n");
		printf("\n");
	}
	else
	{
		printf("\n\n\n\n\n\n\n\n");
	}
}


void alarmLoop(struct alarm *source, int size)
{
	int consoleX, hour, minute, day;
	consoleX = consoleWidth();

	do
	{
		time_t timer;
		struct tm *local;
		timer = time(NULL);
		local = localtime(&timer);

		timeDisplay(local, consoleX);

		int i;
		for(i=0; i<size; i++)
		{
			hour = source[i].hour;
			minute = source[i].minute;
			if( source[i].days.size == 0 )
			{
				if( (hour == local->tm_hour) && (minute == local->tm_min) )
				{
					alarmDisplay(local, consoleX);
					Out32(PORT, 2);
					printf("Out32(PORT,2);");
					break;
				}
				else
				{
					// Out32(PORT,0);
					printf("\nPort Kapandý.\n");
				}
			}
			else
			{
				int j;
				for(j=0; j<source[i].days.size; j++)
				{
					day = source[i].days.week_day[j];
					if( (hour == local->tm_hour) && (minute == local->tm_min) && (day == local->tm_wday) )
					{
						alarmDisplay(local, consoleX);
						Out32(PORT,2);
						printf("Out32(PORT,2);");
						break;
					}
					else
					{
						// Out32(PORT,0);
						printf("\nPort Kapandý. Day\n");
					}
				}
			}
		}
		
		Sleep(1000);

		if( _kbhit() )
		{
			if( _getch() == 27 )
				break;
		}
	} while (!NULL);
}