#ifndef MEDIA_RENDERER_VIDEO_RENDERER_H
#define MEDIA_RENDERER_VIDEO_RENDERER_H

#include "media/base/video_frame.h"
#include "media/base/pipeline_status.h"

namespace media {
class DemuxerStream;
class VideoRenderer {
 public:
  typedef boost::function<void(std::shared_ptr<VideoFrame>)> PaintCB;
  typedef boost::function<int64_t(void)> GetTimeCB;

  ~VideoRenderer() {}
  virtual void Initialize(DemuxerStream* demuxer_stream,
                          PipelineStatusCB init_cb,
                          PipelineStatusCB status_cb,
                          PaintCB paint_cb,
                          GetTimeCB get_time_cb) = 0;
  virtual void StartPlayingFrom(int64_t offset) = 0;
  virtual void SetPlaybackRate(float rate) = 0;
};
}  // namespace media
#endif