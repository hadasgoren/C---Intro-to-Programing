#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER 255
#define TRUE 1
#define FALSE 0

//Struct Definitions
typedef struct {
	int day;
	int month;
	int year;
}Date;

typedef struct {
	int id;
	char *name;
	int shirtNum;
	Date birthday;
}Player;

typedef struct {
	char *teamName;
	int maxNum;
	int actualNum;
	Player *players;
	int wins;
}Team;

//Function Declaration
void readPlayer(Player *player, Team *team);
int checkID(int playerID);
int checkShirt(int shirtNum, Team *team);
void initTeam(Team *team, Team *teams);
int isUnique(char *teamName, Team *teams);
void addPlayer(Team* team);
Team* maxSuccess(Team *teams, int size);
Player** nameLikeA(Team *team, int* size);
int isBirthday(Date date);
int checkDate(Date date);
void printPlayer(Player player);
void printTeam(Team team);
void freeTeam(Team *team);

void main()
{
	int size, i, sizeA;
	Team* teams=NULL;
	Player** theATeam;

	printf("How many teams would you like to enter? ");
	scanf("%d", &size);
	getchar();
	printf("\n");
	teams = (Team*)malloc(sizeof(Team)*size);

	//Check Allocation
	if (teams == NULL)
	{
		printf("Memory was not assigned\n");
		exit(0);
	}

	for (i = 0; i < size; i++)
	{
		initTeam(teams + i, teams);
		addPlayer(teams + i);
		addPlayer(teams + i);
		printTeam(teams[i]);
	}

	printf("\n\nThe team with the most wins is: %s\n", (maxSuccess(teams, size)->teamName));

	//Not requested in instructions - just to check nameLikeA function
	printf("The A team is:\n");
	theATeam = nameLikeA(teams, &sizeA);

	for (i = 0; i < sizeA; i++)
	{
		printPlayer(*theATeam[i]);
	}

	for (i = 0; i < size; i++)
	{
		freeTeam(teams + i);
	}

	free(teams);
	free(theATeam);
}

//Function Definitions

//This function will get player info and team - team is so we can check the shirt number
void readPlayer(Player *player, Team *team)
{
	int error = TRUE; // will be used to print in case the entered date is not valid
	char buffer[BUFFER]; //temp string to get players name and get size
	do {
		if (!error) printf("Invalid ID!\n");
		printf("Please enter new player's ID number: ");
		scanf("%d", &player->id);
		getchar();
		error = checkID(player->id);
	} while (!error);

	printf("\nPlease enter player's name: ");
	gets(buffer);
	player->name = (char*)malloc((sizeof(char))*(strlen(buffer))+1); //Get exact size of players name

	//Check Allocation
	if (player->name == NULL) 
	{ 
		printf("Memory was not assigned\n");
		exit(0); 
	}
	
	strcpy(player->name, buffer); //copy name from buffer to struct

	do {
		if (!error) printf("\nShirt number already taken!\n");
		printf("\nPlease enter shirt number of player: ");
		scanf("%d", &player->shirtNum);
		getchar();
		error = checkShirt(player->shirtNum, team);
	} while (!error);

	do {
		if (!error) printf("Wrong Date entered!\n");
		printf("\nPlease enter the player's birthday date: dd/mm/yyyy :  ");
		scanf("%d/%d/%d", &player->birthday.day, &player->birthday.month, &player->birthday.year);
		getchar();
		error = checkDate(player->birthday);
	} while (!error);
	
	printf("\n");
}

//This function checks that the ID of the player is of 9 digits
int checkID(int playerID)
{
	if (playerID <= 99999999 || playerID >=1000000000) return FALSE;
	return TRUE;
}

//This function will check if the shirt number is already taken by another team member
int checkShirt(int shirtNum, Team *team)
{
	int i;
	for (i = 0; i < team->actualNum-1; i++)
	{
		if ((team->players + i)->shirtNum == shirtNum)
			return FALSE;
	}
	return TRUE;
}

//This function will initialize team and get team details
void initTeam(Team *team, Team *teams)
{
	int error = TRUE;
	char buffer[BUFFER];
	team->players = NULL;
	team->teamName = NULL;

	do
	{
		if(!error) printf("\nTeam name already in use!\n");
		printf("\nPlease enter Team name: ");
		gets(buffer);
		error = isUnique(buffer, teams);
	} while (!error);

	team->teamName = (char*)malloc((sizeof(char)*strlen(buffer)) + 1); //Get exact size of team name

		//Check Allocation
	if (team->teamName == NULL)
	{
		printf("Memory was not assigned\n");
		exit(0);
	}
	
	strcpy(team->teamName, buffer); //copy name from buffer to struct

	printf("\nPlease enter max number of team members: ");
	scanf("%d", &team->maxNum);
	getchar();
	printf("\n");
	team->actualNum = 0;

	//Randomize number of wins for the team
	srand((unsigned)time(NULL));

	team->wins = rand() % 21;
}

