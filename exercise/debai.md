# Bài tập trên lớp ngày 18/03/2024
## Bài 1.
Ứng dụng info_client lấy tên thư mục hiện tại, danh sách các tập tin và kích thước (trên máy client). Các dữ liệu này sau đó được đóng gói và chuyển sang info_server.
Ứng dụng info_server nhận dữ liệu từ info_client, tách các dữ liệu và in ra màn hình.
Yêu cầu kích thước dữ liệu được gửi là nhỏ nhất.
Ví dụ:
/mnt/c/Users/admin/source/baitap \
bai1.c – 12871 bytes \
bai2.c – 1437 bytes \
bai3.c – 1461 bytes 
 
## Bài 2.
Ứng dụng client đọc nội dung file văn bản và gửi sang server.
Ứng dụng server nhận dữ liệu từ client, in ra màn hình số lần xuất hiện xâu ký tự (server không cần tạo file để chứa nội dung nhận được): “0123456789”
Chú ý cần xử lý trường hợp khi xâu “0123456789” nằm giữa 2 lần truyền.
Ví dụ nội dung file văn bản:
SOICTSOICT0123456789012345678901234567890123456789SOICTSOICTSOICT01234567890123456789012345678901234567890123456789012345678901234567890123456789SOICTSOICT