#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXCHAR 1000

typedef struct normal_menu
{
    char *date;
    char *soup;
    char *main_dish;
    char *side_dish;
    char *extra;
    int *sale_count;
} normal_menu;

typedef struct vegan_menu
{
    char *date;
    char *soup;
    char *main_dish;
    char *side_dish;
    char *extra;
    int *sale_count;
} vegan_menu;

typedef struct vegetarian_menu
{
    char *date;
    char *soup;
    char *main_dish;
    char *side_dish;
    char *extra;
    int *sale_count;
} vegetarian_menu;

typedef struct cafeteria
{
    char *month;
    normal_menu *normal_menus;
    vegan_menu *vegan_menus;
    vegetarian_menu *vegetarian_menus;
} cafeteria;

void initialize_menus(cafeteria *cafe);
int get_count_of_menu_type(char *type);
void record_customer_counts(cafeteria *cafe, int normal_size, int vegetarian_size, int vegan_size);
void calc_and_show_income(cafeteria *cafe);

int main(int argc, char const *argv[])
{

    cafeteria cafe;
    int normal_size = get_count_of_menu_type("\"Normal\"");
    int vegetarian_size = get_count_of_menu_type("\"Vegetarian\"");
    int vegan_size = get_count_of_menu_type("\"Vegan\"");

    normal_menu *normals = malloc(sizeof(normal_menu) * normal_size);
    vegetarian_menu *vegetarians = malloc(sizeof(normal_menu) * vegetarian_size);
    vegan_menu *vegans = malloc(sizeof(normal_menu) * vegan_size);

    cafe.normal_menus = normals;
    cafe.vegetarian_menus = vegetarians;
    cafe.vegan_menus = vegans;
    initialize_menus(&cafe);
    record_customer_counts(&cafe, normal_size, vegetarian_size, vegan_size);
    printf("%d\n", cafe.vegetarian_menus[11].sale_count[2]);

    printf("%s\n", "-----Normal menu for the first and the last days of the month:");
    printf("%s %s %s %s %s %s %s %s %s\n", cafe.normal_menus[0].date, " | ", cafe.normal_menus[0].soup, " | ", cafe.normal_menus[0].main_dish, "| ", cafe.normal_menus[0].side_dish, " | ", cafe.normal_menus[0].extra);
    printf("%s %s %s %s %s %s %s %s %s\n\n", cafe.normal_menus[22].date, " | ", cafe.normal_menus[22].soup, " | ", cafe.normal_menus[22].main_dish, "| ", cafe.normal_menus[22].side_dish, " | ", cafe.normal_menus[22].extra);
    printf("%s\n", "-----Vegan menu for the first and the last days of the month:");
    printf("%s %s %s %s %s %s %s %s %s\n", cafe.vegan_menus[0].date, " | ", cafe.vegan_menus[0].soup, " | ", cafe.vegan_menus[0].main_dish, "| ", cafe.vegan_menus[0].side_dish, " | ", cafe.vegan_menus[0].extra);
    printf("%s %s %s %s %s %s %s %s %s\n\n", cafe.vegan_menus[22].date, " | ", cafe.vegan_menus[22].soup, " | ", cafe.vegan_menus[22].main_dish, "| ", cafe.vegan_menus[22].side_dish, " | ", cafe.vegan_menus[22].extra);
    printf("%s\n", "-----Vegetarian menu for the first and the last days of the month:");
    printf("%s %s %s %s %s %s %s %s %s\n", cafe.vegetarian_menus[0].date, " | ", cafe.vegetarian_menus[0].soup, " | ", cafe.vegetarian_menus[0].main_dish, "| ", cafe.vegetarian_menus[0].side_dish, " | ", cafe.vegetarian_menus[0].extra);
    printf("%s %s %s %s %s %s %s %s %s\n\n", cafe.vegetarian_menus[22].date, " | ", cafe.vegetarian_menus[22].soup, " | ", cafe.vegetarian_menus[22].main_dish, "| ", cafe.vegetarian_menus[22].side_dish, " | ", cafe.vegetarian_menus[22].extra);


    int dates[] = {2, 9, 19};
    for (int i = 0; i < 3; i++)
    {
        printf("%s %d %s\n", "March", dates[i]+1 , "customers: ");
        printf("%s %d %s", "Students:", cafe.normal_menus[dates[i]].sale_count[0] + cafe.vegan_menus[dates[i]].sale_count[0] + cafe.vegetarian_menus[dates[i]].sale_count[0] + ", ");
        printf("%s %d %s", "Academic:", cafe.normal_menus[dates[i]].sale_count[1] + cafe.vegan_menus[dates[i]].sale_count[1] + cafe.vegetarian_menus[dates[i]].sale_count[1] + ", ");
        printf("%s %d %s\n\n", "Administrative:", cafe.normal_menus[dates[i]].sale_count[2] + cafe.vegan_menus[dates[i]].sale_count[2] + cafe.vegetarian_menus[dates[i]].sale_count[2]);
    }
    calc_and_show_income(&cafe);

    return 0;
}  

