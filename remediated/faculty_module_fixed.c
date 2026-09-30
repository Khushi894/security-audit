#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#define MAX_NAME 32U
#define INITIAL_MARKS 5U
#define RESIZED_MARKS 10U

typedef struct {
    char *name;
    int *marks;
    size_t count;
} Student;

static int checked_size_mul(size_t a, size_t b, size_t *result)
{
    if (b != 0U && a > SIZE_MAX / b) {
        return 0;
    }
    *result = a * b;
    return 1;
}

static int checked_int_add(int a, int b, int *result)
{
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    *result = a + b;
    return 1;
}

static Student *create_student(const char *name, size_t count)
{
    if (name == NULL || strlen(name) > MAX_NAME) {
        return NULL;
    }

    Student *s = malloc(sizeof *s);
    if (s == NULL) {
        return NULL;
    }
    s->name = NULL;
    s->marks = NULL;
    s->count = 0U;

    size_t name_bytes;
    if (checked_size_mul(strlen(name) + 1U, sizeof(char), &name_bytes) == 0) {
        free(s);
        return NULL;
    }

    s->name = malloc(name_bytes);
    if (s->name == NULL) {
        free(s);
        return NULL;
    }
    memcpy(s->name, name, name_bytes);

    size_t mark_bytes = 0U;
    if (count != 0U && checked_size_mul(count, sizeof *s->marks, &mark_bytes) == 0) {
        free(s->name);
        free(s);
        return NULL;
    }

    if (count != 0U) {
        s->marks = malloc(mark_bytes);
        if (s->marks == NULL) {
            free(s->name);
            free(s);
            return NULL;
        }
    }
    s->count = count;
    return s;
}

static int calculate_total(const Student *s, int *total_out)
{
    if (s == NULL || total_out == NULL) {
        return 0;
    }

    int total = 0;
    for (size_t i = 0U; i < s->count; ++i) {
        if (checked_int_add(total, s->marks[i], &total) == 0) {
            return 0;
        }
    }
    *total_out = total;
    return 1;
}

static int add_bonus(Student *s, int bonus)
{
    if (s == NULL) {
        return 0;
    }

    for (size_t i = 0U; i < s->count; ++i) {
        int updated;
        if (checked_int_add(s->marks[i], bonus, &updated) == 0) {
            return 0;
        }
        s->marks[i] = updated;
    }
    return 1;
}

static int resize_marks(Student *s, size_t new_count)
{
    if (s == NULL) {
        return 0;
    }

    if (new_count == 0U) {
        free(s->marks);
        s->marks = NULL;
        s->count = 0U;
        return 1;
    }

    size_t bytes;
    if (checked_size_mul(new_count, sizeof *s->marks, &bytes) == 0) {
        return 0;
    }

    int *new_marks = realloc(s->marks, bytes);
    if (new_marks == NULL) {
        return 0;
    }

    if (new_count > s->count) {
        memset(new_marks + s->count, 0,
               (new_count - s->count) * sizeof *new_marks);
    }

    s->marks = new_marks;
    s->count = new_count;
    return 1;
}

static void display_student(const Student *s)
{
    if (s == NULL) {
        return;
    }

    printf("Student: %s\n", s->name);
    for (size_t i = 0U; i < s->count; ++i) {
        printf("Mark[%zu] = %d\n", i, s->marks[i]);
    }
}

static void destroy_student(Student *s)
{
    if (s == NULL) {
        return;
    }
    free(s->marks);
    free(s->name);
    free(s);
}

int main(void)
{
    Student *student = create_student("Alice", INITIAL_MARKS);
    if (student == NULL) {
        fprintf(stderr, "Failed to create student\n");
        return EXIT_FAILURE;
    }

    const int initial_marks[INITIAL_MARKS] = {80, 75, 90, 85, 95};
    memcpy(student->marks, initial_marks, sizeof initial_marks);

    display_student(student);

    int total;
    if (calculate_total(student, &total) == 0) {
        fprintf(stderr, "Total calculation overflowed\n");
        destroy_student(student);
        return EXIT_FAILURE;
    }
    printf("Total = %d\n", total);

    if (add_bonus(student, 5) == 0) {
        fprintf(stderr, "Bonus operation overflowed\n");
        destroy_student(student);
        return EXIT_FAILURE;
    }

    if (resize_marks(student, RESIZED_MARKS) == 0) {
        fprintf(stderr, "Failed to resize marks array\n");
        destroy_student(student);
        return EXIT_FAILURE;
    }
    display_student(student);

    destroy_student(student);
    student = NULL;
    return EXIT_SUCCESS;
}
