import serial
import time

def read_from_serial(port='COM6', baud_rate=115200):
    ser = None
    try:
        # Mở kết nối serial
        ser = serial.Serial(port, baud_rate, timeout=1)
        print(f"Đang kết nối với {port} với baud rate {baud_rate}")

        # Đợi một chút cho kết nối ổn định
        time.sleep(2)

        # Xóa buffer serial trước khi bắt đầu
        ser.flushInput()

        print("Đang đọc dữ liệu từ Arduino...")

        while True:
            if ser.inWaiting() > 0:
                data = ser.readline().decode('utf-8').rstrip()
                print(data)

    except serial.SerialException as e:
        print(f"Không thể kết nối với {port}: {e}")
    except PermissionError as e:
        print(f"Không có quyền truy cập vào {port}: {e}")
    except KeyboardInterrupt:
        print("Đã dừng kết nối.")
    finally:
        if ser and ser.is_open:
            ser.close()
            print("Đã đóng kết nối serial.")

# Thử kết nối với COM6
read_from_serial('COM6', 115200)
