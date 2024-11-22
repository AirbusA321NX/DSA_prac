#include <stdio.h>
#include <stdlib.h>

#define SIZE 100  // For array implementation

// Node structure for singly linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Node structure for doubly linked list
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

// Stack implementation using arrays
typedef struct {
    int arr[SIZE];
    int top;
} StackArray;

// Queue implementation using arrays
typedef struct {
    int arr[SIZE];
    int front;
    int rear;
} QueueArray;

// Stack implementation using singly linked list
typedef struct {
    Node* top;
} StackSLL;

// Queue implementation using singly linked list
typedef struct {
    Node* front;
    Node* rear;
} QueueSLL;

// Stack implementation using doubly linked list
typedef struct {
    DNode* top;
} StackDLL;

// Queue implementation using doubly linked list
typedef struct {
    DNode* front;
    DNode* rear;
} QueueDLL;

// Stack operations (Pop, Push, Traverse, Peek) - using array implementation
int popStackArray(StackArray* stack) {
    if (stack->top < 0) {
        printf("NULL\n");
        return 0;  // Indicates underflow
    } else {
        return stack->arr[stack->top--];
    }
}
void pushStackArray(StackArray* s, int item) {
    if (s->top == SIZE - 1) {
        printf("Stack overflow\n");
        return;
    }
    s->arr[++(s->top)] = item;
}

void traverseStackArray(StackArray* stack) {
    int BS[SIZE], TOSB = 0;  // Backup stack
    int flag = 0;            // Initialize flag
    StackArray tempStack = *stack;  // Create a temporary copy of the stack to avoid modifying the original

    while (1) {
        int info = popStackArray(&tempStack);  // Pop from the temporary stack
        if (info == 0) {  // Stack underflow in temporary stack
            break;
        } else {
            printf("%d ", info);
            BS[TOSB++] = info;  // Backup popped item
            flag = 1;  // Mark that we have popped at least one item
        }
    }
    
    printf("\n");

    // Restore the stack by pushing the items back
    for (int i = TOSB - 1; i >= 0; i--) {
        pushStackArray(stack, BS[i]);
    }
}


int peekStackArray(StackArray* stack, int* pInfo) {
    int BS[SIZE], TOSB = 0;  // Backup stack
    int flag = 0;            // Initialize flag

    while (1) {
        int info = popStackArray(stack);
        if (info == 0) {
            if (flag == 1 && TOSB != 0) {
                for (int i = 0; i < TOSB; i++) {
                    pushStackArray(stack, BS[i]);
                }
            }
            return 0;  // Indicates underflow
        } else if (TOSB == 0) {
            *pInfo = info;  // The top element to peek
            flag = 1;  // Mark that we found the top element
        } else {
            BS[TOSB++] = info;  // Backup popped item
        }
    }
    return flag;  // Return flag indicating success
}

// Queue operations (Traverse, Insert, Delete) - using array implementation
void traverseQueueArray(QueueArray* q) {
    if (q->front == -1) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    for (int i = q->front; i <= q->rear; i++) {
        printf("%d ", q->arr[i]);
    }
    printf("\n");
}

void insertQueueArray(QueueArray* q, int item) {
    if (q->rear == SIZE - 1) {
        printf("Queue is full\n");
        return;
    }
    
    // If the queue is empty
    if (q->front == -1) {
        q->front = 0;
        q->arr[++q->rear] = item;
        return;
    }

    int i;
    // Find the correct position for the item
    for (i = q->rear; i >= q->front && q->arr[i] > item; i--) {
        q->arr[i + 1] = q->arr[i];  // Shift elements to the right
    }

    // Insert the item in the correct position
    q->arr[i + 1] = item;
    q->rear++;
}

int deleteQueueArray(QueueArray* q) {
    if (q->front == -1) {
        printf("Queue is empty\n");
        return -1;
    }
    int deletedItem = q->arr[q->front];
    printf("Deleted item: %d\n", deletedItem);
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;  // Reset the queue
    }
    return deletedItem;
}

// Stack operations (Pop, Push, Traverse, Peek) - using singly linked list
int popStackSLL(StackSLL* stack) {
    if (stack->top == NULL) {
        printf("NULL \n");
        return 0;  // Indicates underflow
    } else {
        Node* temp = stack->top;
        int poppedData = temp->data;
        stack->top = stack->top->next;
        free(temp);
        return poppedData;
    }
}

