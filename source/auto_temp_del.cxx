#include <windows.h>
#define WIN32_LEAN_AND_MEAN

#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

namespace fs = std::filesystem;

// Performs an approximate timepoint conversion from one clock to another.
template<typename FROM_CLOCK_T, typename TO_CLOCK_T>
std::chrono::time_point<TO_CLOCK_T> ConvertTimepointClock(std::chrono::time_point<FROM_CLOCK_T> timepoint) {
	using namespace std::chrono;
	return time_point_cast<typename TO_CLOCK_T::duration>((timepoint - FROM_CLOCK_T::now()) + TO_CLOCK_T::now());
}

void WatchFolder(const std::string& folder, const uint64_t& threshold_seconds = 21600, const uint64_t scan_delay = 3600) {
	std::vector<fs::path> existing_files;
	
	for(;fs::is_directory(folder); Sleep(scan_delay)) {
		using namespace std::chrono;

		auto current_system_timepoint = system_clock::now();
		uint64_t current_system_timestamp = duration_cast<seconds>(current_system_timepoint.time_since_epoch()).count();
		
		auto last_folder_modified_timepoint = ConvertTimepointClock<fs::file_time_type::clock, system_clock>(fs::last_write_time(fs::path(folder)));
		uint64_t last_folder_modified_timestamp = duration_cast<seconds>(last_folder_modified_timepoint.time_since_epoch()).count();

		if((current_system_timestamp - last_folder_modified_timestamp) > threshold_seconds) {
			for(const auto& entry : fs::recursive_directory_iterator(folder)) {
				if(fs::is_regular_file(entry)) {
					current_system_timepoint = system_clock::now();
					current_system_timestamp = duration_cast<seconds>(current_system_timepoint.time_since_epoch()).count();
								
					auto last_file_modified_timepoint = ConvertTimepointClock<fs::file_time_type::clock, system_clock>(fs::last_write_time(entry));
					uint64_t last_file_modified_timestamp = duration_cast<seconds>(last_file_modified_timepoint.time_since_epoch()).count();
								
					if((current_system_timestamp - last_file_modified_timestamp) > threshold_seconds) {
						fs::remove(entry);
					}
				}
			}
		}
	}
}

constexpr const char* HELP_TEXT =
"--target (-t) [target_directory]		Required argument, specifies the directory that should be watched.\n"
"\n"
"--max-time-seconds (-mt) [seconds]		The maximum amount of time that a file can remain unmodified within\n"
"		    	  		    	the folder, before it is deleted. This argument also applies to the\n"
"					    	amount of time that the target folder can remain unmodified for\n"
"					    	(adding a file counts as modifying) before its files are evaluated.\n"
"					    	This means that old files added to a folder will not immediately meet\n"
"					    	the condition, and won't get deleted until the folder's last modification\n"
"					    	date exceeds this argument. Default: 21600 Seconds (6 hours).\n"
"\n"
"--scan-frequency-ms (-sf) [milliseconds]	The frequency at which the target folder should be scanned for changes.\n"
"		      	   			Default: 3600 Seconds (1 hour).\n"
"\n"
"--background (-b)				Hide the main window of the program when monitoring.\n"
"\n"
"--help (-h)					Shows this help text.\n";

int main(int argc, char* argv[]) {
	char* target_folder_path = nullptr;
	
	uint64_t maximum_unmodified_time_s = 21600; // 21600 Seconds = 6 hours.
	uint64_t scan_frequency_ms = 3600; // 3600 Seconds = 1 hour.
	
	bool run_in_background = false;
	
	for(int i=0; i<argc; ++i) {
		char* next_argument = i+1 < argc ? argv[i+1] : nullptr;
		char* argument = argv[i];

		if(!_stricmp(argument, "--help") || !_stricmp(argument, "-h")) {
			std::cout << HELP_TEXT << std::endl;
			return 0;
		} else if(!_stricmp(argument, "--background") || !_stricmp(argument, "-b")) {
			run_in_background = true;
		}
		
		if(next_argument != nullptr) {
			if(!_stricmp(argument, "--target") || !_stricmp(argument, "-t")) {
				target_folder_path = next_argument;
			}
			
			else if(!_stricmp(argument, "--max-time-seconds") || !_stricmp(argument, "-mt")) {
				try {
					maximum_unmodified_time_s = std::stoi(next_argument);
				} catch(const std::invalid_argument& invalid_arg_exception) {		    
					std::cerr << "The string supplied for --max-time-seconds (-mt) cannot be converted into an integer, due to an invalid_argument exception: ";
					std::cerr << invalid_arg_exception.what() << std::endl;
					return 1;
				} catch(const std::out_of_range& out_of_range_exception) {
					std::cerr << "The string supplied for --max-time-second (-mt) cannot be converted to an integer, due to an out_of_range exception: ";
					std::cerr << out_of_range_exception.what() << std::endl;
					return 1;
				}
			}

			else if(!_stricmp(argument, "--scan-frequency-ms") || !_stricmp(argument, "-sf")) {
				try {
					scan_frequency_ms = std::stoi(next_argument);
				} catch(const std::invalid_argument& invalid_arg_exception) {		    
					std::cerr << "The string supplied for --scan--frequency-ms (-sf) cannot be converted into an integer, due to an invalid_argument exception: ";
					std::cerr << invalid_arg_exception.what() << std::endl;
					return 1;
				} catch(const std::out_of_range& out_of_range_exception) {
					std::cerr << "The string supplied for --scan--frequency-ms (-sf) cannot be converted to an integer, due to an out_of_range exception: ";
					std::cerr << out_of_range_exception.what() << std::endl;
					return 1;
				}
			}
		}
	}
	
	if(target_folder_path != nullptr) {
		if(run_in_background) ShowWindow(GetConsoleWindow(), SW_HIDE);
		WatchFolder(target_folder_path, maximum_unmodified_time_s, scan_frequency_ms);
	} else {
		std::cerr << "No folder to monitor, please supply one with --target (-t)" << std::endl;
		return 1;
	}

	return 0;
}
