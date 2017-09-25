#define ARCHIVO "datos.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
  #include <locale.h>
#endif
// http://stackoverflow.com/questions/1627624/search-for-string-in-text-file-c
// http://pubs.opengroup.org/onlinepubs/007908799/xsh/strstr.html
// void make_directory(const char* name)
//    {
//    #ifdef __linux__
//        mkdir(name, 777);
//    #else
//        _mkdir(name);
//    #endif
//    }

typedef struct {
  int year;
  char sex;
  char name[26], lastname[26], turno[15], cedula[15];
} person;
// 02 00 0719 02461 BUITRAGO                       EUFEMIA                        F    Diurno    2013
// %s %s %s %s %s %s %c %s %d
int struct_cmp(const void *a, const void *b);
int struct_cmp(const void *a, const void *b){ 
    person *ia = (person *)a;
    person *ib = (person *)b;
    return strcmp(ia->cedula, ib->cedula);
	/* strcmp functions works exactly as expected from
	comparison function */ 
} 

int main() {
  void students(person student[1000], FILE * data, int d_career[4]);
  void directory(char name[10]);
  void savefile(person student[1000], int size, char fileToSave[20]);
  int readData(char cedula[20]);
  int searchData(int size, char cedInt[20], char file[20], person student2[200]);
  void showData(int index, person student[1000]);

  char ING_FILE[40] = "DATA_Ingenieria";
  char LIC_FILE[40] = "DATA_Licenciatura";
  char TEC_FILE[40] = "DATA_Tecnico";

  directory(ING_FILE);
  directory(LIC_FILE);
  directory(TEC_FILE);
  // exit(1);

  strcat(ING_FILE, "/ING_students.txt");
  strcat(LIC_FILE, "/LIC_students.txt");
  strcat(TEC_FILE, "/TEC_students.txt");


  // printf("\n\tFIEC - ARCHIVO DE DATOS DE ESTUDIANTES\n");

  FILE *data;
  person student[1000];
  char career[11][9], career_data[3];
  char line[13];
  int i, d_career[4];
  memset(d_career, 0, 4*sizeof(int));
  data = fopen (ARCHIVO,"r");
  if (data!=NULL) {
    // printf("\nLeyendo archivo de datos...\n");
    i = 0;
    while (feof(data) == 0) {
      if (fgets(line, 13, data) != NULL) {
        if ((sscanf(line, " Sede: %s", career_data) == 1)) {
          strcpy(career[i], career_data);
        } else if (sscanf(line, " Facultad: %s", career_data) == 1) {
          strcat(career[i], career_data);
/*
04 24 02 01 → Ingeniería
04 24 02 02 → Técnico
04 24 02 05 → Licenciatura
04 24 03 01 → Técnico
04 24 03 02 → Licenciatura
*/
        } else if (sscanf(line, " Escuela: %s", career_data) == 1) {
          strcat(career[i], career_data);
        } else if (sscanf(line, " Carrera: %s", career_data) == 1) {
          strcat(career[i], career_data);
          // printf("%s → ", career[i]);
          // printf("%c\n", career[i][7]);
          if ((career[i][5] == '2') && (career[i][7] == '1')) {
            // printf("Ingeniería\n");
            d_career[3] = 1;
            d_career[0] = 0;
            students(student, data, d_career);
            savefile(student, d_career[0], ING_FILE);
          } else if (((career[i][5] == '2') && (career[i][7] == '5')) ||\
                     ((career[i][5] == '3') && (career[i][7] == '1'))){
            // printf("Técnico\n");
            d_career[3] = 2;
            d_career[1] = 0;
            students(student, data, d_career);
            savefile(student, d_career[1], TEC_FILE);
          } else if (((career[i][5] == '2') && (career[i][7] == '2')) ||\
                     ((career[i][5] == '3') && (career[i][7] == '2'))){
            // printf("Licenciatura\n");
            d_career[3] = 3;
            d_career[2] = 0;
            students(student, data, d_career);
            savefile(student, d_career[2], LIC_FILE);
          } else {
            printf("Código inválido\n");
          }
          // printf("ing=%d tec=%d lic=%d\n", d_career[0], d_career[1], d_career[2]);
          // printf("TOTAL=%d\n", d_career[0] + d_career[1] + d_career[2]);
          i++;
        }
      }
    }
    // printf("\n%s\n", career[0]);
    fclose (data);
    // printf("¡Se han guardado los datos correctamente!\n");
    char cedula[20];
    int index=0, carrera;
    char res[2];
    person student2[200];
    do {
      #ifdef WIN32
        system("cls");
      #elif __linux__
        system("clear");
      #endif
      printf("\n\tFIEC - ARCHIVO DE DATOS DE ESTUDIANTES\n");
      printf("\n\tINSTRUCCIONES:\n\n");
      printf("\tEJEMPLO DE CEDULA: 8 00 0843 00341\n");
      printf("\tFORMATO DE CEDULA: A BB CCCC DDDDD\n");
      printf("\tA = Provincia, para extranjeros es 0\n");
      printf("\tBB = Código de pais, para nacionales es 00\n");
      printf("\tCCCC = Tomo\n");
      printf("\tDDDDD = Asiento\n");
      printf("\n\tCODIGO DE CARRERA: Ingeniería (1), Licenciatura (5), Técnico (2)\n\n");
      fflush(stdin);
      carrera = readData(cedula);
      // printf("%s, %d\n", cedula, carrera);
      if (carrera == 1) {
        index = searchData(35, cedula, ING_FILE, student2);
      } else if (carrera == 2){
        index = searchData(93, cedula, TEC_FILE, student2);
      } else if (carrera == 5){
        index = searchData(154, cedula, LIC_FILE, student2);
      }
      if (index != -1) {
        // printf("Encontrado----------\n");
        showData(index, student2);
      } else {
        printf("\nEl estudiante no ha sido encontrado\n");
      }
      // struct _IO_FILE * STDIN;
      int resError=1;
      while (resError) {
        printf("\n¿Desea buscar otro estudiante?, s/n: ");
        // fgets(res, 2, STDIN);
        scanf("%s", res);
        if(sscanf(res, "%[sn]", &res[0]) == 1) {
          resError = 0;
        } else {
          printf("Escriba s(si) o n(no)");
        }
      }
    } while (res[0] == 's');
    printf("\n¡Gracias por usar este programa!\n\n");
  } else {
    printf("\nERROR AL LEER EL ARCHIVO DE DATOS\n\n");
  }
  // students();
  return 0;
}