void pushStackSLL(StackSLL* s, int item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = item;
    newNode->next = s->top;
    s->top = newNode;
}
void traverseStackSLL(StackSLL* stack) {
    Node* temp = stack->top;
    Node* backupStack = NULL;  // Temporary stack to hold popped elements

    if (temp == NULL) {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack elements: ");
    while (temp != NULL) {
        printf("%d ", temp->data);

        // Push the elements into backup stack
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = temp->data;
        newNode->next = backupStack;
        backupStack = newNode;

        temp = temp->next;
    }
    printf("\n");

    // Restore the original stack from the backup stack
    while (backupStack != NULL) {
        pushStackSLL(stack, backupStack->data);  // Push back to the original stack
        Node* toFree = backupStack;
        backupStack = backupStack->next;
        free(toFree);
    }
}


// Queue operations (Traverse, Insert, Delete) - using singly linked list
void traverseQueueSLL(QueueSLL* q) {
    Node* temp = q->front;
    if (temp == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void insertQueueSLL(QueueSLL* q, int item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = item;
    newNode->next = NULL;

    // Case 1: Queue is empty
    if (q->front == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    // Case 2: Insert at the front if it's smaller than the first element
    if (item <= q->front->data) {
        newNode->next = q->front;
        q->front = newNode;
        return;
    }

    // Case 3: Insert in the middle or at the end
    Node* current = q->front;
    Node* previous = NULL;

    // Traverse the list to find the right place to insert
    while (current != NULL && current->data < item) {
        previous = current;
        current = current->next;
    }

    // Insert the new node in its correct position
    newNode->next = current;
    previous->next = newNode;

    // If inserting at the rear, update the rear pointer
    if (current == NULL) {
        q->rear = newNode;
    }
}

int deleteQueueSLL(QueueSLL* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return 0;  // Indicates underflow
    }

    Node* temp = q->front;
    int deletedData = temp->data;  // Store the deleted item's data
    printf("Deleted item: %d\n", deletedData);

    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;  // Queue is now empty, reset rear as well
    }

    free(temp);
    return deletedData;  // Return the deleted item
}

// Stack operations (Pop, Push, Traverse, Peek) - using doubly linked list
int popStackDLL(StackDLL* stack) {
    if (stack->top == NULL) {
        printf("NULL \n");
        return 0;  // Indicates underflow
    } else {
        DNode* temp = stack->top;
        int poppedData = temp->data;
        stack->top = stack->top->next;
        if (stack->top != NULL) {
            stack->top->prev = NULL;  // Update previous link
        }
        free(temp);
        return poppedData;
    }
}

void pushStackDLL(StackDLL* s, int item) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    newNode->data = item;
    newNode->prev = NULL;
    newNode->next = s->top;
    if (s->top != NULL) {
        s->top->prev = newNode;
    }
    s->top = newNode;
}

void traverseStackDLL(StackDLL* stack) {
    DNode* temp = stack->top;
    DNode* backupStack = NULL;  // Temporary stack to hold popped elements

    if (temp == NULL) {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack elements: ");
    while (temp != NULL) {
        printf("%d ", temp->data);

        // Push the elements into backup stack
        DNode* newNode = (DNode*)malloc(sizeof(DNode));
        newNode->data = temp->data;
        newNode->prev = NULL;
        newNode->next = backupStack;
        if (backupStack != NULL) {
            backupStack->prev = newNode;
        }
        backupStack = newNode;

        temp = temp->next;
    }
    printf("\n");

    // Restore the original stack from the backup stack
    while (backupStack != NULL) {
        pushStackDLL(stack, backupStack->data);  // Push back to the original stack
        DNode* toFree = backupStack;
        backupStack = backupStack->next;
        free(toFree);
    }
}


// Queue operations (Traverse, Insert, Delete) - using doubly linked list
void traverseQueueDLL(QueueDLL* q) {
    DNode* temp = q->front;
    if (temp == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void insertQueueDLL(QueueDLL* q, int item) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    newNode->data = item;
    newNode->next = NULL;
    newNode->prev = NULL;

    // Case 1: Queue is empty
    if (q->front == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    // Case 2: Insert at the front if the new item is smaller than the front element
    if (item <= q->front->data) {
        newNode->next = q->front;
        q->front->prev = newNode;
        q->front = newNode;
        return;
    }

    // Case 3: Insert in the middle or at the end
    DNode* current = q->front;

    // Traverse the list to find the correct insertion point
    while (current != NULL && current->data < item) {
        current = current->next;
    }

    // Case 4: Insert at the rear if the current is NULL (item is larger than all existing items)
    if (current == NULL) {
        newNode->prev = q->rear;
        q->rear->next = newNode;
        q->rear = newNode;
    } else {
        // Case 5: Insert in the middle
        newNode->next = current;
        newNode->prev = current->prev;
        if (current->prev != NULL) {
            current->prev->next = newNode;
        }
        current->prev = newNode;
    }
}


int deleteQueueDLL(QueueDLL* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return 0;  // Indicates underflow
    }

    DNode* temp = q->front;
    int deletedData = temp->data;  // Store the deleted item's data
    printf("Deleted item: %d\n", deletedData);

    q->front = q->front->next;

    if (q->front != NULL) {
        q->front->prev = NULL;
    } else {
        q->rear = NULL;  // Queue is now empty, reset rear as well
    }

    free(temp);
    return deletedData;  // Return the deleted item
}

void displayMenu();
void handleUserChoice(int choice, StackArray* sa, QueueArray* qa, 
                      StackSLL* sll, QueueSLL* qll, 
                      StackDLL* dll, QueueDLL* qdll);

// Main switch-case menu
int main() {
    int choice, subChoice, item;
    StackArray sa; // Stack using array
    sa.top = -1;

    QueueArray qa; // Queue using array
    qa.front = qa.rear = -1;

    StackSLL sll; // Stack using singly linked list
    sll.top = NULL;

    QueueSLL qll; // Queue using singly linked list
    qll.front = qll.rear = NULL;

    StackDLL dll; // Stack using doubly linked list
    dll.top = NULL;

    QueueDLL qdll; // Queue using doubly linked list
    qdll.front = qdll.rear = NULL;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 7) break;  // Exit condition

        handleUserChoice(choice, &sa, &qa, &sll, &qll, &dll, &qdll);
    }
    return 0;
}

// Function to display the menu
void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Stack (Array)\n");
    printf("2. Queue (Array)\n");
    printf("3. Stack (Singly Linked List)\n");
    printf("4. Queue (Singly Linked List)\n");
    printf("5. Stack (Doubly Linked List)\n");
    printf("6. Queue (Doubly Linked List)\n");
    printf("7. Exit\n");
}

