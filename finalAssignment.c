#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#define NAME_SIZE 20
#define ROLL_SIZE 15
#define SUBJECT_SIZE 5

typedef enum
{
    PENDING,
    CLEAR
} FEE_STATUS;
typedef enum
{
    NOTAPPLIED,
    APPLIED
} APPLICANT;
typedef enum
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH
} YEAR;

typedef struct Details_tag
{
    char name[NAME_SIZE];
    char dep[5];
    YEAR year;
    char rollno[ROLL_SIZE];
    int classesConducted[5];
    int classesAttended[5];
    FEE_STATUS fee;
    APPLICANT app;
} Details;
typedef Details Item_Type;

typedef struct Attendance_List_Tag
{
    char rollno[ROLL_SIZE];
    char dep[5];
    float attendance_Percent;
    struct Attendance_List_Tag *left, *right;
} Attendance_List_Node;

typedef struct FeeStatus_List_Tag
{
    char roll[ROLL_SIZE];
    FEE_STATUS fee;
    struct FeeStatus_List_Tag *left, *right;
} FeeStatus_List_Node;
typedef struct ApplicantStatus_List_Tag
{
    char name[NAME_SIZE];
    char roll[ROLL_SIZE];
    APPLICANT apply;
    struct ApplicantStatus_List_Tag *left, *right;
} ApplicantStatus_List_Node;

typedef struct node_tag
{
    Item_Type data;
    struct node_tag *left;
    struct node_tag *right;
    int height;
    //

    Attendance_List_Node *ptr_To_Attendance_List_Node;
    FeeStatus_List_Node *ptr_To_Fee_Status_List_Node;
    ApplicantStatus_List_Node *ptr_To_Applicant_Status_List_Node;

} Tree_Node;

int MCC[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};
int MCA[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};

//

//

