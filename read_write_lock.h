#ifndef _READ_WRITE_LOCK_H__
#define _READ_WRITE_LOCK_H__
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace util {
	class read_write_mutex {
	private:
		std::atomic<int> reader_count_{ 0 };
		std::atomic<int> writer_count_{ 0 };
		std::mutex write_lck_;

		std::condition_variable reader_wait_;
		std::mutex reader_wait_lck_;

		std::condition_variable write_wait_;
		std::mutex write_wait_lck_;

	public:
		void read_lock();
		void write_lock();

		void read_unlock();
		void write_unlock();
	};

	class read_lock {
	private:
		read_write_mutex& m_;
	public:
		read_lock(read_write_mutex& m) :m_{ m } { m_.read_lock();}
		~read_lock() { m_.read_unlock(); }
	};

	class write_lock {
	private:
		read_write_mutex & m_;
	public:
		write_lock(read_write_mutex& m) :m_{ m } {
			m_.write_lock();
		}
		~write_lock() { m_.write_unlock(); }
	};
}
#endif//_READ_WRITE_LOCK_H__