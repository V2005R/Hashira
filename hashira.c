#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void solveSystem(int n, double A[n][n], double B[n], double X[n]) {
    for (int i = 0; i < n; i++) {
        int max = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(A[j][i]) > fabs(A[max][i])) max = j;
        }
        for (int k = 0; k < n; k++) {
            double tmp = A[i][k];
            A[i][k] = A[max][k];
            A[max][k] = tmp;
        }
        double tmpB = B[i]; B[i] = B[max]; B[max] = tmpB;
        for (int j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            B[j] -= factor * B[i];
            for (int k = i; k < n; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) sum += A[i][j] * X[j];
        X[i] = (B[i] - sum) / A[i][i];
    }
}
int baseToDec(const char *str, int base) {
    return (int)strtol(str, NULL, base);
}
int extractInt(const char *json, const char *key) {
    char *pos = strstr(json, key);
    if (!pos) return -1;
    return atoi(pos + strlen(key) + 2); // skip ": "
}

int main() {
    // Read file into buffer
    FILE *f = fopen("data.json", "r");
    if (!f) { perror("File open failed"); return 1; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *json = malloc(len + 1);
    fread(json, 1, len, f);
    json[len] = '\0';
    fclose(f);

    int n = extractInt(json, "\"n\"");
    int k = extractInt(json, "\"k\"");
    int degree = k - 1;

    double Xs[k], Ys[k];
    int count = 0;

    char *p = json;
    while ((p = strstr(p, "\"base\"")) && count < k) {
        char *q = p;
        while (*q != '{' && q > json) q--;
        while (*q != '"' && q > json) q--;
        char *start = q + 1;
        while (*q != '{' && q > json) q--;
        int x = atoi(start);

        char baseStr[16], valStr[64];
        sscanf(p, "\"base\": \"%[^\"]\", \"value\": \"%[^\"]\"", baseStr, valStr);
        int base = atoi(baseStr);
        int y = baseToDec(valStr, base);

        Xs[count] = x;
        Ys[count] = y;
        count++;
        p++;
    }

    double A[k][k];
    double B[k];
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A[i][j] = pow(Xs[i], degree - j);
        }
        B[i] = Ys[i];
    }

    double coeffs[k];
    solveSystem(k, A, B, coeffs);

    printf("hihest degree poly:\n");
    for (int i = 0; i < k; i++) {
        printf("a[%d] = %lf\n", i, coeffs[i]);
    }
    printf("c value = %lf\n", coeffs[k - 1]);

    free(json);
    return 0;
}
