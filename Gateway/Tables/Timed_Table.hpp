#ifndef TIMED_TABLE_HPP
#define TIMED_TABLE_HPP

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>

class Timed_Table{
	public:
		void start_thread();
		void stop_thread();

	protected:
		boost::asio::io_service table_io;
		boost::mutex table_mutex;

	private:
		boost::thread table_thread;
		boost::shared_ptr<boost::asio::io_service::work> table_work;
		void run();

};

#endif
