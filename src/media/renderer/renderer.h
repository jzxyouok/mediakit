#ifndef MEDIA_RENDERER_RENDERER_H
#define MEDIA_RENDERER_RENDERER_H

#include "media/base/pipeline_status.h"
#include "media/demuxer/demuxer_stream_provider.h"
#include "media/renderer/video_renderer.h"

namespace media {
class Renderer {
 public:
  virtual ~Renderer(){};
  virtual void Initialize(DemuxerStreamProvider* demuxer_stream_provider,
                          PipelineStatusCB init_cb, PipelineStatusCB status_cb,
                          VideoRenderer::PaintCB paint_cb) = 0;
  virtual void StartPlayingFrom(int64_t time_offset) = 0;
  virtual void SetPlaybackRate(float rate) = 0;
  virtual void SetVolume(float volume) = 0;
};
}  // namespace media

#endif