void initialize_menus(cafeteria *cafe)
{
    FILE *fp;
    char row[MAXCHAR];
    char delimiter[] = ",";
    fp = fopen("cafeteria_march_menu.csv", "r");
    int curr = 0;
    int vegetariancurr = 0;
    int vegancurr = 0;

    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);
        char *ptr = strtok(row, delimiter);
        if (strcmp(ptr, "\"Normal\"") == 0)
        {

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
            curr += 1;
        }
        else if (strcmp(ptr, "\"Vegetarian\""))
        {
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
            vegetariancurr += 1;
        }
        else if (strcmp(ptr, "\"Vegan\""))
        {
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
            vegancurr += 1;
        }
    }
}

void record_customer_counts(cafeteria *cafe)
{
    for (int i = 0; i < get_count_of_menu_type("\"Normal\""); i++)
    {
        cafe->normal_menus[i].sale_count[0] = rand() % 51;
        cafe->normal_menus[i].sale_count[1] = rand() % 51;
        cafe->normal_menus[i].sale_count[2] = rand() % 51;
    }
    for (int i = 0; i < get_count_of_menu_type("\"Vegetarian\""); i++)
    {
        cafe->vegetarian_menus[i].sale_count[0] = rand() % 51;
        cafe->vegetarian_menus[i].sale_count[1] = rand() % 51;
        cafe->vegetarian_menus[i].sale_count[2] = rand() % 51;
    }
    for (int i = 0; i < get_count_of_menu_type("\"Vegan\""); i++)
    {
        cafe->vegan_menus[i].sale_count[0] = rand() % 51;
        cafe->vegan_menus[i].sale_count[1] = rand() % 51;
        cafe->vegan_menus[i].sale_count[2] = rand() % 51;
    }
}

int get_count_of_menu_type(char *type)
{
    FILE *fp;
    char row[MAXCHAR];
    char delimiter[] = ",";
    int count = 0;
    fp = fopen("cafeteria_march_menu.csv", "r");

    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);
        char *ptr = strtok(row, delimiter);
        if (strcmp(ptr, type) == 0)
        {
            count++;
        }
    }
    return count;
}


void calc_and_show_income(cafeteria *cafe){
    int price_for_student = 6;
    int price_for_academic = 16;
    int price_for_administrative = 12;
    int normal_income= 0;
    int vegetarian_income = 0;
    int vegan_income = 0;
    int student_income = 0;
    int academic_income = 0;
    int administrative_income = 0;

    for (int i = 0; i <= get_count_of_menu_type("\"Normal\""); i++){
        normal_income += cafe->normal_menus[i].sale_count[0] * price_for_student 
                        + cafe->normal_menus[i].sale_count[1] * price_for_academic 
                        + cafe->normal_menus[i].sale_count[2] * price_for_administrative;

        student_income += cafe->normal_menus[i].sale_count[0] * price_for_student;
        academic_income += cafe->normal_menus[i].sale_count[1] * price_for_academic;
        administrative_income += cafe->normal_menus[i].sale_count[2] * price_for_administrative;
    }

    for (int i = 0; i < get_count_of_menu_type("\"Vegetarian\""); i++)
    {
        vegetarian_income += cafe->vegetarian_menus[i].sale_count[0] * price_for_student 
                            + cafe->vegetarian_menus[i].sale_count[1] * price_for_academic 
                            + cafe->vegetarian_menus[i].sale_count[2] * price_for_administrative;
        
        student_income += cafe->vegetarian_menus[i].sale_count[0] * price_for_student;
        academic_income += cafe->vegetarian_menus[i].sale_count[1] * price_for_academic;
        administrative_income += cafe->vegetarian_menus[i].sale_count[2] * price_for_administrative;
    }

    for (int i = 0; i < get_count_of_menu_type("\"Vegan\""); i++)
    {
        vegan_income += cafe->vegan_menus[i].sale_count[0] * price_for_student 
                        + cafe->vegan_menus[i].sale_count[1] * price_for_academic 
                        + cafe->vegan_menus[i].sale_count[2] * price_for_administrative;
        
        student_income += cafe->vegan_menus[i].sale_count[0] * price_for_student;
        academic_income += cafe->vegan_menus[i].sale_count[1] * price_for_academic;
        administrative_income += cafe->vegan_menus[i].sale_count[2] * price_for_administrative;
    }

    printf("%s\n\n", "************ The Sales Results ************");
    printf("%s %d %s %d %s %d\n", "Normal menu sales: ", normal_income, " TL, Vegetarian menu sales: ", vegetarian_income, " TL, Vegan menu sales: ", vegan_income);
    printf("%s %d %s %d %s %d\n", "Student sales: ", student_income, " TL, Academic personal sales: ", academic_income, " TL, Administrative personal sales: ", administrative_income, " TL");
    printf("%s %d", "Total sale income: ", student_income + academic_income + administrative_income);


    



}

