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

  if(!m_avfcx)
  {
    assert(false&&"avformat_alloc_output_context2 fail");
    return false; 
  }

  int i;
  for(i = 0; i<m_avfcx->nb_streams; i++)
  {
    m_avfcx->streams[i] = avformat_new_stream(m_avfcx, NULL); 
  }

  if (!(m_avfcx->oformat->flags & AVFMT_NOFILE)) 
  {
	  if(avio_open(&m_avfcx->pb, m_filename.c_str(), AVIO_FLAG_WRITE)<0)
	  {
	    assert(false&&"avio_open fail");
	    return false;
	  }
  }

  if(avformat_write_header(m_avfcx, NULL)<0)
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
