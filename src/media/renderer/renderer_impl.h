#ifndef MEDIA_RENDERER_RENDERER_IMPL_H
#define MEDIA_RENDERER_RENDERER_IMPL_H

#include "base/base_type.h"
#include "media/base/pipeline_status.h"
#include "media/renderer/renderer.h"
#include "media/renderer/audio_renderer.h"
#include "media/renderer/video_renderer.h"
#include "media/decoder/audio_decoder.h"
#include "media/decoder/video_decoder.h"
#include "media/demuxer/demuxer_stream_provider.h"
#include "media/base/time_source.h"
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace media {

class RendererImpl : public Renderer {
 public:
  RendererImpl(TaskRunner* task_runner,
               std::shared_ptr<AudioRenderer> audio_renderer,
               std::shared_ptr<VideoRenderer> video_renderer);
  virtual void Initialize(DemuxerStreamProvider* demuxer_stream_provider,
                          PipelineStatusCB init_cb,
                          PipelineStatusCB status_cb,
                          VideoRenderer::PaintCB paint_cb);
  virtual void StartPlayingFrom(int64_t time_offset);
  virtual void SetPlaybackRate(float rate);
  virtual void SetVolume(float volume);

 private:
  enum State {
    STATE_UNINITIALIZED,
    STATE_AUDIO_RENDERER_INITIALIZING,
    STATE_VIDEO_RENDERER_INITIALIZING,
    STATE_AUDIO_RENDERER_INIT_FAILED,
    STATE_VIDEO_RENDERER_INIT_FAILED,
    STATE_NORMAL,
    STATE_ERROR,
  };

  void InitializeAction();

  void InitializeAudioRenderer();
  void OnInitializeAudioRendererDone(PipelineStatus status);

  void InitializeVideoRenderer();
  void OnInitializeVideoRendererDone(PipelineStatus status);

  int64_t GetCurrentTime();

  TaskRunner* task_runner_;
  VideoRenderer::PaintCB paint_cb_;
  PipelineStatusCB init_cb_;
  PipelineStatusCB status_cb_;
  State state_;
  boost::scoped_ptr<TimeSource> time_source_;
  DemuxerStreamProvider* demuxer_stream_provider_;
  std::shared_ptr<AudioRenderer> audio_renderer_;
  std::shared_ptr<VideoRenderer> video_renderer_;
};
}
#endif