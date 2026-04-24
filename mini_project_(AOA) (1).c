#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 30
#define MAX_STR 120

typedef struct {
    int id;
    char text[MAX_STR];
    int marks;
    int difficulty; // 1-Easy,2-Medium,3-Hard
    char topic[30];
} Question;

Question bank[MAX];
int n;

// ---------- MERGE SORT ----------
void merge(Question arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1, n2 = r - m;

    Question L[n1], R[n2];

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = j = 0; k = l;

    while (i < n1 && j < n2) {
        if (L[i].difficulty <= R[j].difficulty)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(Question arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// ---------- LCS (SAFE VERSION) ----------
int max(int a, int b) { return (a > b) ? a : b; }

int LCS(char *X, char *Y) {
    int m = strlen(X), n = strlen(Y);
    int L[m+1][n+1];

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i-1] == Y[j-1])
                L[i][j] = 1 + L[i-1][j-1];
            else
                L[i][j] = max(L[i-1][j], L[i][j-1]);
        }
    }
    return L[m][n];
}

// ---------- VALIDATION (RELAXED) ----------
int isValid(int selected[], int count, int index) {
    for (int i = 0; i < count; i++) {
        // relaxed threshold (only block near-duplicate questions)
        if (LCS(bank[selected[i]].text, bank[index].text) > 20)
            return 0;
    }
    return 1;
}

// ---------- BACKTRACKING ----------
int found = 0;

void printPaper(int selected[], int count) {
    printf("\n=====================================\n");
    printf("        FINAL QUESTION PAPER\n");
    printf("=====================================\n");

    printf("\nSection A (Easy):\n");
    for (int i = 0; i < count; i++)
        if (bank[selected[i]].difficulty == 1)
            printf("Q%d: %s (%d)\n", bank[selected[i]].id, bank[selected[i]].text, bank[selected[i]].marks);

    printf("\nSection B (Medium):\n");
    for (int i = 0; i < count; i++)
        if (bank[selected[i]].difficulty == 2)
            printf("Q%d: %s (%d)\n", bank[selected[i]].id, bank[selected[i]].text, bank[selected[i]].marks);

    printf("\nSection C (Hard):\n");
    for (int i = 0; i < count; i++)
        if (bank[selected[i]].difficulty == 3)
            printf("Q%d: %s (%d)\n", bank[selected[i]].id, bank[selected[i]].text, bank[selected[i]].marks);

    printf("\n=====================================\n");
}

void generatePaper(int index, int selected[], int count,
                   int totalMarks, int target) {

    if (found) return;

    if (totalMarks == target) {
        found = 1;
        printPaper(selected, count);
        return;
    }

    if (index >= n || totalMarks > target) return;

    if (isValid(selected, count, index)) {
        selected[count] = index;
        generatePaper(index + 1, selected, count + 1,
                      totalMarks + bank[index].marks, target);
    }

    generatePaper(index + 1, selected, count,
                  totalMarks, target);
}

// ---------- FALLBACK (GUARANTEED OUTPUT) ----------
void fallback(int target) {
    printf("\n[Fallback Mode Activated]\n");

    int sum = 0;
    printf("\nSimple Generated Paper:\n");

    for (int i = 0; i < n; i++) {
        if (sum + bank[i].marks <= target) {
            printf("Q%d: %s (%d)\n", bank[i].id, bank[i].text, bank[i].marks);
            sum += bank[i].marks;
        }
        if (sum == target) break;
    }
}

// ---------- INPUT ----------
void inputQuestions() {
    printf("Enter number of questions: ");
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; i++) {
        bank[i].id = i + 1;

        printf("\nEnter question: ");
        scanf(" %[^\n]", bank[i].text);

        printf("Marks: ");
        scanf("%d", &bank[i].marks);
        getchar();

        printf("Difficulty (1-Easy,2-Medium,3-Hard): ");ˀ
        scanf("%d", &bank[i].difficulty);
        getchar();

        printf("Topic: ");
        scanf(" %[^\n]", bank[i].topic);
    }
}

// ---------- GENERATE ----------
void generate() {
    int totalMarks;
    printf("\nEnter total exam marks: ");
    scanf("%d", &totalMarks);

    mergeSort(bank, 0, n - 1);

    int selected[MAX];
    found = 0;

    generatePaper(0, selected, 0, 0, totalMarks);

    if (!found) {
        fallback(totalMarks);
    }
}

// ---------- MENU ----------
void menu() {
    int choice;

    while (1) {
        printf("\n====== MENU ======\n");
        printf("1. Enter Questions\n");
        printf("2. Generate Paper\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: inputQuestions(); break;
            case 2: generate(); break;
            case 3: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}

// ---------- MAIN ----------
int main() {
    srand(time(0));
    menu();
    return 0;
}
