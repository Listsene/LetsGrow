#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define DATA_SIZE 6400
#define TEST_SIZE 3200

struct plants {
	int temp;
	int ph;
	int light;
	char soil[10];
	char plant[10];
};
plants createPlant() {
	plants p;
	int r = rand() % 5 + 1; //random number 1-5
	if (r == 1) {
		p.temp = rand() % 7 + 19; //18 - 24 C
		p.ph = rand() % 3 + 6; //5 - 7 pH
		p.light = rand() % 4 + 8; // 7 - 10 hrs
		strncpy(p.soil, "c-moist", 10);
		strncpy(p.plant, "tomato", 10);
	}
	else if (r == 2) {
		p.temp = rand() % 12 + 22; //21 - 32 C
		p.ph = 7;
		p.light = rand() % 3 + 5; // 4 - 6 hrs
		strncpy(p.soil, "c-moist", 10);
		strncpy(p.plant, "basil", 10);
	}
	else if (r == 3) {
		p.temp = rand() % 12 + 8; //7 - 18 C
		p.ph = rand() % 3 + 7; //6 - 8 pH
		p.light = rand() % 5 + 3; // 2 - 6 hrs
		strncpy(p.soil, "c-drain", 10);
		strncpy(p.plant, "lettuce", 10);
	}
	else if (r == 4) {
		p.temp = rand() % 7 + 19; //18 - 24 C
		p.ph = rand() % 3 + 6; //5 - 7 pH
		p.light = rand() % 3 + 5; // 4 - 6 hrs
		strncpy(p.soil, "c-drain", 10);
		strncpy(p.plant, "rosemary", 10);
	}
	else if (r == 5) {
		p.temp = rand() % 7 + 19; //18 - 24 C
		p.ph = rand() % 2 + 8; //7 - 8 pH
		p.light = rand() % 3 + 7; // 6 - 8 hrs
		strncpy(p.soil, "c-drain", 10);
		strncpy(p.plant, "lavender", 10);
	}
	return p;
}
int createData(plants p[], FILE *w) {
	for (int i = 0; i < DATA_SIZE; i++) {
		p[i] = createPlant();
		fprintf(w, "%d,%d,%d,%s,%s\n", p[i].temp, p[i].ph, p[i].light, p[i].soil, p[i].plant);
	}
	return 1;
}
int createTrain(plants p[], FILE *w2) {
	for (int i = 0; i < TEST_SIZE; i++) {
		p[i] = createPlant();
		fprintf(w2, "%d,%d,%d,%s,%s\n", p[i].temp, p[i].ph, p[i].light, p[i].soil, p[i].plant);
	}
	return 1;
}

int main()
{
	struct plants p[DATA_SIZE];
	struct plants p2[TEST_SIZE];
	srand(time(NULL));   // should only be called once

	FILE *w, *w2;
	w = fopen("plant.data", "w+");
	w2 = fopen("plant.test", "w+");
	if (createData(p, w) == 1) {
		printf("Data Creation Success\n");
	}
	if (createTrain(p2, w2) == 1) {
		printf("Test Creation Success\n");
	}

	fclose(w);

	return 0;
}