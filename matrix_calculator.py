from matrix_functions import *
import tkinter as tk
from tkinter import messagebox
import random

def generate_random_matrix():
    rows = entry_rows.get()
    cols = entry_cols.get()

    if not rows or not cols:
        messagebox.showwarning("Cảnh báo", "Vui lòng nhập giá trị cho hàng và cột!")
        return

    if not rows.isdigit() or not cols.isdigit():
        messagebox.showwarning("Cảnh báo", "Giá trị hàng và cột phải là số nguyên dương!")
        return

    rows = int(rows)
    cols = int(cols)
    matrix = [[random.randrange(1, 100) for _ in range(cols)] for _ in range(rows)]
    write_matrix(matrix, file_path)
    messagebox.showinfo("Thông báo", "Đã tạo và lưu ma trận ngẫu nhiên thành công!")

def perform_addition():
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if matrix1 and matrix2:
        result = add_matrices(matrix1, matrix2)
        if result:
            save_matrix_to_file(result, "result.txt")
            messagebox.showinfo("Thông báo", "Đã tính toán và lưu kết quả cộng hai ma trận!")
        else:
            messagebox.showinfo("Thông báo", "Hai ma trận không cùng cỡ!")

def perform_subtraction():
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if matrix1 and matrix2:
        result = subtract_matrices(matrix1, matrix2)

        if result:
            save_matrix_to_file(result, "result.txt")
            messagebox.showinfo("Thông báo", "Đã tính toán và lưu kết quả trừ hai ma trận!")

        else:
            messagebox.showinfo("Thông báo", "Hai ma trận không cùng cỡ!")

def perform_multiplication():
    matrix1 = read_matrix(file_path)
    matrix2 = read_matrix(file_path2)
    if matrix1 and matrix2:
        result = multiply_matrices(matrix1, matrix2)

        if result:
            save_matrix_to_file(result, "result.txt")
            messagebox.showinfo("Thông báo", "Đã tính toán và lưu kết quả nhân hai ma trận!")

        else:
            messagebox.showinfo("Thông báo", "Kích cỡ hai ma trận không thoả mãn!")

def perform_scalar_multiplication():
    matrix = read_matrix(file_path)
    scalar = float(entry_scalar.get())
    if matrix:
        result = multiply_scalar(matrix, scalar)
        save_matrix_to_file(result, "result.txt")
        messagebox.showinfo("Thông báo", "Đã tính toán và lưu kết quả nhân ma trận với số thực!")

def perform_inverse():
    matrix = read_matrix(file_path)
    if matrix:
        try:
            inverse = gauss_jordan_inverse(matrix)

            if inverse:
                save_matrix_to_file(inverse, "result.txt")
                messagebox.showinfo("Thông báo", "Đã tính toán và lưu kết quả ma trận nghịch đảo!")

            else:
                messagebox.showinfo("Lỗi", f"Ma trận không vuông!")

        except ValueError as e:
            messagebox.showerror("Lỗi", str(e))

def perform_rank():
    matrix = read_matrix(file_path)
    if matrix:
        rank = calculate_matrix_rank(matrix)
        messagebox.showinfo("Thông báo", f"Hạng của ma trận là: {rank}")

def perform_determinant():
    matrix = read_matrix(file_path)
    if matrix:

        if len(matrix) != len(matrix[0]):
            messagebox.showinfo("Lỗi", f"Ma trận không vuông!")

        else:
            try:
                determinant = matrix_determinant(matrix)
                messagebox.showinfo("Thông báo", f"Định thức của ma trận là: {determinant}")
                
            except ValueError as e:
                messagebox.showerror("Lỗi", str(e))

def perform_eigen():
    matrix = read_matrix(file_path)
    if matrix:
        try:
            eigenvalues, eigenvectors = eigen(matrix)
            messagebox.showinfo("Thông báo", f"Trị riêng: {eigenvalues}\nVectơ riêng: {eigenvectors}")

        except ValueError as e:
            messagebox.showerror("Lỗi", str(e))

def read_matrix_from_file(file_path):
    try:
        with open(file_path, "r") as file:
            lines = file.readlines()
            matrix = [[float(value) for value in line.strip().split()] for line in lines]
            return matrix

    except IOError:
        return None

def save_matrix_to_file(matrix, file_path):
    if matrix:
        try:
            with open(file_path, "w") as file:
                for row in matrix:
                    file.write(" ".join(str(value) for value in row) + "\n")

        except IOError:
            messagebox.showerror("Lỗi", "Không thể lưu ma trận vào file!")

    else:
        messagebox.showinfo("Thông báo", f"Kiểm tra lại kích cỡ ma trận!")


# Hiển thị menu lên màn hình
root = tk.Tk()
root.title("Menu Điều Khiển")

# Tạo các phần tử trong menu
label_rows = tk.Label(root, text="Số hàng:")
label_cols = tk.Label(root, text="Số cột:")
entry_rows = tk.Entry(root)
entry_cols = tk.Entry(root)
button_generate = tk.Button(root, text="Tạo ma trận ngẫu nhiên", command=generate_random_matrix)
button_addition = tk.Button(root, text="Cộng hai ma trận", command=perform_addition)
button_subtraction = tk.Button(root, text="Trừ hai ma trận", command=perform_subtraction)
button_multiplication = tk.Button(root, text="Nhân hai ma trận", command=perform_multiplication)
button_scalar_multiplication = tk.Button(root, text="Nhân ma trận với số thực", command=perform_scalar_multiplication)
button_inverse = tk.Button(root, text="Tính ma trận nghịch đảo", command=perform_inverse)
button_rank = tk.Button(root, text="Tính hạng của ma trận", command=perform_rank)
button_determinant = tk.Button(root, text="Tính định thức của ma trận", command=perform_determinant)
button_eigen = tk.Button(root, text="Tìm trị riêng và vectơ riêng", command=perform_eigen)
label_scalar = tk.Label(root, text="Số thực:")
entry_scalar = tk.Entry(root)

# Định vị trí các phần tử trong menu
label_rows.grid(padx=10, pady=5)
label_cols.grid(padx=10, pady=5)
entry_rows.grid(row=0, column=1, padx=10, pady=5)
entry_cols.grid(row=1, column=1, padx=10, pady=5)
button_generate.grid(row=2, columnspan=2, padx=10, pady=5)
button_addition.grid(row=4, columnspan=2, padx=10, pady=5)
button_subtraction.grid(row=5, columnspan=2, padx=10, pady=5)
button_multiplication.grid(row=6, columnspan=2, padx=10, pady=5)
label_scalar.grid(row=7, column=0, padx=10, pady=5)
entry_scalar.grid(row=7, column=1, padx=10, pady=5)
button_scalar_multiplication.grid(row=8, columnspan=2, padx=10, pady=5)
button_inverse.grid(row=9, columnspan=2, padx=10, pady=5)
button_rank.grid(row=10, columnspan=2, padx=10, pady=5)
button_determinant.grid(row=11, columnspan=2, padx=10, pady=5)
button_eigen.grid(row=12, columnspan=2, padx=10, pady=5)

root.mainloop()