// Function to handle the user's choice
void handleUserChoice(int choice, StackArray* sa, QueueArray* qa, 
                      StackSLL* sll, QueueSLL* qll, 
                      StackDLL* dll, QueueDLL* qdll) {
    int subChoice, item;

    switch (choice) {
        case 1:  // Stack (Array)
            printf("\n--- Stack (Array) ---\n");
            printf("1. Push\n2. Pop\n3. Traverse\n4. Peek\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to push: ");
                    scanf("%d", &item);
                    pushStackArray(sa, item);
                    break;
                case 2:
                    item = popStackArray(sa);
                    if (item) printf("Popped: %d\n", item);
                    break;
                case 3:
                    traverseStackArray(sa);
                    break;
                case 4:
                    if (peekStackArray(sa, &item)) printf("Top element: %d\n", item);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        case 2:  // Queue (Array)
            printf("\n--- Queue (Array) ---\n");
            printf("1. Insert\n2. Delete\n3. Traverse\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to insert: ");
                    scanf("%d", &item);
                    insertQueueArray(qa, item);
                    break;
                case 2:
                    item = deleteQueueArray(qa);
                    if (item) printf("Deleted: %d\n", item);
                    break;
                case 3:
                    traverseQueueArray(qa);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        case 3:  // Stack (Singly Linked List)
            printf("\n--- Stack (SLL) ---\n");
            printf("1. Push\n2. Pop\n3. Traverse\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to push: ");
                    scanf("%d", &item);
                    pushStackSLL(sll, item);
                    break;
                case 2:
                    item = popStackSLL(sll);
                    if (item) printf("Popped: %d\n", item);
                    break;
                case 3:
                    traverseStackSLL(sll);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        case 4:  // Queue (Singly Linked List)
            printf("\n--- Queue (SLL) ---\n");
            printf("1. Insert\n2. Delete\n3. Traverse\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to insert: ");
                    scanf("%d", &item);
                    insertQueueSLL(qll, item);
                    break;
                case 2:
                    item = deleteQueueSLL(qll);
                    if (item) printf("Deleted: %d\n", item);
                    break;
                case 3:
                    traverseQueueSLL(qll);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        case 5:  // Stack (Doubly Linked List)
            printf("\n--- Stack (DLL) ---\n");
            printf("1. Push\n2. Pop\n3. Traverse\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to push: ");
                    scanf("%d", &item);
                    pushStackDLL(dll, item);
                    break;
                case 2:
                    item = popStackDLL(dll);
                    if (item) printf("Popped: %d\n", item);
                    break;
                case 3:
                    traverseStackDLL(dll);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        case 6:  // Queue (Doubly Linked List)
            printf("\n--- Queue (DLL) ---\n");
            printf("1. Insert\n2. Delete\n3. Traverse\n");
            scanf("%d", &subChoice);
            switch (subChoice) {
                case 1:
                    printf("Enter item to insert: ");
                    scanf("%d", &item);
                    insertQueueDLL(qdll, item);
                    break;
                case 2:
                    item = deleteQueueDLL(qdll);
                    if (item) printf("Deleted: %d\n", item);
                    break;
                case 3:
                    traverseQueueDLL(qdll);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
            break;

        default:
            printf("Invalid choice\n");
            break;
    }
}