#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

typedef struct {
    int m;
    int n;
    double** data;
} Matrix;

void show_message(const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s",
                                               message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

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
//============================================================Phần này là các hàm thuật toán============================================================

void generate_random_matrix(int rows, int cols, const char* file_path) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Không thể mở file!\n");
        return;
    }

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

    // Tạo ma trận được kết hợp bởi ma trận đơn vị và ma trận đầu vào [ma trận đầu vao | ma trận đơn vị]
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

        // Biến đổi phần tử trên hàng chéo chính về 1
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

    // Khởi tạo ma trận mới từ ma trận đầu vào
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            new_matrix[i][j] = matrix->data[i][j];
        }
    }

    // Khử Gauss
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

         // Tìm trị riêng
        *eigenvalue = 0.0;
        for (int i = 0; i < n; i++) {
            *eigenvalue += eigenvector[i] * temp[i];
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

//============================================================Phần này để khởi tạo các nút trên cửa sổ giao diện============================================================
// Đường dẫn tới file chứa ma trận
const char* file_path1 = "/Users/hungbach/Bài tập lớn/matrix.txt";
const char* file_path2 = "/Users/hungbach/Bài tập lớn/matrix2.txt";

// Khai báo các biến cục bộ
Matrix* matrix1;
Matrix* matrix2;
GtkWidget* current_button = NULL;


// Khởi tạo ô để người dùng nhập các giá trị
GtkWidget* entry_rows;
GtkWidget* entry_cols;
GtkWidget* entry_scalar;

// Khởi tạo ô hiển thị ma trận 1 và 2
GtkWidget* matrix1_label;
GtkWidget* matrix2_label;


// Khởi tạo của sổ giao diện
GtkWidget* create_window() {
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Matrix Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    return window;
}

// Các hàm xử lý sự kiện ấn nút để gọi lại các hàm thuật toán tương ứng

void add_button_clicked(GtkWidget *widget, gpointer data) {
    // Đọc ma trận từ file
    Matrix* matrix1 = read_matrix(file_path1);
    Matrix* matrix2 = read_matrix(file_path2);

    Matrix* result = add_matrices(matrix1, matrix2);
    if (result == NULL) {
        show_message("Hai ma trận không cùng kích thước!");
    } else {
        write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
        show_message("Tổng hai ma trận đã được lưu vào file!");
        free_matrix(result);

    }
}

void subtract_button_clicked(GtkWidget *widget, gpointer data) {
    // Đọc ma trận từ file
    Matrix* matrix1 = read_matrix(file_path1);
    Matrix* matrix2 = read_matrix(file_path2);

    Matrix* result = subtract_matrices(matrix1, matrix2);
    if (result == NULL) {
        show_message("Hai ma trận không cùng kích thước!");
    } else {
        write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
        show_message("Hiệu hai ma trận đã được lưu vào file!");

        free_matrix(result);

    }
}

void multiply_button_clicked(GtkWidget *widget, gpointer data) {
    // Đọc ma trận từ file
    Matrix* matrix1 = read_matrix(file_path1);
    Matrix* matrix2 = read_matrix(file_path2);

    Matrix* result = multiply_matrices(matrix1, matrix2);
    if (result == NULL) {
        show_message("Kích thước hai ma trận không phù hợp để nhân!");
    } else {
        write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
        show_message("Tích hai ma trận đã được lưu vào file!");

        free_matrix(result);
    }
}

void inverse_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix1 = read_matrix(file_path1);

    Matrix* result = gauss_jordan_inverse(matrix1);
    if (result == NULL) {
        show_message("Ma trận không vuông!");
    } else {
        write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
        show_message("Ma trận nghịch đảo đã được lưu ra file!");
        
        free_matrix(result);
    }
}

void determinant_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix = read_matrix(file_path1);

    double determinant = matrix_determinant(matrix);
    if (determinant == -1) {
        // Hiển thị hộp thoại thông báo báo lỗi
        show_message("Ma trận không vuông!");
    } else {
        gchar* result_text = g_strdup_printf("Định thức của ma trận: %.0lf", determinant);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_text);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(result_text);
    }
}

