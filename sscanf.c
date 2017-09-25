#include <stdio.h>
#include <string.h>

typedef struct {
  int year;
  char sex;
  char name[20], lastname[20], turno[7], cedula[15];
} person;

int main() {
  person student;
  char line[100];
  int n;
  strcpy(line, "09 00 0720 02009 LIMA                           RANDALL                        M       diurno       2013");
	n = sscanf(line, "%[0-9 ] %12[^0-9] %8[^0-9] %c %s %d", student.cedula,\
                                                      student.lastname,\
                                                      student.name,\
                                                      &student.sex,\
                                                      student.turno,\
                                                      &student.year);
  if (n == 5){
    sscanf(student.turno, "%i", &student.year);
    strcpy(student.turno, "");
  }
  printf("N:%d, %s %s %s %c %s año:%d\n", n, student.cedula,\
                                  student.lastname,\
                                  student.name,\
                                  student.sex,\
                                  student.turno,\
                                  student.year);
	return 0;
}