void filling_MCC(Item_Type data)
{
    int row = data.year;
    int col = -1;
    char c = data.dep[0];
    switch (c)
    {
    case 'C': //CS or CV
        if (data.dep[1] == 'S')
            col = 0;
        else
            col = 1;
        break;
    case 'E':
        if (data.dep[1] == 'C')
            col = 2;
        else
            col = 3;
        break;
    case 'M':
        col = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    if (col != -1 && MCC[row][col] == -1) // TABULATION
    {
        int maxclasses_conducted = data.classesConducted[0];

        for (int i = 1; i < 5; i++) //no of subjects
        {
            if (maxclasses_conducted < data.classesConducted[i])
            {
                maxclasses_conducted = data.classesConducted[i];
            }
        }
        MCC[row][col] = maxclasses_conducted;
    }
    //filling MCA for MCC[row][col]
    for (int i = 1; i < 5; i++) //no of subjects
    {
        if (MCC[row][col] == data.classesConducted[i])
        {
            if (MCA[row][col] < data.classesAttended[i])
            {
                // printf("HELLO");
                MCA[row][col] = data.classesAttended[i];
            }
        }
    }
}

//

//

/*To Compare 2 nodes on the basis of year=>department=>roll no
    -1 for 1st node greater, 1 for 2nd node greater*/
int comparator(Details d1, Details d2)
{
    int ret_val;
    //SORT FROM YEAR 4 TO YEAR 1
    if (d1.year < d2.year)
    {
        ret_val = -1;
    }
    else if (d1.year > d2.year)
    {
        ret_val = 1;
    }
    else // YEAR IS SAME
    {
        if (strcmp(d1.dep, d2.dep) < 0)
        {
            ret_val = 1;
        }
        else if (strcmp(d1.dep, d2.dep) > 0)
        {
            ret_val = -1;
        }
        else //Department is same
        {
            if (strcmp(d1.rollno, d2.rollno) < 0)
            {
                ret_val = 1;
            }
            else
            {
                ret_val = -1;
            }
        }
    }
    return ret_val;
}

//

//Insertion and deletion in AVL Tree
Tree_Node *NewNode(Details data)
{
    Tree_Node *temp = (Tree_Node *)malloc(sizeof(Tree_Node));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 1;
    temp->ptr_To_Applicant_Status_List_Node = (ApplicantStatus_List_Node *)malloc(sizeof(ApplicantStatus_List_Node));
    temp->ptr_To_Attendance_List_Node = (Attendance_List_Node *)malloc(sizeof(Attendance_List_Node));
    temp->ptr_To_Fee_Status_List_Node = (FeeStatus_List_Node *)malloc(sizeof(FeeStatus_List_Node));

    temp->ptr_To_Applicant_Status_List_Node->apply = data.app;
    strcpy(temp->ptr_To_Applicant_Status_List_Node->name, data.name);
    strcpy(temp->ptr_To_Applicant_Status_List_Node->roll, data.rollno);

    temp->ptr_To_Fee_Status_List_Node->fee = data.fee;
    strcpy(temp->ptr_To_Fee_Status_List_Node->roll, data.rollno);

    strcpy(temp->ptr_To_Attendance_List_Node->dep, data.dep);
    strcpy(temp->ptr_To_Attendance_List_Node->rollno, data.rollno);

    return temp;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(Tree_Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int Balance(Tree_Node *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

Tree_Node *LeftRotate(Tree_Node *z)
{
    Tree_Node *y = z->right;
    Tree_Node *t2 = y->left;

    y->left = z;
    z->right = t2;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Tree_Node *RightRotate(Tree_Node *z)
{
    Tree_Node *y = z->left;
    Tree_Node *t3 = y->right;

    y->right = z;
    z->left = t3;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

void preorder(Tree_Node *root)
{
    if (root == NULL)
        return;

    printf("\n%20s %s", root->data.name, root->data.rollno);
    preorder(root->left);
    preorder(root->right);
}

Tree_Node *FindMin(Tree_Node *node)
{
    while (node->left != NULL)
        node = node->left;

    return node;
}

Tree_Node *Delete(Tree_Node *root, Item_Type data)
{
    if (root == NULL)
        return root;

    // if (data < root->data)
    if (comparator(data, root->data) == 1)
        root->left = Delete(root->left, data);
    // else if (data > root->data)
    else if (comparator(data, root->data) == -1)
        root->right = Delete(root->right, data);

    else
    {
        if (root->right == NULL && root->left == NULL)
        {
            free(root);
            root = NULL;
        }

        else if (root->left != NULL && root->right == NULL)
        {
            Tree_Node *temp = root->left;
            root = root->left;
            free(temp);
        }

        else if (root->right != NULL && root->left == NULL)
        {
            Tree_Node *temp = root->right;
            root = root->right;
            free(temp);
        }

        else
        {
            Tree_Node *temp = FindMin(root->right);
            root->data = temp->data;
            root->right = Delete(root->right, temp->data);
        }
    }
    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = Balance(root);

    //Left Left Case
    if (balance > 1 && Balance(root->left) >= 0)
        return RightRotate(root);

    // Right Right Case
    if (balance < -1 && Balance(root->right) <= 0)
        return LeftRotate(root);

    // Left Right Case
    if (balance > 1 && Balance(root->left) < 0)
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }

    //Right Left Case
    if (balance < -1 && Balance(root->right) > 0)
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    return root;
}

Tree_Node *Insert(Tree_Node *root, Item_Type data)
{
    if (root == NULL)
        return NewNode(data);

    // else if (data < root->data)
    else if (comparator(data, root->data) == 1)
        root->left = Insert(root->left, data);

    // else if (data > root->data)
    else if (comparator(data, root->data) == -1)
        root->right = Insert(root->right, data);

    else
        return root;

    root->height = max(height(root->left), height(root->right)) + 1;

    int balance = Balance(root);

    // Left Left Case
    // if (balance > 1 && data < root->left->data)
    if (balance > 1 && comparator(data, root->left->data) == 1)
        return RightRotate(root);

    // Right Right Case
    // if (balance < -1 && data > root->right->data)
    if (balance < -1 && comparator(data, root->right->data) == -1)
        return LeftRotate(root);

    //Left Right Case
    // if (balance > 1 && data > root->left->data)
    if (balance > 1 && comparator(data, root->left->data) == -1)
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }

    // Right Left Case
    // if (balance < -1 && data < root->right->data)
    if (balance < -1 && comparator(data, root->right->data) == 1)
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }

    return root;
}

// inorder traversal of the tree
void inorder(Tree_Node *root1)
{
    if (root1)
    {
        inorder(root1->left);
        printf("\n%20s\t%s ", root1->data.name, root1->data.rollno);
        inorder(root1->right);
    }
}

//

void readStudent(Item_Type *s, FILE *fp)
{
    fscanf(fp, "%s %s %d %s", s->name, s->dep, &s->year, s->rollno);
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%d", &s->classesConducted[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%d", &s->classesAttended[i]);
    }
    fscanf(fp, "%d %d", &s->fee, &s->app);
}
Tree_Node *inputFromFile(char *filename, Details student, Tree_Node *head)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp))
    {
        readStudent(&student, fp);
        student.year = student.year - 1; // TO USE ENUM LOGIC

        filling_MCC(student);
        head = Insert(head, student);
    }

    fclose(fp);
    return head;
}
Tree_Node *takeInput()
{
    Tree_Node *head = NULL;
    Details d;
    head = inputFromFile("sample.txt", d, head);
    return head;
}

