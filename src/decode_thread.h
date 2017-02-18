
#include <string>
#include "opencv2/opencv.hpp"

struct Decoder_Input {
  int sourceInt;
  std::string rtsp_URL;
  cv::Mat InputData;
};

struct Decoder_Output {
  int sourceInt;
  std::string rtsp_URL;
  cv::Mat OutputData;
};

int Decoder_Push(Decoder_Input *data_in);
Decoder_Output* Decoder_Pop();

void Decoder_ThreadPool_Init();
void Decoder_ThreadPool_Close();

extern char eventLoopWatchVariable;