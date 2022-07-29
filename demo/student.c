/*!
\file student.c
\author Edward Ji
\date 27 Jul 2022

\brief
A demonstration of dynamic array with a more complex structure.

\note
This program is interactive, make and run to try it out.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../darray.h"

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x
#define NAME_LEN 32
#define BUF_LEN 128
#define SCORE_MAX 100
#define CSV_NAME "./demo/student.csv"

typedef struct {
    size_t id;
    char name[NAME_LEN];
    unsigned char score; // range 0 - SCORE_MAX
} student;

student *new_student(unsigned id, const char *name, unsigned char score) {
    student *stu = malloc(sizeof(student));

    stu->id = id;
    strncpy(stu->name, name, NAME_LEN - 1);
    stu->score = score;

    return stu;
}

student *student_from_line(char *line) {
    size_t id;
    char name[NAME_LEN];
    unsigned char score;

    if (sscanf(line,
                "%zu,%" STRINGIFY(NAME_LEN) "[^,],%hhu",
                &id, name, &score) != 3 ||
            score > SCORE_MAX) {
        return NULL;
    }

    return new_student(id, name, score);
}

void print_student(void *p) {
    student *stu = p;
    printf("%08zu %-" STRINGIFY(NAME_LEN) "s %3hhu/" STRINGIFY(SCORE_MAX) "\n",
            stu->id, stu->name, stu->score);
}

int student_has_id(const void *p1, const void *p2) {
    const student *stu = p1;
    const int *p_id = p2;

    return (stu->id > *p_id) - (stu->id < *p_id);
}

int student_has_name(const void *p1, const void *p2) {
    const student *stu = p1;
    const char *name = p2;

    return strcmp(stu->name, name);
}

int student_cmp_id(const void *p1, const void *p2) {
    const student *stu1 = p1;
    const student *stu2 = p2;

    return (stu1->id > stu2->id) - (stu1->id < stu2->id);
}

int student_cmp_name(const void *p1, const void *p2) {
    const student *stu1 = p1;
    const student *stu2 = p2;

    return strcmp(stu1->name, stu2->name);
}

void *shallow_cpy(const void *p) {
    return (void *) p;
}

darray *read_csv(const char *fname) {
    FILE *csv = fopen(fname, "r");
    if (csv == NULL) {
        perror("fail to read CSV");
        return NULL;
    }

    char buffer[BUF_LEN];
    size_t line_no = 1;
    darray *students = new_darray(free);
    while (fgets(buffer, BUF_LEN, csv) != NULL) {
        student *stu = student_from_line(buffer);
        if (stu == NULL) {
            fprintf(stderr, "fail to parse line %zu\n", line_no);
        }
        darray_append(students, stu);
        line_no++;
    }
    fclose(csv);

    return students;
}

void list(darray *students) {
    darray_foreach(students, print_student);
}

void search_id(darray *students) {
    printf("<id>: ");

    char buffer[BUF_LEN] = { 0 };
    if (fgets(buffer, BUF_LEN, stdin) == NULL) {
        puts("missing id");
        return;
    }

    int id;
    if (sscanf(buffer, "%d", &id) != 1) {
        puts("invalid id");
        return;
    }

    size_t idx;
    if (darray_search(students, &id, student_has_id, &idx) == 0) {
        puts("not found");
        return;
    }

    print_student(darray_get(students, idx));
}

void search_name(darray *students) {
    char name[NAME_LEN] = { 0 };
    size_t idx;

    printf("<name>: ");
    if (fgets(name, NAME_LEN, stdin) == NULL) {
        puts("missing name");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    if (darray_search(students, &name, student_has_name, &idx) == 0) {
        puts("not found");
        return;
    }

    print_student(darray_get(students, idx));
}

void search(darray *students) {
    char buffer[BUF_LEN];
    printf("id, name, quit: ");
    if (fgets(buffer, BUF_LEN, stdin) == NULL) {
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strcmp(buffer, "id") == 0) {
        search_id(students);
    } else if (strcmp(buffer, "name") == 0) {
        search_name(students);
    } else {
        puts("invalid option");
        return;
    }
}

void sort(darray *students) {
    char buffer[BUF_LEN];
    printf("id, name, quit: ");
    if (fgets(buffer, BUF_LEN, stdin) == NULL) {
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strcmp(buffer, "id") == 0) {
        darray_sort(students, student_cmp_id);
    } else if (strcmp(buffer, "name") == 0) {
        darray_sort(students, student_cmp_name);
    } else {
        puts("invalid option");
        return;
    }
}

int main() {
    darray *students = read_csv(CSV_NAME);

    char buffer[BUF_LEN];
    do {
        printf("list, search, sort, quit: ");
        if (fgets(buffer, BUF_LEN, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "list") == 0) {
            list(students);
        } else if (strcmp(buffer, "search") == 0) {
            search(students);
        } else if (strcmp(buffer, "sort") == 0) {
            sort(students);
        } else if (strcmp(buffer, "quit") == 0) {
            break;
        } else {
            puts("invalid command");
        }
    } while (1);

    del_darray(students);

    return 0;
}
