#include "Database.hpp"

Date::Date(char *date){ //metatrepw ta Date se Datesize opou to datesize einai oi meres pou exoun perasei apo to 00/00/0000 an den exei apla to vazw kati terastio wste na 
	char temp[20];
	strcpy(temp,date); //ikanopoiei tis diafores sun8ikes pou xreizontai meta3u entry date kai exit date
	char *token = strtok(temp,"-");
	day = atoi(token);
	token = strtok(NULL,"-");
	month = atoi(token);
	token = strtok(NULL,"-");
	year = atoi(token);
	date_size = (year*365) + (month*30) + day;
}