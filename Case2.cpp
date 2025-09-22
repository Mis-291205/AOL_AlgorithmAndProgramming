#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_COLUMNS 8
#define MAX_CHARACTERS 201
#define MAX_ROWS 4000

// Define struct for each row of data
typedef struct {
    char columns[MAX_COLUMNS][MAX_CHARACTERS];
} DataRow;

DataRow dataArray[MAX_ROWS];

char typeColumns[][101] = {"Location", "City", "Price", "Rooms", "Bathrooms", "Carpark", "Type", "Furnish"};
int indexToSort;
bool dataSorted = false;

// Function to print the category names
void printCategory() {
    char *token;
    char line[201];
    FILE *fp = fopen("fileaol.csv", "r");
    fscanf(fp, "%[^\n]\n", line);
    token = strtok(line, ",");
    while (token != NULL) {
        printf("%s", token);
        int len = strlen(token);

        for (int i = 0; i < 18-len; i++) { //total huruf terbanyak 17, maka dilebihkan dikit jadi 18
            printf(" ");
        }
        token = strtok(NULL, ",");
    }
    printf("\n");
    fclose(fp);
}

// Function to find column name
int findColumn(char column[]) {
    for (int i = 0; i < MAX_COLUMNS; i++) {
        if (strcmp(typeColumns[i], column) == 0)
            return i;
    }
    return -1;
}

// Function to input data into the array of structs
int inputToArray() {
    char *token;
    char line[201];
    FILE *fp = fopen("fileaol.csv", "r");
    int i = -1;
    
    while (fgets(line, sizeof(line), fp) != NULL) {
    	int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        
        int j = 0;
        token = strtok(line, ",");
        while (token != NULL && i >= 0) {
            strcpy(dataArray[i].columns[j], token);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    
    fclose(fp);
    return i;
}

// Function for option 1: Display data
void displayData(int rows) {
    char *token;
    char line[201];
    int total_data = inputToArray();

    FILE *fp = fopen("fileaol.csv", "r");
    
    rows = (rows < total_data) ? rows : total_data;
    for (int i = 0; i <= rows; i++) {
        fscanf(fp, "%[^\n]\n", line);
        token = strtok(line, ",");
        while (token != NULL) {
            printf("%s", token);
            
            int len = strlen(token);
            for (int j = 0; j < 18 - len; j++) {
                printf(" ");
            }
            token = strtok(NULL, ",");
        }
        printf("\n");
    }
    fclose(fp);
}

// Function for option 2: Search data
void searchData(char column[], char nameColumn[]) {
    int counter = 1;
    char *token, line[201], temp[201];
    bool x = false;
    int index = findColumn(column);
    FILE *fp = fopen("fileaol.csv", "r");
    while (!feof(fp)) {
        fscanf(fp, "%[^\n]\n", line);
        strcpy(temp, line);
        token = strtok(line, ",");
        int columnIndex = 0;
        while (token != NULL) {
            if (strcmp(token, nameColumn) == 0 && columnIndex == index) {
                x = true;
                if (counter == 1) {
                    printf("Data found. Detail of data:\n");
                    printCategory();
                    counter++;
                }
            }
            token = strtok(NULL, ",");
            columnIndex++;
        }

        if (x) {
            token = strtok(temp, ",");
            while (token != NULL) {
                printf("%s", token);
                printf(" ");
                int len = strlen(token);
                for (int i = 0; i < 17 - len; i++) {
                    printf(" ");
                }
                token = strtok(NULL, ",");
            }
            printf("\n");
        }
        x = false;
    }

    if (counter == 1) {
        printf("Data not found!\n");
    }

    fclose(fp);
}

// Functions for option 3: Sort data
// find data is digit or string, then compare
int compareValues(const char *value1, const char *value2) {
    if (isdigit(value1[0]) && isdigit(value2[0])) {
        int num1 = atoi(value1);
        int num2 = atoi(value2);
        return num1 - num2;
    } else {
        return strcmp(value1, value2);
    }
}

// merge
void merge(int arr[], int left[], int leftSize, int right[], int rightSize, char sort[]) {
    int i = 0, j = 0, k = 0;
	
	if (strcmp(sort, "asc") == 0) {
		while (i < leftSize && j < rightSize) {
        	if (compareValues(dataArray[left[i]].columns[indexToSort], dataArray[right[j]].columns[indexToSort]) <= 0) {
            	arr[k] = left[i];
            	i++;
        	} else {
            	arr[k] = right[j];
            	j++;
        	}
        	k++;
    	}
	}
	else if (strcmp(sort, "desc") == 0) {
		while (i < leftSize && j < rightSize) {
        	if (compareValues(dataArray[left[i]].columns[indexToSort], dataArray[right[j]].columns[indexToSort]) >= 0) {
            	arr[k] = left[i];
            	i++;
        	} else {
            	arr[k] = right[j];
            	j++;
        	}
        	k++;
    	}
	}
    
    while (i < leftSize) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < rightSize) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

// merge sort
void mergeSort(int arr[], int size, char sort[]) {
    if (size <= 1) {
        return;
    }

    int mid = size / 2;
    int left[mid];
    int right[size - mid];

    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }

    for (int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }

    mergeSort(left, mid, sort);
    mergeSort(right, size - mid, sort);
    merge(arr, left, mid, right, size - mid, sort);
}

// sort the data
int *sortData(char column[], char sort[], int *numRowsToDisplay) {
    indexToSort = findColumn(column);
    int totalData = inputToArray();

    int *indexxss = (int *)malloc(totalData * sizeof(int));
    for (int i = 0; i < totalData; i++) {
        indexxss[i] = i;
    }
	
	mergeSort(indexxss, totalData, sort);
	
//    if (strcmp(sort, "asc") == 0) {
//        mergeSort(indexxss, totalData);
//    } else if (strcmp(sort, "dsc") == 0) {
//        mergeSort(indexxss, totalData);
//        for (int i = 0; i < totalData / 2; i++) {
//            int temp = indexxss[i];
//            indexxss[i] = indexxss[totalData - i - 1];
//            indexxss[totalData - i - 1] = temp;
//        }
//    }

    *numRowsToDisplay = (totalData < 10) ? totalData : 10;

    return indexxss;
}

// Functions for option 4: Export data to a file
// export original file
void exportFile(char name[], int total_data) {
    strcat(name, ".csv");
    FILE *fp2 = fopen(name, "a");
	
	for (int i = 0; i < MAX_COLUMNS; i++) {
        fprintf(fp2, "%s", typeColumns[i]);
        if (i < MAX_COLUMNS - 1) {
            fprintf(fp2, ",");
        }
    }
    fprintf(fp2, "\n");
    
    for (int i = 0; i < total_data; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            fprintf(fp2, "%s", dataArray[i].columns[j]);
            if (j < MAX_COLUMNS - 1) {
                fprintf(fp2, ",");
            }
        }
        fprintf(fp2, "\n");
    }

    fclose(fp2);
}

