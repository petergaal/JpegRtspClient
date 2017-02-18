
#include "decode_thread.h"
#include "ctpl_stl.h"
#include <queue>
#include <mutex>
#include <thread>
#include "opencv2/opencv.hpp"


std::queue<Decoder_Input*> InputQueue;
std::queue<Decoder_Output*> OutputQueue;
std::mutex InputLock;
std::mutex OutputLock;

ctpl::thread_pool *decoder_pool;

int decode_thread_func(int id)
{
  Decoder_Input *inp;
  Decoder_Output *outp;
  {
    std::unique_lock<std::mutex> lock(InputLock);
    inp = InputQueue.front();
    InputQueue.pop();
  }

  // this pointer will be released outside, in the thread which calls Decoder_Pop()
  // and uses the output data
  outp = new Decoder_Output;
  outp->rtsp_URL = inp->rtsp_URL;
  outp->sourceInt = inp->sourceInt;
  outp->OutputData = cv::imdecode(inp->InputData, 1);
  // here we are releasing the input data, after 
  // processing it - it's a responsibility of this thread
  delete inp;
  {
    std::unique_lock<std::mutex> lock(OutputLock);
    OutputQueue.push(outp);
  }

  return 0;
}

int Decoder_Push(Decoder_Input *data_in)
{
  {
    std::unique_lock<std::mutex> lock(InputLock);
    InputQueue.push(data_in);
  }
  decoder_pool->push(decode_thread_func);
  return 0;
}

Decoder_Output* Decoder_Pop()
{
  Decoder_Output *outp = NULL;
  {
    std::unique_lock<std::mutex> lock(OutputLock);
    if (!OutputQueue.empty())
    {
      outp = OutputQueue.front();
      OutputQueue.pop();
    }
  }
  return outp;
}

void Decoder_ThreadPool_Init()
{
  int Num_Threads = std::thread::hardware_concurrency();
  if (Num_Threads < 2) {
    Num_Threads = 2;
  }
  decoder_pool = new ctpl::thread_pool(Num_Threads-1);
}

void Decoder_ThreadPool_Close()
{
  delete decoder_pool;
}
