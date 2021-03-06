//
//  ffmpeg_demuxer.h
//  mediacore
//
//  Created by 李楠 on 15/8/28.
//
//

#ifndef MEDIA_DEMUXER_FFMPEG_DEMUXER_H_
#define MEDIA_DEMUXER_FFMPEG_DEMUXER_H_

#include <memory>

#include "boost/function.hpp"
#include <boost/thread/thread.hpp>

#include "demuxer.h"
#include "base/base_type.h"
#include "net/io_channel.h"
#include "ffmpeg_demuxer_stream.h"

namespace media {
class FFmpegDemuxer : public Demuxer {
 public:
  typedef boost::function<void(bool)> ActionCB;

  FFmpegDemuxer(TaskRunner* task_runner,
                std::shared_ptr<net::IOChannel> data_source);
  ~FFmpegDemuxer();

  // demuxer implementation
  virtual void Initialize(PipelineStatusCB status_cb) override;
  //micro second
  virtual void Seek(int64_t timestamp, PipelineStatusCB status_cb) override;
  virtual void Stop() override;
  virtual int64_t GetStartTime() override;
  virtual int64_t GetDuration() override;
  virtual int64_t GetTimelineOffset() override;
  virtual void ShowMediaConfigInfo() override;

  // DemuxerStreamProvier implementation
  virtual DemuxerStream* GetDemuxerStream(DemuxerStream::Type type) override;

  void NotifyDemuxerCapacityAvailable();

 private:
  void StartBlockingTaskRunner();
  void StopBlockingTaskRunner();
  // ffmpeg callback function
  static int FFmpegReadPacketCB(void* opaque,
                                unsigned char* buffer,
                                int buffer_size);
  static int64_t FFmpegSeekCB(void* opaque, int64_t offset, int whence);

  int ReadPacketCB(unsigned char* buffer, int buffer_size);
  int64_t SeekCB(int64_t offset, int whence);
  
  void OpenAVFormatContextAction(PipelineStatusCB status_cb,
                                 ActionCB action_cb);
  void OnOpenAVFormatContextDone(PipelineStatusCB status_cb, bool result);

  void FindStreamInfoAction(PipelineStatusCB status_cb, ActionCB action_cb);
  void OnFindStreamInfoDone(PipelineStatusCB status_cb, bool result);

  void SeekAction(int64_t timestamp,
                  PipelineStatusCB state_cb,
                  ActionCB action_cb);
  void OnSeekDone(PipelineStatusCB state_cb, bool result);
  
  void ReadFrameIfNeeded();
  
  void ReadFrameAction(std::shared_ptr<EncodedAVFrame> encoded_avframe,
                       ActionCB action_cb);
  void OnReadFrameDone(std::shared_ptr<EncodedAVFrame> encoded_avframe,
                       bool result);

  int64_t EstimateStartTimeFromProbeAVPacketBuffer(AVFormatContext* av_format_context);

 private:
  typedef std::vector<FFmpegDemuxerStream*> DemuxerStreamVector;

  bool demux_complete_;
  bool data_source_complete;
  bool pending_seek_;
  std::shared_ptr<net::IOChannel> data_source_;
  int64_t duration_;
  int64_t start_time_;
  DemuxerStreamVector streams_;
  unsigned char* av_io_buffer_;
  AVIOContext* av_io_context_;
  AVFormatContext* av_format_context_;
  TaskRunner* task_runner_;
  boost::scoped_ptr<TaskRunner> blocking_task_runner_;
  boost::scoped_ptr<boost::thread> blocking_thread;
  boost::scoped_ptr<boost::asio::io_service::work> blocking_io_service_work_;
};

}  // namespace media

#endif /* defined(__mediacore__ffmpeg_demuxer__) */
