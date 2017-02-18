# JpegRtspClient - RTSP client for MJPEG video, with multiply channels


###A multithreaded RTSP client demo project, using live555 and OpenCV library.

This project was originally written in year 2015. It runs on Windows and it should run on Linux too (but haven't tested yet).

You will need to install OpenCV library to compile and run this project. At the time of writing this documentation I am using OpenCV version 2.4.13 but it was running also with some older version. The installer for OpenCV 2.4.13 is available [here](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.13/opencv-2.4.13.exe/download). On my installation I unpacked it into folder "D:\OpenCV2.4.13" so if you will unpack somewhere else you'll need to update the setting in Visual Studio. The OpenCV settings is kept separately in property sheets (files OpenCV\_Release\_2.4.13.props and OpenCV\_Debug\_2.4.13.props). If you will have a different version I strongly suggest to make a copy of those property sheets, rename it to OpenCV\_(Debug|Release)\_($version).props and update the right paths and library names inside, then change the active property sheets in the main project. I am not sure if OpenCV 3.0.X or higher will run with this, haven't tested with a new version yet.

OpenCV is used here to do the image decoding (from jpeg format) and displaying it using cv::imshow function. The live555 library is used for RTSP protocol processing, as a client. The application is running in several threads. There is one thread created for RTSP client. Other thread (main thread) is used for displaying the frames in main loop. The rest of the created threads are created in thread pool and are used for decoding images. The number of decoding threads are depending on the available cpu threads and it will creates CPU_THREADS-1 threads, to utilise efficiently the CPU resources if we have high number of channels. The threading part uses some new C++11 features so you will need a compiler with C++11 support.
When the frame is received from live555 library this is pushed into queue together with source URL and channel index. The thread running from thread pool will pop out the frame a will run a decode process on the image, the uncompressed frame is pushed to output queue. The main thread will check the queue (currently each 5ms, which is equivalent to 200fps and should be enough for most application), will pop all the frames and will display it using the show function (which will create different window for each rtsp url, with a title of rtsp url in window).

Program usage:

```
JpegRtspClient.exe <rtsp_url1> [rtsp_url2] ... [rtsp_urlN]
```

Run it on the command line, you need to specify at least one RTSP url. It will be one window for each RTSP url (if they work correctly and they supplying valid images to the client).
