import numpy as np

# Điền đường dẫn file input vào đây 🗿
file_path = "matrix.txt"
file_path2 = "matrix2.txt" 

def read_matrix(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        m, n = map(int, lines[0].strip().split())
        matrix = []
        for line in lines[1:]:
            row = list(map(float, line.strip().split()))
            matrix.append(row)
        return matrix

def write_matrix(matrix, file_path):
    with open(file_path, 'w') as file:
        m, n = len(matrix), len(matrix[0])
        file.write(f"{m} {n}\n")
        for row in matrix:
            file.write(" ".join(str(x) for x in row))
            file.write("\n")

def add_matrices(matrix1, matrix2):
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if len(matrix1) != len(matrix2) or len(matrix1[0]) != len(matrix2[0]):
        return None

    result = [[0] * len(matrix1[0]) for _ in range(len(matrix1))]

    for i in range(len(matrix1)):
        for j in range(len(matrix1[0])):
            result[i][j] = matrix1[i][j] + matrix2[i][j]

    return result


def subtract_matrices(matrix1, matrix2):
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if len(matrix1) != len(matrix2) or len(matrix1[0]) != len(matrix2[0]):
        return None

    result = [[0] * len(matrix1[0]) for _ in range(len(matrix1))]

    for i in range(len(matrix1)):
        for j in range(len(matrix1[0])):
            result[i][j] = matrix1[i][j] - matrix2[i][j]

    return result

def multiply_matrices(matrix1, matrix2):
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if len(matrix1[0]) != len(matrix2):
        return None

    result = [[0] * len(matrix2[0]) for _ in range(len(matrix1))]

    for i in range(len(matrix1)):
        for j in range(len(matrix2[0])):
            for k in range(len(matrix2)):
                result[i][j] += matrix1[i][k] * matrix2[k][j]

    return result

def multiply_scalar(matrix, scalar):
    matrix = read_matrix(file_path)
    result = [[0] * len(matrix[0]) for _ in range(len(matrix))]

    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            result[i][j] = matrix[i][j] * scalar

    return result

def gauss_jordan_inverse(matrix):
    # Kiểm tra ma trận vuông
    if len(matrix) != len(matrix[0]):
        return None

    n = len(matrix)
    # Tạo ma trận bổ sung (ma trận đơn vị)
    augmented_matrix = []
    for i in range(n):
        row = []
        for j in range(n):
            if i == j:
                row.append(1)
            else:
                row.append(0)
        augmented_matrix.append(row)

    # Kết hợp ma trận ban đầu và ma trận bổ sung
    for i in range(n):
        matrix[i] += augmented_matrix[i]

    # Áp dụng phương pháp Gauss-Jordan
    for i in range(n):
        
        # Chuyển phần tử chính không thành 1
        pivot = matrix[i][i]
        for j in range(i, n * 2):
            matrix[i][j] /= pivot

        # Đưa các phần tử dưới và trên phần tử chính không về 0
        for j in range(n):
            if j != i:
                ratio = matrix[j][i]
                for k in range(i, n * 2):
                    matrix[j][k] -= ratio * matrix[i][k]

    # Tách ma trận nghịch đảo từ ma trận kết hợp
    inverse_matrix = []
    for i in range(n):
        inverse_matrix.append(matrix[i][n:])

    return inverse_matrix


def calculate_matrix_rank(matrix):
    matrix = read_matrix(file_path)
    num_rows = len(matrix)
    num_cols = len(matrix[0])

    rank = min(num_rows, num_cols)

    for row in range(rank):
        if matrix[row][row] != 0:
            for col in range(row + 1, num_rows):
                coefficient = matrix[col][row] / matrix[row][row]
                for i in range(rank):
                    matrix[col][i] -= coefficient * matrix[row][i]
        else:
            reduce = True
            for i in range(row + 1, num_rows):
                if matrix[i][row] != 0:
                    matrix[row], matrix[i] = matrix[i], matrix[row]
                    reduce = False
                    break
            if reduce:
                rank -= 1
                for i in range(num_rows):
                    matrix[i][row] = matrix[i][rank]

    return rank

def submatrix(matrix, i, j):
    # Trả lại ma trận con bằng cách bỏ hàng i cột j
    return [row[:j] + row[j + 1:] for row in (matrix[:i] + matrix[i + 1:])]

def matrix_determinant(matrix):
    rows = len(matrix)
    det = 1.0

    for k in range(rows):
        for i in range(k + 1, rows):
            factor = matrix[i][k] / matrix[k][k]

            for j in range(k, rows):
                matrix[i][j] -= factor * matrix[k][j]

    for i in range(rows):
        det *= matrix[i][i]

    return det

def eigen(matrix):
    matrix = np.array(matrix)
    eigenvalues, eigenvectors = np.linalg.eig(matrix)
    eigenvalues = eigenvalues.tolist()
    eigenvectors = eigenvectors.tolist()
    return eigenvalues, eigenvectors