//

//

void printFeeStatus(Tree_Node *root)
{
    if (root)
    {
        printFeeStatus(root->left);
        printf("\nFee Status is %d for %s ", root->ptr_To_Fee_Status_List_Node->fee, root->ptr_To_Fee_Status_List_Node->roll);
        printFeeStatus(root->right);
    }
}

void printAttendance(Tree_Node *root)
{
    if (root)
    {
        printAttendance(root->left);
        printf("\n%20s\t%s\t%f", root->data.name, root->ptr_To_Attendance_List_Node->rollno, root->ptr_To_Attendance_List_Node->attendance_Percent);
        printAttendance(root->right);
    }
}

void connector(Tree_Node *root)
{
    if (root)
    {
        connector(root->right);
        connector(root->left);
        if (root->left == NULL && root->right == NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = root->ptr_To_Fee_Status_List_Node->right = NULL;

            root->ptr_To_Applicant_Status_List_Node->left = root->ptr_To_Applicant_Status_List_Node->right = NULL;

            root->ptr_To_Attendance_List_Node->left = root->ptr_To_Attendance_List_Node->right = NULL;
        }
        else if (root->left != NULL && root->right == NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = root->left->ptr_To_Fee_Status_List_Node;

            root->ptr_To_Applicant_Status_List_Node->left = root->left->ptr_To_Applicant_Status_List_Node;

            root->ptr_To_Attendance_List_Node->left = root->left->ptr_To_Attendance_List_Node;

            //

            root->ptr_To_Fee_Status_List_Node->right = NULL;

            root->ptr_To_Applicant_Status_List_Node->right = NULL;

            root->ptr_To_Attendance_List_Node->right = NULL;
        }
        else if (root->left == NULL && root->right != NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = NULL;

            root->ptr_To_Applicant_Status_List_Node->left = NULL;

            root->ptr_To_Attendance_List_Node->left = NULL;

            //

            root->ptr_To_Fee_Status_List_Node->right = root->right->ptr_To_Fee_Status_List_Node;

            root->ptr_To_Applicant_Status_List_Node->right = root->right->ptr_To_Applicant_Status_List_Node;

            root->ptr_To_Attendance_List_Node->right = root->right->ptr_To_Attendance_List_Node;
        }
        else
        {
            root->ptr_To_Fee_Status_List_Node->left = root->left->ptr_To_Fee_Status_List_Node;

            root->ptr_To_Applicant_Status_List_Node->left = root->left->ptr_To_Applicant_Status_List_Node;

            root->ptr_To_Attendance_List_Node->left = root->left->ptr_To_Attendance_List_Node;

            //

            root->ptr_To_Fee_Status_List_Node->right = root->right->ptr_To_Fee_Status_List_Node;

            root->ptr_To_Applicant_Status_List_Node->right = root->right->ptr_To_Applicant_Status_List_Node;

            root->ptr_To_Attendance_List_Node->right = root->right->ptr_To_Attendance_List_Node;
        }
    }
}

