#ifndef MUX_H_
#define MUX_H_

class Muxer
{
public:
  Muxer(std::string out_filename);
  ~Muxer();
  bool init();
  void process();
  void finish();
private:
  std::string m_filename;
  AVFormatContext * m_avfcx;
  AVPacket m_pkt;
};

#endif

