//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : timeinfo.h
//  Author  : Stefan Steiner
//  Copyright (c) 2014 Stefan Steiner
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef _timeinfo_h
#define _timeinfo_h

//gibt minuten der Uhrzeit zurueck !!nicht absolut
int getMinutes(int seconds);

//gibt Stunden zurueck
int getHours(int seconds);

//gibt sekunden der Uhrzeit zurueck !!nicht absolut
int getSeconds(int seconds);

//schreibt timerdaten in file
int writeTime2File(int timerArrC, int *timerArrV, char *dataFolderName); 

//schreibt Zeitinformation auf Bildschirm
int plotTime( int *timerArrV, int wert);
#endif 
//-------------------------------------------------------------------------------------------------------------------------------





 

