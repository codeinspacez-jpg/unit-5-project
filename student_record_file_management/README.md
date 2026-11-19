# Student Record and File Management System (Unit 5)

A menu-driven beginner–intermediate C project based on the uploaded Unit 5 project description.

## Features
- Add, display, search, update, and delete student records
- Count records and filter by marks
- Save student records to binary (`students.dat`) and readable text (`students.txt`)
- Load saved records when the program starts
- Dynamic memory allocation using `calloc`, `realloc`, and `free`
- Uses `fopen`, `fread`, `fwrite`, `fprintf`, `fclose`, read-error checks, and `static` storage class
- Student fields: roll number, full name, department, year, marks, grade

## Open in VS Code
1. Download and extract this ZIP.
2. In VS Code choose **File → Open Folder**.
3. Open the extracted `student_record_file_management` folder.
4. Open `main.c`.

## Compile and run

### Windows with GCC / MinGW
```bash
gcc main.c -o student_records
.\student_records.exe
```

### Linux or macOS
```bash
gcc main.c -o student_records
./student_records
```

## Menu
1. Add Student
2. Display All
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Students
7. Filter by Marks
8. Save Text File
9. Exit

## Grade calculation
- 90–100: A
- 80–89: B
- 70–79: C
- 60–69: D
- Below 60: F

## Files created when the program runs
- `students.dat` — binary persistence
- `students.txt` — text export, each record separated by `|`

## GitHub upload
Create a new repository on GitHub. In VS Code, open the project folder, then use the Source Control panel to initialize the repository, commit the files, and publish/push to GitHub. Do not upload generated executables or personal student records; `.gitignore` excludes these.

This project is an educational sample. It can be extended with more robust input handling and safe transactional file replacement.
