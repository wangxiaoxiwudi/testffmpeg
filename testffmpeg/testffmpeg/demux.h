
class Demuxer
{
public:
  Demuxer(std::string file_name);
  ~Demuxer();
  bool init();
  void process();
  void finish();
private:
  AVFormatContext * m_avfcx;
  std::string m_filename;
  AVPacket m_pkt;
};