# Student Database Management System

## Language : C

## Description: 
The Student Database Management System is a project implemented in the C programming language. It aims to provide a platform for managing student records efficiently. The system utilizes various algorithms and data structures to perform different operations on the student database, allowing administrators to handle student-related tasks effectively.

## Data Structures Used:

 AVL Tree : The student records are stored in a AVL Tree for efficient searching, insertion, and deletion operations.

## Algorithms Used:

1. Binary Search Tree Operations: The BST is used for storing and managing student records. Operations such as insertion, deletion, and traversal (in-order) are performed on the BST.
1. AVL Tree Balancing: The AVL tree balancing algorithm is employed to maintain the height balance of the BST, ensuring optimal performance and reducing the likelihood of skewed trees.

## Operations on the Student Database:

1. Printing Student Records: \
   - **printGSRL**: Prints all student records in the BST using an in-order traversal.
1. Printing Non-Applicant Students:   \
   - **printAllNonApplicantStudents**: Prints the records of students who have not applied for a specific program/course.
1. Printing Eligible Students:   
   - **printAllEligibleStudents**: Prints the records of students who meet specific eligibility criteria, such as having attendance above 75%, applying fees, and being an applicant.
1. Printing Low Attendance Students:   
   - **printAllLowAttendanceStudents**: Prints the records of students with attendance less than or equal to 75%.
1. Deleting Student Records:    
   - **deleteStudentRecord**: Deletes a student record from the BST based on the provided roll number.
1. Printing Students with Pending Fees and Good Attendance:   
   - **Question6**: Prints the records of students who have attendance above 75% and have not applied fees.
1. Printing Defaulter Students and Department-wise Defaulters:   
   - **printAllDefaulterStudents**: Prints the records of students who have pending fees or attendance less than or equal to 75% and displays the count of defaulters in each department.
1. Printing Students Within a Range of Roll Numbers:   
   - **printAllStudentsWithinRange**: Prints the records of students within a specified range of roll numbers.



By utilizing these operations, the Student Database Management System allows administrators to efficiently manage student records, track attendance, process applications, handle fees, and identify defaulters based on the specified criteria.

The project focuses on optimizing the retrieval and manipulation of student data through the use of a BST data structure and various algorithms to ensure efficient performance and accurate results.

