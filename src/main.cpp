
#include <thread>
#include "rtsp_thread.h"
#include "decode_thread.h"
#include "opencv2/opencv.hpp"

/*
int jpeg_decode_test()
{
  cv::Mat Input;

  char data[64 * 1024];
  int datasize;
  FILE *f;
  f = fopen("image.jpg","rb");
  datasize = fread(data, 1, 64 * 1024, f);
  fclose(f);
  //image.jpg

  Input = cv::Mat(1, datasize, CV_8UC1, data);

  printf("Dimensions: %dx%d\n", Input.cols, Input.rows);

  cv::Mat matrixJprg = cv::imdecode(Input, 1);
  printf("Dimensions: %dx%d\n", matrixJprg.cols, matrixJprg.rows);

  return 0;
  
}
*/

int main(int argc, char** argv) {

  // run the rtsp thread in first case
  std::thread t1(rtsp_thread_main, argc, argv);
  Decoder_ThreadPool_Init();
  // just an endless loop currently in the main thread, with a low CPU usage (sleep cycles)
  // later we can handle the GUI here
  //int key;
  while (true)
  {
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    Decoder_Output *outp;

    while (outp = Decoder_Pop())
    {
      // in a real environment (where you have different URLs, use just the commented line with the URL
//      cv::imshow(outp->rtsp_URL, outp->OutputData);

      // here I will use also the source int because I had just two different cameras
      char str[100];
      snprintf(str, 100, "%d %s", outp->sourceInt, outp->rtsp_URL.c_str());
      cv::imshow(str, outp->OutputData);

      // delete the data from output queue after using it
      delete outp;
    }
    // just to make sure the screen will refresh (otherwise there is just a gray box)
    cv::waitKey(5);

    /*
    // just in case if you would like to exit the program
    key = cv::waitKey(5);
    if (key != -1) {
      break;
    }
    */
  }

  // we never reach this part really, but this is what we should do (I tested it once)
  // signal the end for rtsp thread
  eventLoopWatchVariable = 1;
  // wait for finish rtsp thread
  t1.join();
  // finish and release the threadpool
  Decoder_ThreadPool_Close();
  return 0;
//  return rtsp_thread_main(argc, argv);
}


