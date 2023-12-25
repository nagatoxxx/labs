#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define readString(buffer, size)                  \
    do {                                          \
        fgets(buffer, size, stdin);               \
        size_t len = strlen(buffer);              \
        if (len > 0 && buffer[len - 1] == '\n') { \
            buffer[len - 1] = '\0';               \
        }                                         \
    } while (0)

typedef struct
{
    int coefficient;
    int exponent;
} Element;

typedef struct
{
    struct Node* next;
    Element      data;
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
static Node* append(List* list, Element element);
/*
 * выводит содержимое списка
 */
static void printList(List* list);

static void parse(List* list, const char* expression);

/*
 * составляет новый многочлен и помещает его в dest
 */
static void solve(List* dest, List* l1, List* l2);

static void listToString(List* list);

int main()
{
    const char* e1 = "x^2-x^5+7+x";
    const char* e2 = "-5x^2+x^3+x-3x^5";

    List* l1  = createList();
    List* l2  = createList();
    List* res = createList();

    parse(l1, e1);
    parse(l2, e2);

    solve(res, l1, l2);

    printf("%s\n", e1);
    printf("%s\n", e2);

    listToString(res);

    freeList(l1);
    freeList(l2);
    freeList(res);

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

static Node* append(List* list, Element element)
{

    Node* new = malloc(sizeof(Node));
    new->next = NULL;
    new->data = element;

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
        printf("%i\t%i\n", current->data.coefficient, current->data.exponent);
        current = (Node*)current->next;
    }
}


static void parse(List* list, const char* expression)
{
    char* copy = strdup(expression);

    // добавляем пробел перед каждым плюсом и минусом
    int len = strlen(copy);
    for (int i = 0; i < len; i++) {
        if (copy[i] == '+' || copy[i] == '-') {
            len += 2;
            copy = realloc(copy, len);
            memmove(copy + i + 1, copy + i, strlen(copy + i));
            copy[i] = ' ';
            i++;
        }
    }

    char* tok = strtok(copy, " ");
    while (tok != NULL) {
        // обработка одночлена

        int isX = (strcspn(tok, "x") == strlen(tok)) ? 0 : 1; // фигурирует ли переменная?
        int isPower = (strcspn(tok, "^") == strlen(tok)) ? 0 : 1; // указана ли степень одночлена?

        int coef = 0;
        int exp  = 0;

        if (isX) {
            char buf[32] = {0};
            int  xpos    = strcspn(tok, "x");
            strncpy(buf, tok, xpos);

            if (strlen(buf) == 0) {
                coef = 1;
            }
            else if (strlen(buf) == 1 && !isdigit(buf[0])) {
                coef = (buf[0] == '+') ? 1 : -1;
            }
            else {
                coef = atoi(buf);
            }
            exp = 1;
        }

        if (isPower) {
            char buf[32] = {0};
            int  ppos    = strcspn(tok, "^");
            strcpy(buf, tok + ppos + 1);

            exp = atoi(buf);
        }

        if (!isPower && !isX) {
            coef = atoi(tok);
        }

        Element e = {coef, exp};
        append(list, e);

        tok = strtok(NULL, " ");
    }

    free(copy);
}

static void solve(List* dest, List* l1, List* l2)
{
    Node* i1 = l1->head;

    while (i1 != NULL) {
        Node* i2 = l2->head;
        while (i2 != NULL) {
            if (i1->data.exponent == i2->data.exponent) {
                Element e = {i1->data.coefficient + i2->data.coefficient,
                             i1->data.exponent};
                append(dest, e);
            }
            i2 = (Node*)i2->next;
        }
        i1 = (Node*)i1->next;
    }
}

static void listToString(List* list)
{
    Node* n = list->head;
    while (n != NULL) {
        if (n->data.coefficient > 0) {
            printf("+");
        }
        printf("%d", n->data.coefficient);

        if (n->data.exponent != 0) {
            printf("x^%d", n->data.exponent);
        }

        n = (Node*)n->next;
    }
    printf("\n");
}
