#include <iostream>
#include "read_write_lock.h"
#include <vector>
#include <thread>

using namespace std;

constexpr int READ_COUNT{ 100 };

template<typename LOCK, typename T>
void read_value(LOCK& lck, const T& data, int times) {
	while (times--) {
		cout << "going to read\n";
		util::read_lock l{ lck };
		cout << "entered read \n";
		for (const auto& i : data) {
			cout << i;
		}

		cout << "read done going to sleep\n";
		this_thread::sleep_for(10s);
	}
}

template<typename LOCK, typename T>
void write_value(LOCK& lck, T& data, int times) {
	int i = 90;
	while (times--) {
		cout << "going to write\n";
		util::write_lock l{ lck };
		cout << "entered write\n";
		cout << " write id " << this_thread::get_id() << " i = " << i << "****************************\n";
		data.push_back(i++);
		this_thread::sleep_for(10s);
	}
}

int main() {
	util::read_write_mutex lock;
	vector<int> a{ 1,2,3 };
	vector<thread> arr;

	
	arr.push_back(thread{ read_value<util::read_write_mutex, vector<int>>,  ref(lock), a, 1 });

	this_thread::sleep_for(4ms);
	arr.push_back(thread{ read_value<util::read_write_mutex, vector<int>>,  ref(lock), a, 1 });
	this_thread::sleep_for(4ms);
	arr.push_back(thread{ write_value<util::read_write_mutex, vector<int>>, ref(lock), ref(a), 1 });
	this_thread::sleep_for(4ms);
	arr.push_back(thread{ read_value<util::read_write_mutex, vector<int>>,  ref(lock), a, 1 });
	this_thread::sleep_for(4ms);
	arr.push_back(thread{ read_value<util::read_write_mutex, vector<int>>,  ref(lock), a, 1 });

	
	for (auto& i : arr) {
		i.join();
	}
}