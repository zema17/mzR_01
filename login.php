<?php
// Thiết lập kết nối tới MySQL
$conn = mysqli_connect("127.0.0.1", "root", "123456", "dulieu");

// Kiểm tra kết nối
if (!$conn) {
    die("Kết nối thất bại: " . mysqli_connect_error());
}

// Dữ liệu đầu vào (tên đăng nhập và mật khẩu để kiểm tra)
$uname = "admin1";
$pwd = "123456";

// Truy vấn để lấy thông tin người dùng dựa trên tên đăng nhập và mật khẩu
$qry = "SELECT * FROM user WHERE username='$uname' AND password='$pwd'";

// Thực thi truy vấn
$raw = mysqli_query($conn, $qry);

// Kiểm tra số lượng dòng trả về
$count = mysqli_num_rows($raw);

// Xử lý kết quả
if ($count > 0) {
    // Người dùng được tìm thấy, lấy thông tin chi tiết người dùng
    $row = mysqli_fetch_assoc($raw);

    // Ví dụ về dữ liệu phản hồi
    $response = array(
        'ID' => $row['ID'],
        'username' => $row['username'],
        'name' => $row['name'],
        'email' => $row['email'],
        'title' => $row['title']
    );

    // Xuất thông báo thành công
    echo "Người dùng được tìm thấy: ";
    print_r($response); // In ra thông tin người dùng
} else {
    // Không tìm thấy người dùng
    echo "Không tìm thấy người dùng";
}

// Đóng kết nối
mysqli_close($conn);
?>
