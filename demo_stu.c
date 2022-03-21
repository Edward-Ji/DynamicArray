#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

#define NAMELEN 32
#define BUFLEN 64

typedef struct {
    char name[NAMELEN];
    unsigned char age;
} student;

student *new_student(char *name, unsigned char age) {
    student *stu = malloc(sizeof(student));
    strncpy(stu->name, name, NAMELEN);
    stu->age = age;

    return stu;
}

void student_print(student *stu) {
    printf("%s %hhu\n", stu->name, stu->age);
}

int student_has_name(student *stu, char *name) {
    return strcmp(stu->name, name);
}

void student_age_add(student *stu, unsigned *sum) {
    *sum += stu->age;
}

int main() {
    darray *students = new_darray(sizeof(student), free);

    darray_append(students, new_student("Edward", 19));
    darray_append(students, new_student("Somebody", 21));
    darray_append(students, new_student("Nobody", 23));
    darray_append(students, new_student("Old Man", 100));
    darray_insert(students, 1, new_student("Super Old Man", 200));
    darray_pop(students, 0);

    printf("Students\n");
    darray_foreach(students, (consumer) student_print);
    putchar('\n');

    darray *old_folks = new_darray(sizeof(student), NULL);

    for (size_t i = 0; i < students->len; i++) {
        student *stu = darray_get(students, i);
        if (stu->age > 50) {
            darray_append(old_folks, stu);
        }
    }

    printf("Old folks\n");
    darray_foreach(old_folks, (consumer) student_print);
    putchar('\n');

    printf("Student with name Nobody\n");
    size_t idx;
    if (darray_search(students, "Nobody", (comparator) student_has_name, &idx)) {
        student_print(darray_get(students, idx));
    }

    unsigned age_sum = 0;
    darray_aggregate(students, &age_sum, (aggregate) student_age_add);
    printf("Average age\n%lf\n\n", (double) age_sum / students->len);

    del_darray(old_folks);

    del_darray(students);

    return 0;
}
