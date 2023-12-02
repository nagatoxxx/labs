#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    struct Node* next;
    char*        data;
} Node;

typedef struct
{
    Node* head;
} List;

/*
 * возвращает указатель на созданный односвязный список
 * использует malloc
 */
static List* createList();
/*
 * освобождает выделенное под список место
 */
static void freeList(List* list);
/*
 * добавляет элемент в конец списка
 */
static Node* append(List* list, const char* data);
/*
 * выводит содержимое списка
 */
static void printList(List* list);
/*
 * парсит text в список
 */
static void parse(List* list, const char* text);
/*
 * перемещает n в начало списка
 */
static void moveFront(List* list, Node* n);
/*
 * перемещает все слова, начинающиеся на гласные буквы, в начало списка
 */
static void solve(List* list);

int main()
{
    char   buf[512];
    size_t i = 0;

    printf("Введите предложение:\n");
    char c;
    // читаем предложение до точки в buf
    while ((c = fgetc(stdin)) != '.') {
        if (c != '\n')
            buf[i++] = c;
    }
    buf[i] = '\0';

    List* l = createList();

    parse(l, buf);

    printf("\nСписок, составленный из предложения:\n");
    printList(l);

    solve(l);
    printf("\nСписок с измененным порядком слов:\n");
    printList(l);

    freeList(l);

    return 0;
}

static List* createList()
{
    List* l = malloc(sizeof(List));

    l->head = NULL;

    return l;
}

static void freeList(List* list)
{
    Node* current = list->head;

    if (current == NULL) {
        return;
    }

    while (current->next != NULL) {
        Node* next = (Node*)current->next;

        free(current);

        current = next;
    }
}

static Node* append(List* list, const char* data)
{

    Node* new = malloc(sizeof(Node));
    new->next = NULL;
    new->data = malloc(strlen(data) + 1); // +1 для '\0'
    strcpy(new->data, data);

    if (list->head == NULL) {
        list->head = new;
    }
    else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = (Node*)current->next;
        }

        current->next = (struct Node*)new;
    }

    return new;
}

static void printList(List* list)
{
    Node* current = list->head;

    while (current != NULL) {
        printf("%s\n", current->data);
        current = (Node*)current->next;
    }
}

static void parse(List* list, const char* text)
{
    char* copy = strdup(text);

    char* token = strtok(copy, " ");

    while (token != NULL) {
        append(list, token);
        token = strtok(NULL, " ");
    }

    free(copy);
}

static void moveFront(List* list, Node* n)
{
    Node* prev = list->head;

    while ((Node*)prev->next != n) {
        prev = (Node*)prev->next;
    }

    prev->next = n->next;

    n->next = (struct Node*)list->head;

    list->head = n;
}

static void solve(List* list)
{
    static const char vowels[] = {'a', 'e', 'o', 'u', 'i', 'y', '\0'};

    Node* current = list->head;

    while (current != NULL) {
        Node* next = (Node*)current->next;
        if (strcspn(current->data, vowels) == 0) {
            moveFront(list, current);
        }

        current = next;
    }
}
