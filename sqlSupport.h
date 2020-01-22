#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/////////////////
// Definitions //
/////////////////

#ifndef FIELD_LENGTH
#define FIELD_LENGTH 20
#endif

#ifndef MAX_COLUMNS
#define MAX_COLUMNS 20
#endif

#ifndef SQL_TYPE_INT
#define SQL_TYPE_INT 1
#endif

#ifndef SQL_TYPE_TEXT
#define SQL_TYPE_TEXT 2
#endif

#ifndef SQL_TYPE_FLOAT
#define SQL_TYPE_FLOAT 3
#endif

//////////////////
// Declarations //
//////////////////

struct SQL_FILE{
	char *Filename;
	sqlite3 *_Database;
};

struct SQL_TABLE{
	char Name[FIELD_LENGTH];
	char *ColumnNames[MAX_COLUMNS];
	int ColumnTypes[MAX_COLUMNS];
	int ColumnsCount;
};


struct SQL_ROW{
	int Value_INT[MAX_COLUMNS];
	float Value_FLOAT[MAX_COLUMNS];
	char *Value_TEXT[MAX_COLUMNS];
};


///////////////////////////
// Function declarations //
///////////////////////////

struct SQL_TABLE* NEW_TABLE(char* Name, int ColumnsCount);
int ADD_COLUMN(struct SQL_TABLE *rTable, char *Name, int SQL_TYPE);
struct SQL_FILE* SQL_OPEN(char *FileFE);
void SQL_CLOSE();
int callback(void *parameter, int argc, char **argv, char **columnName);
int SAVE_TABLE(struct SQL_FILE *rSQL, struct SQL_TABLE *rTable);



///////////////
// Functions //
///////////////

struct SQL_FILE* SQL_OPEN(char *FileFE){
	//Opens a new SQL database.
	struct SQL_FILE  *rSQL_FILE = (struct SQL_FILE*)malloc(sizeof(struct SQL_FILE));
	rSQL_FILE->Filename=FileFE;
	rSQL_FILE->_Database=NULL;
	int iResult;
	iResult=sqlite3_open(rSQL_FILE->Filename, &rSQL_FILE->_Database);
	
	return rSQL_FILE;
}

void SQL_CLOSE(struct SQL_FILE *rSQL){
	sqlite3_close(rSQL->_Database);
}


struct SQL_TABLE* NEW_TABLE(char* Name, int ColumnsCount){
	//Creates a new table, allocates memory,
	//and returns the pointer.
	//Example usage:
	// struct SQL_TABLE *example=NEW_TABLE("Example Table",3);
	
	struct SQL_TABLE *rTable;
	rTable=(struct SQL_TABLE*)malloc(sizeof(struct SQL_TABLE));
	memset(rTable, 0, sizeof(struct SQL_TABLE));
	
	strcpy(rTable->Name, Name);
	rTable->ColumnsCount=ColumnsCount;
	printf("New table name: %s\n",rTable->Name);
	printf("Number of columns: %d\n",rTable->ColumnsCount);
	
	return rTable;
}

int ADD_COLUMN(struct SQL_TABLE *rTable, char *Name, int SQL_TYPE){
	//Inserts the column parameters into
	//the next blank columns in the arrays Names and Types.
	//Example usage: ADD_COLUMN(rTable,"Finances",SQL_
	
	//Find the next blank column.
	int i;
	int iIndex=-1;

	char *cpColumnName=(char*)malloc(strlen(Name)+1);
	strcpy(cpColumnName,Name);
	
	for (i=0; i<rTable->ColumnsCount;i++){
		printf("Column %d address: %08x\n",i,&rTable->ColumnNames[i]);
		if (rTable->ColumnNames[i]==0){
			iIndex=i;
			break;
		}
	}
	if (iIndex==-1){ 
		printf("Failed to find next unused column.\n");
		return EXIT_FAILURE;
	}
	
	//store the name and type
	
	printf("Using column index: %d\n",iIndex);
	rTable->ColumnNames[iIndex]=cpColumnName;
	rTable->ColumnTypes[iIndex]=SQL_TYPE;
	
	return EXIT_SUCCESS;
}

int SAVE_TABLE(struct SQL_FILE *rSQL, struct SQL_TABLE *rTable){
	char *pcSQL=(char*)malloc(sizeof(char)*1000);
	memset(pcSQL,0,1000);
	
	strcpy(pcSQL,"CREATE TABLE \0");
	strcat(pcSQL, rTable->Name);
	strcat(pcSQL," (\0");
	
	int i;
	for (i=0; i<rTable->ColumnsCount;i++){
		strcat(pcSQL, rTable->ColumnNames[i]);
		strcat(pcSQL, " ");
		switch (rTable->ColumnTypes[i]){
			case SQL_TYPE_INT:
				strcat(pcSQL, "INT\0");
				break;
			case SQL_TYPE_TEXT:
				strcat(pcSQL, "TEXT\0");
				break;
			case SQL_TYPE_FLOAT:
				strcat(pcSQL, "FLOAT\0");
				break;
		}
		if (i<rTable->ColumnsCount-1) strcat(pcSQL, ", \0");
	}
	
	strcat(pcSQL,") \0");
	
	printf("SQL: %s\n",pcSQL);
	
	char *pcErrors;
	int iReturnVal;
	iReturnVal=sqlite3_exec(rSQL->_Database, pcSQL, callback, 0, &pcErrors);
	
	free(pcSQL);
	return 0;
}