//This function will check that entered team name is unique
int isUnique(char *teamName, Team *teams)
{
	Team *temp = teams;
	while (temp->teamName != NULL && strcmp(teamName, temp->teamName) != FALSE)
	{
		temp++;
	}
	if (temp->teamName == NULL) return TRUE;
	return FALSE;
}

//This Function will add player to team and update team's size
void addPlayer(Team* team)
{
	if (team->actualNum == team->maxNum)
	{
		printf("Team is full!\n");
		return;
	}
	
	//if team is empty
	if (team->actualNum == 0)
	{
		team->players = (Player*)malloc(sizeof(Player));
		team->actualNum = 1;
	}
	else
	{
		team->actualNum++; // increase count of team members by 1 when adding player
		team->players = (Player*)realloc(team->players, sizeof(Player)*team->actualNum);
	}

	//Check allocation
	if (team->players == NULL) 
	{
		printf("Allocation failed\n");
		exit(0);
	}
	readPlayer(team->players + (team->actualNum - 1), team);
}

//This function will check which team has the highest number of wins
Team* maxSuccess(Team *teams, int size)
{
	Team *maxWins = NULL;
	if (size == 1) return teams;
	maxWins = maxSuccess(teams + 1, size - 1);
	if (maxWins->wins < teams->wins) return teams;
	return maxWins;
}

//This function will return an array of pointers to all team members that their name starts with an A
Player** nameLikeA(Team *team, int *size)
{
	int i, numA=0;
	Player **theATeam = NULL;

	for (i = 0; i < team->actualNum; i++)
	{
		if ((team->players+i)->name[0] == 'a' || (team->players+i)->name[0] == 'A')
		{
			numA++;
			if (theATeam == NULL) theATeam = (Player**)malloc(sizeof(Player*)); //if array is empty
			else theATeam = (Player**)realloc(theATeam, sizeof(Player*)*numA);
			
			//Check allocation
			if (theATeam == NULL)
			{
				printf("Allocation failed\n");
				exit(0);
			}
			theATeam[numA-1] = team->players + i;
		}
	}
	*size = numA; //update number of team members in new 'A' group
	return theATeam;
}

//This function will check if today is the player's birthday
int isBirthday(Date date)
{
	time_t now = time(0);  
	struct tm *ltm = localtime(&now);
	if (date.day == ltm->tm_mday)
	{ 
		if (date.month == ltm->tm_mon + 1)    
			return TRUE; 
	}  
	return FALSE;
}

//This function will print player's info and check if it's his birthday today
void printPlayer(Player player)
{
	printf("ID: %d\n", player.id);
	printf("Name: ");
	puts(player.name);
	printf("Shirt number: %d\n", player.shirtNum);
	printf("Player's birthday is: %d/%d/%d\n", player.birthday.day, player.birthday.month, player.birthday.year);
	if (isBirthday(player.birthday)) printf("Today is the players birthday, Happy Birthday!!\n");
}

//This function will print team info and each team member's details
void printTeam(Team team)
{
	int i;
	printf("\nTeam Details:\nTeam's name: ");
	puts(team.teamName);
	printf("Actual number of team members: %d\n", team.actualNum);
	printf("Number of wins: %d\n", team.wins);
	
	for (i = 0; i < team.actualNum; i++)
	{
		printf("\nPlayer #%d info:\n", i+1);
		printPlayer(team.players[i]);
	}
	printf("\n");
}

//This function checks that the date of the birthday is valid
int checkDate(Date date)
{
	time_t now = time(0);
	struct tm *ltm = localtime(&now);
	int isLeapYear;
	

	if (date.year > (ltm->tm_year + 1900) || (date.year == (ltm->tm_year + 1900) && date.month > ltm->tm_mon + 1 && date.day > ltm->tm_mday)) return FALSE; // Check if birthday is not in the future
	if (date.month > 12) return FALSE; // Check that the month is not larger than 12
	// Chcek the birthday day match the month and the year
	if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)
	{
		if (date.day > 30)
			return FALSE;
	}
	else if (date.month == 2)
	{
		isLeapYear = (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);
		if (isLeapYear)
		{
			if (date.day > 29)
				return FALSE;
		}
		else
		{
			if (date.day > 28)
				return FALSE;
		}
	}
	else
	{
		if (date.day > 31)
			return FALSE;
	}
	return TRUE;
}

//This function will free the memory allocated for the team
void freeTeam(Team *team)
{
	int i;
	//Free Players
	for (i = 0; i < team->actualNum; i++)
	{
		free((team->players+i)->name);
	}
	free(team->players);
	free(team->teamName);
}
