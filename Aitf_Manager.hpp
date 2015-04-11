class Aitf_Manager
{
public:
  Aitf_Manager();
  void start_thread();
  void join();

private:
	void run();
	boost::thread aitf_thread;

};