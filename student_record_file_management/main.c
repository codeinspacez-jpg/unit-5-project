#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"
#define TEXT_FILE "students.txt"
#define INITIAL_CAPACITY 4

typedef struct {
    int roll;
    char name[80];
    char department[60];
    int year;
    float marks;
    char grade;
} Student;

static int menuOperations = 0;

char gradeFromMarks(float marks) {
    if (marks >= 90) return 'A';
    if (marks >= 80) return 'B';
    if (marks >= 70) return 'C';
    if (marks >= 60) return 'D';
    return 'F';
}

void printStudent(const Student *s) {
    printf("\nRoll Number : %d\nName        : %s\nDepartment   : %s\nYear        : %d\nMarks       : %.2f\nGrade       : %c\n",
           s->roll, s->name, s->department, s->year, s->marks, s->grade);
}

int findByRoll(Student *students, int count, int roll) {
    for (int i = 0; i < count; i++)
        if (students[i].roll == roll) return i;
    return -1;
}

int loadRecords(Student **students, int *count, int *capacity) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Student record;

    if (fp == NULL) return 1; /* A new project may not have a data file yet. */

    while (fread(&record, sizeof(Student), 1, fp) == 1) {
        if (*count >= *capacity) {
            int newCapacity = (*capacity == 0) ? INITIAL_CAPACITY : *capacity * 2;
            Student *temp = realloc(*students, newCapacity * sizeof(Student));
            if (temp == NULL) {
                fclose(fp);
                return 0;
            }
            *students = temp;
            *capacity = newCapacity;
        }
        (*students)[*count] = record;
        (*count)++;
    }

    if (ferror(fp)) {
        printf("Error reading records file.\n");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int saveBinary(Student *students, int count) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Could not open binary file for saving.\n");
        return 0;
    }

    if (count > 0 && fwrite(students, sizeof(Student), count, fp) != (size_t)count) {
        printf("Error writing binary records.\n");
        fclose(fp);
        return 0;
    }
    return fclose(fp) == 0;
}

int saveText(Student *students, int count) {
    FILE *fp = fopen(TEXT_FILE, "w");
    if (fp == NULL) {
        printf("Could not open text file for saving.\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%d|%s|%s|%d|%.2f|%c\n",
                    students[i].roll, students[i].name, students[i].department,
                    students[i].year, students[i].marks, students[i].grade) < 0) {
            fclose(fp);
            return 0;
        }
    }
    return fclose(fp) == 0;
}

int saveAll(Student *students, int count) {
    if (!saveBinary(students, count)) return 0;
    if (!saveText(students, count)) return 0;
    return 1;
}

void addStudent(Student **students, int *count, int *capacity) {
    Student s;

    printf("Roll number: ");
    if (scanf("%d", &s.roll) != 1) { puts("Invalid roll."); return; }
    if (findByRoll(*students, *count, s.roll) != -1) {
        puts("That roll number already exists.");
        return;
    }

    printf("Name (spaces allowed): ");
    scanf(" %79[^\n]", s.name);
    printf("Department (spaces allowed): ");
    scanf(" %59[^\n]", s.department);
    printf("Year of study: ");
    if (scanf("%d", &s.year) != 1) { puts("Invalid year."); return; }
    printf("Marks (0-100): ");
    if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
        puts("Marks must be between 0 and 100.");
        return;
    }
    s.grade = gradeFromMarks(s.marks);

    if (*count == *capacity) {
        int newCapacity = (*capacity == 0) ? INITIAL_CAPACITY : *capacity * 2;
        Student *temp = realloc(*students, newCapacity * sizeof(Student));
        if (temp == NULL) { puts("Memory allocation failed; record not added."); return; }
        *students = temp;
        *capacity = newCapacity;
    }

    (*students)[*count] = s;
    (*count)++;
    if (saveAll(*students, *count)) puts("Record added and saved.");
    else puts("Record added to memory, but file saving failed.");
}

