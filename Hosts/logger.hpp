//http://stackoverflow.com/questions/6168107/how-to-implement-a-good-debug-logging-feature-in-a-project

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>


enum loglevel_e
{logMESSAGE, logERROR, logWARNING, logINFO, logDEBUG, logDEBUG2, logLATTESTS, logBNDTESTS, logCPUTESTS};

extern loglevel_e loglevel;

class logIt
{
	loglevel_e loglevel2;
	int end_line;
	public:
	logIt(loglevel_e _loglevel = logERROR, int endline = 1) {
		end_line = endline;
		loglevel2 = _loglevel;

		_buffer << (boost::posix_time::second_clock::local_time().time_of_day().seconds()) << " - ";

		if(endline != 2){
			switch(loglevel2){
				case 1: _buffer << "ERROR   : ";
					break;
				case 2: _buffer << "WARNING : ";
					break;
				case 3: _buffer << "INFO    : ";
					break;
				case 4: _buffer << "DEBUG   : ";
					break;
				case 5: _buffer << "DEBUG2  : ";
					break;		
				case 6: _buffer << "TEST: LATENCY TIMESTAMP : ";
					break;
				case 7: _buffer << "TEST: PACKETS SENT : ";
					break;
				case 8: _buffer << "TEST: CPU TIMESTAMP : ";
					break;
			}
		}
	}

	template <typename T>
		logIt & operator<<(T const & value)
		{
			_buffer << value;
			return *this;
		}

	void write(char* s, std::streamsize n){
		_buffer.write(s, n);
	}

	~logIt()
	{
		if(end_line == 1){
			_buffer << std::endl;
		}

		std::cout << _buffer.str();
	}

	private:
	std::ostringstream _buffer;
};




#define llog(level) \
	if (level > loglevel) {} \
else logIt(level)

#define llog2(level, line) \
	if (level > loglevel) {} \
else logIt(level, line)




#endif
