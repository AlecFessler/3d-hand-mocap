// © 2024 Alec Fessler
// MIT License
// See LICENSE file in the project root for full license information.

#ifndef VIDENC_H
#define VIDENC_H

#include <cstdint>
#include <functional>
#include <memory>
#include "config.h"
#include "connection.h"
extern "C" {
#include <libavcodec/avcodec.h>
}

class videnc {
public:
  videnc(const config& config);
  ~videnc();

  void encode_frame(uint8_t* data);
  void flush();
  uint8_t* recv_frame(int& size);

private:
  int width;
  int height;
  int64_t pts_counter;
  const AVCodec* codec;
  AVCodecContext* ctx;
  AVFrame* frame;
  AVPacket* pkt;
};

#endif
