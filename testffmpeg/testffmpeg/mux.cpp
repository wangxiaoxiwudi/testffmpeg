#include <stdafx.h>
#include "mux.h"

Muxer::Muxer(std::string out_filename):m_filename(out_filename),m_avfcx(NULL)
{

}

Muxer::~Muxer()
{

}

bool Muxer::init()
{
  av_register_all();
  avformat_alloc_output_context2(&m_avfcx, NULL, NULL, m_filename.c_str());

  if (!m_avfcx)
  {
    assert(false&&"avformat_alloc_output_context2 fail");
    return false; 
  }

  if (m_avfcx->oformat->video_codec != AV_CODEC_ID_NONE)
  {
    AVCodec * vcodec = avcodec_find_encoder(m_avfcx->oformat->video_codec);
	AVStream * vstream = avformat_new_stream(m_avfcx, vcodec);
	AVCodecContext * vcodectx = vstream->codec;
#if 0
	vcodectx->codec_id = m_avfcx->oformat->video_codec;

    vcodectx->bit_rate = 400000;
     /* Resolution must be a multiple of two. */
    vcodectx->width    = 352;
    vcodectx->height   = 288;
     /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
      * identical to 1. */
	AVRational vtimebase = { 1, 25 };
	vstream->time_base = vtimebase;
    vcodectx->time_base = vstream->time_base;
    vcodectx->gop_size      = 12; /* emit one intra frame every twelve frames at most */
    vcodectx->pix_fmt       = AV_PIX_FMT_YUV420P;
    if (vcodectx->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B frames */
    vcodectx->max_b_frames = 2;
    }
    if (vcodectx->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
    vcodectx->mb_decision = 2;
    }
#endif
  }

  if (m_avfcx->oformat->audio_codec != AV_CODEC_ID_NONE) 
  {
    int i;
	AVCodec * acodec = avcodec_find_encoder(m_avfcx->oformat->video_codec);
	AVStream * astream = avformat_new_stream(m_avfcx, acodec);
#if 0
	AVCodecContext * acodectx = astream->codec;
	acodectx->codec_id = m_avfcx->oformat->audio_codec;
	acodectx->sample_fmt  = acodec->sample_fmts ?
		acodec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
	acodectx->bit_rate    = 64000;
	acodectx->sample_rate = 44100;
	if (acodec->supported_samplerates) {
		acodectx->sample_rate = acodec->supported_samplerates[0];
		for (i = 0; acodec->supported_samplerates[i]; i++) {
			if (acodec->supported_samplerates[i] == 44100)
				acodectx->sample_rate = 44100;
		}
	}
	acodectx->channels        = av_get_channel_layout_nb_channels(acodectx->channel_layout);
	acodectx->channel_layout = AV_CH_LAYOUT_STEREO;
	if (acodec->channel_layouts) {
		acodectx->channel_layout = acodec->channel_layouts[0];
		for (i = 0; acodec->channel_layouts[i]; i++) {
			if (acodec->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
				acodectx->channel_layout = AV_CH_LAYOUT_STEREO;
		}
	}
	acodectx->channels        = av_get_channel_layout_nb_channels(acodectx->channel_layout);
    AVRational atimebase = { 1, acodectx->sample_rate };
	astream->time_base = atimebase;
#endif
  }

  if (!(m_avfcx->oformat->flags & AVFMT_NOFILE)) 
  {
	  if(avio_open(&m_avfcx->pb, m_filename.c_str(), AVIO_FLAG_WRITE)<0)
	  {
	    assert(false&&"avio_open fail");
	    return false;
	  }
  }
  //avformat_write_header fail because this ,why?
  //#define AVFMT_NODIMENSIONS  0x0800 /**< Format does not need width/height */
  m_avfcx->oformat->flags = AVFMT_NODIMENSIONS;

  if (avformat_write_header(m_avfcx, NULL)<0)
  {
    assert(false&&"avformat_write_header fail"); 
	return false;
  }

  av_init_packet(&m_pkt);

}

void Muxer::process()
{
  av_interleaved_write_frame(m_avfcx, &m_pkt);
}

void Muxer::finish()
{
  av_write_trailer(m_avfcx);
  avformat_free_context(m_avfcx);
  av_free_packet(&m_pkt);
}
