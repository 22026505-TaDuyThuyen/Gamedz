// #ifndef IMP_TIMER_H_
// #define IMP_TIMER_H_

class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
//hàm bắt đầu chạy sau đó đếm thời gian
    void start();
    void stop();
    void paused();
    void unpaused();

    int get_ticks();

//hai hàm trả lại trạng thái là is_pause và is_start
    bool is_started();
    bool is_paused();

private:
// thời điểm bắt đầu chạy là thời điểm nào 
    int start_tick_;
// thời điểm tạm dừng
    int paused_tick_;
// trạng thái dừng hay bắt đầu
    bool is_paused_;
    bool is_started_;

};

#endif