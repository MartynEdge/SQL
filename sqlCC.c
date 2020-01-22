#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlSupport.h"


int main(int argc, char* argw[]){
	
	//Setup the table
	struct SQL_TABLE *rTable;
	rTable=NEW_TABLE("NewTable",3);
	
	ADD_COLUMN(rTable, "Column1", SQL_TYPE_INT);
	ADD_COLUMN(rTable, "Column2", SQL_TYPE_TEXT);
	ADD_COLUMN(rTable, "Column3", SQL_TYPE_FLOAT);
	
	//Print table information
	printf("Column 1 name is '%s' and is type INT (%d)\n",rTable->ColumnNames[0],rTable->ColumnTypes[0]);
	printf("Column 2 name is '%s' and is type TEXT (%d)\n",rTable->ColumnNames[1],rTable->ColumnTypes[1]);
	printf("Column 3 name is '%s' and is type FLOAT (%d)\n",rTable->ColumnNames[2],rTable->ColumnTypes[2]);
	
	//Save the table
	struct SQL_FILE *rFile=SQL_OPEN("DelthisDB.db");
	SAVE_TABLE(rFile, rTable);
	
	//Setup a row
	struct SQL_ROW *rRow=NEW_ROW();
	int Value_int;
	char *Value_text=(char*)malloc(sizeof(char)*100);
	float Value_float;
	
	//Row 1
	Value_int=150;
	Value_float=1.234f;
	strcpy(Value_text,"Test cell 1\0");
	ADD_CELL(rTable, rRow, "Column1",&Value_int);
	ADD_CELL(rTable, rRow, "Column2",Value_text);
	ADD_CELL(rTable, rRow, "Column3",&Value_float);
	SAVE_ROW(rFile, rTable, rRow);
	
	//Row 2
	Value_int=250;
	strcpy(Value_text,"Test cell 2\0");
	Value_float=9.8765f;
	RESET_ROW(rRow);
	ADD_CELL(rTable, rRow, "Column1",&Value_int);
	ADD_CELL(rTable, rRow, "Column2",Value_text);
	ADD_CELL(rTable, rRow, "Column3",&Value_float);
	SAVE_ROW(rFile, rTable, rRow);

	//Finalise
	SQL_CLOSE(rFile);
	free(rTable);
	free(rRow);
	
	return EXIT_SUCCESS;
}
