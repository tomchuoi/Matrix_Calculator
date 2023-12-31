// Đây là phiên bản không có giao diện đồ hoạ của chương trình mà dùng CLI (command-line-interface)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    int m;
    int n;
    double** data;
} Matrix;

Matrix* read_matrix(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Không thể mở file!\n");
        return NULL;
    }

    int m, n;
    fscanf(file, "%d %d", &m, &n);

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            fscanf(file, "%lf", &matrix[i][j]);
        }
    }

    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->m = m;
    result->n = n;
    result->data = matrix;

    fclose(file);

    return result;
}

void generate_random_matrix(int rows, int cols, const char* file_path) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Không thể mở file!\n");
        return;
    }

    srand(time(NULL));

    fprintf(file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double random_number = (double)(rand() % 100 + 1);
            fprintf(file, "%.2lf ", random_number);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void write_matrix(const Matrix* matrix, const char* file_path) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Không thể mở file!\n");
        return;
    }

    fprintf(file, "%d %d\n", matrix->m, matrix->n);
    for (int i = 0; i < matrix->m; i++) {
        for (int j = 0; j < matrix->n; j++) {
            fprintf(file, "%.7lf ", matrix->data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

Matrix* add_matrices(const Matrix* matrix1, const Matrix* matrix2) {
    if (matrix1->m != matrix2->m || matrix1->n != matrix2->n) {
        return NULL;
    }

    int m = matrix1->m;
    int n = matrix1->n;

    double** result_data = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        result_data[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            result_data[i][j] = matrix1->data[i][j] + matrix2->data[i][j];
        }
    }

    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->m = m;
    result->n = n;
    result->data = result_data;

    return result;
}

Matrix* subtract_matrices(const Matrix* matrix1, const Matrix* matrix2) {
    if (matrix1->m != matrix2->m || matrix1->n != matrix2->n) {
        return NULL;
    }

    int m = matrix1->m;
    int n = matrix1->n;

    double** result_data = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        result_data[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            result_data[i][j] = matrix1->data[i][j] - matrix2->data[i][j];
        }
    }

    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->m = m;
    result->n = n;
    result->data = result_data;

    return result;
}

Matrix* multiply_matrices(const Matrix* matrix1, const Matrix* matrix2) {
    if (matrix1->n != matrix2->m) {
        return NULL;
    }

    int m = matrix1->m;
    int n = matrix2->n;
    int p = matrix1->n;

    double** result_data = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        result_data[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            result_data[i][j] = 0;
            for (int k = 0; k < p; k++) {
                result_data[i][j] += matrix1->data[i][k] * matrix2->data[k][j];
            }
        }
    }

    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->m = m;
    result->n = n;
    result->data = result_data;

    return result;
}

Matrix* multiply_scalar(const Matrix* matrix, double scalar) {
    int m = matrix->m;
    int n = matrix->n;

    double** result_data = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        result_data[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            result_data[i][j] = matrix->data[i][j] * scalar;
        }
    }

    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->m = m;
    result->n = n;
    result->data = result_data;

    return result;
}

Matrix* gauss_jordan_inverse(const Matrix* matrix) {
    // Kiểm tra ma trận vuông
    if (matrix->m != matrix->n) {
        return NULL; 
    }

    int n = matrix->n;

    // Tạo ma trận được kết hợp bởi ma trận đơn vị và ma trận đầu vào [ma trận | ma trận đầu vào]
    Matrix* augmented_matrix = (Matrix*)malloc(sizeof(Matrix));
    augmented_matrix->m = n;
    augmented_matrix->n = 2 * n;
    augmented_matrix->data = (double**)malloc(n * sizeof(double*));

    // Bỏ ma trận đầu vào vào ma trận kết hợp
    for (int i = 0; i < n; i++) {
        augmented_matrix->data[i] = (double*)malloc(2 * n * sizeof(double));
        for (int j = 0; j < n; j++) {
            augmented_matrix->data[i][j] = matrix->data[i][j];
        }
        augmented_matrix->data[i][n + i] = 1.0; // Phân tử trên hàng chéo chính của ma trận đơn vị đặt là 1
    }

    // Áp dụng thuật toán Gauss-Jordan
    for (int i = 0; i < n; i++) {
        int pivot_row = i;
     
        // Biến đổi phần tử trên hầng chéo chính về 1
        double pivot = augmented_matrix->data[i][i];
        for (int j = 0; j < 2 * n; j++) {
            augmented_matrix->data[i][j] /= pivot;
        }

        for (int j = 0; j < n; j++) {
            if (j != i) {
                double factor = augmented_matrix->data[j][i];
                for (int k = 0; k < 2 * n; k++) {
                    augmented_matrix->data[j][k] -= factor * augmented_matrix->data[i][k];
                }
            }
        }
    }

    // Tạo ma trận nghịch đảo kích thước nxn
    Matrix* inverse = (Matrix*)malloc(sizeof(Matrix));
    inverse->m = n;
    inverse->n = n;
    inverse->data = (double**)malloc(n * sizeof(double*));

    for (int i = 0; i < n; i++) {
        inverse->data[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            inverse->data[i][j] = augmented_matrix->data[i][n + j];
        }
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < n; i++) {
        free(augmented_matrix->data[i]);
    }
    free(augmented_matrix->data);
    free(augmented_matrix);

    return inverse;
}

