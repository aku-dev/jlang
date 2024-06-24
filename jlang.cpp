#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

#include "json.hpp"
#include "jlang.hpp"

using json::JSON;
using namespace jlang;
using namespace std;
using namespace filesystem;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	SArgument args[3] = {
		{"l", "All text inline.", false}, 
		{"rt", "Remove titles.", false},
		{"br", "Add break lines.", false}
	};
	    
	cout << "JSON Language formatter by Alexandr Kuznecov" << endl;
	cout << "Version 1.00" << endl << endl;

    if (argc < 2) {
		// No arguments - Usage help
		cout << "Usage: jlang.exe *.cpp";
		for (auto& a : args) {
			cout << " [/" << a.s_name << "]";
		}
		cout << endl;

		// Notices
		for (auto& a : args) {
			cout << "/" << a.s_name << " " << a.s_notice << endl;
		}

		cout << endl;
    } else {
		// Main
		int i_count;
		string s_result;		
		string s_out_name;
		string s_file_name = argv[1];
		stringstream buffer;
		
		// Open files
		ifstream fin(s_file_name.c_str());
		unsigned int pos = s_file_name.find_last_of(".");
		
		s_out_name = s_file_name.substr(0, pos);
		s_out_name.append(".txt");
		ofstream fout(s_out_name.c_str());
		
		if (!fin.is_open()) {
			cerr << "ERROR: Could not open file " << s_file_name << endl;
			return 0;
		}												
		
		if (!fout.is_open()) {
			cerr << "ERROR: Could not create file " << s_out_name << endl;
			return 0;
		}				
		
		buffer << fin.rdbuf();
		
		JSON json_data = JSON::Load(buffer.str());
		
		// Fill arguments struct
		for (int i = 2; i < argc; i++)
		{
			string s = strtolower(argv[i]);
			for (auto& a : args) {
				if ("/" + a.s_name == s) {
					a.b_on = true;
				}
			}
		}

		// Format text
		i_count = 0;
		for(auto &v : json_data["values"].ArrayRange()) {
			string s_body = v["value"].ToString();
			
			// l - Inline
			if (!args[0].b_on) { 
			
				// rt - Titles
				if (!args[1].b_on) {
					s_result.append("==================================================\n");
					s_result.append("== ");
					s_result.append(v["key"].ToString());
					s_result.append("\n");
					s_result.append("==================================================\n");
				}

				// br - Break lines
				if (!args[2].b_on)
					s_body = regex_replace(s_body, regex(R"(\\n)"), "\n");

				s_result.append(s_body);

				s_result.append("\n\n");
			}
			else
			{
				s_result.append(s_body);
				s_result.append("\n");
			}

			i_count++;
		}
		
		fin.close();
					
		fout << s_result;
	
		fout.close();

		// Output statisics
		cout << "Arguments: " << endl;
		for (auto& a : args) {
			if (a.b_on) {
				cout << "[/" << a.s_name << "] " << a.s_notice << endl;
			}			
		}
		cout << endl;

		cout << "Success write file: " << s_out_name << endl;
		cout << "Contains: " << i_count << " entries." << endl;
		cout << "Size: " << file_size(s_out_name) << " bytes." << endl;
	}

    cout << endl;
    system("pause");
}