void directory(char name[10]) {
  #ifdef WIN32
    #include <dir.h>
    if(_mkdir(name) == -1) {
      _rmdir(name);
      _mkdir(name);
      system("cls");
    }
  #elif __linux__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    if (mkdir(name, 0700) == -1) {
      system("rm -rf DATA_* && clear");
      mkdir(name, 0700);
    }
  #endif
  // exit(0);
}


void savefile(person student[1000], int size, char fileToSave[20]){
  FILE *data;
  // printf("-----------------------------------%s\n", fileToSave);
  data = fopen(fileToSave,"a+");
  int i;
  if (data != NULL) {
    for(i=0; i<size; i++) {
      fprintf(data, "0%s %s %s %c %-10s %d\n", student[i].cedula,\
                                              student[i].lastname,\
                                              student[i].name,\
                                              student[i].sex,\
                                              student[i].turno,\
                                              student[i].year);
    }
    fclose (data);
  }
}


void students(person student[1000], FILE * data, int d_career[4]) {
  // person student[1000];
  // FILE * data;
  char line[120];
  int i, n, NULO, start;
  // data = fopen (ARCHIVO,"r");
  // if (data!=NULL)
    i = 0;
    n = 0;
    NULO = 0;
    start = 0;
    // while ((feof(data) == 0) && (NULO < 2)) {
    while (NULO < 2) {
      if (fgets(line, 120, data) != NULL) {
        n = sscanf(line, "0%15[0-9PE ] %25[^0-9] %25[^0-9] %c %s %d", student[i].cedula,\
                                                                  student[i].lastname,\
                                                                  student[i].name,\
                                                                  &student[i].sex,\
                                                                  student[i].turno,\
                                                                  &student[i].year);
        // printf("%d\n", n);
        if (n == 5){
          sscanf(student[i].turno, "%i", &student[i].year);
          strcpy(student[i].turno, "");
          n++;
        }
        if (n == 6) {
          // printf("0%s %s %s %c %-10s %d\n",\
          //                                   student[i].cedula,\
          //                                   student[i].lastname,\
          //                                   student[i].name,\
          //                                   student[i].sex,\
          //                                   student[i].turno,\
          //                                   student[i].year);
          // printf("%i. %s\n", i+1, student[i].name);
          start = 1;
          NULO = 0;
          i++;
          switch (d_career[3]) {
            case 1:
              d_career[0] += 1;
              break;
            case 2:
              d_career[1] += 1;
              break;
            case 3:
              d_career[2] += 1;
              break;
          }
        } else if (start) {
          NULO++;
        }
      }
    }
    // fclose (data);
  // }
  // return i;
}

typedef struct {
  int prov[2], tomo[4], asiento[5];
  char pais[2];
} Cedula;