int calculate_matrix_rank(const Matrix* matrix) {
    int num_rows = matrix->m;
    int num_cols = matrix->n;

    int rank = (num_rows < num_cols) ? num_rows : num_cols;

    for (int row = 0; row < rank; row++) {
        if (matrix->data[row][row] != 0) {
            for (int col = row + 1; col < num_rows; col++) {
                double coefficient = matrix->data[col][row] / matrix->data[row][row];
                for (int i = 0; i < rank; i++) {
                    matrix->data[col][i] -= coefficient * matrix->data[row][i];
                }
            }
        } else {
            int reduce = 1;
            for (int i = row + 1; i < num_rows; i++) {
                if (matrix->data[i][row] != 0) {
                    double* temp = matrix->data[row];
                    matrix->data[row] = matrix->data[i];
                    matrix->data[i] = temp;
                    reduce = 0;
                    break;
                }
            }
            if (reduce) {
                rank--;
                for (int i = 0; i < num_rows; i++) {
                    matrix->data[i][row] = matrix->data[i][rank];
                }
            }
        }
    }

    return rank;
}

double matrix_determinant(const Matrix* matrix) {
    int rows = matrix->m;
    int cols = matrix->n;

    if (rows != cols) {
        return -1;
    }

    double det = 1.0;
    double new_matrix[rows][rows];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            new_matrix[i][j] = matrix->data[i][j];
        }
    }

    for (int k = 0; k < rows; k++) {
        for (int i = k + 1; i < rows; i++) {
            double factor = new_matrix[i][k] / new_matrix[k][k];

            for (int j = k; j < rows; j++) {
                new_matrix[i][j] -= factor * new_matrix[k][j];
            }
        }
    }

    // Tính định thức
    for (int i = 0; i < rows; i++) {
        det *= new_matrix[i][i];
    }

    return det;
}

void find_eigenvalue_and_eigenvector(const Matrix* matrix, double epsilon, int max_iterations, double* eigenvalue, double* eigenvector) {
    int n = matrix->n;

    // Tạo vectơ riêng
    for (int i = 0; i < n; i++) {
        eigenvector[i] = (double)(rand() % 100 + 1);
    }

    // Chuẩn hoá
    double norm = 0.0;
    for (int i = 0; i < n; i++) {
        norm += eigenvector[i] * eigenvector[i];
    }
    norm = sqrt(norm);
    for (int i = 0; i < n; i++) {
        eigenvector[i] /= norm;
    }

    // Dùng thuật toán phép lặp mũ
    int iterations = 0;
    double prev_eigenvalue;
    do {
        prev_eigenvalue = *eigenvalue;

        // Nhân ma trận vói vectơ riêng
        double* temp = (double*)malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            temp[i] = 0.0;
            for (int j = 0; j < n; j++) {
                temp[i] += matrix->data[i][j] * eigenvector[j];
            }
        }

        // Tính vectơ riêng mới
        norm = 0.0;
        for (int i = 0; i < n; i++) {
            eigenvector[i] = temp[i];
            norm += eigenvector[i] * eigenvector[i];
        }
        norm = sqrt(norm);
        for (int i = 0; i < n; i++) {
            eigenvector[i] /= norm;
        }

        // Tìm trị riêng
        *eigenvalue = 0.0;
        for (int i = 0; i < n; i++) {
            *eigenvalue += eigenvector[i] * temp[i];
        }

        free(temp);

        iterations++;
    } while (fabs(*eigenvalue - prev_eigenvalue) > epsilon && iterations < max_iterations);
}