void eigenvalue_eigenvector_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix = read_matrix(file_path1);

    int n = matrix->n;
    int m = matrix->m;
    double epsilon = 0.00001;
    int max_iterations = 100;
    double eigenvalue;
    double* eigenvector = (double*)malloc(n * sizeof(double*));

    if (m != n) {
        show_message("Ma trận không vuông!");
    } else {
        find_eigenvalue_and_eigenvector(matrix, epsilon, max_iterations, &eigenvalue, eigenvector);
        // Chuẩn hoá vectơ riêng để biến đổi phần tử cuối cùng của vectơ riêng về 1
        double norm = eigenvector[n-1];
        for (int i = 0; i < n; i++) {
            eigenvector[i] /= norm;
        }
    
        gchar* eigenvalue_text = g_strdup_printf("Trị riêng: %.4lf", eigenvalue);
        gchar* eigenvector_text = g_strdup("Vector riêng tương ứng:\n");
        for (int i = 0; i < n-1; i++) {
            gchar* number_text = g_strdup_printf("%.4lf ", eigenvector[i]);
            eigenvector_text = g_strconcat(eigenvector_text, number_text, NULL);
            g_free(number_text);
        }
        eigenvector_text = g_strconcat(eigenvector_text, "1", NULL);
    
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s\n%s", eigenvalue_text, eigenvector_text);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    
        g_free(eigenvalue_text);
        g_free(eigenvector_text);
        free(eigenvector);
    }
}

void rank_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix = read_matrix(file_path1);

    int rank = calculate_matrix_rank(matrix);
    gchar* result_text = g_strdup_printf("Hạng của ma trận: %d\n", rank);
    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_text);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    g_free(result_text);
}

void multiply_scalar_clicked(GtkWidget *widget, gpointer data) {
    Matrix * matrix = read_matrix(file_path1);
    GtkWidget* get_scalar = GTK_WIDGET(data);
    const gchar* scalar_text = gtk_entry_get_text(GTK_ENTRY(get_scalar));
    char *end_ptr;

    // Chuyển đổi scalar nhập từ người dùng thành giá trị số thực và kiểm tra
    double scalar = strtod(scalar_text, &end_ptr);
    
    if (*end_ptr != '\0' || end_ptr == scalar_text) {
        show_message("Hệ số không hợp lệ!");
        return;
    }
    
    // Kiểm tra nếu số hàng hoặc số cột chưa được nhập
    if (strlen(scalar_text) == 0 || strlen(scalar_text) == 0) {
        show_message("Vui lòng nhập hệ số!");
        return;
    }

    Matrix *result = multiply_scalar(matrix, scalar);
    write_matrix(result, "/Users/hungbach/Bài tập lớn/result.txt");
    show_message("Kết quả nhân ma trận với một số thực đã được lưu vào file!");

    free_matrix(result);    
}

void random_matrix_button_clicked(GtkWidget *widget, gpointer data) {
    // Lấy giá trị số hàng và số cột từ người dùng
    GtkWidget* entry_rows = GTK_WIDGET(data);
    GtkWidget* entry_cols = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "entry_cols"));

    const gchar* rows_text = gtk_entry_get_text(GTK_ENTRY(entry_rows));
    const gchar* cols_text = gtk_entry_get_text(GTK_ENTRY(entry_cols));

    int i;
    for (i = 0; rows_text[i] && cols_text[i] != '\0'; i++) {
        if (!isdigit(rows_text[i]) || !isdigit(cols_text[i])) {
            show_message("Số hàng và số cột phải là số nguyên!");
            return;
        }
    }

    int num_rows = atoi(rows_text);
    int num_cols = atoi(cols_text);

    // Kiểm tra nếu số hàng hoặc số cột chưa được nhập
    if (strlen(rows_text) == 0 || strlen(cols_text) == 0) {
        show_message("Vui lòng nhập số hàng và số cột!");
        return;
    }

    const char* file_path = "/Users/hungbach/Bài tập lớn/matrix.txt";

    // Tạo ma trận ngẫu nhiên
    generate_random_matrix(num_rows, num_cols, file_path);

    // Giải phóng bộ nhớ của ma trận cũ (nếu có)
    if (matrix1 != NULL) {
        free_matrix(matrix1);
    }

    // Đọc ma trận mới từ file
    matrix1 = read_matrix(file_path);

    // Hiển thị hộp thoại thông báo thành công
    show_message("Ma trận ngẫu nhiên đã được lưu ra file!");
}