void fillAttendance(Tree_Node *root)
{
    if (root)
    {
        float att_percent = -1;

        int row = root->data.year;
        int col = -1;
        // char c = ptr->student_Data.dep[0];
        char c = root->data.dep[0];
        switch (c)
        {
        case 'C': //CS or CV
            if (root->data.dep[1] == 'S')
                col = 0;
            else
                col = 1;
            break;
        case 'E':
            if (root->data.dep[1] == 'C')
                col = 2;
            else
                col = 3;
            break;
        case 'M':
            col = 4;
            break;
        default:
            printf("\nWrong character is %c", c);
            printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
            break;
        }
        for (int i = 0; i < SUBJECT_SIZE; i++)
        {
            // if (MCC[row][col] == ptr->student_Data.classesConducted[i])
            if (MCC[row][col] == root->data.classesConducted[i])
            {
                // float att_For_Current_Subject = (ptr->student_Data.classesAttended[i] * 1.0) / (MCA[row][col]) * 100.0;
                float att_For_Current_Subject = (root->data.classesAttended[i] * 1.0) / (MCA[row][col]) * 100.0;

                if (att_percent < att_For_Current_Subject && att_percent <= 100)
                {
                    att_percent = att_For_Current_Subject;
                }
            }
        }
        root->ptr_To_Attendance_List_Node->attendance_Percent = att_percent;
        fillAttendance(root->left);
        fillAttendance(root->right);
    }
}
//

//

void Question1(Tree_Node *root);
void Question2(Tree_Node *root);
void Question3(Tree_Node *root);
void Question4(Tree_Node *root);
Tree_Node *Question5(Tree_Node *root);
void Question6(Tree_Node *root);
void Question7(Tree_Node *root);
void Question8(Tree_Node *root);

//

void printApplicantList(ApplicantStatus_List_Node *root)
{
    if (root)
    {
        printApplicantList(root->left);
        printf("\n%20s\t%s\tApply=%d", root->name, root->roll, root->apply);
        printApplicantList(root->right);
    }
}
//

//
int main()
{
    Tree_Node *root_Of_Records = NULL;
    root_Of_Records = takeInput();

    // printf("\nInorder");
    // inorder(root_Of_Records);
    // printf("\nBefore connecting\n");
    connector(root_Of_Records);
    // printf("\nAfter connecting\n");
    // printApplicantList(root_Of_Records->ptr_To_Applicant_Status_List_Node);

    printf("\nPrinting MCC\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("\t %d", MCC[i][j]);
        }
        printf("\n");
    }
    printf("\nPrinting MCA\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("\t %d", MCA[i][j]);
        }
        printf("\n");
    }
    fillAttendance(root_Of_Records);

    int choice = 0;

    while (choice != 9)
    {
        printf("\n1:Q1\n2:Q2\n3:Q3\n4:Q4\n5:Delete\n6:Q6\n7:Q7\n8:Range search\n9:Exit\n");
        printf("\nEnter choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            Question1(root_Of_Records);

            break;
        case 2:
            Question2(root_Of_Records);

            break;
        case 3:
            Question3(root_Of_Records);

            break;
        case 4:
            Question4(root_Of_Records);

            break;
        case 5:
            root_Of_Records = Question5(root_Of_Records);

            break;
        case 6:
            Question6(root_Of_Records);

            break;
        case 7:
            Question7(root_Of_Records);

            break;
        case 8:
            Question8(root_Of_Records);

            break;
        case 9:

            break;

        default:
            printf("\n No such choice, enter again\n");
            break;
        }
    }

    // Question1(root_Of_Records);
    // Question2(root_Of_Records);
    // Question3(root_Of_Records);
    // Question4(root_Of_Records);
    // printf("\nPreorder");
    // preorder(root_Of_Records);
    // printf("\nInorder");
    // inorder(root_Of_Records);
    // root_Of_Records = Question5(root_Of_Records);
    // connector(root_Of_Records);
    // inorder(root_Of_Records);
    // Question6(root_Of_Records);
    // Question7(root_Of_Records);
    // Question8(root_Of_Records);

    return 0;
}

