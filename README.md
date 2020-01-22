# SQL
Simplifies writing a SQL database.

Include sqlSupport.h in your main file. See sqlCC.c for an example. It allows the coder to create a SQL table and write rows of
data using only a few lines of code.

To create a table:

...

//Declare the table variable.
struct SQL_TABLE *rTable;
rTable=NEW_TABLE("NewTable",3);

//Table setup.
ADD_COLUMN(rTable, "Column1", SQL_TYPE_INT);
ADD_COLUMN(rTable, "Column2", SQL_TYPE_TEXT);
ADD_COLUMN(rTable, "Column3", SQL_TYPE_FLOAT);
	
//Save the table
struct SQL_FILE *rFile=SQL_OPEN("DelthisDB.db");
SAVE_TABLE(rFile, rTable);

...

To write a row of data: 

...

//Declare the row variables.
struct SQL_ROW *rRow=NEW_ROW();
int Value_int;
char *Value_text=(char*)malloc(sizeof(char)*100);
float Value_float;
	
//Assign the row values.
Value_int=150;
Value_float=1.234f;
strcpy(Value_text,"Test cell 1\0");
ADD_CELL(rTable, rRow, "Column1",&Value_int);
ADD_CELL(rTable, rRow, "Column2",Value_text);
ADD_CELL(rTable, rRow, "Column3",&Value_float);
SAVE_ROW(rFile, rTable, rRow);

...

Here is a sample console output:

New table name: NewTable
Number of columns: 3
Column 0 address: 09bf33e8
Using column index: 0
Column 0 address: 09bf33e8
Column 1 address: 09bf33f0
Using column index: 1
Column 0 address: 09bf33e8
Column 1 address: 09bf33f0
Column 2 address: 09bf33f8
Using column index: 2
Column 1 name is 'Column1' and is type INT (1)
Column 2 name is 'Column2' and is type TEXT (2)
Column 3 name is 'Column3' and is type FLOAT (3)
SQL: CREATE TABLE NewTable (Column1 INT, Column2 TEXT, Column3 FLOAT) 
The value of column 0 is 150
The value of column 1 is Test cell 1
The value of column 2 is 1.234000
SQL: INSERT INTO NewTable (Column1 , Column2 , Column3 ) VALUES (150 , "Test cell 1" , 1.234000 )
The value of column 0 is 250
The value of column 1 is Test cell 2
The value of column 2 is 9.876500
SQL: INSERT INTO NewTable (Column1 , Column2 , Column3 ) VALUES (250 , "Test cell 2" , 9.876500 )
