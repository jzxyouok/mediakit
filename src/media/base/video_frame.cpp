#include "media/base/video_frame.h"

namespace media {

VideoFrame::VideoFrame(int w, int h, enum Format format)
    : _w(w), _h(h), _format(format), _data(0), _pts(0) {
  switch (_format) {
    case RGB:
      _data = new unsigned char[_w * _h * 3];
      if (!_data) {
        // throw exception("new the image data buffer failed");
        std::cout << "create the image data buffer failed" << std::endl;
      }
      break;
    case ARGB:
      _data = new unsigned char[_w * _h * sizeof(unsigned char) * 4];
      if (!_data) {
        // throw exception("new the image data buffer failed");
        std::cout << "create the image data buffer failed" << std::endl;
      }
      break;
    case YUV:
      _yuvStride[0] = w;
      _yuvStride[1] = w >> 1;
      _yuvStride[2] = w >> 1;
      _yuvLineCnt[0] = h;
      _yuvLineCnt[1] = h >> 1;
      _yuvLineCnt[2] = h >> 1;
      _yuvData[0] = (unsigned char*)malloc(_yuvStride[0] * _yuvLineCnt[0]);
      _yuvData[1] = (unsigned char*)malloc(_yuvStride[1] * _yuvLineCnt[1]);
      _yuvData[2] = (unsigned char*)malloc(_yuvStride[2] * _yuvLineCnt[2]);

      /*	_yuvData  = (unsigned
       * char*)malloc(_yuvStrid[0]*_yuvLineCnt[0]+_yuvStride[1]*_yuvLineCnt[1]+_yuvStride[2]*_yuvLineCnt[2]);*/

      _yuv = (unsigned char*)malloc(_w * _h * sizeof(unsigned char) * 3);
      break;
    default:
      break;
  }  // switch(_type)
}

VideoFrame::~VideoFrame() {
  if (_data) {
    // cout<<"delete the data"<<endl;
    delete (_data);
  }

  if (_format == YUV) {
    free(_yuvData[0]);
    free(_yuvData[1]);
    free(_yuvData[2]);
    // free(_yuvData);
    free(_yuv);
  }
}

}  // namespace media