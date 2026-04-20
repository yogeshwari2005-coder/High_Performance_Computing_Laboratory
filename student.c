#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

typedef struct Student
{
    char name[20];
    int roll_no;
    float total_marks;
    char grade;
} Student;

int main()
{
    int my_rank, comm_sz;
    int total_students;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0)
    {
        FILE* fp = fopen("input.txt", "r");
        if (fp == NULL)
        {
            printf("Error: Could not open input.txt\n");
            total_students = -1;
        }
        else
        {
            fscanf(fp, "%d", &total_students);
            fclose(fp);
        }
    }

    MPI_Bcast(&total_students, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (total_students == -1)
    {
        MPI_Finalize();
        return 0;
    }

    int n_per_proc = total_students / comm_sz;
    Student *all_students = NULL;
    Student *local_students = malloc(n_per_proc * sizeof(Student));

    MPI_Datatype student_mpi_t;
    int blocklengths[4] = {20, 1, 1, 1};
    MPI_Datatype types[4] = {MPI_CHAR, MPI_INT, MPI_FLOAT, MPI_CHAR};
    MPI_Aint displacements[4];

    Student sample;
    MPI_Aint base_addr, name_addr, roll_addr, marks_addr, grade_addr;
    MPI_Get_address(&sample, &base_addr);
    MPI_Get_address(sample.name, &name_addr);
    MPI_Get_address(&sample.roll_no, &roll_addr);
    MPI_Get_address(&sample.total_marks, &marks_addr);
    MPI_Get_address(&sample.grade, &grade_addr);

    displacements[0] = name_addr - base_addr;
    displacements[1] = roll_addr - base_addr;
    displacements[2] = marks_addr - base_addr;
    displacements[3] = grade_addr - base_addr;

    MPI_Type_create_struct(4, blocklengths, displacements, types, &student_mpi_t);
    MPI_Type_commit(&student_mpi_t);

    if (my_rank == 0)
    {
        all_students = malloc(total_students * sizeof(Student));
        FILE* fp = fopen("input.txt", "r");
        int dummy;
        fscanf(fp, "%d", &dummy);
        for (int i = 0; i < total_students; i++)
            fscanf(fp, "%s %d %f", all_students[i].name, &all_students[i].roll_no, &all_students[i].total_marks);
        fclose(fp);
    }

    MPI_Scatter(all_students, n_per_proc, student_mpi_t, local_students, n_per_proc, student_mpi_t, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n_per_proc; i++)
    {
        if (local_students[i].total_marks >= 85)
            local_students[i].grade = 'O';
        else if (local_students[i].total_marks >= 70)
            local_students[i].grade = 'A';
        else if (local_students[i].total_marks >= 50)
            local_students[i].grade = 'B';
        else
            local_students[i].grade = 'F';
    }

    MPI_Gather(local_students, n_per_proc, student_mpi_t, all_students, n_per_proc, student_mpi_t, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        FILE* out = fopen("grade.txt", "w");
        for (int i = 0; i < total_students; i++)
            fprintf(out, "Roll: %d | Name: %s | Grade: %c\n", all_students[i].roll_no, all_students[i].name, all_students[i].grade);
        fclose(out);
        printf("Processed %d students. Results in grades.txt\n", total_students);
        free(all_students);
    }

    free(local_students);
    MPI_Type_free(&student_mpi_t);
    MPI_Finalize();
    return 0;
}
