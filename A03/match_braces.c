# include <stdio.h>
# include <stdlib.h>

struct node {
    int row;
    int col;
    struct node *next;
};

void push(struct node** head, int row, int col) {
    struct node* new_node = malloc(sizeof(struct node));
    new_node->row = row;
    new_node->col = col;
    new_node->next = *head;
    *head = new_node;
}

void pop(struct node** head) {
    struct node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

void clear(struct node* head) {
    struct node* current = head;
    while (current != NULL) {
        struct node* next = current->next;
        free(current);
        current = next;
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s \n", argv[0]);
        return 1;
    }

    // open file
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }

    // read file
    char c;
    int row = 1;
    int col = 0;
    struct node* head = NULL;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') { // newline
            row++;
            col = 0;
        } else {
            if (c == '{') { // push open brace
                push(&head, row, col);
            } else if (c == '}') { 
                if (head == NULL) {
                    printf("Unmatched braces on Line %d and  Column %d\n", row, col); // error if no open brace to match close brace
                } else if (c == '}') {
                    printf("Found matching braces: (%d, %d) -> (%d, %d)\n", head->row, head->col, row, col); 
                    pop(&head); // pop close brace
                } 
            }
        }
        col++;
    }
    while (head != NULL) {
        printf("Unmatched braces on Line %d and Column %d\n", head->row, head->col);
        pop(&head);
    }

    // clear stack
    clear(head);

    // close file
    fclose(fp);
    return 0;
}