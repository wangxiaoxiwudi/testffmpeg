#include <stdafx.h>
#include "demux.h"

Demuxer::Demuxer(std::string file_name):m_filename(file_name)
{

}

Demuxer::~Demuxer()
{

}

bool Demuxer::init()
{
  av_register_all();

  if (avformat_open_input(&m_avfcx, m_filename.c_str(), 0, 0) < 0) 
  {
    fprintf(stderr, "Could not open input file '%s'", m_filename.c_str());
	return false;
  }

  if (avformat_find_stream_info(m_avfcx, 0) < 0) 
  {
    fprintf(stderr, "Failed to retrieve input stream information");
	return false;
  }

  av_dump_format(m_avfcx, 0, m_filename.c_str(), 0);
  av_init_packet(&m_pkt);
}

void Demuxer::process()
{
  av_read_frame(m_avfcx,&m_pkt);
}

void Demuxer::finish()
{
  avformat_close_input(&m_avfcx);
  av_free_packet(&m_pkt);
}