// In ma trận ra hộp thoại thông báo trên màn hình
void display_matrix_dialog(const gchar* title, Matrix* matrix) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(title, NULL, GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_NONE,
                                                    NULL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_NONE);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    // Tạo khỏng cách giữa các hàng và cột khi hiển thị ma trận
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    for (int i = 0; i < matrix->m; i++) {
        for (int j = 0; j < matrix->n; j++) {
            gchar number_text[50];
            g_snprintf(number_text, sizeof(number_text), "%.2lf", matrix->data[i][j]);

            GtkWidget *label = gtk_label_new(number_text);
            gtk_grid_attach(GTK_GRID(grid), label, j, i, 1, 1);
        }
    }

    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void print_matrix1_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix = read_matrix(file_path1);
    if (matrix == NULL) {
        show_message("Lỗi không thể đọc ma trận từ file.");
        return;
    }

    display_matrix_dialog("Ma trận 1", matrix);
    free_matrix(matrix);
}

void print_matrix2_button_clicked(GtkWidget *widget, gpointer data) {
    Matrix* matrix = read_matrix(file_path2);
    if (matrix == NULL) {
        show_message("Lỗi không thể đọc ma trận từ file.");
        return;
    }

    display_matrix_dialog("Ma trận 2", matrix);
    free_matrix(matrix);
}
gboolean on_key_press(GtkWidget* widget, GdkEventKey* event, gpointer data) {
    if (event->type == GDK_KEY_PRESS && current_button != NULL) {
        if (event->keyval == GDK_KEY_Up) {
            GtkWidget* prev_button = GTK_WIDGET(g_object_get_data(G_OBJECT(current_button), "prev_button"));
            if (prev_button != NULL) {
                gtk_widget_grab_focus(prev_button);
            }
        }else if(event->keyval == GDK_KEY_Down) {
            GtkWidget* next_button = GTK_WIDGET(g_object_get_data(G_OBJECT(current_button), "next_button"));
            if (next_button != NULL) {
                gtk_widget_grab_focus(next_button);
            }
        }
    }

    // Tiếp tục xử lý các sự kiện khác (nếu có)
    return FALSE;
}

