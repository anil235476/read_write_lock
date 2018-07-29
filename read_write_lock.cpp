#include "read_write_lock.h"


namespace util {
	using namespace std;

	void read_write_mutex::read_lock() {
		while (true) {
			if (writer_count_.load() > 0) {
				unique_lock<mutex> lck{ reader_wait_lck_ };
				reader_wait_.wait(lck, [this]() {return writer_count_ == 0; });
			}
			reader_count_.fetch_add(1);
			if (writer_count_.load() == 0)
				break;
			reader_count_.fetch_sub(1);
		}
	}

	void read_write_mutex::write_lock() {
		writer_count_.fetch_add(1);
		lock_guard<mutex> lck{ write_lck_ };
		if (reader_count_.load() > 0) {
			unique_lock<mutex> lck_{ write_wait_lck_ };
			write_wait_.wait(lck_, [this]() {return  reader_count_ == 0; });
		}
	}

	void read_write_mutex::read_unlock() {
		if (reader_count_.fetch_sub(1) == 1) {
			lock_guard<mutex> lck_{ write_wait_lck_ };
			write_wait_.notify_all();
		}
	}

	void read_write_mutex::write_unlock() {
		if (writer_count_.fetch_sub(1) == 1) {
			lock_guard<mutex> lck{ reader_wait_lck_ };
			reader_wait_.notify_all();
		}
	}

}//namespace util