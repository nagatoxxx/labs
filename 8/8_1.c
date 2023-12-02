#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * измененная версия strcspn из исходного кода apple
 */
size_t cspn(const char* s1, const char* s2)
{
    register const char *p, *spanp;
    register char        c, sc;

    for (p = (s1 + 1);;) { // изменение здесь
        c     = *p++;
        spanp = s2;
        do {
            if ((sc = *spanp++) == c) {
                return (s1 > p) ? 0 : (p - 1 - s1); // и здесь
            }
        } while (sc != 0);
    }
}

/*
 * составляет новый многочлен и помещает его в dest
 */
void solve(List* dest, List* l1, List* l2);

int main()
{
    List* l1  = createList();
    List* l2  = createList();
    List* res = createList();

    const char* e1 = "2x^2-4x^5+7";
    const char* e2 = "-5x^2+x^3+x-3x^5";

    parse(l1, e1);
    parse(l2, e2);

    solve(res, l1, l2);

    printf("%s:\n", e1);
    printList(l1);
    putc('\n', stdout);
    printf("%s:\n", e2);
    printList(l2);
    putc('\n', stdout);
    printf("res: \n");
    printList(res);

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
    size_t left = 0;
    char   buf[64];
    memset(buf, 0, 64);

    while (strlen(expression) - left > 0) {
        size_t len = cspn((expression + left), "+-");
        memcpy(buf, (expression + left), len);
        left += len;

        char num[sizeof(buf)]; // строка в которую поммещаются цифры числа
        memset(num, 0, sizeof(buf));

        Element e = {0, 0};
        int isX = (cspn(buf, "x") == strlen(buf)) ? 0 : 1; // фигурирует ли переменная?
        int isPower = (cspn(buf, "^") == strlen(buf)) ? 0 : 1; // указана ли степень одночлена?

        if (isX) {
            int res  = 0;
            int xpos = strcspn(buf, "x");
            strncpy(num, buf, xpos);
            if (strlen(num) == 1 && (num[0] == '-' || num[0] == '+'))
                res = (num[0] == '-') ? -1 : 1;
            else {
                res = atoi(num);
            }

            e.coefficient = res;
            e.exponent    = 1;
            memset(num, 0, sizeof(num));
        }

        if (isPower) {
            int res  = 0;
            int ppos = strcspn(buf, "^");

            strcpy(num, (buf + ppos + 1));

            res        = atoi(num);
            e.exponent = res;
            memset(num, 0, sizeof(num));
        }

        if (!isPower && !isX) {
            int res = atoi(buf);

            e.coefficient = res;
            e.exponent    = 0;
        }

        append(list, e);

        memset(buf, '\0', sizeof(buf));
    }
}

void solve(List* dest, List* l1, List* l2)
{
    //  очень неэффективно
    //  если использовать hashset, то можно сделать быстрее
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
