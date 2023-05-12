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
<img width="999" alt="imagemake" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/bee32731-f6dd-43b3-af94-6d4e84f2541b">

* bật Terminal và bắt đầu make :
<img width="289" alt="terminal" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/33d6e98c-570a-4ad3-b770-b2a0e585503f">

* Sử dụng file .exe đã hiện lên và bắt đầu trải nghiệm game thôi nào ^_^
5) Cấu trúc của game:
* Game được chia làm 12 module và 1 main chính chạy game.
  * (BaseObject.h và BaseObject.cpp) lớp cơ sở cho các đối tượng trong game.
  * (BossObject.h và BossObject.cpp) sử dụng để quản lí đối tượng Boss với 32 khung hình frame chân thực sống động hơn.
  * (BulletObject.h và BulletObject.cpp) sử dụng để quản lí và set các chức năng cho đạn của nhân vật chính ![sphere_bullet](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/40935cdf-21fc-4f4c-8f8f-899845ef80bb)
 và đạn của "quái thạch" ![la3](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/89a3e509-17c0-4986-8977-8bea0ba7c8ec)

  * (CommonFunc.h và CommonFunc.cpp) sử dụng để gọi các thông số cơ bản cho quá trình làm game, gọi các thư viện để có thể chạy game.
  * (ExplosionObject.h và ExplosionObject.cpp) quản lí vụ nổ sau mỗi lần va chạm 
  * (game_map.h và game_map.cpp) quản lí việc tạo Map : tạo các ô tile map, load các thông số cho các ô tile map để có thể vẽ hình ảnh tương ứng với các trạng thái của ô tile map tạo nên   một bản đồ hoàn chỉnh.
  * (Geometric.h và Geometric.cpp) để tạo hình chữ nhật bao quanh và tô màu phần bên trong hình chữ nhật cho các hình ảnh phụ trợ của game như điểm số, số mạng, thời gian...
  * (ImpTimer.h và ImpTimer.cpp) xử lí Timmer và FPS.
  * (MainObject.h và MainObject.cpp) quản lí việc tạo hình ảnh và các chức năng, thông số cho nhân vật main.
  * (PlayerPower.h và PlayerPower.cpp) đưa hình ảnh phụ trợ như đồng tiền ![money](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/cc3fcf72-e25d-4d8e-9699-a427913b9b60)
 và số mạng ![player_pw](https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/493ef9c7-c7cd-43f0-998b-ae9248065977)
 lên màn hình.
  * (TextObject.h và TextObject.cpp) hiển thị các text lên màn hình.
  * (ThreatsObject.h và ThreatsObject.cpp) tạo quái vật và set các thông số, tính năng cho quái.
  * main.cpp sử dụng để lấy các tính năng được code trong các lớp trên và cho chạy chương trình.
 
6) Luật chơi game và cách chơi game
* Bạn sẽ có 5 phút để chơi game, nếu hết 5 phút chưa giết được Boss thì thua.
* Ban đầu bạn sẽ có 4 mạng.
* Di chuyển nhân vật main bắn các loại quái vật ban đầu gồm lính canh<img width="35" alt="linhcanh" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/ddd73f01-4bd8-47a0-aa34-79a2ca0a6cfe">, quái thạch<img width="29" alt="quaithach2" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/242f14e0-0aaa-4d95-a254-ba570f06d1b5">, khủng long bạo chúa<img width="32" alt="dino" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/0accedcf-be43-4870-956a-c8cde2224db5">,xử lí mỗi đối tượng quái được cộng 1 điểm. 
* Đến cuối Map, gặp Boss<img width="83" alt="boss" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/10ad8f43-0050-4a66-aff4-bce8061b4a4c">
có 100 máu, Boss bị dính 1 đạn thì trừ 1 máu, bạn được cộng 2 điểm.
* Ngược lại nếu bạn bị dính 1 đạn của bất kì thể loại quái nào, hay chạm vào người quái thì sẽ bị trừ 1 mạng.
* Khi rơi xuống vực thì không bị chết mà chỉ thiệt thời gian để quay lại Map.
* Chiến thắng sau khi giết được Boss.
* Sử dụng phím<img width="89" alt="phải" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/629a5bf5-d836-4cde-8661-e1389c06f95c">
 để quay sang phải, phím<img width="89" alt="trái" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/d91ea0eb-b212-4dd1-9bf2-f86874b28992">
 để quay sang trái, chuột phải để nhảy, chuột trái để bắn đạn.

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
* Tạo thêm được nhiều nhân vật hơn để lựa chọn (hiện tại đã có 2 nhân vật) <img width="28" alt="main1" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/3acfa401-2dd5-4b4f-83aa-233f3a358694"> và <img width="28" alt="mainmain" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/0a4de63c-ca2e-432e-adfc-35dd6fd513cd">


* Âm thanh sống động phù hợp với từng trạng thái: ăn tiền, nổ, hồi sinh, bấm nút tại menu.
# HÌNH ẢNH MÔ TẢ GAME
  * Menu
  <img width="962" alt="menu" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/25b4c2bd-e371-4024-bf10-c60a2d411ddc">

 * GameOver
 <img width="959" alt="over" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/637348f0-d7dc-4c15-b0e6-80d2b71d52c8">

 
 * Win game
 <img width="957" alt="win" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/369fe73e-7a4a-49ee-8ab0-b9a2a5b58cfa">

 * Play Game
    * Main nổ
    <img width="464" alt="ảnh nổ của main" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/b7bc8245-4c85-405a-b16f-70e5e37bf77a">

    
    
    * Chơi game
    <img width="959" alt="play2" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/8f989d89-c63a-40c4-a172-82dcee44e23c">




    * Hang rồng
    <img width="958" alt="hang rồng" src="https://github.com/22026505-TaDuyThuyen/Gamedz/assets/125566588/c9ec26d8-292d-4573-8095-827c0625ac83">


# RÚT RA KINH NGHIỆM
   * Qua quá trình làm game em rút thêm cho mình được nhiều kinh nghiệm quý giá về code như vận dụng và sắp xếp vị trí của các dòng code một cách hợp lí hơn, hiểu thêm về lập trình hướng đối tượng, sắp xếp các chức năng của game một cách hợp lí. Ngoài ra còn giúp em có thêm kiến thức về phần thiết kế cấu trúc và bố cục của một game, biết tìm kiếm thông tin cần thiết cho project. 
   * Em xin chân thành cảm ơn các thầy lí thuyết và thầy thực hành đã giúp đỡ và giảng dạy cho em kiến thức để em hoàn thành được project đầu tay của mình.
 # LIÊN HỆ VỚI EM QUA:
 _ Gmail : taduythuyen2004@gmail.com
 
 _ Facebook : Tạ Duy Thuyên.