//Question 1
void Question1util(Tree_Node *root)
{
    if (root)
    {
        Question1util(root->left);
        printf("\n%20s\t%s\t%d\t%s ", root->data.name, root->data.dep, root->data.year + 1, root->data.rollno);
        Question1util(root->right);
    }
}
void Question1(Tree_Node *root)
{
    printf("\nQuestion 1");
    Question1util(root);
}

void Question2util(Tree_Node *root)
{
    if (root)
    {
        Question2util(root->left);
        if (root->ptr_To_Applicant_Status_List_Node->apply == NOTAPPLIED)
        {
            printf("\n%20s \t%s\tApplicant Status-> %d", root->ptr_To_Applicant_Status_List_Node->name, root->ptr_To_Applicant_Status_List_Node->roll, root->ptr_To_Applicant_Status_List_Node->apply);
        }
        Question2util(root->right);
    }
}
void Question2(Tree_Node *root)
{
    printf("\nQuestion 2");
    Question2util(root);
}

void Question3util(Tree_Node *root)
{
    if (root)
    {
        Question3util(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent > 75 && root->ptr_To_Fee_Status_List_Node->fee == APPLIED && root->ptr_To_Applicant_Status_List_Node->apply == APPLIED)
        {
            printf("\n%20s\t%s\t%f\tFee Status=>%d\tApplicant=%d", root->data.name, root->ptr_To_Attendance_List_Node->rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee, root->ptr_To_Applicant_Status_List_Node->apply);
        }
        Question3util(root->right);
    }
}
void Question3(Tree_Node *root)
{
    printf("\nQuestion 3");
    Question3util(root);
}

void Question4util(Tree_Node *root)
{
    if (root)
    {
        Question4util(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent <= 75)
        {
            printf("\n%20s\t%s\t%f", root->data.name, root->ptr_To_Attendance_List_Node->rollno, root->ptr_To_Attendance_List_Node->attendance_Percent);
        }
        Question4util(root->right);
    }
}
void Question4(Tree_Node *root)
{
    printf("\nQuestion 4");
    Question4util(root);
}

Tree_Node *Question5util(Tree_Node *root, char *key)
{
    if (root == NULL)
        return root;

    // if (data < root->data)
    if (strcmp(key, root->data.rollno) < 0)
        root->left = Question5util(root->left, key);
    // else if (data > root->data)
    else if (strcmp(key, root->data.rollno) > 0)
        root->right = Question5util(root->right, key);

    else
    {
        if (root->right == NULL && root->left == NULL)
        {
            free(root);
            root = NULL;
        }

        else if (root->left != NULL && root->right == NULL)
        {
            Tree_Node *temp = root->left;
            root = root->left;
            free(temp);
        }

        else if (root->right != NULL && root->left == NULL)
        {
            Tree_Node *temp = root->right;
            root = root->right;
            free(temp);
        }

        else
        {
            Tree_Node *temp = FindMin(root->right);
            root->data = temp->data;
            root->right = Question5util(root->right, temp->data.rollno);
        }
    }
    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = Balance(root);

    //Left Left Case
    if (balance > 1 && Balance(root->left) >= 0)
        return RightRotate(root);

    // Right Right Case
    if (balance < -1 && Balance(root->right) <= 0)
        return LeftRotate(root);

    // Left Right Case
    if (balance > 1 && Balance(root->left) < 0)
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }

    //Right Left Case
    if (balance < -1 && Balance(root->right) > 0)
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    return root;
}
Tree_Node *Question5(Tree_Node *root)
{
    printf("\nQuestion 5");

    char name_Question5[ROLL_SIZE];
    printf("\nEnter roll no for deletion\n");
    scanf("%s", name_Question5);

    printf("\nBefore Deleting");
    Question1(root);
    root = Question5util(root, name_Question5);
    printf("\nAfter Deleting");
    Question1(root);
    connector(root);
    return root;
}

