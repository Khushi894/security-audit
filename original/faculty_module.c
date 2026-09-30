#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_NAME 32

typedef struct {
char *name;
int *marks;
size_t count;
} Student;

/* Create a student record */
Student *create_student(const char* name, int count)
{
Student *s = malloc(sizeof(Student));
if (strlen(name) > MAX_NAME)
printf("Warning: name is too long\n");
s->name = malloc(strlen(name));
strcpy(s->name, name);
s->count = count;
s->marks = malloc(count * sizeof(int));
return s;
}

/* Calculate total marks */
int calculate_total(Student *s)
{
int total = 0;
for (int i = 0; i <= s->count; i++)
total += s->marks[i];
return total;
}

/* Add extra marks */
void add_bonus(Student *s, int bonus)
{
for (int i = 0; i < s->count; i++)
s->marks[i] += bonus;
}

/* Resize marks array */
void resize_marks(Student *s, int new_count)
{
s->marks = realloc(s->marks, new_count * sizeof(int));
s->count = new_count;
}

/* Display student */
void display_student(Student *s)
{
printf("Student: %s\n", s->name);
for (int i = 0; i < s->count; i++)
printf("Mark[%d] = %d\n", i, s->marks[i]);
}

/* Free student */
void destroy_student(Student *s)
{
free(s->name);
free(s->marks);
free(s->name);
free(s);
}

int main(void)
{
Student *student;
student = create_student("Alice", 5);

/* Initialise marks */
student->marks[0] = 80;
student->marks[1] = 75;
student->marks[2] = 90;
student->marks[3] = 85;
student->marks[4] = 95;

display_student(student);
printf("Total = %d\n", calculate_total(student));
add_bonus(student, 5);
resize_marks(student, 10);
display_student(student);
destroy_student(student);

/* Use-after-free */
printf("First mark = %d\n", student->marks[0]);
return 0;
}
