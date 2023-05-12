# PROJECT GAME ĐẦU TIÊN CỦA TẠ DUY THUYÊN 
Xin chào thầy cô và các bạn em tên là Tạ Duy Thuyên, sinh viên lớp K67J Đại học Công nghệ - ĐHQGHN. Đây là project game đầu tiên cũng như project code đầu tiên của em. 
* Đây là bài tập lớn của môn Lập trình nâng cao
* Sử dụng ngôn ngữ C++ và thư viện SDL2
# TỔNG QUAN
1) Ý TƯỞNG BAN ĐẦU CỦA GAME
* Lấy ý tưởng từ trò chơi tuổi thơ của em trên game24h.io là Brave Soldier, em đã tạo ra được 1 game bắn súng diệt quái vật mang tên là Mighty Warrior (chiến binh mạnh mẽ) với hình ảnh, đồ họa sắc nét và chân thực hơn.
* Nhân vật chính sẽ lần lượt giết hết quái vật trên dường đi để tích lũy điểm, ăn tiền đủ một lượng để có thêm mạng, đến được vị trí cuối cùng để chiến đấu với Boss để nhận được chiến thắng.
 
* Game sẽ có thời gian để người chơi phải biết căn giờ chiến đấu với các đối tượng quái.
* Game có menu phù hợp với tên và nội dung của game, có âm thanh phù hợp với từng trạng thái game, có gameover và chơi lại.
2) LÍ DO CHỌN GAME:
* Mong muốn dự án game đầu tay của mình được phát triển từ chính game gắn với tuổi thơ của mình.
* Từ quá trình chơi game hồi bé mình nhận thấy các bạn nhỏ đặc biệt là các bạn trai đam mê trở thành những siêu anh hùng nên thường rất thích các trò chơi hành động, phiêu lưu như bắn súng, còn người lớn thì thường rất thích những trò chơi có tính thách thức và độ khó cao để có thể tận hưởng niềm vui chiến thắng một cách trọn vẹn nhất.
* Chơi game giúp người chơi thư giãn và thoải mái hơn sau những giờ làm việc căng thẳng và mệt mỏi, ngoài ra còn rèn tính tư duy và phản xạ do có rất nhiều mối nguy hiểm đang chờ đợi bạn trong game.
3) Các nguồn tham khảo
* Về phần Code:

+) LAZY FOO : https://lazyfoo.net/tutorials/SDL/index.php

+) Phát triển phần mềm 123az : https://www.youtube.com/watch?v=q1WzniyeGTU&list=PLR7NDiX0QsfTIEQUeYCfc2MyCquX0ig9V
* Về phần hình ảnh: 

+) Phát triển phần mềm 123az : https://www.youtube.com/watch?v=q1WzniyeGTU&list=PLR7NDiX0QsfTIEQUeYCfc2MyCquX0ig9V

+) Google images.
* Về âm thanh: 

+) Mixkit : https://mixkit.co/free-sound-effects/

+) Pixabay : https://pixabay.com/sound-effects/

4) Cách chạy chương trình 
* Tải visual studio code 
* Cài đặt các môi trường cần thiết để chạy chương trình 
* Tải SDL2 và các thư viện cần thiết như âm thanh, font chữ, hình ảnh theo các hướng dẫn trong Lazy Foo để có thể tận hưởng game của mình một cách đầy đủ trọn vẹn nhất.
* Cài đặt makefile trong visual studio code để có thể chạy chương trình
* Sau khi tải code của mình về thì đưa hết các file .h và .cpp ra ngoài thư mục chính, sử dụng make file như minh họa dưới đây và lưu ý cần đưa đầy đủ các file .cpp vào trong OBJS
* <img width="999" alt="imagemake" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/dd17d356-3c61-44ca-be76-45c20a19638b">
* bật Terminal và bắt đầu make :
* <img width="289" alt="terminal" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/c7bafb34-5f12-49a4-817e-a5ca57a6a10f">
* Sử dụng file .exe đã hiện lên và bắt đầu trải nghiệm game thôi nào ^_^
5) Cấu trúc của game:
* Game được chia làm 12 module và 1 main chính chạy game.
  * (BaseObject.h và BaseObject.cpp) lớp cơ sở cho các đối tượng trong game.
  * (BossObject.h và BossObject.cpp) sử dụng để quản lí đối tượng Boss với 32 khung hình frame chân thực sống động hơn.
  * (BulletObject.h và BulletObject.cpp) sử dụng để quản lí và set các chức năng cho đạn của nhân vật chính ![sphere_bullet](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/8d4e7407-b0b0-4a3c-86b6-6854d98335e5) và đạn của "quái thạch" ![la3](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/f0a15a9e-ddb4-4d61-b89f-85d696e06baa)
  * (CommonFunc.h và CommonFunc.cpp) sử dụng để gọi các thông số cơ bản cho quá trình làm game, gọi các thư viện để có thể chạy game.
  * (ExplosionObject.h và ExplosionObject.cpp) quản lí vụ nổ sau mỗi lần va chạm 
  * (game_map.h và game_map.cpp) quản lí việc tạo Map : tạo các ô tile map, load các thông số cho các ô tile map để có thể vẽ hình ảnh tương ứng với các trạng thái của ô tile map tạo nên một bản đồ hoàn chỉnh.
  * (Geometric.h và Geometric.cpp) để tạo hình chữ nhật bao quanh và tô màu phần bên trong hình chữ nhật cho các hình ảnh phụ trợ của game như điểm số, số mạng, thời gian...
  * (ImpTimer.h và ImpTimer.cpp) xử lí Timmer và FPS.
  * (MainObject.h và MainObject.cpp) quản lí việc tạo hình ảnh và các chức năng, thông số cho nhân vật main.
  * (PlayerPower.h và PlayerPower.cpp) đưa hình ảnh phụ trợ như đồng tiền ![money](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/05fa49d1-c423-418a-9d11-e4f0694be449)