void create_menu(GtkWidget* window, Matrix* matrix1, Matrix* matrix2) {
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* button1 = gtk_button_new_with_label("Button 1");
    GtkWidget* button2 = gtk_button_new_with_label("Button 2");
    g_object_set_data(G_OBJECT(button1), "next_button", button2);
    g_object_set_data(G_OBJECT(button2), "prev_button", button1);
   
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    GtkWidget* matrix1_label = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), matrix1_label, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(window), "matrix1_label", matrix1_label);

    GtkWidget* matrix2_label = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), matrix2_label, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(window), "matrix2_label", matrix2_label);

    GtkWidget* label = gtk_label_new("Chọn một yêu cầu thực hiện:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 10);

    Matrix** matrices = (Matrix**)malloc(2 * sizeof(Matrix*));
    matrices[0] = matrix1;
    matrices[1] = matrix2;

    int num_rows = 0;
    int num_cols = 0;


    // Khởi tạo các nút bấm trên giao diện GTK
    GtkWidget* button_print_matrix1 = gtk_button_new_with_label("1. In ra ma trận 1");
    g_signal_connect(button_print_matrix1, "clicked", G_CALLBACK(print_matrix1_button_clicked), matrix1);
    gtk_box_pack_start(GTK_BOX(vbox), button_print_matrix1, FALSE, FALSE, 5);

    GtkWidget* button_print_matrix2 = gtk_button_new_with_label("2. In ra ma trận 2");
    g_signal_connect(button_print_matrix2, "clicked", G_CALLBACK(print_matrix2_button_clicked), matrix2);
    gtk_box_pack_start(GTK_BOX(vbox), button_print_matrix2, FALSE, FALSE, 5);

    GtkWidget* button_add = gtk_button_new_with_label("3. Tính tổng hai ma trận");
    g_signal_connect(button_add, "clicked", G_CALLBACK(add_button_clicked), matrices);
    gtk_box_pack_start(GTK_BOX(vbox), button_add, FALSE, FALSE, 5);

    GtkWidget* button_subtract = gtk_button_new_with_label("4. Tính hiệu hai ma trận");
    g_signal_connect(button_subtract, "clicked", G_CALLBACK(subtract_button_clicked), matrices);
    gtk_box_pack_start(GTK_BOX(vbox), button_subtract, FALSE, FALSE, 5);

    GtkWidget* button_multiply = gtk_button_new_with_label("5. Tính tích hai ma trận");
    g_signal_connect(button_multiply, "clicked", G_CALLBACK(multiply_button_clicked), matrices);
    gtk_box_pack_start(GTK_BOX(vbox), button_multiply, FALSE, FALSE, 5);

    // Tạo ô để người dùng nhập số thực vào và nút nhân ma trận với một số thực
    GtkWidget* scalar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scalar_box, FALSE, FALSE, 0);

    GtkWidget* label_scalar = gtk_label_new("Số thực:");
    gtk_box_pack_start(GTK_BOX(scalar_box), label_scalar, FALSE, FALSE, 5);

    GtkWidget* entry_scalar = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_scalar), "");
    gtk_box_pack_start(GTK_BOX(scalar_box), entry_scalar, FALSE, FALSE, 5);

    GtkWidget* button_multiply_scalar = gtk_button_new_with_label("6. Nhân ma trận với một số thực");
    g_signal_connect(button_multiply_scalar, "clicked", G_CALLBACK(multiply_scalar_clicked), entry_scalar);
    gtk_box_pack_start(GTK_BOX(vbox), button_multiply_scalar, FALSE, FALSE, 5);

    GtkWidget* button_inverse = gtk_button_new_with_label("7. Tìm ma trận nghịch đảo");
    g_signal_connect(button_inverse, "clicked", G_CALLBACK(inverse_button_clicked), matrices);
    gtk_box_pack_start(GTK_BOX(vbox), button_inverse, FALSE, FALSE, 5);

    GtkWidget* button_determinant = gtk_button_new_with_label("8. Tính định thức ma trận");
    g_signal_connect(button_determinant, "clicked", G_CALLBACK(determinant_button_clicked), matrix1);
    gtk_box_pack_start(GTK_BOX(vbox), button_determinant, FALSE, FALSE, 5);

    GtkWidget* button_rank = gtk_button_new_with_label("9. Tính hạng của ma trận");
    g_signal_connect(button_rank, "clicked", G_CALLBACK(rank_button_clicked), matrix1);
    gtk_box_pack_start(GTK_BOX(vbox), button_rank, FALSE, FALSE, 5);

    // Tạo ô để người dùng nhập số hàng với số cột vào
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget* label_rows = gtk_label_new("Số hàng:");
    gtk_box_pack_start(GTK_BOX(hbox), label_rows, FALSE, FALSE, 5);

    entry_rows = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_rows), "");
    gtk_box_pack_start(GTK_BOX(hbox), entry_rows, FALSE, FALSE, 5);

    GtkWidget* label_cols = gtk_label_new("Số cột:");
    gtk_box_pack_start(GTK_BOX(hbox), label_cols, FALSE, FALSE, 5);

    entry_cols = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_cols), "");
    gtk_box_pack_start(GTK_BOX(hbox), entry_cols, FALSE, FALSE, 5);

    // Tạo nút "Tạo ma trận ngẫu nhiên"
    GtkWidget* button_random_matrix = gtk_button_new_with_label("10. Tạo ma trận ngẫu nhiên");
    g_signal_connect(button_random_matrix, "clicked", G_CALLBACK(random_matrix_button_clicked), entry_rows);
    g_object_set_data(G_OBJECT(button_random_matrix), "entry_cols", entry_cols);
    gtk_box_pack_start(GTK_BOX(vbox), button_random_matrix, FALSE, FALSE, 5);


    GtkWidget* button_eigenvalue_eigenvector = gtk_button_new_with_label("11. Tìm trị riêng và vector riêng");
    g_signal_connect(button_eigenvalue_eigenvector, "clicked", G_CALLBACK(eigenvalue_eigenvector_button_clicked), matrix1);
    gtk_box_pack_start(GTK_BOX(vbox), button_eigenvalue_eigenvector, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);
}


int main(int argc, char* argv[]) {
    // Khởi tạo
    gtk_init(&argc, &argv);

    // Đọc ma trận từ file
    Matrix* matrix1 = read_matrix(file_path1);
    Matrix* matrix2 = read_matrix(file_path2);

    if (matrix1 == NULL || matrix2 == NULL) {
        show_message("Đọc ma trận không thành công!");
        return 1;
    }

    GtkWidget* window = create_window();
    create_menu(window, matrix1, matrix2);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    // Giải phóng bộ nhớ
    free_matrix(matrix1);
    free_matrix(matrix2);

    return 0;
}