// export sorted file by user
void exportSortedFile(char name[], int *sortedIndexxss, int total_data) {
	strcat(name, ".csv");
    FILE *fp2 = fopen(name, "a");
	
	for (int i = 0; i < MAX_COLUMNS; i++) {
        fprintf(fp2, "%s", typeColumns[i]);
        if (i < MAX_COLUMNS - 1) {
            fprintf(fp2, ",");
        }
    }
    fprintf(fp2, "\n");
    
    for (int i = 0; i < total_data; i++) {
        int rowIndex = sortedIndexxss[i];
        for (int j = 0; j < MAX_COLUMNS; j++) {
            fprintf(fp2, "%s", dataArray[rowIndex].columns[j]);
            if (j < MAX_COLUMNS - 1) {
                fprintf(fp2, ",");
            }
        }
        fprintf(fp2, "\n");
    }

    fclose(fp2);
}

int main() {
    int option = 0;
    int numRowsToDisplay;
    int *sortedIndexxss = NULL;
    int total_data = inputToArray();
    
    do {
        system("cls");
        printf("Program by: Michael Ivan Santoso - 2702300120 - LA95\n");
        printf("What do you want to do?\n");
        printf("1. Display data\n");
        printf("2. Search Data\n");
        printf("3. Sort Data\n");
        printf("4. Export Data\n");
        printf("5. Exit\n");

        printf("Your choice: ", option);
        scanf("%d", &option); getchar();

        switch (option) {
        	case 1: {
            	int rows;
            	printf("Number of row: ");
            	scanf("%d", &rows);
            	printf("\n");
            	displayData(rows);
            	system("pause");
            	break;
        	}

        	case 2: {
            	char column[101], nameColumn[101];
            	printf("Choose column: ");
            	scanf("%s", column);
            	printf("What data do you want? ");
            	scanf("%s", nameColumn);
            	searchData(column, nameColumn);
            	system("pause");
            	break;
        	}

        	case 3: {
            	char column[101], sort[11];
            	bool a = false;
            	printf("Choose column: ");
            	scanf("%s", column);
            	printf("Sort ascending (asc) or descending (desc)? ");
            	scanf("%s", sort);

            	for (int i = 0; i < MAX_COLUMNS; i++) {
                	if (strcmp(column, typeColumns[i]) == 0) {
                    	printf("Data found. Detail of data:\n");
                    	printCategory();
                    	a = true;
                    	break;
                	}
            	}

            	if (!a) {
                	printf("Data not found!\n");
                	system("pause");
                	continue;
            	}

            	sortedIndexxss = sortData(column, sort, &numRowsToDisplay);
            	dataSorted = true;

            	for (int i = 0; i < numRowsToDisplay; i++) {
                	int rowIndex = sortedIndexxss[i];
                	for (int j = 0; j < MAX_COLUMNS; j++) {
                    	printf("%s", dataArray[rowIndex].columns[j]);
                    	int len = strlen(dataArray[rowIndex].columns[j]);

                    	for (int k = 0; k < 18 - len; k++) {
                        	printf(" ");
                    	}
                	}
                printf("\n");
            	}

            	system("pause");
            	break;
        	}

        	case 4: {
            	char name[101];
            	printf("File name: ");
            	scanf("%[^\n]", name);
            	
            	if (dataSorted) {
                	exportSortedFile(name, sortedIndexxss, total_data);
            	} else {
                	exportFile(name, total_data);
            	}
            	
            	printf("Data successfully written to file %s!\n", name);
            	system("pause");
            	break;
        	}
        }
    } while (option != 5);

    return 0;
}