và số mạng ![player_pw](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/f0296938-e122-4ece-92c7-b1448d32628d) lên màn hình.
  * (TextObject.h và TextObject.cpp) hiển thị các text lên màn hình.
  * (ThreatsObject.h và ThreatsObject.cpp) tạo quái vật và set các thông số, tính năng cho quái.
  * main.cpp sử dụng để lấy các tính năng được code trong các lớp trên và cho chạy chương trình.
 
6) Luật chơi game và cách chơi game
* Bạn sẽ có 5 phút để chơi game, nếu hết 5 phút chưa giết được Boss thì thua.
* Ban đầu bạn sẽ có 4 mạng.
* Di chuyển nhân vật main bắn các loại quái vật ban đầu gồm lính canh<img width="35" alt="linhcanh" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/4b753743-8bd3-46f1-a123-e994904da913">, quái thạch<img width="29" alt="quaithach2" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/e4b63441-3204-4def-9d63-05694c81fa48">, khủng long bạo chúa<img width="32" alt="dino" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/b4a6d62f-c7c9-4a51-b1c5-6b13dd7e4887">,xử lí mỗi đối tượng quái được cộng 1 điểm. 
* Đến cuối Map, gặp Boss<img width="83" alt="boss" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/19d10afa-0bf0-41de-bd18-1c339dadd026">
 có 100 máu, Boss bị dính 1 đạn thì trừ 1 máu, bạn được cộng 2 điểm.
* Ngược lại nếu bạn bị dính 1 đạn của bất kì thể loại quái nào, hay chạm vào người quái thì sẽ bị trừ 1 mạng.
* Khi rơi xuống vực thì không bị chết mà chỉ thiệt thời gian để quay lại Map.
* Chiến thắng sau khi giết được Boss.
* Sử dụng phím<img width="89" alt="phải" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/3a3668df-766a-49e8-b52e-734ada2e4f7f"> để quay sang phải, phím<img width="89" alt="trái" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/9a43a431-0960-4a39-a7a2-98fefd1205a7"> để quay sang trái, chuột phải để nhảy, chuột trái để bắn đạn.

7) Các chức năng hay được phát triển thêm 
* Menu phù hợp với nội dung và tên của game.
* Show ra Gameover nếu thua và Win nếu thắng.
* Hiển thị ra điểm của mình sau mỗi lần chơi game.
* Có chế độ chơi lại để người chơi có thể chinh phục lại game.
* Tạo ra được đối tượng Boss chân thật sống động với 32 khung hình frame.
* Quái khủng long chỉ bắt đầu hoạt động nếu như nhân vật main đang cách vị trí cuối cùng của Map 2 lần SCREEN_WIDTH.
* Nếu ăn được lớn hơn hoặc bằng 25 đồng tiền thì bạn sẽ có thêm 1 mạng nữa sau khi chết đủ 4 mạng.

8) Một số cải tiến mong muốn trong tương lai
* Thiết kế được thêm nhiều Map hơn ( hiện tại e đã có được 3 Map)
* Có thể tạo ra các level chơi để sử dụng được tất cả các Map đã thiết kế được và tăng giảm độ khó cho game.
* Đồ hỗ trợ nhiều hơn ví dụ như giáp bảo vệ, kim cương( ăn 1 kim cương thì được cộng 5 tiền )...
* Tạo thêm được nhiều nhân vật hơn để lựa chọn (hiện tại đã có 2 nhân vật) <img width="28" alt="main1" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/64feafef-c7de-4d53-80d4-0e488a4af28c"> <img width="28" alt="mainmain" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/e4f2aab6-7558-485f-beaa-6f1049515430">
* Âm thanh sống động phù hợp với từng trạng thái: ăn tiền, nổ, hồi sinh, bấm nút tại menu.
# HÌNH ẢNH MÔ TẢ GAME
  * Menu
  <img width="962" alt="menu" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/c3dab3b0-1dc7-4432-8ca4-c40fb093dc4a">

 * GameOver
 <img width="959" alt="over" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/7ed2ffb6-0b6e-46b3-a6ab-277ba290c35d">
 
 * Win game
 <img width="957" alt="win" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/d0a07428-1334-4745-a1ad-dc7bc8538f2c">
 
 * Play Game
    * Main nổ
    <img width="464" alt="ảnh nổ của main" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/7c4a81bb-f86d-4e03-a3a5-b8862ce0509b">
    
    
    * Chơi game
    <img width="959" alt="play2" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/b387adbe-8678-4589-9cec-aef5660e7c7b">



    * Hang rồng
    <img width="958" alt="hang rồng" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/fed16f8e-b830-4a0e-bd03-c55ad0d7556c">

     