void displayAll(Student *students, int count) {
    if (count == 0) { puts("No records."); return; }
    printf("\nTotal records: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("\n--- Student %d ---", i + 1);
        printStudent(&students[i]);
    }
}

void searchStudent(Student *students, int count) {
    int roll;
    printf("Roll number to search: ");
    if (scanf("%d", &roll) != 1) { puts("Invalid roll."); return; }
    int index = findByRoll(students, count, roll);
    if (index == -1) puts("Student not found.");
    else printStudent(&students[index]);
}

void updateStudent(Student *students, int count) {
    int roll;
    printf("Roll number to update: ");
    if (scanf("%d", &roll) != 1) { puts("Invalid roll."); return; }
    int index = findByRoll(students, count, roll);
    if (index == -1) { puts("Student not found."); return; }

    Student changed = students[index];
    printf("New name: ");
    scanf(" %79[^\n]", changed.name);
    printf("New department: ");
    scanf(" %59[^\n]", changed.department);
    printf("New year: ");
    if (scanf("%d", &changed.year) != 1) { puts("Invalid year."); return; }
    printf("New marks (0-100): ");
    if (scanf("%f", &changed.marks) != 1 || changed.marks < 0 || changed.marks > 100) {
        puts("Marks must be between 0 and 100.");
        return;
    }
    changed.grade = gradeFromMarks(changed.marks);
    students[index] = changed;
    if (saveAll(students, count)) puts("Record updated.");
    else puts("Updated in memory, but saving failed.");
}

void deleteStudent(Student *students, int *count) {
    int roll;
    printf("Roll number to delete: ");
    if (scanf("%d", &roll) != 1) { puts("Invalid roll."); return; }
    int index = findByRoll(students, *count, roll);
    if (index == -1) { puts("Student not found."); return; }

    for (int i = index; i < *count - 1; i++)
        students[i] = students[i + 1];
    (*count)--;

    if (saveAll(students, *count)) puts("Record deleted.");
    else puts("Deleted in memory, but saving failed.");
}

void filterByMarks(Student *students, int count) {
    float minimum;
    printf("Display students with marks greater than: ");
    if (scanf("%f", &minimum) != 1) { puts("Invalid marks."); return; }
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].marks > minimum) {
            printStudent(&students[i]);
            found = 1;
        }
    }
    if (!found) puts("No matching records.");
}

int main(void) {
    Student *students = calloc(INITIAL_CAPACITY, sizeof(Student));
    int count = 0, capacity = INITIAL_CAPACITY, choice = 0;

    if (students == NULL) {
        puts("Could not allocate memory.");
        return 1;
    }

    if (!loadRecords(&students, &count, &capacity)) {
        puts("Could not load records. Exiting.");
        free(students);
        return 1;
    }

    do {
        printf("\n===== STUDENT RECORD & FILE MANAGEMENT =====\n");
        printf("1. Add Student\n2. Display All\n3. Search by Roll Number\n");
        printf("4. Update Student\n5. Delete Student\n6. Count Students\n");
        printf("7. Filter by Marks\n8. Save Text File\n9. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            puts("Invalid input. Exiting.");
            break;
        }

        menuOperations++;
        switch (choice) {
            case 1: addStudent(&students, &count, &capacity); break;
            case 2: displayAll(students, count); break;
            case 3: searchStudent(students, count); break;
            case 4: updateStudent(students, count); break;
            case 5: deleteStudent(students, &count); break;
            case 6: printf("Student count: %d\n", count); break;
            case 7: filterByMarks(students, count); break;
            case 8: if (saveText(students, count)) puts("Text file saved."); else puts("Text save failed."); break;
            case 9: puts("Exiting..."); break;
            default: puts("Invalid choice.");
        }
    } while (choice != 9);

    printf("Menu operations: %d\n", menuOperations);
    free(students);
    puts("Memory freed. Goodbye!");
    return 0;
}