void Question6util(Tree_Node *root)
{
    if (root)
    {
        Question6util(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent > 75 && root->ptr_To_Fee_Status_List_Node->fee == NOTAPPLIED)
        {
            printf("\n%20s\t%s\t%f\tFee Status=>%d", root->data.name, root->ptr_To_Attendance_List_Node->rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee);
        }
        Question6util(root->right);
    }
}
void Question6(Tree_Node *root)
{
    printf("\nQuestion 6");
    Question6util(root);
}

int findDepIndex(char *dep)
{
    int c = dep[0], x = -1;
    switch (c)
    {
    case 'C': //CS or CV
        if (dep[1] == 'S')
            x = 0;
        else
            x = 1;
        break;
    case 'E':
        if (dep[1] == 'C')
            x = 2;
        else
            x = 3;
        break;
    case 'M':
        x = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    return x;
}
void print_Dep_With_Most_Defaulters(int count_Of_Defaulters_Department[5])
{
    int maxcount = count_Of_Defaulters_Department[0];
    int idx = 0;
    for (int i = 1; i < 5; i++)
    {
        if (maxcount < count_Of_Defaulters_Department[i])
        {
            maxcount = count_Of_Defaulters_Department[i];
            idx = i;
        }
    }
    printf("\nMost Defaulters From :->");
    switch (idx)
    {
    case 0:
        printf("\tCS");
        break;
    case 1:
        printf("\tCV");
        break;
    case 2:
        printf("\tEC");
        break;
    case 3:
        printf("\tEE");
        break;
    case 4:
        printf("\tME");
        break;

    default:
        printf("\t Error dep doesnt exist");
        break;
    }
}
void Question7util(Tree_Node *root, int count_department_wise[5])
{
    if (root)
    {
        Question7util(root->left, count_department_wise);
        if (root->ptr_To_Fee_Status_List_Node->fee == PENDING || root->ptr_To_Attendance_List_Node->attendance_Percent <= 75)
        {
            printf("\n%20s\t%s\t%s\tAttendance = %f\tFeeStatus = %d", root->data.name, root->data.dep, root->data.rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee);
            int dep_Index = findDepIndex(root->data.dep);
            count_department_wise[dep_Index]++;
        }
        Question7util(root->right, count_department_wise);
    }
}
void Question7(Tree_Node *root)
{
    printf("\nQuestion 7");
    int count_department_wise[5] = {0};
    Question7util(root, count_department_wise);
    printf("\n");
    for (int i = 0; i < SUBJECT_SIZE; i++)
    {
        printf("\tDEP_%d = %d", i, count_department_wise[i]);
    }
    print_Dep_With_Most_Defaulters(count_department_wise);
}

void Question8util(Tree_Node *root, char *key1, char *key2)
{
    if (root)
    {
        if (strcmp(root->data.rollno, key1) < 0)
        {
            Question8util(root->right, key1, key2);
        }
        else if (strcmp(root->data.rollno, key1) == 0)
        {
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
            Question8util(root->right, key1, key2);
        }
        else if (strcmp(root->data.rollno, key1) > 0 && strcmp(root->data.rollno, key2) < 0)
        {
            Question8util(root->left, key1, key2);
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
            Question8util(root->right, key1, key2);
        }
        else if (strcmp(root->data.rollno, key2) == 0)
        {
            Question8util(root->left, key1, key2);
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
        }
        else
        {
            Question8util(root->left, key1, key2);
        }
    }
}
void Question8(Tree_Node *root)
{
    printf("\nQuestion 8");

    char key1[ROLL_SIZE], key2[ROLL_SIZE];
    printf("\nEnter starting roll no for question 8\n");
    scanf("%s", key1);
    printf("\nEnter ending roll no for question 8\n");
    scanf("%s", key2);

    printf("\nstr1=%s str2=%s\n", key1, key2);
    Question8util(root, key1, key2);
}