void free_matrix(Matrix* matrix) {
    for (int i = 0; i < matrix->m; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

int main() {
    // Đường dẫn tới file chứa ma trận
    const char* file_path1 = "matrix.txt";
    const char* file_path2 = "matrix2.txt";

    // Đọc ma trận từ file
    Matrix* matrix1 = read_matrix(file_path1);
    Matrix* matrix2 = read_matrix(file_path2);

    if (matrix1 == NULL || matrix2 == NULL) {
        printf("Đọc ma trận không thành công!\n");
        return 1;
    }

    int choice, rows, cols;
    double scalar;
    double determinant;
    double rank;
    double eigenvalue;
    double* eigenvector = (double*)malloc(matrix1->n * sizeof(double));
    
    do {
        // Hiển thị menu
        printf("\n------------- MENU ----------------\n");
        printf("| 1. Hiển thị ma trận 1             |\n");
        printf("| 2. Hiển thị ma trận 2             |\n");
        printf("| 3. Tính tổng hai ma trận          |\n");
        printf("| 4. Tính hiệu hai ma trận          |\n");
        printf("| 5. Tính tích hai ma trận          |\n");
        printf("| 6. Nhân ma trận với một số        |\n");
        printf("| 7. Tìm ma trận nghịch đảo         |\n");
        printf("| 8. Tính định thức của ma trận     |\n");
        printf("| 9. Tính hạng của ma trận          |\n");
        printf("| 10. Tạo ma trận ngẫu nhiên        |\n");
        printf("| 11. Tìm trị riêng và vectơ riêng  |\n");
        printf("| 0. Thoát                          |\n");
        printf("-------------------------------------\n");
        printf("Chọn một yêu cầu thực hiện: ");

        scanf("%d", &choice);
        
        Matrix* result;

        switch (choice) {
            case 1:
                printf("\nMa trận 1:\n");
                for (int i = 0; i < matrix1->m; i++) {
                    for (int j = 0; j < matrix1->n; j++) {
                        printf("%.2lf ", matrix1->data[i][j]);
                    }
                    printf("\n");
                }
                break;

            case 2:
                printf("\nMa trận 2:\n");
                for (int i = 0; i < matrix2->m; i++) {
                    for (int j = 0; j < matrix2->n; j++) {
                        printf("%.2lf ", matrix2->data[i][j]);
                    }
                    printf("\n");
                }
                break;

            case 3:
                result = add_matrices(matrix1, matrix2);
                if (result == NULL) {
                    printf("Hai ma trận không cùng kích thước!\n");
                } else {
                    write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
                    printf("\nTổng hai ma trận đã được lưu vào file.\n");
                    free_matrix(result);
                }
                break;

            case 4:
                result = subtract_matrices(matrix1, matrix2);
                if (result == NULL) {
                    printf("Hai ma trận không cùng kích thước!\n");
                } else {
                    write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
                    printf("\nHiệu hai ma trận đã được lưu vào file.\n");
                    free_matrix(result);
                }
                break;

            case 5:
                result = multiply_matrices(matrix1, matrix2);
                if (result == NULL) {
                    printf("Kích thước ma trận không phù hợp để nhân!\n");
                } else {
                    write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
                    printf("\nTích hai ma trận đã được lưu vào file.\n");
                    free_matrix(result);
                }
                break;

            case 6:
                printf("\nNhập một số: ");
                scanf("%lf", &scalar);
                result = multiply_scalar(matrix1, scalar);
                write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
                printf("\nMa trận nhân với số đã được lưu vào file.\n");
                free_matrix(result);
                break;

            case 7:
                result = gauss_jordan_inverse(matrix1);
                if (result == NULL) {
                    printf("Ma trận không vuông!\n");
                } else {
                    write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
                    printf("\nMa trận nghịch đảo đã được lưu vào file.\n");
                    free_matrix(result);
                }
                break;

            case 8:
                determinant = matrix_determinant(matrix1);
                if (determinant == -1.0) {
                    printf("\nMa trận không vuông!\n");
                } else {
                    printf("\nĐịnh thức của ma trận: %.2lf\n", determinant);
                }
                break;

            case 9:
                rank = calculate_matrix_rank(matrix1);
                printf("\nHạng của ma trận: %.2lf", rank);
                break;

            case 10:
                printf("\nNhập số hàng: ");
                scanf("%d", &rows);
                printf("Nhập số cột: ");
                scanf("%d", &cols);
                generate_random_matrix(rows, cols, file_path1);
                printf("\nĐã tạo và lưu ma trận ngẫu nhiên thành công!\n");

                // Giải phóng bộ nhớ lưu ma trận cũ và cập nhật ma trận mới
                free_matrix(matrix1);
                matrix1 = read_matrix(file_path1);
                break;

            case 11:
                find_eigenvalue_and_eigenvector(matrix1, 0.0001, 1000, &eigenvalue, eigenvector);
                // Chuẩn hoá để đưa phần tử cuối cùn của vectơ riêng về 1
                double norm = eigenvector[matrix1->n - 1];
                for (int i = 0; i < matrix1->n; i++){
                    eigenvector[i] /= norm; // Chia các phần tử trong vectơ riêng cho phần tử cuối cùng
                }
                printf("\nGiá trị riêng: %.4lf\n", eigenvalue);
                printf("Vectơ riêng tương ứng: ");
                for (int i = 0; i < matrix1->n-1; i++) {
                    printf("%.4lf ", eigenvector[i]);
                }
                printf("1\n");

                free(eigenvector);
                break;
                
            case 0:
                printf("\nThoát chương trình.\n");
                break;
            default:
                printf("\nLựa chọn không hợp lệ!\n");
        }
    } while (choice != 0);

    // Giải phóng bộ nhớ
    free_matrix(matrix1);
    free_matrix(matrix2);

    return 0;
}
