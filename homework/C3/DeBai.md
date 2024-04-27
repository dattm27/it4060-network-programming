# Bài tập chương 3: Các kiến trúc client - server
## Bài 1
Sử  dụng  hàm  select()/poll(),  viết  chương  trìnhchat_serverthực hiện các chức năng sau:Nhận kết nối từ các client, và vào hỏi tên client chođến khi client gửi đúng cú pháp:“client_id: client_name”\
trong đó client_name là tên của client, xâu ký tự viếtliền.\
Sau đó nhận dữ liệu từ một client và gửi dữ liệu đóđến các client còn lại, ví dụ: client có id “abc” gửi “xinchao” thì các client khác sẽ nhận được: “abc: xin chao”hoặc  có  thể  thêm  thời  gian  vào  trước  ví  dụ:“2023/05/06 11:00:00PM abc: xin chao”.

## Bài 2
Sử dụng hàm select()/poll(), viết chương trìnhtelnet_serverthực hiện cácchức năng sau:\
Khi đã kết nối với 1 client nào đó, yêu cầu client gửi user và pass, so sánhvới file cơ sở dữ liệu là một file text, mỗi dòng chứa một cặp user + pass\
 vídụ:admin adminguest nopass...\
Nếu so sánh sai, không tìm thấy tài khoản thì báo lỗi đăng nhập.Nếu đúng thì đợi lệnh từ client, thực hiện lệnh và trả kết quả cho client.\
Dùng hàm system(“dir > out.txt”) để thực hiện lệnh.dir là ví dụ lệnh dir mà client gửi.\
 \ > out.txt để định hướng lại dữ liệu ra từ lệnh dir, khi đó kết quả lệnh dir sẽđược ghi vào file văn bản.

 