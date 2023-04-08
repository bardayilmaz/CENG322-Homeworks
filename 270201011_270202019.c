#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>


#define MAXCHAR 1000

typedef struct normal_menu {
    char* date;
    char* soup;
    char* main_dish;
    char* side_dish;
    char* extra;
    int* sale_count;
} normal_menu;

typedef struct vegan_menu {
    char* date;
    char* soup;
    char* main_dish;
    char* side_dish;
    char* extra;
    int* sale_count;
} vegan_menu;

typedef struct vegetarian_menu {
    char* date;
    char* soup;
    char* main_dish;
    char* side_dish;
    char* extra;
    int* sale_count;
} vegetarian_menu;

typedef struct cafeteria {
    char* month;
    normal_menu* normal_menus;
    vegan_menu* vegan_menus;
    vegetarian_menu* vegetarian_menus;
} cafeteria;

void read_csv(cafeteria* cafe);
int get_count_of_menu_type(char* type);
void set_customer_counts(cafeteria* cafe, int normal_size, int vegetarian_size, int vegan_size);

int main(int argc, char const *argv[])
{

    cafeteria cafe;
    int normal_size = get_count_of_menu_type("\"Normal\"");
    int vegetarian_size = get_count_of_menu_type("\"Vegetarian\"");
    int vegan_size = get_count_of_menu_type("\"Vegan\"");
    normal_menu* normals = malloc(sizeof(normal_menu) * normal_size);
    vegetarian_menu* vegetarians = malloc(sizeof(normal_menu) * vegetarian_size);
    vegan_menu* vegans = malloc(sizeof(normal_menu) * vegan_size);
    
    cafe.normal_menus = normals;
    cafe.vegetarian_menus = vegetarians;
    cafe.vegan_menus = vegans;
    read_csv(&cafe);
    set_customer_counts(&cafe, normal_size, vegetarian_size, vegan_size);
    printf("%d\n", cafe.vegetarian_menus[11].sale_count[2]);
    return 0;
}
    

void read_csv(cafeteria* cafe) {
    FILE *fp;
    char row[MAXCHAR];
    char delimiter[] = ",";
    fp = fopen("cafeteria_march_menu.csv","r");
    int curr = 0;
    int vegetariancurr = 0;
    int vegancurr = 0;

    while (feof(fp) != true) {
        fgets(row, MAXCHAR, fp);
        char *ptr = strtok(row, delimiter);
        if(strcmp(ptr, "\"Normal\"") == 0) {
            
            ptr = strtok(NULL, delimiter);
            cafe->normal_menus[curr].date = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->normal_menus[curr].date, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->normal_menus[curr].soup = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->normal_menus[curr].soup, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->normal_menus[curr].main_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->normal_menus[curr].main_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->normal_menus[curr].side_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->normal_menus[curr].side_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->normal_menus[curr].extra = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->normal_menus[curr].extra, ptr);
            cafe->normal_menus[curr].sale_count = malloc(3 * sizeof(int));
            curr+=1;

        } else if (strcmp(ptr, "\"Vegetarian\"")) {
            ptr = strtok(NULL, delimiter);
            cafe->vegetarian_menus[vegetariancurr].date = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegetarian_menus[vegetariancurr].date, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegetarian_menus[vegetariancurr].soup = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegetarian_menus[vegetariancurr].soup, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegetarian_menus[vegetariancurr].main_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegetarian_menus[vegetariancurr].main_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegetarian_menus[vegetariancurr].side_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegetarian_menus[vegetariancurr].side_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegetarian_menus[vegetariancurr].extra = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegetarian_menus[vegetariancurr].extra, ptr);
            cafe->vegetarian_menus[vegetariancurr].sale_count = malloc(3 * sizeof(int));
            vegetariancurr+=1;
            
        } else if (strcmp(ptr, "\"Vegan\"")) {
            ptr = strtok(NULL, delimiter);
            cafe->vegan_menus[vegancurr].date = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegan_menus[vegancurr].date, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegan_menus[vegancurr].soup = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegan_menus[vegancurr].soup, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegan_menus[vegancurr].main_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegan_menus[vegancurr].main_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegan_menus[vegancurr].side_dish = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegan_menus[vegancurr].side_dish, ptr);
            ptr = strtok(NULL, delimiter);
            cafe->vegan_menus[vegancurr].extra = malloc(sizeof(char) * strlen(ptr));
            strcpy(cafe->vegan_menus[vegancurr].extra, ptr);
            cafe->vegan_menus[vegancurr].sale_count = malloc(3 * sizeof(int));
            vegancurr+=1;
        }
    }
    
}

void set_customer_counts(cafeteria* cafe, int normal_size, int vegetarian_size, int vegan_size) {
    for(int i = 0; i < normal_size; i++) {
        cafe->normal_menus[i].sale_count[0] = rand() % 51;
        cafe->normal_menus[i].sale_count[1] = rand() % 51;
        cafe->normal_menus[i].sale_count[2] = rand() % 51;
    }
    for(int i = 0; i < vegetarian_size; i++) {
        cafe->vegetarian_menus[i].sale_count[0] = rand() % 51;
        cafe->vegetarian_menus[i].sale_count[1] = rand() % 51;
        cafe->vegetarian_menus[i].sale_count[2] = rand() % 51;
    }
    for(int i = 0; i < vegan_size; i++) {
        cafe->vegan_menus[i].sale_count[0] = rand() % 51;
        cafe->vegan_menus[i].sale_count[1] = rand() % 51;
        cafe->vegan_menus[i].sale_count[2] = rand() % 51;
    }
}

int get_count_of_menu_type(char* type) {
    FILE *fp;
    char row[MAXCHAR];
    char delimiter[] = ",";
    int count = 0;
    fp = fopen("cafeteria_march_menu.csv","r");

     while (feof(fp) != true) {
        fgets(row, MAXCHAR, fp);
        char *ptr = strtok(row, delimiter);
        if(strcmp(ptr, type) == 0) {
            count++;
        }
    }
    return count;
}