struct SQL_ROW* NEW_ROW(){
	//Creates a new row by allocating
	//memory to a structure containing
	//an array of cells.
	//Example usage: struct SQL_ROW *example_row=NEW_ROW();
	
	struct SQL_ROW *rRow=(struct SQL_ROW*)malloc(sizeof(struct SQL_ROW));
	memset(rRow, 0, sizeof(struct SQL_ROW));
	return rRow;
}

void RESET_ROW(struct SQL_ROW *rRow){
	memset(rRow, 0, sizeof(struct SQL_ROW));
}
	
int ADD_CELL(struct SQL_TABLE *rTable, struct SQL_ROW *rRow, char *ColumnName, void *Value){
	//Searches the column names for a matching
	//ColumnName, stores the column index,
	//then retrieves the column type and stores
	//the Value.
	
	//Find the matching column name.
	int i;
	int iIndex=-1;
	for (i=0;i<rTable->ColumnsCount;i++){
		if (strcmp(rTable->ColumnNames[i],ColumnName)==0){
			//Found the matching column name.
			iIndex=i;
			break;
		}
	}
	if (iIndex==-1){
		printf("Unable to find matching column name.");
		return EXIT_FAILURE;
	}
	
	//Retrieve the column type
	int iColType=rTable->ColumnTypes[iIndex];
	
	
	//Store the value into the row.
	switch (iColType){
		case SQL_TYPE_INT:
			rRow->Value_INT[iIndex]=*(int*)Value;
			printf("The value of column %d is %d\n",iIndex,rRow->Value_INT[iIndex]);
			break;
		case SQL_TYPE_TEXT:
			rRow->Value_TEXT[iIndex]=(char*)Value;
			printf("The value of column %d is %s\n",iIndex,rRow->Value_TEXT[iIndex]);
			break;
		case SQL_TYPE_FLOAT:
			rRow->Value_FLOAT[iIndex]=*(float*)Value;
			printf("The value of column %d is %f\n",iIndex,rRow->Value_FLOAT[iIndex]);
			break;
		
	}
	
	return EXIT_SUCCESS;
}

int SAVE_ROW(struct SQL_FILE *rSQL, struct SQL_TABLE *rTable, struct SQL_ROW *rRow){
	
	char *pcSQL=(char*)malloc(sizeof(char)*1000);
	memset(pcSQL,0,1000);
	
	strcpy(pcSQL,"INSERT INTO \0");
	strcat(pcSQL, rTable->Name);
	strcat(pcSQL, " (\0");
	
	//Contatenate columns
	int i;
	for (i=0; i<rTable->ColumnsCount;i++){
		strcat(pcSQL, rTable->ColumnNames[i]);
		strcat(pcSQL, " \0");
		
		if (i<rTable->ColumnsCount-1) strcat(pcSQL, ", \0");
	}
	
	//Contatenate values
	strcat(pcSQL,") \0");
	strcat(pcSQL,"VALUES (\0");
	
	int Value_INT=0;
	char *Value_TEXT=(char*)malloc(sizeof(char)*10);
	float Value_FLOAT;
	for (i=0; i<rTable->ColumnsCount;i++){
		memset(Value_TEXT,0,10);
		switch (rTable->ColumnTypes[i]){
			case SQL_TYPE_INT:
				Value_INT=rRow->Value_INT[i];
				//itoa(Value_INT,Value_TEXT,10);
				sprintf(Value_TEXT, "%d",Value_INT);
				strcat(pcSQL,Value_TEXT);
				break;
			case SQL_TYPE_FLOAT:
				Value_FLOAT=rRow->Value_FLOAT[i];
				sprintf(Value_TEXT, "%f",Value_FLOAT);
				strcat(pcSQL,Value_TEXT);
				break;
			case SQL_TYPE_TEXT:
				strcat(pcSQL,"\"");
				strcat(pcSQL,rRow->Value_TEXT[i]);
				
				strcat(pcSQL,"\"");
				break;
		}
		strcat(pcSQL, " \0");
		if (i<rTable->ColumnsCount-1) strcat(pcSQL, ", \0");
	}
	
	strcat(pcSQL, ")\0");
	printf("SQL: %s\n",pcSQL);
	
	char *pcErrors;
	int iReturnVal;
	iReturnVal=sqlite3_exec(rSQL->_Database, pcSQL, callback, 0, &pcErrors);
	free(pcSQL);
	return 0;
}

int callback(void *parameter, int argc, char **argv, char **columnName){
	for (int i = 0; i < argc; i++)
		printf("%s : %s\n", columnName[i], argv[i]);
	return 0;
}