int readData(char cedula[20]) {
  char line[2];
  int carrera;
  Cedula ID;
  int isError=1, n;
  long int cedInt;
  while (isError) {
    printf("Escriba un número de cédula, A BB CCCC DDDDD: ");
    // scanf("%*[^\n]%*c");
    // fgets(cedula, 20, stdin);
    scanf(" %[^\n]", cedula);
    // printf("%s", cedula);
    // while (getchar() != '\n' && !feof(stdin));
    if (cedula != NULL) {
      n = sscanf(cedula, "%1d %1[0PE]%1[0PE] %1d%1d%1d%1d %1d%1d%1d%1d%1d\n",\
                                                      &ID.prov[0],\
                                                       &ID.pais[0], &ID.pais[1],\
                                                       &ID.tomo[0], &ID.tomo[1],\
                                                       &ID.tomo[2], &ID.tomo[3],\
                                                       &ID.asiento[0], &ID.asiento[1],\
                                                       &ID.asiento[2], &ID.asiento[3],\
                                                       &ID.asiento[4]);
      // n = sscanf(cedula, "%1d%1d\n", &ID.prov[0], &ID.prov[1]);
      // printf("%d%d\n", ID.prov[0], ID.prov[1]);
      // printf("%d\n", n);
      // 02-00-0732-00727
      if (n == 12){
        while (isError) {
          sprintf(cedula, "%d %c%c %d%d%d%d% d%d%d%d%d", ID.prov[0],\
                                                  ID.pais[0], ID.pais[1],\
                                                  ID.tomo[0], ID.tomo[1],\
                                                  ID.tomo[2], ID.tomo[3],\
                                                  ID.asiento[0], ID.asiento[1],\
                                                  ID.asiento[2], ID.asiento[3],\
                                                  ID.asiento[4]);
          // sscanf(cedula, "%s", &cedInt);
          // printf("Leyendo stdin %s\n", cedula);
          printf("Escriba un número de carrera, 1-2-5: ");
          // fgets(line, 2, stdin);
          scanf("%s", line);
          if (sscanf(line, " %d", &carrera) == 1) {
            if (carrera == 1 || carrera == 2 || carrera == 5) {
              isError = 0;
            }
          } else {
            printf("\nCodigo de carrera incorrecto\n");
          }
        }
      } else {
        printf("Cedula escrita incorrectamente\n");
      }
    }
  }
  return carrera;
  // exit(0);
}

// REALIZAR LA UNION DE LAS FUNCIONES


int searchData(int size, char cedInt[20], char file[50], person student2[200]) {
  int i, n, isOkay=0;
  int index=-1, compare, j=0;
  void showData(int index, person student[200]);
  char line[120];
  // person student[1000];
  FILE *data;
  // printf("%s\n", file);
  data = fopen(file,"r");
  if (data != NULL) {
    for(i=0; i<size; i++) {
      if (fgets(line, 120, data) != NULL) {
        n = sscanf(line, "0%15[0-9PE ] %25[^0-9] %25[^0-9] %c %s %d", student2[i].cedula,\
                                                                  student2[i].lastname,\
                                                                  student2[i].name,\
                                                                  &student2[i].sex,\
                                                                  student2[i].turno,\
                                                                  &student2[i].year);
        // printf("%d\n", n);
        if (n == 5){
          sscanf(student2[i].turno, "%i", &student2[i].year);
          strcpy(student2[i].turno, "");
          n++;
        }
        if (n == 6) {
          // showData(i, student2);
          isOkay++;
        }
      }
    }
    fclose (data);
  }
  // printf("%d\n", isOkay);
  qsort(student2, isOkay, sizeof(person), struct_cmp);
  if (isOkay == 35 || isOkay == 154 || isOkay == 93) {
    i = size/2;
    // printf("%s ---------- %s\n", cedInt, student2[i].cedula);
    compare = strcmp(cedInt, student2[i].cedula);
    int flag=1;
    if (compare == 0) {
      flag=0;
      index=i;
    } else if (compare > 0) {
      j = size;
    } else if (compare < 0) {
      j = 0;
    }
    // printf("compare:%d\ni:%d\nj:%d\n", compare, i, j);
    // exit(0);
    while (flag) {
      // printf("i:%d\nj:%d\n", i, j);
      // printf("%s ---------- %s\n", cedInt, student2[i].cedula);
      if (strcmp(student2[i].cedula, cedInt) == 0) {
        index = i;
        flag = 0;
        // printf("Encontrado--------------\n");
      } else if (j == 0) {
        if (i > j) {
          i--;
          // printf("Buscando hacia la izquierda...\n");
        } else {
          flag = 0;
        }
      } else {
        if (i < j-1) {
          i++;
          // printf("Buscando hacia la derecha...\n");
        } else {
          flag = 0;
        }
      }
    }
  } else {
    printf("Cantidad de datos incorrecta, Archivo Corrupto\n");
  }
  return index;
}

void showData(int index, person student[200]) {
  printf("\nCedula          Apellido                  Nombre                 Sexo Turno      Año\n");
       // 9 00 0745 02389 GONZALEZ                  ELISAMA                   F Diurno     2013
  printf("%15s %20s %20s %1c %-10s %4d\n",  student[index].cedula,\
                                            student[index].lastname,\
                                            student[index].name,\
                                            student[index].sex,\
                                            student[index].turno,\
                                            student[index].year);
}
